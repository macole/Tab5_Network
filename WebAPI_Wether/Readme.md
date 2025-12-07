# WebAPI_Wether - 天気予報表示アプリ

M5Stack Tab5で気象庁の天気予報APIから天気情報を取得して表示するアプリケーションです。

## 📋 概要

このプログラムは、気象庁の天気予報API（[気象庁防災情報XML](https://www.data.jma.go.jp/developer/xml/feed/regular.xml)）にアクセスし、天気情報を取得して画面に表示します。NTPで時刻を同期し、1時間ごとに自動的に天気情報を更新します。

参考: [M5Stack で 天気アイコンを表示する](https://bokunimo.net/blog/esp/3426/)

## 🔧 必要なハードウェア

- M5Stack Tab5

## 📚 必要なライブラリ

- **M5Unified**: M5Stack統合ライブラリ
  ```bash
  arduino-cli lib install "M5Unified@0.2.10"
  ```

- **ArduinoJson**: JSONパースライブラリ
  ```bash
  arduino-cli lib install "ArduinoJson@6.21.3"
  ```

- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **HTTPClient**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **WiFiClientSecure**: ESP32標準ライブラリ（Arduino IDEに含まれています）

## 🚀 セットアップ

1. **ボード設定**
   - Board: ESP32P4 Dev Module
   - USB CDC on boot: Enabled
   - Flash Size: 16MB (128Mb)
   - Partition Scheme: Custom
   - PSRAM: Enabled

2. **WiFi設定**
   - `secrets.h.example`をコピーして`secrets.h`を作成してください：
   ```bash
   cp secrets.h.example secrets.h
   ```
   - `secrets.h`ファイル内の`ssid`と`password`を実際の値に編集してください：
   ```cpp
   const char *ssid = "YOUR_WIFI_SSID";
   const char *password = "YOUR_WIFI_PASSWORD";
   ```
   - **重要**: `secrets.h`は`.gitignore`に追加されているため、Gitにコミットされません。

3. **地域コードの設定（オプション）**
   - デフォルトでは東京（130000）の天気情報を取得します
   - 他の地域の天気情報を取得する場合は、`WebAPI_Wether.ino`の`WEATHER_API_URL`を変更してください：
   ```cpp
   // 例: 大阪の地域コード270000
   #define WEATHER_API_URL "https://www.jma.go.jp/bosai/forecast/data/forecast/270000.json"
   ```
   - 地域コードは[気象庁のサイト](https://www.jma.go.jp/bosai/common/const/area.json)で確認できます

4. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

1. WiFiのSSIDとパスワードを設定
2. プログラムをアップロード
3. 起動すると自動的に：
   - WiFiに接続
   - NTPで時刻を同期
   - 気象庁APIから天気情報を取得
   - 画面に天気情報を表示

## 📊 機能

- **NTP時刻同期**: インターネットから正確な時刻を取得
- **天気情報取得**: 気象庁APIから今日と明日の天気情報を取得
- **天気アイコン表示**: 晴れ、曇り、雨、雪の4種類のアイコンを表示
- **気温表示**: 最高気温と最低気温を表示
- **自動更新**: 1時間ごとに天気情報を自動更新

## 🔍 プログラムの動作

### 1. 初期化
- M5Stackの初期化
- WiFi接続
- NTP時刻同期
- RTCに時刻を設定
- 初回天気情報取得

### 2. メインループ
- 1時間ごとに天気情報を更新
- 1秒ごとに画面表示を更新

### 3. 天気情報取得
- HTTPSで気象庁APIにアクセス
- JSONレスポンスをパース
- 天気、気温情報を抽出

### 4. 画面表示
- 現在時刻
- 今日の天気とアイコン
- 最高気温/最低気温
- 明日の天気とアイコン
- 最終更新時刻

## 📝 コードの主要なポイント

### 気象庁APIへのアクセス
```cpp
WiFiClientSecure client;
client.setInsecure(); // SSL証明書の検証をスキップ（開発用）

HTTPClient http;
http.begin(client, WEATHER_API_URL);
int httpCode = http.GET();
```

**注意**: `setInsecure()`は開発用です。本番環境では適切なSSL証明書を設定してください。

### JSONパース
```cpp
DynamicJsonDocument doc(16384);
DeserializationError error = deserializeJson(doc, payload);
```

気象庁APIのレスポンスは大きいため、JSONバッファサイズを16384バイトに設定しています。

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

天気文字列から天気コードを判定します。

### 天気アイコンの描画
```cpp
void drawWeatherIcon(int x, int y, WeatherCode code) {
    switch (code) {
        case SUNNY: // 黄色い円
        case CLOUDY: // 灰色の雲
        case RAIN: // 灰色の雲 + 青い線
        case SNOW: // 灰色の雲 + 白い点
    }
}
```

天気コードに応じてアイコンを描画します。

## ⚙️ カスタマイズ

### 更新間隔の変更
```cpp
const unsigned long WEATHER_UPDATE_INTERVAL = 3600000; // 1時間（ミリ秒）
```

### 表示フォントの変更
```cpp
M5.Display.setFont(&fonts::lgfxJapanMinchoP_16); // 16ptフォント
```

### タイムゾーンの変更
```cpp
#define NTP_TIMEZONE "JST-9" // 日本標準時
```

## ⚠️ トラブルシューティング

### 天気情報が取得できない
- WiFi接続を確認してください
- シリアルモニターでHTTPレスポンスコードを確認してください
- 気象庁APIのURLが正しいか確認してください

### JSONパースエラー
- シリアルモニターでJSONレスポンスを確認してください
- JSONバッファサイズを増やしてみてください（`DynamicJsonDocument doc(32768)`）

### SSL証明書エラー
- 開発環境では`setInsecure()`を使用しています
- 本番環境では適切なルート証明書を設定してください

### 時刻が正しく表示されない
- NTPサーバーへの接続を確認してください
- タイムゾーン設定を確認してください

## 📖 参考資料

- [気象庁防災情報XML](https://www.data.jma.go.jp/developer/xml/feed/regular.xml)
- [気象庁天気予報API](https://www.jma.go.jp/bosai/forecast/)
- [M5Stack で 天気アイコンを表示する](https://bokunimo.net/blog/esp/3426/)
- [ArduinoJson ライブラリ](https://arduinojson.org/)

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月6日  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み
