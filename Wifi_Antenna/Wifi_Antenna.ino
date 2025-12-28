#include "M5Unified.h"

void setup()
{
    auto cfg = M5.config();
    cfg.output_power = true;  // バッテリーユニット使用時にディスプレイ電源を確保
    M5.begin(cfg);

    M5.Display.setTextSize(2);
    M5.Display.setCursor(0, 0);
    M5.Display.println("Wi-Fi Antenna");

    // 内蔵Wi-Fiアンテナを使用
    auto& ioe = M5.getIOExpander(0);
    ioe.digitalWrite(0, false);
    M5.Display.println("Mode: Internal");

    // 外部Wi-Fiアンテナを使用
    // auto& ioe = M5.getIOExpander(0);
    // ioe.digitalWrite(0, true);
    // M5.Display.println("Mode: External");
}

void loop()
{
} 