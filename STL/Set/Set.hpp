#pragma once

#include "RBTree.hpp"

template<class K>
class Set
{
	/// <summary>
	/// 仿函数,获取RBTree中T模板参数的Key的值
	/// </summary>
	struct SetKeyOfT
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};



	/// <summary>
	/// 红黑树
	/// </summary>
	RBTree<K, K, SetKeyOfT> _RBTree;


public:

	//对类模板取内嵌类型,需要加typename,告诉编译器这是一个内嵌类型
	//不加typename,编译器就不知道这个ConstIterator是内嵌类型还是静态成员

	/// <summary>
	/// Set普通迭代器(本质还是const迭代器)
	/// </summary>
	typedef typename RBTree<K, K, SetKeyOfT>::ConstIterator iterator;

	/// <summary>
	/// Set const迭代器
	/// </summary>
	typedef typename RBTree<K, K, SetKeyOfT>::ConstIterator const_iterator;



	/// <summary>
	/// 向Set对象中插入数据
	/// </summary>
	/// <param name="key">待插入的键</param>
	/// <returns>
	/// 插入成功,返回{新插入数据的迭代器,true};
	/// 插入失败,返回{已经存在的键的迭代器,false}
	/// </returns>
	pair<iterator,bool> insert(const K& key) 
	{
		auto insertResult = _RBTree.insert(key);
		return { iterator(insertResult.first,_RBTree.getRoot()),insertResult.second };
	}



	/// <summary>
	/// 返回Set对象的大小
	/// </summary>
	/// <returns>返回Set对象的大小</returns>
	size_t size()
	{
		return _RBTree.size();
	}



	/// <summary>
	/// 返回begin迭代器
	/// </summary>
	/// <returns>返回const begin迭代器</returns>
	iterator begin() const 
	{
		return _RBTree.begin();
	}

	/// <summary>
	/// 返回end迭代器
	/// </summary>
	/// <returns>返回const end迭代器</returns>
	iterator end() const
	{
		return _RBTree.end();
	}
};