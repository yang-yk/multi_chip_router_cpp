#include <iostream>
#include <vector>
#include <stdio.h>
#include <queue>
#include "data_task.h"
#include "phy_core.h"
#include "chip_para.h"

using namespace std;


physical_core:: physical_core(int chip_idx_, int chip_idy_, int core_idx_, int core_idy_)
{
	chip_idx = chip_idx_;
	chip_idy = chip_idy_;
	core_idx = core_idx_;
	core_idy = core_idy_;

	vector<int> src_chip_id = { -1,-1 };
	vector<int> dst_chip_id = { -1,-1 };
	vector<int> src_core_id = { -1,-1 };
	vector<int> dst_core_id = { -1,-1 };
	int final_data_volume = -1, cur_volume = -1;
	phy_core_id phy_core_id_t;
	vector<phy_core_id> core_path;
	core_path.push_back(phy_core_id_t);

	left_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	left_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	right_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	right_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	top_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	top_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	bottom_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	bottom_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	local_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	local_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	left_get_request = 0; 
	right_get_request = 0; 
	top_get_request = 0; 
	bottom_get_request = 0;
	local_get_request = 0;
};


phy_core_id physical_core::get_core_phy_id()
{
	phy_core_id core_id;
	core_id.chip_idx = chip_idx;
	core_id.chip_idy = chip_idy;
	core_id.core_idx = core_idx;
	core_id.core_idy = core_idy;
	return core_id;
}


void physical_core:: clear()
{
	chip_idx = -1;
	chip_idy = -1;
	core_idx = -1;
	core_idy = -1;

	vector<int> src_chip_id = { -1,-1 };
	vector<int> dst_chip_id = { -1,-1 };
	vector<int> src_core_id = { -1,-1 };
	vector<int> dst_core_id = { -1,-1 };
	int final_data_volume = -1, cur_volume = -1;
	phy_core_id phy_core_id_t;
	vector<phy_core_id> core_path;
	core_path.push_back(phy_core_id_t);

	left_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	left_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	right_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	right_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	top_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	top_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	bottom_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	bottom_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	local_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	local_have_rev_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
	left_get_request = 0; 
	right_get_request = 0; 
	top_get_request = 0; 
	bottom_get_request = 0; 
	local_get_request = 0;
	left_get_request_core_id = vector<phy_core_id>(); 
	right_get_request_core_id = vector<phy_core_id>();
	top_get_request_core_id = vector<phy_core_id>(), 
	bottom_get_request_core_id = vector<phy_core_id>(); 
	local_get_request_core_id = vector<phy_core_id>();

	data_sent_memory = queue<data_info>();
	data_rev_memory = queue<data_info>();
}


void physical_core::clear_get_request_state()
{
	left_get_request = 0; 
	right_get_request = 0; 
	top_get_request = 0; 
	bottom_get_request = 0; 
	local_get_request = 0;
	left_get_request_core_id.clear();
	right_get_request_core_id.clear();
	top_get_request_core_id.clear();
	bottom_get_request_core_id.clear();
	local_get_request_core_id.clear();
	/*left_get_request_core_id, right_get_request_core_id, \
		top_get_request_core_id, bottom_get_request_core_id, local_get_request_core_id = vector<phy_core_id>();*/
}

void physical_core::clear_data_rev_memory()
{
	data_rev_memory = queue<data_info>();
}

void physical_core::left_exchange_sent_rev()
{   
	left_for_sent_pack_data_info = left_have_rev_pack_data_info;
	left_have_rev_pack_data_info.clear();
}

void physical_core::right_exchange_sent_rev()
{
	right_for_sent_pack_data_info = right_have_rev_pack_data_info;
	right_have_rev_pack_data_info.clear();
}


void physical_core::top_exchange_sent_rev()
{
	top_for_sent_pack_data_info = top_have_rev_pack_data_info;
	top_have_rev_pack_data_info.clear();
}

