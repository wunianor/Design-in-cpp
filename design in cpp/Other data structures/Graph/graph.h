#pragma once
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <string>
#include <queue>
#include <functional>
#include <iomanip>
#include "UnionFindSet.h"

using namespace std;

//以邻接矩阵存储
namespace matrix
{
	//V是顶点的类型,
	//W是边(权重)的类型,
	//Direction为false表示无向图,
	//从一个点不能到另一个点时使用MAX_W作为标识值
	template<class V, class W,bool Direction=false, W MAX_W = INT_MAX>
	class Graph
	{
		typedef Graph<V, W, Direction, MAX_W> Self;


		vector<V> _vertexs;               //顶点集合
		unordered_map<V, int> _index_map; //顶点映射下标
		vector<vector<W>> _matrix;        //邻接矩阵


		//dfs子函数
		void _dfs(size_t srci, vector<bool>& visited,size_t depth)
		{
			//访问当前结点并标记
			cout << _vertexs[srci] << "(" << depth << ") ";
			visited[srci] = true;

			//寻找 与当前结点相邻并且未访问的点 访问
			for (size_t j = 0; j < _matrix[srci].size(); ++j)
			{
				if (_matrix[srci][j] != MAX_W && visited[j] == false)
				{
					_dfs(j, visited, depth + 1);
				}
			}
		}

	public:
		//内部类边
		struct Edge
		{
			size_t _srci;//边的起点的下标
			size_t _desi;//边的终点的下标
			W _w;//权值

			Edge(size_t srci,size_t desi,const W& w):
				_srci(srci),
				_desi(desi),
				_w(w)
			{}

			//用于排序
			bool operator>(const Edge& b) const 
			{
				return _w > b._w;
			}
		};


		//图的创建:
		//1.IO输入,适合OJ题,不适合测试
		//2.图结构关系写到文件,读取文件
		//3.手动添加边
		Graph(const V* a, size_t n)
		{
			_vertexs.reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				_vertexs.push_back(a[i]);
				_index_map[a[i]] = i;
			}

			_matrix.resize(n);
			for (size_t i = 0; i < n; ++i)
			{
				_matrix[i].resize(n, MAX_W);
			}
		}

		//获取顶点的下标
		size_t get_vertex_index(const V& vertex)
		{
			auto it = _index_map.find(vertex);
			if (it != _index_map.end())
			{
				return it->second;
			}
			else
			{
				throw invalid_argument("顶点不存在");//抛异常
				return -1;
			}
		}

		//通过下标添加边
		void add_edge_by_index(size_t srci, size_t desi, const W& w)
		{
			_matrix[srci][desi] = w;

			if (Direction == false)//无向图
			{
				_matrix[desi][srci] = w;
			}
		}

		//添加边
		void add_edge(const V& src, const V& des, const W& w)
		{
			size_t srci = get_vertex_index(src);
			size_t desi = get_vertex_index(des);

			add_edge_by_index(srci, desi, w);
		}



		//深度优先遍历
		void dfs(const V& src)
		{
			size_t srci = get_vertex_index(src);
			vector<bool> visited(_vertexs.size(),false);
			size_t depth = 0;

			cout << "dfs:\n ";
			
			_dfs(srci, visited, depth);

		}



		//广度优先遍历
		void bfs(const V& src)
		{
			size_t srci = get_vertex_index(src);//获取顶点的下标
			queue<size_t> q;
			unordered_set<size_t> visited;//存储顶点的下标,表示哪些顶点已经遍历

			q.push(srci);
			visited.insert(srci);

			size_t level_count = 0;//记录当前遍历到第几层
			size_t level_size = q.size();//记录当前层有多少个顶点

			cout << "bfs:" << endl;

			while (!q.empty())
			{
				level_size = q.size();
				cout << " 第" << level_count << "层:";
				while (level_size--)
				{
					size_t front = q.front();
					q.pop();
					cout << _vertexs[front] << " ";
					for (size_t j = 0; j < _matrix[front].size(); ++j)
					{
						//如果_vertexs[front]顶点与_vertexs[j]有边连接 且 _vertexs[j]没有遍历过
						if (_matrix[front][j] != MAX_W && visited.count(j) == 0)
						{
							q.push(j);
							visited.insert(j);
						}
					}
				}
				++level_count;
				cout << endl;
			}

		}



