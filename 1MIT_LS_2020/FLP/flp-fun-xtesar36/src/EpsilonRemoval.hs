-- FLP - 1st project (rka-2-dka)
-- @Autor: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
-- @Datum: april, 2020
-- -------------------------
module EpsilonRemoval where
import Data.List
import System.IO
import Types (FiniteMachine(..), Rule(..), RuleNew(..))

------------------------- EPSILON REMOVAL ----------------------------------------------------------------
{--
@function removalEpsilonTransitionMain 
@describe main function to delete the epsilon transition and generate new rules
@note we need a duplicated rule to work with in finding the epsilon closure (uzaver) in function findEpsilon
@return 
	[Rule] - the rules generated while removing epsionn transitions
--}
removalEpsilonTransitionMain :: [Rule] -> [Rule] -> [Rule]
removalEpsilonTransitionMain [] _ = []
removalEpsilonTransitionMain _ [] = []
removalEpsilonTransitionMain (x:xs) oldRulesCopy = removalEpsilonTransition oldRulesCopy x ++ removalEpsilonTransitionMain xs oldRulesCopy


{--
@function removalEpsilonTransition 
@describe is takes one single rule and generates new rules according to epsilon closure (uzaver)
@example (1,b,2), allrules => (1,b,2), (1,b,3)
@note deleteEmptyItemsFromList is just for deleting the empty items from the list
@return 
	[Rule] - the rules generated while removing epsilon transitions from ONE rule
--}
removalEpsilonTransition :: [Rule] -> Rule -> [Rule]
removalEpsilonTransition rulesAll (Rule q a n) = removalEpsilonTransition' (findEpsilonWithoutDuplicities n rulesAll) (Rule q a n)

{--
@function removalEpsilonTransition 
@describe this function takes all states in eps-closure(q) and generates rules
@example (2,3) (1,b,2) => (1,b,2), (1,b,3)
@return 
	[Rule] - generated rules
--}
removalEpsilonTransition' :: [String] -> Rule -> [Rule]
removalEpsilonTransition' _ (Rule q "" n) = []
removalEpsilonTransition'  [] _ = []
removalEpsilonTransition' (x:xs) (Rule q a n) = (Rule q a x) : removalEpsilonTransition' xs (Rule q a n)


{--
@function findEpsilon 
@describe this function finds a eps-closure(state)
@example (2) [(1,b,2), (2,a,2), (2,,3)] => (2,3)
@return 
	[String] - eps-closure(state)
--}
findEpsilonWithoutDuplicities  :: String -> [Rule] -> [String]
findEpsilonWithoutDuplicities q rule = deleteEmptyItemsFromList (findEpsilon q rule)
   where
      {--
      @function deleteEmptyItemsFromList 
      @describe deletes empty "" items from the list
      @example [,,,2,3] => [2,3]
      @return 
	      [String] - list without empty space items
      --}
      deleteEmptyItemsFromList :: [String] -> [String]
      deleteEmptyItemsFromList [] = []
      deleteEmptyItemsFromList ("":xs) = deleteEmptyItemsFromList xs
      deleteEmptyItemsFromList (x:xs) = x : deleteEmptyItemsFromList xs

findEpsilon :: String -> [Rule] -> [String]
findEpsilon q [] = []
findEpsilon q (x:[]) = [q]
findEpsilon q (x:xs) = findEpsilon' q x : findEpsilon q xs 
   where
    -- { eps(p) = q | p ->* q }
    -- this function finds the q from p
    findEpsilon' :: String -> Rule -> String 
    findEpsilon' qdif (Rule q "" n) = if qdif == q then n else []
    findEpsilon' _ _ = []


------------------------------------- END EPSILON REMOVAL ---------------------------------