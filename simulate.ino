//#define BNN
#define INT8

#ifdef BNN
#define NUM_RANDOM 16384
// 16384 -> 64KiB * 2
#endif
#ifdef INT8
#define NUM_RANDOM 65536
#endif

#define NUM_REPEAT 1024


#ifdef BNN
int weight[NUM_RANDOM];
int inputs[NUM_RANDOM];
#endif
#ifdef INT8
char weight[NUM_RANDOM];
char inputs[NUM_RANDOM];
#endif
int correct_value;

// from https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
// 12 instructions for 32-bit bitcount
inline int bitcount(int v) {
  v -= (v >> 1) & 0x55555555;
  v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
  return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 23; // 24 -> 23, returns `bitcount * 2`, [0, 64]
}

void setup() {
  for (int i = 0; i < NUM_RANDOM; ++i) {
#ifdef BNN
    weight[i] = random(INT32_MIN, INT32_MAX);
    inputs[i] = random(INT32_MIN, INT32_MAX);
#endif
#ifdef INT8
    weight[i] = random(INT8_MIN, INT8_MAX);
    inputs[i] = random(INT8_MIN, INT8_MAX);
#endif
  }
  correct_value = 0;
  for (int i = 0; i < NUM_RANDOM; ++i) {
#ifdef BNN
    correct_value += (bitcount(~(weight[i] ^ inputs[i])) - 32);
#endif

#ifdef INT8
    correct_value += weight[i] * inputs[i];
#endif
  }
  
  Serial.begin(9600);
}

void loop() {
  unsigned long start = micros();
  int accum;
  bool correct = true;
  for (int r = 0; r < NUM_REPEAT; ++r) {
    accum = 0;
    
    for (int i = 0; i < NUM_RANDOM; ++i) {
#ifdef BNN
      accum += (bitcount(~(weight[i] ^ inputs[i])) - 32);
#endif

#ifdef INT8
      accum += weight[i] * inputs[i];
#endif
    }

    correct = correct && (accum == correct_value);
  }

  Serial.println((double)(micros() - start) / 1000000);
  Serial.println(correct);
  delay(1000); // wait 1 second
}
