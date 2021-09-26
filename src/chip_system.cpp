#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>
#include "chip_para.h"
#include "phy_core.h"
#include "phy_router.h"
#include "single_chip.h"
#include "chip_system.h"
#include "log_core_graph.h"

using namespace std;

double fpv = 0;

chip_system::chip_system()
{
	for (int i = 0; i < chip_numx; i++)
	{
		vector<single_chip> chip_array_y;
		for (int j = 0; j < chip_numy; j++)
		{
			chip_array_y.push_back(single_chip(i, j));
		}
		chip_array.push_back(chip_array_y);
	}
}


single_chip* chip_system::get_chip_point_from_id(int chip_idx_, int chip_idy_)
{
	return &chip_array[chip_idx_][chip_idy_];
}

physical_core* chip_system::get_sys_phy_core_point_from_id(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_)
{	
	return &chip_array[chip_idx_][chip_idy_].core_array[core_idx_][core_idy_];	
}

merge_router* chip_system:: get_sys_merge_router_point_from_id(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_)
{
	if (core_idx_ == -1 && core_idy_ == 0)
		return &chip_array[chip_idx_][chip_idy_].left_merge_router;
	else if (core_idx_ == -3 && core_idy_ == 0)
		return &chip_array[chip_idx_][chip_idy_].right_merge_router;
	else if (core_idx_ == -2 && core_idy_ == 0)
		return &chip_array[chip_idx_][chip_idy_].bottom_merge_router;
	else if (core_idx_ == -4 && core_idy_ == 0)
		return &chip_array[chip_idx_][chip_idy_].top_merge_router;
	else
	{
		printf("can not get the merge router.");
		exit(1);
	}	
}

split_router* chip_system:: get_sys_split_router_point_from_id(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_)
{
	if (core_idx_ == -1 && core_idy_ == -1)
		return &chip_array[chip_idx_][chip_idy_].left_split_router;
	else if (core_idx_ == -3 && core_idy_ == -1)
		return &chip_array[chip_idx_][chip_idy_].right_split_router;
	else if (core_idx_ == -2 && core_idy_ == -1)
		return &chip_array[chip_idx_][chip_idy_].bottom_split_router;
	else if (core_idx_ == -4 && core_idy_ == -1)
		return &chip_array[chip_idx_][chip_idy_].top_split_router;
	else
	{
		printf("can not get the merge router.");
		exit(1);
	}

}


