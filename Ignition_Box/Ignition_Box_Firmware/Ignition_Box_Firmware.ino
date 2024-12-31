#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1
#define MOSFET  33

uint8_t newData;

void setup() {
  Serial.begin(115200);

  pinMode(MOSFET, OUTPUT);
  digitalWrite(MOSFET, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("RX_Ignition", "Tunguska", CHANNEL, 0);

  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  if (newData == 1) {
    digitalWrite(MOSFET, HIGH);
  }

}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Received: ");
  Serial.println(*data);
  memcpy(&newData, data, sizeof(newData));
}
