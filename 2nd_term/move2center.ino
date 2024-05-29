// 카메라 중심으로 이동
void move2center(int block) {
  delay(100);
  huskylens.request();
  delay(100);
  if (huskylens.isLearned()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      ids[result.ID] = find_abs_coord(result.xCenter, result.yCenter);
    }
  }
  arm.movep((ids[block * 2 - 1].x + ids[block * 2].x) / 2, (ids[block * 2 - 1].y + ids[block * 2].y) / 2);
}
