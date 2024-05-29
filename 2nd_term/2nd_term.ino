#include <AIROARM.h>
#include "SoftwareSerial.h"
#include "HUSKYLENS.h"
#include <Servo.h>
#include "wire.h"

#define button 3
#define green 5
#define red 4
#define height 180

HUSKYLENS huskylens;
AiroArm arm;
Servo myservo3;
Servo myservo4;

typedef struct coordinate {
  float x;
  float y;
  float z;
  int w;
  int slope;
};

bool flag = 0, flag1 = 0, flag2 = 0;
float clue_deg;
float final_deg;

coordinate ids[13] = {};
coordinate mids[7] = {};
coordinate targets[7] = {{},
  {1329, 2166, 50, 90, 43},
  {1950, 924, 10, 107, 178},
  {1673, 1885, 10, 143, 46 },
  {1612, 1109, 10, 90, 160},
  {1805, 810, 10, 90, 150},
  {1988, 1274, 10, 115, 115}
};
coordinate target_coord = {};

void scan_tags();
void move2center(int block);
void grip_and_drop(int block);
coordinate find_abs_coord(float target_x, float target_y);
void rotate_block(int block);

void setup() {
  Serial.begin(115200);
  Serial1.begin(1000000);
  pinMode(button, INPUT_PULLUP);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  //I2C 통신 초기화
  Wire.begin();
  huskylens.begin(Wire);
  arm.offset.m1_deg = 0; //기존 모터 세팅 값 보정
  arm.offset.m2_deg = 0; //기존 모터 세팅 값 보정
  arm.pSerial = &Serial1;

  // GREEN_LED ON!
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);

  

  delay(500);
  arm.WritePosEx(1, 2047, 1000, 50);
  arm.WritePosEx(2, 2047, 1000, 50);
  delay(500);
  arm._init(); // 로봇팔 초기화
  delay(500);
  arm.movep(0, 285, height);

  //서보 초기화
  myservo3.attach(11);
  myservo4.attach(10);
  myservo3.write(100);
  myservo4.write(25);
  delay(500);
}

void loop() {
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  //버튼 누를 때까지 대기
  while (digitalRead(button));

  // RED_LED ON!
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);

  // ids 배열에 태그들의 절대위치 저장
  scan_tags();

  // 블록별 반복
  for (int i = 1; i <= 6; i++) {
    if (ids[i * 2 - 1].x != 0 && ids[i * 2].x != 0) {

      // 중심위치로 이동
      arm.movep(mids[i].x, mids[i].y);

      // 카메라 중심으로 3번 이동
      for (int j = 1; j <= 3; j++) move2center(i);

      // 슬로프 확인
      int x_slope[13], y_slope[13];
      delay(100);
      huskylens.request();
      delay(100);
      if (huskylens.isLearned()) {
        while (huskylens.available()) {
          HUSKYLENSResult result = huskylens.read();
          x_slope[result.ID] = 320 - result.xCenter;
          y_slope[result.ID] = 240 - result.yCenter;

          ids[result.ID] = find_abs_coord(result.xCenter, result.yCenter);
        }
      }
      float slope = atan2(x_slope[i * 2] - x_slope[i * 2 - 1], y_slope[i * 2] - y_slope[i * 2 - 1]) * (180 / PI);
      Serial.print(slope);
      
      // 그리퍼 중심으로 이동
      float EE_radian = radians(arm.pul2deg(arm.ReadPos(1)) + arm.pul2deg(arm.ReadPos(2)));
      arm.movep(arm.EF[0] + 30 * sin(EE_radian), arm.EF[1] + 30 * cos(EE_radian));


      // 슬로프에 맞게 그리퍼 회전
      if (slope < 0) {
        slope += 180;
        flag1 = 1;
      }

      if (i < 4) {
        if (slope < 90) {
          myservo3.write(90 - slope);
        }
        else {
          flag2 = 1;
          myservo3.write(90 + (180 - slope));
        }
      } else {

        myservo3.write(185 - slope);
        if (slope > 90)flag2 = 1;
      }

      delay(500);

      // 내려가서 집고 원하는 위치에 갖다두기
      grip_and_drop(i);


      switch (i) {
        case 1:
          arm.WritePosEx(1, 1507, 1000, 50);
          arm.WritePosEx(2, 2099, 1000, 50);
          myservo3.write(0);
          delay(1000);
          arm.movez(20);
          delay(1000);
          myservo3.write(30);
          delay(500);
          myservo3.write(0);
          delay(500);
          arm.movez(height);
          delay(1000);
          myservo3.write(90);
          break;
        case 2:
          arm.WritePosEx(1, 1960, 1000, 50);
          arm.WritePosEx(2, 1289, 1000, 50);
          delay(500);
          arm.movez(20);
          delay(1000);
          arm.WritePosEx(1, 1960, 1000, 50);
          arm.WritePosEx(2, 1212, 1000, 50);
          delay(500);
          arm.WritePosEx(1, 1960, 1000, 50);
          arm.WritePosEx(2, 1289, 1000, 50);
          delay(500);
          arm.movez(height);
          delay(1000);
          break;
        case 3:
          arm.WritePosEx(1, 1553, 1000, 50);
          arm.WritePosEx(2, 2296, 1000, 50);
          delay(500);
          myservo3.write(30);
          delay(500);
          arm.movez(10);
          delay(1000);
          myservo3.write(60);
          delay(500);
          arm.movez(height);
          delay(1000);
          arm.WritePosEx(1, 1851, 1000, 50);
          arm.WritePosEx(2, 1513, 1000, 50);
          delay(1000);
          arm.movez(10);
          delay(1000);
           myservo3.write(75);
          delay(500);
          myservo3.write(60);
          delay(500);
          arm.movez(height);
          delay(1000);
          myservo3.write(90);
          break;


      }


    }

  }
  arm.WritePosEx(1, 2047, 1000, 50);
  arm.WritePosEx(2, 2047, 1000, 50);
  delay(1000);
  // GREEN_LED ON!
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);

  // 종료
  while (1);
}
