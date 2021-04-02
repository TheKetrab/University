-- PF2018 Lista 10 Zadanie 1
{-# LANGUAGE MultiParamTypeClasses, FlexibleInstances  #-}

import Prioq

data Tree a = Node (Tree (a,a)) | Leaf a

instance Show a => Show (Tree a) where
   show (Node t) = '-' : show t
   show (Leaf a) = '<' : show a


instance Ord a => Base2Tree Tree a where

  -- size
  size (Leaf _) = 1
  size (Node t) = 2 * size t

  -- single
  treeSingle = Leaf

  -- root
  treeRoot (Leaf x) = x                  -- > zdejmij czapeczke <
  treeRoot (Node t) = fst $ (treeRoot t) -- > najbardziej po lewej <

  -- link
  treeLink (Leaf x) (Leaf y) =
    if (x < y)
      then Node(Leaf(x,y))
      else Node(Leaf(y,x))

  treeLink (Node tx) (Node ty) = Node $ (treeLink tx ty)

  -- unlink
  treeUnlink t =
    let (x,xs) = aux t in (x, reverse xs)
      where
        aux (Leaf x) = (x, [])
        aux (Node t) =
          let (fir, sec) = split t in
          let (x, xs) = aux fir in (x, sec:xs)
            where -- split
              split :: Tree (a,a) -> (Tree a, Tree a) -- > trzeba podac typ <
              split (Leaf x) = (Leaf $ fst x, Leaf $ snd x)
              split (Node t) = let (fir, sec) = split t in (Node fir, Node sec)

main = testHeap (fromList :: [Integer] -> DenseHeap Tree Integer)


-- PF2018 Lista 10 Zadanie 2
{-# LANGUAGE MultiParamTypeClasses, FlexibleInstances #-}

import Prioq
import HelperFunctions

data Heap a = Nil | Zero (Heap (a,a)) | One a (Heap (a,a))

instance Show a => Show (Heap a) where
   show Nil = "[]"
   show (Zero h) = "[-" ++ strfix (show h)
   show (One a h) = "[" ++ show a ++ strfix (show h)

instance Ord a => Prioq Heap a where
  
  -- empty
  empty = Nil

  -- isEmpty?
  isEmpty x = 
    case x of
      Nil -> True
      _ -> False

  -- single
  single x = One x Nil

  -- merge
  merge h1 h2 = aux h1 h2 Nothing
    where
      makePair :: Ord a => a -> a -> (a,a)
      makePair x y =
        if (x < y)
        then (x,y)
        else (y,x)
      -- wszystkie mozliwe kombinacje 3*3*2=18
      aux :: Ord a => Heap a -> Heap a -> Maybe a -> Heap a
      aux Nil Nil Nothing = Nil
      aux Nil Nil (Just t) = One t Nil
      aux Nil (Zero xs) Nothing = Zero xs
      aux Nil (Zero xs) (Just t) = One t $ (aux Nil xs Nothing)
      aux Nil (One t xs) Nothing = One t $ (aux Nil xs Nothing)
      aux Nil (One t1 xs) (Just t2) = Zero $ (aux Nil xs (Just (makePair t1 t2)))
      aux (Zero xs) Nil Nothing = Zero xs
      aux (Zero xs) Nil (Just t) = One t $ (aux xs Nil Nothing)
      aux (Zero xs) (Zero ys) Nothing = Zero $ (aux xs ys Nothing)
      aux (Zero xs) (Zero ys) (Just t) = One t $ (aux xs ys Nothing)
      aux (Zero xs) (One t2 ys) Nothing = One t2 $ (aux xs ys Nothing)
      aux (Zero xs) (One t2 ys) (Just t3) = Zero $ (aux xs ys (Just (makePair t2 t3)))
      aux (One t xs) Nil Nothing = One t xs 
      aux (One t1 xs) Nil (Just t3) = Zero $ (aux xs Nil (Just (makePair t1 t3)))
      aux (One t1 xs) (Zero ys) Nothing = One t1 $ (aux xs ys Nothing)
      aux (One t1 xs) (Zero ys) (Just t3) = Zero $ (aux xs ys (Just (makePair t1 t3)))
      aux (One t1 xs) (One t2 ys) Nothing = Zero $ (aux xs ys (Just (makePair t1 t2)))
      aux (One t1 xs) (One t2 ys) (Just t3) = One t3 $ (aux xs ys (Just (makePair t1 t2)))
      
  -- extract min TODO
  extractMin = undefined

main = testHeap (fromList :: [Integer] -> Heap Integer)
