# AUVMEC
## Thruster Orientation for CW and CCW propellors
### Green: CCW , Blue: CW

![](https://www.ardusub.com/images/vectored-frame.png)

## Curve Fitting equations for T200s
*Note: All forces are in Kgf*

#### Curve Fit - PWM(f) @12V
- Negative Half
`PWM(f) = 7.359 * (f^3) + 47.59 * (f^2) + 199.2 * (f) + 1461;`
- Positive Half
`PWM(f) = 3.771 * (f^3) - 31.19 * (f^2) + 159.9 * (f) + 1540;`
- Dead band
`1464 - 1532 => 0 Kgf Force`

#### Surface fit - PWM(f,V)
- Negative Half
`  PWM(f,V) = 1137 + 192.3*f + 39.66*V + 10.32*(f^2)  - 3.917*f*V - 1.189 *(V^2) `      
    - *R-square: 0.9475*
     ![SurfaceFit](images/negHalf.png)
- Positive Half
`  PWM(f,V) = 1682 + 301.9*f - 31.3*V - 93.2*(f^2) + 9.779*f*V + 2.132*(V^2) + 37.13*(f^3) - 4.207*(f^2)*V - 2.7*f*(V^2) - 0.04853*(V^3) - 1.475*(f^4) - 3.012*(f^3)*V + 1.006*(f^2)*(V^2) + 0.09237*f*(V^3) + 0.1227*(f^5) - 0.03288*(f^4)*V + 0.09417*(f^3)*(V^2) - 0.03484*(f^2)*(V^3) `
    - *R-square: 0.9986*
     ![SurfaceFit](images/posHalf.png)
