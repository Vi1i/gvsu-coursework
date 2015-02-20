-- Function 1, GCD
my_gcd :: (Integral a) => a -> a -> a
my_gcd 0 0 = error "gcd 0 0 is undefined"
my_gcd x y = my_gcd' (abs x) (abs y) where
	my_gcd' a 0 = a
	my_gcd' a b = my_gcd' b (a `rem` b)

-- Function 2, PALINDROME
is_palindrome x = x == reverse x


-- Function 3, INTLIST
intlist x = [x,x-1..1]

-- Function 4, ANALYZE
-- Function 5, ONLY-ATOMS-R
-- Function 6, ONLY-ATOMS-I

-- These last three question were not possible to be done in Haskell,
-- the functions were made for lisp, a similiar programming language.
-- Now, in these three questions, they were supposed to tell you when
-- a 'variable' was an atom or list.  Haskell uses lists, but does not
-- define otherthings as atoms.  Another problem is that Haskell has
-- only homogeneous structures, so a list of [1, ["b"], "b", 'b'] will
-- fail before runtime.  Below, the commented out code shows a 'way'
-- to get around this, though there would need to be a mapping element
-- and folding to fully set this up.  The problem with that is, it
-- isn't Haskell.  The next problems are that 'variables' aren't in
-- Haskell.  Patterns in the scope of the definition are.

-- Function 7, QUAD-ROOTS
quad_roots a b c = [((-b + sqrt(b^2 - (4 * a * c))) / (2 * a)), ((-b - sqrt(b^2 - (4 * a * c))) / (2 * a))]

-- Function 8, ROTATE-FROM-LEFT
rotate_from_left l 0 = l
rotate_from_left l z = rotate_from_left((last l):(init l)) (z - 1)

-- Function 9, ODDS-R
odds_r l =	if		(length l) == 0
			then	["Nil"]
			else	(odds_r_helper l 1)

-- Function 9.1, ODDS-R-HELPER
odds_r_helper l z =	if		(z + 1) > length l
					then	l
					else	odds_r_helper ((take z l)++(drop (z + 1) l)) (z + 1)	

-- Function 10, ODDS-I

-- This was also another problem, there are no iteration constructs in
-- Haskell.  Everything is a recurse.  Even the code that you could
-- use, is just a a very efficient tail recursion.  So to do this in
-- Haskell would be to go against what Haskell stands for.


-- This is so that it is possible to have a heterogeneus list
--data Obj = forall a. (Show a) => Obj a
--
--xs :: [Obj]
--xs = [Obj 1, Obj "foo", Obj 'C']
--
--doShow :: [Obj] -> String
--doShow [] = ""
--doShow ((Obj x):xs = show x ++ doShow xs