void physical_core::bottom_exchange_sent_rev()
{
	bottom_for_sent_pack_data_info = bottom_have_rev_pack_data_info;
	bottom_have_rev_pack_data_info.clear();
}

//void physical_core::local_exchange_sent_rev()
//{
//	local_for_sent_pack_data_info = local_have_rev_pack_data_info.copy(local_have_rev_pack_data_info);
//	vector<int> src_chip_id = { -1,-1 };
//	vector<int> dst_chip_id = { -1,-1 };
//	vector<int> src_core_id = { -1,-1 };
//	vector<int> dst_core_id = { -1,-1 };
//	int final_data_volume = -1, cur_volume = -1;
//	phy_core_id phy_core_id_t;
//	vector<phy_core_id> core_path;
//	core_path.push_back(phy_core_id_t);
//	local_for_sent_pack_data_info = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
//}

phy_core_id physical_core::get_first_level_request_core(int pos)
{   
    //first->end:left,right,top,bottom,local
	
	vector<phy_core_id> request_core_id;
	if (pos == -1)
	{
		request_core_id = left_get_request_core_id;
	}
	else if(pos == -3)
	{
		request_core_id = right_get_request_core_id;
	}
	else if (pos == -4)
	{
		request_core_id = top_get_request_core_id;
	}
	else if (pos == -2)
	{
		request_core_id = bottom_get_request_core_id;
	}
	else if (pos == -5)
	{
		request_core_id = local_get_request_core_id;
	}
	else
	{
		printf("input core position is wrong! Error in function get_first_level_request_core.");
		exit(1);
	}

	if (request_core_id.size() == 0)
	{
		phy_core_id tmp_phy_core_id(-1, -1, -1, -1);
		return tmp_phy_core_id;
	}
	else if (request_core_id.size() == 1) 
	{
		return request_core_id[0];
	}
	else
	{   
		phy_core_id left_core_id, right_core_id, top_core_id, bottom_core_id;
		if (core_idx == 0) 
		{   //left split router
			left_core_id = { chip_idx,chip_idy, -1, -1, 0 };
			right_core_id = { chip_idx,chip_idy, core_idx + 1, core_idy,-1 };
	    }
		else if (core_idx == sys_core_numx-1)
		{
			left_core_id = { chip_idx,chip_idy, core_idx - 1, core_idy,-3 };
			right_core_id = { chip_idx,chip_idy, -3, -1, 0 };
		}
		else
		{   // core
			left_core_id = { chip_idx,chip_idy, core_idx - 1, core_idy,-3 };
			right_core_id = { chip_idx,chip_idy, core_idx + 1, core_idy,-1 };
		}

		if (core_idy == 0)
		{   //top split router
			top_core_id = { chip_idx,chip_idy, -4, -1,0 };
			bottom_core_id = { chip_idx,chip_idy, core_idx, core_idy + 1,-4 };
		}
		else if (core_idy == sys_core_numy-1)
		{
			top_core_id = { chip_idx,chip_idy, core_idx, core_idy - 1,-2 };
			bottom_core_id = { chip_idx,chip_idy, -2, -1,0 };
		}
		else
		{   // core
			top_core_id = { chip_idx, chip_idy, core_idx, core_idy - 1, -2 };
			bottom_core_id = { chip_idx,chip_idy, core_idx, core_idy + 1,-4 };
		}

		phy_core_id local_core_id(chip_idx, chip_idy, core_idx, core_idy, -5);
		
		if (std::find(request_core_id.begin(), request_core_id.end(), left_core_id) != request_core_id.end())
		{
			return left_core_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), right_core_id) != request_core_id.end())
		{
			return right_core_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), top_core_id) != request_core_id.end())
		{
			return top_core_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), bottom_core_id) != request_core_id.end())
		{
			return bottom_core_id;
		}
		else if (std::find(request_core_id.begin(), request_core_id.end(), local_core_id) != request_core_id.end())
		{
			return local_core_id;
		}
		else
		{
			printf("requset core is wrong! Error in core get_first_level_request_core.");
			exit(1);
		}
	}
}



