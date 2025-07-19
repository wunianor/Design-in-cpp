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
	struct __List_iterator//List��������ģ��
	{
		typedef List_node<T> Node;
		typedef __List_iterator<T,Ref,Ptr> self;
		Node* _node;//��һ��List_node<T>��ָ��,ָ��List_node<T>���͵�һ������

		__List_iterator(Node* node):
			_node(node){}
		//����Ҫд��������͸�ֵ����------��Ϊǳ�����͹�����,���������
		
		Ref operator*()
		{
			return _node->_data;
		}
		Ptr operator->()//Ϊ�Զ�������׼����
		{
			return &_node->_data;//����->,����&,����һ��T*��ָ��
			//ʹ�÷�ʽ:�ٵ�����->��Ա����
			//		   �ڵ�����.operator->()->��Ա����
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

		//����д����
	};







	template<class T>
	class List
	{
		typedef List_node<T> Node;
		Node* _head;
		size_t _size;

	public:
		typedef __List_iterator<T, T&, T*> iterator;//��ͨ������
		typedef __List_iterator<T,const T& ,const T*> const_iterator;//const������

		typedef Reverse_iterator<iterator, T&, T*> reverse_iterator;//���������
		typedef Reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;//����const������


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
		//List<T>& operator=(const List<T>& l1)//��ֵ���ش�ͳд��
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
		List<T>& operator=(List<T> lt/*��ʱ�����������������*/)//��ֵ�����ִ�д��
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

		//��ȡ��һ��Ԫ��
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

		//��ȡ���һ��Ԫ��
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

		iterator insert(iterator pos,const T& val)//���ᷢ��������ʧЧ
		{
			Node* cur = pos._node;
			Node* new_node = new Node(val);
			Node* cur_prev = cur->_prev;

			new_node->_prev = cur_prev;
			new_node->_next = cur;

			cur_prev->_next = new_node;
			cur->_prev = new_node;

			_size++;
			return new_node;//��ʽ����ת��
		}
		iterator erase(iterator pos)//����ᷢ��������ʧЧ
		{
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* next = cur->_next;

			delete cur;

			prev->_next = next;
			next->_prev = prev;

			_size--;
			return next;//��ʽ����ת��
		}

		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		//������
		iterator begin()
		{
			return _head->_next;//��ʽ����ת��
		}
		const_iterator begin() const
		{
			return _head->_next;//��ʽ����ת��
		}
		iterator end()
		{
			return _head;//��ʽ����ת��
		}
		const_iterator end() const
		{
			return _head;//��ʽ����ת��
		}
		reverse_iterator rbegin()
		{
			return iterator(end());//��ʽ����ת��
		}
		reverse_iterator rend()
		{
			return iterator(begin());//��ʽ����ת��
		}
		const_reverse_iterator rbegin() const
		{
			return const_iterator(end());//��ʽ����ת��
		}
		const_reverse_iterator rend() const
		{
			return const_iterator(begin());//��ʽ����ת��
		}


		~List()
		{
			clear();

			//ͷ�ڵ�ҲҪ�ͷ�
			delete _head;
			_head = nullptr;
		}
	};


}