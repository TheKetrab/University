
-----
-- zadanie 2 DONE

let f xs = [k | k <- xs, k `mod` (head xs) /= 0]
let primes = map head (iterate f [2..10])

-----
-- zadanie 3 DONE
primes' = 2 : [p | p <- [3..], all (\q -> p `mod` q /= 0) (takeWhile (\q -> q*q <= p) primes')]

-----
-- zadanie 4 DONE
fib = 1:1:(zipWith (+) fib (tail fib))

-----
--zadanie 5 DONE

{--- IPERM ---}
:{
iperm :: [a] -> [[a]]
iperm [] = [[]]
iperm (x:xs) =
	-- spermutuj xs i wstaw wszedzie gdzie sie da x
    foldr (++) [] (map (insertEverywhere [] x) (iperm xs))
    where
        insertEverywhere :: [a] -> a -> [a] -> [[a]]
        insertEverywhere xs x [] = [xs ++ [x]]
        insertEverywhere xs x (y:ys) =
            (xs ++ (x : y : ys)) : (insertEverywhere (xs ++ [y]) x ys)
:}

{--- SPERM ---}
:{
sperm :: [a] -> [[a]]
sperm [] = []
sperm [x] = [[x]]
sperm xs =
	-- wybierz element, spermutuj liste bez tego elementu
	-- dolacz element na poczatek kazdej listy wyniku
	-- zrob to dla kazdego elementu w liscie wejsciowej
    [y:zs | (y,ys) <- selectElements xs, zs <- sperm ys]
    where
		-- select zwraca liste par: (x, lista bez x)
        selectElements :: [a] -> [(a, [a])]
        selectElements [x] = [(x, [])]
        selectElements (x:xs) = (x, xs) : [(y, x:ys) | (y, ys) <- selectElements xs]
 
:}


-----
-- zadanie 6 DONE
:{
sublist :: [x] -> [[x]]
sublist [] = [[]]
sublist (x:xs) =
    let tailRes = sublist xs
    in (map (\ys -> x:ys) tailRes) ++ tailRes
:}


-----
-- zadanie 7 DONE
:{
qsortBy :: (a -> a -> Bool) -> [a] -> [a]
qsortBy f [] = []
qsortBy f (x:xs) =   
    let smallerSorted = qsortBy f [a | a <- xs, not (f a x)]  
        biggerSorted  = qsortBy f [a | a <- xs, f a x]  
    in  smallerSorted ++ [x] ++ biggerSorted    
:}

-- > qsortBy (<) [1,1,6,32,457,45,3,345,27,7,5]
-- [457,345,45,32,27,7,6,5,3,1,1]



-----
-- zadanie 8
-- brakuje:
-- setUnion :: Ord a => Set a -> Set a -> Set a
-- setIntersection :: Ord a => Set a -> Set a -> Set a
-- setComplement :: Ord a => Set a -> Set a


data Tree a = Node (Tree a) a (Tree a) | Leaf
data Set a = Fin (Tree a) | Cofin (Tree a)

:{

-- insert 
insert :: Ord a => a -> Tree a -> Tree a
  insert a Leaf = Node Leaf a Leaf
  insert a (Node left val right)
      | a < val = Node (insert a left) val right
      | otherwise = Node left val (insert a right)

-- build tree
buildTree :: Ord a => [a] -> Tree a
  buildTree [] = Leaf
  buildTree (x:xs) = insert x $ buildTree xs

-- setFromList
setFromList :: Ord a => [a] -> Set a
  setFromList [] = Fin Leaf
  setFromList (x:xs) = Fin (buildTree xs)

-- setEmpty
setEmpty :: Ord a => Set a
  setEmpty = Fin Leaf

-- setFull
setFull :: Ord a => Set a
  setFull = Cofin Leaf

-- setMember
setMember :: Ord a => a -> Set a -> Bool
  setMember _ (Fin Leaf) = False
  setMember a (Cofin s) = not (setMember a (Fin s))
  setMember a (Fin (Node left val right))
      |  a == val = True
      |  otherwise = setMember a (Fin left) || setMember a (Fin right)
:}