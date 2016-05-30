#include "simpletools.h"
#include "fdserial.h"
#include "adcDCpropab.h"
#include "math.h"

double min;
double max;
int volume;
double scale = 1;

double max_double(double a, double b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

double readInput() {
  double last = adc_volts(3);
  int reads = 0;
  while (reads < 10) {
    double read = adc_volts(3);
    if (read > last * 0.95 && read < last * 1.05) {
      last = (read + last) / 2
      reads = reads + 1;
    } else {
      last = read;
      reads = 0;
    }
    pause(50);
  }
}

double readWeight() {
  double last = readInput();
  return max(last * scale + min, 0);
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
  scan("");
  min = readInput();
  printf("Fill the container to %dmL, place it on the weight, and press any key.\n", volume);
  scan("");
  max = readInput();

  scale = max/(max-min);
  min = -scale*min;
}

int main() {
  adc_init(21, 20, 19, 18);
  init();
  printf("%s\n", "I'm the main function =)");
  int id = 0;
  while (1) {
    printf("Reading %i: %f\n", id, readWeight());s
    id = id + 1;
    pause(1000);
  }
}
