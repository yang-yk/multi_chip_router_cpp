#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

class router_task
{
public:
	vector<int> src_chip_id, dst_chip_id, src_core_id, dst_core_id;
	int data_volume, cur_volume = 0;
	router_task() {};
	router_task(vector<int> src_chip_id_, vector<int> dst_chip_id_, vector<int> src_core_id_, vector<int> dst_core_id_, \
		int data_volume_, int cur_volume_);
	void clear();
	router_task copy(const router_task& router_t);	
};

struct phy_core_id {

	int chip_idx = -1;
	int chip_idy = -1;
	int core_idx = -1;
	int core_idy = -1;
	int core_dir = 1;

	phy_core_id(int chip_idx_=-1,int chip_idy_=-1,int core_idx_=-1,int core_idy_=-1,int core_dir_=1) 
	{
		this->chip_idx = chip_idx_;
		this->chip_idy = chip_idy_;
		this->core_idx = core_idx_;
		this->core_idy = core_idy_;
		this->core_dir = core_dir_;
	};

	bool operator == (const phy_core_id& a) const
	{
		if ((a.chip_idx == chip_idx) && (a.chip_idy == chip_idy) && (a.core_idx == core_idx) && (a.core_idy == core_idy) &&\
			(a.core_dir == core_dir))
		{
			return true;
		}
		else
		{
			return false;
		}
	};



	

	bool operator < (const phy_core_id &a) const
	{
		if (chip_idx < a.chip_idx) return true;
		if (chip_idx > a.chip_idx) return false;
		if (chip_idy < a.chip_idy) return true;
		if (chip_idy > a.chip_idy) return false;
		if (core_idx < a.core_idx) return true;
		if (core_idx > a.core_idx) return false;
		if (core_idy < a.core_idy) return true;
		if (core_idy > a.core_idy) return false;
		if (core_dir < a.core_dir) return true;
		if (core_dir > a.core_dir) return false;
		return false;
	}	
};





class data_info
{
public:
	vector<int> src_chip_id, dst_chip_id, src_core_id, dst_core_id;
	int final_data_volume = 0, cur_volume = 0;
	vector<phy_core_id> core_path;
	data_info() {};
	data_info(vector<int> src_chip_id_, vector<int> dst_chip_id_, vector<int> src_core_id_, vector<int> dst_core_id_, \
		int final_data_volume_, int cur_volume_, vector<phy_core_id> core_path_);
	void clear();
	data_info copy(const data_info& data_i);
};
