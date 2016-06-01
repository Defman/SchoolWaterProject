#include "simpletools.h"
#include "fdserial.h"
#include "adcDCpropab.h"

static volatile double min;
static volatile double max;
static volatile int volume;
static volatile double scale;

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
  print("%s\n", "I write data to the SD card =)");
}

int readData() {
  print("%s\n", "I read data from the SD card =)");
  return 0;
}

void sendData() {
  print("%s\n", "I send data from the propeller to the app via bluetooth =)");
}

/*
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
*/

void init() {
  print("Please enter the bottle's volume in mL.\n");
  scan("%d", &volume);

  print("Place the empty bottle on the weight, and press any key.\n");
  scan("");
  min = readInput();

  print("Fill the bottle to %imL, place it on the weight, and press any key.\n", volume);
  scan("");
  max = readInput();

  scale = volume/(max-min);
  min = -scale*min;
  max = volume * 1.1;
}

static long W_STACK[200];
static long B_STACK[700];
static long T_STACK[200];

static volatile double mesurements[256];
static volatile long timestamps[256];
static volatile char index = 0;

static volatile long cur_time;

void timerworker() {
  long delay = 80000;
  long next_cnt = CNT + delay;
  while(1) {
      cur_time++;
      next_cnt = waitcnt2(next_cnt, delay);
  }
}

void waterWorker(void *arg) {
  double lastKnown = readWeight();

  while(1) {
    double weight = readWeight();
    if (weight > lastKnown * 0.95 && weight < lastKnown * 1.05) {
      mesurements[index] = weight - lastKnown;
      timestamps[index] = cur_time;
      index++;
      lastKnown = weight;
    }
    pause(1000);
  }
}

void bluetoothWorker() {

  fdserial *bluetooth = fdserial_open(9, 8, 0, 115200);

  while(1) {
    int index_cp;
    memcpy(index, index_cp, sizeof(char));
    index = 0;

    double mesurements_cp[256];
    memcpy(mesurements_cp, mesurements, 256*sizeof(double));
    memset(mesurements, 0, 256*sizeof(double));

    long timestamps_cp[256];
    memcpy(timestamps_cp, timestamps, 256*sizeof(long));
    memset(timestamps, 0, 256*sizeof(long));

    for(index_cp; index_cp > 0; index_cp--) {
        writeFloat(bluetooth, mesurements_cp[index_cp]);
    }
    pause(1000);
  }
}

int main() {
  print("This is automatic water bottle mesurer made by Abdul, Tabita, and Jacob.\n");
  adc_init(21, 20, 19, 18);
  init();

  cogstart(waterWorker, NULL, W_STACK, sizeof(W_STACK));
  cogstart(bluetoothWorker, NULL, B_STACK, sizeof(B_STACK));
  cogstart(timerworker, NULL, T_STACK, sizeof(T_STACK));

  while(1) {
    ;
  }
}
