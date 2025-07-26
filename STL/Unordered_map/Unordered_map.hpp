#pragma once

//#include <utility>
#include "HashTable.hpp"


template<class K,class V,class Hash=Hash<K>>
class Unordered_map
{
	/// <summary>
	/// 仿函数,用于求数据中键的值
	/// </summary>
	struct MapKeyOfT
	{
		const K& operator()(const pair<K,V>& kv)
		{
			return kv.first;
		}
	};

	/// <summary>
	/// 哈希表
	/// </summary>
	HashTable<K, pair<const K, V>, MapKeyOfT, Hash> _hashTable;


public:
	typedef typename HashTable<K, pair<const K, V>, MapKeyOfT, Hash>::Iterator iterator;
	typedef typename HashTable<K, pair<const K, V>, MapKeyOfT, Hash>::ConstIterator const_iterator;

	/// <summary>
	/// 无参默认构造
	/// </summary>
	Unordered_map() = default;

	/// <summary>
	/// 拷贝构造
	/// </summary>
	/// <param name="other"></param>
	Unordered_map(const Unordered_map<K,V>& other)
	{
		_hashTable = other._hashTable;
	}



	/// <summary>
	/// 插入键值对
	/// </summary>
	/// <param name="kv">待插入的键值对</param>
	/// <returns>
	/// 插入成功,返回新插入的键的迭代器和true;
	/// 插入失败,返回键(已经存在)的迭代器的false
	/// </returns>
	pair<iterator,bool> insert(const pair<K, V>& kv)
	{
		return _hashTable.insert(kv);
	}

	/// <summary>
	/// 重载[]
	/// </summary>
	/// <param name="key">键</param>
	/// <returns>返回 值的引用</returns>
	V& operator[](const K& key)
	{
		return (insert({ key, V() }).first)->second;
	}
	
	/// <summary>
	/// 查找键
	/// </summary>
	/// <param name="key">待查找的键</param>
	/// <returns>找到返回键的迭代器,否则返回end()</returns>
	iterator find(const K& key)
	{
		return _hashTable.find(key);
	}

	/// <summary>
	/// 计算键在容器内的个数
	/// </summary>
	/// <param name="key">待计算的键</param>
	/// <returns>键在容器内的个数;由于Unordered_map中键是唯一的,所以只会返回0 or 1</returns>
	size_t count(const K& key)
	{
		return _hashTable.find(key) != _hashTable.end();
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
	/// 返回普通begin迭代器
	/// </summary>
	/// <returns>返回普通begin迭代器</returns>
	iterator begin()
	{
		return _hashTable.begin();
	}

	/// <summary>
	/// 返回const begin迭代器
	/// </summary>
	/// <returns>返回const begin迭代器</returns>
	const_iterator begin() const
	{
		return _hashTable.begin();
	}

	/// <summary>
	/// 返回普通end迭代器
	/// </summary>
	/// <returns>返回普通end迭代器</returns>
	iterator end()
	{
		return _hashTable.end();
	}

	/// <summary>
	/// 返回const end迭代器
	/// </summary>
	/// <returns>返回const end迭代器</returns>
	const_iterator end() const
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