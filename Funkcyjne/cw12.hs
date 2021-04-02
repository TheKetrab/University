{-# LANGUAGE FunctionalDependencies, FlexibleContexts, FlexibleInstances #-}

import Data.Char (toLower)

import qualified Data.Char as C
import System.IO

class Monad m => StreamTrans m i o | m -> i o where
    readS :: m (Maybe i)
    emitS :: o -> m ()

-- zad 1
toLower :: StreamTrans m Char Char => m Integer
toLower = aux 0 where
    aux i = do 
        c <- readS
        case c of Nothing -> return i
        case c of (Just a) -> emitS (Data.Char.toLower a)
        aux (i+1)


-- zad 2
instance StreamTrans IO Char Char where
    readS = do b <- isEOF
               case b of
                True -> return Nothing
                False -> do
                  c <- getChar
                  return (Just c)
    emitS = putChar


newtype ListTrans i o a = LT { unLT :: [i] -> ([i], [o], a) }

--transform :: ListTrans i o a -> [i] -> ([o], a)

instance Functor (ListTrans i o)
instance Applicative (ListTrans i o)
instance Monad (ListTrans i o) where
    return a = Main.LT (\i -> (i, [], a))
    (>>=) lt f = Main.LT (\i0 ->let (i1, o1, a) = unLT lt i0
                                in let (i2, o2, a2) = (unLT (f a)) i1
                                in (i2, o1 ++ o2, a2) )



instance StreamTrans (ListTrans i o) i o where
    readS = undefined
    emitS = undefined



-- zad 4
class Monad m => Random m where
   random :: m Int
 
 
newtype RS t = RS {unRS :: Int -> (Int, t)}
 
withSeed :: RS a -> Int -> a
withSeed (RS unRS) n = snd (unRS n)
 
instance Functor RS
instance Applicative RS
 
instance Monad RS where
  return x = RS (\seed -> (seed, x))
  (>>=) r f = RS (\seed -> let (new_seed, a) = unRS r seed
                      in unRS (f a) new_seed)
 
 
instance Random RS where
  random = RS (\s ->
    let b = 16807 * (mod s  127773) - 2836 * (div s 127773) in
    let a = if b > 0 then b else b + 2147483647
    in (a, b))
