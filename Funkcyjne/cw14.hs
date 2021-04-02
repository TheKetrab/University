

-- rozw nie zachowuje priorytetów działań!


import Data.Char
import Data.List

infix 6 :*, :/
infix 5 :+, :-
infix 4 :=

data Cryptarithm = Expression := Expression
                    deriving (Show)

data Expression = C String
    | Expression :+ Expression
    | Expression :- Expression
    | Expression :* Expression
    | Expression :/ Expression

instance Show Expression where
    show (C s) = "C" ++ show s
    show (e1 :+ e2) = "(" ++ show e1 ++ " :+ " ++ show e2 ++ ")"
    show (e1 :- e2) = "(" ++ show e1 ++ " :- " ++ show e2 ++ ")"
    show (e1 :* e2) = "(" ++ show e1 ++ " :* " ++ show e2 ++ ")"
    show (e1 :/ e2) = "(" ++ show e1 ++ " :/ " ++ show e2 ++ ")"

isBigLetter :: Char -> Bool
isBigLetter c = (ord c >= ord 'A' && ord c <= ord 'Z')

isWhiteChar :: Char -> Bool
isWhiteChar c =
    case c of
        ' ' -> True
        '\t' -> True
        '\n' -> True
        otherwise -> False

isOperator :: Char -> Bool
isOperator c =
    case c of
        '+' -> True
        '-' -> True
        '*' -> True
        '/' -> True
        otherwise -> False

isEqualChar :: Char -> Bool
isEqualChar c = c == '='


-- zwraca stringa od znaku c (bez c) oraz indeks tego znaku
getRestOfString :: String -> Char -> (String, Int)
getRestOfString = getRestOfString' 0 where
    getRestOfString' :: Int -> String -> Char -> (String, Int)
    getRestOfString' _ [] c = error ("not found char " ++ [c])
    getRestOfString' n (x:xs) c | x==c = (xs,n+1)
    getRestOfString' n (x:xs) c = getRestOfString' (n+1) xs c 

parse :: String -> Either String Cryptarithm
parse str =
    let (correct,left,right,info) = parse' in
    if (correct) then Right $ left := right
    else error "xyz"
        where
            parse' :: (Bool, Expression, Expression, String)
            parse' = do {
                    let e1 = getFstExpr str 0 "" 0 in
                    let (str',index) = getRestOfString str '=' in
                    let e2 = getSndExpr str' 0 "" index in
                        (True,e1,e2,"")
            }
                where
                    -- mode: 0 - none, 1 - var, 2 - finished var, 3 - op            // wyr i komunikat o bledzie
                    getFstExpr :: String -> Int -> String -> Int -> Expression
                    getFstExpr [] _ _ _ = error "error -> not eq sign"
                    getFstExpr (c:cs) mode var i =

                        if (isBigLetter c) then
                            case mode of
                                0 -> getFstExpr cs 1 [c] (i+1)
                                1 -> getFstExpr cs 1 (c:var) (i+1)
                                2 -> error ("TODO error next variable without operator at char " ++ show i)
                                3 -> getFstExpr cs  1 [c] (i+1)

                        else if (isOperator c) then
                            case mode of
                                0 -> error ("TODO operator cannot be at the begining, at char " ++ show i)

                                1 -> 
                                    let v = reverse var in
                                    if (c == '+') then (C v :+ (getFstExpr cs 3 "" (i+1)))
                                    else if (c == '-') then (C v :- (getFstExpr cs 3 "" (i+1)))
                                    else if (c == '*') then (C v :* (getFstExpr cs 3 "" (i+1)))
                                    else if (c == '/') then (C v :/ (getFstExpr cs 3 "" (i+1)))
                                    else error "unknown operator"

                                2 -> 
                                    let v = reverse var in
                                    if (c == '+') then (C v :+ (getFstExpr cs 3 "" (i+1)))
                                    else if (c == '-') then (C v :- (getFstExpr cs 3 "" (i+1)))
                                    else if (c == '*') then (C v :* (getFstExpr cs 3 "" (i+1)))
                                    else if (c == '/') then (C v :/ (getFstExpr cs 3 "" (i+1)))
                                    else error "unknown operator"
    
                                3 -> error ("operator obok operatora at char " ++ show i)                            

                        else if (isWhiteChar c) then
                            case mode of
                                0 -> getFstExpr cs 0 "" (i+1)
                                1 -> getFstExpr cs 2 var (i+1)
                                2 -> getFstExpr cs 2 var (i+1)
                                3 -> getFstExpr cs 3 "" (i+1)


                        else if (isEqualChar c) then
                            let v = reverse var in
                            case mode of
                                0 -> error ("xyz at char " ++ show i)
                                1 -> C v
                                2 -> C v
                                3 -> error ("'=' after operator at char " ++ show i)

                        else
                            error "Unknown character"
                
                    -- mode: 0 - none, 1 - var, 2 - finished var, 3 - op            // wyr i komunikat o bledzie
                    getSndExpr :: String -> Int -> String -> Int -> Expression
                    getSndExpr [] mode var i = 
                        case mode of
                            0 -> error ("empty expression at char " ++ show i)
                            1 -> let v = reverse var in C v
                            2 -> let v = reverse var in C v
                            3 -> error ("operator is on the end of expression at char " ++ show i)
                    getSndExpr (c:cs) mode var i =

                        if (isBigLetter c) then
                            case mode of
                                0 -> getSndExpr cs 1 [c] (i+1)
                                1 -> getSndExpr cs 1 (c:var) (i+1)
                                2 -> error ("TODO error next variable without operator at char " ++ show i)
                                3 -> getSndExpr cs  1 [c] (i+1)

                        else if (isOperator c) then
                            case mode of
                                0 -> error ("TODO operator cannot be at the begining, at char " ++ show i)

                                1 -> 
                                    let v = reverse var in
                                    if (c == '+') then (C v :+ (getSndExpr cs 3 "" (i+1)))
                                    else if (c == '-') then (C v :- (getSndExpr cs 3 "" (i+1)))
                                    else if (c == '*') then (C v :* (getSndExpr cs 3 "" (i+1)))
                                    else if (c == '/') then (C v :/ (getSndExpr cs 3 "" (i+1)))
                                    else error "unknown operator"

                                2 -> 
                                    let v = reverse var in
                                    if (c == '+') then (C v :+ (getSndExpr cs 3 "" (i+1)))
                                    else if (c == '-') then (C v :- (getSndExpr cs 3 "" (i+1)))
                                    else if (c == '*') then (C v :* (getSndExpr cs 3 "" (i+1)))
                                    else if (c == '/') then (C v :/ (getSndExpr cs 3 "" (i+1)))
                                    else error "unknown operator"
    
                                3 -> error ("operator obok operatora at char " ++ show i)                            

                        else if (isWhiteChar c) then
                            case mode of
                                0 -> getSndExpr cs 0 "" (i+1)
                                1 -> getSndExpr cs 2 var (i+1)
                                2 -> getSndExpr cs 2 var (i+1)
                                3 -> getSndExpr cs 3 "" (i+1)

                        else if (isEqualChar c) then
                            error ("found sign '=' twice! at char " ++ show i)

                        else
                            error "Unknown character"










-- parse "SEND + MORE + MORE = MANEY"

