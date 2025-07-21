#pragma once

#include "RBTree.hpp"

template<class K>
class Set
{
	/// <summary>
	/// �º���,��ȡRBTree��Tģ�������Key��ֵ
	/// </summary>
	struct SetKeyOfT
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};



	/// <summary>
	/// �����
	/// </summary>
	RBTree<K, K, SetKeyOfT> _RBTree;


public:

	//����ģ��ȡ��Ƕ����,��Ҫ��typename,���߱���������һ����Ƕ����
	//����typename,�������Ͳ�֪�����ConstIterator����Ƕ���ͻ��Ǿ�̬��Ա

	/// <summary>
	/// Set��ͨ������(���ʻ���const������)
	/// </summary>
	typedef typename RBTree<K, K, SetKeyOfT>::ConstIterator iterator;

	/// <summary>
	/// Set const������
	/// </summary>
	typedef typename RBTree<K, K, SetKeyOfT>::ConstIterator const_iterator;



	/// <summary>
	/// ��Set�����в�������
	/// </summary>
	/// <param name="key">������ļ�</param>
	/// <returns>
	/// ����ɹ�,����{�²������ݵĵ�����,true};
	/// ����ʧ��,����{�Ѿ����ڵļ��ĵ�����,false}
	/// </returns>
	pair<iterator,bool> insert(const K& key) 
	{
		auto insertResult = _RBTree.insert(key);
		return { iterator(insertResult.first,_RBTree.getRoot()),insertResult.second };
	}



	/// <summary>
	/// ����Set����Ĵ�С
	/// </summary>
	/// <returns>����Set����Ĵ�С</returns>
	size_t size()
	{
		return _RBTree.size();
	}



	/// <summary>
	/// ����begin������
	/// </summary>
	/// <returns>����const begin������</returns>
	iterator begin() const 
	{
		return _RBTree.begin();
	}

	/// <summary>
	/// ����end������
	/// </summary>
	/// <returns>����const end������</returns>
	iterator end() const
	{
		return _RBTree.end();
	}
};