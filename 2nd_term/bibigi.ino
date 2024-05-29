void bibigi(int block) {
  int vibrate = 70;
  switch (block) {
    case 1:

      arm.WritePosEx(2, targets[block].y - vibrate*2, 1000, 50);
      delay(100);

      arm.WritePosEx(2, targets[block].y + vibrate, 1000, 50);

      break;
    default:
      break;
  }
}
