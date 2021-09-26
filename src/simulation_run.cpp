#include "data_task.h"
#include "chip_system.h"
#include "decoder.h"
#include "encoder.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <time.h>
using namespace std;

double send_request_time = 0.0;
double process_request_time = 0.0;
double send_data_time = 0.0;
double update_state_time = 0.0;
double control_time = 0.0;

bool is_cur_group_step_done(vector<map<string, vector<vector<vector<int>>>>>& dst_all_st_matrix, 
int group_id, int step_id, map<int, vector<int>>&core_placement, chip_system &chip_sys,
vector<map<string, vector<int>>> &all_dst_rev_data_vol, vector<int>& start, vector<int>& take, int now_clock){
    if (step_id == dst_all_st_matrix[group_id].size()-1) {
        if (now_clock >= start[group_id]+take[group_id]) return true;
        else return false;
    }
    map<string, vector<vector<vector<int>>>> &group_steps = dst_all_st_matrix[group_id];
    vector<string> step_key;
    for (auto iter = group_steps.begin(); iter != group_steps.end(); iter ++) {
        step_key.push_back(iter->first);
    }
    string cur_key = step_key[step_id];
    vector<vector<vector<int>>>& step_tasks = group_steps[cur_key];
    vector<int> logic_used_cores;
    for(int logic_core_id = 0; logic_core_id < step_tasks.size(); logic_core_id ++) {
        if (step_tasks[logic_core_id].size()>0) {
            logic_used_cores.push_back(logic_core_id);
        }
    }
    vector<vector<int> > physical_core_id;
    for (int i = 0; i < logic_used_cores.size(); i++) {
        physical_core_id.push_back(core_placement[logic_used_cores[i]]);
    }
    int num = 0;
    for (int id = 0; id < physical_core_id.size(); id++) {
        int chip_x = physical_core_id[id][0];
        int chip_y = physical_core_id[id][1];
        int core_x = physical_core_id[id][2];
        int core_y = physical_core_id[id][3];
        physical_core* phy_core = chip_sys.get_sys_phy_core_point_from_id(chip_x, chip_y, core_x, core_y);
        int cur_rev_data_vol = phy_core->data_rev_memory.size();
        int core_final_rev_data_vol = all_dst_rev_data_vol[group_id][cur_key][logic_used_cores[id]];
        if (cur_rev_data_vol != core_final_rev_data_vol) 
            return false;
        else num += 1;
    }

    if (num == physical_core_id.size()) return true;
    else {
        cout << "wrong step state"<<endl;
        return false;
    }
}

bool is_group_done(vector<map<string, vector<vector<vector<int>>>>>& dst_all_st_matrix, int group_id, vector<vector<int>>& cur_group_step) {
    map<string, vector<vector<vector<int>>>>& group_steps = dst_all_st_matrix[group_id];
    int task_num = group_steps.size();
    if (task_num == cur_group_step[group_id][1]) return true;
    else return false;
}

bool is_all_done(vector<map<string, vector<vector<vector<int>>>>>& dst_all_st_matrix, vector<vector<int>>& cur_group_step) {
    int num = 0;
    for(int group_id = 0; group_id < dst_all_st_matrix.size(); group_id++) {
        //map<string, vector<vector<vector<int>>>> group_steps = dst_all_st_matrix[group_id];
        if(!is_group_done(dst_all_st_matrix, group_id, cur_group_step)) return false;
        else num += 1;
    }
    if(num == dst_all_st_matrix.size()) return true;
    else {
        cout << "wrong overall state"<<endl;
        return false;
    }
}

