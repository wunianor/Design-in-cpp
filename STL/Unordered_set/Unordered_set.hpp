#pragma once

#include "HashTable.hpp"


template<class K,class Hash = Hash<K>>
class Unordered_set
{
	/// <summary>
	/// 仿函数,用于数据中求键的值
	/// </summary>
	struct SetKeyOfT
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};

	/// <summary>
	/// 哈希表
	/// </summary>
	HashTable<K, K, SetKeyOfT, Hash> _hashTable;

public:

	/// <summary>
	/// 普通迭代器(本质还是const迭代器)
	/// </summary>
	typedef typename HashTable<K, K, SetKeyOfT, Hash>::ConstIterator iterator;

	/// <summary>
	/// const迭代器
	/// </summary>
	typedef typename HashTable<K, K, SetKeyOfT, Hash>::ConstIterator const_iterator;



	/// <summary>
	/// 无参默认构造函数
	/// </summary>
	Unordered_set() = default;

	/// <summary>
	/// 拷贝构造函数
	/// </summary>
	/// <param name="other"></param>
	Unordered_set(const Unordered_set<K>& other)
	{
		_hashTable = other._hashTable;
	}



	/// <summary>
	/// 插入键
	/// </summary>
	/// <param name="key">待插入的键</param>
	/// <returns>
	/// 插入成功,返回新插入的键的迭代器和true;
	/// 插入失败,返回键(已经存在)的迭代器的false
	/// </returns>
	pair<iterator,bool> insert(const K& key)
	{
		auto insertResult = _hashTable.insert(key);
		return { iterator(insertResult.first._node,insertResult.first._buckets_ptr),insertResult.second };
	}

	/// <summary>
	/// 查找键
	/// </summary>
	/// <param name="key">待查找的键</param>
	/// <returns>找到返回键的迭代器,否则返回end()</returns>
	iterator find(const K& key)
	{
		auto ret = _hashTable.find(key);
		return iterator(ret._node, ret._buckets_ptr);
	}

	/// <summary>
	/// 计算键在容器内出现的次数
	/// </summary>
	/// <param name="key">待计算的键</param>
	/// <returns>返回键在容器内出现的次数;由于Unordered_set的键是唯一的,所以只会返回0 or 1</returns>
	size_t count(const K& key)
	{
		return find(key) != end();
	}

	/// <summary>
	/// 删除键
	/// </summary>
	/// <param name="key">待删除的键</param>
	/// <returns>键不存在返回false,否则返回true</returns>
	bool erase(const K& key)
	{
		return _hashTable.erase(key);
	}



	/// <summary>
	/// 返回普通begin迭代器(本质是const_iterator)
	/// </summary>
	/// <returns>返回普通begin迭代器(本质是const_iterator)</returns>
	iterator begin() const
	{
		return _hashTable.begin();
	}
	
	/// <summary>
	/// 返回普通end迭代器(本质是const_iterator)
	/// </summary>
	/// <returns>返回普通end迭代器(本质是const_iterator)</returns>
	iterator end() const
	{
		return _hashTable.end();
	}



	/// <summary>
	/// 返回对象的元素个数
	/// </summary>
	/// <returns>返回对象的元素个数</returns>
	size_t size() const
	{
		return _hashTable.size();
	}
};