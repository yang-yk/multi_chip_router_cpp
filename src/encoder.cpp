#include "encoder.h"
#include <json/json.h>
#include <fstream>

#if defined(GNUC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored ¡°-Wdeprecated-declarations¡±
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace std;
void encode_core_placement(const map<int, vector<int>>&core_placement, int num){
    Json::Value root;
    for (auto iter = core_placement.begin(); iter != core_placement.end(); iter ++) {
        Json::Value member;
        for (int i = 0; i < iter->second.size(); i++) {
            member[i] = iter->second[i];
        }
        root[to_string(iter->first)] = member;
    }
    Json::FastWriter fw;
    fstream os;
    os.open("core_placement/placement"+to_string(num)+".json", fstream::out);
    os << fw.write(root);
    os.close();
}

void encode_int(int clocks, string path) {
    Json::Value root;
    root[0] = clocks;
    Json::FastWriter fw;
    fstream os;
    os.open(path, fstream::out);
    os << fw.write(root);
    os.close();
}
