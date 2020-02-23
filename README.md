# AUVMEC
## Thruster Orientation for CW and CCW propellors
### Green: CCW , Blue: CW

![Vectored](images/vectored-frame.png)

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
`  PWM(f,V) = 1278 + 109.7*f + 35.57*V + 15.03*(f^2) - 15.48*f*V - 15.13*(V^2) + 5.851*(f^3) - 3.23*(f^2)*V + 6.245*f*(V^2) + 0.3979*(V^3) + 2.991*(f^4) - 3.445*(f^3)*V + 0.205*(f^2)*(V^2) - 1.823*f*(V^3) + 1.233*(f^5) + 1.296*(f^4)*V + 2.783*(f^3)*(V^2) + 1.489*(f^2)*(V^3) `    
    - *R-square: 0.9993*
    ![SurfaceFit](images/negHalf.png)
- Positive Half
`  PWM(f,V) = 1682 + 301.9*f - 31.3*V - 93.2*(f^2) + 9.779*f*V + 2.132*(V^2) + 37.13*(f^3) - 4.207*(f^2)*V - 2.7*f*(V^2) - 0.04853*(V^3) - 1.475*(f^4) - 3.012*(f^3)*V + 1.006*(f^2)*(V^2) + 0.09237*f*(V^3) + 0.1227*(f^5) - 0.03288*(f^4)*V + 0.09417*(f^3)*(V^2) - 0.03484*(f^2)*(V^3) `
    - *R-square: 0.9986*
    ![SurfaceFit](images/posHalf.png)
	
	
## Dev Process
- [ ] Rebuild Kill switch(KS)
- [ ] Full system KS that turns off ESCs, & Resets Arduino.
	- [ ] Power The KS using Main Batteries.
	- [ ] Src -> Buck (5V) -> KS
	- [ ] When KS is removed, a *3.27* output is generated. 