vector<phy_core_id> chip_system::get_router_task_path(router_task rt, bool xy)
{
	int src_chip_idx = rt.src_chip_id[0];
	int src_chip_idy = rt.src_chip_id[1];
	int dst_chip_idx = rt.dst_chip_id[0];
	int dst_chip_idy = rt.dst_chip_id[1];
	
	int src_core_idx = rt.src_core_id[0];
	int src_core_idy = rt.src_core_id[1];
	int dst_core_idx = rt.dst_core_id[0];
	int dst_core_idy = rt.dst_core_id[1];

	int chip_pathx = dst_chip_idx - src_chip_idx;
	int chip_pathy = dst_chip_idy - src_chip_idy;

	vector<phy_core_id> rt_path;
	// src core
	rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx, src_core_idy));
	// same chip
	if (chip_pathx == 0 && chip_pathy == 0)
	{
		int core_pathx = dst_core_idx - src_core_idx;
		int core_pathy = dst_core_idy - src_core_idy;

		if (core_pathx < 0)
		{
			for (int i = 0; i < abs(core_pathx);i++)			
				rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx - 1 - i, src_core_idy));
		}
		else if (core_pathx > 0)
		{
			for (int i = 0; i < abs(core_pathx); i++)
				rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx + 1 + i, src_core_idy));
		}

		if (core_pathy < 0)
		{
			for (int i = 0; i < abs(core_pathy); i++)
				rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, dst_core_idx, src_core_idy-1-i));
		}
		else if (core_pathy > 0)
		{
			for (int i = 0; i < abs(core_pathy); i++)
				rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, dst_core_idx, src_core_idy+1+i));
		}


		vector<phy_core_id> rt_path_with_dir;
		phy_core_id src_core_id_with_dir(src_chip_idx, src_chip_idy, src_core_idx, src_core_idy, 0);
		rt_path_with_dir.push_back(src_core_id_with_dir);

		for (int i = 0; i < rt_path.size() - 1; i++)
		{
			phy_core_id cur_core_id = rt_path[i];
			phy_core_id next_core_id = rt_path[i + 1];

			int cur_core_idx = cur_core_id.core_idx;
			int cur_core_idy = cur_core_id.core_idy;
			int next_core_idx = next_core_id.core_idx;
			int next_core_idy = next_core_id.core_idy;

			int next_core_out_dir;
			if (cur_core_idx < 0 && next_core_idx < 0)
				next_core_out_dir = 1;
			else if (cur_core_idx >= 0 && next_core_idx < 0)
				next_core_out_dir = next_core_idx;
			else if (cur_core_idx < 0 && next_core_idx >= 0)
				next_core_out_dir = 1;
			//same chip core->core
			else if (cur_core_idy == next_core_idy && (next_core_idx - cur_core_idx == 1))
				next_core_out_dir = -3;
			else if (cur_core_idy == next_core_idy && (next_core_idx - cur_core_idx == -1))
				next_core_out_dir = -1;
			else if (cur_core_idx == next_core_idx && (next_core_idy - cur_core_idy == 1))
				next_core_out_dir = -2;
			else if (cur_core_idx == next_core_idx && (next_core_idy - cur_core_idy == -1))
				next_core_out_dir = -4;
			else
			{
				printf("core output direction wrong! Error in chip_system:get_router_task_path.");
				exit(1);
			}

			rt_path_with_dir.push_back(phy_core_id(cur_core_id.chip_idx, cur_core_id.chip_idy, \
				cur_core_id.core_idx, cur_core_id.core_idy, next_core_out_dir));
		}
		//phy_core_id last_core_id = rt_path_with_dir.back();
		rt_path_with_dir.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, dst_core_idx, \
			dst_core_idy, -5));

		return rt_path_with_dir;

	}
    // different chips
	else
	{
		//1. intra src chip x
		bool cross_chip_x = false;
		//(1) ->left
		if (chip_pathx < 0)
		{
			cross_chip_x = true;
			int core_pathx = 0 - src_core_idx;
			
			if (core_pathx < 0)
			{
				for (int i = 0; i < abs(core_pathx); i++)
					rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx - 1 - i, src_core_idy));
			}
			//(2) ->src chip left merge
			rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, -1, 0));
		}
		//(1) -> right
		else if (chip_pathx > 0)
		{
			cross_chip_x = true;
			int core_pathx = sys_core_numx -1 - src_core_idx;
			if (core_pathx > 0)
			{
				for (int i = 0; i < abs(core_pathx); i++)
					rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx + 1 + i, src_core_idy));
			}
			//(2) ->src chip right merge
			rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, -3, 0));
		}
		
		// 1. intra src chip y
		if (!cross_chip_x)
		{
			if (chip_pathy < 0)
			{
				int core_pathy = 0 - src_core_idy;
				if (core_pathy < 0)
				{
					for (int i = 0; i < abs(core_pathy); i++)
						rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx, src_core_idy-1-i));				
				}
				rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, -4, 0));
			}
			else if (chip_pathy > 0)
			{
				int core_pathy = sys_core_numy -1 - src_core_idy;
				if (core_pathy > 0)
				{
					for (int i = 0; i < abs(core_pathy); i++)
						rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, src_core_idx, src_core_idy+1+i));
				}
				rt_path.push_back(phy_core_id(src_chip_idx, src_chip_idy, -2, 0));
			}

		}

		//2. inter chip 
		// right->left
		if (chip_pathx < 0)
		{
			for (int i = 0; i < abs(chip_pathx); i++)
			{
				if (i == abs(chip_pathx) - 1)
					rt_path.push_back(phy_core_id(src_chip_idx - 1 - i, src_chip_idy, -3, -1));
				else
				{
					rt_path.push_back(phy_core_id(src_chip_idx - 1 - i, src_chip_idy, -3, -1));
					rt_path.push_back(phy_core_id(src_chip_idx - 1 - i, src_chip_idy, -1, 0));
				}
			}
		}
		//left->right
		else if (chip_pathx > 0)
		{
			for (int i = 0; i < abs(chip_pathx); i++)
			{
				if (i == abs(chip_pathx) - 1)
					rt_path.push_back(phy_core_id(src_chip_idx + 1 + i, src_chip_idy, -1, -1));
				else
				{
					rt_path.push_back(phy_core_id(src_chip_idx + 1 + i, src_chip_idy, -1, -1));
					rt_path.push_back(phy_core_id(src_chip_idx + 1 + i, src_chip_idy, -3, 0));
				}
			}

		}

		//bottom->top
		if (chip_pathy < 0)
		{
			if (cross_chip_x)
				rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy, -4, 0));
			for (int i = 0; i < abs(chip_pathy); i++)
			{
				if (i == abs(chip_pathy) - 1)
					rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy-1-i, -2, -1));
				else
				{
					rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy - 1 - i, -2, -1));
					rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy - 1 - i, -4, 0));
				}
			}		

		}
		//top->bottom
		else if (chip_pathy > 0)
		{
			if (cross_chip_x)
				rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy, -2, 0));
			for (int i = 0; i < abs(chip_pathy); i++)
			{
				if (i == abs(chip_pathy) - 1)
					rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy +1 +i, -4, -1));
				else
				{
					rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy +1 + i, -4, -1));
					rt_path.push_back(phy_core_id(dst_chip_idx, src_chip_idy +1 + i, -2, 0));
				}
			}
		}

		//3. intra dst chip
		phy_core_id dst_chip_in_core = rt_path.back();
		int chip_in_dir = dst_chip_in_core.core_idx;

		//right in
		if (chip_in_dir == -3)
		{
			int core_pathx = dst_core_idx - (sys_core_numx - 1);
			if (core_pathx < 0)
			{
				for (int i = 0; i < abs(core_pathx); i++)
					rt_path.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, sys_core_numx - i - 1, dst_core_idy));
			}		
		}
		//left in
		else if (chip_in_dir == -1)
		{
			int core_pathx = dst_core_idx;
			if (core_pathx > 0)
			{
				for (int i = 0; i < abs(core_pathx); i++)
					rt_path.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, i, dst_core_idy));
			}
		}
		//bottom in
		else if (chip_in_dir == -2)
		{
			int core_pathy = dst_core_idy - (sys_core_numy-1);
			if (core_pathy < 0)
			{
				for (int i = 0; i < abs(core_pathy); i++)
					//rt_path.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, dst_core_idx, dst_core_idy-i-1));
					rt_path.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, dst_core_idx, sys_core_numy - i - 1));
			}
		}
		else if (chip_in_dir == -4)
		{
			int core_pathy = dst_core_idy;
			if (core_pathy > 0)
			{
				for (int i = 0; i < abs(core_pathy); i++)
					rt_path.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, dst_core_idx,i));
			}
		}
		else
		{
			printf("chip direction wrong! Error in chip_system:get_router_task_path.");
			exit(1);
		}

		rt_path.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, dst_core_idx, dst_core_idy));

		
		vector<phy_core_id> rt_path_with_dir;
		phy_core_id src_core_id_with_dir(src_chip_idx, src_chip_idy, src_core_idx, src_core_idy, 0);
		rt_path_with_dir.push_back(src_core_id_with_dir);

		for (int i = 0; i < rt_path.size()-1; i++)
		{
			phy_core_id cur_core_id = rt_path[i];
			phy_core_id next_core_id = rt_path[i + 1];

			int cur_core_idx = cur_core_id.core_idx;
			int cur_core_idy = cur_core_id.core_idy;
			int next_core_idx = next_core_id.core_idx;
			int next_core_idy = next_core_id.core_idy;

			int next_core_out_dir;
			if (cur_core_idx < 0 && next_core_idx < 0)
				next_core_out_dir = 1;
			else if (cur_core_idx >= 0 && next_core_idx < 0)
				next_core_out_dir = next_core_idx;
			else if (cur_core_idx < 0 && next_core_idx >= 0)
				next_core_out_dir = 1;
			//same chip core->core
			else if (cur_core_idy == next_core_idy && (next_core_idx - cur_core_idx == 1))
				next_core_out_dir = -3;
			else if (cur_core_idy == next_core_idy && (next_core_idx - cur_core_idx == -1))
				next_core_out_dir = -1;
			else if (cur_core_idx == next_core_idx && (next_core_idy - cur_core_idy == 1))
				next_core_out_dir = -2;
			else if (cur_core_idx == next_core_idx && (next_core_idy - cur_core_idy == -1))
				next_core_out_dir = -4;
			else
			{
				printf("core output direction wrong! Error in chip_system:get_router_task_path.");
				exit(1);
			}			

			rt_path_with_dir.push_back(phy_core_id(cur_core_id.chip_idx, cur_core_id.chip_idy, \
				cur_core_id.core_idx, cur_core_id.core_idy, next_core_out_dir));
		}

		//phy_core_id last_core_id = rt_path_with_dir.back();
		rt_path_with_dir.push_back(phy_core_id(dst_chip_idx, dst_chip_idy, dst_core_idx, \
			dst_core_idy, -5));

		return rt_path_with_dir;
	}
}

