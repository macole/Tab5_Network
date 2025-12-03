/*
 * M5Stack Tab5 天気予報表示アプリ
 * 気象庁の天気予報APIから天気情報を取得して表示します
 * 
 * 参考: https://bokunimo.net/blog/esp/3426/
 */

#define NTP_TIMEZONE "JST-9"
#define NTP_SERVER1  "0.pool.ntp.org"
#define NTP_SERVER2  "1.pool.ntp.org"
#define NTP_SERVER3  "2.pool.ntp.org"

#include <Arduino.h>
#include <M5GFX.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp_sntp.h>
#include <sntp.h>

#define SDIO2_CLK GPIO_NUM_12
#define SDIO2_CMD GPIO_NUM_13
#define SDIO2_D0  GPIO_NUM_11
#define SDIO2_D1  GPIO_NUM_10
#define SDIO2_D2  GPIO_NUM_9
#define SDIO2_D3  GPIO_NUM_8
#define SDIO2_RST GPIO_NUM_15

// WiFi認証情報は secrets.h から読み込みます
#include "secrets.h"

// 気象庁APIのエンドポイント（例: 東京の地域コード130000）
// 地域コードは気象庁のサイトで確認してください
#define WEATHER_API_URL "https://www.jma.go.jp/bosai/forecast/data/forecast/130000.json"

// 天気コードの定義（気象庁APIの天気コード）
// 100: 晴れ, 200: 曇り, 300: 雨, 400: 雪
enum WeatherCode {
    SUNNY = 100,
    CLOUDY = 200,
    RAIN = 300,
    SNOW = 400
};

// 天気情報構造体
struct WeatherInfo {
    String weather;      // 天気（晴れ、曇り、雨、雪）
    int tempMax;         // 最高気温
    int tempMin;         // 最低気温
    int pop;             // 降水確率（%）
    WeatherCode code;    // 天気コード
};

WeatherInfo todayWeather;
WeatherInfo tomorrowWeather;
bool weatherUpdated = false;
unsigned long lastWeatherUpdate = 0;
const unsigned long WEATHER_UPDATE_INTERVAL = 3600000; // 1時間ごとに更新
String lastError = "";

void setup() {
    M5.begin();
    Serial.begin(115200);
    delay(1000);

    M5.Display.setRotation(3); // 横向き
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.println("Weather App");
    M5.Display.println("Initializing...");

    // WiFi設定
    WiFi.setPins(SDIO2_CLK, SDIO2_CMD, SDIO2_D0, SDIO2_D1, SDIO2_D2, SDIO2_D3, SDIO2_RST);
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

    // NTP時刻同期
    M5.Display.println("Syncing time...");
    configTzTime(NTP_TIMEZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
    
    while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
        delay(1000);
        M5.Display.print(".");
    }
    M5.Display.println("\nTime synced!");

    // RTCに時刻を設定
    time_t now = time(nullptr);
    if (now > 0) {
        M5.Rtc.setDateTime(gmtime(&now));
    }

    delay(1000);
    M5.Display.fillScreen(BLACK);
    
    // 初回天気情報取得
    updateWeather();
}

void loop() {
    M5.update();
    
    // 1時間ごとに天気情報を更新
    if (millis() - lastWeatherUpdate > WEATHER_UPDATE_INTERVAL) {
        updateWeather();
    }

    // 画面表示を更新（1秒ごと）
    static unsigned long lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate > 1000) {
        displayWeather();
        lastDisplayUpdate = millis();
    }

    delay(100);
}

void updateWeather() {
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.println("Updating weather...");

    WiFiClientSecure client;
    client.setInsecure(); // SSL証明書の検証をスキップ（開発用）
    
    HTTPClient http;
    http.begin(client, WEATHER_API_URL);
    http.setTimeout(10000);

    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Response received:");
        Serial.println(payload);

        // JSONパース
        DynamicJsonDocument doc(16384); // 気象庁APIのレスポンスは大きいのでバッファを拡大
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            // 気象庁APIのJSON構造をパース
            // 構造: [{ "timeSeries": [{ "areas": [{ "weathers": [...] }] }] }]
            if (doc.is<JsonArray>() && doc.size() > 0) {
                JsonObject forecast = doc[0];
                
                if (forecast.containsKey("timeSeries")) {
                    JsonArray timeSeries = forecast["timeSeries"];
                    
                    // 天気情報（timeSeries[0]）
                    if (timeSeries.size() > 0 && timeSeries[0].containsKey("areas")) {
                        JsonArray areas = timeSeries[0]["areas"];
                        if (areas.size() > 0 && areas[0].containsKey("weathers")) {
                            JsonArray weathers = areas[0]["weathers"];
                            if (weathers.size() > 0) {
                                String weatherStr = weathers[0].as<String>();
                                todayWeather.weather = weatherStr;
                                todayWeather.code = parseWeatherCode(weatherStr);
                            }
                            if (weathers.size() > 1) {
                                String weatherStr2 = weathers[1].as<String>();
                                tomorrowWeather.weather = weatherStr2;
                                tomorrowWeather.code = parseWeatherCode(weatherStr2);
                            }
                        }
                    }
                    
                    // 気温情報（timeSeries[2]）
                    if (timeSeries.size() > 2 && timeSeries[2].containsKey("areas")) {
                        JsonArray tempAreas = timeSeries[2]["areas"];
                        if (tempAreas.size() > 0 && tempAreas[0].containsKey("temps")) {
                            JsonArray temps = tempAreas[0]["temps"];
                            if (temps.size() >= 2) {
                                todayWeather.tempMax = temps[0].as<int>();
                                todayWeather.tempMin = temps[1].as<int>();
                            }
                        }
                    }
                }
            }
            
            weatherUpdated = true;
            lastWeatherUpdate = millis();
            Serial.println("Weather updated successfully");
        } else {
            Serial.print("JSON parse error: ");
            Serial.println(error.c_str());
            M5.Display.setCursor(0, 50);
            M5.Display.print("JSON Error: ");
            M5.Display.println(error.c_str());
        }
    } else {
        Serial.print("HTTP error: ");
        Serial.println(httpCode);
        lastError = "HTTP Error: " + String(httpCode);
        M5.Display.setCursor(0, 100);
        M5.Display.print("HTTP Error: ");
        M5.Display.println(httpCode);
    }

    http.end();
    delay(1000); // APIへの負荷を軽減
}

