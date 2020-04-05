-- FLP - 1st project (rka-2-dka)
-- @Autor: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
-- @Datum: april, 2020
-- -------------------------
module GeneralFunctions where
import Types (FiniteMachine(..), Rule(..), RuleNew(..))

------------------------------------- COMPARING FUNCTIONS ---------------------------------------

-- comparing 2 lists (the lists are not ordered)
compareListAll :: [String] -> [String] -> Bool
compareListAll list1 list2 = comparingLists list1 list2 && comparingListsReverse list1 list2
    where 
        -- everything what is on left side is on right side
        comparingLists :: [String] -> [String] -> Bool
        comparingLists [] [] = True
        comparingLists [] x = True
        comparingLists x [] = False
        comparingLists (x:xs) list = comparingLists' x list && comparingLists xs list
        
        -- everything what is on right side is on left side
        comparingListsReverse :: [String] -> [String] -> Bool
        comparingListsReverse [] [] = True
        comparingListsReverse [] x = False
        comparingListsReverse x [] = True
        comparingListsReverse list (x:xs)= comparingLists' x list && comparingListsReverse list xs

        comparingLists' x [] = False 
        comparingLists' x (y:ys) 
             | x == y = True
             | otherwise = comparingLists' x ys
     
-----------------------------------------------------------
printRules :: [Rule] -> String
printRules [] = ""
printRules (x:xs) = printRule x ++ "\n" ++ printRules xs 
  where
     printRule :: Rule -> String
     printRule (Rule s a n) = s ++ "," ++ a ++ "," ++ n
------------------------------------------------------------
printNewRules :: [RuleNew] -> String
printNewRules [] = ""
printNewRules (x:xs) = printNewRule x ++ "\n" ++ printNewRules xs 
  where
     printNewRule :: RuleNew -> String
     printNewRule (RuleNew s a n) = "[" ++ "" ++ (printindEpsilon s) ++ "," ++ a ++ "," ++ (printindEpsilon n) ++ "]"


-- helpful function for printing 
printindEpsilon :: [String] -> String
printindEpsilon [] = []
printindEpsilon (x:[]) =  x 
printindEpsilon (x:xs) = "[" ++ x ++ ","++ printindEpsilon xs ++"]"

printerList :: [[String]] -> [String]
printerList [] = [[]]
printerList (x:xs) = x ++ printerList xs

printerList2 :: [[String]] -> String
printerList2 [] = []
printerList2 (x:xs) = "|" ++ printindEpsilon(x) ++ "|" ++ printerList2 xs

-- @function member
-- @describe finds out if element is inside an array
-- @return True|False if this element is in Array
member :: [String] -> [[String]] -> Bool
member elem [] = False
member elem (x:xs)
   | compareListAll elem x = True -- no need of order
   | otherwise = member elem xs
   
-- @function member
-- @describe finds out if element is inside an array
-- @return True|False if this element is in Array
member2 :: String -> [String] -> Bool
member2 elem [] = False
member2 elem (x:xs)
   | elem == x = True -- no need of order
   | otherwise = member2 elem xs