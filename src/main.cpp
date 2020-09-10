#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include "SdFat.h"
#if ENABLE_SOFTWARE_SPI_CLASS  // Must be set in SdFat/SdFatConfig.h

// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 26;
const uint8_t SOFT_MOSI_PIN = 14;
const uint8_t SOFT_SCK_PIN  = 15;
// Chip select may be constant or RAM variable.
const uint8_t SD_CHIP_SELECT_PIN = 10;

// SdFat software SPI template
SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;

// Test file.
SdFile file;

String DATA;

void setup() {

  Serial.begin(112500);   //  Init serial connection to main mcu
  Serial1.begin(115200);  //  Init serial connection for debug
  _delay_ms(10);          // Short delay to allow serial connections to get ready 
  
  if (!sd.begin(SD_CHIP_SELECT_PIN)) {
    sd.initErrorHalt();
  }

  if (!file.open("SoftSPI.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.println(F("This line was printed using software SPI."));

  file.rewind();
  
  while (file.available()) {
    Serial1.write(file.read());
  }

  file.close();

  if (!file.open("DATA.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  Serial1.println(F("Done."));

}

//------------------------------------------------------------------------------

void loop() {
    if(Serial.available()) {
      DATA = Serial.readStringUntil('\n');
      file.println(DATA);
    
  }
  _delay_ms(1);
}

#else  // ENABLE_SOFTWARE_SPI_CLASS
#error ENABLE_SOFTWARE_SPI_CLASS must be set non-zero in SdFat/SdFatConfig.h
#endif  //ENABLE_SOFTWARE_SPI_CLASS