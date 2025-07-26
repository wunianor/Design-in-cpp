#pragma once

#include <vector>
#include <string>
using namespace std;

/// <summary>
/// 哈希结点
/// </summary>
/// <typeparam name="K">键的类型</typeparam>
/// <typeparam name="T">数据的类型</typeparam>
template<class K,class T>
struct HashNode
{
	/// <summary>
	/// 结点的数据
	/// </summary>
	T _data;

	/// <summary>
	/// 下一个结点
	/// </summary>
	HashNode<K,T>* _next;



	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="data">用于初始化数据</param>
	HashNode(const T& data):
		_data(data),
		_next(nullptr)
	{}
};


/// <summary>
/// 哈希仿函数,求键的哈希值
/// </summary>
/// <typeparam name="K">键的类型</typeparam>
template<class K>
struct Hash
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

/// <summary>
/// 字符串哈希仿函数,求字符串的哈希值
/// BKDRHash算法
/// </summary>
template<>
struct Hash<string>//类模板特化
{
	/// <summary>
	/// 求字符串的哈希值
	/// </summary>
	/// <param name="key">待求哈希值的字符串</param>
	/// <returns>返回字符串的哈希值</returns>
	size_t operator()(const string& key)
	{
		size_t hashValue = 0;

		for (auto ch : key)
		{
			hashValue = hashValue * 31 + ch;
		}

		return hashValue;
	}

};



/// <summary>
/// 哈希表迭代器类
/// </summary>
/// <typeparam name="K">键的类型</typeparam>
/// <typeparam name="T">数据的类型</typeparam>
/// <typeparam name="Hash">哈希函数</typeparam>
/// <typeparam name="Ref">T类型数据的引用类型</typeparam>
/// <typeparam name="Ptr">T类型数据的指针类型</typeparam>
/// <typeparam name="KeyOfT">求 T类型数据的键的值 的仿函数</typeparam>
template<class K, class T, class Ref,class Ptr,class KeyOfT, class Hash>
struct __HashTable_Iterator
{
	typedef HashNode<K, T> Node;
	typedef __HashTable_Iterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;

	/// <summary>
	/// 哈希结点的指针
	/// </summary>
	Node* _node;

	/// <summary>
	/// 哈希桶(单链表)数组的指针
	/// </summary>
	const vector<Node*>* _buckets_ptr;

	/// <summary>
	/// 获取T类型的值的仿函数
	/// </summary>
	KeyOfT _keyOfT;

	/// <summary>
	/// 计算键的哈希值的仿函数
	/// </summary>
	Hash _hashFunc;

	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="node">哈希结点的指针</param>
	/// <param name="buckets_ptr">哈希桶数组的指针</param>
	__HashTable_Iterator(Node* const node,const vector<Node*>* const buckets_ptr):
		_node(node),
		_buckets_ptr(buckets_ptr)
	{ }

	/// <summary>
	/// 重载*
	/// </summary>
	/// <returns></returns>
	Ref operator*()
	{
		return _node->_data;
	}

	/// <summary>
	/// 重载->
	/// </summary>
	/// <returns></returns>
	Ptr operator->()
	{
		return &(_node->_data);
	}



	/// <summary>
	/// 重载前置++
	/// </summary>
	/// <returns></returns>
	Self& operator++()
	{
		if (_node->_next)//如果当前桶没有遍历完
		{
			
			_node = _node->_next;
		}
		else//如果当前桶遍历完了
		{
			//计算_node在哪个哈希桶
			size_t index = _hashFunc(_keyOfT(_node->_data)) % _buckets_ptr->size();

			++index;

			while (index < _buckets_ptr->size())
			{
				if (_buckets_ptr->operator[](index))
				{
					_node = _buckets_ptr->operator[](index);
					break;
				}

				++index;
			}

			if (index == _buckets_ptr->size())
			{
				_node = nullptr;
			}
		}


		return *this;
	}

	/// <summary>
	/// 重载后置++
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Self operator++(int)
	{
		Self tmp = *this;

		++*this;

		return tmp;
	}



	/// <summary>
	/// 重载==
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator==(const Self& other)
	{
		return _node == other._node;
	}

	/// <summary>
	/// 重载!=
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator!=(const Self& other)
	{
		return !(*this == other);
	}


};



/// <summary>
/// 哈希表类
/// </summary>
/// <typeparam name="K">键的类型</typeparam>
/// <typeparam name="T">数据的类型</typeparam>
/// <typeparam name="KeyOfT">求 T类型数据的键的值 的仿函数</typeparam>
/// <typeparam name="Hash">哈希函数</typeparam>
template<class K,class T,class KeyOfT,class Hash>
class HashTable
{
	typedef HashNode<K, T> Node;
	
	/// <summary>
	/// 哈希桶数组,一个以Node*指针为元素类型的单链表表示一个哈希桶
	/// </summary>
	vector<Node*> _buckets;  

	/// <summary>
	/// HashTable中键的个数
	/// </summary>
	size_t _size = 0;        

	/// <summary>
	/// 哈希仿函数,用于计算键的哈希值
	/// </summary>
	Hash _hashFunc;          

	/// <summary>
	/// 取T类型的仿函数
	/// </summary>
	KeyOfT _keyOfT;          

public:

	/// <summary>
	/// 普通迭代器
	/// </summary>
	typedef __HashTable_Iterator<K, T, T&, T*, KeyOfT, Hash> Iterator;

