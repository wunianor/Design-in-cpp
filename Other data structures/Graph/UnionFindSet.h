#pragma once


#include <vector>
using namespace std;

//���鼯(���ͽṹ)
//_ufs�ڵ�����ֵ��ʼĬ��Ϊ-1,
//    ���һ���±��Ӧ��ֵ�Ǹ���,˵������±���һ�����ĸ�,���Ĵ�СΪ��������ľ���ֵ;
//    ���һ���±��Ӧ��ֵ�ǷǸ���,˵������±�ĸ���������Ӧ����(Ҳ��_ufs�ڵ��±�)
class UnionFindSet
{
	vector<int> _ufs;

public:
	//���캯��
	UnionFindSet(size_t n) :
		_ufs(n, -1)
	{
	}

	//Ѱ�Ҹ������x�ĸ�
	int Find_root(int x)
	{
		int root = x;
		while (_ufs[root] >= 0)
		{
			root = _ufs[root];
		}

		//·��ѹ��(�Ż�)
		while (_ufs[x] >= 0)//���x��Ϊ��,����·��ѹ��,ѹ����x���������н��(��������)��·��
		{
			int parent = _ufs[x];
			_ufs[x] = root;
			x = parent;
		}

		return root;
	}

	//�ϲ���������(��)---ʱ�临�Ӷ�ΪO(��(n)),���Ц�Ϊ�����������ķ�����,���������仺��,���Խ�����Ϊ��O(1)
	void Union(int x1, int x2)
	{
		int root1 = Find_root(x1);
		int root2 = Find_root(x2);

		//���x1��x2��ͬһ������,����Ҫ�ϲ�,ֱ��return
		if (root1 == root2)
		{
			return;
		}

		//���root1�ļ���������С��root2���ϵ�������
		//(���Ⱥϲ�(����ʽ�ϲ�)�Ż�:������С������������ĺϲ�)
		if (abs(_ufs[root1]) < abs(_ufs[root2]))
		{
			swap(root1, root2);
		}

		//������С�ļ�������������ļ��Ϻϲ�
		_ufs[root1] += _ufs[root2];
		_ufs[root2] = root1;
	}

	//�ж�x1��x2�Ƿ���ͬһ������---ʱ�临�Ӷ�ΪO(��(n)),���Ц�Ϊ�����������ķ�����,���������仺��,���Խ�����Ϊ��O(1)
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