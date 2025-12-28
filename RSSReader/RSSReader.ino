#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi認証情報は secrets.h から読み込みます
// secrets.h.example をコピーして secrets.h を作成し、実際のSSIDとパスワードを設定してください
#include "secrets.h"

void setup() {
  // M5Unifiedの初期化
  auto cfg = M5.config();
  M5.begin(cfg);
  
  delay(500);
  
  // ディスプレイの初期化
  M5.Display.setFont(&fonts::lgfxJapanGothic_16);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("WiFi接続中...");
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
  M5.Display.setFont(&fonts::lgfxJapanGothic_20);
  M5.Display.println("WiFi接続完了!");
  M5.Display.println();
  M5.Display.setFont(&fonts::lgfxJapanGothic_16);
  M5.Display.print("IP: ");
  M5.Display.println(WiFi.localIP());
  
  delay(2000);

  // ディスプレイをクリア
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.setFont(&fonts::lgfxJapanGothic_20);
  M5.Display.println("RSSフィードに");
  M5.Display.println("接続中...");
  M5.Display.println();
  
  // メモリ情報を表示
  M5.Display.setFont(&fonts::lgfxJapanGothic_12);
  M5.Display.print("空きメモリ: ");
  M5.Display.print(ESP.getFreeHeap());
  M5.Display.println(" bytes");
  M5.Display.println();
  
  // HTTPクライアントの準備
  HTTPClient http;
  
  // タイムアウトを長めに設定（デフォルトは5秒）
  http.setTimeout(15000);  // 15秒
  
  // Yahoo News RSSフィード（HTTPS）
  // HTTPClient.h は内部で適切にHTTPS通信を処理します
  if (http.begin("https://news.yahoo.co.jp/rss/topics/top-picks.xml")) {
    M5.Display.println("Requesting...");
    int status = http.GET();
    
    // デバッグ情報を表示
    M5.Display.print("Status code: ");
    M5.Display.println(status);
    
    if (status > 0) {
      if (status == HTTP_CODE_OK) {
        // 得られたXMLを取得する
        String xml = http.getString();
        
        // RSSニュースをディスプレイに表示
        displayRSSNews(xml);
      }
      else {
        // ディスプレイにエラーを表示
        M5.Display.fillScreen(BLACK);
        M5.Display.setCursor(0, 0);
        M5.Display.setFont(&fonts::lgfxJapanGothic_16);
        M5.Display.println("HTTP Error");
        M5.Display.print("Status: ");
        M5.Display.println(status);
      }
    }
    else {
      // ディスプレイにエラーを表示
      M5.Display.fillScreen(BLACK);
      M5.Display.setCursor(0, 0);
      M5.Display.setFont(&fonts::lgfxJapanGothic_16);
      M5.Display.println("Get Failed");
      M5.Display.println();
      M5.Display.print("Error code: ");
      M5.Display.println(status);
      M5.Display.println();
      
      // エラーコードの説明
      if (status == -1) {
        M5.Display.println("Connection failed");
      } else if (status == -11) {
        M5.Display.println("Timeout");
      } else {
        M5.Display.println("Unknown error");
      }
    }
    http.end();
  }
  else {
    // ディスプレイにエラーを表示
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.setFont(&fonts::lgfxJapanGothic_16);
    M5.Display.println("Connect Error");
  }
}

void loop() {
  M5.update();
  delay(100);
}

// XMLから指定タグの内容を抽出する関数
String extractTag(String xml, String tagName, int startPos = 0) {
  String openTag = "<" + tagName + ">";
  String closeTag = "</" + tagName + ">";
  
  int start = xml.indexOf(openTag, startPos);
  if (start == -1) return "";
  start += openTag.length();
  
  int end = xml.indexOf(closeTag, start);
  if (end == -1) return "";
  
  return xml.substring(start, end);
}

// RSSのニュースタイトルを抽出して表示する関数
void displayRSSNews(String xml) {
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  
  // 日本語フォントを設定
  M5.Display.setFont(&fonts::lgfxJapanGothic_16);
  M5.Display.setTextColor(YELLOW, BLACK);
  M5.Display.println("=== Yahoo News ===");
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.println();
  
  int itemCount = 0;
  int searchPos = 0;
  
  // <item>タグを順に検索
  while (itemCount < 5) {  // 最大5件表示
    int itemStart = xml.indexOf("<item>", searchPos);
    if (itemStart == -1) break;
    
    int itemEnd = xml.indexOf("</item>", itemStart);
    if (itemEnd == -1) break;
    
    // <item>内の<title>を抽出
    String title = extractTag(xml, "title", itemStart);
    if (title.length() > 0) {
      itemCount++;
      M5.Display.setTextColor(CYAN, BLACK);
      M5.Display.printf("%d. ", itemCount);
      M5.Display.setTextColor(WHITE, BLACK);
      
      // タイトルを表示（日本語フォントで表示）
      M5.Display.println(title);
      M5.Display.println();
    }
    
    searchPos = itemEnd + 7;  // </item>の後から検索
  }
  
  M5.Display.println();
  M5.Display.setTextColor(GREEN, BLACK);
  M5.Display.printf("合計: %d 件\n", itemCount);
}
