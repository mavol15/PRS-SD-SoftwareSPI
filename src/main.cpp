#include <Arduino.h>
#include <SPI.h>
#include <Wire1.h>
#include <string.h>
#include "SdFat.h"
#if ENABLE_SOFTWARE_SPI_CLASS  // Must be set in SdFat/SdFatConfig.h

struct structA
{
    const char struct_id = 'A';     // 1 byte
    unsigned int seq_id;            // 2 bytes
    unsigned long time;             // 4 bytes
    float acc_x;                    // 4 bytes
    float acc_y;                    // 4 bytes
    float acc_z;                    // 4 bytes
    float gyro_x;                   // 4 bytes
    float gyro_y;                   // 4 bytes
    float gyro_z;                   // 4 bytes
};                                  // Total 31 bytes

struct structB
{
    const char struct_id = 'B';     // 1 byte
    unsigned int seq_id;            // 2 bytes
    float dt;                       // 4 bytes
    float roll;                     // 4 bytes
    float pitch;                    // 4 bytes
    float acc_roll;                 // 4 bytes
    float acc_pitch;                // 4 bytes
    float gyro_roll_cont;           // 4 bytes
    float gyro_pitch_cont;          // 4 bytes
};                                  // Total 31 bytes

struct structC
{
    const char struct_id = 'C';     // 1 byte
    unsigned int seq_id;            // 2 bytes
    float gnss_lat;
    float gnss_lon;
    float gnss_alt;
    float baro_alt;
    float comb_alt;
};

structA dataA;
structB dataB;
structC datac;

void receiveEvent(int bytes);

// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = PIN_PC0;
const uint8_t SOFT_MOSI_PIN = PIN_PE3;
const uint8_t SOFT_SCK_PIN  = PIN_PC1;
// Chip select may be constant or RAM variable.
const uint8_t SD_CHIP_SELECT_PIN = PIN_PE2;

char wire_buffer;

// SdFat software SPI template
SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;

// Test file.
SdFile file;

String DATA;

void setup() {

  Wire1.begin(9);
  Wire1.onReceive(receiveEvent);


  if (!sd.begin(SD_CHIP_SELECT_PIN)) {
    sd.initErrorHalt();
  }

  if (!file.open("SoftSPI.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.println(F("This line was printed using software SPI."));

  file.rewind();
  file.close();

  if (!file.open("DATA.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

}

//------------------------------------------------------------------------------


void receiveEvent(int bytes) {
  while(Wire1.available()) {
    Wire1.readBytes((uint8_t*)&dataB, 31);
    /*
    file.print(dataA.time); file.print(",");
    file.print(dataA.acc_x, 4); file.print(",");
    file.print(dataA.acc_y, 4); file.print(",");
    file.print(dataA.acc_z, 4); file.print(",");
    file.print(dataA.gyro_x, 4); file.print(",");
    file.print(dataA.gyro_y, 4); file.print(",");
    file.print(dataA.gyro_z, 4); file.print("\n");
    */
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

void loop() {
  _delay_ms(1);
}

#else  // ENABLE_SOFTWARE_SPI_CLASS
#error ENABLE_SOFTWARE_SPI_CLASS must be set non-zero in SdFat/SdFatConfig.h
#endif  //ENABLE_SOFTWARE_SPI_CLASS