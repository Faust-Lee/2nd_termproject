#define stp 9
#define dir 8
#define upper_limit 7
#define lower_limit 6

void setup() {
  // put your setup code here, to run once:
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(upper_limit, INPUT);
  pinMode(lower_limit, INPUT);
  Serial.begin(115200);
 // 위쪽으로 초기화
  while (!digitalRead(7)) {
    digitalWrite(dir, HIGH); // 위로
    digitalWrite(stp, HIGH);
    delayMicroseconds(50);
    digitalWrite(stp, LOW);
    delayMicroseconds(50);
  }
}

void loop() {
  while (!digitalRead(6)) {
    digitalWrite(dir, LOW); // 아래로
    digitalWrite(stp, HIGH);
    delayMicroseconds(50);
    digitalWrite(stp, LOW);
    delayMicroseconds(50);
  }
  delay(500);
  while (!digitalRead(7)) {
    digitalWrite(dir, HIGH); // 위로
    digitalWrite(stp, HIGH);
    delayMicroseconds(50);
    digitalWrite(stp, LOW);
    delayMicroseconds(50);
  }
  delay(500);
}
