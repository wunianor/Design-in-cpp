#pragma once


#include "Reverse_iterator.h"


namespace LIST
{
	template<class T>
	struct List_node
	{
		T _data;
		List_node<T>* _next;
		List_node<T>* _prev;

		List_node(const T& x = T()) 
			:_data(x),
			_next(nullptr),
			_prev(nullptr)
		{}
	};



	template<class T,class Ref ,class Ptr>
	struct __List_iterator//List迭代器类模板
	{
		typedef List_node<T> Node;
		typedef __List_iterator<T,Ref,Ptr> self;
		Node* _node;//是一个List_node<T>的指针,指向List_node<T>类型的一个对象

		__List_iterator(Node* node):
			_node(node){}
		//不需要写拷贝构造和赋值重载------因为浅拷贝就够用了,不能用深拷贝
		
		Ref operator*()
		{
			return _node->_data;
		}
		Ptr operator->()//为自定义类型准备的
		{
			return &_node->_data;//先算->,再算&,返回一个T*的指针
			//使用方式:①迭代器->成员变量
			//		   ②迭代器.operator->()->成员变量
		}
		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			_node = _node->_next;
			return tmp;
		}
		self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			_node = _node->_prev;
			return tmp;
		}


		self& operator+=(const T& val)
		{
			_node->_data += val;
			return *this;
		}


		bool operator!=(const self& l) 
		{
			return _node != l._node;
		}
		bool operator==(const self& l)
		{
			return _node == l._node;
		}

		//不能写析构
	};







	template<class T>
	class List
	{
		typedef List_node<T> Node;
		Node* _head;
		size_t _size;

	public:
		typedef __List_iterator<T, T&, T*> iterator;//普通迭代器
		typedef __List_iterator<T,const T& ,const T*> const_iterator;//const迭代器

		typedef Reverse_iterator<iterator, T&, T*> reverse_iterator;//反向迭代器
		typedef Reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;//反向const迭代器


		void empty_init()
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			_size = 0;
		}
		List()
		{
			empty_init();
		}
		List(const List<T>& lt)
		{
			empty_init();
			for (auto it : lt)
			{
				push_back(it);
			}
		}
		//List<T>& operator=(const List<T>& l1)//赋值重载传统写法
		//{
		//	if (this != &s1)
		//	{
		//		clear();
		//		for (auto it : l1)
		//		{
		//			push_back(it);
		//		}
		//	}
		//	return *this;
		//}
		List<T>& operator=(List<T> lt/*临时对象出作用域会调析构*/)//赋值重载现代写法
		{
			swap(lt);
			return *this;
		}

		void swap(List<T>& lt)
		{
			std::swap(_head,lt._head);
			std::swap(_size,lt._size);
		}
		size_t size() const
		{
			return _size;
		}

		void push_front(const T& val)
		{
			insert(begin(), val);
		}
		void push_back(const T& val)
		{
			insert(end(),val);
		}
		void pop_front()
		{
			erase(begin());
		}
		void pop_back()
		{
			erase(--end());
		}

		//获取第一个元素
		T& front()
		{
			assert(size() > 0);
			return _head->_next->_data;
		}

		const T& front() const
		{
			assert(size() > 0);
			return _head->_next->_data;
		}

		//获取最后一个元素
		T& back()
		{
			assert(size() > 0);
			return _head->_prev->_data;
		}

		const T& back() const 
		{
			assert(size() > 0);
			return _head->_prev->_data;
		}

		iterator insert(iterator pos,const T& val)//不会发生迭代器失效
		{
			Node* cur = pos._node;
			Node* new_node = new Node(val);
			Node* cur_prev = cur->_prev;

			new_node->_prev = cur_prev;
			new_node->_next = cur;

			cur_prev->_next = new_node;
			cur->_prev = new_node;

			_size++;
			return new_node;//隐式类型转换
		}
		iterator erase(iterator pos)//这里会发生迭代器失效
		{
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* next = cur->_next;

			delete cur;

			prev->_next = next;
			next->_prev = prev;

			_size--;
			return next;//隐式类型转换
		}

		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		//迭代器
		iterator begin()
		{
			return _head->_next;//隐式类型转换
		}
		const_iterator begin() const
		{
			return _head->_next;//隐式类型转换
		}
		iterator end()
		{
			return _head;//隐式类型转换
		}
		const_iterator end() const
		{
			return _head;//隐式类型转换
		}
		reverse_iterator rbegin()
		{
			return iterator(end());//隐式类型转换
		}
		reverse_iterator rend()
		{
			return iterator(begin());//隐式类型转换
		}
		const_reverse_iterator rbegin() const
		{
			return const_iterator(end());//隐式类型转换
		}
		const_reverse_iterator rend() const
		{
			return const_iterator(begin());//隐式类型转换
		}


		~List()
		{
			clear();

			//头节点也要释放
			delete _head;
			_head = nullptr;
		}
	};


}