#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

/*****************
 * wi-fi設定
 * 各環境ごとに設定の必要あり
 * 2.4Gに対応
 *****************/
const char *ssid      = "xxxxxxxx"; // @todo: 各環境の設定
const char *password  = "xxxxxxxx"; // @todo: 各環境の設定
IPAddress ip(192, 168, 1, 123);     // IPアドレス（本機が利用するIP）
IPAddress gateway(192, 168, 1, 1);  // デフォルトゲートウェイ
IPAddress subnet(255, 255, 255, 0); // サブネットマスク

AsyncWebServer webServer ( 80 ); // port80

/********************
 * ステッピングモーター設定
 ********************/
#define STEPS 2048 // ステッピングモーターのステップ数
#define MOTOR_RPM 10 // RMP設定
#define MOTOR_PIN1 13 // モーターのピン接続
#define MOTOR_PIN2 12
#define MOTOR_PIN3 14
#define MOTOR_PIN4 27

/********************
 * タイマ設定
 ********************/
#define TIMER_DIVIDER 80 // ESP32は80MHzで動作
#define TIMER_ALARM_SEC 60 // タイマ割り込みを秒単位で設定

Stepper stepper(STEPS, MOTOR_PIN1, MOTOR_PIN2, MOTOR_PIN3, MOTOR_PIN4);
int isRotation = 0;

hw_timer_t * timer = NULL;
/********************
 * タイマ割り込み
 ********************/
void IRAM_ATTR onTimer() {
  Serial.println("timer interrupt");
  isRotation = 1;
}

/********************
 * ステッピングモーターを回転させる
 ********************/
void rotateMotor() {
  if (isRotation == 1) {
    Serial.println("start rotation");
    stepper.step(STEPS);
    isRotation = 0;
    Serial.println("end rotation");
  }
}

void setup(void) {
  Serial.begin(9600);
  Serial.println ("start");

  WiFi.config( ip, gateway, subnet );
  WiFi.begin ( ssid, password );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 1000 );
    Serial.print ( "." );
  }
  Serial.print ( "Wi-Fi Connected! IP address: " );
  Serial.println ( WiFi.localIP() );

  /**********************
   * homepage
   **********************/
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send ( 200, "text/html", "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>test</title><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/css/bootstrap.min.css\"></head><body><div class=\"container d-flex justify-content-center align-items-center vh-100\"><form action=\"/request\" method=\"get\"><div class=\"d-grid\"><button type=\"submit\" class=\"btn btn-primary\">Submit</button></div></form></div><script src=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/js/bootstrap.bundle.min.js\"></script></body></html>");
    Serial.println ("home page");
  });
  /**********************
   * ボタンを押した後のページ
   **********************/
  webServer.on("/request", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send ( 200, "text/html", "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>test</title><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/css/bootstrap.min.css\"></head><body><div class=\"container d-flex justify-content-center align-items-center vh-100\"><div class=\"d-grid\">thanks</div></div><script src=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/js/bootstrap.bundle.min.js\"></script></body></html>" );
    Serial.println ("button pressed, rotation.");
    isRotation = 1;
  });
  webServer.begin();
  Serial.println ( "Web server started" );

  /**********************
   * ステッピングモーターの設定
   **********************/
  stepper.setSpeed(MOTOR_RPM);

  /**********************
   * タイマ割り込み設定
   **********************/
  timer = timerBegin(0, TIMER_DIVIDER, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIMER_ALARM_SEC * 1000000, true);
  timerAlarmEnable(timer);
}

void loop(void){
  rotateMotor();
  isRotation = 0;
}