	/// <summary>
	/// const迭代器
	/// </summary>
	typedef __HashTable_Iterator<K, T, const T&, const T*, KeyOfT, Hash> ConstIterator;

	/// <summary>
	/// 构造函数
	/// </summary>
	HashTable()
	{
		_buckets.resize(13, nullptr);
	}

	/// <summary>
	/// 拷贝构造
	/// </summary>
	/// <param name="other"></param>
	HashTable(const HashTable<K, T, KeyOfT,Hash>& other)
	{
		_buckets.resize(other._buckets.size(), nullptr);
		for (Node* cur : other._buckets)
		{
			while (cur)
			{
				insert(cur->_data);
				cur = cur->_next;
			}
		}
	}

	/// <summary>
	/// 赋值重载
	/// </summary>
	/// <param name="other"></param>
	void operator=(HashTable<K, T, KeyOfT, Hash> other)
	{
		_buckets.swap(other._buckets);
		swap(_size, other._size);
	}



	/// <summary>
	/// 插入键值
	/// </summary>
	/// <param name="kv">待插入的键值</param>
	/// <returns>插入成功返回true,失败返回false</returns>
	pair<Iterator, bool> insert(const T& data)
	{
		//如果键已经存在
		Iterator it = find(_keyOfT(data));
		if (it != end())
		{
			return { it,false };
		}

		//如果负载因子等于1
		if (_size == _buckets.size())
		{
			vector<Node*> newBuckets(_buckets.size() * 2 + 1);

			for (Node*& cur : _buckets) //加了引用,可以自动让cur最后变为nullptr
			{
				while (cur)
				{
					Node* next = cur->_next;

					//将cur结点移动到新的哈希桶
					size_t newIndex = _hashFunc(_keyOfT(cur->_data)) % newBuckets.size();
					cur->_next = newBuckets[newIndex];
					newBuckets[newIndex] = cur;

					cur = next;
				}
			}

			_buckets.swap(newBuckets);
		}

		//计算键的存储位置
		size_t index = _hashFunc(_keyOfT(data)) % _buckets.size();

		//创建新结点
		Node* newNode = new Node(data);

		//头插
		newNode->_next = _buckets[index];
		_buckets[index] = newNode;

		//更新哈希表元素个数
		++_size;

		return { Iterator(newNode,&_buckets),true };
	}

	/// <summary>
	/// 查找键
	/// </summary>
	/// <param name="key">需要查找的键</param>
	/// <returns>找到返回键的迭代器,否则返回end()</returns>
	Iterator find(const K& key)
	{
		//计算键的在哪个桶
		size_t index = _hashFunc(key) % _buckets.size();

		//在桶里面寻找key
		Node* cur = _buckets[index];
		while (cur)
		{
			if (_keyOfT(cur->_data) == key)
			{
				return Iterator(cur, &_buckets);
			}
			cur = cur->_next;
		}

		//走到这里说明桶内没有key,返回end()
		return end();
	}

	/// <summary>
	/// 删除键
	/// </summary>
	/// <param name="key">待删除的键</param>
	/// <returns>删除成功返回true,否则返回false</returns>
	bool erase(const K& key)
	{
		//计算键在哪个桶
		size_t index = _hashFunc(key) % _buckets.size();

		//在桶里面寻找这个元素并删除
		Node* prev = nullptr;
		Node* cur = _buckets[index];
		while (cur)
		{
			if (_keyOfT(cur->_data) == key)
			{
				if (prev)
				{
					prev->_next = cur->_next;
				}
				else
				{
					_buckets[index] = cur->_next;
				}

				delete cur;

				--_size;

				return true;
			}
			prev = cur;
			cur = cur->_next;
		}

		//走到这里cur为空,说明桶内没有key,返回false
		return false;
	}



	/// <summary>
	/// 返回普通begin迭代器
	/// </summary>
	/// <returns>返回普通begin迭代器</returns>
	Iterator begin()
	{
		Node* cur = nullptr;
		for (Node* head : _buckets)
		{
			if (head)
			{
				cur = head;
				break;
			}
		}
		return Iterator(cur, &_buckets);
	}

	/// <summary>
	/// 返回const begin迭代器
	/// </summary>
	/// <returns>返回const begin迭代器</returns>
	ConstIterator begin() const
	{
		Node* cur = nullptr;
		for (Node* head : _buckets)
		{
			if (head)
			{
				cur = head;
				break;
			}
		}
		return ConstIterator(cur, &_buckets);
	}

	/// <summary>
	/// 返回普通end迭代器
	/// </summary>
	/// <returns>返回普通end迭代器</returns>
	Iterator end()
	{
		return Iterator(nullptr, &_buckets);
	}

	/// <summary>
	/// 返回const end迭代器
	/// </summary>
	/// <returns>返回const end迭代器</returns>
	ConstIterator end() const
	{
		return ConstIterator(nullptr, &_buckets);
	}



	/// <summary>
	/// 求哈希表的元素个数
	/// </summary>
	/// <returns>返回哈希表的元素个数</returns>
	size_t size() const
	{
		return _size;
	}



	/// <summary>
	/// 析构函数
	/// </summary>
	~HashTable()
	{
		for (Node*& cur : _buckets)//加了引用,可以自动让cur最后变为nullptr
		{
			while (cur)
			{
				Node* next = cur->_next;
				delete cur;
				cur = next;
			}
		}
	}
};