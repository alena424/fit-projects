#!/usr/bin/python

#from __future__ import print_function
# argv argument
import sys
# REGEX
import re
# prace se soubory
import os.path
#xml data
import xml.etree.ElementTree as ET

class Operand:
    def __init__(self, type_arg, contain):
        self.type_arg = type_arg
        self.contain = contain

# promenna bude dedit z operandu
class Variable( Operand ):
    inicialized = 0
    
    def __init__(self, contain):
        self.type_arg = 'var'
        self.contain = contain
     
        
    def define():
        self.inicialized = 1
        
class Frame:
     def __init__(self, frame ):
         self.table = []
         self.frame = frame
         self.inicialized = 1
    
class Interpret:
    
    def __init__(self, structure ):
       self.glob_frame = Frame ('global' ) ;
       self.local_frame = Frame ('local' ) ;
       self.tmp_frame = Frame ('temporary' ) ;
       
       # inicializujeme globalni ramec, dame sem empty array
       self.glob_frame.table.append( [] )
       
       self.table_glob = []
       self.table_local = []
       self.table_temp = []
       
       # kontexty
       self.globOn = 1;
       self.localOn = 0; # defaultne vypnuty lokalni kontext
       self.tmpOn = 0; # defaultne vypnuty temporary kontext
      
       self.expectedLabel = []
       self.labels = []
       self.dataStack = []
       self.callStack = []; # zasobnik volani
       self.structure = structure
       # zacneme u instrukce 1
       self.processedInstruction = 1
       
       # abychom vedeli jake volat funkce k jednotlivym instrkcim
       self.callInstruction = {
            "ADD" : "getAdd", "SUB" : "getSub" , "MUL" : "getMul", "IDIV" : "getIdiv",
            "LT" : "getLT" , "GT" : "getGT" , "EQ" :"getEQ", "AND" :"getAnd",
            "OR" : "getOr", "STRI2INT" : "getStri2int", "GETCHAR" : "getGetchar" , "SETCHAR" : "getSetchar","CONCAT" : "getConcat",
            "JUMPIFEQ": "getJumpIfEq", "JUMPIFNEQ" : "getJumpIfNeq",
            "INT2CHAR" : "getInt2char", "STRLEN" : "getStrlen", "TYPE" : "getTypeI", "MOVE" : "getMove" ,"NOT" : "getNot",
            "READ" :"getRead" ,
            "DEFVAR" : "getDefvar" , "POPS" : "getPops",  
            "PUSHS" : "getPushs", "WRITE" : "getWrite", "DPRINT" : "getDprint",
            "CALL" : "getCall", "LABEL" :"getLabel", "JUMP" : "getJump" ,
            "BREAK" : "getBreak", "CREATEFRAME" : "getCreateFrame" , "PUSHFRAME" : "getPushFrame", "POPFRAME" : "getPopFrame",
            "RETURN" : "getReturn"
        }
    
    def getTableAccorrdingToFrame( self, frame ):
        # je neco na vrcholu zasobniku
        if ( frame == 'GF' ):
             table_name = 'glob_frame';
        if ( frame == 'TF' ):
            table_name = 'tmp_frame';
        elif( frame == 'LF' ):
            table_name = 'local_frame';
         
        frame_obj = getattr(self, table_name )
        #print( "Frame: " + frame)
        #print( frame_obj.table )
        #print( "len: ")
        #print (len( frame_obj.table ) )
        if ( len( frame_obj.table ) != 0):
            #return self.table_name.table[0] #vracim co je navrchu
            #print ("Vrchol: ")
            #print (frame_obj.table[len( frame_obj.table ) - 1])
            return frame_obj.table[len( frame_obj.table ) - 1]
        else:
            print ( "Snazime se deklarovat lokalni promennou bez definovani ramce" , file=sys.stderr )
            exit ( 55 ) # ramec neexistuje
        
    
    def getTableAccorrdingToFrame2version( self, frame ):
        table = self.table_glob   
        if ( frame == 'TF' ):
            table = self.table_temp
            if ( self.tmpOn != 1):
                print ( "Snazime se deklarovat docasnou promennou bez vytvoreni ramce"  , file=sys.stderr )
                exit ( 55 ) # ramec neexistuje
        elif( frame == 'LF' ):
            table = self.table_local
            if ( self.localOn != 1 ) :
                print ( "Snazime se deklarovat lokalni promennou bez definovani ramce" , file=sys.stderr )
                exit ( 55 ) # ramec neexistuje               
        return table
    
    # pridame item do tabulky podle framu
    def addNewItem( self, name, frame ):
        # mohu vlozit pouze pokud na vrcholu zasobniku je nejake pole, do ktereho muzu vlozit
        table = self.getTableAccorrdingToFrame(frame)
        table.append( { 'name' : name, 'inicialized' : 0 } ) 
                     
    # hledam item podle jmena a framu
    def searchItem( self, name, frame ):
        table = self.getTableAccorrdingToFrame(frame)
        
        for hash_item in table:
            if hash_item[ 'name' ] == name:
                return hash_item
        # pokud nic nenajdeme
        return 0
    
    # najdu v tabulce, inicliazuju
    # pro bool si to ukladame v pythonovkych makrech True a False
    def initItem( self, name, value, typevar, frame ):
        table = self.getTableAccorrdingToFrame(frame);
        for hash_item in table:
            if name in hash_item[ 'name' ] == name:
                hash_item[ 'inicialized' ] = 1
                if typevar == 'bool':
                    if ( value == 'true' or value == True ):
                        value = True
                    else:
                        value = False 
                hash_item[ 'value' ] = value
                hash_item[ 'type' ] = typevar
                return hash_item # jak najdeme, muzeme skoncit
        return None
    
    def initItemElseExit( self, name, value, typevar, frame ):
        table = self.getTableAccorrdingToFrame(frame)
        #print(name)
        #print(value)
        #print(typevar)
        #print(frame)
        #print (table);
        for hash_item in table:
            if name in hash_item[ 'name' ] == name:
                hash_item[ 'inicialized' ] = 1
                if typevar == 'bool':
                    if ( value == 'true' or value == True ):
                        value = True
                    else:
                        value = False 
                hash_item[ 'value' ] = value
                hash_item[ 'type' ] = typevar
                #print (table);
                return hash_item # jak najdeme, muzeme skoncit
        
        #nenasli jsme, neni v tabulce
        print ( "Nedefinovana promenna %s" % name, file=sys.stderr )
        exit ( 54) #semanticka kontrola
            
    # zjistim, jestli uz je inicilaizovana
    def isInit( self, name, frame ):
        item = self.searchItem(name, frame)
        if item:
            return item[ 'inicialized' ]
        else:
            return 0
        
    # funkce vraci typ promenne       
    def getTypeVar(self, name, frame):
        item = self.searchItem(name, frame)
        # vracim hodnotu pouze pokud je inicializovana
        if item and item[ 'inicialized' ]:
            return item[ 'type' ]
        else:
            return None
    
     # funkce vraci hodnotu promenne pokud je inicializovana        
    def getValueVar(self, name, frame):
        item = self.searchItem(name, frame)
        # vracim hodnotu pouze pokud je inicializovana
        if item and item[ 'inicialized' ]:
            # pokud se jedna o typ bool => vracim 1/0
            #if item[ 'type' ] == 'bool':
             #   return True if item[ 'value' ] == 'true' else False
            #else:
            return item[ 'value' ]
        else:
            return None
        
    # funkce vraci hodnotu symbolu => kontanta/promenna
    def getVal(self, symb):
        
        if ( symb[ 'type' ] == 'var' ):
            # u promenne hledame v tabulce
            return self.getValueVar( symb[ 'name' ], symb[ 'frame' ] )
        else:
            # u retezce primo vypiseme
            return symb[ 'text' ]
        
    # funkce vraci typ symbolu => kontanta/promenna
    def getType(self, symb):
        if ( symb[ 'type' ] == 'var' ):
            # hledame v tabulce jaky ma typ
            return self.getTypeVar( symb[ 'name' ], symb[ 'frame' ] )
        else:
            return symb[ 'type' ]
    
    # tisknu tabulku podle framu
    def printStructure(self, frame):
        table = self.getTableAccorrdingToFrame(frame)
        print (table)
     
     # nazev promenne v argumentech metody
     # kontrola, ze promenna je  inicializovana a musi mit ocekavany typ jinak chyba
    def _checkVarIfInitIfType(self, var_name, expected_type, frame):
        if ( self.isInit( var_name, frame ) ):
                if ( self.getTypeVar( var_name, frame ) != expected_type ):
                     print ( "Ocekavam typ %s u promenne %s" % ( expected_type, var_name), file=sys.stderr )
                     exit ( 53) #semanticka kontrola
        else:
            # zjistim, jestli vubecexistuje a podle toho vyhodim hlasku
            if self.searchItem(var_name, frame):
                print ( "Cteni neinicializovane promenne %s" % ( var_name), file=sys.stderr )
                exit ( 56) #semanticka kontrola
            else:
                print ( "Cteni nedefinovane promenne %s" % ( var_name), file=sys.stderr )
                exit ( 54) #semanticka kontrola
    
    # vraci hodnotu promenne, ktera musi existovat, jinak chyba
    # vraci promennou v pripade uspechu
    def _checkVarIfInit(self, var_name, frame):
        if self.isInit( var_name, frame ):
            return self.searchItem( var_name, frame )
        else:
            # zjistim, jestli vubecexistuje a podle toho vyhodim hlasku
            if self.searchItem(var_name, frame):
                print ( "Cteni neinicializovane promenne %s" % ( var_name), file=sys.stderr )
                exit ( 56) #semanticka kontrola
            else:
                print ( "Cteni nedefinovane promenne %s" % ( var_name), file=sys.stderr )
                exit ( 54) #semanticka kontrola
                
            
    # vrati nam poradi navesti, nazev navesti je unikatni    
    def _orderOfLabel(self, label_name ):
        for lab in self.labels:
            if lab[ 'text' ] == label_name:
                return lab[ 'order' ]
        # pokud nic nenajdeme
        return 0
    
    def _orderOfLabelElseExit(self, label_name ):
        for lab in self.labels:
            if lab[ 'text' ] == label_name:
                return lab[ 'order' ]
        # pokud nic nenajdeme
        print ( "Neexistujici navesti %s" % label_name, file=sys.stderr )
        exit ( 52)
    
    
    #############
    # INSTRUKCE #
    #############
    
    # kontrola, ze label existuje, musim to kontrolovat az bude konec prochazeni, ulozim si proto ocekavane labely
    def getJump(self, label):
        orderLabel = self._orderOfLabelElseExit( label[ 'text' ] );
        self.processedInstruction = orderLabel;
      
    #pokud o redefinici navesti => 52            
    def getLabel( self, label ):
        if ( self._orderOfLabel( label[ 'text' ] ) ):
            print ( "Pokud o redefinici navesti %s" % label[ 'text' ], file=sys.stderr )
            exit ( 52)
        self.labels.append( { "text" : label[ 'text' ], "order" : self.processedInstruction } );
        #print ( self.labels );
    
    # 1. pokud ctu promennou, musi existovat
    # 2. pokud zapisuji do promenne, muzu menit jeji typ
    # 3. pristupy k ramcum, pristup k nedefinovanemu ramci vede na chybu 55 => resi interpret
    def getMove(self, var, symb):
        # var musi existovat
        searchvar = self.searchItem(  var[ 'name' ], var[ 'frame' ] )
        # nedefinovana
        if ( searchvar == 0):
            print ( "Zapis nedefinovane promenne %s pomoci instrukce MOVE" % ( var[ 'name' ]), file=sys.stderr )
            exit ( 54) #semanticka kontrola
            
        if ( symb[ 'type' ] == 'var' ):
            variable = self.searchItem( symb[ 'name' ], symb[ 'frame' ] )
            # zjistim si typ a hodnoty z tabulky promennych
            typ = variable[ 'type' ]
            val = variable[ 'value' ]
        else:
            typ = symb[ 'type' ]
            val = symb[ 'text' ]
            
        # definovana je, ulozim si informace
        self.initItemElseExit( var[ 'name' ], val, typ, var[ 'frame' ])
             
    # pokud vytvorim ramec, mohu pouzivat TF temporary
    def getCreateFrame(self):
        self.tmp_frame.table.append([])
        #print( self.tmp_frame.table )
        
    # uz nebudu moct pouzivat TF, ale muzu pouzivat LF
    def getPushFrame(self):
        if ( len( self.tmp_frame.table ) ):
            self.local_frame.table.append( self.tmp_frame.table[len( self.tmp_frame.table ) - 1]  )
            self.tmp_frame.table.pop()
        else:
            print ( "Pristup k nedefinovanym ramci", file=sys.stderr )
            exit ( 55 ) # semanticka kontrola
            

        
    # presun LF do TF, pokud nic v LF => 55 chyba
    def getPopFrame(self):
        if ( len( self.local_frame.table ) ):
            self.tmp_frame.table.append( self.local_frame.table[len( self.local_frame.table ) - 1] )
            self.local_frame.table.pop()
        else:
            print ( "Pristup k nedefinovanym ramci", file=sys.stderr )
            exit ( 55 ) # semanticka kontrola
        
    # definuje promennou
    # 1. pozor, kdy muzu definovat LF a TF, kontrola
    def getDefvar(self, arg):
        self.addNewItem( arg[ 'name' ], arg[ 'frame' ] )
        
    # kontrola na existenci label, musim si pockat na konci    
    def getCall(self, label):
        order = self._orderOfLabelElseExit( label[ 'text' ] )
        # ulozim si navratovou adresu
        self.callStack.append( self.processedInstruction );
        self.processedInstruction = order;

        
    # vyjmu pozici ze zasobniku volani
    # uklid lokalnich ramcu zajistuji jine instrukce
    def getReturn(self):
        order = self.callStack.pop();
        self.processedInstruction = order;
    
    # pokud prom => musi byt definiovana a inicializovana
    def getPushs(self,symb):
        value = self.getVal( symb );
        if ( value is not None ):
             self.dataStack.append( symb );
        else:
            print ( "Promenna %s, kterou chceme vlozit na zasobnik, neni inicializovana" % ( symb[ 'text' ] ), file=sys.stderr )
            exit ( 56) # chybejici hodnota
                  
    # je zasobnik prazdny ? -> 56
    def getPops(self, var):
        if ( len( self.dataStack ) == 0 ):
            print ( "Datovy zasobnik je prazdny", file=sys.stderr )
            exit ( 56)
        
        value = self.dataStack.pop();
        # zmenim typ a jmeno o promenne var na typ a hodnotu z value
        # do var dam value
        self.getMove(var, value);

     
    # 1.source musi byt typu expected_type
    def _controlType(self, source, expected_type):
        # pokud se jedna o promennou, musim zjistit, jestli existuje a je inicializovana a pokud ano zkontrolovat jeji typ
        if ( source[ 'type' ] == 'var' ):
            self._checkVarIfInitIfType( source[ 'name' ], expected_type, source[ 'frame' ] )
            
        elif ( source[ 'type' ] != expected_type ):
            print ( "Ocekavam typ %s" % expected_type, file=sys.stderr )
            exit ( 53) #semanticka kontrola

            
    # 1.source1 a source2 musi byt typu expected_type
    def _controlTypeTwoValues(self, source1, source2, expected_type):
        # pokud se jedna o promennou, musim zjistit, jestli existuje a je inicializovana a pokud ano zkontrolovat jeji typ
        self._controlType( source1, expected_type )
        self._controlType( source2, expected_type )
    
    # dostaneme vysledek operace
    def _operation( self, source1, source2, operation ):
        if ( source1[ 'type' ] == 'var' ):
            val1 = self.getValueVar( source1[ 'name' ], source1[ 'frame' ] )
        else:
            val1 = source1[ 'text' ]
        
        if ( source2[ 'type' ] == 'var' ):
            val2 = self.getValueVar( source2[ 'name' ], source2[ 'frame' ] )
        else:
            val2 = source2[ 'text' ]
        
        if ( operation == 'plus' ):
            return int(val1) + int(val2)
        elif( operation == 'minus'  ):
            return int(val1) - int(val2)
        elif( operation == 'mul'  ):
            return int(val1) * int(val2)
        elif( operation == 'idiv'  ):
            #konstrola nuly
            if int(val2) != 0:
                return int ( int(val1) / int(val2) )
            else:
                print ( "POZOR, snazite se delit nulou" , file=sys.stderr )
                exit ( 57) # deleni nulou
                
        elif( operation == 'lt' ):
            return (val1 < val2)
        elif( operation == 'gt' ):
            return (val1 > val2)
        elif( operation == 'eq' ):
            return (val1 == val2)
        
        elif( operation == 'and' ):
            #print ( "Val1 %s AND val2 %s" % (val1, val2))
            return (val1 and val2)
        elif( operation == 'or' ):
            #print ( "Val1 %s OR val2 %s" % (val1, val2))
            return (val1 or val2)
        elif( operation == 'not' ):
            #print ( "Val1 %s NOT val2 %s" % (val1, val2))
            return (not val1 )
                            
    def _getArithmetic( self, desc, source1, source2, expected_type, operation  ):
        self._controlTypeTwoValues(source1, source2, expected_type )
        vysl = self._operation( source1, source2, operation )
        self.initItemElseExit( desc[ 'name' ], vysl, expected_type, desc[ 'frame' ] )

        
    # 2. vysledek do desc a kdyztak to prepisu
    def getAdd(self, desc, source1, source2):
       self._getArithmetic( desc, source1, source2, 'int', 'plus' )
             
    def getSub(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, 'int','minus' )
        
    def getMul(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, 'int','mul' )
    
    # pozor na deleni nulou => 57    
    def getIdiv(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, 'int','idiv' )
        
    # vraci typ a kontroluje zda jsou typy stejne
    def _sameType(self, source1, source2):
        if ( source1[ 'type' ] == 'var' ):
            type1 = self.getTypeVar( source1[ 'name' ], source1[ 'frame' ] )
        else:
            type1 = source1[ 'type' ]
            
        if ( source2[ 'type' ] == 'var' ):
            type2 = self.getTypeVar( source2[ 'name' ], source2[ 'frame' ] )
        else:
            type2 = source2[ 'type' ]
        
        if ( type1 == type2  ):
            return type1
        else:
            print ( "U instrukci pracujeme s odlisnymi typy, typ1 %s != typ2 %s" % ( type1, type2 ), file=sys.stderr )
            exit ( 53 ) # jine typy
            
    # 1. ukladam stejny typ
    # do bool prom var zapise false pri neplatnosti nebo true v pripade platnosti relace
    # false < true
    # muzeme pouzit AND,OR< NOT
    def getLT(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, self.getType(source1 ), 'lt' ) 
        
    def getGT(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, self.getType(source1), 'gt' )
        
    def getEQ(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, self.getType(source1), 'eq' )
        
    # kontrola zda se jedna o bool    
    def getAnd(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, 'bool', 'and' )
        
    def getOr(self, desc, source1, source2):
        self._getArithmetic( desc, source1, source2, 'bool', 'or' )
       
        
    def getNot(self, desc, source1 ):
        self._getArithmetic( desc, source1, source1, 'bool', 'not' )
            
    # var - kdyztak prepisu
    # symb - typ int, musi byt validni ordinalni hodnota znaku v unicode (viz chr) => 58
    def getInt2char(self, var, symb):
        self._controlType( symb, 'int' )
        val = int ( self.getVal( symb ) );
        if ( val is not None ):
            try:
                ordinal_val = chr( val )
            except ValueError:
                print ( "Pokus o prevedeni nevalidni ordinalni hodnoty %s na unicode znak" % ( val ), file=sys.stderr )
                exit ( 58 )
            
            # povedlo se, prepisu hodnotu a typ promenne
            # musim ji najit napred v tabulce
            self.initItemElseExit( var[ 'name' ], ordinal_val, 'string', var[ 'frame' ] )
       
        else:
            print ( "Prace s neinicializovanou promennou %s" % ( symb[ 'name' ]), file=sys.stderr )
            exit ( 56 )
            
        
    # symb2 - pozice, index od 0, musi byt int
    # symb1 - typ string
    # ve var je string
    # indexace mimo => 58 viz ord, pozice nesmi byt zaporna, nesmi byt vetsi nez pocet znaku v symb1
    def getStri2int(self, var, symb1, symb2):
        self._controlType( symb1, 'string' )
        self._controlType( symb2, 'int' )
        
        val1 = str ( self.getVal( symb1 ) )   #string 
        val2 = int( self.getVal( symb2 ) ) # int
        
        if ( len(val1) > val2 and val2 >= 0 ):
            try:
                ordinal_val = ord( val1[ val2 ] )
            except TypeError:
                print ( "Pokus o prevedeni nevalidni unicode hodnoty %s na Int" % ( val1[ val2 ] ), file=sys.stderr )
                exit ( 58 )
        else:
            print ( "Indexace mimo pole retezec %s, index %s" % ( val1, val2, ), file=sys.stderr )
            exit ( 58) 
        # povedlo se, prepisu hodnotu a typ promenne
        # musim ji najit napred v tabulce
        # priradim ji nove hodnoty a jdu dal
        self.initItemElseExit( var[ 'name' ], ordinal_val, 'int', var[ 'frame' ] )
   

       
    # var musi existovat, nemusi byt inicializovana  
    # cte typ, funkci input
    # provede se konverze na specifikovany typ, retezec true se prevadi na bool@true jinak bool@false
    # v pripade chybneho vstupu do var ulozena implicitni hodnota (0,prazdny retezec nebo false)
    def getRead(self, var, type_input):
        input_val = input()
        
        # musime osetrit chybny vstup
        if ( type_input[ 'text' ] == 'int' ):
            # matchuje cislo
            if ( re.match( input_val, '^((\+|-)?\d+|)$'  ) == 0 ):
                input_val = 0
        if ( type_input[ 'text' ] == 'string' ):
            # retezec
            if ( re.match(  input_val, '^(([^\\\\\s#]|(\\\\[0-9]{3}))*)$' ) == 0 ):
                input_val = ""
        
        # bool se prevede vzdy dobre
        # priradim ji nove hodnoty a jdu dal
        self.initItemElseExit( var['name'], input_val, type_input[ 'text' ], var[ 'frame' ] )
  
             
    # var musi existovat, musi byt inicializovana  
    def getWrite(self, symb):
        if ( symb[ 'type' ] == 'var' ):
            self._checkVarIfInit( symb[ 'name' ], symb[ 'frame' ] )
        #print(symb)
        val = self.getVal(symb)
        print( val, end='' )
    
    # symb1, symb2 jsou retezce
    # ve var bude string
    def getConcat(self, desc, source1, source2):
        self._controlTypeTwoValues( source1, source2, 'string' )
        val1 = self.getVal(source1)
        val2 = self.getVal(source2)
        ret_val = val1 + val2
        variable = self.searchItem( desc[ 'name' ], desc[ 'frame' ] )
        # priradim ji nove hodnoty a jdu dal
        self.initItemElseExit( desc['name'], ret_val, 'string', desc[ 'frame' ] )

               
    # ve var bude int
    # symb je string
    def getStrlen(self, var, symb):
        self._controlType( var, 'int' )
        self._controlType( symb, 'string' )
        val = self.getVal( symb )
        length = len( val )
        self.initItemElseExit( var[ 'name' ], length, 'int', var[ 'frame' ] )
    
    # funkce na konvertovani spravneho typu  
    def _getConvertVal(self, symb):
        # musim porovnavat spravne typy
        val = self.getVal(symb)
        typ = self.getType(symb)
        if (typ == 'int'):
            return int(val)
        if (typ == 'string'):
            return str(val)
        
    # symb2 - int (pozice)
    # sym1 - string
    # ve var bude string
    # indexace mimo => 58 viz ord, pozice nesmi byt zaporna, nesmi byt vetsi nez pocet znaku v symb1
    def getGetchar(self, desc, symb1, symb2):
        self._controlType( symb1, 'string' )
        self._controlType( symb2, 'int' )
        
        val1 = self._getConvertVal( symb1 )    
        val2 = self._getConvertVal( symb2 )
    
        if ( len( val1 ) > val2 and val2 >= 0):
            ordinal_val = val1[ val2 ]
        else:
            print ( "Indexace mimo pole retezec %s, index %s" % ( val1, val2 ), file=sys.stderr )
            exit ( 58) 
 
        # povedlo se, prepisu hodnotu a typ promenne
        # musim ji najit napred v tabulce
        self.initItemElseExit( desc[ 'name' ], ordinal_val, 'string', desc[ 'frame' ] )
    
    # symb2 - string, beru prvni znak, pokud symb2 obsahuje vice znaku
    # symb1 - int, pozice, index od nuly
    # ve var musi byt na zacatku string => modifukuju pozici symb1 znakem symb2
    # indexace mimo => 58 viz ord, pozice nesmi byt zaporna, nesmi byt vetsi nez pocet znaku v symb1
    def getSetchar(self, desc, symb1, symb2):
        self._controlType( symb1, 'int' )
        self._controlType( symb2, 'string' )
        self._controlType( desc, 'string' )
        
        val1 = self._getConvertVal( symb1 )    
        val2 = self._getConvertVal( symb2 )
        var = self.getVal( desc )
        
        # bereme prvni znak
        print("val2: " + val2)
        if ( val2 ):
            val2 = val2[0]
        #else:
            #todo
                
        if ( len( var ) > val1 and val1 >= 0):
            var[ val1 ] = val2;
        else:
            print ( "Indexace mimo pole retezec %s, index %s, hodnota %s" % ( var, val1, val2 ), file=sys.stderr )
            exit ( 58) 
 
        # povedlo se, prepisu hodnotu a typ promenne
        # musim ji najit napred v tabulce
        self.initItemElseExit( desc[ 'name' ], var, 'string', desc[ 'frame' ] )
        
    # zde pouze kontrola, aby symb v pripade var byla definovana, ale nemusi byt inicializovana
    def getTypeI(self, var, symb):
        #print(symb)
        if ( symb[ 'type' ] == 'var' ):
            if ( self.searchItem( symb[ 'name' ], symb[ 'frame' ] ) == 0):
                print ( "Prace s nedefinovanou promennou %s" % ( symb[ 'name' ]), file=sys.stderr )
                exit ( 54) #54 pokus o cteni neexistujici promenne
        typ = self.getType(symb)
        if ( typ is None ):
            # neni inicializovana, jedna se o prazdny string
            typ = 'string'
            
            
        # zapisi retezec znacici tento typ symbolu
        self.initItemElseExit( var[ 'name' ], typ, 'string', var[ 'frame' ] )
            
    # pridam label do expected label, pokud neni v labels
    # symb1 a symb2 musi byt stejneho typu
    def getJumpIfEq(self, label, symb1, symb2):
        #if ( label[ 'text' ] not in self.labels ):
            #self.expectedLabel.append( label[ 'text' ] )  
        self._sameType(symb1, symb2)
        
        val1 = self._getConvertVal(symb1)
        val2 = self._getConvertVal(symb2)
        
        if ( val1 == val2 ):
            self.getJump( label )
    
    def getJumpIfNeq(self, label, symb1, symb2):
        #if ( label[ 'text' ] not in self.labels ):
            #self.expectedLabel.append( label[ 'text' ] )
        self._sameType(symb1, symb2)
        val1 = self._getConvertVal(symb1)
        val2 = self._getConvertVal(symb2)
        if ( val1 != val2 ):
            self.getJump( label )
    
    # pokud promenna -musi mit init    
    def getDprint(self, symb):
        if ( symb[ 'type' ] == 'var' ):
            self._checkVarIfInit( symb[ 'name' ], symb[ 'frame' ] )
        
    #def getBreak(self):
    
    def findAllLabels(self):
        instructionInfo = self.structure[ self.processedInstruction ]
        while ( instructionInfo[ 'instruction_name' ] != 'END' ):
            # zavolam danou funkci k dane instrukci s parametry instrukce
            if ( instructionInfo[ 'instruction_name' ] == 'LABEL' ):
                getattr(self, self.callInstruction[ instructionInfo[ 'instruction_name' ] ] )( *instructionInfo[ 'arg' ] )
            
            self.processedInstruction = self.processedInstruction + 1
            instructionInfo = self.structure[ self.processedInstruction ]
         
        # musime zase na zacatek    
        self.processedInstruction = 1;
        
    def proceed(self):
        # jedu az do az do konce
        instructionInfo = self.structure[ self.processedInstruction ]
        while ( instructionInfo[ 'instruction_name' ] != 'END' ):
            # zavolam danou funkci k dane instrukci s parametry instrukce
            #print( self.callInstruction[ instructionInfo[ 'instruction_name' ] ] )
            #print( *instructionInfo[ 'arg' ] )
            if ( instructionInfo[ 'instruction_name' ] != 'LABEL' ):
                getattr(self, self.callInstruction[ instructionInfo[ 'instruction_name' ] ] )( *instructionInfo[ 'arg' ] )
            
            #self.printStructure( 'GF' );
            self.processedInstruction = self.processedInstruction +1
            instructionInfo = self.structure[ self.processedInstruction ]
        
        # todo pridat linebreaky
        print()
     
