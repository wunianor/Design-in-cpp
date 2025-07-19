#pragma once


#include <vector>
using namespace std;

//并查集(树型结构)
//_ufs内的所有值初始默认为-1,
//    如果一个下标对应的值是负数,说明这个下标是一棵树的根,树的大小为这个负数的绝对值;
//    如果一个下标对应的值是非负数,说明这个下标的父亲是它对应的数(也是_ufs内的下标)
class UnionFindSet
{
	vector<int> _ufs;

public:
	//构造函数
	UnionFindSet(size_t n) :
		_ufs(n, -1)
	{
	}

	//寻找给定编号x的根
	int Find_root(int x)
	{
		int root = x;
		while (_ufs[root] >= 0)
		{
			root = _ufs[root];
		}

		//路径压缩(优化)
		while (_ufs[x] >= 0)//如果x不为根,进行路径压缩,压缩从x到根的所有结点(不包括根)的路径
		{
			int parent = _ufs[x];
			_ufs[x] = root;
			x = parent;
		}

		return root;
	}

	//合并两个集合(树)---时间复杂度为O(α(n)),其中α为阿克曼函数的反函数,其增长极其缓慢,可以近似认为是O(1)
	void Union(int x1, int x2)
	{
		int root1 = Find_root(x1);
		int root2 = Find_root(x2);

		//如果x1和x2在同一个集合,不需要合并,直接return
		if (root1 == root2)
		{
			return;
		}

		//如果root1的集合数据量小于root2集合的数据量
		//(按秩合并(启发式合并)优化:数据量小的往数据量大的合并)
		if (abs(_ufs[root1]) < abs(_ufs[root2]))
		{
			swap(root1, root2);
		}

		//数据量小的集合往数据量大的集合合并
		_ufs[root1] += _ufs[root2];
		_ufs[root2] = root1;
	}

	//判断x1和x2是否在同一个集合---时间复杂度为O(α(n)),其中α为阿克曼函数的反函数,其增长极其缓慢,可以近似认为是O(1)
	bool In_same_set(int x1, int x2)
	{
		return Find_root(x1) == Find_root(x2);
	}

	//获取并查集中有多少个集合(树)
	size_t Set_count()
	{
		size_t count = 0;
		for (auto& e : _ufs)
		{
			if (e < 0)
			{
				++count;
			}
		}
		return count;
	}

};