-- FLP - 1st project (rka-2-dka)
-- @Autor: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
-- @Datum: april, 2020
-- -------------------------
module Types where
-- Import modules
import Data.List
import System.IO

-- new datatype Rule for rules in finit machine
data Rule = Rule {
   fromState :: String, -- s
   alphabetSymbol :: String, -- a
   toState :: String -- f
} deriving (Eq)

data RuleNew = RuleNew {
   fromStateNew :: [String], -- s
   alphabetSymbolNew :: String, -- a
   toStateNew :: [String] -- f
} deriving (Eq)

-- new datatype FiniteMachine
data FiniteMachine = FiniteMachine {
   states :: [String], -- Q
   alphabet :: [String], -- A as Alfabeta
   initState :: String, -- S
   finitStates :: [String], -- F
   rules :: [Rule] -- D for delta
} deriving (Eq)

-- printing the State machine according to the assesment
instance Show Rule where
   show (Rule s a f) = show s ++ ", " ++ show a ++ ", " ++ show f ++ "\n"

instance Show FiniteMachine where
    show (FiniteMachine q a s f d) = show q ++ "\n" ++ show a ++ "\n" ++ show s ++ "\n" ++ show f ++ "\n" ++ show d