class SyntaxParse:
    
    def __init__(self,xml):
        self.xml = xml
        
        self.instruction_map = {
            "ADD": "A1", "SUB" : "A1", "MUL" : "A1", "IDIV" : "A1", "LT" : "A1", "GT" : "A1", "EQ" : "A1", "AND" : "A1",
            "OR" : "A1", "STRI2INT" : "A1", "GETCHAR" : "A1", "SETCHAR" : "A1","CONCAT" : "A1",
            
            "JUMPIFEQ" : "A2", "JUMPIFNEQ" : "A2",
            
            "INT2CHAR" : "B1", "STRLEN" : "B1", "TYPE" : "B1", "MOVE" : "B1","NOT" : "B1",
            
            "READ" : "B2",
            
            "DEFVAR" : "C1", "POPS" : "C1",
            
            "PUSHS" : "C2", "WRITE" : "C2", "DPRINT" : "C2",
            
            "CALL" : "C3", "LABEL" : "C3", "JUMP" : "C3",
            
            "BREAK" : "D", "CREATEFRAME" : "D", "PUSHFRAME" : "D", "POPFRAME" : "D", "RETURN" : "D",
        }
        type_xml = "type"
        type_reg = "(string|int|bool)"
        label_reg = "\D(\w|%|\*|&|\\$|-|)+"
        variable_reg = "^(GF|LF|TF)@("+label_reg+")$"
        int_reg = '^((\+|-)?\d+|)$'
        bool_reg = '^(false|true)$'
        string_reg = '^(([^\\\\\s#]|(\\\\[0-9]{3}))*)$'
        
        constant_reg = '^(([^\\\\\s#]|(\\\\[0-9]{3}))*)|(((\+|-)?\d+|))|(false|true)$'
        symbol_reg = "("+ constant_reg +"|"+ variable_reg +")"
        
        # a) 1. ADD, SUB, MUL, IDIV, LT, GT, EQ, AND, OR, STRI2INT, GETCHAR, SETCHAR, CONCAT (3 operandy) <var> <symm1> <symb2> ^
        #    2. JUMPIFEQ, JUMPIFNEQ, <label> <symm1> <symb2>
        #
        # b) 1. INT2CHAR, STRLEN, TYPE, MOVE, NOT <var> <symb> 
        #    2. READ, <var> <type>  (2 operandy)
        #
        # c) 1. DEFVAR, POPS (1 operand) <var>
        #    2. PUSHS, WRITE, DPRINT <symb>
        #    3. CALL, LABEL, JUMP <label>
        #
        # d) BREAK, CREATEFRAME, PUSHFRAME, POPFRAME, RETURN (0 operandu)
        self.dictionaryInstruction = {
            "A1": { 'number' : 3, 'arg0' : variable_reg, 'arg1' : symbol_reg, 'arg2' : symbol_reg },
            "A2": { 'number' : 3, 'arg0' : label_reg, 'arg1' : symbol_reg, 'arg2' : symbol_reg },
            "B1": { 'number' : 2, 'arg0' : variable_reg, 'arg1' : symbol_reg },
            "B2": { 'number' : 2, 'arg0' : variable_reg, 'arg1' : type_reg },
            "C1": { 'number' : 1, 'arg0' : variable_reg },
            "C2": { 'number' : 1, 'arg0' : symbol_reg },
            "C3": { 'number' : 1, 'arg0' : label_reg },
            "D": { 'number' : 0 }  ,
        }
        self.dictionaryType = {
            "int" : int_reg,
            "string" : string_reg,
            "bool" : bool_reg,
            "label" : '^'+label_reg+'$',
            "var" : variable_reg,
            "type" : type_reg
        }
         
     
    def _checkFormatOfOneInstruction( self, instruction_name, instruction_arg ):    
        if ( instruction_name in self.instruction_map ):
             type_instruction = self.instruction_map[ instruction_name ]
        else:
           print ( "Spatny nazev instrukce  %s" % instruction_name, file=sys.stderr )
           exit ( 32)
        
        #print (type_instruction)
        #print (instruction_name)   
        structure = self.dictionaryInstruction[ type_instruction ]
        #print (structure)
        if ( len( instruction_arg ) == structure[ 'number' ] ):
            for arg in range( structure[ 'number' ]  ):
                #print (arg)
                index = "arg" + str(arg);
                if ( instruction_arg[ arg ][ 'text' ] is None ):
                    # aby regex nehodil chybu
                    instruction_arg[ arg ][ 'text' ] = ''
                if ( re.match(  structure[ index ], instruction_arg[ arg ][ 'text' ] ) ):
                    # kontrola regexem kazdy operand instrukce
                    # proslo, tak jeste zkontroluj typ
                    if ( instruction_arg[ arg ][ 'type' ] in self.dictionaryType ):

                        if ( re.match(  self.dictionaryType[ instruction_arg[ arg ][ 'type' ]  ], instruction_arg[ arg ][ 'text' ] ) is None):
                            print ( "Spatny typ operandu u instrukce %s, operand %s" % ( instruction_name, instruction_arg[ arg ][ 'text' ]  ), file=sys.stderr )
                            #print >> sys.stderr, "Spatny format operandu u instrukce "
                            exit ( 32)
                        # tady by se hodilo prevedeni stringu na normalni string :)
                        if ( instruction_arg[ arg ][ 'type' ] == 'string' ):
                            # prevedeme
                            def replace(m): return chr(int(m.group(1)))
                            instruction_arg[ arg ][ 'text' ] = re.sub( '\\\\([0-9]{3})', replace, instruction_arg[ arg ][ 'text' ]  )
                            
                    else:
                        print ( "Neznamy typ u instrukce %s, typ %s" % ( instruction_name, instruction_arg[ arg ][ 'type' ]  ), file=sys.stderr )                     
                        exit ( 32) 
                            
                else:
                    print ( "Spatny format operandu u instrukce %s, operand %s" % ( instruction_name, instruction_arg[ arg ][ 'text' ]  ), file=sys.stderr )
                    #print >> sys.stderr, "Spatny format operandu u instrukce "
                    exit ( 32) 
        else:
            print ( "Spatny pocet operandu u instrukce %s" % instruction_name, file=sys.stderr )
            exit ( 32)
        
        #print ("match %s" % instruction_name)
        return True
            
    # funkce zkontroluje syntax a v pripade uspechu vraci strukturu xml v poli slovniku, ktera vypada asi takto:
    # [ {'instruction_name': 'JUMP', 'arg': [{'text': 'MAIN', 'type': 'label'}], 'order': '1'},
    # {'instruction_name': 'LABEL', 'arg': [{'text': 'FOO', 'type': 'label'}], 'order': '2'},
    # {'instruction_name': 'ADD', 'arg': [{'text': 'LF@counter', 'type': 'var'}, {'text': 'LF@counter', 'type': 'var'}, {'text': '34', 'type': 'int'}], 'order': '3'}, ]
    # klice: instruction_name => string, arg => @array, order => int
    def checkParseSyntax(self):
        
        tree = ET.parse( self.xml)
        root = tree.getroot()
        sums = 0
        instruction_arg = []
        instruction = {}
        for child in root:
            #print(child.tag, child.attrib)
            if ( 'opcode' in child.attrib and 'order' in child.attrib ):        
                instruction_name = child.attrib[ 'opcode' ]
                instruction_order = int (child.attrib[ 'order' ])
            else:
                print ( "Chybi argument opcode nebo order u instrukce %s" % instruction_name, file=sys.stderr )
                exit ( 31)
            for child2 in child:
                # ulozim struktutu a zkontroluju syntax
                #print(child2.tag, child2.attrib, child2.text)
                sums = sums +1
                if ('type' in child2.attrib ):
                    # pokud se jedna o promennou, chceme si navic ukladat zvlast promennou a zvlast ramec
                    if ( child2.attrib[ 'type' ] == 'var' ):
                         var = re.match(  '^(GF|LF|TF)@(.*)$', child2.text)
                         if (var is not None):
                             instruction_arg.append( { 'type' : child2.attrib[ 'type' ], 'text' : child2.text, 'name' : var.group(2), 'frame' : var.group(1) } )
                    else:        
                        instruction_arg.append( { 'type' : child2.attrib[ 'type' ], 'text' : child2.text } )
                else:
                     print ( "Chybi argument type u instrukce %s" % instruction_name, file=sys.stderr )
                     exit ( 31)
            # ulozime do struktury, kterou budeme vracet
            instruction[ instruction_order ] = { 'instruction_name' : instruction_name, 'arg' : instruction_arg } 
            #print (instruction_name)
            #print (instruction_arg);
            if ( self._checkFormatOfOneInstruction( instruction_name, instruction_arg ) == False ):
                 print ( "Syntakticka chyba u instrukce %s" % instruction_name, file=sys.stderr )
                 exit ( 32)
            instruction_arg = []
            
        # konec zpracovani oznacime retezcem END, zarazka
        instruction[ instruction_order+1 ] = { 'instruction_name' : 'END', 'arg' : [] }
        #print (int(instruction_order)+1)
        return instruction
            
            