void chip_system::init_src_core(router_task rt, vector<phy_core_id> core_path)
{
	int src_chip_idx = rt.src_chip_id[0];
	int src_chip_idy = rt.src_chip_id[1];
	int src_core_idx = rt.src_core_id[0];
	int src_core_idy = rt.src_core_id[1];

	physical_core* src_core = get_sys_phy_core_point_from_id(src_chip_idx, src_chip_idy, src_core_idx, src_core_idy);

	for (int i = 0; i < rt.data_volume; i++)
		src_core->data_sent_memory.push(data_info(rt.src_chip_id, rt.dst_chip_id, rt.src_core_id, rt.dst_core_id, rt.data_volume, \
			i + 1, core_path));
	/*src_core->src_pack_data_info = data_info(rt.src_chip_id, rt.dst_chip_id, rt.src_core_id, rt.dst_core_id, rt.data_volume, \
		1, core_path);*/
	//chip_array[src_chip_idx][src_chip_idy].core_array[src_core_idx][src_core_idy] = src_core;
}

void chip_system::add_router_task_to_system(router_task rt)
{
	vector<phy_core_id> core_path = get_router_task_path(rt);
	init_src_core(rt, core_path);
}


void chip_system::core_send_request()
{
	// core send request;
	vector<vector<phy_core_id>> all_core_edges;
	for (int i = 0; i < sys_chip_numx; i++)
		for (int j = 0; j < sys_chip_numy; j++)
			for (int m = 0; m < sys_core_numx; m++)
				for (int n = 0; n < sys_core_numy; n++)
				{
					//printf("visiting (%d,%d,%d,%d)\n", i, j, m, n);
					physical_core* cur_core = get_sys_phy_core_point_from_id(i, j, m, n);
					if (cur_core->left_for_sent_pack_data_info.cur_volume > 0)
					{
						phy_core_id cur_core_id(i, j, m, n, -1);
						vector<phy_core_id> core_path = cur_core->left_for_sent_pack_data_info.core_path;
						int id = findPosVector(core_path, cur_core_id);
						//if (id < 0 || id == core_path.size() - 1)
						if (id < 0)
						{
							printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							exit(1);
						}
						else
						{
							int next_id = id + 1;
							phy_core_id next_core_id = core_path[next_id];
							if (next_core_id.core_dir < 0)
								//core to core
							{
								physical_core* next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								single_core_send_request(next_core, next_core_id, cur_core_id);
							}

							else
							{
								merge_router* next_router = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								router_send_request(next_router, next_core_id, cur_core_id);
							}
							vector<phy_core_id> tmp_core_edge;
							tmp_core_edge.push_back(cur_core_id);
							tmp_core_edge.push_back(next_core_id);
							all_core_edges.push_back(tmp_core_edge);
						}
					}

					if (cur_core->right_for_sent_pack_data_info.cur_volume > 0)
					{
						phy_core_id cur_core_id(i, j, m, n, -3);
						vector<phy_core_id> core_path = cur_core->right_for_sent_pack_data_info.core_path;
						int id = findPosVector(core_path, cur_core_id);
						if (id < 0)
						//if (id < 0 || id == core_path.size() - 1)
						{
							printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							exit(1);
						}
						else
						{
							int next_id = id + 1;
							phy_core_id next_core_id = core_path[next_id];
							if (next_core_id.core_dir < 0)
								//core to core
							{
								physical_core* next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
							    single_core_send_request(next_core, next_core_id, cur_core_id);
							}

							else 
							{
								merge_router* next_router = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								router_send_request(next_router, next_core_id, cur_core_id);
							}
							vector<phy_core_id> tmp_core_edge;
							tmp_core_edge.push_back(cur_core_id);
							tmp_core_edge.push_back(next_core_id);
							all_core_edges.push_back(tmp_core_edge);
						}
					}

					if (cur_core->bottom_for_sent_pack_data_info.cur_volume > 0)
					{
						phy_core_id cur_core_id(i, j, m, n, -2);
						vector<phy_core_id> core_path = cur_core->bottom_for_sent_pack_data_info.core_path;
						int id = findPosVector(core_path, cur_core_id);
						if (id < 0)
						//if (id < 0 || id == core_path.size() - 1)
						{
							printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							exit(1);
						}
						else
						{
							int next_id = id + 1;
							phy_core_id next_core_id = core_path[next_id];
							if (next_core_id.core_dir < 0)
								//core to core
							{
								physical_core *next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								single_core_send_request(next_core, next_core_id, cur_core_id);
							}

							else
							{
								merge_router *next_router = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								router_send_request(next_router, next_core_id, cur_core_id);
							}
							vector<phy_core_id> tmp_core_edge;
							tmp_core_edge.push_back(cur_core_id);
							tmp_core_edge.push_back(next_core_id);
							all_core_edges.push_back(tmp_core_edge);
						}
					}

					if (cur_core->top_for_sent_pack_data_info.cur_volume > 0)
					{
						phy_core_id cur_core_id(i, j, m, n, -4);
						vector<phy_core_id> core_path = cur_core->top_for_sent_pack_data_info.core_path;
						int id = findPosVector(core_path, cur_core_id);
						if (id < 0)
						//if (id < 0 || id == core_path.size() - 1)
						{
							printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							exit(1);
						}
						else
						{
							int next_id = id + 1;
							phy_core_id next_core_id = core_path[next_id];
							if (next_core_id.core_dir < 0)
								//core to core
							{
								physical_core *next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								single_core_send_request(next_core, next_core_id, cur_core_id);
							}
							else
							{
								merge_router *next_router = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								router_send_request(next_router, next_core_id, cur_core_id);
							}
							vector<phy_core_id> tmp_core_edge;
							tmp_core_edge.push_back(cur_core_id);
							tmp_core_edge.push_back(next_core_id);
							all_core_edges.push_back(tmp_core_edge);
						}
					}

					if (cur_core->data_sent_memory.size() > 0)
					{
						phy_core_id cur_core_id(i, j, m, n, 0);
						//vector<phy_core_id> core_path = cur_core->src_pack_data_info.core_path;
						vector<phy_core_id> core_path = cur_core->data_sent_memory.front().core_path;
						int id = findPosVector(core_path, cur_core_id);
						if (id < 0)
						//if (id < 0 || id == core_path.size() - 1)
						{
							printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							exit(1);
						}
						else
						{
							int next_id = id + 1;
							phy_core_id next_core_id = core_path[next_id];
							// core to core
							if (next_core_id.core_dir < 0)
							{
								
								physical_core* next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								single_core_send_request(next_core, next_core_id, cur_core_id);
							}
							// core to merge router
							else
							{
								merge_router *next_router = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
								router_send_request(next_router, next_core_id, cur_core_id);
							}
							vector<phy_core_id> tmp_core_edge;
							tmp_core_edge.push_back(cur_core_id);
							tmp_core_edge.push_back(next_core_id);
							all_core_edges.push_back(tmp_core_edge);
						}
					}
				}



	for(int i =0; i<chip_numx; i++)
		for (int j = 0; j < chip_numy; j++)
		{
			//router
			vector<int> router_dir = { -1,-2,-3,-4 };
			for (int k = 0; k < router_dir.size(); k++)
			{
				merge_router *mrouter = get_sys_merge_router_point_from_id(i, j, router_dir[k], 0);
				split_router *srouter = get_sys_split_router_point_from_id(i, j, router_dir[k], -1);

				phy_core_id mrouter_id(i, j, router_dir[k], 0, 1);
				phy_core_id srouter_id(i, j, router_dir[k], -1, 1);

				//merger router
				//if (mrouter->local_for_sent_pack_data_info.cur_volume > 0)
				if (mrouter->pack_data_fifo.size() > 0)
				{
					//vector<phy_core_id> core_path = mrouter->local_for_sent_pack_data_info.core_path;
					vector<phy_core_id> core_path = mrouter->pack_data_fifo.front().core_path;
					int id = findPosVector(core_path, mrouter_id);
					if (id < 0)
					//if (id < 0 || id == core_path.size() - 1)
					{
						printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
						exit(1);
					}
					else
					{
						int next_id = id + 1;
						phy_core_id next_core_id = core_path[next_id];
						//merge router to split router
						if (next_core_id.core_dir > 0)
						{
							split_router *next_router = get_sys_split_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
							router_send_request(next_router, next_core_id, mrouter_id);

						}
						else if (next_core_id.core_dir < 0)
						{
							printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							exit(1);
						}
						vector<phy_core_id> tmp_core_edge;
						tmp_core_edge.push_back(mrouter_id);
						tmp_core_edge.push_back(next_core_id);
						all_core_edges.push_back(tmp_core_edge);
					}
				}
				//split router
				//if (srouter->local_for_sent_pack_data_info.cur_volume > 0)
				if (srouter->pack_data_fifo.size() > 0)
				{
					//vector<phy_core_id> core_path = srouter->local_for_sent_pack_data_info.core_path;
					vector<phy_core_id> core_path = srouter->pack_data_fifo.front().core_path;
					int id = findPosVector(core_path, srouter_id);
					if (id < 0)
					//if (id < 0 || id == core_path.size() - 1)
					{
						printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
						exit(1);
					}
					else
					{
						int next_id = id + 1;
						phy_core_id next_core_id = core_path[next_id];
						//merge router to split router
						if (next_core_id.core_dir > 0)
						{
							merge_router *next_router = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
							router_send_request(next_router, next_core_id, srouter_id);

						}
						else if (next_core_id.core_dir < 0)
						{
							//printf("core not in the path or core is in the final postion. Error in chip_system.cpp.");
							//exit(1);
							physical_core* next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
							single_core_send_request(next_core, next_core_id, srouter_id);
						}
						vector<phy_core_id> tmp_core_edge;
						tmp_core_edge.push_back(srouter_id);
						tmp_core_edge.push_back(next_core_id);
						all_core_edges.push_back(tmp_core_edge);
					}

				}
				
			}
		}

	//create graph
	lasts = gen_last(all_core_edges);
}


