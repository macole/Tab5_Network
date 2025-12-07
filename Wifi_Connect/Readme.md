# Wifi_Connect - WiFi接続テスト（最小構成）

M5Stack Tab5でWiFiに接続する最小構成のサンプルプログラムです。

## 📋 概要

このプログラムは、M5Stack Tab5のWiFi機能を使用して、WiFiアクセスポイントに接続する最小構成のサンプルです。HTTPS通信の例として、Yahoo NewsのRSSフィードを取得して表示します。

## 🔧 必要なハードウェア

- M5Stack Tab5

## 📚 必要なライブラリ

- **M5Unified**: M5Stack統合ライブラリ
  ```bash
  arduino-cli lib install "M5Unified@0.2.10"
  ```
- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **HTTPClient**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **WiFiClientSecure**: ESP32標準ライブラリ（Arduino IDEに含まれています）

## 🚀 セットアップ

1. **ボード設定**
   - Board: ESP32P4 Dev Module
   - USB CDC on boot: Enabled
   - Flash Size: 16MB (128Mb)
   - Partition Scheme: Default
   - PSRAM: Enabled

2. **WiFi設定**
   - `WIFI00_Connect.ino`内の`ssid`と`pass`を実際の値に編集してください：
   ```cpp
   const char *ssid = "YOUR_WIFI_SSID";
   const char *pass = "YOUR_WIFI_PASSWORD";
   ```
   - **重要**: ハードコードされた認証情報は本番環境では使用しないでください。`secrets.h`を使用することを推奨します。

3. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

1. WiFiのSSIDとパスワードを設定
2. プログラムをアップロード
3. 起動すると自動的に：
   - WiFiに接続
   - NTPで時刻を同期
   - Yahoo NewsのRSSフィードを取得
   - 画面とシリアルモニターに結果を表示

## 📊 機能

- **WiFi接続**: Tab5のSDIO2ピンを使用したWiFi接続
- **NTP時刻同期**: インターネットから正確な時刻を取得
- **HTTPS通信**: SSL/TLSを使用したセキュアな通信
- **RSS取得**: Yahoo NewsのRSSフィードを取得
- **画面表示**: 接続状態と結果を画面に表示

## 🔍 プログラムの動作

### 1. 初期化
- M5Stackの初期化
- シリアルポートの初期化
- ディスプレイの初期化

### 2. WiFi接続
- WiFiアクセスポイントに接続
- IPアドレスを取得
- 接続状態を画面に表示

### 3. 時刻同期
- NTPサーバーから時刻を取得
- システム時刻を設定

### 4. HTTPS通信
- ルートCA証明書を設定
- Yahoo NewsのRSSフィードにHTTPS接続
- XMLデータを取得
- シリアルモニターに出力

## 📝 コードの主要なポイント

### WiFi接続
```cpp
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print(".");
}
```

### NTP時刻同期
```cpp
void setClock() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    time_t nowSecs = time(nullptr);
    while (nowSecs < 8 * 3600 * 2) {
        delay(500);
        yield();
        nowSecs = time(nullptr);
    }
}
```

### HTTPS通信
```cpp
WiFiClientSecure *client = new WiFiClientSecure;
client->setCACert(rootCA);

HTTPClient https;
https.begin(*client, "https://news.yahoo.co.jp/pickup/rss.xml");
int status = https.GET();
```

## ⚙️ カスタマイズ

### 別のHTTPSエンドポイントに接続
```cpp
https.begin(*client, "https://example.com/api/endpoint");
```

### ルートCA証明書の変更
```cpp
const char* rootCA = "-----BEGIN CERTIFICATE-----\n" \
                      "...証明書データ...\n" \
                      "-----END CERTIFICATE-----\n";
```

### secrets.hの使用（推奨）
```cpp
#include "secrets.h"

const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASSWORD;
```

## ⚠️ トラブルシューティング

### WiFiに接続できない
- SSIDとパスワードが正しいか確認してください
- WiFiアクセスポイントが動作しているか確認してください
- シリアルモニターでエラーメッセージを確認してください

### HTTPS接続エラー
- ルートCA証明書が正しいか確認してください
- インターネット接続を確認してください
- シリアルモニターでHTTPステータスコードを確認してください

### 時刻が正しく設定されない
- NTPサーバーへの接続を確認してください
- ファイアウォールでNTPポート（123）がブロックされていないか確認してください

## 📖 参考資料

- [ESP32 WiFi Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)
- [ESP32 HTTPS Client Example](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient/examples)

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月6日  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み

