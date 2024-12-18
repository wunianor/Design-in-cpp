#pragma once
#include <vector>
using namespace std;

//���鼯
//_ufs�ڵ�����ֵ��ʼĬ��Ϊ-1,
//    ���һ���±��Ӧ��ֵ�Ǹ���,˵������±���һ�����ĸ�,���Ĵ�СΪ��������ľ���ֵ;
//    ���һ���±��Ӧ��ֵ�ǷǸ���,˵������±�ĸ���������Ӧ����(Ҳ��_ufs�ڵ��±�)
class UnionFindSet
{
	vector<int> _ufs;

public:
	UnionFindSet(size_t n):
		_ufs(n,-1)
	{}

	//Ѱ�Ҹ������x�ĸ�
	int Find_root(int x)
	{
		int root = x;
		while (_ufs[root] >= 0)
		{
			root = _ufs[root];
		}

		//·��ѹ��
		if (_ufs[x] >= 0)//���x��Ϊ��,����·��ѹ��
		{
			_ufs[x] = root;
		}

		return root;
	}

	//�ϲ���������(��)
	void Union(int x1, int x2)
	{
		int root1 = Find_root(x1);
		int root2 = Find_root(x2);
		if (root1 == root2)//���x1��x2��ͬһ������,����Ҫ�ϲ�,ֱ��return
		{
			return;
		}
		
		_ufs[root1] += _ufs[root2];
		_ufs[root2] = root1;
	}

	//�ж�x1��x2�Ƿ���ͬһ������
	bool In_same_set(int x1, int x2)
	{
		return Find_root(x1) == Find_root(x2);
	}

	//��ȡ���鼯���ж��ٸ�����(��)
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