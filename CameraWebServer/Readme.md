# CameraWebServer - カメラ Web サーバー

M5Stack Tab5でカメラ映像をブラウザに配信するWebサーバーアプリケーションです。

## 📋 概要

このプログラムは、ESP32のカメラ機能を使用して、カメラ映像をブラウザに配信するWebサーバーです。ESP32-CAM風のデモとして、マルチパートJPEGストリーミングを実装しています。

## 🔧 必要なハードウェア

- M5Stack Tab5
- 対応するカメラモジュール（`board_config.h`で設定）

## 📚 必要なライブラリ

- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **esp_camera**: ESP32標準ライブラリ（Arduino IDEに含まれています）

## 🚀 セットアップ

1. **ボード設定**
   - Board: ESP32P4 Dev Module
   - USB CDC on boot: Enabled
   - Flash Size: 16MB (128Mb)
   - Partition Scheme: Custom（`partitions.csv`を使用）
   - PSRAM: Enabled

2. **カメラモデルの選択**
   - `board_config.h`で使用するカメラモデルを選択してください
   - 対応するカメラピン設定が`camera_pins.h`に定義されています

3. **WiFi設定**
   - `CameraWebServer.ino`内の`ssid`と`password`を実際の値に編集してください：
   ```cpp
   const char *ssid = "YOUR_WIFI_SSID";
   const char *password = "YOUR_WIFI_PASSWORD";
   ```

4. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

1. WiFiのSSIDとパスワードを設定
2. カメラモデルを選択
3. プログラムをアップロード
4. シリアルモニターで表示されたIPアドレスにブラウザでアクセス
   - 例: `http://192.168.1.100`
5. ブラウザでカメラ映像が表示されます

## 📊 機能

- **カメラ初期化**: ESP32カメラドライバを使用したカメラ初期化
- **WiFi接続**: WiFi経由でのネットワーク接続
- **Webサーバー**: HTTPサーバーでカメラ映像を配信
- **マルチパートJPEG**: ストリーミング用のマルチパートJPEG配信
- **カメラ設定**: フレームサイズ、JPEG品質などの設定

## 🔍 プログラムの動作

### 1. 初期化
- シリアルポートの初期化
- カメラ設定の読み込み（`board_config.h`から）
- カメラの初期化
- センサー設定（明るさ、彩度、フリップなど）

### 2. WiFi接続
- WiFiアクセスポイントに接続
- IPアドレスを取得

### 3. Webサーバー起動
- `app_httpd.cpp`で定義されたHTTPサーバーを起動
- カメラ映像を配信するエンドポイントを設定

### 4. カメラ映像配信
- ブラウザからのリクエストに応じて、カメラ映像をJPEG形式で配信
- マルチパート形式でストリーミング

## 📝 コードの主要なポイント

### カメラ設定
```cpp
camera_config_t config;
config.frame_size = FRAMESIZE_UXGA;
config.pixel_format = PIXFORMAT_JPEG;
config.fb_location = CAMERA_FB_IN_PSRAM;
config.jpeg_quality = 12;
```

### カメラ初期化
```cpp
esp_err_t err = esp_camera_init(&config);
if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
}
```

### Webサーバー起動
```cpp
startCameraServer();
```

`app_httpd.cpp`でHTTPサーバーとエンドポイントが定義されています。

## ⚙️ カスタマイズ

### フレームサイズの変更
```cpp
config.frame_size = FRAMESIZE_QVGA;  // 320x240
// または
config.frame_size = FRAMESIZE_VGA;   // 640x480
// または
config.frame_size = FRAMESIZE_UXGA;  // 1600x1200
```

### JPEG品質の変更
```cpp
config.jpeg_quality = 10;  // 0-63（小さいほど高品質）
```

### ピクセルフォーマットの変更
```cpp
config.pixel_format = PIXFORMAT_JPEG;  // ストリーミング用
// または
config.pixel_format = PIXFORMAT_RGB565; // 顔認識用
```

## ⚠️ トラブルシューティング

### カメラが初期化できない
- カメラモジュールが正しく接続されているか確認してください
- `board_config.h`で正しいカメラモデルが選択されているか確認してください
- カメラピン設定（`camera_pins.h`）が正しいか確認してください

### WiFiに接続できない
- SSIDとパスワードが正しいか確認してください
- シリアルモニターでエラーメッセージを確認してください

### 映像が表示されない
- ブラウザで正しいIPアドレスにアクセスしているか確認してください
- シリアルモニターでHTTPリクエストが来ているか確認してください
- カメラが正常に動作しているか確認してください

### メモリ不足エラー
- PSRAMが有効になっているか確認してください
- フレームサイズを小さくしてみてください（例: `FRAMESIZE_QVGA`）
- JPEG品質を下げてみてください

## 📖 参考資料

- [ESP32 Camera Driver](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/camera.html)
- [ESP32-CAM Web Server Example](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer)

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月6日  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み

