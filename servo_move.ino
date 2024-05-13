#include <SCServo.h>
SMS_STS servo;  //635~3484

#define l2 150 //unit: mm
#define l3 100
#define servo_v 3000
#define servo_a 50
float current_deg[2] = {90, 0};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(1000000);
  servo.pSerial = &Serial1;
  servo.WritePosEx(9, 2047, 2400, 50);              // 위치 초기화
  servo.WritePosEx(11, 2047, 2400, 50);
  delay(1000);

}

void loop() {


}

void servo_move2(int x, int y) {
  bool theta1 = 1, theta2 = 1;                                                          // 구해진 2개의 값중 사용할수 있는 값만 1
  float target_deg[2] = {current_deg[0], current_deg[1]}, deg[2][2];                    // 구해진 값들중 사용할 수 있는값이 없을때 현재위치에서 움직이지 않기 위해 현재위치로 초기화

  float d = sqrt(pow(x, 2) + pow(y, 2));
  float D = (pow(d, 2) - pow(l2, 2) - pow(l3, 2)) / (2.0 * l2 * l3);

  deg[1][0] = atan2(sqrt(1 - pow(D, 2)) , D);                                                          // theta2_1
  deg[1][1] = atan2(-sqrt(1 - pow(D, 2)) , D);                                                         // theta2_2
  deg[0][0] = atan2(y , x) - atan2(l3 * sin(deg[1][0]) , l2 + l3 * cos(deg[1][0]));                    // theta1_1
  deg[0][1] = atan2(y , x) - atan2(l3 * sin(deg[1][1]) , l2 + l3 * cos(deg[1][1]));                    // theta1_2
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      deg[i][j] *= (180 / PI);

  if (deg[0][0] < 0 || 180 < deg[0][0] || -90 > deg[1][0] || 90 < deg[1][0]) theta1 = 0; // 사용할수 있는 값인지 판별
  if (deg[0][1] < 0 || 180 < deg[0][1] || -90 > deg[1][1] || 90 < deg[1][1]) theta2 = 0;

  if (theta1) {
    target_deg[0] = deg[0][0];
    target_deg[1] = deg[1][0];
  }
  if (theta2) {
    target_deg[0] = deg[0][1];
    target_deg[1] = deg[1][1];
  }
  if (theta1 && theta2) {
    if ( abs(current_deg[0] - deg[0][0]) + abs(current_deg[1] - deg[1][0]) < abs(current_deg[0] - deg[0][1]) + abs(current_deg[1] - deg[1][1])) { // 둘다 사용할 수 있을때 움직여야하는 범위가 더 작은 값을 선택
      target_deg[0] = deg[0][0];
      target_deg[1] = deg[1][0];
    }
    else {
      target_deg[0] = deg[0][1];
      target_deg[1] = deg[1][1];
    }
  }

  int current_deg_pwm[2] = {map (current_deg[0], 0, 180, 3072, 1024), map (current_deg[1], -90, 90, 3072, 1024)};
  int target_deg_pwm[2] = {map (target_deg[0], 0, 180, 3072, 1024), map (target_deg[1], -90, 90, 3072, 1024)};
  int delta_pwm[2] = {current_deg_pwm[0] - target_deg_pwm[0], current_deg_pwm[1] - target_deg_pwm[1]};

  if (delta_pwm[0] > delta_pwm[1]) {
    float tmp = delta_pwm[1] / delta_pwm[0];
    servo.WritePosEx(9, target_deg_pwm[0], servo_v, servo_a);
    servo.WritePosEx(11, target_deg_pwm[1], servo_v * tmp, servo_a * tmp);
  } else {
    float tmp = delta_pwm[0] / delta_pwm[1];
    servo.WritePosEx(9, target_deg_pwm[0], servo_v * tmp, servo_a * tmp);
    servo.WritePosEx(11, target_deg_pwm[1], servo_v, servo_a);
  }
  delay(1000);

  current_deg[0] = target_deg[0];                  // 현재 위치 업데이트
  current_deg[1] = target_deg[1];

}
