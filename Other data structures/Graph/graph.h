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

//���ڽӾ���洢
namespace matrix
{
	//V�Ƕ��������,
	//W�Ǳ�(Ȩ��)������,
	//DirectionΪfalse��ʾ����ͼ,
	//��һ���㲻�ܵ���һ����ʱʹ��MAX_W��Ϊ��ʶֵ
	template<class V, class W,bool Direction=false, W MAX_W = INT_MAX>
	class Graph
	{
		typedef Graph<V, W, Direction, MAX_W> Self;


		vector<V> _vertexs;               //���㼯��
		unordered_map<V, int> _index_map; //����ӳ���±�
		vector<vector<W>> _matrix;        //�ڽӾ���


		//dfs�Ӻ���
		void _dfs(size_t srci, vector<bool>& visited,size_t depth)
		{
			//���ʵ�ǰ��㲢���
			cout << _vertexs[srci] << "(" << depth << ") ";
			visited[srci] = true;

			//Ѱ�� �뵱ǰ������ڲ���δ���ʵĵ� ����
			for (size_t j = 0; j < _matrix[srci].size(); ++j)
			{
				if (_matrix[srci][j] != MAX_W && visited[j] == false)
				{
					_dfs(j, visited, depth + 1);
				}
			}
		}

	public:
		//�ڲ����
		struct Edge
		{
			size_t _srci;//�ߵ������±�
			size_t _desi;//�ߵ��յ���±�
			W _w;//Ȩֵ

			Edge(size_t srci,size_t desi,const W& w):
				_srci(srci),
				_desi(desi),
				_w(w)
			{}

			//��������
			bool operator>(const Edge& b) const 
			{
				return _w > b._w;
			}
		};


		//ͼ�Ĵ���:
		//1.IO����,�ʺ�OJ��,���ʺϲ���
		//2.ͼ�ṹ��ϵд���ļ�,��ȡ�ļ�
		//3.�ֶ���ӱ�
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

		//��ȡ������±�
		size_t get_vertex_index(const V& vertex)
		{
			auto it = _index_map.find(vertex);
			if (it != _index_map.end())
			{
				return it->second;
			}
			else
			{
				throw invalid_argument("���㲻����");//���쳣
				return -1;
			}
		}

		//ͨ���±���ӱ�
		void add_edge_by_index(size_t srci, size_t desi, const W& w)
		{
			_matrix[srci][desi] = w;

			if (Direction == false)//����ͼ
			{
				_matrix[desi][srci] = w;
			}
		}

		//��ӱ�
		void add_edge(const V& src, const V& des, const W& w)
		{
			size_t srci = get_vertex_index(src);
			size_t desi = get_vertex_index(des);

			add_edge_by_index(srci, desi, w);
		}



		//������ȱ���
		void dfs(const V& src)
		{
			size_t srci = get_vertex_index(src);
			vector<bool> visited(_vertexs.size(),false);
			size_t depth = 0;

			cout << "dfs:\n ";
			
			_dfs(srci, visited, depth);

		}