WeatherCode parseWeatherCode(String weather) {
    weather.toLowerCase();
    if (weather.indexOf("晴") >= 0) {
        return SUNNY;
    } else if (weather.indexOf("雨") >= 0) {
        return RAIN;
    } else if (weather.indexOf("雪") >= 0) {
        return SNOW;
    } else {
        return CLOUDY;
    }
}

void displayWeather() {
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_16);
    M5.Display.setTextColor(WHITE, BLACK);
    
    int y = 20;
    
    // 現在時刻表示
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    M5.Display.setCursor(10, y);
    M5.Display.printf("%04d/%02d/%02d %02d:%02d:%02d\n",
                      timeinfo->tm_year + 1900,
                      timeinfo->tm_mon + 1,
                      timeinfo->tm_mday,
                      timeinfo->tm_hour,
                      timeinfo->tm_min,
                      timeinfo->tm_sec);
    y += 30;

    if (!weatherUpdated) {
        M5.Display.setCursor(10, y);
        M5.Display.println("Weather data not available");
        if (lastError.length() > 0) {
            M5.Display.setCursor(10, y + 25);
            M5.Display.setFont(&fonts::lgfxJapanMinchoP_12);
            M5.Display.println(lastError);
        }
        return;
    }

    // 今日の天気
    M5.Display.setCursor(10, y);
    M5.Display.print("Today: ");
    M5.Display.println(todayWeather.weather);
    y += 25;

    // 天気アイコン（簡易版）
    M5.Display.setCursor(10, y);
    drawWeatherIcon(10, y, todayWeather.code);
    y += 60;

    // 気温
    M5.Display.setCursor(10, y);
    M5.Display.printf("Temp: %d / %d C\n", todayWeather.tempMax, todayWeather.tempMin);
    y += 25;

    // 明日の天気
    M5.Display.setCursor(10, y);
    M5.Display.print("Tomorrow: ");
    M5.Display.println(tomorrowWeather.weather);
    y += 25;

    // 明日の天気アイコン
    M5.Display.setCursor(10, y);
    drawWeatherIcon(10, y, tomorrowWeather.code);
    y += 60;

    // 更新時刻
    M5.Display.setCursor(10, M5.Display.height() - 30);
    M5.Display.setFont(&fonts::lgfxJapanMinchoP_12);
    M5.Display.print("Last update: ");
    unsigned long minutesAgo = (millis() - lastWeatherUpdate) / 60000;
    if (minutesAgo < 60) {
        M5.Display.print(minutesAgo);
        M5.Display.println(" min ago");
    } else {
        unsigned long hoursAgo = minutesAgo / 60;
        M5.Display.print(hoursAgo);
        M5.Display.println(" hours ago");
    }
    
    // WiFi接続状態
    M5.Display.setCursor(10, M5.Display.height() - 15);
    if (WiFi.status() == WL_CONNECTED) {
        M5.Display.setTextColor(GREEN, BLACK);
        M5.Display.print("WiFi: OK");
    } else {
        M5.Display.setTextColor(RED, BLACK);
        M5.Display.print("WiFi: Disconnected");
    }
    M5.Display.setTextColor(WHITE, BLACK);
}

void drawWeatherIcon(int x, int y, WeatherCode code) {
    int size = 50;
    switch (code) {
        case SUNNY:
            // 晴れ: 黄色い円
            M5.Display.fillCircle(x + size/2, y + size/2, size/2 - 5, YELLOW);
            break;
        case CLOUDY:
            // 曇り: 灰色の雲
            M5.Display.fillCircle(x + size/3, y + size/2, size/4, DARKGREY);
            M5.Display.fillCircle(x + size*2/3, y + size/2, size/4, DARKGREY);
            M5.Display.fillRect(x + size/4, y + size/2 - size/8, size/2, size/4, DARKGREY);
            break;
        case RAIN:
            // 雨: 灰色の雲 + 青い線
            M5.Display.fillCircle(x + size/3, y + size/3, size/4, DARKGREY);
            M5.Display.fillCircle(x + size*2/3, y + size/3, size/4, DARKGREY);
            M5.Display.fillRect(x + size/4, y + size/3 - size/8, size/2, size/4, DARKGREY);
            for (int i = 0; i < 5; i++) {
                M5.Display.drawLine(x + size/4 + i*size/8, y + size*2/3, 
                                   x + size/4 + i*size/8, y + size*3/4, BLUE);
            }
            break;
        case SNOW:
            // 雪: 灰色の雲 + 白い点
            M5.Display.fillCircle(x + size/3, y + size/3, size/4, DARKGREY);
            M5.Display.fillCircle(x + size*2/3, y + size/3, size/4, DARKGREY);
            M5.Display.fillRect(x + size/4, y + size/3 - size/8, size/2, size/4, DARKGREY);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    M5.Display.fillCircle(x + size/3 + i*size/6, y + size*2/3 + j*size/8, 3, WHITE);
                }
            }
            break;
        default:
            // 不明: グレーの四角
            M5.Display.fillRect(x, y, size, size, DARKGREY);
            break;
    }
}
