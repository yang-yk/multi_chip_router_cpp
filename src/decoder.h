#ifndef DECODER_H
#define DECODER_H
#include <iostream>
#include <vector>
#include <map>
using namespace std;
vector<map<string, vector<vector<vector<int>>>>> decode_st_matrix(string path);
int decode_cores_num(string path);
map<int, vector<int> > decode_core_placement(string path);
#endif
