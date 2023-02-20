 /*
* GPS-Tracker
*/
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "TinyGPSPlus.h"
#include "SoftwareSerial.h"

static const int RXPin = 3, TXPin = 1;
static const uint32_t GPSBaud = 9600;
double previous_location_lat;
double previous_location_lng;
String File0;
const char* F;
// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void appendFile(fs::FS &fs, const char * path, const char * message){ // Function to write information to SD card
    Serial.printf(" Appending to file: %s ", path);
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.printf(" Appended file ");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void setup(){
    
    Serial.begin(19200); // Serial Moniter
    ss.begin(GPSBaud); // GPS  

    if(!SD.begin()){
        Serial.println("Card Mount Failed"); // This either means you need to check the wiring or if the actual SD card isn't there
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    while(!gps.date.isValid() || !gps.time.isValid()){delay(1000);}
    previous_location_lat = gps.location.lat(); // Saving previous location for weeding out extraneous Latitude data
    previous_location_lng = gps.location.lng(); // Saving previous location for weeding out extraneous Longitude data
    File0 = "/GpsData_"+String(gps.date.month())+"_"+String(gps.date.day())+"_"+String(gps.date.year())+"_"+String(gps.time.hour())+".csv";
    F = File0.c_str();
    // save data header
    appendFile(SD, F,"Date [mm/dd/yyyy], Time [HH:MM:SS.ZZ], Latitude [deg], Longitude [deg], Speed [kmph]\n");
    
}

void loop(){
  // Exicute code only if there is a SD card detected
  if(SD.cardType() != CARD_NONE){ 

  while (ss.available() > 0){
    if (gps.encode(ss.read()) && gps.location.isValid() && gps.date.isValid() && gps.time.isValid()){
      String data_to_save = ""; // data string for saving
      data_to_save += String(gps.date.month())+"/"+String(gps.date.day())+"/"+  // date
                      String(gps.date.year())+",";
      data_to_save += String(gps.time.hour())+":"+String(gps.time.minute())+":"+  //  time
                      String(gps.time.second())+"."+String(gps.time.centisecond())+",";
      data_to_save += String(gps.location.lat(),8)+","; // latitude
      data_to_save += String(gps.location.lng(),8)+","; // longitude
      data_to_save += String(gps.speed.kmph()); // Speed
      data_to_save += "\n";

      if(abs(previous_location_lat - gps.location.lat()) < 10 && abs(previous_location_lng - gps.location.lng()) < 10){
      
        const char* d = data_to_save.c_str(); // convert data string into const char * (so it is compatible with appendFile function)
        appendFile(SD, F,d); // save new data points
        Serial.println(d); // uncomment to print GPS data
      }
      previous_location_lat = gps.location.lat();
      previous_location_lng = gps.location.lng();
      delay(100);  // wait one tenth of a sec before looping  
    }
  }
  }else{delay(1000);// If the SD card is not detected, wait a sec
    Serial.println("No SD card attached");
        if(!SD.begin()){
        Serial.println("Card Mount Failed");
    }} 
  if (millis() > 5000 && gps.charsProcessed() < 10) // checking to see if the GPS is sending data
  {
    Serial.println(F("No GPS detected: check wiring."));
    delay(1000);
  }
}