vector<vector<int>>& add_tasks_to_task_pool(vector<map<string, vector<vector<vector<int>>>>>& src_all_st_matrix, int group_id, int step_id, vector<vector<int>>& cur_task_pool) {
    map<string, vector<vector<vector<int>>>> &group_steps = src_all_st_matrix[group_id];
    vector<string> step_key;
    for (auto iter = group_steps.begin(); iter != group_steps.end(); iter++) {
        step_key.push_back(iter->first);
    }
    string cur_key = step_key[step_id];
    vector<vector<vector<int>>> step_tasks = group_steps[cur_key];
    for (vector<vector<int>> single_task: step_tasks) {
        for (vector<int> task: single_task) {
            cur_task_pool.push_back(task);
        }
    }
    // cout << "group "+to_string(group_id) + " step " + cur_key + " are added to task pool!!!" << endl;
    return cur_task_pool;
}

void add_task_to_system_from_task_pool(chip_system &chip_sys, vector<vector<int>>& cur_task_pool, map<int, vector<int>>&core_placement) {
    vector<int> added_src_cores;
    for (auto iter = cur_task_pool.begin(); iter!=cur_task_pool.end();) 
    {
        vector<int> single_data_task = *iter;
        if (single_data_task.size()==0) break;
        int src_logic_core_id = single_data_task[2];
        if (find(added_src_cores.begin(), added_src_cores.end(), src_logic_core_id)!=added_src_cores.end()) {
            iter++;
            continue;
        }
        else 
        {
            added_src_cores.push_back(src_logic_core_id);
            int dst_logic_core_id = single_data_task[3];
            int data_vol = single_data_task[4];
            vector<int> src_phy_core_id = core_placement[src_logic_core_id];
            vector<int> dst_phy_core_id = core_placement[dst_logic_core_id];
            int src_chip_x = src_phy_core_id[0];
            int src_chip_y = src_phy_core_id[1];
            int src_core_x = src_phy_core_id[2];
            int src_core_y = src_phy_core_id[3];
            int dst_chip_x = dst_phy_core_id[0];
            int dst_chip_y = dst_phy_core_id[1];
            int dst_core_x = dst_phy_core_id[2];
            int dst_core_y = dst_phy_core_id[3];
            

            physical_core* phy_core = chip_sys.get_sys_phy_core_point_from_id(src_chip_x, src_chip_y, src_core_x, src_core_y);
            
            if (phy_core->data_sent_memory.size()==0) 
            {


                //cout << "add tasks to the system: src_core ("+to_string(src_chip_x) +","+to_string(src_chip_y)+","+to_string(src_core_x)+","+to_string(src_core_y)\
                +")-->dst_core("+to_string(dst_chip_x)+","+to_string(dst_chip_y)+","+to_string(dst_core_x)+","+to_string(dst_core_y)+") data_val "+to_string(data_vol) << endl;
                router_task core_router_task = router_task(vector<int>{src_chip_x, src_chip_y}, vector<int>{dst_chip_x, dst_chip_y}, vector<int>{src_core_x, src_core_y}, vector<int>{dst_core_x, dst_core_y}, data_vol, 0);
             
                iter = cur_task_pool.erase(iter);
                
                chip_sys.add_router_task_to_system(core_router_task);

                continue;
            }
            iter++;
        }
    }
}



void clear_core_after_step_done(vector<map<string, vector<vector<vector<int>>>>>& dst_all_st_matrix, int group_id, int step_id, map<int, vector<int>>&core_placement, chip_system& chip_sys) {
    map<string, vector<vector<vector<int>>>> &group_steps = dst_all_st_matrix[group_id];
    vector<string> step_key;
    for (auto iter = group_steps.begin(); iter != group_steps.end(); iter++) {
        step_key.push_back(iter->first);
    }
    string cur_key = step_key[step_id];
    vector<vector<vector<int>>> step_tasks = group_steps[cur_key];
    vector<int> logic_used_cores;
    for (int logic_core_id = 0; logic_core_id < step_tasks.size(); logic_core_id++) {
        vector<vector<int>> tasks = step_tasks[logic_core_id];
        if (tasks.size()>0) logic_used_cores.push_back(logic_core_id);
    }
    vector<vector<int>> physical_core_id;
    for (int log_id: logic_used_cores) {
        physical_core_id.push_back(core_placement[log_id]);
    }

    for (vector<int> phy_id: physical_core_id) {
        int chip_x = phy_id[0];
        int chip_y = phy_id[1];
        int core_x = phy_id[2];
        int core_y = phy_id[3];
        physical_core* phy_core = chip_sys.get_sys_phy_core_point_from_id(chip_x, chip_y, core_x, core_y);
        phy_core->clear_data_rev_memory();
        // cout << "dst_core ("+to_string(chip_x)+","+to_string(chip_y)+","+to_string(core_x)+","+to_string(core_y)+") is cleared!!"<<endl;
    }
    // cout << "group "+to_string(group_id)+" step "+cur_key+" is done"<< endl;
}

