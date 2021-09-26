#include <iostream>
#include <vector>
#include <stdio.h>
#include "chip_para.h"
#include "phy_core.h"
#include "phy_router.h"
#include "single_chip.h"

using namespace std;
//using namespace boost;


single_chip::single_chip(int chip_idx_, int chip_idy_)
{
	chip_idx = chip_idx_;
	chip_idy = chip_idy_;
	
	//printf("%d\n",(core_numx));

	for (int i = 0;  i < core_numx; i++)
	{
		vector<physical_core> core_array_y;
		for (int j = 0; j < core_numy; j++)
		{   
			physical_core init_core(chip_idx, chip_idy, i, j);
			core_array_y.push_back(init_core);
		}
		core_array.push_back(core_array_y);
	}

	left_merge_router = merge_router(chip_idx, chip_idy, -1, 0);
	right_merge_router = merge_router(chip_idx, chip_idy, -3, 0);
	top_merge_router = merge_router(chip_idx, chip_idy, -4, 0);
	bottom_merge_router = merge_router(chip_idx, chip_idy, -2, 0);

	left_split_router = split_router(chip_idx, chip_idy, -1, -1);
	right_split_router = split_router(chip_idx, chip_idy, -3, -1);
	top_split_router = split_router(chip_idx, chip_idy, -4, -1);
	bottom_split_router = split_router(chip_idx, chip_idy, -2, -1);

}

physical_core single_chip::get_phy_core_from_id(int core_idx_, int core_idy_)
{
	physical_core &select_core = core_array[core_idx_][core_idy_];
	return select_core;
}

physical_core* single_chip::get_phy_core_point_from_id(int core_idx_, int core_idy_)
{
	physical_core *select_core = &(core_array[core_idx_][core_idy_]);
	return select_core;
}

merge_router single_chip::get_merge_router_from_id(int core_idx_, int core_idy_)
{
	if (core_idx_ == -1 && core_idy_ == 0)
	{
		merge_router &select_mrouter = left_merge_router;
		return select_mrouter;
	}
	else if (core_idx_ == -3 && core_idy_ == 0)
	{
		merge_router &select_mrouter = right_merge_router;
		return select_mrouter;
	}
	else if (core_idx_ == -2 && core_idy_ == 0)
	{
		merge_router &select_mrouter = bottom_merge_router;
		return select_mrouter;
	}
	else if (core_idx_ == -4 && core_idy_ == 0)
	{
		merge_router &select_mrouter = top_merge_router;
		return select_mrouter;
	}
	else
	{
		printf("error in single_chip get_merge_router_from_id.\n");
		exit(1);
	}
}



split_router single_chip::get_split_router_from_id(int core_idx_, int core_idy_)
{
	if (core_idx_ == -1 && core_idy_ == -1)
	{
		split_router &select_srouter = left_split_router;
		return select_srouter;
	}
	else if (core_idx_ == -3 && core_idy_ == -1)
	{
		split_router &select_srouter = right_split_router;
		return select_srouter;
	}
	else if (core_idx_ == -2 && core_idy_ == -1)
	{
		split_router &select_srouter = bottom_split_router;
		return select_srouter;
	}
	else if (core_idx_ == -4 && core_idy_ == -1)
	{
		split_router &select_srouter = top_split_router;
		return select_srouter;
	}
	else
	{
		printf("error in single_chip get_split_router_from_id.\n");
		exit(1);
	}
}


//void main()
//{
//	single_chip chip(0, 0);
//	physical_core core = chip.get_phy_core_from_id(1, 0);
//	merge_router mrouter = chip.get_merge_router_from_id(-3, 0);
//	split_router srouter = chip.get_split_router_from_id(-3, -1);
//	cout << "temp" << endl;
//	printf("%s\n",srouter.type.c_str());
//	if (srouter.type == "split_router")
//	{
//		printf("this is a test!\n");
//	}
//	printf("%d\n",chip.core_array[15][9].core_idy);
//	printf("over\n");
//	//std::cout << _MSC_VER << std::endl;
//}
