-- FLP - 1st project (rka-2-dka)
-- @Autor: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
-- @Datum: april, 2020
-- -------------------------
module Main (main) where
-- Import modules
import Data.List
import System.IO
import System.Environment -- we will need to get the arguments

import EpsilonRemoval
import Determinization
import Types (FiniteMachine(..), Rule(..), RuleNew(..))
import GeneralFunctions
import EliminateUnreachable

{--
@function processArguments 
@describe to process the arguments on input
@return 
	Bool - conversion Yes/No
	String - name of file or empty string
--}
processArguments :: [String] -> (Bool, String)
processArguments [x]  
   | x == "-i" = (False, "") 
   | x == "-t" = (True,"")
   | otherwise = error "Input is in incorrect format, run like: ./rka-2-dka (-i|-t) [file]."
processArguments[x,y] 
   | x == "-i" = (False, y) 
   | x == "-t" = (True, y)
   |otherwise= error "Input is in incorrect format, run like: ./rka-2-dka (-i|-t) [file]."
processArguments _ = error "Input is in incorrect format, run like: ./rka-2-dka (-i|-t) [file]."

{--
@function splitOn 
@describe to split a string delimite by comma
@return 
[String] - separated string
--}
splitOn :: String -> [String]
splitOn [] = []
splitOn s = x : splitOn (drop 1 y) where (x,y) = span (/= ',') s

----- processing alphabets ---------------------------------------------------
processAlphabet' :: String -> [String]
processAlphabet' [] = []
processAlphabet' (x:xs) = [x] : processAlphabet' xs

