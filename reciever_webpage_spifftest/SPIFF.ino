void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, "w");
  if (!file)
  {
    return;
  }
  file.print(message);
  file.close();
}

// Read File from SPIFFS
String readFile(fs::FS &fs, const char * path) {
  String fileContent = "";
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    return fileContent;
  }
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  file.close();
  return fileContent;
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}
