module Demo2 where
import Interface
import Language.Copilot

import qualified Prelude as P

import Copilot.Compile.C99

scale = 1000
ledNum = 8
ledNumS = fromIntegral ledNum
{-
 bump: 当为True时，点亮LED，否则每步LED亮度按decay衰减
 decay: 每步LED衰减的亮度
 返回LED的亮度
-}
ledVal :: Stream Bool -> Stream Float -> Stream Float
ledVal bump decay =
       let r1 = ([0.5] ++ r0) - decay -- r0 in last step
           r0 = case' [bump, r1 < 0] [1.0, 0.0, r1]
       in r0

ledSeq :: Stream Int32
ledSeq = r0 `div` scale where
       r1 = ([0] ++ r0) + speed
       r0 = (r1 + ledNumS * scale) `mod` (ledNumS * scale)

{-
 id: LED的编号，使得不同的LED能在不同的时候点亮
-}
ledBump :: Word8 -> Stream Bool
ledBump id = (seq' /= seq) && (seq == (constI32 $ fromIntegral id))
        where
        seq = ledSeq
        seq' = [0] ++ ledSeq

--- scaled by 1000
speed0 = 4
decay0 = 8e-4

speed :: Stream Int32
speed = mux clicked (-speed') speed'
      where
       speed' = [speed0] ++ speed

clicked :: Stream Bool
clicked = not ([False] ++ btnStatus) && btnStatus

-- see (!!) of Language.Copilot.!!
idx :: (Typed a, Typed b, Integral b) =>
    [Stream a] -> Stream b -> Stream a -> Stream a
idx group sel def =
    let tbl = map (constant . fromIntegral) [0 .. P.length group - 1]
        f (x:xs) (y:ys) = mux (x == sel) y (f xs ys)
        f [] _ = def
        f _ [] = def
    in f tbl group

idS :: Stream Word8
idS = [0 .. ledNum - 1] ++ idS

ledSpec = do
        let genLedDrv id = ledVal (ledBump id) decay0
        let grp = map genLedDrv [0 .. ledNum - 1]
        let val = idx grp idS $ constF 0
        ledPwmSet idS val

main = do
     reify ledSpec >>= compile defaultParams
