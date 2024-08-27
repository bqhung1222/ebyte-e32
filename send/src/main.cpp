#include "Arduino.h"
#include "LoRa_E32.h"
#include "NeoPixelBus.h"

const int buttonPin = 2;  // Chân kết nối với nút nhấn
const int PixelCount = 1;
const int PixelPin = 16;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> myStrip(PixelCount, PixelPin);
LoRa_E32 e32ttl100(&Serial1, 3, 7, 8);
void E32() {
    // Bắt đầu giao tiếp với module E32
    e32ttl100.begin();

    // Cấu hình module E32
    ResponseStructContainer c = e32ttl100.getConfiguration();
        Configuration configuration = *(Configuration*) c.data;
        configuration.ADDL = 0x03; // Địa chỉ thấp
        configuration.ADDH = 0x00; // Địa chỉ cao
        configuration.CHAN = 0x04; // Kênh
        configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
        configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
        configuration.OPTION.fec = FEC_0_OFF;
        configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
        configuration.OPTION.transmissionPower = POWER_30;
        configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
        configuration.SPED.uartBaudRate = UART_BPS_9600;
        configuration.SPED.uartParity = MODE_00_8N1;

        e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    c.close();
}

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);  // Kích hoạt điện trở kéo lên nội bộ
    pinMode(3, INPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    
    Serial1.begin(9600);
    Serial.begin(9600);

    myStrip.Begin();
    myStrip.SetPixelColor(0, RgbColor(0, 0, 0));
    myStrip.Show();

    // Khởi tạo module E32
    E32();

    Serial.println("Ready...");
}

void loop() {
    if (digitalRead(buttonPin) == LOW) {
        
        E32();
        delay(1000);  
    }

    // Gửi dữ liệu
    Serial1.println("a");
    Serial.println("Sent: a");
    myStrip.SetPixelColor(0, RgbColor(255, 0, 0));
    myStrip.Show();
    delay(1000);
    myStrip.SetPixelColor(0, RgbColor(0, 0, 0));
    myStrip.Show();
    
    // Nhận dữ liệu
    if (Serial1.available() > 0) {
        String input = Serial1.readString();
        Serial.println("Received: " + input);
        myStrip.SetPixelColor(0, RgbColor(255, 255, 0));
        myStrip.Show();
        delay(1000);
        myStrip.SetPixelColor(0, RgbColor(0, 0, 0));
        myStrip.Show();
    }

    delay(5000);  // Chờ một chút trước khi tiếp tục gửi và nhận
}

void initE32() {
    // Bắt đầu giao tiếp với module E32
    e32ttl100.begin();

    // Cấu hình module E32
    ResponseStructContainer c = e32ttl100.getConfiguration();
        Configuration configuration = *(Configuration*) c.data;
        configuration.ADDL = 0x03; // Địa chỉ thấp
        configuration.ADDH = 0x00; // Địa chỉ cao
        configuration.CHAN = 0x04; // Kênh
        configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
        configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
        configuration.OPTION.fec = FEC_0_OFF;
        configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
        configuration.OPTION.transmissionPower = POWER_30;
        configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
        configuration.SPED.uartBaudRate = UART_BPS_9600;
        configuration.SPED.uartParity = MODE_00_8N1;

        e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    c.close();
}