phy_core_id chip_system::get_pre_core_id(phy_core_id cur_core_id)
{
	//core
	phy_core_id process_request_core_id;
	if (cur_core_id.core_dir == 0)
	{
		return phy_core_id(-1, -1, -1, -1);
	}
	else if (cur_core_id.core_dir < 0)
	{
		physical_core *cur_core = get_sys_phy_core_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
		process_request_core_id = cur_core->get_first_level_request_core(cur_core_id.core_dir);
	}
	//merge router
	else if (cur_core_id.core_dir > 0 && cur_core_id.core_idy == 0)
	{
		merge_router *mrouter = get_sys_merge_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
		process_request_core_id = mrouter->get_first_level_request_core();
	}
	else if (cur_core_id.core_dir > 0 && cur_core_id.core_idy == -1)
	{
		split_router *srouter = get_sys_split_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
		process_request_core_id = srouter->get_first_level_request_core();
	}
	else
	{
		printf("Error in core process request!");
		exit(1);
	}

	return process_request_core_id;
}


void chip_system::core_process_request(int clock)
{
	all_used_cores.clear();
	//int graph_num = graphs.size();
	int last_num = lasts.size();
	for (int i = 0; i < last_num; i++)
	{
		vector<phy_core_id> path_used_cores;
		//map <phy_core_id, vector<phy_core_id>> graph = graphs[i];
		//phy_core_id cur_node = get_last_node_in_graph(graph);
		phy_core_id cur_node = lasts[i];
		path_used_cores.push_back(cur_node); 

		bool get_final_src = false;
		while (!get_final_src)
		{   
			//printf("processing request core: (%d,%d,%d,%d,%d)\n", cur_node.chip_idx, cur_node.chip_idy, cur_node.core_idx, cur_node.core_idy, cur_node.core_dir);
			if (cur_node.core_dir == 0)
			{
				get_final_src = true;
				//break;
			}
			phy_core_id pre_node_id = get_pre_core_id(cur_node);
			cur_node = pre_node_id;
			if (cur_node.chip_idx == -1)
				break;
			path_used_cores.push_back(cur_node);
		}
		all_used_cores.push_back(path_used_cores);
	}

	multi_chip_all_used_cores.clear();
	if (clock % 4 == 0)
	{
		multi_chip_all_used_cores = all_used_cores;
	}
	else
	{
		for (int i = 0; i < all_used_cores.size(); i++)
		{   
			int path_flag = 1;
			vector<phy_core_id> path_used_cores = all_used_cores[i];
			vector<phy_core_id> multi_chip_path_used_cores;
			for (int j = 0; j < path_used_cores.size(); j++)
			{
				phy_core_id cur_core_id = path_used_cores[j];
				if (path_flag)
				{
					//merge router
					if (cur_core_id.core_dir > 0 && cur_core_id.core_idy == 0)
					{
						merge_router* mrouter = get_sys_merge_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
						if (mrouter->pack_data_fifo.size() == router_memory_size)
							path_flag = 0;
					}
					// core or split router
					else
					{
						multi_chip_path_used_cores.push_back(cur_core_id);
					}						
				}
			}
			multi_chip_all_used_cores.push_back(multi_chip_path_used_cores);
		}

	}
}

