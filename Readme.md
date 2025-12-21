# M5Stack Tab5 Network プロジェクト（Wi-Fi / HTTPS / MQTT / Camera）

このリポジトリは、M5Stack Tab5 向けの **ネットワーク・クラウド連携サンプル** をまとめたものです。  
GUI / LVGL を中心としたサンプルは `Tab5_GUI`、センサーや表示など Arduino スケッチ単体のサンプルは `Tab5_Arduino` を参照してください。

---

## 📋 プロジェクト概要

- **プロジェクト名**: M5Stack Tab5 Network / Web & Cloud サンプル集  
- **対象デバイス**: M5Stack Tab5 (ESP32-P4)  
- **総プロジェクト数**: 10個
- **主な機能**:
  - Wi-Fi 接続・スキャン・アンテナテスト
  - HTTPS / REST API アクセス
  - MQTT 通信
  - SNTP（時刻同期）
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
│   └── secrets.h.example         # API キー等のサンプル
│
├── HttpsClient/                  # 汎用 HTTPS クライアント
│   ├── HttpsClient.ino
│   ├── Readme.md
│   └── secrets.h.example
│
├── MQTT/                         # MQTT 通信サンプル
│   ├── MQTT.ino
│   ├── Readme.md
│   └── secrets.h.example
│
├── SNTP/                         # SNTP（時刻同期）サンプル
│   ├── SNTP.ino
│   ├── Readme.md
│   └── secrets.h.example
│
├── WebAPI_Wether/                # 天気 Web API アクセス
│   ├── WebAPI_Wether.ino
│   ├── Readme.md
│   └── secrets.h.example
│
├── WebServer/                    # シンプル Web サーバ
│   ├── webserver.ino
│   ├── Readme.md
│   └── secrets.h.example
│
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

---

## 📊 プロジェクト一覧（全10個）

### Wi-Fi基本機能（3個）
| No. | プログラム名 | 状態 | 主要機能 |
|-----|-------------|------|----------|
| 1 | Wifi_Connect | ✅ 完了 | Wi-Fi接続テスト、Yahoo News RSS取得 |
| 2 | Wifi_Scan | ✅ 完了 | Wi-Fiアクセスポイントスキャン |
| 3 | Wifi_Antenna | ✅ 完了 | Wi-Fiアンテナ感度テスト |

### Web/API通信（4個）
| No. | プログラム名 | 状態 | 主要機能 |
|-----|-------------|------|----------|
| 4 | HttpsClient | ✅ 完了 | 汎用HTTPSクライアント |
| 5 | WebAPI_Wether | ✅ 完了 | 天気情報API取得 |
| 6 | WebServer | ✅ 完了 | シンプルWebサーバー |
| 7 | ChatGPT | ✅ 完了 | ChatGPT API連携 |

### IoT通信（2個）
| No. | プログラム名 | 状態 | 主要機能 |
|-----|-------------|------|----------|
| 8 | MQTT | ✅ 完了 | MQTTブローカー接続・Pub/Sub |
| 9 | SNTP | ✅ 完了 | 時刻同期（NTPサーバー） |

### カメラ・マルチメディア（1個）
| No. | プログラム名 | 状態 | 主要機能 |
|-----|-------------|------|----------|
| 10 | CameraWebServer | ✅ 完了 | カメラ映像Webストリーミング |

---

## 🌐 代表的なサンプル

- **HttpsClient/HttpsClient.ino**
  - 任意の HTTPS サーバへ接続するクライアントサンプル
  - REST API や Web API アクセスの雛形として利用可能

- **MQTT/MQTT.ino**
  - MQTT ブローカーへの接続・購読・発行のサンプル
  - IoT デバイス間通信の基本パターンをカバー

- **SNTP/SNTP.ino**
  - SNTP（Simple Network Time Protocol）による時刻同期サンプル
  - NTPサーバーから正確な時刻を取得してRTCに設定
  - タイムゾーン設定とサマータイム対応

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

## 📊 プロジェクト統計

- **総プログラム数**: 10個
- **ドキュメント数**: 11個（README.md + 各プロジェクトReadme.md）
- **動作確認率**: 100%
- **カテゴリ数**: 4カテゴリ
  - Wi-Fi基本機能: 3個
  - Web/API通信: 4個
  - IoT通信: 2個
  - カメラ・マルチメディア: 1個

### 🎓 教育価値

このプロジェクトは、M5Stack Tab5でのネットワーク・クラウド連携開発の包括的なリファレンスとして、以下の用途で活用できます：

1. **教育・学習用途**
   - IoTデバイスのネットワーク通信学習
   - REST API / Web API 連携の実践例
   - MQTT / SNTP などのプロトコル学習

2. **研究・開発用途**
   - プロトタイプ開発の基盤
   - クラウドサービス連携の参考実装
   - カメラストリーミングのベースコード

3. **商用用途**
   - 製品開発の参考実装
   - API連携の技術検証
   - カスタマイズ可能なソリューション

---

## 🔗 参考リンク集

このプロジェクトのサンプルは、以下の資料やWebサイトを参考に作成されています。

### 公式ドキュメント
- [M5Stack Tab5 公式ドキュメント](https://docs.m5stack.com/)
- [M5Stack Core2 公式ドキュメント](https://docs.m5stack.com/en/core/core2)
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/en/latest/)

### ライブラリ
- [M5Unified GitHub](https://github.com/m5stack/M5Unified)
- [M5GFX GitHub](https://github.com/m5stack/M5GFX)
- [PubSubClient (MQTT) GitHub](https://github.com/knolleary/pubsubclient)
- [ArduinoJson GitHub](https://github.com/bblanchon/ArduinoJson)

### サンプル別参考リンク

#### ChatGPT
- [M5StackでChatGPTを動かす - Device Plus](https://deviceplus.jp/mc-general/m5stack-chatgpt-01/)
- [OpenAI API Documentation](https://platform.openai.com/docs/api-reference)

#### MQTT
- [M5Stack Core2 MQTT Example](https://docs.m5stack.com/en/core/core2)
- [MQTT.org](https://mqtt.org/)

#### WebAPI_Wether
- [気象庁の天気予報APIを使う - ボクにもわかる地上デジタル](https://bokunimo.net/blog/esp/3426/)
- [気象庁 天気予報API](https://www.jma.go.jp/bosai/forecast/)

#### SNTP
- [ESP32 SNTP Time Synchronization](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html)

---

## 📝 ライセンス

このプロジェクトは MIT ライセンスで公開されています。  
詳細は `LICENSE` ファイルを参照してください。

外部ライブラリ（M5Unified, M5GFX, LovyanGFX, PubSubClient, ArduinoJson など）は、それぞれの公式リポジトリのライセンスに従います。商用利用の際は各ライブラリのライセンスを必ず確認してください。

---

**作成日**: 2025年12月6日  
**最終更新**: 2025年12月21日（参考リンク集を追加）  
**対象デバイス**: M5Stack Tab5 (ESP32-P4)  
**開発環境**: Arduino IDE / Arduino CLI  
**動作確認**: ✅ 全10プロジェクト正常動作確認済み

