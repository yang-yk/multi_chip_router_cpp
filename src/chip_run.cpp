//#include <iostream>
//#include <vector>
//#include <stdio.h>
//#include <ctime>
//#include <cstdlib>
//#include <ctime>
//#include "chip_para.h"
//#include "phy_core.h"
//#include "phy_router.h"
//#include "single_chip.h"
//#include "chip_system.h"
//#include "log_core_graph.h"
//
//
//using namespace std;
//
//int gen_rand(int max_, int min_)
//{
//	return rand() % (max_- min_ + 1) + min_;
//}
//
//void main()
//{
//	printf("Start chip router system!\n");
//
//	chip_system test_chip_system = chip_system();
//
//	vector<phy_core_id> dst_list;
//
//	map<phy_core_id, int> dst_core_vec;
//
//	// single chip test
//	/*int router_task_num = 30;
//	for (int i = 0; i < router_task_num; i++)
//	{
//		vector<int> src_chip_id = { 0,0 };
//		vector<int> dst_chip_id = { 0,0 };
//
//		int src_core_idx = gen_rand(sys_core_numx - 1, 0);
//		int src_core_idy = gen_rand(sys_core_numy - 1, 0);
//
//		int dst_core_idx = gen_rand(sys_core_numx - 1, 0);
//		int dst_core_idy = gen_rand(sys_core_numy - 1, 0);
//
//		vector<int> src_core_id = { src_core_idx,src_core_idy };
//		vector<int> dst_core_id = { dst_core_idx,dst_core_idy };
//
//		int data_volume = gen_rand(10, 50);
//		int cur_volume = 0;
//
//		phy_core_id tmp_dst_core_id(dst_chip_id[0], dst_chip_id[1], dst_core_idx, dst_core_idy);
//		if (dst_core_vec.count(tmp_dst_core_id) == 0)
//		{
//			dst_core_vec[tmp_dst_core_id] = data_volume;
//		}
//		else
//		{
//			dst_core_vec[tmp_dst_core_id] = dst_core_vec[tmp_dst_core_id] + data_volume;
//		}
//
//		router_task router_t = router_task(src_chip_id, dst_chip_id, src_core_id, dst_core_id, data_volume, cur_volume);
//		
//		test_chip_system.add_router_task_to_system(router_t);
//		
//	}*/
//
//	/*vector<int> src_chip_id = { 0,0 };
//	vector<int> dst_chip_id = { 1,1 };
//	vector<int> src_core_id = { 15,1 };
//	vector<int> dst_core_id = { 10,3 };
//	int data_volume = 4, cur_volume = 0;
//	router_task router_t1 = router_task(src_chip_id, dst_chip_id, src_core_id, dst_core_id, data_volume, cur_volume);
//
//	src_chip_id = { 0,0 };
//	dst_chip_id = { 1,1 };
//	src_core_id = { 3,0 };
//    dst_core_id = { 4,4 };
//	data_volume = 10, cur_volume = 0;
//	router_task router_t2 = router_task(src_chip_id, dst_chip_id, src_core_id, dst_core_id, data_volume, cur_volume);*/
//	
//	//test_chip_system.add_router_task_to_system(router_t1);
//	//test_chip_system.add_router_task_to_system(router_t2);
//
//
//	int router_task_num = 50;
//	for (int i = 0; i < router_task_num; i++)
//	{
//
//		int src_chip_idx = gen_rand(sys_chip_numx-1, 0);
//		int src_chip_idy = gen_rand(sys_chip_numy-1, 0);
//
//		int dst_chip_idx = gen_rand(sys_chip_numx-1, 0);
//		int dst_chip_idy = gen_rand(sys_chip_numy-1, 0);
//
//
//		vector<int> src_chip_id = { src_chip_idx,src_chip_idy };
//		vector<int> dst_chip_id = { dst_chip_idx,dst_chip_idy };
//
//		int src_core_idx = gen_rand(sys_core_numx - 1, 0);
//		int src_core_idy = gen_rand(sys_core_numy - 1, 0);
//
//		int dst_core_idx = gen_rand(sys_core_numx - 1, 0);
//		int dst_core_idy = gen_rand(sys_core_numy - 1, 0);
//
//		vector<int> src_core_id = { src_core_idx,src_core_idy };
//		vector<int> dst_core_id = { dst_core_idx,dst_core_idy };
//
//		int data_volume = gen_rand(100, 140);
//		int cur_volume = 0;
//
//		phy_core_id tmp_dst_core_id(dst_chip_id[0], dst_chip_id[1], dst_core_idx, dst_core_idy);
//		if (dst_core_vec.count(tmp_dst_core_id) == 0)
//		{
//			dst_core_vec[tmp_dst_core_id] = data_volume;
//		}
//		else
//		{
//			dst_core_vec[tmp_dst_core_id] = dst_core_vec[tmp_dst_core_id] + data_volume;
//		}
//
//		router_task router_t = router_task(src_chip_id, dst_chip_id, src_core_id, dst_core_id, data_volume, cur_volume);
//
//		test_chip_system.add_router_task_to_system(router_t);
//
//	}
//	
//	clock_t start_clock, end_clock;
//	start_clock = clock();
//
//	for (int i = 0; i < 5000; i++)
//	{  
//		if ((i+1)%10==0)
//		   printf("-------------clock %d--------------\n", i + 1);
//		/*if (i + 1 == 3)
//		{
//			printf("pause\n");
//		}*/
//		test_chip_system.core_send_request();
//		test_chip_system.core_process_request(i+1);
//		test_chip_system.core_send_data();
// 		test_chip_system.core_update_state();
//	}
//
//	end_clock = clock();
//	cout << "The run time is: " << (double)(end_clock - start_clock) / CLOCKS_PER_SEC << "s" << endl;
//
//
//	map<phy_core_id, int>::iterator iter;
//	for (iter = dst_core_vec.begin(); iter != dst_core_vec.end(); iter++)
//	{
//		phy_core_id dst_core_id = iter->first;
//		int exp_data_vol = iter->second;
//		physical_core* dst_core = test_chip_system.get_sys_phy_core_point_from_id(dst_core_id.chip_idx, dst_core_id.chip_idy, dst_core_id.core_idx, dst_core_id.core_idy);
//		int rev_data_vol = dst_core->data_rev_memory.size();
//		printf("expected data vol:%d; receive data vol:%d;\n", exp_data_vol, rev_data_vol);
//	}
//}