# pozn:
# CREATEFRAME - zahodi docesne ramce
# po PUSHFRAME -> vse, co bylo v TF bude v LF, lokalni se pouzivaji ve funkci
# po POPFRAME -> mam v TF to, co bylo v LF


class Main:
    def __init__(self):
        ########################
        # ZPRACOVANI ARGUMENTU #
        ########################
        
        if (len( sys.argv ) != 2 ):
            # ocekavame vzdy jeden argument
            print >> sys.stderr, "Spatny pocet argumentu, ocekavam jeden"
            exit( 10 )
        
        if (sys.argv[ 1 ] == '--help') :
            print( "NAPOVEDA" )
            exit(0)
        #print (sys.argv[1])
        split = re.match(  r"(--source=)(.+)", sys.argv[1] )
        if (split is None):
            print >> sys.stderr, "Pouzijte argument --help pro vypsani napovedy"
            exit( 10 )
        
        syntax = SyntaxParse( split.group(2) )
        structTree = syntax.checkParseSyntax()
        #print("Syntax OK")
        # mame strukturu, musime provest semantickou kontrolu
        #print(structTree)
        interpret = Interpret( structTree )
        #print( structTree[ '1' ] )
        interpret.findAllLabels();
        #print ( interpret.labels )
        interpret.proceed()
        #print("Interpret OK")
        
           
main = Main()

exit(0)    
    
