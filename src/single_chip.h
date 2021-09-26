#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include "chip_para.h"
#include "phy_core.h"
#include "phy_router.h"

using namespace std;


class single_chip
{
public:
	int chip_idx = 0, chip_idy = 0;
	int core_numx = sys_core_numx, core_numy = sys_core_numy;
	vector<vector<physical_core>> core_array;
	single_chip() {};
	single_chip(int chip_idx, int chip_idy);
	merge_router left_merge_router, right_merge_router, top_merge_router, bottom_merge_router;
	split_router left_split_router, right_split_router, top_split_router, bottom_split_router;
		
	physical_core get_phy_core_from_id(int core_idx_, int core_idy_);
	merge_router get_merge_router_from_id(int core_idx_, int core_idy_);
	split_router get_split_router_from_id(int core_idx_, int core_idy_);

	physical_core* get_phy_core_point_from_id(int core_idx_, int core_idy_);
};


