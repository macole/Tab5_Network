# RSSReader - RSSフィードリーダー

M5Stack Tab5のWiFi機能を使用して、RSSフィードを取得して表示するプログラムです。

## 📋 概要

このプログラムは、M5Stack Tab5のWiFi機能を使用して、Yahoo NewsのRSSフィードを取得し、ニュースタイトルを画面に表示します。HTTPS通信でRSSフィードを読み込み、XMLを解析してタイトルを抽出します。

## 🔧 必要なハードウェア

- M5Stack Tab5

## 📚 必要なライブラリ

- **M5Unified**: M5Stack統合ライブラリ
  ```bash
  arduino-cli lib install "M5Unified@0.2.10"
  ```
- **WiFi**: ESP32標準ライブラリ（Arduino IDEに含まれています）
- **HTTPClient**: ESP32標準ライブラリ（Arduino IDEに含まれています）

## 🚀 セットアップ

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

3. **プログラムのアップロード**
   - Arduino IDEまたはArduino CLIでプログラムをアップロード

## 💻 使用方法

1. WiFiのSSIDとパスワードを設定
2. プログラムをアップロード
3. デバイスがWiFiネットワークに接続し、RSSフィードを取得します
4. 最大5件のニュース/記事タイトルが画面に日本語で表示されます

## 📊 機能

- **HTTPS接続**: Yahoo News RSSフィードサーバーへのセキュアな接続
- **RSS解析**: XMLからニュースタイトルを抽出
- **日本語表示**: 日本語フォントでタイトルを表示
- **複数記事表示**: 最大5件のニュースを表示

## 🔍 プログラムの動作

1. **初期化**
   - M5Stackの初期化
   - WiFi接続
   - HTTPクライアントの設定

2. **RSS取得**
   - Yahoo News RSSフィードサーバーへのHTTPS接続
   - HTTPリクエストの送信
   - XMLレスポンスの受信

3. **表示**
   - XMLから最大5件のニュースタイトルを抽出
   - 日本語フォントで画面に表示

## 📝 コードの主要なポイント

### HTTPクライアントの初期化
```cpp
HTTPClient http;
http.setTimeout(15000);  // タイムアウトを15秒に設定
```

### HTTPリクエストの送信
```cpp
// Yahoo News RSSフィード（HTTPS）
if (http.begin("https://news.yahoo.co.jp/rss/topics/top-picks.xml")) {
  int status = http.GET();
  if (status == HTTP_CODE_OK) {
    String xml = http.getString();
    // XMLを解析
  }
  http.end();
}
```

### XMLからタイトルを抽出
```cpp
String extractTag(String xml, String tagName, int startPos = 0) {
  String openTag = "<" + tagName + ">";
  String closeTag = "</" + tagName + ">";
  int start = xml.indexOf(openTag, startPos);
  int end = xml.indexOf(closeTag, start);
  return xml.substring(start + openTag.length(), end);
}
```

### 日本語フォントの設定
```cpp
M5.Display.setFont(&fonts::lgfxJapanGothic_16);
M5.Display.println("日本語のタイトル");
```

## 📄 ライセンス

このプログラムはMITライセンスの下で公開されています。

Copyright (c) 2025 macole

詳細はプロジェクトルートの`LICENSE`ファイルを参照してください。

---

**作成日**: 2025年12月6日  
**最終更新**: 2025年12月28日  
**対象デバイス**: M5Stack Tab5  
**動作確認**: ✅ 正常動作確認済み

## 📌 注意事項

- このサンプルはHTTPS通信を使用しています（`https://news.yahoo.co.jp/rss/topics/top-picks.xml`）
- HTTPClient.hは内部でHTTPS通信を適切に処理します
- 証明書検証は行っていませんが、通信自体は暗号化されています
- サンプルでは「Yahoo!ニュース・トピックス - 主要」のRSSフィードを使用しています
- 他のRSSフィードを使用する場合は、URLを変更してください

## 🔗 RSSフィードの情報

- **URL**: https://news.yahoo.co.jp/rss/topics/top-picks.xml
- **提供元**: Yahoo! JAPAN
- **説明**: Yahoo! JAPANのニュース・トピックスで取り上げている最新の見出しを提供
- **参考**: [Yahoo!ニュース・トピックス](https://news.yahoo.co.jp/topics/top-picks?source=rss)