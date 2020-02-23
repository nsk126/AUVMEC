# AUVMEC
## Thruster Orientation for CW and CCW propellors
### Green: CW , Blue: CCW

![](https://www.ardusub.com/images/vectored-frame.png)

## Curve Fitting equations for T200s

#### 3rd order Polynomial fit @12V
- Negative Half
`PWM(f) = 7.359 * (f^3) + 47.59 * (f^2) + 199.2 * (f) + 1461;`
- Positive Half
`PWM(f) = 3.771 * (f^3) - 31.19 * (f^2) + 159.9 * (f) + 1540;`
- Dead band
`1464 - 1532 => 0 Kgf Force`

#### Surface fit w.r.t Kgf & Volts
- Negative Half
`  f(x,y) = 1137 + 192.3*x + 39.66*y + 10.32*x^2  -3.917*x*y -1.189 *y^2 `      
    - *R-square: 0.9475*
