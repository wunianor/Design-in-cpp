#pragma once

#include <vector>
#include <string>
using namespace std;

/// <summary>
/// ��ϣ���
/// </summary>
/// <typeparam name="K">��������</typeparam>
/// <typeparam name="T">���ݵ�����</typeparam>
template<class K,class T>
struct HashNode
{
	/// <summary>
	/// ��������
	/// </summary>
	T _data;

	/// <summary>
	/// ��һ�����
	/// </summary>
	HashNode<K,T>* _next;



	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="data">���ڳ�ʼ������</param>
	HashNode(const T& data):
		_data(data),
		_next(nullptr)
	{}
};


/// <summary>
/// ��ϣ�º���,����Ĺ�ϣֵ
/// </summary>
/// <typeparam name="K">��������</typeparam>
template<class K>
struct Hash
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

/// <summary>
/// �ַ�����ϣ�º���,���ַ����Ĺ�ϣֵ
/// BKDRHash�㷨
/// </summary>
template<>
struct Hash<string>//��ģ���ػ�
{
	/// <summary>
	/// ���ַ����Ĺ�ϣֵ
	/// </summary>
	/// <param name="key">�����ϣֵ���ַ���</param>
	/// <returns>�����ַ����Ĺ�ϣֵ</returns>
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
/// ��ϣ���������
/// </summary>
/// <typeparam name="K">��������</typeparam>
/// <typeparam name="T">���ݵ�����</typeparam>
/// <typeparam name="Hash">��ϣ����</typeparam>
/// <typeparam name="Ref">T�������ݵ���������</typeparam>
/// <typeparam name="Ptr">T�������ݵ�ָ������</typeparam>
/// <typeparam name="KeyOfT">�� T�������ݵļ���ֵ �ķº���</typeparam>
template<class K, class T, class Ref,class Ptr,class KeyOfT, class Hash>
struct __HashTable_Iterator
{
	typedef HashNode<K, T> Node;
	typedef __HashTable_Iterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;

	/// <summary>
	/// ��ϣ����ָ��
	/// </summary>
	Node* _node;

	/// <summary>
	/// ��ϣͰ(������)�����ָ��
	/// </summary>
	const vector<Node*>* _buckets_ptr;

	/// <summary>
	/// ��ȡT���͵�ֵ�ķº���
	/// </summary>
	KeyOfT _keyOfT;

	/// <summary>
	/// ������Ĺ�ϣֵ�ķº���
	/// </summary>
	Hash _hashFunc;

	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="node">��ϣ����ָ��</param>
	/// <param name="buckets_ptr">��ϣͰ�����ָ��</param>
	__HashTable_Iterator(Node* const node,const vector<Node*>* const buckets_ptr):
		_node(node),
		_buckets_ptr(buckets_ptr)
	{ }

	/// <summary>
	/// ����*
	/// </summary>
	/// <returns></returns>
	Ref operator*()
	{
		return _node->_data;
	}

	/// <summary>
	/// ����->
	/// </summary>
	/// <returns></returns>
	Ptr operator->()
	{
		return &(_node->_data);
	}



