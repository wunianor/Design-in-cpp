#pragma once

#include "HashTable.hpp"


template<class K,class Hash = Hash<K>>
class Unordered_set
{
	/// <summary>
	/// �º���,���������������ֵ
	/// </summary>
	struct SetKeyOfT
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};

	/// <summary>
	/// ��ϣ��
	/// </summary>
	HashTable<K, K, SetKeyOfT, Hash> _hashTable;

public:

	/// <summary>
	/// ��ͨ������(���ʻ���const������)
	/// </summary>
	typedef typename HashTable<K, K, SetKeyOfT, Hash>::ConstIterator iterator;

	/// <summary>
	/// const������
	/// </summary>
	typedef typename HashTable<K, K, SetKeyOfT, Hash>::ConstIterator const_iterator;



	/// <summary>
	/// �޲�Ĭ�Ϲ��캯��
	/// </summary>
	Unordered_set() = default;

	/// <summary>
	/// �������캯��
	/// </summary>
	/// <param name="other"></param>
	Unordered_set(const Unordered_set<K>& other)
	{
		_hashTable = other._hashTable;
	}



	/// <summary>
	/// �����
	/// </summary>
	/// <param name="key">������ļ�</param>
	/// <returns>
	/// ����ɹ�,�����²���ļ��ĵ�������true;
	/// ����ʧ��,���ؼ�(�Ѿ�����)�ĵ�������false
	/// </returns>
	pair<iterator,bool> insert(const K& key)
	{
		auto insertResult = _hashTable.insert(key);
		return { iterator(insertResult.first._node,insertResult.first._buckets_ptr),insertResult.second };
	}

	/// <summary>
	/// ���Ҽ�
	/// </summary>
	/// <param name="key">�����ҵļ�</param>
	/// <returns>�ҵ����ؼ��ĵ�����,���򷵻�end()</returns>
	iterator find(const K& key)
	{
		auto ret = _hashTable.find(key);
		return iterator(ret._node, ret._buckets_ptr);
	}

	/// <summary>
	/// ������������ڳ��ֵĴ���
	/// </summary>
	/// <param name="key">������ļ�</param>
	/// <returns>���ؼ��������ڳ��ֵĴ���;����Unordered_set�ļ���Ψһ��,����ֻ�᷵��0 or 1</returns>
	size_t count(const K& key)
	{
		return find(key) != end();
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
	/// ������ͨbegin������(������const_iterator)
	/// </summary>
	/// <returns>������ͨbegin������(������const_iterator)</returns>
	iterator begin() const
	{
		return _hashTable.begin();
	}
	
	/// <summary>
	/// ������ͨend������(������const_iterator)
	/// </summary>
	/// <returns>������ͨend������(������const_iterator)</returns>
	iterator end() const
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