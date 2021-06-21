#include <CAN.h>
#include <avr/wdt.h>
#include <cstring>

#define CAN_BITRATE 500E3
#define SAMPLE_RATE_HZ 10
#define CELL_COUNT 130
#define MEMORY_FRAME_DEPTH 20
#define Z_SCORE_THRESHOLD 10 //TODO: Find a real value for this
#define NOT_SHUTDOWN 13 //The pin that connects to the tractive system shutdown circuit. Normally high, low if we want to force a shutdown
#define WDT_PERIOD WDTO_500MS

#define INFINITY 65535

size_t valid_rows = 0; //Number of initialised rows in the memory frame
uint16_t memory_frame[MEMORY_FRAME_DEPTH][CELL_COUNT];
uint16_t *memory_frame_base = (uint16_t*) memory_frame; //Base pointer for the memory frame
uint16_t *memory_frame_start = memory_frame_base; //Current first element of the memory frame

uint16_t current_sample[CELL_COUNT];
uint16_t zscores[CELL_COUNT];

bool read_yet[CELL_COUNT];

void setup() {
  Serial.begin(9600);
  pinMode(NOT_SHUTDOWN, OUTPUT);
  if(!CAN.begin(CAN_BITRATE)){
    Serial.println("Starting CAN failed. Halting!");
    asm("BREAK");
  }

  //Initialise the current sample and z scores array to a known (and very large) value
  memset(current_sample, -1, CELL_COUNT);
  memset(zscores, -1, CELL_COUNT);

  //Enable the watchdog timer
  wdt_enable(WDT_PERIOD);
  WDTCSR |= 0b000111000;
  WDTCSR = 0b01000000 | WDT_PERIOD;
  wdt_reset();
  digitalWrite(NOT_SHUTDOWN, HIGH);
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

  //Feed the watchdog timer
  wdt_reset();

  if(fuseDetectionAlgorithm()){
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
bool fuseDetectionAlgorithm(){
  uint16_t median, med_abs_dev;
  medDev(current_sample, &median, &med_abs_dev);
  calcZscores(current_sample, median, med_abs_dev, zscores);
  updateMemory();
  return detect_fuse();
}

/*
 * Takes a memory frame input, with n most recent
 * Z-Score measurements for each parallel cell brick. Each parallel cell
 * brick is checked to see if a fuse has blown.
 *
 * Parameters:
 * memoryframe: Array of z-scores. Each row corresponds to a sample from a given time step. Each collumn corresponds to a parallel cell brick.
 * threshold: A constant integer. If the difference in z-score from the beginning of the memory frame to the end exceeds this value, then a fuse has blown.
 *
 * Returns: true if we think a fuse has blown
 */
bool detect_fuse(){

  uint16_t *ptr = nullptr;
  uint16_t *next_ptr = memory_frame_start;
  #if MEMORY_FRAME_DEPTH < 1
    #error do while will break for an array shorter than two-tall
  #endif
  
  do{
    ptr = next_ptr;
    //Measures difference between first and last values in each row, and compares to threshold
    if(abs(*ptr - *(ptr + (CELL_COUNT - 1))) > Z_SCORE_THRESHOLD){
      return true;
    }
    next_ptr = (ptr - memory_frame_base + CELL_COUNT) % (CELL_COUNT * MEMORY_FRAME_DEPTH) + memory_frame_base;
  }while(next_ptr != memory_frame_start);
  return false;
}

/*
 * Adds newest sample to the memory frame, dropping the oldest one
 */
void updateMemory(){
  // Calculate new start position
  int offset = (memory_frame_start - memory_frame_base - CELL_COUNT) % (CELL_COUNT * MEMORY_FRAME_DEPTH);
  if(offset < 0) offset = sizeof(memory_frame)/sizeof(uint16_t) + offset; // Add since offset is negative
  memory_frame_start = memory_frame_base + offset;
  // Copy new row, overwriting oldest row
  memcpy(memory_frame_start, current_sample, CELL_COUNT * sizeof(uint16_t));

  // Increment valid rows
  if (valid_rows < CELL_COUNT) {
    valid_rows++;
  }
}

// FIXME: is there a way to make this less computationally expensive? If not, that's okay. But worth
// trying to find a method that's a) faster and b) occupies less memory. But I also don't know how large
// these arrays are, haven't read all the code yet
//
// This only uses 2N (260 bytes) of heap space so I don't think we can shrink it any farther. I wasn't
// able to find a faster algorithm that doesn't do estimations, lmk if you can find anything -Ege
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
void calcZscores(uint16_t* sample, uint16_t median, uint16_t med_abs_dev, uint16_t *zscores) {
  if (med_abs_dev != 0)
    for (int i = 0; i < CELL_COUNT; i++) zscores[i] = (sample[i] - median) / med_abs_dev;
  else
    for (int i = 0; i < CELL_COUNT; i++) zscores[i] = INFINITY;
}

ISR(WDT_vect){
  digitalWrite(NOT_SHUTDOWN, LOW);
  Serial.println("Watchdog not fed. Shutting car down NOW!");
  while(1);
}
