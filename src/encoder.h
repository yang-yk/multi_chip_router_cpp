#ifndef ENCODER_H
#define ENCODER_H
#include <vector>
#include <map>
using namespace std;
void encode_core_placement(const map<int, vector<int>>&core_placement, int num);
void encode_int(int clocks, string path);
#endif