#include <stdio.h>

// logic borrowed from here:
// https://github.com/oliexdev/openScale/blob/master/android_app/app/src/main/java/com/health/openscale/core/bluetooth/BluetoothMiScale2.java
bool is_bit_set(int value, int bit) {
    return (value & (1 << bit)) != 0;
}

int main() {
  char c[255] = { 0 };
  int b[13] = { 0 };
  while (fgets(c, 255, stdin)) {
    for(int i = 0; i < 13; ++i) {
      sscanf(c + i * 3, "%02x", b + i);
    }
    int ctrl0 = b[0];
    int ctrl1 = b[1];

    bool isWeightRemoved = is_bit_set(ctrl1, 7);
    bool isStabilized = is_bit_set(ctrl1, 5);
    bool isLBSUnit = is_bit_set(ctrl0, 0);
    bool isCattyUnit = is_bit_set(ctrl1, 6);
    bool isImpedance = is_bit_set(ctrl1, 1);

    int year = ((b[3] & 0xFF) << 8) | (b[2] & 0xFF);
    int month = (int) b[4];
    int day = (int) b[5];
    int hours = (int) b[6];
    int min = (int) b[7];
    int sec = (int) b[8];

    float weight;

    if (!isStabilized || isWeightRemoved) {
      continue;
    }
    if (isLBSUnit || isCattyUnit) {
      weight = (float) (((b[12] & 0xFF) << 8) | (b[11] & 0xFF)) / 100.0f;
    }
    else {
      weight = (float) (((b[12] & 0xFF) << 8) | (b[11] & 0xFF)) / 200.0f;
    }
    if (weight < 20.0) {
      // lol, probably not
      continue;
    }
    printf("%04d-%02d-%02d %02d:%02d:%02d %f\n", year, month, day, hours, min, sec, weight);
  }
  return 0;
}
