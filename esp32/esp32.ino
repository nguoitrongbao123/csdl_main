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
unsigned long last_request = 0;

String incomingData = "";

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
  }
}


// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  Wire.begin(0x42);  // ESP32 làm slave địa chỉ 0x42
  Wire.onReceive(receiveEvent);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// ==== LOOP ====
void loop() {
Blynk.run();
}
