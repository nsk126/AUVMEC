int force_to_pwm_cw(float force){
  if(force < 0){
    return 1464 + 169.2 * force + 34.8 * pow(force,2) + 4.506 * pow(force,3); 
  }else if(force > 0){
    return 1536 + 134.7 * force - 22.07 * pow(force,2) + 2.183 * pow(force,3);
  }else{
    return 1500;//done
  }
}
int force_to_pwm_ccw(float force){
  if (force < 0){  
    return 1536 - 169.2 * force - 34.8 * pow(force,2) - 4.506 * pow(force,3);
  }else if(force > 0){
    return 1464 - 134.7 * force + 22.07 * pow(force,2) + -2.183 * pow(force,3);
  }else{
    return 1500;
  }
}
void Surge(float force,int duration){
  // Total force = 2 * sqrt(2) * F per thruster
  // 0 is T1 1 is T2 are CCW
  PWM[0] = PWM[1] = force_to_pwm_ccw(force/(2.828));
  // 2 is T3 3 is T4 are CW
  PWM[2] = PWM[3] = force_to_pwm_cw(force/(2.828));
  ESC_write();
  delay(duration);
}
void Sway(float force,int duration){
  // Total force = 2 * sqrt(2) * F per thruster
  PWM[0] = force_to_pwm_ccw(-1 * force / (2.828));
  PWM[1] = force_to_pwm_ccw(force / 2.828);
  PWM[2] = force_to_pwm_cw(force / (2.828));
  PWM[3] = force_to_pwm_cw(-1 * force / 2.828);  
  ESC_write();
  delay(duration);
}
void Heave(float force,int duration){  
  PWM[4] = force_to_pwm_cw(-force/2);// T5
  PWM[5] = force_to_pwm_ccw(force/2);// T6  
  ESC_write();
  delay(duration);
}
void Yaw(float force,int duration){
  PWM[0] = force_to_pwm_ccw(force/4); 
  PWM[1] = force_to_pwm_ccw(-force/4);
  PWM[2] = force_to_pwm_cw(force/4);
  PWM[3] = force_to_pwm_cw(-force/4);
  ESC_write();
  delay(duration);
}
void Stall(int duration){
  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500;
  ESC_write();
  delay(duration);
}
void Hstall(int duration){
  PWM[0] = PWM[1] = PWM[2] = PWM[3] = 1500;
  ESC_write();
  delay(duration);
}
void ESC_write(){
  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
}
void PWM_print(){
  Serial.print(PWM[0]);Serial.print("  ");
  Serial.print(PWM[1]);Serial.print("  ");
  Serial.print(PWM[2]);Serial.print("  ");
  Serial.print(PWM[3]);Serial.print("  ");
  Serial.print(PWM[4]);Serial.print("  ");
  Serial.print(PWM[5]);Serial.println();
}