void init_chip_task(vector<vector<int>>& cur_group_step, chip_system& chip_sys, vector<vector<int>>& cur_task_pool, map<int, vector<int>>&core_placement, vector<map<string, vector<vector<vector<int>>>>>& src_all_st_matrix) {
    int length = cur_group_step.size();
    for(int group_id=0; group_id<length; group_id++) {
        int step_id = cur_group_step[group_id][1];
        add_tasks_to_task_pool(src_all_st_matrix, group_id, step_id, cur_task_pool);
        add_task_to_system_from_task_pool(chip_sys, cur_task_pool, core_placement);
    }
}

//void chip_clock_running(chip_system& chip_sys) {
//    chip_sys.core_send_request();
//    chip_sys.core_process_request();
//    chip_sys.core_send_data();
//    chip_sys.core_update_state();
//}

void chip_clock_running(chip_system& chip_sys, int clocks) {
	//printf("------------------\n");
	clock_t t1 = clock();
	chip_sys.core_send_request();
	clock_t t2 = clock();
	//cout << "send request time is: " << (double)(t2 - t1) / CLOCKS_PER_SEC << "s" << endl;
	send_request_time = send_request_time + (double)(t2 - t1) / CLOCKS_PER_SEC;
	//cout << send_request_time << endl;
	//printf("%f\n", send_request_time);
	chip_sys.core_process_request(clocks);
	clock_t t3 = clock();
	//cout << "process request time is: " << (double)(t3 - t2) / CLOCKS_PER_SEC << "s" << endl;
	process_request_time = process_request_time + (double)(t3 - t2) / CLOCKS_PER_SEC;
	chip_sys.core_send_data();
	clock_t t4 = clock();
	//cout << "send data time is: " << (double)(t4 - t3) / CLOCKS_PER_SEC << "s" << endl;
	send_data_time = send_data_time + (double)(t4 - t3) / CLOCKS_PER_SEC;
	chip_sys.core_update_state();
	clock_t t5 = clock();
	//cout << "update state time is: " << (double)(t5 - t4) / CLOCKS_PER_SEC << "s" << endl;
	update_state_time = update_state_time + (double)(t5 - t4) / CLOCKS_PER_SEC;
}

