-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Alena Tesarova (xtesar36)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- nadefinujeme si instrukce
 
type instruction_type is (
inc_pointer,  -- >
dec_pointer,  -- <
inc_value,    -- +
dec_value,    -- -
left_par,     -- [
right_par,    -- ]
print_val,    -- .
break_while,  -- ~
loadsave_val,  -- ,
stop_program,  -- null
nope -- cokoliv jineho
);
 
type state is(
state_init, run_program, run_program1,
s_inc_pointer,  -- >
s_dec_pointer,  -- <
s_inc_value, s_inc_value_next,   -- +
s_dec_value, s_dec_value_next,   -- -
s_left_par,     -- [
s_right_par,    -- ]
s_print_val,    -- .
s_break_while, s_break_while_next, s_break_while_counter, -- ~
s_loadsave_val, s_loadsave_val_next,  -- ,
s_stop_program,  -- null
s_nope,
decode_state,

s_begin_loop,
s_begin_loop_counter,
s_begin_loop_end,
s_begin_loop_end_counter,


s_end_counter,
s_end_loop_counter,
s_end_loop_end_counter,
s_end_loop_jump,
s_end_condition,

s_print_val_get,
s_break_while_jump

);


signal instruction: instruction_type;

signal actual_state : state;
signal next_state : state;

-- ---------- POCITADLA --------------

-- PTR bude obsahovat adresu do pameti ROM, proto 11
signal pc_register_counter: std_logic_vector(11 downto 0);
signal pc_inc : std_logic; -- inkrementujeme (1)
signal pc_dec : std_logic; -- nekrementujeme (0)

signal cnt_register_counter : std_logic_vector(7 downto 0);
signal cnt_inc : std_logic; -- inkrementujeme (1)
signal cnt_dec : std_logic; -- nekrementujeme (0)

-- PTR bude obsahovat adresu do pameti RAM, proto 9
signal ptr_register_counter : std_logic_vector(9 downto 0);
signal ptr_inc : std_logic; -- inkrementujeme (1)
signal ptr_dec : std_logic; -- nekrementujeme (0)

signal sel: std_logic_vector(1 downto 0); -- sel pro multoplexor

begin



-- ---------- PC REGISTR -------------