void chip_system::core_send_data()
{
	for(int i=0;i<multi_chip_all_used_cores.size();i++)
		for (int j = 0; j<multi_chip_all_used_cores[i].size(); j++)
		{
			phy_core_id cur_core_id = multi_chip_all_used_cores[i][j];
			//phy_core_id next_core_id = multi_chip_all_used_cores[i][j - 1];
			data_info cur_pack_data_info;

			// get data
			if (cur_core_id.core_dir == 0)
			{
				physical_core *cur_core = get_sys_phy_core_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
				cur_pack_data_info = cur_core->data_sent_memory.front();
				cur_core->data_sent_memory.pop();
			}
			else if (cur_core_id.core_dir < 0)
			{
				physical_core *cur_core = get_sys_phy_core_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
				if (cur_core_id.core_dir == -1)
					cur_pack_data_info = cur_core->left_for_sent_pack_data_info;
				else if (cur_core_id.core_dir == -3)
					cur_pack_data_info = cur_core->right_for_sent_pack_data_info;
				else if (cur_core_id.core_dir == -2)
					cur_pack_data_info = cur_core->bottom_for_sent_pack_data_info;
				else if (cur_core_id.core_dir == -4)
					cur_pack_data_info = cur_core->top_for_sent_pack_data_info;
			}
			else if (cur_core_id.core_dir > 0)
			{
				if (cur_core_id.core_idy == 0)
				{
					merge_router *mrouter = get_sys_merge_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
					//cur_pack_data_info = mrouter->local_for_sent_pack_data_info;
					if (mrouter->pack_data_fifo.size() > 0)
					{
						cur_pack_data_info = mrouter->pack_data_fifo.front();
						mrouter->pack_data_fifo.pop();
					}
					else
					{
						vector<int> src_chip_id = { -1,-1 };
						vector<int> dst_chip_id = { -1,-1 };
						vector<int> src_core_id = { -1,-1 };
						vector<int> dst_core_id = { -1,-1 };
						int final_data_volume = -1, cur_volume = -1;
						phy_core_id phy_core_id_t;
						vector<phy_core_id> core_path;
						core_path.push_back(phy_core_id_t);
						cur_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
					}
					
				}
				else if (cur_core_id.core_idy == -1)
				{
					split_router *srouter = get_sys_split_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
					if (srouter->pack_data_fifo.size() > 0)
					{
						cur_pack_data_info = srouter->pack_data_fifo.front();
						srouter->pack_data_fifo.pop();
					}
					else
					{
						vector<int> src_chip_id = { -1,-1 };
						vector<int> dst_chip_id = { -1,-1 };
						vector<int> src_core_id = { -1,-1 };
						vector<int> dst_core_id = { -1,-1 };
						int final_data_volume = -1, cur_volume = -1;
						phy_core_id phy_core_id_t;
						vector<phy_core_id> core_path;
						core_path.push_back(phy_core_id_t);
						cur_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
					}				
				}
			};

			// receive data
			if (cur_pack_data_info.cur_volume > 0)
			{
				int cur_path_id = findPosVector(cur_pack_data_info.core_path, cur_core_id);
				if (cur_pack_data_info.core_path.size() == 0) cout << "====== core_path size = 0 ======" << endl;
				phy_core_id next_core_id = cur_pack_data_info.core_path[cur_path_id + 1];
				if (next_core_id.core_dir == -5)
				{
					physical_core *next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
					next_core->data_rev_memory.push(cur_pack_data_info);
				}
				else if (next_core_id.core_dir < 0)
				{
					physical_core *next_core = get_sys_phy_core_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
					if (next_core_id.core_dir == -1)
					{
						next_core->left_have_rev_pack_data_info = cur_pack_data_info;
					}
					else if (next_core_id.core_dir == -3)
					{
						next_core->right_have_rev_pack_data_info = cur_pack_data_info;
					}
					else if (next_core_id.core_dir == -2)
					{
						next_core->bottom_have_rev_pack_data_info = cur_pack_data_info;
					}
					else if (next_core_id.core_dir == -4)
					{
						next_core->top_have_rev_pack_data_info = cur_pack_data_info;
					}
				}
				else if (next_core_id.core_dir > 0)
				{
					if (next_core_id.core_idy == 0)
					{
						merge_router *mrouter = get_sys_merge_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
						mrouter->local_have_rev_pack_data_info = cur_pack_data_info;
					}
					else if (next_core_id.core_idy == -1)
					{
						split_router *srouter = get_sys_split_router_point_from_id(next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy);
						srouter->local_have_rev_pack_data_info = cur_pack_data_info;
					}
				}

				//printf("(%d,%d,%d,%d,%d)->(%d,%d,%d,%d,%d) data:%d.\n", cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy, cur_core_id.core_dir, \
					next_core_id.chip_idx, next_core_id.chip_idy, next_core_id.core_idx, next_core_id.core_idy, next_core_id.core_dir, cur_pack_data_info.cur_volume);

			}		
			
		}
	
}