		//最小生成树Kruskal算法(贪心+排序(堆)+并查集)
		W kruskal(Self& min_tree,bool debug = false)
		{
			priority_queue<Edge, vector<Edge>, greater<Edge>> minpq;//小堆,按照边的权值来排序

			//找到所有的边
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				for (size_t j = (Direction == false ? i+1 : 0) ; j < _matrix[i].size(); ++j)
				{
					if (i == j) continue;//如果这条边的两个顶点是同一个顶点

					if (_matrix[i][j] != MAX_W)
					{
						minpq.push(Edge(i,j,_matrix[i][j]));
					}
				}
			}

			size_t min_tree_edge_size = 0; //最小生成树的边的条数
			W min_tree_total_w = W();      //最小生成树的总权值
			UnionFindSet ufs(_vertexs.size()); //并查集,记录哪些点是一个集合的

			//按照权值从小到大开始选边
			while (!minpq.empty())
			{
				Edge edge = minpq.top();
				minpq.pop();

				//如果这条边的两个顶点不在一个集合中(即不会成环)
				if (ufs.In_same_set(edge._srci, edge._desi) == false)
				{
					if (debug)//debug模式下打印选择的边
					{
						cout << "Selected Edge:" << _vertexs[edge._srci] << "->" << _vertexs[edge._desi] << "(w:" << edge._w << ")" << endl;
					}

					min_tree.add_edge_by_index(edge._srci, edge._desi, edge._w);
					ufs.Union(edge._srci, edge._desi);//合并集合
					min_tree_total_w += edge._w;
					++min_tree_edge_size;
				}
				else if (debug)//debug模式下打印未选择的边
				{
					cout << "Unselected Edge:" << _vertexs[edge._srci] << "->" << _vertexs[edge._desi] << "(w:" << edge._w << ")" << endl;
				}
			}

			//没找到最小生成树
			if (min_tree_edge_size != _vertexs.size() - 1)
			{
				return W();
			}

