
void Robot_Forward() {
  digitalWrite(motor_dir_left, dir_back);
  analogWrite(motor_pwm_left, PWM_Speed);
  digitalWrite(motor_dir_right, dir_back);
  analogWrite(motor_pwm_right, PWM_Speed);
  delay(SamplingTime);
}

void Robot_Backward() {
  digitalWrite(motor_dir_left, dir_fwd);
  analogWrite(motor_pwm_left, PWM_Speed);
  digitalWrite(motor_dir_right, dir_fwd);
  analogWrite(motor_pwm_right, PWM_Speed);
  delay(SamplingTime * 2);
}

void Robot_Stop() {
  digitalWrite(motor_dir_left, dir_back);
  analogWrite(motor_pwm_left, 0);
  digitalWrite(motor_dir_right, dir_fwd);
  analogWrite(motor_pwm_right, 0);
  delay(SamplingTime);
}

void Robot_Left() {
  digitalWrite(motor_dir_left, 0);
  analogWrite(motor_pwm_left, 0);
  digitalWrite(motor_dir_right, 0);
  analogWrite(motor_pwm_right, PWM_Speed);
  delay(SamplingTime);
}

void Robot_Right() {
  digitalWrite(motor_dir_left, 0);
  analogWrite(motor_pwm_left, PWM_Speed);
  digitalWrite(motor_dir_right, 0);
  analogWrite(motor_pwm_right, 0);
  delay(SamplingTime);
}

