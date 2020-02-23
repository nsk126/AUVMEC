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
`  PWM(f,V) = 1137 + 192.3*f + 39.66*V + 10.32*(f^2)  - 3.917*f*V - 1.189 *(V^2) `      
    - *R-square: 0.9475*
    - ![SurfaceFit](negHalf.png)
