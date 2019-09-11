#include "MazeData.h"
uint8_t Data2[] = {
  10, 10, 10, 10, 10, 2, 10, 10, 10, 3, 6, 10, 2, 10, 10, 3, 
  2, 10, 10, 10, 10, 9, 14, 10, 11, 5, 13, 15, 5, 7, 7, 5, 
  1, 6, 10, 10, 2, 10, 10, 10, 10, 8, 10, 10, 9, 5, 13, 5, 
  1, 5, 6, 3, 5, 6, 2, 2, 2, 3, 6, 10, 10, 0, 2, 9, 
  1, 5, 12, 9, 5, 4, 0, 0, 0, 1, 5, 7, 7, 5, 5, 7, 
  1, 5, 14, 10, 9, 12, 8, 8, 8, 9, 5, 5, 5, 5, 5, 5, 
  1, 12, 10, 10, 10, 10, 2, 10, 10, 2, 1, 5, 5, 4, 9, 5, 
  8, 10, 10, 2, 10, 11, 5, 6, 3, 5, 5, 5, 5, 5, 7, 5, 
  3, 14, 11, 5, 6, 10, 9, 12, 8, 1, 5, 5, 5, 5, 5, 5, 
  0, 10, 10, 9, 5, 14, 10, 10, 10, 9, 13, 5, 13, 5, 13, 5, 
  1, 6, 10, 10, 8, 10, 10, 10, 3, 6, 10, 8, 10, 8, 10, 1, 
  1, 5, 14, 10, 10, 10, 10, 11, 5, 5, 14, 10, 10, 10, 11, 5, 
  1, 12, 10, 10, 10, 2, 10, 10, 8, 1, 14, 10, 10, 10, 10, 9, 
  1, 14, 10, 10, 11, 5, 14, 10, 11, 12, 10, 10, 10, 10, 10, 3, 
  0, 10, 10, 2, 10, 8, 10, 10, 10, 10, 10, 10, 10, 3, 15, 5, 
  9, 14, 11, 12, 10, 10, 10, 10, 11, 14, 10, 10, 10, 8, 10, 9};
