# WebAPI_Wether - 気象庁天気予報アプリ

M5Stack Tab5のWiFi機能を使用して、**気象庁の天気予報API**から天気情報を取得し、ディスプレイに表示するプログラムです。

## 📋 概要

このプログラムは、M5Stack Tab5のWiFi機能を使用して、気象庁が提供する天気予報APIにアクセスし、今日と明日の天気・気温を取得して表示します。HTTPS通信、JSON解析、時刻同期（SNTP）、グラフィカルな天気アイコン表示など、実用的なIoTアプリケーションの要素を含んでいます。

## 🔧 必要なハードウェア

- M5Stack Tab5

## 📚 必要なライブラリ

- **M5Unified**: M5Stack統合ライブラリ
  ```bash
  arduino-cli lib install "M5Unified@0.2.10"
  ```
- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **WiFiClientSecure**: ESP32標準ライブラリ（HTTPS通信用、Arduino IDEに含まれています）
- **HTTPClient**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **ArduinoJson**: JSON解析ライブラリ
  ```bash
  arduino-cli lib install "ArduinoJson"
  ```
- **esp_sntp**: ESP32標準ライブラリ（時刻同期用、Arduino IDEに含まれています）

## 🚀 セットアップ

### 1. ボード設定

- Board: ESP32P4 Dev Module
- USB CDC on boot: Enabled
- Flash Size: 16MB (128Mb)
- Partition Scheme: Default
- PSRAM: Enabled

### 2. WiFi設定

`secrets.h.example`をコピーして`secrets.h`を作成してください：

```bash
cp secrets.h.example secrets.h
```

`secrets.h`ファイル内の`ssid`と`password`を実際の値に編集してください：

```cpp
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";
```

**重要**: `secrets.h`は`.gitignore`に追加されているため、Gitにコミットされません。認証情報が漏れる心配はありません。

### 3. 地域コードの設定（オプション）

デフォルトでは東京の天気情報（地域コード: 130000）を取得します。別の地域の天気を表示したい場合は、コード内の地域コードを変更してください：

```cpp
#define WEATHER_API_URL "https://www.jma.go.jp/bosai/forecast/data/forecast/130000.json"
```

主な地域コード：
- 東京: 130000
- 大阪: 270000
- 北海道（札幌）: 016000
- 福岡: 400000
- 沖縄: 471000

