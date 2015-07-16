module Interface where

import Language.Copilot
import qualified Prelude as P

ledPwmSet :: Stream Word8 -> Stream Float -> Spec
ledPwmSet id val = trigger "led_pwm_set"
                           (constB True) [arg id, arg val]

btnStatus :: Stream Bool
btnStatus = externFun "btn_status" [] $ Just $ constB False
