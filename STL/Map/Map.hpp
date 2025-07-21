#pragma once

#include "RBTree.hpp"


template<class K,class V>
class Map
{
	/// <summary>
	/// �º���,��ȡRBTree��Tģ�������Key��ֵ
	/// </summary>
	struct MapKeyOfT
	{
		const K& operator()(const pair<K, V>& kv)
		{
			return kv.first;
		}
	};



	/// <summary>
	/// �����
	/// </summary>
	RBTree<K, pair<const K, V>, MapKeyOfT> _RBTree;


public:
	//����ģ��ȡ��Ƕ����,��Ҫ��typename,���߱���������һ����Ƕ����
	//����typename,�������Ͳ�֪�����Iterator����Ƕ���ͻ��Ǿ�̬��Ա
	
	/// <summary>
	/// map ��ͨ������
	/// </summary>
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::Iterator iterator;

	/// <summary>
	/// map const������
	/// </summary>
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::ConstIterator const_iterator;



	/// <summary>
	/// ��Map�����в�������
	/// </summary>
	/// <param name="kv">�����������</param>
	/// <returns>
	/// ����ɹ�,����{�²������ݵĵ�����,true};
	/// ����ʧ��,����{�Ѿ����ڵļ��ĵ�����,false}
	/// </returns>
	pair<iterator,bool> insert(const pair<K,V>& kv) 
	{
		auto insertResult = _RBTree.insert(kv);//������pair�Ĺ��캯��,pair<K,V>���� ���Գ�ʼ�� pair<const K,V>����

		return { iterator(insertResult.first,_RBTree.getRoot()),insertResult.second };
	}

	/// <summary>
	/// ����[],
	/// ������,�򷵻ؼ���Ӧ��ֵ,
	/// ��������,�򴴽���Ӧ�ļ�,������V()
	/// </summary>
	/// <param name="key">��</param>
	/// <returns>
	/// ������,�򷵻ؼ���Ӧ��ֵ,
	/// ��������,�򴴽���Ӧ�ļ�,������V()
	/// </returns>
	V& operator[](const K& key)
	{
		return (insert({ key,V() }).first)->second;
	}

	

	/// <summary>
	/// ����Map����Ĵ�С
	/// </summary>
	/// <returns>����Map����Ĵ�С</returns>
	size_t size()
	{
		return _RBTree.size();
	}



	/// <summary>
	/// ����begin������
	/// </summary>
	/// <returns>����begin������</returns>
	iterator begin() 
	{
		return _RBTree.begin();
	}

	/// <summary>
	/// ����const begin������
	/// </summary>
	/// <returns>����const begin������</returns>
	const_iterator begin() const
	{
		return _RBTree.begin();
	}

	/// <summary>
	/// ����end������
	/// </summary>
	/// <returns>����end������</returns>
	iterator end()
	{
		return _RBTree.end();
	}

	/// <summary>
	/// ����const end������
	/// </summary>
	/// <returns>����const end������</returns>
	const_iterator end() const
	{
		return _RBTree.end();
	}



	/// <summary>
	/// �������
	/// </summary>
	void clear()
	{
		_RBTree.clear();
	}
};