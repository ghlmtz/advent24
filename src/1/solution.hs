import Helper
import Data.List (sort)

second :: [a] -> [a]
second (x:y:xs) = x : second xs
second (x:xs) = [x]
second [] = []

part1 :: String -> Int
part1 = sum . map abs . diffs
    where diffs = zipWith (-) . sort . second . ints <*> sort . second . drop 1 . ints

foo :: [Int] -> Int -> Int
foo arr el = (length . filter (==el)) arr * el 

part2 :: String -> Int
part2 = sum . similarity
    where similarity = map . foo . second . drop 1 . ints <*> second . ints

main :: IO ()
main = do
    run part1
    run part2