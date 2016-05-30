#include "simpletools.h"
#include "fdserial.h"
#include "adcDCpropab.h"
#include "math.h"

int min;
int max;
int volume;
double scale;

int max_int(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

double readWeight() {
  int reads = 0;
  int last = adc_volts(3);
  while (reads < 10) {
    int read = adc_volts(3);
    if (read > last * 0.95 && read < last * 1.05) {
      last = (read+last)/2
      reads = reads + 1;
    } else {
      reads = 0;
    }
    pause(100);
  }
  return last * scale;
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
  return max_int(last - min, 0);
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

void init() {
  printf("%s\n", "Please enter the containers volume in mL?");
  scan("%d", &volume);
  printf("%s\n", "Place the empty container on the weight, and press any key.");
  scan("", &min);
  printf("Fill the container to mL, and press any key.\n", volume);
  scan("", &max);
  scale = (volume-min)/max
  printf("The scale constant is: %d\n", scale);
}

int main() {
  init();
  printf("%s\n", "I'm the main function =)");
  adc_init(21, 20, 19, 18);
  int id = 0;
  while (1) {
    printf("Reading %i: %f\n", id, readWeight());
    id = id + 1;
    pause(1000);
  }
}
