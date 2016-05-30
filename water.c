#include "simpletools.h"
#include "fdserial.h"
#include "adcDCpropab.h"
#include "math.h"

const min;
const max;
const voltToGram = 1.0;

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int readWeight() {
  return adc_volts(3) * voltToGram;
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

void writeData() {
  printf("%s\n", "I write data to the SD card =)");
}

int readData() {
  printf("%s\n", "I read data from the SD card =)");
  return 0;
}

void sendData() {
  printf("%s\n", "I send data from the propeller to the app via bluetooth =)");
}

int main() {
  printf("%s\n", "I'm the main function =)");
  adc_init(21, 20, 19, 18);
  int id = 0;
  while (1) {
    printf("Reading %i: %f\n", id, readWeight());
    int id = id + 1;
    pause(100);
  }
}
