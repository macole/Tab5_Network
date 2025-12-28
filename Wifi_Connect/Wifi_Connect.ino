#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

void setup() {
  // M5Unifiedの初期化
  auto cfg = M5.config();
  M5.begin(cfg);
  // ディスプレイの初期化
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("WiFi Connecting...");
  M5.Display.println(ssid);
  
  // アクセスポイントに接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print(".");
  }
  // ディスプレイにIPアドレスを表示
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(5);
  M5.Display.println("WiFi Connected!");
  M5.Display.println();
  M5.Display.setTextSize(3);
  M5.Display.print("IP: ");
  M5.Display.println(WiFi.localIP());
  
  delay(2000);

  // ディスプレイをクリア
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(5);
  M5.Display.println("HTTP Test");
  M5.Display.println();
  M5.Display.setTextSize(3);
  M5.Display.println("Connecting...");
  
  // HTTPクライアントの準備
  HTTPClient http;
  
  // HTTPリクエストの例（httpbin.orgのテストAPI）
  if (http.begin("http://httpbin.org/get")) {
    M5.Display.println("Requesting...");
    int status = http.GET();
    
    if (status > 0) {
      if (status == HTTP_CODE_OK) {
        // ディスプレイに成功メッセージを表示
        M5.Display.fillScreen(BLACK);
        M5.Display.setCursor(0, 0);
        M5.Display.setTextSize(5);
        M5.Display.println("Success!");
        M5.Display.println();
        M5.Display.setTextSize(3);
        M5.Display.println("HTTP OK");
        M5.Display.print("Size: ");
        M5.Display.println(response.length());
        M5.Display.println();
        M5.Display.setTextSize(2);
        M5.Display.println("Check Serial");
        M5.Display.println("for details");
      }
      else {
        // ディスプレイにエラーを表示
        M5.Display.fillScreen(BLACK);
        M5.Display.setCursor(0, 0);
        M5.Display.setTextSize(5);
        M5.Display.println("HTTP Error");
        M5.Display.setTextSize(3);
        M5.Display.print("Status: ");
        M5.Display.println(status);
      }
    }
    else {
      // ディスプレイにエラーを表示
      M5.Display.fillScreen(BLACK);
      M5.Display.setCursor(0, 0);
      M5.Display.setTextSize(3);
      M5.Display.println("Get Failed");
    }
    http.end();
  }
  else {
　   // ディスプレイにエラーを表示
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(3);
    M5.Display.println("Connect Error");
  }
}

void loop() {
  M5.update();
  delay(100);
}
