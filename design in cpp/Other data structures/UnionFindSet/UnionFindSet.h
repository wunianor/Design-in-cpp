#pragma once
#include <vector>
using namespace std;

//并查集
//_ufs内的所有值初始默认为-1,
//    如果一个下标对应的值是负数,说明这个下标是一棵树的根,树的大小为这个负数的绝对值;
//    如果一个下标对应的值是非负数,说明这个下标的父亲是它对应的数(也是_ufs内的下标)
class UnionFindSet
{
	vector<int> _ufs;

public:
	UnionFindSet(size_t n):
		_ufs(n,-1)
	{}

	//寻找给定编号x的根
	int Find_root(int x)
	{
		int root = x;
		while (_ufs[root] >= 0)
		{
			root = _ufs[root];
		}

		//路径压缩
		if (_ufs[x] >= 0)//如果x不为根,进行路径压缩
		{
			_ufs[x] = root;
		}

		return root;
	}

	//合并两个集合(树)
	void Union(int x1, int x2)
	{
		int root1 = Find_root(x1);
		int root2 = Find_root(x2);
		if (root1 == root2)//如果x1和x2在同一个集合,不需要合并,直接return
		{
			return;
		}
		
		_ufs[root1] += _ufs[root2];
		_ufs[root2] = root1;
	}

	//判断x1和x2是否在同一个集合
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