詳細な地域コードは[気象庁のサイト](https://www.jma.go.jp/bosai/common/const/area.json)で確認できます。

### 4. プログラムのアップロード

Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

1. WiFiのSSIDとパスワードを設定
2. 必要に応じて地域コードを変更
3. プログラムをアップロード
4. デバイスがWiFiネットワークに接続し、SNTP時刻同期後、天気情報を取得します
5. 取得した天気情報が画面に表示されます

## 📊 機能

- **HTTPS通信**: SSL/TLSを使用したセキュアな通信で気象庁APIにアクセス
- **JSON解析**: ArduinoJsonライブラリを使用して気象庁APIのレスポンスを解析
- **SNTP時刻同期**: NTPサーバーから正確な時刻を取得してRTCに設定
- **天気情報表示**: 今日と明日の天気、最高気温・最低気温を表示
- **天気アイコン**: 晴れ・曇り・雨・雪の天気アイコンをグラフィカルに表示
- **自動更新**: 1時間ごとに天気情報を自動更新
- **日本語表示**: 日本語フォント（lgfxJapanMinchoP_16）を使用して天気情報を表示
- **エラー表示**: HTTP通信エラーやJSON解析エラーを画面に表示

## 🔍 プログラムの動作

### 1. 初期化

- M5Stackの初期化
- WiFi接続
- SNTP時刻同期（日本時間: JST-9）
- 初回天気情報取得

### 2. メインループ

- 1時間ごとに天気情報を更新
- 1秒ごとに画面表示を更新（現在時刻、天気、WiFi状態など）

### 3. 天気情報取得

- 気象庁APIにHTTPSリクエストを送信
- JSONレスポンスを解析
- 今日と明日の天気・気温を抽出
- 天気コードを解析（晴れ・曇り・雨・雪）

### 4. 画面表示

- 現在時刻（年月日 時分秒）
- 今日の天気と天気アイコン
- 今日の最高気温/最低気温
- 明日の天気と天気アイコン
- 最終更新時刻
- WiFi接続状態

## 📝 コードの主要なポイント

### NTP設定

```cpp
#define NTP_TIMEZONE "JST-9"  // 日本時間（UTC+9）
#define NTP_SERVER1  "0.pool.ntp.org"
#define NTP_SERVER2  "1.pool.ntp.org"
#define NTP_SERVER3  "2.pool.ntp.org"
```

### 気象庁APIのエンドポイント

```cpp
#define WEATHER_API_URL "https://www.jma.go.jp/bosai/forecast/data/forecast/130000.json"
```

### 天気情報構造体

```cpp
struct WeatherInfo {
    String weather;      // 天気（晴れ、曇り、雨、雪）
    int tempMax;         // 最高気温
    int tempMin;         // 最低気温
    int pop;             // 降水確率（%）
    WeatherCode code;    // 天気コード
};
```

### HTTPS通信と証明書検証

```cpp
WiFiClientSecure client;
client.setInsecure(); // SSL証明書の検証をスキップ（開発用）

HTTPClient http;
http.begin(client, WEATHER_API_URL);
http.setTimeout(10000);
int httpCode = http.GET();
```

**注意**: 本番環境では、適切なルートCA証明書を設定し、証明書検証を有効にすることを推奨します。

### JSON解析

```cpp
DynamicJsonDocument doc(16384); // 気象庁APIのレスポンスは大きいのでバッファを拡大
DeserializationError error = deserializeJson(doc, payload);
```

### 天気コードの解析

```cpp
WeatherCode parseWeatherCode(String weather) {
    weather.toLowerCase();
    if (weather.indexOf("晴") >= 0) return SUNNY;
    else if (weather.indexOf("雨") >= 0) return RAIN;
    else if (weather.indexOf("雪") >= 0) return SNOW;
    else return CLOUDY;
}
```

### 天気アイコンの描画

```cpp
void drawWeatherIcon(int x, int y, WeatherCode code) {
    // 晴れ: 黄色い円
    // 曇り: 灰色の雲
    // 雨: 灰色の雲 + 青い線
    // 雪: 灰色の雲 + 白い点
}
```

## 🌍 気象庁API仕様

### エンドポイント

```
https://www.jma.go.jp/bosai/forecast/data/forecast/{地域コード}.json
```

### レスポンス構造

```json
[
  {
    "timeSeries": [
      {
        "areas": [
          {
            "weathers": ["晴れ", "曇り時々雨"],
            ...
          }
        ]
      },
      ...
      {
        "areas": [
          {
            "temps": ["25", "18"]  // 最高気温、最低気温
          }
        ]
      }
    ]
  }
]
```

## 🔧 トラブルシューティング

### WiFi接続できない場合
- SSIDとパスワードが正しいか確認してください
- WiFiルーターが2.4GHz帯に対応しているか確認してください（5GHz帯は未対応）

### 時刻同期できない場合
- インターネット接続が利用可能か確認してください
- ファイアウォールがNTP通信（UDPポート123）をブロックしていないか確認してください

### 天気情報が取得できない場合
- インターネット接続が利用可能か確認してください
- 地域コードが正しいか確認してください
- 気象庁APIがメンテナンス中でないか確認してください

### JSON解析エラーが表示される場合
- `DynamicJsonDocument`のバッファサイズ（現在16384）を拡大してください
- 気象庁APIのレスポンス構造が変更されていないか確認してください

### 画面表示がおかしい場合
- 日本語フォント（`lgfxJapanMinchoP_16`）が正しく読み込まれているか確認してください
- M5Unifiedライブラリが最新版であることを確認してください

## 🎨 カスタマイズ例

### 更新間隔の変更

```cpp
const unsigned long WEATHER_UPDATE_INTERVAL = 1800000; // 30分ごとに更新
```

### タイムゾーンの変更

```cpp
#define NTP_TIMEZONE "UTC+0"  // UTC時刻
```

### 天気アイコンのサイズ変更

```cpp
void drawWeatherIcon(int x, int y, WeatherCode code) {
    int size = 80; // アイコンサイズを拡大
    // ...
}
```

### 別の地域の天気を表示

```cpp
// 大阪の天気を表示
#define WEATHER_API_URL "https://www.jma.go.jp/bosai/forecast/data/forecast/270000.json"
```

## 📖 参考資料

- [気象庁の天気予報APIを使う - ボクにもわかる地上デジタル](https://bokunimo.net/blog/esp/3426/)
- [気象庁 天気予報API](https://www.jma.go.jp/bosai/forecast/)
- [気象庁 地域コード一覧](https://www.jma.go.jp/bosai/common/const/area.json)
- [ArduinoJson ドキュメント](https://arduinojson.org/)
- [ESP32 SNTP Time Synchronization](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html)

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月6日  
**最終更新**: 2025年12月28日（ドキュメント作成）  
**対象デバイス**: M5Stack Tab5 (ESP32-P4)  
**動作確認**: ✅ 正常動作確認済み

