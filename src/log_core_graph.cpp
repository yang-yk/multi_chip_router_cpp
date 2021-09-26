#include<iostream>
#include<iomanip>
#include<queue>
#include<stack>
#include<set>
#include<unordered_map>
#include "data_task.h"
#include "log_core_graph.h"

using namespace std;

//int *parent;
vector<int> parent;

int root(int i)
{
	if (parent[i] < 0)
		return i;
	parent[i] = root(parent[i]);
	return parent[i];
}

void union_path(int i, int j)
{
	int ri = root(i);
	int rj = root(j);
	if (ri < rj)
		parent[rj] = ri;
	if (ri > rj)
		parent[ri] = rj;
}

inline bool core_in_graph_key(map <phy_core_id, vector<phy_core_id>> &graph, phy_core_id core_id)
{
	if (graph.count(core_id) == 0)
		return false;
	else
		return true;
}


namespace std {

	template <>
	struct hash<phy_core_id>
	{
		std::size_t operator()(const phy_core_id& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return k.core_idx<<9 | k.core_idy<<4 | k.core_dir;
		}
	};
}

map<phy_core_id, vector<phy_core_id>> t_graph, d_graph;
unordered_map<phy_core_id, int> TTs;
vector<phy_core_id> glast;
set<phy_core_id> visited;
vector<phy_core_id> gen_last(vector<vector<phy_core_id>>& paths) {
	t_graph.clear(); d_graph.clear(); glast.clear(); visited.clear();
	for (auto& path : paths)
	{
		d_graph[path[0]].push_back(path[1]);
		t_graph[path[0]].push_back(path[1]);
		t_graph[path[1]].push_back(path[0]);
	}
	for (auto& path : paths)
	{
		if (d_graph[path[1]].size() == 0 && visited.find(path[1]) == visited.end()) {
			queue<phy_core_id> q;
			q.push(path[1]);
			visited.insert(path[1]);
			while (!q.empty()) {
				auto& cur = q.front(); q.pop();
				for (auto core : t_graph[cur]) {
					if (visited.find(core) == visited.end()) {
						q.push(core);
						visited.insert(core);
					}
				}
			}
			glast.push_back(path[1]);
		}
	}
	return glast;
}

vector<set<phy_core_id>> non_last, last;
vector <map <phy_core_id, vector<phy_core_id>>> gen_graph_from_paths(vector<vector<phy_core_id>> &paths)
{
	map <phy_core_id, vector<int>> exist;
	for (int i = 0; i < paths.size(); i++)
	{
		vector<phy_core_id> &path = paths[i];
		for (auto &j : path) exist[j].push_back(i);
	}

	int N = paths.size();
	parent = vector<int>(N, -1);

	for (auto &it : exist)
		for(int i=0;i<it.second.size();i++)
			union_path(it.second[0], it.second[i]);

	non_last.clear(); last.clear();


	vector<int> to_graph(N, -1);
	vector <map <phy_core_id, vector<phy_core_id>>> graphs;
	int graph_num = 0;
	for (int i = 0; i < N; i++)
	{
		if (root(i) == i)
		{   
			vector<int> tmp;
			tmp.push_back(i);
			graph_num = graph_num + 1;
			to_graph[i] = graph_num - 1;
			map <phy_core_id, vector<phy_core_id>> graph_tmp;
			graphs.push_back(graph_tmp);

			set<phy_core_id> tmp1, tmp2;
			non_last.push_back(tmp1);
			last.push_back(tmp2);
		}

		map <phy_core_id, vector<phy_core_id>> &graph = graphs[to_graph[root(i)]];

		for (int j = 0; j < paths[i].size(); j++)
		{
			if (!core_in_graph_key(graph, paths[i][j]))
			{
				vector<phy_core_id> path_tmp;
				graph[paths[i][j]] = (path_tmp);
			}
			if (j < paths[i].size() - 1) {
				graph[paths[i][j]].push_back(paths[i][j + 1]);
				non_last[to_graph[root(i)]].insert(paths[i][j]);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		auto& path = paths[i];
		for (auto& j : path)
			if (non_last[to_graph[root(i)]].find(j) == non_last[to_graph[root(i)]].end()) {
				//cout << "here" << endl;
				last[to_graph[root(i)]].insert(j);
			}
	}

	parent.clear();
	return graphs;

}

phy_core_id get_last_node_in_graph_i(int i)
{
	return *(last[i].begin());
}

phy_core_id get_last_node_in_graph(map <phy_core_id, vector<phy_core_id>> &graph)
{
	vector<phy_core_id> last_node_list;
	map <phy_core_id, vector<phy_core_id>>::iterator  it;
	for (it = graph.begin(); it != graph.end(); ++it)
	{
		if ((it->second).size() == 0)
		{
			last_node_list.push_back(it->first);
		}
	}

	if (last_node_list.size() == 1)
		return last_node_list[0];
	else
	{
		printf("Caution. Graph has more than one last nodes.");
		return last_node_list[0];
	}

}


//void main()
//{
//	vector<vector<phy_core_id>> paths;
//	vector <phy_core_id> path1{ phy_core_id(0, 0, 0, 1), phy_core_id(0, 0, 0, 2),phy_core_id(0, 0, 0, 3) };
//	vector <phy_core_id> path2{ phy_core_id(0, 0, 0, 1), phy_core_id(0, 0, 0, 4) };
//	vector <phy_core_id> path3{ phy_core_id(0, 0, 0, 2), phy_core_id(0, 0, 0, 5) };
//	vector <phy_core_id> path4{ phy_core_id(0, 0, 0, 6), phy_core_id(0, 0, 0, 7) };
//	vector <phy_core_id> path5{ phy_core_id(0, 0, 0, 7), phy_core_id(0, 0, 0, 8) };
//
//	paths = { path1,path2,path3,path4,path5 };
//	vector <map <phy_core_id, vector<phy_core_id>>> graphs = gen_graph_from_paths(paths);
//	get_last_node_in_graph(graphs[0]);
//	get_last_node_in_graph(graphs[1]);
//	//path1.clear();
//
//	printf("pause");
//
//	
//	//gen_graph_from_paths(paths);
//
//	/*phy_core_id a(0, 0, 0, 1);
//	phy_core_id b(0, 0, 1, 1);
//	bool result1 = a < b;
//	int c = 1;
//	int d = 2;
//	bool result2 = a < b;
//	cout << result1 << endl;
//	cout << result2 << endl;*/
//}