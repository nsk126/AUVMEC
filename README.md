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
`      f(x,y) = p00 + p10*x + p01*y + p20*x^2 + p11*x*y + p02*y^2 + p30*x^3 + p21*x^2*y 
                    + p12*x*y^2 + p03*y^3 + p40*x^4 + p31*x^3*y + p22*x^2*y^2 
                    + p13*x*y^3 + p50*x^5 + p41*x^4*y + p32*x^3*y^2 + p23*x^2*y^3 `
                     p00 =        1278  (1277, 1278)
       p10 =       109.7  (108.6, 110.8)
       p01 =       35.57  (34.54, 36.59)
       p20 =       15.03  (13.4, 16.67)
       p11 =      -15.48  (-17.09, -13.86)
       p02 =      -15.13  (-15.61, -14.66)
       p30 =       5.851  (4.72, 6.983)
       p21 =       -3.23  (-4.963, -1.498)
       p12 =       6.245  (5.321, 7.17)
       p03 =      0.3979  (-0.1449, 0.9407)
       p40 =       2.991  (2.149, 3.834)
       p31 =      -3.445  (-4.517, -2.373)
       p22 =       0.205  (-0.844, 1.254)
       p13 =      -1.823  (-2.566, -1.079)
       p50 =       1.233  (0.8957, 1.569)
       p41 =       1.296  (0.47, 2.122)
       p32 =       2.783  (1.66, 3.906)
       p23 =       1.489  (0.663, 2.314)
       
        R-square: 0.9993
        
        
    - *R-square: 0.9475*
     ![SurfaceFit](images/negHalf.png)
- Positive Half
`  PWM(f,V) = 1682 + 301.9*f - 31.3*V - 93.2*(f^2) + 9.779*f*V + 2.132*(V^2) + 37.13*(f^3) - 4.207*(f^2)*V - 2.7*f*(V^2) - 0.04853*(V^3) - 1.475*(f^4) - 3.012*(f^3)*V + 1.006*(f^2)*(V^2) + 0.09237*f*(V^3) + 0.1227*(f^5) - 0.03288*(f^4)*V + 0.09417*(f^3)*(V^2) - 0.03484*(f^2)*(V^3) `
    - *R-square: 0.9986*
     ![SurfaceFit](images/posHalf.png)
