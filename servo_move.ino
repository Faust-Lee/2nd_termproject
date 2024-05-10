#include <SCServo.h>
SMS_STS servo;  //635~3484

#define l2 150 //unit: mm
#define l3 100

int x = 0, y = 1, z = 2;
float pose[3] = { 60, 120, 90 };
float deg[2][2];
float d, D, theta1, theta2_1, theta2_2;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(1000000);
  servo.pSerial = &Serial1;
  servo.WritePosEx(9, 2047, 2400, 50);
  servo.WritePosEx(11, 2047, 2400, 50);
  delay(1000);

}

void loop() {

  d = sqrt(pow(pose[x], 2) + pow(pose[y], 2));
  D = (pow(d, 2) - pow(l2, 2) - pow(l3, 2)) / (2.0 * l2 * l3);
  deg[1][0] = atan2(sqrt(1 - pow(D, 2)) , D);                                                                                      // theta2_1
  deg[1][1] = atan2(-sqrt(1 - pow(D, 2)) , D);                                                                                     // theta2_2
  deg[0][0] = atan2(pose[y] , pose[x]) - atan2(l3 * sin(deg[1][0]) , l2 + l3 * cos(deg[1][0]));                    // theta1_1
  deg[0][1] = atan2(pose[y] , pose[x]) - atan2(l3 * sin(deg[1][1]) , l2 + l3 * cos(deg[1][1]));                    // theta1_2
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      deg[i][j] *= (180 / PI);
      deg[i][j] = (i == 0 ? map (deg[i][j], 0, 180, 3072, 1024) : map (deg[i][j], -90, 90, 3072, 1024));
    }
  }


  Serial.print("1번: ");
  Serial.print(deg[0][0]);
  Serial.print(",   ");
  Serial.println(deg[1][0]);

  Serial.print("2번: ");
  Serial.print(deg[0][1]);
  Serial.print(",   ");
  Serial.println(deg[1][1]);
  Serial.println();
  delay(1000);
}
