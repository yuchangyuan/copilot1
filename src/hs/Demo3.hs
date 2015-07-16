module Demo3 where
import Interface
import Language.Copilot

import qualified Prelude as P

import Copilot.Compile.C99

scale = 1000
ledNum = 8
ledNumS = fromIntegral ledNum

accX = externF "acc_x" Nothing
accY = externF "acc_y" Nothing
accZ = externF "acc_z" Nothing

accAbs :: Stream Float
accAbs = sqrt(accX * accX + accY * accY + accZ * accZ)

-- direction vector
dirTbl :: [(Float, Float, Float)]
dirTbl = map (\id ->
       let t = fromIntegral id / fromIntegral ledNum * 2 * pi
           in (cos t, sin t, 0))
        [0 .. fromIntegral ledNum - 1]

ledVal :: Word8 -> Stream Float
ledVal id = mux (proj < 0) 0 proj
       where (dirX, dirY, dirZ) = dirTbl P.!! (fromIntegral id)
             proj = (accX * constF dirX +
                     accY * constF dirY +
                     accZ * constF dirZ) / accAbs * dir

clicked :: Stream Bool
clicked = not ([False] ++ btnStatus) && btnStatus

dir :: Stream Float
dir = [1.0] ++ (mux clicked (-dir) dir)

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
       let grp = map ledVal [0 .. ledNum - 1]
       let val = idx grp idS $ constF 0
       ledPwmSet idS val

main = do
     reify ledSpec >>= compile defaultParams
