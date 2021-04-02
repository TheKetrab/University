-- PF2018 Lista 11 Zadanie 1
{-# LANGUAGE GADTs, MultiParamTypeClasses, FlexibleInstances #-}

data Expr a where
  C :: a -> Expr a
  P :: (Expr a, Expr b) -> Expr (a,b)
  Not :: Expr Bool -> Expr Bool
  (:+), (:-), (:*) :: Expr Integer -> Expr Integer -> Expr Integer
  (:/) :: Expr Integer -> Expr Integer -> Expr (Integer,Integer)
  (:<), (:>), (:<=), (:>=), (:!=), (:==) :: Expr Integer -> Expr Integer -> Expr Bool
  (:&&), (:||) :: Expr Bool -> Expr Bool -> Expr Bool
  (:?) :: Expr Bool -> Expr a -> Expr a -> Expr a
  Fst :: Expr (a,b) -> Expr a
  Snd :: Expr (a,b) -> Expr b

infixl 6 :*, :/
infixl 5 :+, :-
infixl 4 :<, :>, :<=, :>=, :!=, :==
infixl 3 :&&
infixl 2 :||
infixl 1 :?
  
-- EVAL
eval :: Expr a -> a
eval (C x) = x
eval (P (e1,e2)) = (eval e1, eval e2)
eval (Not e) = not (eval e)
eval ((:+) e1 e2) = (eval e1) + (eval e2)
eval ((:-) e1 e2) = (eval e1) - (eval e2)
eval ((:*) e1 e2) = (eval e1) * (eval e2)

eval ((:/) e1 e2) =
  let res1 = (eval e1) in
  let res2 = (eval e2) in
  (res1 `div` res2,res1 `mod` res2)

eval ((:<) e1 e2) = (eval e1) < (eval e2)
eval ((:>) e1 e2) = (eval e1) > (eval e2)
eval ((:<=) e1 e2) = (eval e1) <= (eval e2)
eval ((:>=) e1 e2) = (eval e1) >= (eval e2)
eval ((:!=) e1 e2) = (eval e1) /= (eval e2)
eval ((:==) e1 e2) = (eval e1) == (eval e2)
eval ((:&&) e1 e2) = (eval e1) && (eval e2)
eval ((:||) e1 e2) = (eval e1) || (eval e2)
eval ((:?) e1 e2 e3) = if (eval e1) then (eval e2) else (eval e3)
eval (Fst e) = fst (eval e)
eval (Snd e) = snd (eval e)

-- test
-- eval (C 2 :* C 7 :> Fst (C 6 :/ C 3) :&& C 3 :< C 5 :? Snd (C 15 :/ C 12) :+ C 3 $ C 7 :* C 8)
-- -> 6




-- PF2018 Lista 11 Zadanie 4
{-# LANGUAGE Rank2Types, MultiParamTypeClasses, FlexibleInstances #-} 

newtype Church = Ch (forall a . (a->a)->(a->a))


----- ----- nums ----- -----
----- ----- ----- ----- -----
zeroCh :: Church
zeroCh = Ch (flip const)

oneCh :: Church
oneCh = Ch id

----- ----- succ, prev, is zero ----- -----
----- ----- ----- ----- -----
succCh :: Church->Church
succCh (Ch a) = Ch (\f -> a f . f)


prevCh (Ch n) =
  if (toInt (Ch n) == 0) then zeroCh -- jak inaczej?
  else Ch (\s z -> fst (n (\(_,x)->(x,s x)) (undefined,z)))

prevCh' (Ch n) =
  if (toInt (Ch n) == 0) then zeroCh -- jak inaczej?
  else fst (n (\(_,x)->(x,succCh x)) (undefined,zeroCh))

iszero (Ch n) = n (\z -> False) True

----- ----- +, -, *, ^ ----- -----
----- ----- ----- ----- -----
addCh' (Ch n) m = n succCh m
addCh (Ch n) (Ch m) = Ch (\s -> n s . m s)

subCh n (Ch m) = m prevCh n
subCh' n (Ch m) = m prevCh' n

mulCh (Ch n) (Ch m) = Ch (n . m)
mulCh' (Ch n) m = n (addCh m) zeroCh

powCh (Ch n) (Ch m) = Ch (m n)


----- ----- helper ----- -----
----- ----- ----- ----- -----
toInt :: Church -> Integer
toInt (Ch n) = n (1 +) 0

toChurch :: Integer -> Church
toChurch n = toChurch' n zeroCh where
  toChurch' :: Integer -> Church -> Church
  toChurch' 0 ch = ch
  toChurch' n ch = succCh (toChurch' (n-1) ch)

printChurch :: Church -> [Char]
printChurch ch = "<" ++ (printChurch' (toInt ch)) ++ ">" where
  printChurch' :: Integer -> [Char]
  printChurch' 0 = ""
  printChurch' n = "O"++(printChurch' (n-1))
  

----- ----- instances ----- -----
----- ----- ----- ----- -----
instance Num Church where
  x + y = addCh x y
  x * y = mulCh x y
  x - y = subCh x y -- n-m = 0 jesli n<=m -> o to zadba prevCh
  signum zeroCh = 0
  abs x = x
  fromInteger = toChurch
  negate = undefined
  
instance Show Church where
  show = printChurch  

instance Eq Church where
  x == y =
    case (iszero x, iszero y) of
      (True,True) -> True
      (False,False) -> (prevCh x) == (prevCh y)
      _ -> False

instance Ord Church where
  compare x y =
    if x == y then EQ
    else if not (iszero (x - y)) then GT
    else LT
