#include "simpletools.h"
#include "fdserial.h"
#include "adcDCpropab.h"

const min;
const max;
const voltToGram = 1;

int main() {
  printf("%s\n", "I'm the main function =)");
  min = readVolume();
  max = readVolume();
  while (true) {
    printf("Read weight: %s\n", readWeight());
  }
}

int readVolume() {
  printf("%s\n", "Begin: Reading volume.");
  int last = readWeight();
  int reads = 0;
  while (reads < 10) {
    int read = readWeight;
    if (read > last * 0.9 && read < last * 1.1 ) {

      last = (last + read) / 2;
      reads = reads + 1;
    } else {
      reads = 0;
    }
  }
  printf("%s\n", "Done: Reading volume.");
  return max(last - min, 0);
}

int readWeight() {
  return adc_volts(3) * voltToGram;
}

bool writeData(unsigned int volume, unsigned long timestamp) {
  printf("%s\n", "I write data to the SD card =)");
}

int readData() {
  printf("%s\n", "I read data from the SD card =)");
  return {0x00, 0x01, 0x02};
}

bool sendData(char[] data) {
  printf("%s\n", "I send data from the propeller to the app via bluetooth =)");
}

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}
