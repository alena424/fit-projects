-- FLP - 1st project (rka-2-dka)
-- @Autor: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
-- @Datum: april, 2020
-- -------------------------
module Determinization where
-- Import modules
import Data.List
import System.IO
import Types (FiniteMachine(..), Rule(..), RuleNew(..))
import GeneralFunctions(compareListAll)

------------------------------------- DETERMINIZATION -------------------------------------

-- allRules -> allSymbols -> AllStates -> newRules
makingTableForDetermineIterateSymbols :: [Rule] -> [String] -> [[String]] -> [RuleNew]
makingTableForDetermineIterateSymbols rules [] states  = []
makingTableForDetermineIterateSymbols rules (x:xs) states = makingTableForDetermineIterateStates rules x states ++ makingTableForDetermineIterateSymbols rules (xs) states 
  where
      -- allrules -> symbol -> powerSetStates 
      makingTableForDetermineIterateStates :: [Rule] -> String -> [[String]] -> [RuleNew]
      makingTableForDetermineIterateStates rules symbol [] = []
      makingTableForDetermineIterateStates rules symbol [[]] = [] -- emty set not needed
      makingTableForDetermineIterateStates rules symbol (x:xs) = makingTableForDetermineIterateStates' x symbol rules : makingTableForDetermineIterateStates rules symbol xs
      
      -- fromStates -> symbol -> rules -> toStates
      makingTableForDetermineIterateStates' :: [String] -> String -> [Rule] -> RuleNew
      makingTableForDetermineIterateStates' q a rules = RuleNew q a (removeDup nextStates) 
          where nextStates = findStates q a rules

      -- fromStates -> symbol -> rules -> toStates
      findStates :: [String] -> String -> [Rule] -> [String]
      findStates [] _ _ = []
      findStates (x:xs) symbol rules = findStates' x symbol rules ++ findStates xs symbol rules

      -- fromState (ONE) -> symbol (ONE) -> rules -> toStates
      findStates' :: String -> String -> [Rule] -> [String]
      findStates' _ _ [] = []
      findStates'  startState symbol ((Rule q a n):xs) = (if startState == q && symbol == a then [n] else []) ++ findStates'  startState symbol xs
      
-- function removes duplicates string in list
removeDup :: [String] -> [String]
removeDup [] = []
removeDup (x:xs) = x : (removeDup (removeDup' x xs))
   where 
      removeDup' :: String -> [String] -> [String]
      removeDup' x [] = []
      removeDup' x (y:ys)
              | x == y = removeDup' x ys
              | otherwise = y:(removeDup' x ys)


------------------------------------- END DETERMINIZATION ---------------------------------------