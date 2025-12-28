/*
 * M5Stack Tab5 ChatGPT 通信アプリ
 * OpenAI APIを使用してChatGPTと対話するアプリケーション
 * 
 * 参考: https://deviceplus.jp/mc-general/m5stack-chatgpt-01/
 */

#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi認証情報とOpenAI APIキーは secrets.h から読み込みます
#include "secrets.h"

// OpenAI API設定
#define OPENAI_API_URL "https://api.openai.com/v1/chat/completions"
#define MODEL "gpt-3.5-turbo"  // または "gpt-4"

// 画面表示用
String chatHistory = "";
String currentResponse = "";
bool isWaitingResponse = false;
unsigned long lastUpdate = 0;

// プロンプトテンプレート
const char* prompts[] = {
    "面白い短い話をしてください",
    "今日の天気について教えてください",
    "簡単ななぞなぞを出してください",
    "今日の運勢を教えてください",
    "短い詩を作ってください"
};
int promptIndex = 0;

void setup() {
    M5.begin();
    delay(1000);

    M5.Display.setRotation(3); // 横向き
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.println("ChatGPT App");
    M5.Display.println("Initializing...");

    // WiFi設定
    // Arduino IDEでM5Tab5ボードを選択した場合、定義済みのデフォルトピンを使用できます
    WiFi.setPins(BOARD_SDIO_ESP_HOSTED_CLK, BOARD_SDIO_ESP_HOSTED_CMD, BOARD_SDIO_ESP_HOSTED_D0,
        BOARD_SDIO_ESP_HOSTED_D1, BOARD_SDIO_ESP_HOSTED_D2, BOARD_SDIO_ESP_HOSTED_D3,
        BOARD_SDIO_ESP_HOSTED_RESET);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    M5.Display.print("Connecting WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Display.print(".");
    }
    M5.Display.println("\nWiFi Connected!");
    M5.Display.print("IP: ");
    M5.Display.println(WiFi.localIP());

    delay(1000);
    M5.Display.fillScreen(BLACK);
    displayWelcome();
}

void loop() {
    M5.update();

    // タッチ操作の処理
    auto t = M5.Touch.getDetail();
    if (t.wasPressed()) {
        int x = t.x;
        int y = t.y;
        
        // 画面を4分割して操作エリアを定義
        int screenWidth = M5.Display.width();
        int screenHeight = M5.Display.height();
        
        // 左上: 前のプロンプト
        if (x < screenWidth / 2 && y < screenHeight / 2) {
            promptIndex = (promptIndex - 1 + 5) % 5;
            displayPromptSelection();
        }
        // 右上: 次のプロンプト
        else if (x >= screenWidth / 2 && y < screenHeight / 2) {
            promptIndex = (promptIndex + 1) % 5;
            displayPromptSelection();
        }
        // 左下: プロンプト送信
        else if (x < screenWidth / 2 && y >= screenHeight / 2) {
            if (!isWaitingResponse) {
                sendChatGPTRequest(prompts[promptIndex]);
            }
        }
        // 右下: 画面クリア
        else if (x >= screenWidth / 2 && y >= screenHeight / 2) {
            chatHistory = "";
            currentResponse = "";
            displayWelcome();
        }
    }

    // 画面更新（1秒ごと）
    if (millis() - lastUpdate > 1000) {
        displayStatus();
        lastUpdate = millis();
    }

    delay(100);
}

void displayWelcome() {
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("ChatGPT App");
    M5.Display.println("============");
    M5.Display.println("");
    M5.Display.println("Touch areas:");
    M5.Display.println("Top-Left: Prev prompt");
    M5.Display.println("Top-Right: Next prompt");
    M5.Display.println("Bottom-Left: Send");
    M5.Display.println("Bottom-Right: Clear");
    M5.Display.println("");
    displayPromptSelection();
}

void displayPromptSelection() {
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setTextColor(CYAN, BLACK);
    M5.Display.setCursor(10, 200);
    M5.Display.print("Prompt [");
    M5.Display.print(promptIndex + 1);
    M5.Display.print("/5]: ");
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.println(prompts[promptIndex]);
}

