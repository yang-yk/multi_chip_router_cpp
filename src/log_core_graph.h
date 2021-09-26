#pragma once
#include<iostream>
#include<iomanip>
#include<queue>
#include<stack>
#include<map>
#include "data_task.h"

using namespace std;



vector <map <phy_core_id, vector<phy_core_id>>> gen_graph_from_paths(vector<vector<phy_core_id>> &paths);

phy_core_id get_last_node_in_graph(map <phy_core_id, vector<phy_core_id>> &graph);

phy_core_id get_last_node_in_graph_i(int i);
vector<phy_core_id> gen_last(vector<vector<phy_core_id>>& paths);