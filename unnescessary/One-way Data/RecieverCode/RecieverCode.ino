#include <esp_now.h>
#include <WiFi.h>

//pin voor led
int led = 4;

//Maak een structure voor de data
typedef struct struct_bericht
{
  char a[32];
  int b;
  float c;
  bool d;
} struct_bericht;

//Maak een object aan van een struct
struct_bericht messageData;

//maak een callback functie voor wanneer er data word ontvangen
void onvangenData(const uint8_t *mac_address, const uint8_t *incomingData, int len)
{
  memcpy(&messageData, incomingData, sizeof(messageData));
  Serial.print("Ontvangen Data: ");
  Serial.println(len);
  Serial.print("Character waarde: ");
  Serial.println(messageData.a);
  Serial.print("Integer waarde: ");
  Serial.println(messageData.b);
  Serial.print("Float waarde: ");
  Serial.println(messageData.c);
  Serial.print("Boolean waarde: ");
  Serial.println(messageData.d);
  if(messageData.d == 1)
  {
    digitalWrite(led, HIGH);
  }else
  {
    digitalWrite(led, LOW);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(led, OUTPUT);

  //zet de ESP in station mode
  WiFi.mode(WIFI_STA);
  //initialiseer ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //Registreer callback functie
  esp_now_register_recv_cb(onvangenData);
}

void loop()
{
  
}