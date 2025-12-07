# M5Stack Tab5 Network プロジェクト（Wi-Fi / HTTPS / MQTT / Camera）

このリポジトリは、M5Stack Tab5 向けの **ネットワーク・クラウド連携サンプル** をまとめたものです。  
GUI / LVGL を中心としたサンプルは `Tab5_GUI`、センサーや表示など Arduino スケッチ単体のサンプルは `Tab5_Arduino` を参照してください。

---

## 📋 プロジェクト概要

- **プロジェクト名**: M5Stack Tab5 Network / Web & Cloud サンプル集  
- **対象デバイス**: M5Stack Tab5 (ESP32-P4)  
- **主な機能**:
  - Wi-Fi 接続・スキャン・アンテナテスト
  - HTTPS / REST API アクセス
  - MQTT 通信
  - Web サーバ / カメラ Web サーバ
  - ChatGPT Web API 連携 など

---

## 📁 リポジトリ構造

実ディレクトリ構成（簡略版）は以下の通りです。

```
Tab5_Network/
├── CameraWebServer/              # カメラ Web サーバ（ESP32-CAM 風デモ）
│   ├── CameraWebServer.ino
│   ├── app_httpd.cpp, camera_index.h など
│   ├── board_config.h, camera_pins.h
│   ├── partitions.csv
│   ├── ci.json
│   └── Readme.md
│
├── ChatGPT/                      # ChatGPT API 連携サンプル
│   ├── ChatGPT.ino
│   ├── Readme.md
│   ├── secrets.h.example         # API キー等のサンプル
│   └── secrets.h                 # ユーザー作成（コミットしない）
│
├── HttpsClient/                  # 汎用 HTTPS クライアント
│   ├── HttpsClient.ino
│   ├── Readme.md
│   ├── secrets.h.example
│   └── secrets.h
│
├── MQTT/                         # MQTT 通信サンプル
│   ├── MQTT.ino
│   ├── Readme.md
│   ├── secrets.h.example
│   └── secrets.h
│
├── WebAPI_Wether/                # 天気 Web API アクセス
│   ├── WebAPI_Wether.ino
│   ├── Readme.md
│   ├── secrets.h.example
│   └── secrets.h
│
├── WebServer/                    # シンプル Web サーバ
│   ├── webserver.ino
│   ├── Readme.md
│   ├── secrets.h.example
│   └── secrets.h                 # ユーザー作成（コミットしない）
│
├── Wifi/                         # 共通 Wi-Fi 設定（シークレット置き場）
│   └── secrets.h                 # SSID / パスワード等（ユーザー作成）
│
├── Wifi_Antenna/                 # Wi-Fi アンテナテスト
│   ├── Wifi_Antenna.ino
│   └── Readme.md
│
├── Wifi_Connect/                 # Wi-Fi 接続テスト（最小構成）
│   ├── WIFI00_Connect.ino
│   └── Readme.md
│
├── Wifi_Scan/                    # Wi-Fi スキャン
│   ├── WIFI01_Scan.ino
│   └── Readme.md
│
├── Docs/                         # 共通ドキュメント・スクリプト
│   ├── Arduino_Basic_Syntax_Guide.md
│   ├── GPIO_Application_Guide.md
│   ├── Manufacturing_Arduino_Samples.md
│   ├── USB_Application_Guide.md
│   ├── compile.sh                # ネットワーク系スケッチのビルド用
│   └── monitor.sh                # シリアルモニタスクリプト
│
├── LICENSE
└── Readme.md                     # このファイル
```

---

## 🚀 セットアップ

### 必要な環境

- Arduino IDE 2.x または Arduino CLI  
- ESP32-P4 ボードサポート（M5Stack Tab5）  
- M5Stack Tab5 デバイス  

### ボード設定（例）

- **Board**: ESP32P4 Dev Module  
- **USB CDC on boot**: Enabled  
- **Flash Size**: 16MB (128Mb)  
- **Partition Scheme**: Default または各サンプル付属の `partitions.csv`  
- **PSRAM**: Enabled  
- **Upload Mode**: UART / Hardware CDC  
- **USB Mode**: Hardware CDC and JTAG  

Arduino CLI での再現性のある設定例（共通）:

```bash
arduino-cli compile \
  --fqbn esp32:esp32:esp32p4:PSRAM=enabled,FlashSize=16M,PartitionScheme=default,CDCOnBoot=cdc,USBMode=hwcdc,UploadSpeed=921600 \
  HttpsClient/HttpsClient.ino

arduino-cli upload -p /dev/cu.usbmodem21201 \
  --fqbn esp32:esp32:esp32p4:PSRAM=enabled,FlashSize=16M,PartitionScheme=default,CDCOnBoot=cdc,USBMode=hwcdc,UploadSpeed=921600 \
  HttpsClient/HttpsClient.ino
```

※ `CameraWebServer` など一部サンプルは付属の `partitions.csv` を使用してください。

### ライブラリのインストール（ベース）

ネットワーク系サンプルでも、表示や基本制御には Tab5_Arduino と同じ M5 系ライブラリを利用します。

```bash
arduino-cli lib install "M5Unified@0.2.10"
arduino-cli lib install "M5GFX@0.2.15"
arduino-cli lib install "LovyanGFX@1.2.7"
```

MQTT や他のプロトコル用には、必要に応じて以下のようなライブラリを使用します（例）:

- `PubSubClient`（MQTT 用）
- `ArduinoJson`（JSON パース）

---

## 🔐 シークレットファイルの扱い

多くのサンプルで、Wi-Fi SSID / パスワードや API キーを `secrets.h` に分離しています。

- テンプレート: `secrets.h.example`
- 実ファイル: `secrets.h`（ユーザーがコピーして編集）

`secrets.h` は **Git にコミットしない** 想定です。API キーやパスワードを含めないよう注意してください。

---

## 🌐 代表的なサンプル

- **HttpsClient/HttpsClient.ino**
  - 任意の HTTPS サーバへ接続するクライアントサンプル
  - REST API や Web API アクセスの雛形として利用可能

- **MQTT/MQTT.ino**
  - MQTT ブローカーへの接続・購読・発行のサンプル
  - IoT デバイス間通信の基本パターンをカバー

- **WebAPI_Wether/WebAPI_Wether.ino**
  - 天気情報などの Web API にアクセスするサンプル
  - JSON 解析や API レスポンス表示の参考になります

- **WebServer/webserver.ino**
  - シンプルな HTTP Web サーバ
  - ブラウザから Tab5 にアクセスして状態を確認できます

- **CameraWebServer/CameraWebServer.ino**
  - カメラ映像をブラウザへ配信する Web サーバ
  - `app_httpd.cpp` などを用いたマルチパート JPEG 配信の例
  - ESP32カメラドライバを使用したカメラ制御

- **Wifi_Connect/WIFI00_Connect.ino**
  - 最小構成の Wi-Fi 接続テスト
  - HTTPS通信の例として、Yahoo NewsのRSSフィードを取得

- **Wifi_Scan/WIFI01_Scan.ino**
  - 周囲の Wi-Fi アクセスポイントをスキャンして一覧表示

- **Wifi_Antenna/Wifi_Antenna.ino**
  - Wi-Fi アンテナの感度・受信状態を確認するテスト

- **ChatGPT/ChatGPT.ino**
  - OpenAI / ChatGPT API を利用したシンプルなチャットクライアント例
  - プロンプト送信とレスポンス表示の基本フローを実装

---

## 📖 関連リポジトリ

- センサー / IMU / タッチ / 音声などの基本サンプル: `Tab5_Arduino`  
- GUI / LVGL / DrawingCameraApp などのフロントエンド UI: `Tab5_GUI`

3つのリポジトリを組み合わせることで、**デバイス制御 + GUI + ネットワーク** を一通りカバーできます。

---

## 📝 ライセンス

このプロジェクトは MIT ライセンスで公開されています。  
詳細は `LICENSE` ファイルを参照してください。

外部ライブラリ（M5Unified, M5GFX, LovyanGFX, PubSubClient, ArduinoJson など）は、それぞれの公式リポジトリのライセンスに従います。商用利用の際は各ライブラリのライセンスを必ず確認してください。

---

**作成日**: 2025年12月6日  
**対象デバイス**: M5Stack Tab5 (ESP32-P4)  
**開発環境**: Arduino IDE / Arduino CLI

