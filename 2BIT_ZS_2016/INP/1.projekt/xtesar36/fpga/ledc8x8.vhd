library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.
-- vstupy a vystupy
	SMCLK:	 in std_logic; 
	RESET:	 in std_logic;
	ROW:		 out std_logic_vector (7 downto 0);
	LED:		 out std_logic_vector (0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is
	signal ce			: std_logic; -- zpomalena frekcence
	signal cnt_row		: std_logic_vector(7 downto 0);
	signal cnt_ce		: std_logic_vector(7 downto 0);

begin
	 
	 --------------------------------------
	 -- citac, abychom snizili frekvenci --
	 -------------------------------------- 
	 ce_gener: process(SMCLK, RESET)
	 begin
		if (RESET = '1') then
			cnt_ce <= X"00"; -- vynulujeme
		else
			if rising_edge(smclk) then
				cnt_ce 	<= cnt_ce + 1;
			end if;
		end if;
	end process ce_gener;	
	 
	 ce <= '1' when cnt_ce = X"FF" else '0';
	 
	 -------------------
	 -- pocitac radku --
	 -------------------
	 
	 counter_rows: process(RESET, SMCLK)
	 begin
		if (RESET = '1') then
			cnt_row <= X"01";
		else
			if rising_edge(smclk) then
				if (ce = '1') then
					cnt_row <=  cnt_row (6 downto 0) & cnt_row(7) ;
				end if;
			end if;
		end if;
	 end process counter_rows;
	 
	 
	-- prirazeni signalu na vystup --
	ROW <= 	cnt_row;
	
	led <= 	"00001111" when cnt_row = "00000001" else
				"01101111" when cnt_row = "00000010" else
				"00001111" when cnt_row = "00000100" else
				"01101111" when cnt_row = "00001000" else
				"01100000" when cnt_row = "00010000" else
				"11111011" when cnt_row = "00100000" else
				"11111011" when cnt_row = "01000000" else
				"11111011" when cnt_row = "10000000" else
				"11111111";
				

	
end architecture main;
