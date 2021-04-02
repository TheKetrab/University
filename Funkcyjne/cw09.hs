


{-# LANGUAGE KindSignatures, MultiParamTypeClasses, FlexibleInstances #-}

import Data.List (unfoldr)
import Data.Bool (bool)

(><) :: (a -> b) -> (a -> c) -> a -> (b,c)
(f >< g) x = (f x, g x)

warbler :: (a -> a -> b) -> a -> b
warbler f x = f x x




-- cw1

(<+>) :: Ord a => [a] -> [a] -> [a]
xs <+> [] = xs
[] <+> ys = ys
(x:xs) <+> (y:ys) =
    if x<y
    then (x : (xs <+> (y:ys)))
    else (y : ((x:xs) <+> ys))



class Ord a => Prioq (t :: * -> *) (a :: *) where
    empty :: t a
    isEmpty :: t a -> Bool
    single :: a -> t a
    insert :: a -> t a -> t a
    merge :: t a -> t a -> t a
    extractMin :: t a -> (a, t a)
    findMin :: t a -> a
    deleteMin :: t a -> t a
    fromList :: [a] -> t a
    toList :: t a -> [a]
    insert = merge . single
    single = flip insert empty
    extractMin = findMin >< deleteMin
    findMin = fst . extractMin
    deleteMin = snd . extractMin
    fromList = foldr insert empty
    toList = unfoldr . warbler $ bool (Just . extractMin) (const Nothing) . isEmpty


{-
	inscance (Ord a) => Prioq (ListPrioq) a where
		empty = LP[]
		isEmpty = null . unLP
		singl = LP . {:[]}
		merge (LP xs ) (LP ys) = LP $ xs <+> ys
		extractMin = (head >< {LP . tail}) . unLP

	instance(Ord a,  Show a) => Show (ListPrioq a) where
		show = show . toList
-}
	
	
	

newtype ListPrioq a = LP { unLP :: [a] }

instance (Ord a) => Prioq (ListPrioq) a where

  empty = LP([])

  isEmpty pq = 
    let LP(xs) = pq in
    if xs == [] then True
    else False

  single x = LP([x])

  insert x pq =
    let LP(xs) = pq in
    LP(xs<+>[x])

  merge pq1 pq2 =
    let (LP(xs),LP(ys)) = (pq1, pq2) in
    LP(xs<+>ys)

  extractMin pq =
    let LP(x:xs) = pq in
    (x,LP(xs))

  findMin pq =
    let LP(x:xs) = pq in
    x

  deleteMin pq =
    let LP(x:xs) = pq in
    LP(xs)

  fromList lst =
    case lst of
      [] -> empty
      (x:xs) -> (single x) `merge` (fromList xs)

  toList pq =
    case pq of
      empty -> []
      _ -> let extracted = (extractMin pq) in
           (fst extracted) : (toList (snd extracted))
  
  