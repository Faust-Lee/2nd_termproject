// 타겟의 절대좌표를 반환
coordinate find_abs_coord(float target_x, float target_y) {
  float current_deg = arm.pul2deg(arm.ReadPos(1)) + arm.pul2deg(arm.ReadPos(2));
  float moniter_center[2] = {160, 120};
  float moniter_deg = atan2(-target_y + moniter_center[1], target_x - moniter_center[0]) * (180 / PI);
  float abs_deg = (moniter_deg + current_deg) * (PI / 180);
  float dist = sqrt(pow(target_x - moniter_center[0], 2) + pow(target_y - moniter_center[1], 2));
  float delta_x = -dist * cos(abs_deg) / 4;
  float delta_y = dist * sin(abs_deg) / 4 ;
  coordinate temp = {arm.EF[0] + delta_x, arm.EF[1] + delta_y, };
  return temp;
}
