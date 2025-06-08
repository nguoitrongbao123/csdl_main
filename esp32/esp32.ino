// ==== BLYNK CONFIG ====
#define BLYNK_TEMPLATE_ID "TMPL6e2s7Kwul"
#define BLYNK_TEMPLATE_NAME "Smart farm"
#define BLYNK_AUTH_TOKEN "5vlr8xngSIrmudPdFWSBr0J87uPtYt9V"

#include <WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>

// ==== WIFI ====
char ssid[] = "thangdan";      // WiFi SSID
char pass[] = "22022111";      // WiFi password

// ==== I2C ====
#define I2C_SLAVE_ADDR 0x42     // Đúng với STM32 (0x42 << 1)
#define SDA_PIN 21              // I2C SDA pin
#define SCL_PIN 22               // I2C SCL pin

// ==== GLOBAL VARIABLES ====
int temp = 0, hum = 0, soil = 0;         // Sensor data values
int pumpStatus = 0, lightStatus = 0;     // Status of pump and light
unsigned long last_request = 0;          // (Optional) For future use – timestamp of last event
 
String incomingData = "";                 // Buffer to store incoming I2C data from STM32

// ==== I2C RECEIVE CALLBACK FUNCTION ====
// This function is automatically called when STM32 sends data via I2C
void receiveEvent(int len) {
  incomingData = "";                  // Clear the previous data
  while (Wire.available()) {
    char c = Wire.read();             // Read each byte
    incomingData += c;                 // Append to the buffer string
  }  

  // ==== Parse and extract values from the incoming string ====
  Serial.print("Nhận STM32: ");
  Serial.println(incomingData);

  // Parse nếu có đủ
  if (incomingData.indexOf("TEMP:") >= 0) {
    // Extract integer values from structured string
    int temp = incomingData.substring(incomingData.indexOf("TEMP:") + 5, incomingData.indexOf(",HUM")).toInt();
    int hum  = incomingData.substring(incomingData.indexOf("HUM:") + 4, incomingData.indexOf(",SOIL")).toInt();
    int soil = incomingData.substring(incomingData.indexOf("SOIL:") + 5, incomingData.indexOf(",P")).toInt();
    int pumpStatus = incomingData.substring(incomingData.indexOf("P:") + 2, incomingData.indexOf(",D")).toInt();
    int lightStatus = incomingData.substring(incomingData.indexOf("D:") + 2).toInt();

    // ==== Update Blynk virtual pins with latest sensor data ====
    Blynk.virtualWrite(V2, temp);         // Temperature → Virtual Pin V2
    Blynk.virtualWrite(V0, hum);          // Humidity → V0
    Blynk.virtualWrite(V1, soil);         // Soil moisture → V1
    Blynk.virtualWrite(V3, pumpStatus);   // Pump status → V3
    Blynk.virtualWrite(V4, lightStatus);  // Light status → V4
  }
}


// ==== SETUP FUNCTION ====
// Called once when ESP32 boots
void setup() {
  Serial.begin(115200);                        // Initialize serial for debugging
  Wire.begin(0x42);                            // ESP32 làm slave địa chỉ 0x42
  Wire.onReceive(receiveEvent);                // Register callback to handle I2C receive
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);   // Connect to Blynk cloud
}

// ==== MAIN LOOP ====
// Runs continuously after setup()
void loop() {
Blynk.run();                                    // Maintain Blynk connection and event handling
}