	/// <summary>
	/// ����ǰ��++
	/// </summary>
	/// <returns></returns>
	Self& operator++()
	{
		if (_node->_next)//�����ǰͰû�б�����
		{
			
			_node = _node->_next;
		}
		else//�����ǰͰ��������
		{
			//����_node���ĸ���ϣͰ
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
	/// ���غ���++
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
	/// ����==
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator==(const Self& other)
	{
		return _node == other._node;
	}

	/// <summary>
	/// ����!=
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator!=(const Self& other)
	{
		return !(*this == other);
	}


};



/// <summary>
/// ��ϣ����
/// </summary>
/// <typeparam name="K">��������</typeparam>
/// <typeparam name="T">���ݵ�����</typeparam>
/// <typeparam name="KeyOfT">�� T�������ݵļ���ֵ �ķº���</typeparam>
/// <typeparam name="Hash">��ϣ����</typeparam>
template<class K,class T,class KeyOfT,class Hash>
class HashTable
{
	typedef HashNode<K, T> Node;
	
	/// <summary>
	/// ��ϣͰ����,һ����Node*ָ��ΪԪ�����͵ĵ������ʾһ����ϣͰ
	/// </summary>
	vector<Node*> _buckets;  

	/// <summary>
	/// HashTable�м��ĸ���
	/// </summary>
	size_t _size = 0;        

	/// <summary>
	/// ��ϣ�º���,���ڼ�����Ĺ�ϣֵ
	/// </summary>
	Hash _hashFunc;          

	/// <summary>
	/// ȡT���͵ķº���
	/// </summary>
	KeyOfT _keyOfT;          

public:

	/// <summary>
	/// ��ͨ������
	/// </summary>
	typedef __HashTable_Iterator<K, T, T&, T*, KeyOfT, Hash> Iterator;

	/// <summary>
	/// const������
	/// </summary>
	typedef __HashTable_Iterator<K, T, const T&, const T*, KeyOfT, Hash> ConstIterator;

	/// <summary>
	/// ���캯��
	/// </summary>
	HashTable()
	{
		_buckets.resize(13, nullptr);
	}

	/// <summary>
	/// ��������
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
	/// ��ֵ����
	/// </summary>
	/// <param name="other"></param>
	void operator=(HashTable<K, T, KeyOfT, Hash> other)
	{
		_buckets.swap(other._buckets);
		swap(_size, other._size);
	}



	/// <summary>
	/// �����ֵ
	/// </summary>
	/// <param name="kv">������ļ�ֵ</param>
	/// <returns>����ɹ�����true,ʧ�ܷ���false</returns>
	pair<Iterator, bool> insert(const T& data)
	{
		//������Ѿ�����
		Iterator it = find(_keyOfT(data));
		if (it != end())
		{
			return { it,false };
		}

		//����������ӵ���1
		if (_size == _buckets.size())
		{
			vector<Node*> newBuckets(_buckets.size() * 2 + 1);

			for (Node*& cur : _buckets) //��������,�����Զ���cur����Ϊnullptr
			{
				while (cur)
				{
					Node* next = cur->_next;

					//��cur����ƶ����µĹ�ϣͰ
					size_t newIndex = _hashFunc(_keyOfT(cur->_data)) % newBuckets.size();
					cur->_next = newBuckets[newIndex];
					newBuckets[newIndex] = cur;

					cur = next;
				}
			}

			_buckets.swap(newBuckets);
		}

		//������Ĵ洢λ��
		size_t index = _hashFunc(_keyOfT(data)) % _buckets.size();

		//�����½��
		Node* newNode = new Node(data);

		//ͷ��
		newNode->_next = _buckets[index];
		_buckets[index] = newNode;

		//���¹�ϣ��Ԫ�ظ���
		++_size;

		return { Iterator(newNode,&_buckets),true };
	}

	/// <summary>
	/// ���Ҽ�
	/// </summary>
	/// <param name="key">��Ҫ���ҵļ�</param>
	/// <returns>�ҵ����ؼ��ĵ�����,���򷵻�end()</returns>
	Iterator find(const K& key)
	{
		//����������ĸ�Ͱ
		size_t index = _hashFunc(key) % _buckets.size();

		//��Ͱ����Ѱ��key
		Node* cur = _buckets[index];
		while (cur)
		{
			if (_keyOfT(cur->_data) == key)
			{
				return Iterator(cur, &_buckets);
			}
			cur = cur->_next;
		}

		//�ߵ�����˵��Ͱ��û��key,����end()
		return end();
	}

	/// <summary>
	/// ɾ����
	/// </summary>
	/// <param name="key">��ɾ���ļ�</param>
	/// <returns>ɾ���ɹ�����true,���򷵻�false</returns>
	bool erase(const K& key)
	{
		//��������ĸ�Ͱ
		size_t index = _hashFunc(key) % _buckets.size();

		//��Ͱ����Ѱ�����Ԫ�ز�ɾ��
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

		//�ߵ�����curΪ��,˵��Ͱ��û��key,����false
		return false;
	}



	/// <summary>
	/// ������ͨbegin������
	/// </summary>
	/// <returns>������ͨbegin������</returns>
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
	/// ����const begin������
	/// </summary>
	/// <returns>����const begin������</returns>
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
	/// ������ͨend������
	/// </summary>
	/// <returns>������ͨend������</returns>
	Iterator end()
	{
		return Iterator(nullptr, &_buckets);
	}

	/// <summary>
	/// ����const end������
	/// </summary>
	/// <returns>����const end������</returns>
	ConstIterator end() const
	{
		return ConstIterator(nullptr, &_buckets);
	}



	/// <summary>
	/// ���ϣ���Ԫ�ظ���
	/// </summary>
	/// <returns>���ع�ϣ���Ԫ�ظ���</returns>
	size_t size() const
	{
		return _size;
	}



	/// <summary>
	/// ��������
	/// </summary>
	~HashTable()
	{
		for (Node*& cur : _buckets)//��������,�����Զ���cur����Ϊnullptr
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