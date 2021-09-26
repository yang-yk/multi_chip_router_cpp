#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include "chip_para.h"
#include "phy_core.h"
#include "phy_router.h"
#include "single_chip.h"
#include "log_core_graph.h"


using namespace std;

class chip_system
{
public:
	int chip_numx = sys_chip_numx;
	int chip_numy = sys_chip_numy;
	vector<vector<single_chip>> chip_array;

	vector<phy_core_id> visited_cores;
	vector<phy_core_id> visited_routers;
	
	vector <map <phy_core_id, vector<phy_core_id>>> graphs;
	vector <phy_core_id> lasts;
	vector<vector<phy_core_id>> multi_chip_all_used_cores;

	vector<vector<phy_core_id>> all_used_cores;

	chip_system();

	single_chip* get_chip_point_from_id(int chip_idx_,int chip_idy_);
	physical_core* get_sys_phy_core_point_from_id(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_);
	merge_router* get_sys_merge_router_point_from_id(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_);
	split_router* get_sys_split_router_point_from_id(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_);

	
	vector<phy_core_id> get_router_task_path(router_task rt, bool xy = true);

	void init_src_core(router_task rt, vector<phy_core_id> core_path);
	void add_router_task_to_system(router_task rt);

	/*vector<phy_core_id> get_visited_cores();
	vector<phy_core_id> get_visited_routers();*/

	phy_core_id  get_pre_core_id(phy_core_id cur_core_id);

	void core_send_request();
	void core_process_request(int clock=0);
	void core_send_data();
	void core_update_state();
};

extern double fpv;

template <class T>
int findPosVector(T core_id_list_, phy_core_id core_id_)
{
	clock_t a = clock();
	vector<phy_core_id>::iterator iter = find(core_id_list_.begin(), core_id_list_.end(), core_id_);
	fpv += (double)(clock()-a) / CLOCKS_PER_SEC;
	// fpv += 100;
	if (iter == core_id_list_.end())
	{
		return -1;
	}
	else {
		return distance(core_id_list_.begin(), iter);
	}
}

template <class T>
void single_core_send_request(T* get_request_core, phy_core_id get_request_core_id, phy_core_id send_request_core_id)
{
	if (get_request_core->type == "core")
	{
		if (get_request_core_id.core_dir == -1)
		{
			get_request_core->left_get_request = 1;
			get_request_core->left_get_request_core_id.push_back(send_request_core_id);
		}
		else if (get_request_core_id.core_dir == -3)
		{
			get_request_core->right_get_request = 1;
			get_request_core->right_get_request_core_id.push_back(send_request_core_id);
		}
		else if (get_request_core_id.core_dir == -2)
		{
			get_request_core->bottom_get_request = 1;
			get_request_core->bottom_get_request_core_id.push_back(send_request_core_id);
		}
		else if (get_request_core_id.core_dir == -4)
		{
			get_request_core->top_get_request = 1;
			get_request_core->top_get_request_core_id.push_back(send_request_core_id);
		}
		else if (get_request_core_id.core_dir == -5)
		{
			get_request_core->local_get_request = 1;
			get_request_core->local_get_request_core_id.push_back(send_request_core_id);
		}
		else
		{
			printf("send request wrong. Error in chip_system.h");
			exit(1);
		}
		
	}
	//return get_request_core;
}

template <class T>
void router_send_request(T* get_request_core, phy_core_id get_request_core_id, phy_core_id send_request_core_id)
{
	if (get_request_core->type == "merge_router" || get_request_core->type == "split_router")
	{
		get_request_core->get_request = 1;
		get_request_core->get_request_core_id.push_back(send_request_core_id);
	}
	//return get_request_core;
}