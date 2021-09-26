#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <queue>
#include "data_task.h"
#include "chip_para.h"

using namespace std;

class physical_router {
public:
	vector<int> chip_id, core_id;
	int chip_idx = -1, chip_idy = -1;
	int core_idx = 1, core_idy = 1;
	int core_dir = 0;
	int memory_size;
	string type = "router";
	queue<data_info> data_sent_memory, data_rev_memory;
	queue<data_info> pack_data_fifo;
	data_info local_for_sent_pack_data_info, local_have_rev_pack_data_info;

	int get_request=0;
	vector<phy_core_id> get_request_core_id;

	physical_router() {};
	physical_router(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_, int memory_size_= router_memory_size);
	phy_core_id get_core_phy_id();

	void clear();
	void clear_get_request_state();
	void clear_data_rev_memory();

	void put_rev_data_into_fifo();

	phy_core_id get_first_level_request_core();

};

class merge_router: public physical_router
{
public:
	int core_idy = 0;
	using physical_router::physical_router;
	string type = "merge_router";
	phy_core_id get_first_level_request_core();
};

class split_router : public physical_router
{
public:
	int core_idy = -1;
	using physical_router::physical_router;
	string type = "split_router";
	phy_core_id get_first_level_request_core();
};