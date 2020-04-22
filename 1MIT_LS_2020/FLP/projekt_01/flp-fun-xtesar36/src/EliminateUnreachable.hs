-- FLP - 1st project (rka-2-dka)
-- @Autor: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
-- @Datum: april, 2020
-- -------------------------
module EliminateUnreachable where

import Types (FiniteMachine(..), Rule(..), RuleNew(..))
import GeneralFunctions(compareListAll,member)

------------------------------------- REMOVAL OF UNREACHABLE STATES -----------------------------
{--
@function eliminateUnreachable
@describe functions starts with initial state and goes throw all states that initialState generates ->
then deletes all the rules that contains unreachable states
@return RuleNew new rules
--}
eliminateUnreachable :: [RuleNew] -> [[String]] -> [RuleNew] -> [RuleNew]
eliminateUnreachable [] initState rules = []
eliminateUnreachable ((RuleNew q a n):xs) initState rules = do
    let reachableStates = findStatesWithStartState initState rules
    if member q reachableStates && n /= [] -- is reachable and not empty
    then (RuleNew q a n) : eliminateUnreachable (xs) initState rules
    else eliminateUnreachable (xs) initState rules
    where
       findStatesWithStartState :: [[String]] -> [RuleNew] -> [[String]]
       findStatesWithStartState initState rules = do
           let nextStates = findStatesWithStartState' initState rules
           if nextStates == initState -- attention, may be a problem, check order
              then nextStates
              else findStatesWithStartState nextStates rules
       -- puts 2 arrays together to be able to compare previous and actual state
       findStatesWithStartState' :: [[String]] -> [RuleNew] -> [[String]]
       findStatesWithStartState' [] rules = []
       findStatesWithStartState' (x:xs) rules = unionArrayStrings (findStatesWithStartState'' x rules) (findStatesWithStartState' xs rules )
       -- union 2 arrays
       unionArrayStrings :: [[String]] -> [[String]] -> [[String]]
       unionArrayStrings [] list2 = list2
       unionArrayStrings list [] = list
       unionArrayStrings (x:xs) list
          | member x list = unionArrayStrings xs list
          | otherwise = x : unionArrayStrings xs list

       -- initState -> all rules -> states that I can get from initState
       findStatesWithStartState'' :: [String] -> [RuleNew] -> [[String]]
       findStatesWithStartState'' start [] = [start]
       findStatesWithStartState'' start ((RuleNew q a n):xs)
              | compareListAll start q = n : findStatesWithStartState'' start xs
              | otherwise = findStatesWithStartState'' start xs


   
------------------------------------ REMOVAL OF UNREACHABLE STATES END --------------------------