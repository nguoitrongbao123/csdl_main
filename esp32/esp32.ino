// ==== BLYNK CONFIG ====
#define BLYNK_TEMPLATE_ID "TMPL6e2s7Kwul"
#define BLYNK_TEMPLATE_NAME "Smart farm"
#define BLYNK_AUTH_TOKEN "5vlr8xngSIrmudPdFWSBr0J87uPtYt9V"

#include <WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>

// ==== WIFI ====
char ssid[] = "thangdan";
char pass[] = "22022111";

// ==== I2C ====
#define I2C_SLAVE_ADDR 0x42     // Đúng với STM32 (0x42 << 1)
#define SDA_PIN 21
#define SCL_PIN 22

// ==== BIẾN ====
int temp = 0, hum = 0, soil = 0;
int pumpStatus = 0, lightStatus = 0;
String controlBuffer = "P:0,D:0\n";  // Chuỗi điều khiển gửi về STM32
String incomingData = "";

// ==== NHẬN DỮ LIỆU TỪ STM32 (I2C MASTER GỬI) ====
void receiveEvent(int len) {
  incomingData = "";
  while (Wire.available()) {
    char c = Wire.read();
    incomingData += c;
  }

  Serial.print("Nhận STM32: ");
  Serial.println(incomingData);

  // Parse nếu có đủ
  if (incomingData.indexOf("TEMP:") >= 0) {
    int temp = incomingData.substring(incomingData.indexOf("TEMP:") + 5, incomingData.indexOf(",HUM")).toInt();
    int hum  = incomingData.substring(incomingData.indexOf("HUM:") + 4, incomingData.indexOf(",SOIL")).toInt();
    int soil = incomingData.substring(incomingData.indexOf("SOIL:") + 5, incomingData.indexOf(",P")).toInt();
    int pumpStatus = incomingData.substring(incomingData.indexOf("P:") + 2, incomingData.indexOf(",D")).toInt();
    int lightStatus = incomingData.substring(incomingData.indexOf("D:") + 2).toInt();

    Blynk.virtualWrite(V2, temp);
    Blynk.virtualWrite(V0, hum);
    Blynk.virtualWrite(V1, soil);
    Blynk.virtualWrite(V3, pumpStatus);
    Blynk.virtualWrite(V4, lightStatus);

    updateControlBuffer();  // Đồng bộ lại buffer điều khiển
  }
}

// ==== CẬP NHẬT BUFFER KHI CÓ LỆNH MỚI ====
void updateControlBuffer() {
  controlBuffer = "P:" + String(pumpStatus) + ",D:" + String(lightStatus) + "\n";
  Serial.print("Buffer gửi STM32: ");
  Serial.println(controlBuffer);
}

// ==== NHẬN LỆNH TỪ BLYNK ====
BLYNK_WRITE(V3) {
  pumpStatus = param.asInt();  // 0 hoặc 1
  updateControlBuffer();
}
BLYNK_WRITE(V4) {
  lightStatus = param.asInt();
  updateControlBuffer();
}

// ==== TRẢ DỮ LIỆU KHI STM32 YÊU CẦU ====
void requestEvent() {
  Wire.write((const uint8_t*)controlBuffer.c_str(), controlBuffer.length());
}

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  Wire.begin(0x42);  // ESP32 làm slave địa chỉ 0x42
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);                 // Trả dữ liệu cho STM32 khi được gọi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// ==== LOOP ====
void loop() {
Blynk.run();
}
