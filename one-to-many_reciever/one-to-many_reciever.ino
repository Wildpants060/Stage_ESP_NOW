#include <esp_now.h>
#include <WiFi.h>

int ledpin  = 4;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int board1X;
  int board1Y;
  int board2X; 
  int board2Y; 
}struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board 1 x value: %d \n", myData.board1X);
  Serial.printf("Board 1 y value: %d \n", myData.board1Y);
  Serial.printf("Board 2 x value: %d \n", myData.board2X);
  Serial.printf("Board 2 y value: %d \n", myData.board2Y);
  digitalWrite(ledpin, HIGH);
  delay(10);
  digitalWrite(ledpin, LOW);
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  pinMode(ledpin, OUTPUT);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

  delay(10000);  
}