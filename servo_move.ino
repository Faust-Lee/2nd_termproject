#include <SCServo.h>
SMS_STS servo;  //635~3484

#define l2 150 //unit: mm
#define l3 100

float current_deg[2]={90,0};


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

void servo_move2(int x, int y){
  bool theta1=1,theta2=1;
  float target_deg[2],deg[2][2];
  float d = sqrt(pow(x, 2) + pow(y, 2));
  float D = (pow(d, 2) - pow(l2, 2) - pow(l3, 2)) / (2.0 * l2 * l3);
  
  deg[1][0] = atan2(sqrt(1 - pow(D, 2)) , D);                                                          // theta2_1
  deg[1][1] = atan2(-sqrt(1 - pow(D, 2)) , D);                                                         // theta2_2
  deg[0][0] = atan2(y , x) - atan2(l3 * sin(deg[1][0]) , l2 + l3 * cos(deg[1][0]));                    // theta1_1
  deg[0][1] = atan2(y , x) - atan2(l3 * sin(deg[1][1]) , l2 + l3 * cos(deg[1][1]));                    // theta1_2
  for (int i = 0; i < 2; i++)
    for (int j=0;j<2;j++)
      deg[i][j] *= (180 / PI);
  
  if (deg[0][0]<0||180<deg[0][0]||-90>deg[1][0]||90<deg[1][0]) theta1 = 0;
  if(deg[0][1]<0||180<deg[0][1]||-90>deg[1][1]||90<deg[1][1]) theta2 = 0;
  
  if(theta1&&theta2){
    if( abs(current_deg[0]-deg[0][0])+abs(current_deg[1]-deg[1][0]) < abs(current_deg[0]-deg[0][1])+abs(current_deg[1]-deg[1][1])){
      target_deg[0] = deg[0][0];
      target_deg[1] = deg[1][0];
    }
    else{
      target_deg[0] = deg[0][1];
      target_deg[1] = deg[1][1];
    }
  }
  else if(theta1){
      target_deg[0] = deg[0][0];
      target_deg[1] = deg[1][0];
  }
  else if(theta2){
      target_deg[0] = deg[0][1];
      target_deg[1] = deg[1][1];
  }
  delta_theta1=abs(map (current_deg[0], 0, 180, 3072, 1024)-map (target_deg[0], 0, 180, 3072, 1024));
  delta_theta2=abs(map (current_deg[1], 0, 180, 3072, 1024)-map (target_deg[1], 0, 180, 3072, 1024));

  servo.WritePosEx(9, map (target_deg[0], 0, 180, 3072, 1024), 2400, 50);
  
  servo.WritePosEx(11, map (target_deg[1], -90, 90, 3072, 1024), 2400, 50);
}
