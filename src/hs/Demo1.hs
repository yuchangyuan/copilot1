module Demo1 where
import Interface
import Language.Copilot

import qualified Prelude as P

import Copilot.Compile.C99

ledNum :: Int
ledNum = 8

-- see Language.Copilot.!!
idx :: (Typed a, Typed b, Integral b) =>
       [Stream a] -> Stream b -> Stream a -> Stream a
idx group sel def =
  let tbl = map (constant . fromIntegral) [0 .. P.length group - 1]
      f (x:xs) (y:ys) = mux (x == sel) y (f xs ys)
      f [] _ = def
      f _ [] = def
  in f tbl group

ledSpec = do
  let clicked = not ([False] ++ btnStatus) && btnStatus
  let idxNow = [0 .. fromIntegral ledNum-1] ++ idxNow :: Stream Word8
  let off = [0] ++ (mux clicked (off + 1) off)
  let valList = map constF $ map (/ 8.0) [1 .. fromIntegral ledNum]
  let valNow = idx valList  ((idxNow + off) `mod` 8) $ constF 0
  ledPwmSet idxNow valNow

main = do
  reify ledSpec >>= compile defaultParams
