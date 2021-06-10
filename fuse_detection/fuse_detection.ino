#include <CAN.h>

#define CAN_BITRATE 500E3
#define SAMPLE_RATE_HZ 10
#define CELL_COUNT 130
#define MEMORY_FRAME_DEPTH 20
#define Z_SCORE_THRESHOLD 10 //TODO: Find a real value for this
#define NOT_SHUTDOWN 13 //The pin that connects to the tractive system shutdown circuit. Normally high, low if we want to force a shutdown

#define INFINITY 65535

size_t valid_rows = 0; //Number of initialised rows in the memory frame
uint16_t **memory_frame;
uint16_t *current_sample;

bool read_yet[CELL_COUNT];

void setup() {
  Serial.begin(9600);
  pinMode(NOT_SHUTDOWN, OUTPUT);
  digitalWrite(NOT_SHUTDOWN, HIGH);
  if(!CAN.begin(CAN_BITRATE)){
    Serial.println("Starting CAN failed. Halting!");
    asm("BREAK");
  }

  memory_frame = malloc(CELL_COUNT * sizeof(uint16_t *));
  if (memory_frame == nullptr) {
      Serial.println("Memory allocation failed. Halting!");
      asm("BREAK");
  }
  for (size_t i = 0; i < CELL_COUNT; i++) {
    memory_frame[i] = malloc(MEMORY_FRAME_DEPTH * sizeof(uint16_t));
    if (memory_frame[i] == nullptr) {
        Serial.println("Memory allocation failed. Halting!");
        asm("BREAK");
    }
  }
  current_sample = malloc(CELL_COUNT * sizeof(uint16_t));
  if (current_sample == nullptr) {
      Serial.println("Memory allocation failed. Halting!");
      asm("BREAK");
  }
  for(int i = 0; i < CELL_COUNT; i++){
    current_sample[i] = -1;
  }
}

void loop() {
  int packet_size = CAN.parsePacket();

  //If we received something:
  if(packet_size){
    //Verify packet size;
    if(packet_size != 8){
      Serial.print("Packet size wrong! Received ");
      Serial.print(packet_size);
      Serial.println(" bytes. Throwing packet away.");
      return;
    }

    //Read packet into buffer
    byte packet[8];
    for(int i = 0; i < 8; i++){
      packet[i] = CAN.read();
    }

    //Verify packet integrity
    int checksum = packet[0] + 8 + packet[0] + packet[1] + packet[2] + packet[3] + packet[4] + packet[5] + packet[6];
    checksum &= 0xFF00;
    if(checksum != packet[7]){
      Serial.print("Checksum wrong! Received ");
      Serial.print(packet[7]);
      Serial.print(" but calculated ");
      Serial.print(checksum);
      Serial.println(". Throwing packet away.");
      return;
    }

    //Save sample
    current_sample[packet[0]] = packet[2] | (packet[1] << 8);
    //Remember we did that
    read_yet[packet[0]] = true;
  }

  //If we read everything, process it
  for(int i = 0; i < CELL_COUNT; i++){
    if(!read_yet[i]){
      return;
    }
  }

  if(fuseDetectionAlgorithm(memory_frame, current_sample)){
    //If we did sense a blown fuse, shut down the traction system
    digitalWrite(NOT_SHUTDOWN, LOW);
    Serial.println("Z-score too high! Detected a blown fuse. Shutting car down NOW!");
    while(1);
  }
  //If not, wait until the next sample
  delayMicroseconds(1000000 / SAMPLE_RATE_HZ);
}


int comp(const void* a, const void* b) {
  return ((int)(*(uint16_t*)a - *(uint16_t*)b));
}

/*
 * Returns fuse flag
 */
bool fuseDetectionAlgorithm(uint16_t **memory_frame, const uint16_t *sample){
  uint16_t median, med_abs_dev;
  medDev(sample, &median, &med_abs_dev);
  uint16_t *zscores = calcZscores(sample, median, med_abs_dev);
  updateMemory(memory_frame, zscores);
  free(zscores);
  return detect_fuse(memory_frame);
}