pc_register: process ( pc_inc, pc_dec, CLK, RESET )
begin
	if ( RESET = '1' ) then
		pc_register_counter <= "000000000000";
	elsif ( ( CLK = '1' ) and CLK'event ) then
		if ( pc_inc = '1' ) then
			pc_register_counter <= pc_register_counter + 1;
		elsif ( pc_dec = '1' ) then
			pc_register_counter <= pc_register_counter - 1;
		end if;	
	end if;
end process;

-- ---------- CNT REGISTR ------------ 
cnt_register: process ( cnt_inc, cnt_dec, CLK, RESET )
begin
	if ( RESET = '1' ) then
		cnt_register_counter <= "00000000";
	elsif ( ( CLK = '1' ) and CLK'event ) then
		if ( cnt_inc = '1' ) then
			cnt_register_counter <= cnt_register_counter + 1;
		elsif ( cnt_dec = '1' ) then
			cnt_register_counter <= cnt_register_counter - 1;
		end if;	
	end if;
end process;

-- ---------- PTR REGISTR ------------

ptr_register: process ( ptr_inc, ptr_dec, CLK, RESET )
begin
	if ( RESET = '1' ) then
		ptr_register_counter <= "0000000000";
	elsif ( ( CLK = '1' ) and CLK'event ) then
		if ( ptr_inc = '1' ) then
			ptr_register_counter <= ptr_register_counter + 1;
		elsif ( ptr_dec = '1' ) then
			ptr_register_counter <= ptr_register_counter - 1;
		end if;	
	end if;
end process;

-- dostaneme data z ROM a chceme zjistit instrukci
-- inc_pointer,  -- > 0x3e
-- dec_pointer,  -- < 0x3c
-- inc_value,    -- + 0x2b
-- dec_value,    -- - 0x2d
-- left_par,     -- [ 0x5d
-- right_par,    -- ] 0x5b
-- print_val,    -- . 0x2e
-- break_while   -- ~ 0x7e
-- loadsave_val  -- , 0x2c
-- stop_program  -- null 0x00

get_instruction: process (  CODE_DATA )
begin
	case ( CODE_DATA ) is
		when X"3E" => instruction <= inc_pointer;
		when X"3C" => instruction <= dec_pointer;
		when X"2B" => instruction <= inc_value;
		when X"2D" => instruction <= dec_value;
		when X"5B" => instruction <= left_par;
		when X"5D" => instruction <= right_par;
		when X"2E" => instruction <= print_val;
		when X"7E" => instruction <= break_while;
		when X"2C" => instruction <= loadsave_val;
		when X"00" => instruction <= stop_program;
		when others => instruction <= nope;
	end case;
end process;

-- --------- MULTIPLEXOR ------------------

-- DATA_RDATA jsou data poslana na vystup z ram, bunka na adrese DATA_ADDR
-- IN_DATA jsou dana poslana do ram DATA__WDATA
multiplexor: process (sel, DATA_RDATA, IN_DATA)
begin
	case sel is
		when "00" => DATA_WDATA <= DATA_RDATA + 1;
		when "01" => DATA_WDATA <= DATA_RDATA - 1;
		when "10" => DATA_WDATA <= IN_DATA;
		when "11" => DATA_WDATA <= X"00";
		when others => 
	end case;
end process;

-- --------- KONECNY AUTOMAT --------------

final_state_init: process ( RESET, CLK, EN )
begin
if ( RESET = '1' ) then
 -- inicializujeme stav
	actual_state <= state_init;
elsif ( ( CLK = '1' ) and ( CLK'event ) ) then
	if ( EN = '1' ) then
		actual_state <= next_state;
	end if;
end if;
end process;

-- DATA_RDATA = 
final_state_machine: process ( IN_VLD, OUT_BUSY, instruction, actual_state, DATA_RDATA )
begin
-- inicializace
DATA_EN <= '0';
CODE_EN <= '0';
-- ptr indexuje ram
DATA_ADDR <= ptr_register_counter;
pc_inc <= '0';
cnt_inc <= '0';
ptr_inc <= '0';

pc_dec <= '0';
cnt_dec <= '0';
ptr_dec <= '0';

IN_REQ <= '0';
OUT_WE <= '0';

case actual_state is

	when state_init => next_state <= run_program;
	
	when run_program =>
		CODE_ADDR <= pc_register_counter; --indexace rom
		CODE_EN <= '1'; -- povolenni cinnosti ROM
		next_state <= run_program1;
	
	-- cekam jeden takt, aby se mi ustalila data
	when run_program1 =>
		next_state <= decode_state;
		
	when decode_state =>
		case instruction is
			when inc_pointer => 
					next_state <= s_inc_pointer; -- > 0x3e
			when dec_pointer => 
					next_state  <= s_dec_pointer; -- < 0x3c
			when inc_value => 
					next_state <=  s_inc_value;  -- + 0x2b
			when dec_value => 
					next_state <=  s_dec_value;  -- - 0x2d
			when left_par => 
					next_state <=  s_left_par;   -- [ 0x5d
			when right_par =>
					next_state <=  s_right_par;  -- ] 0x5b
			when print_val => 
					next_state <=  s_print_val_get;  -- . 0x2e
			when break_while => 
					next_state <=  s_break_while; -- ~ 0x7e
			when loadsave_val =>
					next_state <= s_loadsave_val; -- , 0x2c
			when stop_program =>
					next_state <= s_stop_program;
			when others =>
					next_state <= s_nope;
		end case;
	when s_inc_pointer =>
		ptr_inc <= '1';
		pc_inc <= '1';
		next_state <= run_program;
		
	when s_dec_pointer =>
		ptr_dec <= '1';
		pc_inc <= '1'; 
		next_state <= run_program;
		
	when s_inc_value =>
		-- DATA_RDATA <= ram[PTR]
		-- ram[PTR] <= DATA_RDATA + 1
		DATA_RDWR <= '0'; -- chci cist aktualni bunku do DATA_RDATA
		DATA_EN <= '1'; 
		next_state <= s_inc_value_next;
		
	when s_inc_value_next =>
		pc_inc <= '1';
		DATA_RDWR <= '1'; -- budu zapisovat
		DATA_EN <= '1'; 
		sel <= "00"; -- do WDATA dostanu DATA_RDATA + 1
		next_state <= run_program;
	
	when s_dec_value =>
		-- DATA_RDATA <= ram[PTR]
		-- ram[PTR] <= DATA_RDATA - 1
		DATA_RDWR <= '0'; -- povoleni cteni do pameti
		DATA_EN <= '1'; 
		next_state <= s_dec_value_next;
		
	when s_dec_value_next =>
		pc_inc <= '1';
		DATA_RDWR <= '1'; 
		DATA_EN <= '1'; 
		sel <= "01"; -- do WDATA dostanu DATA_RDATA - 1
		next_state <= run_program;
		
	when s_left_par =>
		pc_inc <= '1';
		DATA_RDWR <= '0'; -- povoleni zapisovani do pameti
		DATA_EN <= '1'; --povoleni cinnost pameti
		next_state <= s_begin_loop;
		
	when 	s_begin_loop =>
		if ( DATA_RDATA = "00000000" ) then
			cnt_inc <= '1';
			next_state <= s_begin_loop_counter;
		else	
			next_state <= run_program;
		end if;
		
	when s_begin_loop_counter =>
		-- c = rom[pc]
		CODE_ADDR <= pc_register_counter;
		CODE_EN <= '1'; 
		next_state <= s_begin_loop_end_counter;
		
	when s_begin_loop_end_counter =>
		if ( instruction = left_par) then
			cnt_inc <= '1';
		elsif ( instruction = right_par ) then
			cnt_dec <= '1';
		end if;
			pc_inc <= '1';
			next_state <= s_begin_loop_end;
	-- !		
	
	when s_begin_loop_end =>
		if ( cnt_register_counter = "00000000" ) then
			next_state <= run_program;
		else
			next_state <= s_begin_loop_counter;
		end if;
		
	when s_right_par => -- prepare
		DATA_EN <= '1';
		DATA_RDWR <= '0'; -- chci aktualni hodnotu
		next_state <= s_end_condition;
		
	when s_end_condition =>
		if ( DATA_RDATA = "00000000" ) then
			pc_inc <= '1';
			next_state <= run_program;
		else
			cnt_inc <= '1';	
			pc_dec <= '1';
			next_state <= s_end_counter;
		end if;
		
	-- if (CNT != 0)
	when s_end_counter =>
	
		if ( cnt_register_counter = "00000000" ) then
			next_state <= run_program;
		else
			
			next_state <= s_end_loop_counter;
		end if;
		
	-- c = rom[pc]
	when s_end_loop_counter =>
		CODE_ADDR <= pc_register_counter;
		CODE_EN <= '1'; --!
		next_state <= s_end_loop_end_counter;
	
	-- if (c == ]) CNT = CNT + 1 wlsif ( c == [ ) CNT = CNT - 1	
	when s_end_loop_end_counter =>
		if ( instruction =  left_par) then
			cnt_dec <= '1';
		elsif ( instruction = right_par ) then
			cnt_inc <= '1';
		end if;
		next_state <= s_end_loop_jump;

	-- if ( CNT == 0) PC = PC + 1 else pc = pc - 1
	when s_end_loop_jump =>
		if ( cnt_register_counter = "00000000" ) then
			pc_inc <= '1';
			next_state <= run_program;

		else
			pc_dec <= '1';
			next_state <= s_end_counter;

		end if;
	
	when s_print_val_get =>
		DATA_EN <= '1';
		DATA_RDWR <= '0';
		next_state <= s_print_val;
	
	when s_print_val =>
		if ( OUT_BUSY = '0' ) then
			-- inicializujeme out_data
			OUT_DATA <= DATA_RDATA;
			OUT_WE <= '1';
			DATA_EN <= '1';
			next_state <= run_program;
			pc_inc <= '1';
		else
			next_state <= s_print_val;
		end if;
		
	when s_break_while =>
		cnt_inc <= '1';
		pc_inc <= '1';
		--CODE_EN <= '1';
		--CODE_RDWR <= '0'; -- chci aktualni hodnotu
		next_state <= s_break_while_jump;
		
	when s_break_while_jump =>
		CODE_ADDR <= pc_register_counter;
		CODE_EN <= '1'; --!
		next_state <= s_break_while_next;
		
	when s_break_while_next =>
		if ( instruction =  left_par) then
			cnt_inc <= '1';
		elsif ( instruction = right_par ) then
			cnt_dec <= '1';
		end if;
		pc_inc <= '1';
		next_state <= s_break_while_counter;	
		
	when s_break_while_counter =>
	
		if ( cnt_register_counter = "00000000" ) then
			next_state <= run_program;
		else	
			next_state <= s_break_while_jump;
		end if;
	
	
	when s_loadsave_val =>
		-- pozadavek na data
		IN_REQ <= '1';
		next_state <= s_loadsave_val_next;
		
	when s_loadsave_val_next =>
		if ( IN_VLD = '1') then
			DATA_EN <= '1'; -- pvoleni zapisu
			DATA_RDWR <= '1'; -- povoleni zapisu do pameti
			sel <= "10";
			IN_REQ <= '0';
			pc_inc <= '1';
			next_state <= run_program;
		else
			next_state <= s_loadsave_val_next;
		end if;
		
	when s_stop_program =>
		
		next_state <= s_stop_program;
		
	when s_nope =>
		pc_inc <= '1';
		next_state <= run_program;
		
	when others => next_state <= s_nope;		

	end case;

end process;

end behavioral;
 
