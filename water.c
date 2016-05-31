#include "simpletools.h"
#include "fdserial.h"
#include "adcDCpropab.h"

double min;
double max;
int volume;
double scale;

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
      last = (last + read) / 2;
      reads = reads + 1;
    } else {
      last = read;
      reads = 0;
    }
    pause(50);
  }
  return last;
}

double readWeight() {
  return max_double(readInput() * scale + min, 0);
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

void writeConfig() {
  FILE* fp = fopen("config", "w");
  fwrite(min, sizeof(min), 1, fp);
  fwrite(max, sizeof(max), 1, fp);
  fwrite(volume, sizeof(volume), 1, fp);
  fwrite(scale, sizeof(scale), 1, fp);
  fclose(fp);
}

void readConfig() {
  FILE* fp = fopen("config", "r");
  fread(&min, 4, 1, fp);
  fread(&max, 4, 1, fp);
  fread(&volume, 4, 1, fp);
  fread(&scale, 4, 1, fp);
  fclose(fp);
}

void writeData(long timestamp, int volume) {
  FILE* fp = fopen("data", "w");
  fwrite(timestamp, sizeof(timestamp), 1, fp);
  fwrite(volume, sizeof(volume), 1, fp);
  fclose(fp);
}

void init() {
  printf("Please enter the bottle's volume in mL.\n");
  scan("%d", &volume);

  printf("Place the empty bottle on the weight, and press any key.\n");
  scan("");
  min = readInput();

  printf("Fill the bottle to %imL, place it on the weight, and press any key.\n", volume);
  scan("");
  max = readInput();

  scale = volume/(max-min);
  min = -scale*min;
  max = volume * 1.1;
}

int main() {
  printf("This is automatic water bottle mesurer made by Abdul, Tabita, and Jacob.\n");
  adc_init(21, 20, 19, 18);
  init();
  int id = 0;
  while (1) {
    printf("Reading %i: %f\n", id, readWeight());
    id = id + 1;
    pause(1000);
  }
}
