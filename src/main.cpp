#include <Arduino.h>
#include <SPI.h>
#include <Wire1.h>
#include <string.h>
#include <EEPROM.h>
#include "SdFat.h"
#if ENABLE_SOFTWARE_SPI_CLASS  // Must be set in SdFat/SdFatConfig.h

#define FILE_BASE_NAME "PRS"

void receiveEvent(int bytes);

// Define pins for SPI
const uint8_t SOFT_MISO_PIN = PIN_PC0;
const uint8_t SOFT_MOSI_PIN = PIN_PE3;
const uint8_t SOFT_SCK_PIN  = PIN_PC1;
const uint8_t SD_CHIP_SELECT_PIN = PIN_PE2;

// SdFat software SPI template
SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;

char wire_buffer;

// Test file.
SdFile file;

String DATA;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {

    Serial.begin(115200);
    inputString.reserve(200);

    //Wire1.begin(9);
    //Wire1.setClock(400000);
    //Wire1.onReceive(receiveEvent);


    if (!sd.begin(SD_CHIP_SELECT_PIN)) {
        sd.initErrorHalt();
    }
/*
  if (!file.open("SoftSPI.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.println(F("This line was printed using software SPI."));

  file.rewind();
  file.close();
*/

    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
    char fileName[13] = FILE_BASE_NAME "00.csv";
    
    // Find an unused file name.
    if (BASE_NAME_SIZE > 6) {
        //error("FILE_BASE_NAME too long");
    }
    while (sd.exists(fileName)) {
        if (fileName[BASE_NAME_SIZE + 1] != '9') {
            fileName[BASE_NAME_SIZE + 1]++;
        } 
        else if (fileName[BASE_NAME_SIZE] != '9') {
            fileName[BASE_NAME_SIZE + 1] = '0';
            fileName[BASE_NAME_SIZE]++;
        } 
        else {
            //error("Can't create file name");
        }
    }

    if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
        //error("file.open");
    }

}

/*
void receiveEvent(int bytes) {
  while(Wire1.available()) {
    Wire1.readBytes((uint8_t*)&dataB, 31);
    
    file.print(dataA.time); file.print(",");
    file.print(dataA.acc_x, 4); file.print(",");
    file.print(dataA.acc_y, 4); file.print(",");
    file.print(dataA.acc_z, 4); file.print(",");
    file.print(dataA.gyro_x, 4); file.print(",");
    file.print(dataA.gyro_y, 4); file.print(",");
    file.print(dataA.gyro_z, 4); file.print("\n");
    
   file.print(dataB.dt, 4); file.write(',');
   file.print(dataB.roll, 4); file.write(',');
   file.print(dataB.pitch, 4); file.write(',');
   file.print(dataB.acc_roll, 4); file.write(',');
   file.print(dataB.acc_pitch, 4); file.write(',');
   file.print(dataB.gyro_roll_cont, 4); file.write(',');
   file.print(dataB.gyro_pitch_cont, 4); file.write("\n"); 
  }

  file.flush();
}
*/

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void loop() {
  if(stringComplete == true) {
      noInterrupts();
      file.print(inputString);
      file.flush();
      inputString = "";
      stringComplete = false;
      interrupts();
  }
}





#else  // ENABLE_SOFTWARE_SPI_CLASS
#error ENABLE_SOFTWARE_SPI_CLASS must be set non-zero in SdFat/SdFatConfig.h
#endif
