#include "decoder.h"
#include <fstream>
#include <json/json.h>

vector<map<string, vector<vector<vector<int>>>>> decode_st_matrix(string path) {
    fstream f;
    f.open(path);
    Json::CharReaderBuilder reader;
    Json::Value root;
    Json::String err;
    Json::parseFromStream(reader, f, &root, &err);
    auto result = vector<map<string, vector<vector<vector<int>>>>>();
    for (int i = 0; i < root.size(); i++) {
        map<string, vector<vector<vector<int>>>> target;
        Json::Value::Members members = root[i].getMemberNames();
        for (int j = 0; j < members.size(); j++){
            string key = members[j];
            target[key] = vector<vector<vector<int>>>();
            for (int ii = 0; ii < root[i][key].size(); ii++) {
                target[key].push_back(vector<vector<int>>());
                for (int jj = 0; jj < root[i][key][ii].size(); jj++) {
                    target[key][ii].push_back(vector<int>());
                    for(int kk = 0; kk < root[i][key][ii][jj].size(); kk++) {
                        target[key][ii][jj].push_back(root[i][key][ii][jj][kk].asInt());
                    }
                }
            }
        }
        result.push_back(target);
    }
    f.close();
    return result;
}

int decode_cores_num(string path) {
    fstream f;
    f.open(path);
    Json::CharReaderBuilder reader;
    Json::Value root;
    Json::String err;
    Json::parseFromStream(reader, f, &root, &err);
    int res = root[0].asInt();
    f.close();
    return res;
}

map<int, vector<int>> decode_core_placement(string path) {
    fstream f;
    f.open(path);
    Json::CharReaderBuilder reader;
    Json::Value root;
    Json::String err;
    Json::parseFromStream(reader, f, &root, &err);
    map<int, vector<int>> core_placement;
    for (int i = 0; i < root.size(); i++) {
        vector<int> temp;
        for (int j = 0; j < root[to_string(i)].size(); j++) {
            temp.push_back(root[to_string(i)][j].asInt());
        }
        core_placement[i] = temp;
    }
    return core_placement;
}
