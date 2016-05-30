#include "simpletools.h"
#include "fdserial.h"

int main() {
  printf("%s\n", "I'm the main function =)");
  int min = readVolume();
  int max = readVolume();
  while (true) {
    printf("%s\n", readVolume());
  }
}

int readVolume() {
  printf("%s\n", "Begin: Reading volume.");
  int last = ;
  int reads = 0;
  while (reads < 10) {
    int read = ;
    if (read > last * 0.9 || read < last * 1.1 ) {

      last = (last + read) / 2;
      reads = reads + 1;
    } else {
      reads = 0;
    }
  }
  printf("%s\n", "Done: Reading volume.");
  return last;
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
