#pragma once

#define Z_SCORE_THRESHOLD 10 //TODO: Find a real value for this

int comp(const void* a, const void* b);
bool fuseDetectionAlgorithm(Matrix & memory_frame, float* sample, int size, int threshold);
bool detect_fuse(Matrix memory_frame, int threshold);
void updateMemory(Matrix & memory_frame, float * sample);
void medDev(float* sample, int size, float* median, float* med_abs_dev);
float * calcZscores(float* sample, int size, float median, float med_abs_dev);
