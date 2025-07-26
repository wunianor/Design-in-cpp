#pragma once

//#include <utility>
#include "HashTable.hpp"


template<class K,class V,class Hash=Hash<K>>
class Unordered_map
{
	/// <summary>
	/// �º���,�����������м���ֵ
	/// </summary>
	struct MapKeyOfT
	{
		const K& operator()(const pair<K,V>& kv)
		{
			return kv.first;
		}
	};

	/// <summary>
	/// ��ϣ��
	/// </summary>
	HashTable<K, pair<const K, V>, MapKeyOfT, Hash> _hashTable;


public:
	typedef typename HashTable<K, pair<const K, V>, MapKeyOfT, Hash>::Iterator iterator;
	typedef typename HashTable<K, pair<const K, V>, MapKeyOfT, Hash>::ConstIterator const_iterator;

	/// <summary>
	/// �޲�Ĭ�Ϲ���
	/// </summary>
	Unordered_map() = default;

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="other"></param>
	Unordered_map(const Unordered_map<K,V>& other)
	{
		_hashTable = other._hashTable;
	}



	/// <summary>
	/// �����ֵ��
	/// </summary>
	/// <param name="kv">������ļ�ֵ��</param>
	/// <returns>
	/// ����ɹ�,�����²���ļ��ĵ�������true;
	/// ����ʧ��,���ؼ�(�Ѿ�����)�ĵ�������false
	/// </returns>
	pair<iterator,bool> insert(const pair<K, V>& kv)
	{
		return _hashTable.insert(kv);
	}

	/// <summary>
	/// ����[]
	/// </summary>
	/// <param name="key">��</param>
	/// <returns>���� ֵ������</returns>
	V& operator[](const K& key)
	{
		return (insert({ key, V() }).first)->second;
	}
	
	/// <summary>
	/// ���Ҽ�
	/// </summary>
	/// <param name="key">�����ҵļ�</param>
	/// <returns>�ҵ����ؼ��ĵ�����,���򷵻�end()</returns>
	iterator find(const K& key)
	{
		return _hashTable.find(key);
	}

	/// <summary>
	/// ������������ڵĸ���
	/// </summary>
	/// <param name="key">������ļ�</param>
	/// <returns>���������ڵĸ���;����Unordered_map�м���Ψһ��,����ֻ�᷵��0 or 1</returns>
	size_t count(const K& key)
	{
		return _hashTable.find(key) != _hashTable.end();
	}

	/// <summary>
	/// ɾ����
	/// </summary>
	/// <param name="key">��ɾ���ļ�</param>
	/// <returns>�������ڷ���false,���򷵻�true</returns>
	bool erase(const K& key)
	{
		return _hashTable.erase(key);
	}



	/// <summary>
	/// ������ͨbegin������
	/// </summary>
	/// <returns>������ͨbegin������</returns>
	iterator begin()
	{
		return _hashTable.begin();
	}

	/// <summary>
	/// ����const begin������
	/// </summary>
	/// <returns>����const begin������</returns>
	const_iterator begin() const
	{
		return _hashTable.begin();
	}

	/// <summary>
	/// ������ͨend������
	/// </summary>
	/// <returns>������ͨend������</returns>
	iterator end()
	{
		return _hashTable.end();
	}

	/// <summary>
	/// ����const end������
	/// </summary>
	/// <returns>����const end������</returns>
	const_iterator end() const
	{
		return _hashTable.end();
	}



	/// <summary>
	/// ���ض����Ԫ�ظ���
	/// </summary>
	/// <returns>���ض����Ԫ�ظ���</returns>
	size_t size() const
	{
		return _hashTable.size();
	}
};