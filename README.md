# AUVMEC
![](https://www.ardusub.com/images/vectored-frame.png)

### Curve Fitting equations for T200s

#### 3rd order Polynomial fit @12V
- Negative Half
`f(x) = 7.359 * (x^3) + 47.59 * (x^2) + 199.2 * (x) + 1461;`
- Positive Half
`f(x) = 3.771 * (x^3) - 31.19 * (x^2) + 159.9 * (x) + 1540;`
- Dead band
`1464 - 1532 => 0 Kgf Force`

#### Surface fit w.r.t Kgf & Volts
- Negative Half
`  f(x,y) = 1137 + 192.3*x + 39.66*y + 10.32*x^2  -3.917*x*y -1.189 *y^2 `      
    - *R-square: 0.9475*
