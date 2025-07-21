#pragma once

#include "RBTree.hpp"


template<class K,class V>
class Map
{
	/// <summary>
	/// 仿函数,获取RBTree中T模板参数的Key的值
	/// </summary>
	struct MapKeyOfT
	{
		const K& operator()(const pair<K, V>& kv)
		{
			return kv.first;
		}
	};



	/// <summary>
	/// 红黑树
	/// </summary>
	RBTree<K, pair<const K, V>, MapKeyOfT> _RBTree;


public:
	//对类模板取内嵌类型,需要加typename,告诉编译器这是一个内嵌类型
	//不加typename,编译器就不知道这个Iterator是内嵌类型还是静态成员
	
	/// <summary>
	/// map 普通迭代器
	/// </summary>
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::Iterator iterator;

	/// <summary>
	/// map const迭代器
	/// </summary>
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::ConstIterator const_iterator;



	/// <summary>
	/// 向Map对象中插入数据
	/// </summary>
	/// <param name="kv">待插入的数据</param>
	/// <returns>
	/// 插入成功,返回{新插入数据的迭代器,true};
	/// 插入失败,返回{已经存在的键的迭代器,false}
	/// </returns>
	pair<iterator,bool> insert(const pair<K,V>& kv) 
	{
		auto insertResult = _RBTree.insert(kv);//得益于pair的构造函数,pair<K,V>类型 可以初始化 pair<const K,V>类型

		return { iterator(insertResult.first,_RBTree.getRoot()),insertResult.second };
	}

	/// <summary>
	/// 重载[],
	/// 键存在,则返回键对应的值,
	/// 键不存在,则创建对应的键,并返回V()
	/// </summary>
	/// <param name="key">键</param>
	/// <returns>
	/// 键存在,则返回键对应的值,
	/// 键不存在,则创建对应的键,并返回V()
	/// </returns>
	V& operator[](const K& key)
	{
		return (insert({ key,V() }).first)->second;
	}

	

	/// <summary>
	/// 返回Map对象的大小
	/// </summary>
	/// <returns>返回Map对象的大小</returns>
	size_t size()
	{
		return _RBTree.size();
	}



	/// <summary>
	/// 返回begin迭代器
	/// </summary>
	/// <returns>返回begin迭代器</returns>
	iterator begin() 
	{
		return _RBTree.begin();
	}

	/// <summary>
	/// 返回const begin迭代器
	/// </summary>
	/// <returns>返回const begin迭代器</returns>
	const_iterator begin() const
	{
		return _RBTree.begin();
	}

	/// <summary>
	/// 返回end迭代器
	/// </summary>
	/// <returns>返回end迭代器</returns>
	iterator end()
	{
		return _RBTree.end();
	}

	/// <summary>
	/// 返回const end迭代器
	/// </summary>
	/// <returns>返回const end迭代器</returns>
	const_iterator end() const
	{
		return _RBTree.end();
	}



	/// <summary>
	/// 清空容器
	/// </summary>
	void clear()
	{
		_RBTree.clear();
	}
};