			//找到了最小生成树
			return min_tree_total_w;
		}



		//最小生成树prim算法
		W prim(Self& min_tree,const V& src,bool debug = false)
		{
			size_t srci = get_vertex_index(src);

			vector<bool> X(_vertexs.size(),false); //已经加入最小生成树的顶点 在X中标记为true,否则为false
			X[srci] = true;

			
			W min_tree_total_w = W(); //最小生成树的总权
			size_t min_tree_edge_size = 0;//当前最小生成树边的数量
			priority_queue<Edge, vector<Edge>, greater<Edge>> minpq;//小堆,存边

			//将起点为src,终点为X中被标记成false的顶点 的边push进小堆
			for (size_t j = 0; j < _matrix[srci].size(); ++j)
			{
				if (_matrix[srci][j] != MAX_W && X[j] == false)
				{
					minpq.push(Edge(srci, j, _matrix[srci][j]));
				}
			}

			while (!minpq.empty())
			{
				Edge min_edge = minpq.top();
				minpq.pop();


				//判断是否会成环 
				//(取出的边的起点是否已加入最小生成树(标记成true),终点是否没有加入最小生成树(标记为false))
				if (X[min_edge._srci] == true && X[min_edge._desi] == false)
				{
					min_tree.add_edge_by_index(min_edge._srci, min_edge._desi, min_edge._w);
					min_tree_total_w += min_edge._w;
					++min_tree_edge_size;

					if (debug)//debug模式下打印选择的边
					{
						cout << "Selected Edge:" << _vertexs[min_edge._srci] << "->" << _vertexs[min_edge._desi] << "(w:" << min_edge._w << ")" << endl;
					}

					X[min_edge._desi] = true;//将min_edge._desi标记成true(已加入最小生成树)
						
					//将起点为min_edge._desi,终点为在X被标识成false的顶点 的边push进小堆
					for (size_t j = 0; j < _matrix[min_edge._desi].size(); ++j)
					{
						if (_matrix[min_edge._desi][j] != MAX_W && X[j] == false)
						{
							minpq.push(Edge(min_edge._desi, j, _matrix[min_edge._desi][j]));
						}
					}
				}
				else if (debug)//debug模式下打印未选择的边
				{
					cout << "Unselected Edge:" << _vertexs[min_edge._srci] << "->" << _vertexs[min_edge._desi] << "(w:" << min_edge._w << ")" << endl;
				}
			}

			//如果最小生成树的边不是 顶点数-1 条
			if (min_tree_edge_size != _vertexs.size() - 1)
			{
				return W();
			}

			return min_tree_total_w;
		}

		/*
			最短路径Dijkstra算法--->时间复杂度:O(n^2),空间复杂度O(n),其中n为顶点个数
			思想是贪心
			dist_w存src到各个点的最短路径的权值
			p_path存从src到这个点的最短路径(eg:src->a->b->c->destination)中倒数第二个顶点的下标(如示例中的c的下标),
			p_path可以倒推从src到各个点的最短路径
		*/
		void dijkstra(const V& src, vector<W>& dist_w, vector<size_t>& p_path)
		{
			dist_w.resize(_vertexs.size(), MAX_W);
			p_path.resize(_vertexs.size(), -1);
			vector<bool> hash(_vertexs.size(), false);//最短路径已经确定的顶点标记为true,否则为false

			size_t srci = get_vertex_index(src);
			dist_w[srci] = W();
			p_path[srci] = srci;
			hash[srci] = true;

			size_t cur = srci;//当前要进行松弛更新的点


			while (true)
			{
				//松弛更新操作
				for (size_t j = 0; j < _matrix[cur].size(); ++j)
				{
					if (_matrix[cur][j] != MAX_W && //如果cur顶点能到j顶点(cur顶点和j顶点之间有边连接)
						hash[j] == false &&         //并且 src->j顶点的最短路径还没有确定
						(dist_w[j]==MAX_W || dist_w[cur]+_matrix[cur][j] < dist_w[j]))//并且 到j顶点的权值为MAX_W or src->cur + cur->j < src->j
					{
						dist_w[j] = dist_w[cur] + _matrix[cur][j];
						p_path[j] = cur;
					}
				}

				//寻找被hash标记为false 且 到达它的权值最小的顶点,用于迭代cur
				size_t next = -1;//下一个要进行松弛更新的点
				for (size_t i = 0; i < _vertexs.size(); ++i)
				{
					if (hash[i] == false &&                       //如果i顶点标记为false,
						dist_w[i] != MAX_W &&                     //并且目前判断src能到i顶点,
						(next == -1 || dist_w[i] < dist_w[next])) //并且next顶点不存在 or 到达i顶点的权值小于到达next顶点的权值
					{
						next = i;
					}
				}

				if (next != -1)//如果找到了,迭代cur,并在hash中将cur指向的顶点标记为true
				{
					cur = next;
					hash[cur] = true;
				}
				else //如果没找到,说明src到所有点(可到达的点)的最短路径已经更新完毕
				{
					break;
				}
			}
		}

		//bellman_ford算法
		//思想是暴力
		//可以求带负权图的最短路径问题,可以检测负权回路
		//时间复杂度O(n^3),空间复杂度O(n)
		bool bellman_ford(const V & src, vector<W>&dist_w, vector<size_t>&p_path)
		{
			dist_w.resize(_vertexs.size(), MAX_W);
			p_path.resize(_vertexs.size(), -1);

			//初始化dist_w[srci]和p_path[srci]
			size_t srci = get_vertex_index(src);
			dist_w[srci] = W();
			p_path[srci] = srci;

			//进行n轮更新(src到某一个点最多经过n-2个点)
			//第n+1轮(round==_vertexs.size())是用来检测是否存在负权回路
			for (size_t round = 0; round <= _vertexs.size(); ++round)
			{
				bool update = false;//检查是否进行了松弛更新

				//每一轮找到邻接矩阵中的所有边,对src到所有点的最短路径进行更新
				for (size_t i = 0; i < _matrix.size(); ++i)
				{
					for (size_t j = 0; j < _matrix[i].size(); ++j)
					{
						if (_matrix[i][j] != MAX_W &&                 //如果i可以到j,
							(dist_w[j] == MAX_W ||                    //并且 distw[j]等于标识值(代表不可到达)
						     dist_w[i] + _matrix[i][j] < dist_w[j]))  //     或者 src->i + i->j < src->j
						{   //进行松弛更新
							update = true;
							dist_w[j] = dist_w[i] + _matrix[i][j];
							p_path[j] = i;
						}
					}
				}

				//如果没更新,break
				if (update == false)
				{
					break;
				}

				//如果第n+1轮进行了松弛更新,说明存在负权回路,返回false
				if (round == _vertexs.size() && update == true)
				{
					return false;
				}
			}

			return true;
		}


		//floyd_warshall算法(多源最短路径)
		//求任意两个点之间的最短路径
		//原理是动态规划
		//dist_i_j存i到j点的最短路径的权值
		//p_path_i_j存i->j的最短路径(eg:i->a->b->c->j)上 j的上一个顶点的下标(例子中是c的下标)
		void floyd_warshall(vector<vector<W>>& dist_i_j,vector<vector<size_t>>& p_path_i_j,bool debug = false)
		{
			//初始化dist_i_j和p_path_i_j
			dist_i_j.resize(_vertexs.size());
			p_path_i_j.resize(_vertexs.size());
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				dist_i_j[i].resize(_vertexs.size(), MAX_W);
				dist_i_j[i][i] = W();

				p_path_i_j[i].resize(_vertexs.size(), -1);
				p_path_i_j[i][i] = i;
			}

			//将邻接矩阵中的所有边先添加到dist_i_j内
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				for (size_t j = 0; j < _matrix[i].size(); ++j)
				{
					if (_matrix[i][j] != MAX_W)
					{
						dist_i_j[i][j] = _matrix[i][j];
						p_path_i_j[i][j] = i;
					}
				}
			}

			//核心内容
			for (size_t k = 0; k < _vertexs.size(); ++k)
			{
				for (size_t i = 0; i < _vertexs.size(); ++i)
				{
					for (size_t j = 0; j < _vertexs.size(); ++j)
					{
						if (dist_i_j[i][k] != MAX_W &&                         //如果顶点i可以到顶点k
							dist_i_j[k][j] != MAX_W &&                         //并且顶点k可以到顶点j
							dist_i_j[i][k] + dist_i_j[k][j] < dist_i_j[i][j])  //并且i->k + k->j < i->j
						{
							dist_i_j[i][j] = dist_i_j[i][k] + dist_i_j[k][j];
							p_path_i_j[i][j] = p_path_i_j[k][j];
						}
					}
				}


				if (debug)
				{
					// 打印权值观察数据
					for (size_t i = 0; i < dist_i_j.size(); ++i)
					{
						for (size_t j = 0; j < dist_i_j[i].size(); ++j)
						{
							if (dist_i_j[i][j] == MAX_W)
							{
								cout << setw(5) << "*";
								//printf("%4c", '*');
							}
							else
							{
								cout << setw(5) << dist_i_j[i][j];
								//printf("%4d", dist_i_j[i][j]);
							}


							/*cout << " path:";

							vector<V> path_i_j;
							size_t parent = j;
							while (parent != i)
							{
								path_i_j.push_back(_vertexs[parent]);
								parent = p_path_i_j[i][parent];
							}
							path_i_j.push_back(_vertexs[i]);

							reverse(path_i_j.begin(), path_i_j.end());

							for (auto& vertex : path_i_j)
							{
								if (vertex != _vertexs[j])
								{
									cout << vertex << "->";
								}
								else
								{
									cout << vertex << endl;
								}
							}*/

						}
						cout << endl;
					}
					cout << "---------------------------------------------" << endl;
				}
			}
			
		}



		//打印src到各个点的最短路径
		void print_shortest_path(const V& src, vector<W>& dist_w, vector<size_t>& p_path)
		{
			size_t srci = get_vertex_index(src);

			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				if (i == srci)
				{
					continue;
				}
				if (p_path[i] == -1)
				{
					cout << _vertexs[i] << "不可达" << endl;
					continue;
				}

				vector<size_t> shortest_path;

				size_t parent = p_path[i];
				size_t cur = i;
				while (parent != cur)
				{
					shortest_path.push_back(cur);
					cur = parent;
					parent = p_path[cur];
				}
				shortest_path.push_back(srci);

				reverse(shortest_path.begin(), shortest_path.end());

				for (size_t path_i = 0; path_i < shortest_path.size(); ++path_i)
				{
					size_t pos = shortest_path[path_i];
					if (path_i != shortest_path.size() - 1)
					{
						cout << _vertexs[pos] << "->";
					}
					else
					{
						cout << _vertexs[pos] << " ";
					}
				}
				cout << "w=" << dist_w[i] << endl;
			}
		}





		//打印
		void print()
		{
			// 顶点
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				cout << "[" << i << "]" << "->" << _vertexs[i] << endl;
			}
			cout << endl;

			// 矩阵
			// 横下标
			cout << "  ";
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				cout << setw(5) << i;
			}
			cout << endl;

			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				cout << i << " "; // 竖下标
				for (size_t j = 0; j < _matrix[i].size(); ++j)
				{
					if (_matrix[i][j] == MAX_W)
					{
						cout << setw(5) << "*";
					}
					else
					{
						cout << setw(5) << _matrix[i][j];
					}
				}
				cout << endl;
			}
			cout << endl;
		}
	};
}