uint8_t Data1[] = {
  2, 3, 6, 10, 10, 10, 2, 10, 10, 10, 10, 3, 6, 3, 6, 3, 
  1, 5, 5, 6, 3, 6, 9, 6, 10, 10, 10, 9, 5, 5, 5, 5, 
  1, 12, 9, 5, 4, 8, 11, 12, 10, 10, 10, 3, 5, 12, 9, 5, 
  0, 10, 10, 9, 5, 6, 10, 10, 10, 10, 10, 1, 12, 10, 3, 5, 
  1, 6, 10, 2, 9, 5, 6, 2, 10, 3, 6, 9, 6, 10, 1, 5, 
  0, 9, 6, 9, 7, 5, 5, 4, 3, 5, 12, 3, 5, 6, 9, 5, 
  1, 6, 8, 10, 1, 5, 4, 1, 13, 5, 6, 9, 4, 8, 3, 5, 
  0, 9, 6, 10, 1, 5, 5, 4, 3, 5, 5, 6, 8, 2, 9, 5, 
  0, 2, 8, 3, 5, 5, 5, 4, 9, 5, 4, 9, 6, 8, 11, 5, 
  1, 12, 3, 12, 1, 5, 5, 4, 3, 12, 9, 6, 8, 11, 6, 1, 
  8, 10, 8, 10, 1, 4, 1, 5, 5, 6, 10, 1, 14, 2, 1, 5, 
  2, 10, 10, 10, 1, 5, 5, 4, 1, 4, 10, 9, 6, 9, 5, 5, 
  8, 10, 3, 14, 1, 12, 8, 1, 12, 1, 6, 3, 5, 6, 9, 5, 
  2, 3, 4, 3, 4, 10, 3, 12, 10, 9, 5, 5, 5, 4, 10, 9, 
  1, 5, 5, 5, 5, 14, 8, 10, 10, 10, 9, 12, 9, 12, 10, 3, 
  9, 12, 9, 12, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9
};
uint8_t Data[] = {
  2, 10, 10, 10, 10, 10, 2, 10, 10, 10, 2, 10, 10, 10, 10, 3, 
  8, 3, 6, 2, 10, 10, 8, 10, 10, 2, 9, 6, 10, 2, 2, 1, 
  2, 1, 5, 12, 2, 10, 10, 2, 10, 8, 10, 1, 6, 1, 5, 5, 
  1, 5, 5, 6, 1, 6, 10, 8, 10, 10, 10, 1, 5, 5, 4, 1, 
  0, 9, 12, 1, 12, 1, 6, 2, 10, 2, 3, 12, 1, 4, 1, 5, 
  1, 6, 3, 12, 3, 5, 5, 12, 2, 1, 4, 3, 5, 5, 5, 5, 
  1, 5, 12, 3, 5, 5, 4, 10, 1, 4, 1, 5, 4, 9, 5, 5, 
  1, 12, 3, 12, 8, 9, 5, 6, 1, 5, 5, 5, 5, 6, 9, 5, 
  0, 3, 5, 6, 10, 10, 1, 12, 9, 5, 4, 1, 5, 5, 6, 9, 
  1, 5, 5, 12, 10, 10, 9, 6, 10, 8, 9, 5, 4, 1, 4, 3, 
  1, 5, 4, 2, 10, 10, 3, 12, 10, 10, 2, 1, 5, 5, 5, 5, 
  0, 9, 5, 4, 3, 6, 8, 10, 2, 10, 9, 5, 5, 5, 5, 5, 
  1, 6, 9, 5, 4, 8, 2, 10, 8, 10, 10, 9, 4, 1, 5, 5, 
  1, 12, 3, 4, 8, 10, 8, 10, 10, 3, 6, 10, 9, 13, 4, 9, 
  0, 3, 12, 9, 6, 10, 10, 10, 3, 12, 9, 6, 10, 10, 8, 3, 
  9, 12, 10, 10, 8, 10, 10, 10, 8, 10, 10, 8, 10, 10, 10, 9
};
/*
uint8_t AllJapan_032_2011_classic_exp_fin_16x16[] = {
  0x2, 0xa, 0xa, 0xa, 0xa, 0xa, 0x2, 0x2, 0xa, 0xa, 0xa, 0x2, 0x2, 0x2, 0x2, 0x3, 
  0x0, 0x3, 0x6, 0x2, 0xa, 0xb, 0x5, 0x4, 0x3, 0x6, 0x3, 0xd, 0xd, 0x5, 0x5, 0x5, 
  0x1, 0x5, 0x5, 0xc, 0xa, 0x3, 0x4, 0x1, 0x4, 0x9, 0x4, 0x2, 0xa, 0x0, 0x1, 0x5, 
  0x1, 0x5, 0xc, 0x2, 0xb, 0x5, 0x5, 0x4, 0x9, 0x6, 0x9, 0x4, 0x2, 0x9, 0x5, 0x5, 
  0x1, 0x4, 0xb, 0x5, 0xe, 0x1, 0xc, 0x8, 0xa, 0x9, 0x6, 0x1, 0x5, 0x6, 0x9, 0x5, 
  0x8, 0x0, 0xb, 0xc, 0x3, 0xd, 0x6, 0xa, 0xa, 0x2, 0x1, 0x5, 0x4, 0x9, 0x6, 0x9, 
  0xa, 0x0, 0xa, 0xa, 0x9, 0x6, 0x8, 0xa, 0xa, 0x1, 0xc, 0x0, 0x1, 0xe, 0x1, 0xf, 
  0xa, 0x0, 0xb, 0x6, 0x3, 0x4, 0x3, 0x6, 0x3, 0xd, 0x6, 0x1, 0xd, 0xe, 0x8, 0x3, 
  0xa, 0x0, 0xb, 0x5, 0x5, 0x5, 0x5, 0x4, 0x9, 0x6, 0x1, 0xd, 0x6, 0x3, 0x6, 0x1, 
  0x3, 0xc, 0x3, 0x5, 0x5, 0xc, 0x0, 0x9, 0x6, 0x1, 0xd, 0x6, 0x1, 0xc, 0x9, 0x5, 
  0x1, 0xe, 0x8, 0x1, 0x4, 0x3, 0xc, 0xa, 0x9, 0xd, 0x6, 0x1, 0xd, 0xe, 0xa, 0x1, 
  0x1, 0x6, 0xb, 0xc, 0x1, 0x5, 0x7, 0x7, 0x6, 0x3, 0x5, 0x5, 0x6, 0x2, 0xa, 0x1, 
  0x1, 0x4, 0xb, 0x6, 0x1, 0xc, 0x1, 0x5, 0x5, 0xc, 0x9, 0x4, 0x1, 0xd, 0xe, 0x1, 
  0x0, 0x9, 0x6, 0x9, 0x5, 0x7, 0x4, 0x9, 0x5, 0x7, 0x6, 0x9, 0xc, 0x3, 0xe, 0x1, 
  0x0, 0x2, 0x8, 0xb, 0x5, 0x5, 0x5, 0x6, 0x9, 0x5, 0xc, 0x3, 0x6, 0x8, 0x2, 0x9, 
  0x9, 0xc, 0xa, 0xa, 0x8, 0x8, 0x8, 0x8, 0xa, 0x8, 0xa, 0x8, 0x8, 0xa, 0x8, 0xb
};
uint8_t japan2017eq[] = {
0x7, 0xe, 0x2, 0x2, 0xa, 0xa, 0x2, 0x2, 0x2, 0xa, 0xa, 0x2, 0xb, 0xe, 0x2, 0x3,
0x5, 0x6, 0x9, 0xc, 0x3, 0x6, 0x9, 0x5, 0xc, 0x3, 0x6, 0x8, 0xa, 0x2, 0x9, 0x5,
0x4, 0x0, 0xa, 0xa, 0x1, 0xc, 0x3, 0x5, 0x6, 0x9, 0xc, 0x3, 0xe, 0x0, 0x3, 0x5,
0x5, 0xc, 0x3, 0x6, 0x1, 0x6, 0x8, 0x8, 0x0, 0x3, 0x6, 0x1, 0x6, 0x9, 0xc, 0x1,
0x5, 0x6, 0x0, 0x9, 0xc, 0x0, 0x3, 0xe, 0x9, 0xd, 0x5, 0xc, 0x0, 0x3, 0x6, 0x1,
0x4, 0x9, 0xc, 0x3, 0x6, 0x9, 0x5, 0xe, 0x3, 0x6, 0x1, 0x6, 0x9, 0x4, 0x9, 0x5,
0x4, 0x2, 0x2, 0x9, 0xc, 0x3, 0x5, 0x6, 0x8, 0x1, 0xc, 0x0, 0x3, 0x4, 0x3, 0x5,
0xd, 0x5, 0xc, 0x2, 0xa, 0x8, 0x1, 0x4, 0x3, 0x5, 0x6, 0x1, 0xc, 0x1, 0xc, 0x1,
0x7, 0x5, 0x6, 0x8, 0xb, 0x7, 0xd, 0xc, 0x9, 0x4, 0x9, 0xd, 0xe, 0x9, 0x6, 0x1,
0x4, 0x8, 0x8, 0x3, 0x6, 0x9, 0xe, 0x3, 0xe, 0x9, 0x7, 0x6, 0x3, 0x6, 0x9, 0xd,
0xc, 0x3, 0x6, 0x0, 0x8, 0xa, 0x2, 0x0, 0xb, 0x7, 0xc, 0x1, 0xc, 0x8, 0x2, 0x3,
0x7, 0xc, 0x1, 0xc, 0x3, 0x6, 0x9, 0xc, 0x3, 0x5, 0x6, 0x9, 0x7, 0x6, 0x9, 0x5,
0x5, 0x6, 0x1, 0x6, 0x8, 0x8, 0x3, 0x6, 0x8, 0x8, 0x8, 0x3, 0xc, 0x8, 0x3, 0x5,
0x4, 0x1, 0xc, 0x0, 0x3, 0xe, 0x8, 0x0, 0x3, 0x6, 0xb, 0xc, 0x2, 0x2, 0x0, 0x1,
0x5, 0x5, 0x6, 0x9, 0xc, 0x3, 0x6, 0x9, 0xc, 0x0, 0xa, 0xa, 0x1, 0xd, 0xd, 0x5,
0xd, 0xc, 0x8, 0xa, 0xa, 0x8, 0x8, 0xa, 0xa, 0x8, 0xb, 0xe, 0x8, 0xa, 0xa, 0x9
};
uint8_t apec2019[] = {
0x6, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0x3, 
0x5, 0x6, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0x1, 
0x5, 0x5, 0x7, 0xe, 0x2, 0xa, 0x3, 0xe, 0x2, 0xa, 0x3, 0xe, 0x2, 0xa, 0x3, 0x5, 
0x5, 0x5, 0x4, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0x5, 0x5, 
0x5, 0x5, 0x5, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0xa, 0x9, 0x5, 0x5, 
0x5, 0x5, 0x4, 0xb, 0xc, 0xa, 0x9, 0xe, 0x8, 0xa, 0x9, 0x6, 0x2, 0xa, 0x1, 0x5, 
0x5, 0x5, 0x5, 0x6, 0x3, 0x6, 0xa, 0xa, 0xa, 0xa, 0xa, 0x9, 0xd, 0x6, 0x9, 0x5, 
0x5, 0x5, 0x5, 0x5, 0x5, 0xc, 0xa, 0x2, 0x3, 0x6, 0xa, 0xa, 0xa, 0x9, 0x6, 0x1, 
0x5, 0x5, 0x5, 0x5, 0xc, 0xa, 0x3, 0xc, 0x9, 0xc, 0xa, 0x3, 0x6, 0xa, 0x9, 0x5, 
0x5, 0x5, 0x5, 0xc, 0x3, 0x6, 0x8, 0xa, 0xa, 0x3, 0x6, 0x9, 0xc, 0xa, 0x3, 0x5, 
0x5, 0x5, 0x5, 0x6, 0x9, 0x5, 0x6, 0xa, 0x3, 0xc, 0x9, 0x6, 0xa, 0xa, 0x1, 0x5, 
0x5, 0x5, 0x5, 0xc, 0xa, 0x1, 0xc, 0x3, 0xc, 0xa, 0x3, 0xc, 0xa, 0x3, 0x5, 0x5, 
0x5, 0x5, 0x5, 0x6, 0xa, 0x9, 0xe, 0x1, 0x6, 0xa, 0x9, 0x6, 0x3, 0x5, 0x5, 0x5, 
0x4, 0x1, 0x5, 0xc, 0xa, 0xa, 0xa, 0x9, 0xc, 0xa, 0xa, 0x9, 0xc, 0x9, 0x5, 0x5, 
0x5, 0x5, 0xc, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0x9, 0x5, 
0xd, 0xc, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0x9};
uint8_t taiwan2017f[] = {
0x6, 0xa, 0x2, 0xa, 0x2, 0x2, 0xa, 0x2, 0xa, 0xa, 0x2, 0xa, 0x2, 0xa, 0x2, 0xb, 
0x4, 0xb, 0x5, 0x7, 0x5, 0x5, 0x7, 0x4, 0x3, 0x6, 0x9, 0x6, 0x8, 0x2, 0x8, 0x3, 
0x5, 0xe, 0x1, 0xd, 0x5, 0x4, 0x1, 0x5, 0x4, 0x8, 0xb, 0xc, 0x2, 0x8, 0x2, 0x9, 
0x4, 0xa, 0x0, 0xa, 0x8, 0x1, 0xc, 0x0, 0x9, 0x6, 0xa, 0x2, 0x8, 0x2, 0x8, 0x3, 
0x5, 0x6, 0x0, 0x3, 0x6, 0x8, 0x2, 0x9, 0x6, 0x8, 0x2, 0x8, 0x2, 0x8, 0x2, 0x9, 
0xc, 0x1, 0x5, 0x4, 0x8, 0x2, 0x9, 0x6, 0x8, 0x2, 0x8, 0x2, 0x8, 0x2, 0x8, 0x3, 
0x7, 0xc, 0x0, 0x8, 0x2, 0x8, 0x3, 0xc, 0xa, 0x8, 0x2, 0x8, 0x2, 0x8, 0x2, 0x9, 
0x5, 0xe, 0x8, 0x2, 0x8, 0x2, 0x9, 0x6, 0x3, 0x7, 0xc, 0x2, 0x8, 0x2, 0x8, 0x3, 
0x5, 0x7, 0x6, 0x8, 0x2, 0x8, 0x3, 0xc, 0x8, 0x1, 0x6, 0x8, 0x2, 0x8, 0x2, 0x9, 
0xc, 0x1, 0xc, 0x2, 0x8, 0x2, 0x9, 0x6, 0xa, 0x1, 0xc, 0x2, 0x8, 0x2, 0x8, 0x3, 
0x6, 0x1, 0x7, 0xc, 0x2, 0x8, 0x2, 0x9, 0x6, 0x9, 0x6, 0x8, 0x2, 0x8, 0x2, 0x9, 
0x5, 0xc, 0x1, 0x7, 0xc, 0x2, 0x8, 0x2, 0x8, 0xb, 0xc, 0x2, 0x8, 0x2, 0x8, 0x3, 
0x4, 0xb, 0x4, 0x1, 0xe, 0x8, 0xa, 0x8, 0xb, 0xe, 0xa, 0x8, 0xa, 0x8, 0x2, 0x9, 
0x4, 0xb, 0x5, 0x4, 0x3, 0x6, 0x3, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0xc, 0x3, 
0x4, 0x3, 0xc, 0x1, 0x4, 0x1, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x1, 
0xd, 0xd, 0xe, 0x9, 0xd, 0xc, 0x9, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd};
*/
