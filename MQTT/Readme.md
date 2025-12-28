# MQTT - MQTT通信クライアント

M5Stack Tab5のWiFi機能を使用して、MQTTブローカーとの通信を行うプログラムです。

## 📋 概要

このプログラムは、M5Stack Tab5のWiFi機能を使用して、MQTTブローカーに接続し、メッセージの送受信を行います。IoTデバイス間の通信や、センサーデータの送信、リモート制御などに活用できます。

## 🔧 必要なハードウェア

- M5Stack Tab5
- MQTTブローカー（Mosquitto、HiveMQ、AWS IoT Coreなど）

## 📚 必要なライブラリ

- **M5Unified**: M5Stack統合ライブラリ
  ```bash
  arduino-cli lib install "M5Unified@0.2.10"
  ```
- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **PubSubClient**: MQTTクライアントライブラリ
  ```bash
  arduino-cli lib install "PubSubClient"
  ```

## 🚀 セットアップ

1. **ボード設定**
   - Board: ESP32P4 Dev Module
   - USB CDC on boot: Enabled
   - Flash Size: 16MB (128Mb)
   - Partition Scheme: Default
   - PSRAM: Enabled

2. **WiFi設定とMQTTサーバー設定**
   - `secrets.h.example`をコピーして`secrets.h`を作成してください：
   ```bash
   cp secrets.h.example secrets.h
   ```
   - `secrets.h`ファイル内の設定を編集してください：
   ```cpp
   // WiFi設定
   const char *ssid = "YOUR_WIFI_SSID";
   const char *password = "YOUR_WIFI_PASSWORD";
   
   // MQTTブローカー設定
   const char *mqtt_server = "broker.hivemq.com";  // または自分のMQTTブローカー
   ```
   - **重要**: `secrets.h`は`.gitignore`に追加されているため、Gitにコミットされません。

3. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

1. `secrets.h`にWiFiのSSID、パスワード、MQTTブローカーのアドレスを設定
2. プログラムをアップロード
3. デバイスがWiFiネットワークに接続し、MQTTブローカーに接続します
4. 2秒ごとに"hello world #N"というメッセージを`M5Stack`トピックに発行します
5. 同じトピックを購読しているため、自分が送信したメッセージも受信します

## 📊 機能

- **MQTT接続**: MQTTブローカーへの自動接続と再接続
- **メッセージ発行**: 指定したトピックにメッセージを発行（Publish）
- **メッセージ購読**: 指定したトピックのメッセージを購読（Subscribe）
- **自動再接続**: 接続が切れた場合の自動再接続機能
- **画面表示**: 送受信したメッセージをディスプレイに表示

## 🔍 プログラムの動作

1. **初期化**
   - M5Stackの初期化
   - WiFi接続
   - MQTTブローカーへの接続

2. **メッセージ発行**
   - 2秒ごとにカウンター付きメッセージを発行
   - トピック: `M5Stack`
   - メッセージ形式: `hello world #N`

3. **メッセージ受信**
   - 購読しているトピックからメッセージを受信
   - 受信したメッセージをディスプレイに表示

## 📝 コードの主要なポイント

### MQTTクライアントの初期化
```cpp
WiFiClient espClient;
PubSubClient client(espClient);

// MQTTブローカーの設定
client.setServer(mqtt_server, 1883);
client.setCallback(callback);  // メッセージ受信時のコールバック
```

### メッセージの発行
```cpp
client.publish("M5Stack", "hello world");
```

### トピックの購読
```cpp
client.subscribe("M5Stack");
```

### メッセージ受信時のコールバック
```cpp
void callback(char* topic, byte* payload, unsigned int length) {
    M5.Display.print("Message arrived [");
    M5.Display.print(topic);
    M5.Display.print("] ");
    for (int i = 0; i < length; i++) {
        M5.Display.print((char)payload[i]);
    }
    M5.Display.println();
}
```

## 🌐 MQTTブローカーの例

### 無料の公開ブローカー（テスト用）
- **HiveMQ**: `broker.hivemq.com`
- **Eclipse Mosquitto**: `test.mosquitto.org`
- **EMQX**: `broker.emqx.io`

### プライベートブローカー
- **Mosquitto**: 自分のサーバーにインストール
- **AWS IoT Core**: AWSのマネージドMQTTサービス
- **Azure IoT Hub**: AzureのIoTサービス

⚠️ **注意**: 公開ブローカーは誰でもアクセスできるため、本番環境では使用しないでください。

## 🔧 トラブルシューティング

### MQTTブローカーに接続できない場合
- WiFiのSSIDとパスワードが正しいか確認してください
- MQTTブローカーのアドレスが正しいか確認してください
- MQTTブローカーがポート1883で待ち受けているか確認してください
- ファイアウォールでポート1883がブロックされていないか確認してください

### メッセージが受信できない場合
- トピック名が正しいか確認してください
- 購読（subscribe）が正しく設定されているか確認してください
- MQTTブローカーの動作を確認してください

### 接続が頻繁に切れる場合
- WiFiの電波強度を確認してください
- MQTTブローカーの負荷を確認してください
- Keep-Alive時間を調整してください

## 📌 カスタマイズ例

### トピックの変更
```cpp
// 発行
client.publish("your/topic/name", msg);

// 購読
client.subscribe("your/topic/name");
```

### 発行間隔の変更
```cpp
if (now - lastMsg > 5000) {  // 5秒ごとに変更
    // メッセージ発行
}
```

### 複数トピックの購読
```cpp
client.subscribe("topic1");
client.subscribe("topic2");
client.subscribe("topic3");
```

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

元コード Copyright (c) 2021 by M5Stack  
改変版 Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2021年11月5日（M5Stack Core2サンプル）  
**改変日**: 2025年12月28日（M5Unified対応、コメント日本語化）  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み

