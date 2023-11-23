#include <esp_now.h>
#include <WiFi.h>

#define PinInput1 4
#define PinInput2 5

uint8_t macAddressesp2[] = {0xDC, 0x54, 0x75, 0xD5, 0xE7, 0xB0};
uint8_t macAddressesBalzal2[] = {0x7C, 0x9E, 0xBD, 0x60, 0xEE, 0xC0};

typedef struct test_struct
{
  int x;
  int y;
}test_struct;

test_struct test;

esp_now_peer_info_t peerInfo;

void dataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  //copieer het zender mac addr naar een string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  Serial.begin(115200);

  pinMode(PinInput1, INPUT_PULLUP);
  pinMode(PinInput2, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
  }

  esp_now_register_send_cb(dataSend);

  //registreer u peers hier
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //eerste peer esp32s3 2
  memcpy(peerInfo.peer_addr, macAddressesp2, 6);
  if(esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer 1");
    return;
  }
  //tweede peer
  memcpy(peerInfo.peer_addr, macAddressesBalzal2, 6);
  if(esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer 2");
    return;
  }
}

void loop()
{
  if(digitalRead(PinInput1) == LOW)
  {
    esp_err_t result1 = esp_now_send(macAddressesp2, (uint8_t *) test.x, sizeof(test_struct));
   
    if (result1 == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }

  if(digitalRead(PinInput2) == LOW)
  {
   esp_err_t result2 = esp_now_send(macAddressesBalzal2, (uint8_t *) test.y, sizeof(test_struct));
   
    if (result2 == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }  
}