//以邻接表存储
namespace ad_table
{

	template<class W>
	struct Edge
	{
		size_t _desi; //目标点的下标
		W _w;         //权重
		Edge<W>* _next;  //下一条边

		Edge(size_t desi,const W& w):
			_desi(desi),
			_w(w),
			_next(nullptr)
		{}
	};


	//V是顶点的类型,
	//W是边(权重)的类型,
	//Direction为false表示无向图
	template<class V, class W, bool Direction = false>
	class Graph
	{
		typedef Edge<W> Edge;

		vector<V> _vertexs;               //顶点集合
		unordered_map<V, int> _index_map; //顶点映射下标
		vector<Edge*> _tables;            //邻接表(存的是n条单链表)(出边表)


	public:
		Graph(const V* a, size_t n)
		{
			_vertexs.reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				_vertexs.push_back(a[i]);
				_index_map[a[i]] = i;
			}

			_tables.resize(n, nullptr);
		}

		//获取顶点的下标
		size_t get_vertex_index(const V& vertex)
		{
			auto it = _index_map.find(vertex);
			if (it != _index_map.end())
			{
				return it->second;
			}
			else
			{
				throw invalid_argument("顶点不存在");
				return -1;
			}
		}

		//添加边
		void add_edge(const V& src, const V& des, const W& w)
		{
			size_t srci = get_vertex_index(src);
			size_t desi = get_vertex_index(des);

			//头插新增的边(src->des)
			Edge* new_edge_src_to_des = new Edge(desi, w);
			new_edge_src_to_des->_next = _tables[srci];
			_tables[srci] = new_edge_src_to_des;

			//如果是无向图
			if (Direction == false)
			{
				//头插新增的边(des->src)
				Edge* new_edge_des_to_src = new Edge(srci, w);
				new_edge_des_to_src->_next = _tables[desi];
				_tables[desi] = new_edge_des_to_src;
			}

		}



		//打印
		void print()
		{
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				cout << "[" << i << "] -> " << _vertexs[i] << endl;
			}

			for (size_t i = 0; i < _tables.size(); ++i)
			{
				cout << _vertexs[i] << "-> ";
				Edge* cur = _tables[i];
				while (cur)
				{
					cout << _vertexs[cur->_desi] << ":" << cur->_w << " ";
					cur = cur->_next;
				}
				cout << endl;
			}
		}



		~Graph()
		{
			for (Edge* cur:_tables)
			{
				while (cur)
				{
					Edge* next = cur->_next;
					delete cur;
					cur = next;
				}
			}
		}
	};
}