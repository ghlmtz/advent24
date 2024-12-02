module Helper
( readInput
, readTest
, readAndPrint
, run
, runTest
, intsBy
, wordsBy
, ints
) where

import Data.Char (isDigit)
import System.Directory (doesFileExist)
import System.IO

run :: Show a => (String -> a) -> IO ()
run f = fmap f readInput >>= print

runTest :: Show a => (String -> a) -> IO ()
runTest f = fmap f readTest >>= print

readAndPrint :: IO() 
readAndPrint = readInput >>= print

readInput :: IO String
readInput = do
  result <- doesFileExist "input.txt"
  case result of
    True  -> readFile "input.txt"
    False -> readFile "input"

readTest :: IO String
readTest = readFile "test"

intsBy :: (Char -> Bool) -> String -> [Int]
intsBy isDelim xs =               
  case dropWhile isDelim xs of
    ""  -> []
    xs' -> read w : intsBy isDelim rest
          where (w, rest) = break isDelim xs'

wordsBy :: (Char -> Bool) -> String -> [String]
wordsBy isDelim xs =               
  case dropWhile isDelim xs of
    ""  -> []
    xs' -> w : wordsBy isDelim rest
          where (w, rest) = break isDelim xs'

ints :: String -> [Int]
ints xs =               
  case dropWhile (not . isDigit) xs of
    ""  -> []
    xs' -> read w : ints rest
          where (w, rest) = break (not . isDigit) xs'