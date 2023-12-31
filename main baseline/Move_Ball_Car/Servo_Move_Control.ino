void Servo_Init() {
  for (int i = 0; i < servo_num; i++) {
    myServo.attach(servo_port);
    myServo.write(map(value_init, 0, 180, 500, 2500));
    myServo2.attach(servo_port2);
    myServo2.write(map(value_init2, 0, 180, 500, 2500));
    delay(20);
  }
}

void ServoStop() {  //释放舵机
  myServo.detach();
  digitalWrite(servo_port, LOW);
}

void ServoGo(float where) {  //打开并给舵机写入相关角度
  if (where != 200) {
    if (where == 201) ServoStop();
    else {
      myServo.write(map(where, 0, 180, 500, 2500));
    }
  }
}

void Servo_Move_Single(int Start_angle, int End_angle, unsigned long Servo_move_time) {
  int servo_flags = 0;
  int delta_servo_angle = abs(Start_angle - End_angle);
  if ((Start_angle - End_angle) < 0) {
    servo_flags = 1;
  } else {
    servo_flags = -1;
  }
  for (int i = 0; i < delta_servo_angle; i++) {
    myServo.write(map(Start_angle + (servo_flags * i), 0, 180, 500, 2500));
    delay(Servo_move_time);
  }
}

void Qingdao() {

  for (int i = value_init; i <= f_value_init; i++) {
    myServo.write(map(i, 0, 180, 500, 2500));
    delay(10);
  }
  delay(1000);
  myServo2.write(map(f_value_init2, 0, 180, 500, 2500));
  delay(1000);
  myServo2.write(map(value_init2, 0, 180, 500, 2500));
  delay(1000);
  myServo.write(map(75, 0, 180, 500, 2500));
  delay(1000);
}
