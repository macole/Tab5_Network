# ChatGPT - ChatGPT通信アプリ

M5Stack Tab5でOpenAIのChatGPT APIと通信して対話するアプリケーションです。

## 📋 概要

このプログラムは、OpenAIのChatGPT APIを使用して、M5Stack Tab5からChatGPTと対話できるアプリケーションです。タッチスクリーンでプロンプトを選択し、ChatGPTからの応答を画面に表示します。

参考: [M5StackとChatGPTで作るAIベースのストーリーテリング装置【前編】](https://deviceplus.jp/mc-general/m5stack-chatgpt-01/)

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

2. **OpenAI APIキーの取得**
   - [OpenAI Platform](https://platform.openai.com/api-keys)にアクセス
   - アカウントを作成（またはログイン）
   - "Create new secret key"をクリックしてAPIキーを生成
   - 生成されたAPIキーをコピー（再表示できないため注意）

3. **WiFi設定とAPIキー設定**
   - `secrets.h.example`をコピーして`secrets.h`を作成してください：
   ```bash
   cp secrets.h.example secrets.h
   ```
   - `secrets.h`ファイル内の`ssid`、`password`、`openaiApiKey`を実際の値に編集してください：
   ```cpp
   const char *ssid = "YOUR_WIFI_SSID";
   const char *password = "YOUR_WIFI_PASSWORD";
   const char *openaiApiKey = "sk-..."; // OpenAI APIキー
   ```
   - **重要**: `secrets.h`は`.gitignore`に追加されているため、Gitにコミットされません。

4. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

### 基本操作

1. WiFiのSSID、パスワード、OpenAI APIキーを設定
2. プログラムをアップロード
3. 起動すると自動的にWiFiに接続
4. 画面をタッチして操作：

| 操作エリア | 動作 |
|-----------|------|
| **左上** | 前のプロンプトに切り替え |
| **右上** | 次のプロンプトに切り替え |
| **左下** | 選択したプロンプトをChatGPTに送信 |
| **右下** | 画面をクリア |

### プロンプト

デフォルトで以下の5つのプロンプトが用意されています：

1. "面白い短い話をしてください"
2. "今日の天気について教えてください"
3. "簡単ななぞなぞを出してください"
4. "今日の運勢を教えてください"
5. "短い詩を作ってください"

## 📊 機能

- **WiFi接続**: Tab5のSDIO2ピンを使用したWiFi接続
- **ChatGPT API通信**: OpenAIのChatGPT APIとHTTPS通信
- **タッチ操作**: 画面を4分割して直感的な操作
- **プロンプト選択**: 5つのプリセットプロンプトから選択
- **レスポンス表示**: ChatGPTからの応答を画面に表示
- **エラーハンドリング**: HTTPエラーやJSONパースエラーを処理

## 🔍 プログラムの動作

### 1. 初期化
- M5Stackの初期化
- WiFi接続
- ウェルカム画面の表示

### 2. メインループ
- タッチ操作の検出
- 画面更新（1秒ごと）

### 3. ChatGPT APIリクエスト
- HTTPSでOpenAI APIにアクセス
- JSONリクエストボディを作成
- レスポンスをJSONパース
- 画面に表示

### 4. 画面表示
- プロンプト選択画面
- ChatGPTからの応答表示
- エラーメッセージ表示
- WiFi接続状態表示

## 📝 コードの主要なポイント

### OpenAI APIへのアクセス
```cpp
WiFiClientSecure client;
client.setInsecure(); // SSL証明書の検証をスキップ（開発用）

HTTPClient http;
http.begin(client, OPENAI_API_URL);
http.addHeader("Authorization", String("Bearer ") + openaiApiKey);
```

**注意**: `setInsecure()`は開発用です。本番環境では適切なSSL証明書を設定してください。

### JSONリクエストの作成
```cpp
DynamicJsonDocument doc(2048);
doc["model"] = MODEL;
JsonArray messages = doc.createNestedArray("messages");
JsonObject message = messages.createNestedObject();
message["role"] = "user";
message["content"] = prompt;
```

ChatGPT APIのリクエスト形式に合わせてJSONを作成します。

### JSONレスポンスのパース
```cpp
DynamicJsonDocument responseDoc(8192);
deserializeJson(responseDoc, payload);
String response = responseDoc["choices"][0]["message"]["content"].as<String>();
```

ChatGPT APIのレスポンスからテキストを抽出します。

### タッチ操作の処理
```cpp
auto t = M5.Touch.getDetail();
if (t.wasPressed()) {
    int x = t.x;
    int y = t.y;
    // 画面を4分割して操作エリアを判定
}
```

画面を4分割して、タッチ位置に応じて操作を実行します。

## ⚙️ カスタマイズ

### モデルの変更
```cpp
#define MODEL "gpt-3.5-turbo"  // または "gpt-4"
```

### レスポンスの最大トークン数
```cpp
doc["max_tokens"] = 200; // デフォルト: 200
```

### 創造性の設定
```cpp
doc["temperature"] = 0.7; // 0.0-2.0（デフォルト: 0.7）
// 0.0: より確定的、2.0: より創造的
```

### プロンプトの追加・変更
```cpp
const char* prompts[] = {
    "面白い短い話をしてください",
    "あなたのプロンプトを追加",
    // ...
};
```

### タイムアウト時間の変更
```cpp
http.setTimeout(30000); // 30秒（デフォルト）
```

## ⚠️ トラブルシューティング

### APIキーエラー
- OpenAI APIキーが正しいか確認してください
- APIキーに使用制限がないか確認してください
- [OpenAI Platform](https://platform.openai.com/api-keys)でAPIキーの状態を確認してください

### HTTP 401エラー（Unauthorized）
- APIキーが正しく設定されているか確認してください
- APIキーの先頭に`Bearer `が付いているか確認してください（コード内で自動追加されます）

### HTTP 429エラー（Rate Limit）
- APIの使用制限に達している可能性があります
- しばらく待ってから再試行してください
- OpenAIのプランを確認してください

### レスポンスが取得できない
- WiFi接続を確認してください
- シリアルモニターでHTTPレスポンスコードを確認してください
- JSONパースエラーがないか確認してください

### メモリ不足エラー
- JSONバッファサイズを調整してください
- `max_tokens`を減らしてください

## 💡 使用例

### 例1: カスタムプロンプトの追加
```cpp
const char* prompts[] = {
    "面白い短い話をしてください",
    "プログラミングのヒントを教えてください",
    "今日の運勢を教えてください",
    // カスタムプロンプトを追加
};
```

### 例2: 会話履歴の保持
現在の実装では各リクエストが独立していますが、会話履歴を保持するには、`messages`配列に過去の会話を追加します：

```cpp
// 会話履歴を保持する例
JsonArray messages = doc.createNestedArray("messages");
// 過去の会話を追加
JsonObject prevMessage = messages.createNestedObject();
prevMessage["role"] = "user";
prevMessage["content"] = "前の質問";
// 現在のメッセージを追加
JsonObject currentMessage = messages.createNestedObject();
currentMessage["role"] = "user";
currentMessage["content"] = prompt;
```

## 📖 参考資料

- [OpenAI API Documentation](https://platform.openai.com/docs/api-reference)
- [ChatGPT API Guide](https://platform.openai.com/docs/guides/chat)
- [M5StackとChatGPTで作るAIベースのストーリーテリング装置【前編】](https://deviceplus.jp/mc-general/m5stack-chatgpt-01/)
- [ArduinoJson ライブラリ](https://arduinojson.org/)

## 💰 コストについて

OpenAI APIの使用には料金がかかります：
- **GPT-3.5-turbo**: 比較的安価（1000トークンあたり約$0.002）
- **GPT-4**: より高価（1000トークンあたり約$0.03-0.06）

使用量は[OpenAI Platform](https://platform.openai.com/usage)で確認できます。

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み

