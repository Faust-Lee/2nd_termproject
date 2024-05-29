// 지정한 범위를 움직이며 태그 인식
void scan_tags() {
  int x = 88, y = 210;
  for (int i = 1; i <= 5; i++) {
    for (int j = 1; j <= 5; j++) {
      arm.movep(x, y);
      delay(500);
      huskylens.request();
      delay(100);
      if (huskylens.isLearned()) {
        while (huskylens.available()) {
          HUSKYLENSResult result = huskylens.read();
          ids[result.ID] = find_abs_coord(result.xCenter, result.yCenter);
        }
      }
      x += 30;
    }
    switch (i) {
      case 1:
        x = 80;
        y = 170;
        break;
      case 2:
        x = 100;
        y = 130;
        break;
      case 3:
        x = 130;
        y = 90;
        break;
      case 4:
        x = 190;
        y = 50;
        break;

      default:
        break;
    }
  }
  for (int i = 1; i <= 6; i++) {
    mids[i] = {(ids[i * 2 - 1].x + ids[i * 2].x) / 2, (ids[i * 2 - 1].y + ids[i * 2].y) / 2};
  }
}

// 88 210
// 80 170
// 100 130
// 130 90
// 150 50