{--
@function parserFile 
@describe to parser the arguments and make FSM structure
@return 
	FiniteMachine 
--}
parserFile :: [String] -> FiniteMachine
parserFile [] = error "Nothing on input"
parserFile(states:alphabet:initState:finitStates:rules)
    | states == [] = error "There are no states."
    | alphabet == [] = error "There are no symbols."
    | initState == [] = error "There is no initial state."
    | otherwise = FiniteMachine processStates (processAlphabet' alphabet) initState processFinitStates processRules
        where
          processStates = splitOn states -- split by commas
          processFinitStates = splitOn finitStates -- split by commas
          
          ----- processing rules and saving them to a structure ------------------------
          processRules =  if rules == [] then [] else (map getTransitions rules) -- map applies a function processRule to evry item in the list
          
          getTransitions rule = processRules' (splitOn rule) -- here we can work with a single line of rule
    
          processRules' :: [String] -> Rule
          processRules' [s, a, f] = Rule s a f
          processRules' _ = error "Rules must come in this format: state, alphabet, nextState separated by newline."
          
parserFile(states:alphabet:initState:[]) = FiniteMachine (splitOn states) (processAlphabet' alphabet) initState [] []
parserFile _ = error "Error in input file. Please put: \n<states>\n<alfabet>\n<initialState>\n<finalStates>\n<rule_1>\n<rule_n>"

printerFSM :: FiniteMachine -> IO ()
printerFSM (FiniteMachine states alphabet initState finitStates rules) =
    putStrLn ( q ++ "\n" ++ a ++ "\n" ++ s ++ "\n" ++ f ++ "\n" ++ d)
    where 
       q = printCommas states
       a = printHelp alphabet
       s = initState
       f = printCommas finitStates
       d = printRules rules

       printHelp :: [String] -> String
       printHelp [] = []
       printHelp (x:xs) = x ++ printHelp xs

       printCommas :: [String] -> String
       printCommas [] = []
       printCommas (x:[]) = x -- the end of comma not needed
       printCommas (x:xs)  = x ++ "," ++ printCommas xs

-- allStatesIndex -> expression -> initNumber
getIndex :: [[String]]-> [String] -> Int -> String
getIndex [] state number = "error" -- error index
getIndex (x:xs) state number 
   | compareListAll state  x = show number -- order does not matter -- that is why compareListAll
   | otherwise = getIndex xs state (number + 1)


-- Takes rules and find all the states that will be converted
findAllStates :: [RuleNew] -> [[String]]
findAllStates [] = []
findAllStates ((RuleNew q s n):xs) = q : n : findAllStates xs

-- function removes duplicates string in list
removeDupArray :: [[String]] -> [[String]]
removeDupArray [] = []
removeDupArray (x:xs) = x : (removeDupArray (removeDupArray' x xs))
   where 
      removeDupArray' :: [String] -> [[String]] -> [[String]]
      removeDupArray' x [] = []
      removeDupArray' x (y:ys)
              | compareListAll x y = removeDupArray' x ys
              | otherwise = y:(removeDupArray' x ys)
              
--------------------- RENAMING RULES ----------------------------------------------------------
-- rules must be names with a digit number - this digit feres to index in all rules array
renameRulesToSingleNumber :: [[String]] -> [RuleNew] -> [RuleNew]
renameRulesToSingleNumber [] _ = []
renameRulesToSingleNumber allStatesStringArray [] = []
renameRulesToSingleNumber allStatesStringArray (x:xs) = renameRulesToSingleNumber' x allStatesStringArray : renameRulesToSingleNumber allStatesStringArray xs
   where
      renameRulesToSingleNumber' :: RuleNew -> [[String]] -> RuleNew
      renameRulesToSingleNumber' (RuleNew q a n) allStatesStringArray = RuleNew [(getIndex allStatesStringArray q 0 )] a [(getIndex allStatesStringArray n 0 )]

---------------------- CONVERSION NEWRULES TO RULES --------------------------------------------

conversionRules :: [RuleNew] -> [Rule]
conversionRules [] = []
conversionRules (x:xs) = conversionRules' x : conversionRules xs
   where conversionRules' (RuleNew q a n) = Rule (toString q) a (toString n)
         toString :: [String] -> String
         toString []= []
         toString (x:xs) = x ++ toString xs

--------------------- NEW FSM --------------------------------------
getStatesNew :: [Rule] -> [String]
getStatesNew [] = [] 
getStatesNew ((Rule q a n):xs) = q : n : getStatesNew xs

-- all new states -> finalState -> allStatesArray intersection TODO
getFinalStatesIntersection :: [String] -> [String] -> [[String]] -> [String]
getFinalStatesIntersection _ _ [] = []
getFinalStatesIntersection _ [] _ = []
getFinalStatesIntersection [] _ _ = []
getFinalStatesIntersection ([]:xs) finalStates allStatesIndex = getFinalStatesIntersection xs finalStates allStatesIndex
getFinalStatesIntersection (x:xs) finalStates allStatesIndex
   | findIn finalStates (fromIndexToList allStatesIndex 0 (read x :: Integer)) = x : getFinalStatesIntersection xs  finalStates allStatesIndex
   | otherwise = getFinalStatesIntersection xs finalStates allStatesIndex
   where 
       -- finalStates -> searchedRules
       findIn :: [String] -> [String] -> Bool
       findIn [] p = False
       findIn (x:xs) searchedRules = findIn' x searchedRules || findIn xs searchedRules

       findIn' :: String -> [String] -> Bool
       findIn' x [] = False
       findIn' x (y:ys)
           | x == y = True
           | otherwise = findIn' x ys
           
       fromIndexToList :: [[String]] -> Integer -> Integer -> [String]
       fromIndexToList []  _ _ = []
       fromIndexToList (x:xs) state n 
         | n == state = x
         | otherwise = fromIndexToList xs (state + 1) n

-- function creates new DFSM with already converted ruled, must recount just initalState and finalStates
createNewFinitMachine :: [String] -> [Rule] -> [String] -> [String] -> [String] -> [[String]] -> FiniteMachine
createNewFinitMachine states rules alphabet initStates finalStates allStatesArray = FiniteMachine states alphabet initState finitStates rules
  where
   initState = getIndex allStatesArray initStates 0  
   finitStates = getFinalStatesIntersection states finalStates allStatesArray

------------ MAIN FUNCTION DETERMINIZATING FSM -------------------------------------------------
powerSet :: [String] -> [[String]]
powerSet [] = [[]]
powerSet (x:xs) = [x:ps | ps <- powerSet xs ] ++ powerSet xs

-- main function for determinization
determineFSM :: FiniteMachine -> IO ()
determineFSM (FiniteMachine states alphabet initState finitStates rules) = do
   let updatedRulesCleanedFromEpsilon = removalEpsilonTransitionMain rules rules
   let powerSetAllStates = powerSet states -- 2^Q
        where powerSet :: [String] -> [[String]]
              powerSet [] = [[]]
              powerSet (x:xs) = [x:ps | ps <- powerSet xs ]++ powerSet xs

   let newRules =  ( makingTableForDetermineIterateSymbols updatedRulesCleanedFromEpsilon alphabet powerSetAllStates ) -- making table with all possible rules 2^Q x Sigma -> 2^Q 
   let initStates = findEpsilonClosure [] [initState] rules                                                            -- Qinit' = e-closure(Qunit)
   let removeUnReachable = eliminateUnreachable newRules [initStates] newRules                                         --  2^Q rules minus unreachable rules
   let findAllStatesVar = removeDupArray( findAllStates removeUnReachable)                                             -- array of all state that are in new rules ([NewRule])
   let renamedRules = renameRulesToSingleNumber findAllStatesVar removeUnReachable                                     -- renamed rules by single number [0..n]
   let convertedRules = conversionRules renamedRules                                                                   -- RuleNew -> Rule (Rules we will need in final DFSM)
   let states = removeDup(  getStatesNew convertedRules)                                                               -- array of all state that are in new rules ([Rule])   
   let newFiniteMachine = createNewFinitMachine states convertedRules alphabet initStates finitStates findAllStatesVar -- newFiniteMachine
   printerFSM newFiniteMachine                                                                                         -- printing results newFiniteMachine
   return()

--------------------------- MAIN PROGRAM ---------------------------------------------------
main :: IO ()
main = do
  arguments <- getArgs
  let (willBeConverted, fileName) = processArguments arguments
  fileContent <- if fileName /= "" then readFile fileName else hGetContents stdin -- getting the content of file or stdin
  let lineFile = lines fileContent -- we will parse the file by lines
  let fsm = parserFile lineFile
  fsmOutput <- if willBeConverted then determineFSM fsm else printerFSM fsm
  return ()