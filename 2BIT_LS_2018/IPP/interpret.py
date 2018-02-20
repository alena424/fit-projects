# interpret

# 1. rozparsovani xml a ukladani udaju
# < instrction opcode = >
# <argX type = var, string, int, bool, label, type


class Operand:
    def __init__(self, type_arg, contain):
        self.type_arg = type_arg
        self.contain = contain

# promenna bude dedit z operandu
class Variable( Operand ):
    defined = 0
    
    def __init__(self, contain):
        self.type_arg = 'var'
        self.contain = contain
        
    def define():
        self.defined = 1

class SymTable:
    # { 'varname' : { type : $type, defined : 0|1, params : @{ name : $name, type: $type } } }
    actualnumber = 0
    
    def __init__(self, is_global ):
       self.table = []
       self.is_global = is_global
       

    def addItem( self, type_arg, name, frame ):
         self.table.append ( { 'name' : name, 'type' : type_arg, 'defined' : 0, 'frame' : frame, 'params' : []  } )
         self.actualnumber = self.actualnumber + 1
         
    def addParamToItem( self, name_function, name_par, type_par ):
        for hash_item in self.table:
            if hash_item[ 'name' ] == name:
                # nasli jsme
                hash_item[ 'params' ][ 'name' ] = name_par
                hash_item[ 'params' ][ 'type' ] = type_par
                
    def seachItem( self, name ):
         for hash_item in self.table:
            if hash_item[ 'name' ] == name:
                return hash_item
            else:
                return 0
            
    def defineItem( self, name ):
         for hash_item in self.table:
            if hash_item[ 'name' ] == name:
                hash_item[ 'defined' ] = 1
    
    def isDefined( self, name ):
        for hash_item in self.table:
            if hash_item[ 'name' ] == name:
                return hash_item[ 'defined' ];
            else:
                # pokud neexistuje, neni definovana
                return 0
            
    def printStructure(self):
        for hash_item in self.table:
            print ("[name] = %s, [type] = %s, [defined] = %s, [params] = {" % ( hash_item[ 'name' ], hash_item[ 'type' ], hash_item[ 'defined' ] ) )
            for par in hash_item[ 'params' ]:
                print ( "name = %s, type = %s", par[ 'name' ], par[ 'type' ] )
            print( "}" )
            
        
# pozn:
# CREATEFRAME - zahodi docesne ramce
# po PUSHFRAME -> vse, co bylo v TF bude v LF, lokalni se pouzivaji ve funkci
# po POPFRAME -> mam v TF to, co bylo v LF
      
global_table = SymTable( 1 )
#global_table.printStructure()
global_table.addItem( 'var', 'nazev', 'GF' )
global_table.defineItem( 'nazev' )
global_table.printStructure()

global_table.addItem( 'bool', 'sss' )
        
    
    
    
