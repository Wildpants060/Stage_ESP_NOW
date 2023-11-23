//Dit is de code voor one-way transmitting van esp-now

//include de libraries
#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>



//De variabelen voor de data dat je gaat versturen
int int_value;
float float_value;
bool bool_value = true;

esp_err_t result;

//variablele voor de knop
int button = 12;
bool buttonDown = false;

//De MAC-addressen voor de onvangers 
uint8_t Device2Address[] = {0x94, 0xB9, 0x7E, 0xD9, 0x2C, 0x34};

//Maak een structure voor de data
typedef struct struct_bericht
{
  char a[32];
  int b;
  float c;
  bool d;
}struct_bericht;

//Maak een object aan van een struct
struct_bericht messageData;

//info van de peer
esp_now_peer_info_t peerInfo;

//maak een callbackfunctie voor wanneer data wordt verzonden
void dataIsVerzonden(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLaatste packet status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Levering succes" : "Levering mislukt");
}

void setup() {
  delay(5000);
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);
  //Zet de esp in Station modus
  WiFi.mode(WIFI_MODE_STA);
  //Print MAC Address naar Serial monitor
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  //initialisatie van ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initialisatie van ESP-NOW");
    return;
  }
  //Registreer de aangemaakte callback
  esp_now_register_send_cb(dataIsVerzonden);
  //Registreer de peer
  memcpy(peerInfo.peer_addr, Device2Address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //Voeg de peer toe
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("mislukt om de peer toe te voegen");
    return;
  }
}

void loop() {
  
  //maak hier u test data aan

  //maak een random integer aan
  int_value = random(1,20);

  //maak nu gebruik van de integer om een float aan te maken
  float_value = 1.3* int_value;

  //inverteer de boolean value
  bool_value = !bool_value;

  //Formateer de gestructureerde data
  strcpy(messageData.a, "Testing voor zending");
  messageData.b = int_value;
  messageData.c = float_value;
  messageData.d = bool_value;

  //lees de knop in
  if(digitalRead(button))
  {
    //Detecteer de transitie van laag naar hoog
    if(!buttonDown)
    {
      buttonDown = true;
      esp_err_t result = esp_now_send(Device2Address, (uint8_t *) &messageData, sizeof(messageData));
    }
    delay(500);
  }else{
    buttonDown = false;
  }
  //Stuur het bericht via ESP-NOW
  

  if (result == ESP_OK) {
    Serial.println("zending ok");
  }
  else {
    Serial.println("error in zending");
  }
  delay(1000);
}
