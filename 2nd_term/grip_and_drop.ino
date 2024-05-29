// 잡고 블럭 위치로 이동
void grip_and_drop(int block) {

  arm.movez(targets[block].z);
  myservo4.write(targets[block].w);
  delay(1000);
  arm.movez(height + 150);

  switch (block) {
    case 1:
      if (flag2) {
      rotate_block(block);
      }
      flag1 = 0;
      flag2 = 0;
      break;
      
    case 2:
        if (flag2) {
      rotate_block(block);
      }
      flag1 = 0;
      flag2 = 0;
      break;
    case 3:
       if (flag2) {
      rotate_block(block);
      }
      flag1 = 0;
      flag2 = 0;
      break;
    case 4:
       if (flag1) {
      rotate_block(block);
      }
      flag1 = 0;
      flag2 = 0;
      break;
    case 5:
        if (flag1) {
      rotate_block(block);
      }
      flag1 = 0;
      flag2 = 0;
      break;
    case 6:
        if (flag1) {
      rotate_block(block);
      }
      flag1 = 0;
      flag2 = 0;
      break;

  }

  arm.WritePosEx(1, targets[block].x, 1000, 50);
  arm.WritePosEx(2, targets[block].y, 1000, 50);
  myservo3.write(targets[block].slope);
  delay(500);
  arm.movez(targets[block].z);
  // bibigi(block);
  myservo4.write(25);
  delay(500);
  arm.movez(height);
  myservo3.write(100);

}