		//������ȱ���
		void bfs(const V& src)
		{
			size_t srci = get_vertex_index(src);//��ȡ������±�
			queue<size_t> q;
			unordered_set<size_t> visited;//�洢������±�,��ʾ��Щ�����Ѿ�����

			q.push(srci);
			visited.insert(srci);

			size_t level_count = 0;//��¼��ǰ�������ڼ���
			size_t level_size = q.size();//��¼��ǰ���ж��ٸ�����

			cout << "bfs:" << endl;

			while (!q.empty())
			{
				level_size = q.size();
				cout << " ��" << level_count << "��:";
				while (level_size--)
				{
					size_t front = q.front();
					q.pop();
					cout << _vertexs[front] << " ";
					for (size_t j = 0; j < _matrix[front].size(); ++j)
					{
						//���_vertexs[front]������_vertexs[j]�б����� �� _vertexs[j]û�б�����
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



		//��С������Kruskal�㷨(̰��+����(��)+���鼯)
		W kruskal(Self& min_tree,bool debug = false)
		{
			priority_queue<Edge, vector<Edge>, greater<Edge>> minpq;//С��,���ձߵ�Ȩֵ������

			//�ҵ����еı�
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				for (size_t j = (Direction == false ? i+1 : 0) ; j < _matrix[i].size(); ++j)
				{
					if (i == j) continue;//��������ߵ�����������ͬһ������

					if (_matrix[i][j] != MAX_W)
					{
						minpq.push(Edge(i,j,_matrix[i][j]));
					}
				}
			}

			size_t min_tree_edge_size = 0; //��С�������ıߵ�����
			W min_tree_total_w = W();      //��С����������Ȩֵ
			UnionFindSet ufs(_vertexs.size()); //���鼯,��¼��Щ����һ�����ϵ�

			//����Ȩֵ��С����ʼѡ��
			while (!minpq.empty())
			{
				Edge edge = minpq.top();
				minpq.pop();

				//��������ߵ��������㲻��һ��������(������ɻ�)
				if (ufs.In_same_set(edge._srci, edge._desi) == false)
				{
					if (debug)//debugģʽ�´�ӡѡ��ı�
					{
						cout << "Selected Edge:" << _vertexs[edge._srci] << "->" << _vertexs[edge._desi] << "(w:" << edge._w << ")" << endl;
					}

					min_tree.add_edge_by_index(edge._srci, edge._desi, edge._w);
					ufs.Union(edge._srci, edge._desi);//�ϲ�����
					min_tree_total_w += edge._w;
					++min_tree_edge_size;
				}
				else if (debug)//debugģʽ�´�ӡδѡ��ı�
				{
					cout << "Unselected Edge:" << _vertexs[edge._srci] << "->" << _vertexs[edge._desi] << "(w:" << edge._w << ")" << endl;
				}
			}

			//û�ҵ���С������
			if (min_tree_edge_size != _vertexs.size() - 1)
			{
				return W();
			}

			//�ҵ�����С������
			return min_tree_total_w;
		}



		//��С������prim�㷨
		W prim(Self& min_tree,const V& src,bool debug = false)
		{
			size_t srci = get_vertex_index(src);

			vector<bool> X(_vertexs.size(),false); //�Ѿ�������С�������Ķ��� ��X�б��Ϊtrue,����Ϊfalse
			X[srci] = true;

			
			W min_tree_total_w = W(); //��С����������Ȩ
			size_t min_tree_edge_size = 0;//��ǰ��С�������ߵ�����
			priority_queue<Edge, vector<Edge>, greater<Edge>> minpq;//С��,���

			//�����Ϊsrc,�յ�ΪX�б���ǳ�false�Ķ��� �ı�push��С��
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


				//�ж��Ƿ��ɻ� 
				//(ȡ���ıߵ�����Ƿ��Ѽ�����С������(��ǳ�true),�յ��Ƿ�û�м�����С������(���Ϊfalse))
				if (X[min_edge._srci] == true && X[min_edge._desi] == false)
				{
					min_tree.add_edge_by_index(min_edge._srci, min_edge._desi, min_edge._w);
					min_tree_total_w += min_edge._w;
					++min_tree_edge_size;

					if (debug)//debugģʽ�´�ӡѡ��ı�
					{
						cout << "Selected Edge:" << _vertexs[min_edge._srci] << "->" << _vertexs[min_edge._desi] << "(w:" << min_edge._w << ")" << endl;
					}

					X[min_edge._desi] = true;//��min_edge._desi��ǳ�true(�Ѽ�����С������)
						
					//�����Ϊmin_edge._desi,�յ�Ϊ��X����ʶ��false�Ķ��� �ı�push��С��
					for (size_t j = 0; j < _matrix[min_edge._desi].size(); ++j)
					{
						if (_matrix[min_edge._desi][j] != MAX_W && X[j] == false)
						{
							minpq.push(Edge(min_edge._desi, j, _matrix[min_edge._desi][j]));
						}
					}
				}
				else if (debug)//debugģʽ�´�ӡδѡ��ı�
				{
					cout << "Unselected Edge:" << _vertexs[min_edge._srci] << "->" << _vertexs[min_edge._desi] << "(w:" << min_edge._w << ")" << endl;
				}
			}

			//�����С�������ı߲��� ������-1 ��
			if (min_tree_edge_size != _vertexs.size() - 1)
			{
				return W();
			}

			return min_tree_total_w;
		}

		/*
			���·��Dijkstra�㷨--->ʱ�临�Ӷ�:O(n^2),�ռ临�Ӷ�O(n),����nΪ�������
			˼����̰��
			dist_w��src������������·����Ȩֵ
			p_path���src�����������·��(eg:src->a->b->c->destination)�е����ڶ���������±�(��ʾ���е�c���±�),
			p_path���Ե��ƴ�src������������·��
		*/
		void dijkstra(const V& src, vector<W>& dist_w, vector<size_t>& p_path)
		{
			dist_w.resize(_vertexs.size(), MAX_W);
			p_path.resize(_vertexs.size(), -1);
			vector<bool> hash(_vertexs.size(), false);//���·���Ѿ�ȷ���Ķ�����Ϊtrue,����Ϊfalse

			size_t srci = get_vertex_index(src);
			dist_w[srci] = W();
			p_path[srci] = srci;
			hash[srci] = true;

			size_t cur = srci;//��ǰҪ�����ɳڸ��µĵ�


			while (true)
			{
				//�ɳڸ��²���
				for (size_t j = 0; j < _matrix[cur].size(); ++j)
				{
					if (_matrix[cur][j] != MAX_W && //���cur�����ܵ�j����(cur�����j����֮���б�����)
						hash[j] == false &&         //���� src->j��������·����û��ȷ��
						(dist_w[j]==MAX_W || dist_w[cur]+_matrix[cur][j] < dist_w[j]))//���� ��j�����ȨֵΪMAX_W or src->cur + cur->j < src->j
					{
						dist_w[j] = dist_w[cur] + _matrix[cur][j];
						p_path[j] = cur;
					}
				}

				//Ѱ�ұ�hash���Ϊfalse �� ��������Ȩֵ��С�Ķ���,���ڵ���cur
				size_t next = -1;//��һ��Ҫ�����ɳڸ��µĵ�
				for (size_t i = 0; i < _vertexs.size(); ++i)
				{
					if (hash[i] == false &&                       //���i������Ϊfalse,
						dist_w[i] != MAX_W &&                     //����Ŀǰ�ж�src�ܵ�i����,
						(next == -1 || dist_w[i] < dist_w[next])) //����next���㲻���� or ����i�����ȨֵС�ڵ���next�����Ȩֵ
					{
						next = i;
					}
				}

				if (next != -1)//����ҵ���,����cur,����hash�н�curָ��Ķ�����Ϊtrue
				{
					cur = next;
					hash[cur] = true;
				}
				else //���û�ҵ�,˵��src�����е�(�ɵ���ĵ�)�����·���Ѿ��������
				{
					break;
				}
			}
		}

		//bellman_ford�㷨
		//˼���Ǳ���
		//���������Ȩͼ�����·������,���Լ�⸺Ȩ��·
		//ʱ�临�Ӷ�O(n^3),�ռ临�Ӷ�O(n)
		bool bellman_ford(const V & src, vector<W>&dist_w, vector<size_t>&p_path)
		{
			dist_w.resize(_vertexs.size(), MAX_W);
			p_path.resize(_vertexs.size(), -1);

			//��ʼ��dist_w[srci]��p_path[srci]
			size_t srci = get_vertex_index(src);
			dist_w[srci] = W();
			p_path[srci] = srci;

			//����n�ָ���(src��ĳһ������ྭ��n-2����)
			//��n+1��(round==_vertexs.size())����������Ƿ���ڸ�Ȩ��·
			for (size_t round = 0; round <= _vertexs.size(); ++round)
			{
				bool update = false;//����Ƿ�������ɳڸ���

				//ÿһ���ҵ��ڽӾ����е����б�,��src�����е�����·�����и���
				for (size_t i = 0; i < _matrix.size(); ++i)
				{
					for (size_t j = 0; j < _matrix[i].size(); ++j)
					{
						if (_matrix[i][j] != MAX_W &&                 //���i���Ե�j,
							(dist_w[j] == MAX_W ||                    //���� distw[j]���ڱ�ʶֵ(�����ɵ���)
						     dist_w[i] + _matrix[i][j] < dist_w[j]))  //     ���� src->i + i->j < src->j
						{   //�����ɳڸ���
							update = true;
							dist_w[j] = dist_w[i] + _matrix[i][j];
							p_path[j] = i;
						}
					}
				}

				//���û����,break
				if (update == false)
				{
					break;
				}

				//�����n+1�ֽ������ɳڸ���,˵�����ڸ�Ȩ��·,����false
				if (round == _vertexs.size() && update == true)
				{
					return false;
				}
			}

			return true;
		}


		//floyd_warshall�㷨(��Դ���·��)
		//������������֮������·��
		//ԭ���Ƕ�̬�滮
		//dist_i_j��i��j������·����Ȩֵ
		//p_path_i_j��i->j�����·��(eg:i->a->b->c->j)�� j����һ��������±�(��������c���±�)
		void floyd_warshall(vector<vector<W>>& dist_i_j,vector<vector<size_t>>& p_path_i_j,bool debug = false)
		{
			//��ʼ��dist_i_j��p_path_i_j
			dist_i_j.resize(_vertexs.size());
			p_path_i_j.resize(_vertexs.size());
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				dist_i_j[i].resize(_vertexs.size(), MAX_W);
				dist_i_j[i][i] = W();

				p_path_i_j[i].resize(_vertexs.size(), -1);
				p_path_i_j[i][i] = i;
			}

			//���ڽӾ����е����б�����ӵ�dist_i_j��
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

			//��������
			for (size_t k = 0; k < _vertexs.size(); ++k)
			{
				for (size_t i = 0; i < _vertexs.size(); ++i)
				{
					for (size_t j = 0; j < _vertexs.size(); ++j)
					{
						if (dist_i_j[i][k] != MAX_W &&                         //�������i���Ե�����k
							dist_i_j[k][j] != MAX_W &&                         //���Ҷ���k���Ե�����j
							dist_i_j[i][k] + dist_i_j[k][j] < dist_i_j[i][j])  //����i->k + k->j < i->j
						{
							dist_i_j[i][j] = dist_i_j[i][k] + dist_i_j[k][j];
							p_path_i_j[i][j] = p_path_i_j[k][j];
						}
					}
				}


				if (debug)
				{
					// ��ӡȨֵ�۲�����
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



		//��ӡsrc������������·��
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
					cout << _vertexs[i] << "���ɴ�" << endl;
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





		//��ӡ
		void print()
		{
			// ����
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				cout << "[" << i << "]" << "->" << _vertexs[i] << endl;
			}
			cout << endl;

			// ����
			// ���±�
			cout << "  ";
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				cout << setw(5) << i;
			}
			cout << endl;

			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				cout << i << " "; // ���±�
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








//���ڽӱ�洢
namespace ad_table
{

	template<class W>
	struct Edge
	{
		size_t _desi; //Ŀ�����±�
		W _w;         //Ȩ��
		Edge<W>* _next;  //��һ����

		Edge(size_t desi,const W& w):
			_desi(desi),
			_w(w),
			_next(nullptr)
		{}
	};


	//V�Ƕ��������,
	//W�Ǳ�(Ȩ��)������,
	//DirectionΪfalse��ʾ����ͼ
	template<class V, class W, bool Direction = false>
	class Graph
	{
		typedef Edge<W> Edge;

		vector<V> _vertexs;               //���㼯��
		unordered_map<V, int> _index_map; //����ӳ���±�
		vector<Edge*> _tables;            //�ڽӱ�(�����n��������)(���߱�)


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

		//��ȡ������±�
		size_t get_vertex_index(const V& vertex)
		{
			auto it = _index_map.find(vertex);
			if (it != _index_map.end())
			{
				return it->second;
			}
			else
			{
				throw invalid_argument("���㲻����");
				return -1;
			}
		}

		//��ӱ�
		void add_edge(const V& src, const V& des, const W& w)
		{
			size_t srci = get_vertex_index(src);
			size_t desi = get_vertex_index(des);

			//ͷ�������ı�(src->des)
			Edge* new_edge_src_to_des = new Edge(desi, w);
			new_edge_src_to_des->_next = _tables[srci];
			_tables[srci] = new_edge_src_to_des;

			//���������ͼ
			if (Direction == false)
			{
				//ͷ�������ı�(des->src)
				Edge* new_edge_des_to_src = new Edge(srci, w);
				new_edge_des_to_src->_next = _tables[desi];
				_tables[desi] = new_edge_des_to_src;
			}

		}



		//��ӡ
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