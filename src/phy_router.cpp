#include <iostream>
#include <vector>
#include <stdio.h>
#include <queue>
#include <algorithm>
#include "data_task.h"
#include "chip_para.h"
#include "phy_router.h"

using namespace std;

physical_router::physical_router(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_, int memory_size_)
{
	chip_idx = chip_idx_;
	chip_idy = chip_idy_;
	core_idx = core_idx_;
	core_idy = core_idy_;
	memory_size = memory_size_;

	vector<int> src_chip_id = { -1,-1 };
	vector<int> dst_chip_id = { -1,-1 };
	vector<int> src_core_id = { -1,-1 };
	vector<int> dst_core_id = { -1,-1 };
	int final_data_volume = -1, cur_volume = -1;
	phy_core_id phy_core_id_t;
	vector<phy_core_id> core_path;
	core_path.push_back(phy_core_id_t);

	local_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	local_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	get_request = 0;
};


phy_core_id physical_router::get_core_phy_id()
{
	phy_core_id core_id;
	core_id.chip_idx = chip_idx;
	core_id.chip_idy = chip_idy;
	core_id.core_idx = core_idx;
	core_id.core_idy = core_idy;
	core_id.core_dir = core_dir;
	return core_id;
}


void physical_router::clear()
{
	chip_idx = -1;
	chip_idy = -1;
	core_idx = -1;
	core_idy = -1;
	core_dir = 1;
	memory_size = 2;

	vector<int> src_chip_id = { -1,-1 };
	vector<int> dst_chip_id = { -1,-1 };
	vector<int> src_core_id = { -1,-1 };
	vector<int> dst_core_id = { -1,-1 };
	int final_data_volume = -1, cur_volume = -1;
	phy_core_id phy_core_id_t;
	vector<phy_core_id> core_path;
	core_path.push_back(phy_core_id_t);

	local_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	local_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	
	get_request = 0;
	get_request_core_id = vector<phy_core_id>();
	data_sent_memory, data_rev_memory = queue<data_info>();
}


void physical_router::clear_get_request_state()
{
	get_request = 0;
	get_request_core_id.clear();
}

void physical_router::clear_data_rev_memory()
{
	queue<data_info> empty;
	swap(empty, data_rev_memory);
}

void physical_router::put_rev_data_into_fifo()
{
	if (local_have_rev_pack_data_info.cur_volume > 0)
	{
		pack_data_fifo.push(local_have_rev_pack_data_info.copy(local_have_rev_pack_data_info));
		vector<int> src_chip_id = { -1,-1 };
		vector<int> dst_chip_id = { -1,-1 };
		vector<int> src_core_id = { -1,-1 };
		vector<int> dst_core_id = { -1,-1 };
		int final_data_volume = -1, cur_volume = -1;
		phy_core_id phy_core_id_t;
		vector<phy_core_id> core_path;
		core_path.push_back(phy_core_id_t);
		local_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	}
	
}


phy_core_id merge_router::get_first_level_request_core()
{   
	if (get_request_core_id.size() == 0)
	{
		phy_core_id tmp_phy_core_id(-1, -1, -1, -1);
		return tmp_phy_core_id;
	}
	else if (get_request_core_id.size() == 1)
	{
		return get_request_core_id[0];
	}
	else
	{   
		vector<phy_core_id> request_core_id = get_request_core_id;
		struct phy_core_id left_split_router_id(chip_idx, chip_idy, -1, -1, 0);
		struct phy_core_id right_split_router_id(chip_idx, chip_idy, -3, -1, 0);
		struct phy_core_id bottom_split_router_id(chip_idx, chip_idy, -2, -1, 0);
		struct phy_core_id top_split_router_id(chip_idx, chip_idy, -4, -1, 0);

	    // first split router
		if (std::find(request_core_id.begin(), request_core_id.end(), left_split_router_id) != request_core_id.end())
		{
			return left_split_router_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), right_split_router_id) != request_core_id.end())
		{
			return right_split_router_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), top_split_router_id) != request_core_id.end())
		{
			return top_split_router_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), bottom_split_router_id) != request_core_id.end())
		{
			return bottom_split_router_id;
		}
		// then core
		else
		{   
			vector<int> core_idxy_sum;
			for (int i = 0; i < request_core_id.size(); i++)
			{		
				core_idxy_sum.push_back(request_core_id[i].core_idx+request_core_id[i].core_idy);			
			}
			auto min_pos = min_element(core_idxy_sum.begin(), core_idxy_sum.end());
			int index = distance(core_idxy_sum.begin(), min_pos);
			return request_core_id[index];
		}
	}

}

phy_core_id split_router::get_first_level_request_core()
{   
	if (get_request_core_id.size() == 0)
	{
		phy_core_id tmp_phy_core_id(-1, -1, -1, -1);
		return tmp_phy_core_id;
	}
	else if (get_request_core_id.size()==1)
	{
		return get_request_core_id[0];
	}
	else
	{
		printf("split router get request wrong! Error in function split router get_first_level_request_core.");
		exit(1);
	}
}


//void main()
//{
//	vector<int> test_vec = { 3,2,2,3 };
//	auto test = min_element(test_vec.begin(), test_vec.end());
//	int index = distance(test_vec.begin(), test);
//	printf("over");
//}
//




