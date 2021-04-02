
import Prelude hiding ((++), head, tail, length, null, (!!))
import qualified Prelude ((++), head, tail, length, null, (!!))

-- zad 1
class List l where
    nil :: l a
    cons :: a -> l a -> l a
    head :: l a -> a
    tail :: l a -> l a
    (++) :: l a -> l a -> l a
    (!!) :: l a -> Int -> a
    toList :: [a] -> l a
    fromList :: l a -> [a]


instance List [] where
    nil = []
    cons x xs = x:xs
    head (x:xs) = x
    tail (x:xs) = xs
    [] ++ ys = ys
    (x:xs) ++ ys = x:(xs++ys)
    (x:xs) !! 0 = x
    (x:xs) !! n = xs !! (n-1)
    toList [] = []
    toList xs = xs
    fromList xs = xs

-- zad 2
class List l => SizedList l where
    length :: l a -> Int
    null :: l a -> Bool
    null l = length l == 0

instance SizedList [] where
    length [] = 0
    length (x:xs) = 1 + length xs
    null [] = True
    null (x:xs) = False


-- zad 3
data SL l a = SL { len :: Int, list :: l a }

instance List l => List (SL l) where
    nil = SL 0 nil
    cons x (SL n xs) = SL (n+1) (cons x xs)
    head = head . list
    tail (SL n xs) = SL (n-1) (tail xs)
    (SL n1 xs) ++ (SL n2 ys) = SL (n1+n2) (xs ++ ys)
    xs !! n = (list xs) !! n
    toList = list . toList
    fromList =  fromList . list

instance List l => SizedList (SL l) where
    length (SL n xs) = n
    null xs = let (SL n lst) = xs in
        if (n == 0) then True else False


-- zad 4
infixr 6 :+
data AppList a = Nil | Sngl a | AppList a :+ AppList a

instance (Show a) => Show (AppList a) where
    show Nil = ""
    show (Sngl a) = show a
    show (a :+ b) = (show a ++ ";" ++ show b)

    
example = (Sngl 2) :+ Nil


-- ----- -----
-- idea:
-- cons x xs =     :+
--               x    xs
--
-- append xs ys =   :+
--               xs    ys
--

instance List AppList where

    nil = Nil

    cons x Nil = Sngl x
    cons x (Sngl y) = (Sngl x) :+ (Sngl y)
    cons x (xs :+ ys) = (cons x xs) :+ ys

    head Nil = error "head on nil"
    head (Sngl x) = x
    head (xs :+ ys) = head xs

    tail Nil = error "tail on nil"
    tail (Sngl x) = Nil
    tail (xs :+ ys) = -- chcemy odciac pierwszy element ze struktury, czyli element w drzewie najbardziej po lewej
        case xs of
            Nil -> error "tail on nil"
            (Sngl _) -> ys -- odcinamy z drzewa 'xs'
            (xs' :+ ys') -> (tail xs' :+ ys')

    Nil ++ ys = ys
    xs ++ ys = xs :+ ys

    xs !! n = undefined -- TODO

    toList [] = Nil
    toList (x:xs) = (Sngl x) :+ (toList xs)

    fromList xs = undefined -- TODO
    
instance SizedList (AppList) where
    
    length Nil = 0
    length (Sngl a) = 1
    length (xs :+ ys) = (length xs) + (length ys)

    null Nil = True
    null _ = False

