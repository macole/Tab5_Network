# Wifi_Scan - WiFiスキャン

M5Stack Tab5で周囲のWiFiアクセスポイントをスキャンして一覧表示するプログラムです。

## 📋 概要

このプログラムは、M5Stack Tab5のWiFi機能を使用して、周囲のWiFiアクセスポイントをスキャンし、SSID、RSSI（信号強度）、チャンネル、暗号化方式などの情報を画面に表示します。

## 🔧 必要なハードウェア

- M5Stack Tab5

## 📚 必要なライブラリ

- **M5Unified**: M5Stack統合ライブラリ
  ```bash
  arduino-cli lib install "M5Unified@0.2.10"
  ```
- **M5GFX**: M5Stackグラフィックスライブラリ
  ```bash
  arduino-cli lib install "M5GFX@0.2.15"
  ```
- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）

## 🚀 セットアップ

1. **ボード設定**
   - Board: ESP32P4 Dev Module
   - USB CDC on boot: Enabled
   - Flash Size: 16MB (128Mb)
   - Partition Scheme: Default
   - PSRAM: Enabled

2. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード
   - **注意**: WiFi接続は不要です。スキャン機能のみを使用します。

## 💻 使用方法

1. プログラムをアップロード
2. 起動すると自動的に：
   - WiFiスキャンを開始
   - 検出されたアクセスポイントの情報を画面に表示
   - 5秒ごとに再スキャン

## 📊 機能

- **WiFiスキャン**: 周囲のWiFiアクセスポイントを検出
- **情報表示**: SSID、RSSI、チャンネル、暗号化方式を表示
- **自動再スキャン**: 5秒ごとに自動的に再スキャン
- **SDIO2ピン設定**: Tab5のSDIO2ピンを使用したWiFi制御

## 🔍 プログラムの動作

### 1. 初期化
- M5Stackの初期化
- シリアルポートの初期化
- ディスプレイフォントの設定
- WiFiピンの設定（SDIO2）

### 2. WiFiスキャン
- `WiFi.scanNetworks()`でアクセスポイントをスキャン
- 検出されたネットワーク数を表示

### 3. 情報表示
- 各アクセスポイントの情報を表示：
  - 番号
  - SSID（最大32文字）
  - RSSI（信号強度、dBm）
  - チャンネル
  - 暗号化方式（open, WEP, WPA, WPA2, WPA3など）

### 4. 再スキャン
- スキャン結果をクリア（`WiFi.scanDelete()`）
- 5秒待機後、再スキャン

## 📝 コードの主要なポイント

### WiFiピンの設定
```cpp
#define SDIO2_CLK GPIO_NUM_12
#define SDIO2_CMD GPIO_NUM_13
#define SDIO2_D0  GPIO_NUM_11
#define SDIO2_D1  GPIO_NUM_10
#define SDIO2_D2  GPIO_NUM_9
#define SDIO2_D3  GPIO_NUM_8
#define SDIO2_RST GPIO_NUM_15

WiFi.setPins(SDIO2_CLK, SDIO2_CMD, SDIO2_D0, SDIO2_D1, SDIO2_D2, SDIO2_D3, SDIO2_RST);
```

Tab5では、Arduino IDEでM5Tab5ボードを選択すると、デフォルトピンが定義されているため、以下のように簡略化できます：

```cpp
WiFi.setPins(BOARD_SDIO_ESP_HOSTED_CLK, BOARD_SDIO_ESP_HOSTED_CMD, BOARD_SDIO_ESP_HOSTED_D0,
             BOARD_SDIO_ESP_HOSTED_D1, BOARD_SDIO_ESP_HOSTED_D2, BOARD_SDIO_ESP_HOSTED_D3,
             BOARD_SDIO_ESP_HOSTED_RESET);
```

### WiFiスキャン
```cpp
int n = WiFi.scanNetworks();
```

### アクセスポイント情報の取得
```cpp
WiFi.SSID(i);              // SSID
WiFi.RSSI(i);              // 信号強度（dBm）
WiFi.channel(i);           // チャンネル
WiFi.encryptionType(i);    // 暗号化方式
```

### 暗号化方式の判定
```cpp
switch (WiFi.encryptionType(i)) {
    case WIFI_AUTH_OPEN:
        M5.Display.print("open");
        break;
    case WIFI_AUTH_WEP:
        M5.Display.print("WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        M5.Display.print("WPA");
        break;
    case WIFI_AUTH_WPA2_PSK:
        M5.Display.print("WPA2");
        break;
    // ... その他の暗号化方式
}
```

## ⚙️ カスタマイズ

### スキャン間隔の変更
```cpp
delay(5000);  // 5秒 → 他の値に変更
```

### スキャンモードの変更
```cpp
WiFi.scanNetworks(true);  // 非同期スキャン
// または
WiFi.scanNetworks(false, true);  // 非表示ネットワークもスキャン
```

### 表示フォントの変更
```cpp
M5.Display.setFont(&fonts::FreeMonoBoldOblique9pt7b);
// 他のフォントに変更可能
```

### RSSIによるフィルタリング
```cpp
if (WiFi.RSSI(i) > -70) {  // -70dBm以上の信号のみ表示
    // 表示処理
}
```

## ⚠️ トラブルシューティング

### ネットワークが検出されない
- WiFiピンの設定が正しいか確認してください
- 周囲にWiFiアクセスポイントが存在するか確認してください
- シリアルモニターでエラーメッセージを確認してください

### 表示が乱れる
- ディスプレイのフォントサイズを調整してください
- 表示するネットワーク数を制限してください

### スキャンが遅い
- スキャン間隔を長くしてください
- 非同期スキャンを使用してください

## 📖 参考資料

- [ESP32 WiFi Scan Example](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples/WiFiScan)
- [ESP32 WiFi API Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月6日  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み

