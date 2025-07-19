#pragma once
#include <assert.h>

namespace SL_LIST
{

	//����������ģ��
	template<class T>
	struct sl_ListNode
	{
		T _data;
		sl_ListNode* _next;

		sl_ListNode(const T& data = T()) :
			_data(data), _next(nullptr) {}
	};

	//�������������ģ��
	template<class T,class Ref,class Ptr>
	struct sl_list_iterator
	{
		typedef sl_ListNode<T> Node;
		typedef sl_list_iterator<T, Ref, Ptr> self;
		
		Node* _node;

		sl_list_iterator(Node* node):
			_node(node){}

		//���ؽ����ò�����,����T&(������ָ���Ԫ�ص�����)
		Ref operator*()
		{
			return _node->_data;
		}

		//����->,����T*
		Ptr operator->()
		{
			return &(_node->_data);
		}

		//ǰ��++
		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		//����++
		self operator++(int)
		{
			self tmp = *this;
			_node = _node->_next;
			return tmp;
		}

		//�ж������������Ƿ����
		bool operator==(const self& s)
		{
			return _node == s._node;
		}

		//�ж������������Ƿ񲻵�
		bool operator!=(const self& s)
		{
			return _node != s._node;
		}
	};

	//��������ģ��
	template<class T>
	class sl_list
	{
		typedef sl_ListNode<T> Node;

		Node* _node;
		size_t _size;

		//�����Ӻ���
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
		typedef sl_list_iterator<T, T&, T*> iterator;//��ͨ������
		typedef sl_list_iterator<T, const T&, const T*> const_iterator;//const������

		//�ճ�ʼ��
		void empty_init()
		{
			_node = new Node;//����ͷ���
			_node->_next = new Node;//����β���
			_size = 0;
		}

		//���캯��
		sl_list()
		{
			empty_init();
		}

		//�������캯��
		sl_list(const sl_list<int>& sl)
		{
			empty_init();
			for (auto& e : sl)
			{
				push_back(e);
			}
		}


		//����
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

		//β��
		void push_back(const T& val)
		{
			insert(end(), val);
		}

		//ɾ��
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

		//βɾ
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


		//����
		void reverse()
		{
			if (size() <= 1)
			{
				return;
			}

			Node* virtual_tail = iterator(end())._node;

			//�ж����һ����Ч���������β��������
			Node* cur = _node;
			while (cur->_next != virtual_tail)
			{
				cur = cur->_next;
			}
			cur->_next = nullptr;

			Node* new_head = nullptr;//���ú��µ�ͷ���
			Node* new_tail = nullptr;//���ú��µ�β���

			_reverse(_node->_next, new_head, new_tail);

			_node->_next = new_head;
			new_tail->_next = virtual_tail;
		}


		//���
		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}


		//���ش�С
		size_t size() const 
		{
			return _size;
		}


		//��ͨbegin������
		iterator begin()
		{
			return _node->_next;
		}

		//��ͨend������
		iterator end()
		{
			Node* cur = _node->_next;
			while (cur->_next != nullptr)
			{
				cur = cur->_next;
			}
			return cur;
		}

		//const begin ������
		const_iterator begin() const
		{
			return _node->_next;
		}

		//const end ������
		const_iterator end() const
		{
			Node* cur = _node->_next;
			while (cur->_next != nullptr)
			{
				cur = cur->_next;
			}
			return cur;
		}


		//��������
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