#include <WiFi.h>
#include <ESPAsyncWebServer.h>

/*****************
 * wi-fi設定
 * 各環境ごとに設定の必要あり
 * 2.4Gに対応
 *****************/
const char *ssid      = "XXXXXX"; // @todo: 各環境の設定
const char *password  = "XXXXXX"; // @todo: 各環境の設定
IPAddress ip(192, 168, 1, 123);     // IPアドレス（本機が利用するIP）
IPAddress gateway(192, 168, 1, 1);  // デフォルトゲートウェイ
IPAddress subnet(255, 255, 255, 0); // サブネットマスク

AsyncWebServer webServer ( 80 );

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

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send ( 200, "text/html", "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>test</title><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/css/bootstrap.min.css\"></head><body><div class=\"container d-flex justify-content-center align-items-center vh-100\"><form action=\"/request\" method=\"get\"><div class=\"d-grid\"><button type=\"submit\" class=\"btn btn-primary\">Submit</button></div></form></div><script src=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/js/bootstrap.bundle.min.js\"></script></body></html>");
    Serial.println ("home page");
  });
  webServer.on("/request", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send ( 200, "text/html", "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>test</title><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/css/bootstrap.min.css\"></head><body><div class=\"container d-flex justify-content-center align-items-center vh-100\"><div class=\"d-grid\">thanks</div></div><script src=\"https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.0.2/js/bootstrap.bundle.min.js\"></script></body></html>" );
    Serial.println ("thanks page");
  });
  webServer.begin();
  Serial.println ( "Web server started" );
}

void loop(void){
}
