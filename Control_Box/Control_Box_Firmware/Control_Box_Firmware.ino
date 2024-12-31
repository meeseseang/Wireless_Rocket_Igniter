#include <esp_now.h>
#include <WiFi.h>

// Define WiFi chanel and pinouts
#define CHANNEL  1
#define ARM_SW   1
#define ARM_LED  4
#define BUTTON_1 3
#define BUTTON_2 35

// Slave data struct
esp_now_peer_info_t slave;

// Launch data
uint8_t data = 1;

void setup() {
  Serial.begin(115200);

  // Pin setup
  pinMode(ARM_SW,   INPUT);
  pinMode(ARM_LED,  OUTPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  
  digitalWrite(ARM_LED, LOW);

  // ESP NOW setup
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave(); // WiFi.macAddress() 
  esp_now_add_peer(&slave);
}

void loop() {
  if (digitalRead(ARM_SW) == LOW) {
    digitalWrite(ARM_LED, HIGH);

    if (digitalRead(BUTTON_1) == HIGH) {
      if (digitalRead(BUTTON_2) == HIGH) {
       esp_now_send(slave.peer_addr, &data, sizeof(data));
      }
    }
    
  } else {
    digitalWrite(ARM_LED, LOW);
  }

}

// Scan for slave devices and connect if they meet the SSID requirement
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

  for (int i=0; i<scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);

    // Scan for SSIDS that contain "RX"
    if (SSID.indexOf("RX") == 0) {
      int mac[6];
      if (6==sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
        for (int ii=0; ii<6; ++ii) {
          slave.peer_addr[ii] = (uint8_t) mac[ii];
        }
      }

      slave.channel = CHANNEL;
      slave.encrypt = 0;
      break;
    }
  }
}

// Callback for when data is sent from master to slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Sent data: ");
  Serial.println(data);
}