void displayStatus() {
    int y = M5.Display.height() - 30;
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_12);
    M5.Display.setCursor(10, y);
    
    if (WiFi.status() == WL_CONNECTED) {
        M5.Display.setTextColor(GREEN, BLACK);
        M5.Display.print("WiFi: OK  ");
    } else {
        M5.Display.setTextColor(RED, BLACK);
        M5.Display.print("WiFi: NG  ");
    }
    
    if (isWaitingResponse) {
        M5.Display.setTextColor(YELLOW, BLACK);
        M5.Display.print("Waiting...");
    } else {
        M5.Display.setTextColor(WHITE, BLACK);
        M5.Display.print("Ready");
    }
    M5.Display.setTextColor(WHITE, BLACK);
}

void sendChatGPTRequest(String prompt) {
    isWaitingResponse = true;
    currentResponse = "";
    
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setCursor(10, 10);
    M5.Display.setTextColor(YELLOW, BLACK);
    M5.Display.println("Sending request...");
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.print("Prompt: ");
    M5.Display.println(prompt);
    M5.Display.println("");
    
    WiFiClientSecure client;
    client.setInsecure(); // SSL証明書の検証をスキップ（開発用）
    
    HTTPClient http;
    http.begin(client, OPENAI_API_URL);
    http.setTimeout(30000); // 30秒タイムアウト
    
    // ヘッダー設定
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + openaiApiKey);
    
    // リクエストボディの作成
    DynamicJsonDocument doc(2048);
    doc["model"] = MODEL;
    JsonArray messages = doc.createNestedArray("messages");
    JsonObject message = messages.createNestedObject();
    message["role"] = "user";
    message["content"] = prompt;
    doc["max_tokens"] = 200; // レスポンスの最大トークン数
    doc["temperature"] = 0.7; // 創造性の設定（0.0-2.0）
    
    String requestBody;
    serializeJson(doc, requestBody);
    
    // リクエスト送信
    int httpCode = http.POST(requestBody);
    
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        
        // JSONパース
        DynamicJsonDocument responseDoc(8192);
        DeserializationError error = deserializeJson(responseDoc, payload);
        
        if (!error) {
            if (responseDoc.containsKey("choices") && responseDoc["choices"].size() > 0) {
                JsonObject choice = responseDoc["choices"][0];
                if (choice.containsKey("message")) {
                    JsonObject message = choice["message"];
                    if (message.containsKey("content")) {
                        currentResponse = message["content"].as<String>();
                        displayResponse(prompt, currentResponse);
                    }
                }
            } else {
                displayError("No response in JSON");
            }
        } else {
            displayError("JSON parse error");
        }
    } else {
        String errorMsg = "HTTP Error: " + String(httpCode);
        displayError(errorMsg);
    }
    
    http.end();
    isWaitingResponse = false;
}

void displayResponse(String prompt, String response) {
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    
    // プロンプト表示
    M5.Display.setTextColor(CYAN, BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.print("Q: ");
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.println(prompt);
    
    // 区切り線
    M5.Display.drawLine(10, 50, M5.Display.width() - 10, 50, DARKGREY);
    
    // レスポンス表示
    M5.Display.setTextColor(GREEN, BLACK);
    M5.Display.setCursor(10, 60);
    M5.Display.print("A: ");
    M5.Display.setTextColor(WHITE, BLACK);
    
    // 長いテキストを折り返して表示
    int lineHeight = 25;
    int maxWidth = M5.Display.width() - 20;
    int y = 80;
    int x = 10;
    
    String displayText = response;
    int textLength = displayText.length();
    int charsPerLine = maxWidth / 12; // おおよその文字数
    
    for (int i = 0; i < textLength; i += charsPerLine) {
        int endPos = (i + charsPerLine < textLength) ? (i + charsPerLine) : textLength;
        String line = displayText.substring(i, endPos);
        M5.Display.setCursor(x, y);
        M5.Display.println(line);
        y += lineHeight;
        
        // 画面からはみ出したら終了
        if (y > M5.Display.height() - 50) {
            M5.Display.setCursor(x, y);
            M5.Display.println("...");
            break;
        }
    }
    
    // チャット履歴に追加
    chatHistory += "Q: " + prompt + "\n";
    chatHistory += "A: " + response + "\n\n";
    
    // 操作説明
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_12);
    M5.Display.setCursor(10, M5.Display.height() - 30);
    M5.Display.setTextColor(DARKGREY, BLACK);
    M5.Display.println("Touch Bottom-Left to send, Bottom-Right to clear");
}

void displayError(String errorMsg) {
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setTextColor(RED, BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Error!");
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setCursor(10, 40);
    M5.Display.println(errorMsg);
    M5.Display.setCursor(10, 80);
    M5.Display.println("Check Serial Monitor");
    M5.Display.println("for details.");
}

