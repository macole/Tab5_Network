#include <M5Unified.h>
#include <WiFi.h>

void setup()
{
    M5.begin();
    Serial.begin(115200);

    M5.Display.setFont(&fonts::FreeMonoBoldOblique9pt7b);

    // Arduino IDEでM5Tab5ボードを選択した場合、定義済みのデフォルトピンを使用できます
    WiFi.setPins(BOARD_SDIO_ESP_HOSTED_CLK, BOARD_SDIO_ESP_HOSTED_CMD, BOARD_SDIO_ESP_HOSTED_D0,
                  BOARD_SDIO_ESP_HOSTED_D1, BOARD_SDIO_ESP_HOSTED_D2, BOARD_SDIO_ESP_HOSTED_D3,
                  BOARD_SDIO_ESP_HOSTED_RESET);
}

void loop()
{
    M5.Display.setCursor(0, 0);
    M5.Display.println("Scan start");

    // WiFi.scanNetworksは検出されたネットワークの数を返します
    int n = WiFi.scanNetworks();
    M5.Display.println("Scan done");
    if (n == 0) {
        M5.Display.println("no networks found");
    } else {
        M5.Display.print(n);
        M5.Display.println(" networks found");
        M5.Display.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // 検出された各ネットワークのSSIDとRSSIを表示
            M5.Display.printf("%2d", i + 1);
            M5.Display.print(" | ");
            M5.Display.printf("%-32.32s", WiFi.SSID(i).c_str());
            M5.Display.print(" | ");
            M5.Display.printf("%4ld", WiFi.RSSI(i));
            M5.Display.print(" | ");
            M5.Display.printf("%2ld", WiFi.channel(i));
            M5.Display.print(" | ");
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
                case WIFI_AUTH_WPA_WPA2_PSK:
                    M5.Display.print("WPA+WPA2");
                    break;
                case WIFI_AUTH_WPA2_ENTERPRISE:
                    M5.Display.print("WPA2-EAP");
                    break;
                case WIFI_AUTH_WPA3_PSK:
                    M5.Display.print("WPA3");
                    break;
                case WIFI_AUTH_WPA2_WPA3_PSK:
                    M5.Display.print("WPA2+WPA3");
                    break;
                case WIFI_AUTH_WAPI_PSK:
                    M5.Display.print("WAPI");
                    break;
                default:
                    M5.Display.print("unknown");
            }
            M5.Display.println();
            delay(10);
        }
    }
    M5.Display.println("");
    // スキャン結果を削除してメモリを解放
    WiFi.scanDelete();

    // 次のスキャンまで少し待機
    delay(5000);
} 