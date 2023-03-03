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

void appendFile(fs::FS &fs, const char * path, const char * message){ // function to write information to SD card
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
        Serial.println("Card Mount Failed"); // this either means you need to check the wiring or if the actual SD card isn't there
        return;
    }
   
    // debugging info
    uint8_t cardType = SD.cardType(); // initializing cardType for debugging
    if(cardType == CARD_NONE){ // checking if a SD card is in the reader
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
   
    // waiting for GPS to aquire date and time 
    while(!gps.date.isValid() || !gps.time.isValid()){delay(1000);} // the program will not continue until both date and time are available
    
    // saving current location for weeding out extraneous location data
    previous_location_lat = gps.location.lat(); 
    previous_location_lng = gps.location.lng(); 
 
    // naming the file
    File0 = "/GpsData_"+String(gps.date.month())+"_"+String(gps.date.day())+"_"+String(gps.date.year())+"_"+String(gps.time.hour())+".csv";
    F = File0.c_str();
 
    // save data header
    appendFile(SD, F,"Date [mm/dd/yyyy], Time [HH:MM:SS.ZZ], Latitude [deg], Longitude [deg], Speed [kmph]\n");
    
}

void loop(){
  // exicute code only if there is a SD card detected
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
      
      // if the GPS location is suspiciously jumpy (10 degrees of logitude or latitude movement between loops) the data is ignored
      if(abs(previous_location_lat - gps.location.lat()) < 10 && abs(previous_location_lng - gps.location.lng()) < 10){
      
        const char* d = data_to_save.c_str(); // convert data string into const char * (so it is compatible with appendFile function)
        appendFile(SD, F,d); // save new data points
        Serial.println(d); // uncomment to print GPS data
      }
     
      // saving current location for weeding out extraneous location data
      previous_location_lat = gps.location.lat(); 
      previous_location_lng = gps.location.lng(); 
     
      delay(100);  // wait one tenth of a sec before looping  
    }
  }
  }else{delay(1000);// if the SD card is not detected, wait a sec
    Serial.println("No SD card attached");
        if(!SD.begin()){
        Serial.println("Card Mount Failed");
    }
  } 
}
