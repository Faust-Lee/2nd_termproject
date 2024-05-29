void rotate_block(int block){
  arm.movep(-20, 200);
  myservo3.write(0);
  arm.movez(targets[block].z);
  myservo4.write(25);
  delay(800);
  arm.movez(height);
  myservo3.write(200);
  delay(800);
  arm.movez(targets[block].z);
  myservo4.write(targets[block].w);
  delay(1000);
  arm.movez(height+150);
}
