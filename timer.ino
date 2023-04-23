#include <Stepper.h>
#define STEPS 200 // ステッピングモーターのステップ数
#define MOTOR_PIN1 13 // モーターのピン接続
#define MOTOR_PIN2 12
#define MOTOR_PIN3 14
#define MOTOR_PIN4 27

Stepper stepper(STEPS, MOTOR_PIN1, MOTOR_PIN2, MOTOR_PIN3, MOTOR_PIN4);
int count = 0;

hw_timer_t * timer = NULL;
// タイマ割り込み
void IRAM_ATTR onTimer() {
  count ++;
  Serial.println(count);
  Serial.println("timer interrupt");
}

void setup() {
  Serial.begin(9600);
  while (!Serial); 
  Serial.println("start program");

  stepper.setSpeed(10); // モーターの回転速度を設定

  // タイマ割り込み設定
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
   timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}
void loop() {
  Serial.println("rotation");
  delay(8000); // 8sec
}

void rotation() {
  stepper.step(STEPS); // ステッピングモーターを1回転させる
}