/*
 * Takes a memory frame input, with n most recent
 * Z-Score measurements for each parallel cell brick. Each parallel cell
 * brick is checked to see if a fuse has blown.
 *
 * Parameters:
 * memoryframe: Array of z-scores. Each column corresponds to a sample from a given time step. Each row corresponds to a parallel cell brick.
 * threshold: A constant integer. If the difference in z-score from the beginning of the memory frame to the end exceeds this value, then a fuse has blown.
 *
 * Returns: true if we think a fuse has blown
 */
bool detect_fuse(uint16_t **memory_frame){
  for(int i = 0; i < CELL_COUNT; i++){
    //Measures difference between first and last values in each row, and compares to threshold
    if(abs(memory_frame[i][0] - memory_frame[i][CELL_COUNT - 1]) > Z_SCORE_THRESHOLD){
      return true;
    }
  }
  return false;
}

/*
 * Copies in the new sample and shifts the memory by one row
 */
 // FIXME: we're talking about shifting nearly ALL the on-chip memory here. Why not just
 // adjust a pointer to the current row? This is so computationally expensive right here
 //
 // Will do this once I get external memory working -I don't want to step on malloc's toes.
 // I'll put the memory frame on external memory, then have it read circularly until it
 // reaches the end pointer. This function would move both pointers up by 1 and write the
 // new sample into where the old begin pointer pointed. -Ege
void updateMemory(uint16_t **memory_frame, uint16_t *sample){
  //Shift all the collumns by one
  for(int i = CELL_COUNT - 1; i > 0; i--){
    if (i == valid_rows) {
      break;
    }
    for(int j = 0; j < CELL_COUNT; j++){
      memory_frame[j][i] = memory_frame[j][i-1];
    }
  }

  //Copy in the sample
  for(int i = 0; i < MEMORY_FRAME_DEPTH; i++){
    memory_frame[i][0] = sample[i];
  }
  if (valid_rows < CELL_COUNT) {
    valid_rows++;
  }
}

// FIXME: is there a way to make this less computationally expensive? If not, that's okay. But worth
// trying to find a method that's a) faster and b) occupies less memory. But I also don't know how large
// these arrays are, haven't read all the code yet
void medDev(uint16_t* sample, uint16_t* median, uint16_t* med_abs_dev) {
  //  Takes input column vector (sample), calculates median and median absolute deviation.

  // copied the array so that the original array doesn't get sorted
  float* arr = malloc(sizeof(uint16_t) * CELL_COUNT);
  if (arr == nullptr) {
      Serial.println("Memory allocation failed. Halting!");
      asm("BREAK");
  }
  for (int i = 0; i < CELL_COUNT; i++) arr[i] = sample[i];

  // sort values, find median
  qsort(arr, CELL_COUNT, sizeof(uint16_t), comp);
  if (CELL_COUNT % 2 == 0) *median = (arr[(CELL_COUNT / 2) - 1] + arr[CELL_COUNT/ 2]) / 2;
  else *median = arr[CELL_COUNT / 2];

  // calculate med_abs_dev: subtract median from all values and find absolute value
  for (int j = 0; j < CELL_COUNT; j++) {
    arr[j] = arr[j] - (*median);
    if (arr[j] < 0) arr[j] = -1 * arr[j];
  }

  //sort values, find median
  qsort(arr, CELL_COUNT, sizeof(uint16_t), comp);
  if (CELL_COUNT % 2 == 0) *med_abs_dev = (arr[(CELL_COUNT / 2) - 1] + arr[CELL_COUNT / 2]) / 2;
  else *med_abs_dev = arr[CELL_COUNT / 2];

  free(arr);
}

// Every element of z-scores array = (Element of sample array - median)/Med_abs_dev
uint16_t * calcZscores(uint16_t* sample, uint16_t median, uint16_t med_abs_dev) {
  uint16_t* zscores = malloc(sizeof(uint16_t) * CELL_COUNT); // since the number of zscores = size of sample
  if (med_abs_dev != 0)
    for (int i = 0; i < CELL_COUNT; i++) zscores[i] = (sample[i] - median) / med_abs_dev;
  else
    for (int i = 0; i < CELL_COUNT; i++) zscores[i] = INFINITY;
  return zscores;
}
