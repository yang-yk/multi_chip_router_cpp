#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <queue>
#include <algorithm>
#include "data_task.h"

using namespace std;

class physical_core {
public:
	vector<int> chip_id, core_id;
	int chip_idx = -1, chip_idy = -1;
	int core_idx = -1, core_idy = -1;
	string type = "core";
	queue<data_info> data_sent_memory, data_rev_memory;
	//data_info src_pack_data_info;
	data_info left_for_sent_pack_data_info, left_have_rev_pack_data_info;
	data_info right_for_sent_pack_data_info, right_have_rev_pack_data_info;
	data_info top_for_sent_pack_data_info, top_have_rev_pack_data_info;
	data_info bottom_for_sent_pack_data_info, bottom_have_rev_pack_data_info;
	data_info local_for_sent_pack_data_info, local_have_rev_pack_data_info;
	int left_get_request, right_get_request, top_get_request, bottom_get_request,local_get_request=0;
	vector<phy_core_id> left_get_request_core_id, right_get_request_core_id, \
		top_get_request_core_id, bottom_get_request_core_id, local_get_request_core_id;

	physical_core() {};
	physical_core(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_);
	phy_core_id get_core_phy_id();

	void clear();
	void clear_get_request_state();
	void clear_data_rev_memory();

	void left_exchange_sent_rev();
	void right_exchange_sent_rev();
	void top_exchange_sent_rev();
	void bottom_exchange_sent_rev();
	//void local_exchange_sent_rev();

	phy_core_id get_first_level_request_core(int pos);

};
