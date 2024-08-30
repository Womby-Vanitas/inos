int RightMotor_E_pin = 5;                                        // 오른쪽 모터의 Enable & PWM
int LeftMotor_E_pin = 6;                                         // 왼쪽 모터의 Enable & PWM
int RightMotor_1_pin = 8;                                        // 오른쪽 모터 제어선 IN1
int RightMotor_2_pin = 9;                                        // 오른쪽 모터 제어선 IN2
int LeftMotor_3_pin = 10;                                        // 왼쪽 모터 제어선 IN3
int LeftMotor_4_pin = 11;                                        // 왼쪽 모터 제어선 IN4

int L_Line = A5;                                                 // 왼쪽 라인트레이서 센서는 A5 핀에 연결
int C_Line = A4;                                                 // 가운데 라인트레이서 센서는 A4 핀에 연결
int R_Line = A3;                                                 // 오른쪽 라인트레이서 센서는 A3 핀에 연결

int L_MotorSpeed = 130;                                          // 왼쪽 모터 속도
int R_MotorSpeed = 130;                                          // 오른쪽 모터 속도

int trigPin = 13;
int echoPin = 12;

int SL = 1;
int SC = 1;
int SR = 1;

void setup() {
  pinMode(RightMotor_E_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);

  pinMode(echoPin, INPUT);                            // echoPin 입력
  pinMode(trigPin, OUTPUT);                           // trigPin 출력

  Serial.begin(9600);                                             // PC와의 시리얼 통신 9600bps로 설정
  Serial.println("Welcome Eduino!");
}

void loop() {
  int L = digitalRead(L_Line);
  int C = digitalRead(C_Line);
  int R = digitalRead(R_Line);

  long duration, distance;
  digitalWrite(trigPin, HIGH);                        // trigPin에서 초음파 발생(echoPin도 HIGH)
  //delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);                  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 10000) / 2; 
  Serial.print("distance:");                          // 물체와 초음파 센서간 거리를 표시
  Serial.print(distance);

  Serial.print("digital : "); 
  Serial.print(L); 
  Serial.print(", "); 
  Serial.print(C); 
  Serial.print(", "); 
  Serial.print(R); 
  Serial.print("   ");

  // 장애물 인식을 최우선으로 처리
  if ( distance < 12 ) {                              
    analogWrite(RightMotor_E_pin, 0);
    analogWrite(LeftMotor_E_pin, 0);
    Serial.println("장애물 인식, 정지");
  }
  else if ( distance > 11 && L == LOW && C == LOW && R == LOW ) {           // 0 0 0
    L = SL; C = SC; R = SR;
  }
  else if ( L == 0 && C == 1 && R == 0 ) {          // 0 1 0
    motor_role(HIGH, HIGH, 120, 120);
    Serial.println("직진");
  }
  else if (L == 1 && R == 0 ){                   // 0 0 1, 0 1 1
    motor_role(LOW, HIGH, 140, 140);
    Serial.println("우회전");
  }
  else if (L == 0 && R == 1 ) {                  // 1 0 0, 1 1 0
    motor_role(HIGH, LOW, 140, 140);
    Serial.println("좌회전");
  }
  else if ( (L == 1 && C == 1 && R == 1) ) {                // 1 1 1, 1 0 1
    analogWrite(RightMotor_E_pin, 0);
    analogWrite(LeftMotor_E_pin, 0);
    Serial.println("정지");
  }
  
  SL = L; SC = C; SR = R;
}


void motor_role(int R_motor, int L_motor, int a, int b) {
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, a);  // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, b);   // 좌측 모터 속도값
}