void chip_system::core_update_state()
{
	//clear request state
	for (int i = 0; i < sys_chip_numx; i++)
		for (int j = 0; j < sys_chip_numy; j++)
		{
			for (int m = 0; m < sys_core_numx; m++)
				for (int n = 0; n < sys_core_numy; n++)
				{
					//printf("visiting (%d,%d,%d,%d)\n", i, j, m, n);
					physical_core* cur_core = get_sys_phy_core_point_from_id(i, j, m, n);
					cur_core->clear_get_request_state();
				}

			vector<int> router_dir = { -1,-2,-3,-4 };
			for (int k = 0; k < router_dir.size(); k++)
			{
				merge_router *mrouter = get_sys_merge_router_point_from_id(i, j, router_dir[k], 0);
				split_router *srouter = get_sys_split_router_point_from_id(i, j, router_dir[k], -1);

				mrouter->clear_get_request_state();
				srouter->clear_get_request_state();	

				mrouter->put_rev_data_into_fifo();
			}
		}

	//data swap
	/*for (int i=0;i<all_used_cores.size();i++)
		for (int j = 0; j < all_used_cores[i].size(); j++)*/
	for (int i = 0; i<multi_chip_all_used_cores.size(); i++)
		for (int j = 0; j < multi_chip_all_used_cores[i].size(); j++)
		{
			//phy_core_id cur_core_id = all_used_cores[i][j];
			phy_core_id cur_core_id = multi_chip_all_used_cores[i][j];
			if (cur_core_id.core_dir < 0)
			{
				physical_core *cur_core = get_sys_phy_core_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
				if (cur_core_id.core_dir == -1)
					cur_core->left_exchange_sent_rev();
				else if (cur_core_id.core_dir == -3)
					cur_core->right_exchange_sent_rev();
				else if (cur_core_id.core_dir == -2)
					cur_core->bottom_exchange_sent_rev();
				else if (cur_core_id.core_dir == -4)
					cur_core->top_exchange_sent_rev();
			}
			else if (cur_core_id.core_dir > 0 && cur_core_id.core_idy == 0)
			{
				merge_router *mrouter = get_sys_merge_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
				mrouter->put_rev_data_into_fifo();
			}
			else if (cur_core_id.core_dir > 0 && cur_core_id.core_idy == -1)
			{
				split_router *srouter = get_sys_split_router_point_from_id(cur_core_id.chip_idx, cur_core_id.chip_idy, cur_core_id.core_idx, cur_core_id.core_idy);
				srouter->put_rev_data_into_fifo();
			}
		}

}










