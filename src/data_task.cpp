#include <iostream>
#include <vector>
#include <stdio.h>
#include "data_task.h"

using namespace std;

router_task:: router_task(vector<int> src_chip_id_, vector<int> dst_chip_id_, vector<int> src_core_id_, vector<int> dst_core_id_, \
	int data_volume_, int cur_volume_)
{
	src_chip_id = src_chip_id_;
	dst_chip_id = dst_chip_id_;
	src_core_id = src_core_id_;
	dst_core_id = dst_core_id_;
	data_volume = data_volume_;
	cur_volume = cur_volume_;
};

void router_task::clear()
{
	src_chip_id.clear();
	dst_chip_id.clear();
	src_core_id.clear();
	dst_core_id.clear();
	data_volume = 0, cur_volume = 0;
}

router_task router_task::copy(const router_task& router_t)
{  
	return router_task(router_t.src_chip_id, router_t.dst_chip_id, router_t.src_core_id, router_t.dst_core_id, \
		router_t.data_volume, router_t.cur_volume);
}


data_info:: data_info(vector<int> src_chip_id_, vector<int> dst_chip_id_, vector<int> src_core_id_, vector<int> dst_core_id_, \
	int final_data_volume_, int cur_volume_, vector<phy_core_id> core_path_)
{
	src_chip_id = src_chip_id_;
	dst_chip_id = dst_chip_id_;
	src_core_id = src_core_id_;
	dst_core_id = dst_core_id_;
	final_data_volume = final_data_volume_;
	cur_volume = cur_volume_;
	core_path = core_path_;
};

void data_info:: clear()
{
	src_chip_id.clear();
	dst_chip_id.clear();
	src_core_id.clear();
	dst_core_id.clear();
	final_data_volume = 0, cur_volume = 0;
	core_path.clear();
};

data_info data_info:: copy(const data_info& data_i)
{
	return data_info(data_i.src_chip_id, data_i.dst_chip_id, data_i.src_core_id, data_i.dst_core_id, \
		data_i.final_data_volume, data_i.cur_volume, data_i.core_path);
}


//void test_router_task()
//{
//	vector<int> src_chip_id = { 0,0 };
//	vector<int> dst_chip_id = { 0,1 };
//	vector<int> src_core_id = { 0,0 };
//	vector<int> dst_core_id = { 0,0 };
//	int data_volume = 0, cur_volume = 0;
//	router_task router_t = router_task(src_chip_id, dst_chip_id, src_core_id, dst_core_id, data_volume, cur_volume);
//	router_task router_t1 = router_t.copy(router_t);
//	router_t1.data_volume = 1;
//	router_t1.clear();
//	printf("over");
//}
//
//void test_data_info()
//{
//	vector<int> src_chip_id = { 0,0 };
//	vector<int> dst_chip_id = { 0,1 };
//	vector<int> src_core_id = { 0,0 };
//	vector<int> dst_core_id = { 0,0 };
//	int final_data_volume = 0, cur_volume = 0;
//	phy_core_id phy_core_id_t;
//	vector<phy_core_id> core_path;
//	core_path.push_back(phy_core_id_t);
//	data_info data_t = data_info(src_chip_id, dst_chip_id, src_core_id, dst_core_id, final_data_volume, cur_volume, core_path);
//	data_info data_t1 = data_t.copy(data_t);
//	data_t1.final_data_volume = 1;
//	data_t1.clear();
//	printf("over");
//}
//
//
//
//void main()
//{
//	test_router_task();
//	test_data_info();
//}

//void main()
//{}

