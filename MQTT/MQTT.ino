/*
 * M5Stack Tab5 MQTT通信サンプル
 * MQTTブローカーとの通信を行うプログラム
 * 
 * 改変: M5Unifiedに対応
 */
#include <M5Unified.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// WiFi認証情報とMQTTサーバー設定は secrets.h から読み込みます
// secrets.h.example をコピーして secrets.h を作成し、
// 実際のSSID、パスワード、MQTTサーバーを設定してください
#include "secrets.h"

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setupWifi();
void callback(char* topic, byte* payload, unsigned int length);
void reConnect();

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE, BLACK);
    
    setupWifi();
    client.setServer(mqtt_server, 1883);  // MQTTサーバーの設定
    client.setCallback(callback);  // メッセージ受信時のコールバック関数を設定
}

void loop() {
    if (!client.connected()) {
        reConnect();
    }
    // クライアントのメッセージ処理とサーバー接続維持のため定期的に呼び出す
    client.loop();

    // 起動からの経過時間を取得
    unsigned long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        // メッセージを整形してバッファに格納
        snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
        M5.Display.print("Publish message: ");
        M5.Display.println(msg);
        // 指定したトピックにメッセージを発行
        client.publish("M5Stack", msg);
        if (value % 12 == 0) {
            M5.Display.fillScreen(BLACK);
            M5.Display.setCursor(0, 0);
        }
    }
}

void setupWifi() {
    delay(10);
    M5.Display.printf("Connecting to %s", ssid);
    WiFi.mode(WIFI_STA);  // WiFiステーションモードに設定
    WiFi.begin(ssid, password);  // WiFi接続を開始

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Display.print(".");
    }
    M5.Display.printf("\nSuccess\n");
}

void callback(char* topic, byte* payload, unsigned int length) {
    M5.Display.print("Message arrived [");
    M5.Display.print(topic);
    M5.Display.print("] ");
    for (int i = 0; i < length; i++) {
        M5.Display.print((char)payload[i]);
    }
    M5.Display.println();
}

void reConnect() {
    while (!client.connected()) {
        M5.Display.print("Attempting MQTT connection...");
        // ランダムなクライアントIDを生成
        String clientId = "M5Stack-";
        clientId += String(random(0xffff), HEX);
        // 接続を試行
        if (client.connect(clientId.c_str())) {
            M5.Display.printf("\nSuccess\n");
            // 接続成功後、トピックにメッセージを発行
            client.publish("M5Stack", "hello world");
            // トピックを購読
            client.subscribe("M5Stack");
        } else {
            M5.Display.print("failed, rc=");
            M5.Display.print(client.state());
            M5.Display.println("try again in 5 seconds");
            delay(5000);
        }
    }
}