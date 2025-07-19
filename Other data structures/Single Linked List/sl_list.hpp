#pragma once
#include <assert.h>

namespace SL_LIST
{

	//单链表结点类模板
	template<class T>
	struct sl_ListNode
	{
		T _data;
		sl_ListNode* _next;

		sl_ListNode(const T& data = T()) :
			_data(data), _next(nullptr) {}
	};

	//单链表迭代器类模板
	template<class T,class Ref,class Ptr>
	struct sl_list_iterator
	{
		typedef sl_ListNode<T> Node;
		typedef sl_list_iterator<T, Ref, Ptr> self;
		
		Node* _node;

		sl_list_iterator(Node* node):
			_node(node){}

		//重载解引用操作符,返回T&(迭代器指向的元素的引用)
		Ref operator*()
		{
			return _node->_data;
		}

		//重载->,返回T*
		Ptr operator->()
		{
			return &(_node->_data);
		}

		//前置++
		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		//后置++
		self operator++(int)
		{
			self tmp = *this;
			_node = _node->_next;
			return tmp;
		}

		//判断两个迭代器是否相等
		bool operator==(const self& s)
		{
			return _node == s._node;
		}

		//判断两个迭代器是否不等
		bool operator!=(const self& s)
		{
			return _node != s._node;
		}
	};

	//单链表类模板
	template<class T>
	class sl_list
	{
		typedef sl_ListNode<T> Node;

		Node* _node;
		size_t _size;

		//逆置子函数
		void _reverse(Node* head,Node*& new_head, Node*& new_tail)
		{
			new_tail = head;

			Node* prev = nullptr;
			Node* cur = head;
			Node* next = nullptr;

			while (cur)
			{
				next = cur->_next;
				cur->_next = prev;
				prev = cur;
				cur = next;
			}
			new_head = prev;
		}

	public:
		typedef sl_list_iterator<T, T&, T*> iterator;//普通迭代器
		typedef sl_list_iterator<T, const T&, const T*> const_iterator;//const迭代器

		//空初始化
		void empty_init()
		{
			_node = new Node;//虚拟头结点
			_node->_next = new Node;//虚拟尾结点
			_size = 0;
		}

		//构造函数
		sl_list()
		{
			empty_init();
		}

		//拷贝构造函数
		sl_list(const sl_list<int>& sl)
		{
			empty_init();
			for (auto& e : sl)
			{
				push_back(e);
			}
		}


		//插入
		iterator insert(iterator pos, const T& val)
		{
			Node* cur = pos._node;
			Node* cur_prev = _node;
			while (cur_prev->_next != cur)
			{
				cur_prev = cur_prev->_next;
			}
			
			Node* new_node = new Node(val);
			cur_prev->_next = new_node;
			new_node->_next = cur;

			++_size;
			return new_node;
		}

		//尾插
		void push_back(const T& val)
		{
			insert(end(), val);
		}

		//删除
		iterator erase(iterator pos)
		{
			Node* cur = pos._node;
			Node* cur_prev = _node;
			while (cur_prev->_next != cur)
			{
				cur_prev = cur_prev->_next;
			}
			Node* cur_next = cur->_next;

			cur_prev->_next = cur->_next;
			--_size;
			delete cur;

			return cur_next;
		}

		//尾删
		void pop_back()
		{
			assert(size() > 0);

			iterator it = begin();
			iterator del = begin();
			while (it != end())
			{
				del = it++;
			}
			erase(del);
		}


		//逆置
		void reverse()
		{
			if (size() <= 1)
			{
				return;
			}

			Node* virtual_tail = iterator(end())._node;

			//切断最后一个有效结点与虚拟尾结点的链接
			Node* cur = _node;
			while (cur->_next != virtual_tail)
			{
				cur = cur->_next;
			}
			cur->_next = nullptr;

			Node* new_head = nullptr;//逆置后新的头结点
			Node* new_tail = nullptr;//逆置后新的尾结点

			_reverse(_node->_next, new_head, new_tail);

			_node->_next = new_head;
			new_tail->_next = virtual_tail;
		}


		//清空
		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}


		//返回大小
		size_t size() const 
		{
			return _size;
		}


		//普通begin迭代器
		iterator begin()
		{
			return _node->_next;
		}

		//普通end迭代器
		iterator end()
		{
			Node* cur = _node->_next;
			while (cur->_next != nullptr)
			{
				cur = cur->_next;
			}
			return cur;
		}

		//const begin 迭代器
		const_iterator begin() const
		{
			return _node->_next;
		}

		//const end 迭代器
		const_iterator end() const
		{
			Node* cur = _node->_next;
			while (cur->_next != nullptr)
			{
				cur = cur->_next;
			}
			return cur;
		}


		//析构函数
		~sl_list()
		{
			clear();

			delete _node->_next;
			_node->_next = nullptr;

			delete _node;
			_node = nullptr;

		}
	};
}