int main(int argc, char* argv[]) {
    string path_in = argv[1];
    string path_out = argv[2];
	//string path_in = "./SA_core_placement_dict/cur_core_placement_12.json";
	//string path_out = "./SA_core_placement_dict/cur_core_placement_12_result.json";

    vector<map<string, vector<int>>> all_dst_rev_data_vol;
    auto src_all_st_matrix = decode_st_matrix("./st_table_list/src_all_st_matrix.json");
    auto dst_all_st_matrix = decode_st_matrix("./st_table_list/dst_all_st_matrix.json");
    int all_used_core_num = decode_cores_num("./st_table_list/all_used_core_num.json");
    for (int group_id = 0; group_id < dst_all_st_matrix.size(); group_id++) {
        map<string, vector<vector<vector<int>>>> &group_task = dst_all_st_matrix[group_id];
        map<string, vector<int>> group_dst_rev_data_vol;
        for (auto iter = group_task.begin(); iter != group_task.end(); iter++) {
            vector<vector<vector<int>>>& step = iter->second;
            vector<int> dst_data_vol_list;
            for (vector<vector<int>>& dst_task: step) {
                int dst_data_vol = 0;
                if (dst_task.size()!=0) {
                    for (vector<int> single_data_task: dst_task) {
                        dst_data_vol += single_data_task[single_data_task.size()-1];
                    }
                }
                dst_data_vol_list.push_back(dst_data_vol);
            }
            group_dst_rev_data_vol[iter->first] = dst_data_vol_list;
        }
        all_dst_rev_data_vol.push_back(group_dst_rev_data_vol);
    }
    map<int, vector<int>> core_placement;
    core_placement = decode_core_placement(path_in);
    vector<vector<int>> cur_group_step;
    for (int i = 0; i < src_all_st_matrix.size(); i++) {
        cur_group_step.push_back(vector<int> {i, 0});
    }
    chip_system test_chip_system = chip_system();
    clock_t start_time = clock();
    int clocks = 0;
    vector<vector<int> > cur_task_pool;
    vector<int> start(dst_all_st_matrix.size(), 0);
    vector<int> take(dst_all_st_matrix.size(), 0);
    for (int group_id = 0; group_id < dst_all_st_matrix.size(); group_id++) {
        map<string, vector<vector<vector<int>>>>& group_task = dst_all_st_matrix[group_id];
        vector<vector<vector<int>>>& target_task = group_task.rbegin()->second;
        vector<int> order;
        for (vector<vector<int>>& task: target_task) {
            if (task.size()==0) continue;
            for (vector<int> single_task: task) {
                if (single_task.size()==0) continue;
                order.push_back(single_task[4]);
            }
        }
        sort(order.begin(), order.end());
        take[group_id] = *(order.rbegin());
    }
    init_chip_task(cur_group_step, test_chip_system, cur_task_pool, core_placement, src_all_st_matrix);
    while (!is_all_done(dst_all_st_matrix, cur_group_step)) {
        if (clocks%10000 == 0) 
            printf("-------------clock %d--------------\n", clocks);
        for (int group_id = 0; group_id < cur_group_step.size(); group_id++) {
            if (is_group_done(dst_all_st_matrix, group_id, cur_group_step)) continue;
            else {
                int step_id = cur_group_step[group_id][1];
                if (is_cur_group_step_done(dst_all_st_matrix, group_id, step_id, core_placement, test_chip_system, all_dst_rev_data_vol, start, take, clocks)){
                    if (step_id == dst_all_st_matrix[group_id].size()-2) {
                        start[group_id] = clocks;
                    }
                    clear_core_after_step_done(dst_all_st_matrix, group_id, step_id, core_placement, test_chip_system);
                    step_id += 1;
                    cur_group_step[group_id][1] = step_id;
                    if (step_id < dst_all_st_matrix[group_id].size()) {
                        if (step_id != dst_all_st_matrix[group_id].size()-1)
                            add_tasks_to_task_pool(src_all_st_matrix, group_id, step_id, cur_task_pool);
                    }
                }
                else continue;
            }
        }
        add_task_to_system_from_task_pool(test_chip_system, cur_task_pool, core_placement);
        chip_clock_running(test_chip_system,clocks+1);
        clocks += 1;
    }
    clock_t end_time = clock();

    cout << "The run time is: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
    printf("use %d clocks!!\n", clocks);
	printf("send_request_time:%f \n", send_request_time);
	printf("process_request_time:%f \n", process_request_time);
	printf("send_data_time:%f \n", send_data_time);
	printf("update_state_time:%f \n", update_state_time);

    extern double fpv;
    printf("fpv:%f\n", fpv);
	//printf("control_time:%f \n", control_time);
    encode_int(clocks, path_out);
	return 1;
}