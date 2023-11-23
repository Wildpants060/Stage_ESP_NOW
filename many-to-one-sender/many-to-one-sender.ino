#include <esp_now.h>
#include <WiFi.h>

int ledpin = 4;

uint8_t broadcastAddress[] = {0x94, 0xB9, 0x7E, 0xD9, 0xA5, 0x68};

typedef struct struct_message
{
  int id;
  int x;
  int y;
}struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  digitalWrite(ledpin, HIGH);
  delay(10);
  digitalWrite(ledpin, LOW);
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(ledpin, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  myData.id = 2;
  myData.x = random(0,50);
  myData.y = random(0,50);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if(result == ESP_OK){
    Serial.println("Sent with success");
  }else{
    Serial.println("Error sending Data");
  }
  delay(1000);
}