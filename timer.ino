int count = 0;

hw_timer_t * timer = NULL;
// タイマ割り込み
void IRAM_ATTR onTimer() {
  count ++;
  Serial.print(count);
  Serial.println(": timer interrupt");
}

void setup() {
  Serial.begin(9600);
  Serial.println("start program");

  // タイマ割り込み設定
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() {
}
