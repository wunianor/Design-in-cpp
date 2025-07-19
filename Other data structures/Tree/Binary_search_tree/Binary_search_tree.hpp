#include <iostream>
using namespace std;


namespace K//ֻ�м�Key,û��ֵValue
{

	template<class K>
	struct BST_Node
	{
		K _key;
		BST_Node* _left;
		BST_Node* _right;

		BST_Node(const K& key) :
			_key(key), _left(nullptr), _right(nullptr) {}

	};


	template <class K>
	class BST
	{
		typedef BST_Node<K> Node;

		Node* _root;


		//ǰ������Ӻ���
		void _prev_order(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}


			cout << (root->_key) << " ";
			_prev_order(root->_left);
			_prev_order(root->_right);
		}

		//��������Ӻ���
		void _in_order(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			_in_order(root->_left);
			cout << (root->_key) << " ";
			_in_order(root->_right);
		}


		//���Ҽ�---�ݹ���Ӻ���
		bool _findR(Node* root, const K& key)
		{
			if (root == nullptr)
			{
				return false;
			}

			if (key < root->_key)
			{
				return _findR(root->_left, key);
			}
			else if (root->_key < key)
			{
				return _findR(root->_right, key);
			}

			return true;
		}

		//�����---�ݹ���Ӻ���
		bool _insertR(Node*& root, const K& key)//ע��&������,��������
		{
			if (root == nullptr)
			{
				//root���ϲ���õı���,��root�ĸı��Ӱ���ϲ�
				root = new Node(key);
				return true;
			}

			if (key < root->_key)
			{
				return _insertR(root->_left, key);
			}
			else if (root->_key < key)
			{
				return _insertR(root->_right, key);
			}
			else
			{
				return false;
			}
		}

		//ɾ����---�ݹ���Ӻ���
		bool _eraseR(Node*& root, const K& key)//rootΪ�ϲ��_left��_right�ı���
		{
			if (root == nullptr)//û�ҵ���Ϊkey�Ľ��
			{
				return false;
			}

			if (key < root->_key)
			{
				return _eraseR(root->_left, key);
			}
			else if (root->_key < key)
			{
				return _eraseR(root->_right, key);
			}

			Node* del = root;

			if (root->_left == nullptr)//��ɾ������������Ϊ��
			{
				root = root->_right;
				delete del;
			}
			else if (root->_right == nullptr)//��ɾ������������Ϊ��
			{
				root = root->_left;
				delete del;
			}
			else
			{
				//Ѱ������������С�����(����������С����㱣֤���ֻ��һ���ӽ��)
				Node* min_in_right = root->_right;
				while (min_in_right->_left)
				{
					min_in_right = min_in_right->_left;
				}

				//������
				swap(root->_key, min_in_right->_key);

				//ȥ������ɾ����Ϊkey�Ľ��
				return _eraseR(root->_right, key);
			}
			return true;

		}

		//������������(����)
		void _destroy(Node*& root)
		{
			if (root == nullptr)
			{
				return;
			}

			_destroy(root->_left);
			_destroy(root->_right);
			delete root;
			root = nullptr;
		}

		//�����������캯��(ǰ��)
		Node* _copy(Node* root)
		{
			if (root == nullptr)
			{
				return nullptr;
			}

			Node* new_root = new Node(root->_key);
			new_root->_left = _copy(root->_left);
			new_root->_right = _copy(root->_right);

			return new_root;

		}

	public:
		//�޲ι��캯��
		BST() = default;//C++11д��---ǿ������Ĭ�Ϲ��캯��

		//�������캯��
		BST(const BST<K>& t)
		{
			_root = _copy(t._root);
		}

		//��ֵ�����ִ�д��
		BST<K>& operator=(BST<K> t)
		{
			swap(_root, t._root);
			return *this;
		}

		//�����
		bool insert(const K& key)
		{
			if (_root == nullptr)//��Ϊ��
			{
				_root = new Node(key);
				return true;
			}

			Node* parent = nullptr;//cur�ĸ��׽��
			Node* cur = _root;
			while (cur)
			{
				parent = cur;
				if (cur->_key < key)//key����cur�ļ�,ȥ������
				{
					cur = cur->_right;
				}
				else if (key < cur->_key)//keyС��cur�ļ�,ȥ������
				{
					cur = cur->_left;
				}
				else
				{
					return false;//�������������������ظ���
				}
			}

			if (key < parent->_key)//keyС��parent�ļ�,�����
			{
				parent->_left = new Node(key);
			}
			else//key����parent�ļ�,���ұ�
			{
				parent->_right = new Node(key);
			}
			return true;
		}

		//���Ҽ�
		bool find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (key < cur->_key)//keyС��cur�ļ�,��������
				{
					cur = cur->_left;
				}
				else if (cur->_key < key)//key����cur�ļ�,��������
				{
					cur = cur->_right;
				}
				else
				{
					return true;
				}
			}
			return false;
		}

		//ɾ����
		bool erase(const K& key)
		{
			Node* parent = nullptr;//cur�ĸ����
			Node* cur = _root;
			int flag = 1;
			while (cur)
			{
				if (key < cur->_key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else
				{
					flag = 0;
					break;
				}
			}

			if (flag)//flagΪ1ʱ,��Ϊû�ڶ������������ҵ�key
			{
				return false;
			}

			if (cur->_left == nullptr)//��ɾ����������Ϊ��
			{
				if (parent == nullptr)
				{
					_root = cur->_right;
				}
				else if (parent->_left == cur)
				{
					parent->_left = cur->_right;
				}
				else
				{
					parent->_right = cur->_right;
				}
				delete cur;
			}
			else if (cur->_right == nullptr)//��ɾ����������Ϊ��
			{
				if (parent == nullptr)
				{
					_root = cur->_left;
				}
				else if (parent->_left == cur)
				{
					parent->_left = cur->_left;
				}
				else
				{
					parent->_right = cur->_left;
				}
				delete cur;
			}
			else
			{
				Node* max_in_left_parent = cur;//cur�����������������ĸ���
				Node* max_in_left = cur->_left;//cur�����������������
				while (max_in_left->_right)
				{
					max_in_left_parent = max_in_left;
					max_in_left = max_in_left->_right;
				}
				//��ʱmax_in_left��������һ��Ϊ��,����������ȷ��

				swap(cur->_key, max_in_left->_key);

				if (max_in_left_parent->_left == max_in_left)
				{
					max_in_left_parent->_left = max_in_left->_left;
				}
				else
				{
					max_in_left_parent->_right = max_in_left->_left;
				}
				delete max_in_left;
			}
			return true;

		}

		//�����---�ݹ��
		bool insertR(const K& key)
		{
			return _insertR(_root, key);
		}

		//���Ҽ�---�ݹ��
		bool findR(const K& key)
		{
			return _findR(_root, key);
		}

		//ɾ����--�ݹ��
		bool eraseR(const K& key)
		{
			return _eraseR(_root, key);
		}


		//ǰ�����
		void prev_order()
		{
			_prev_order(_root);
			cout << endl;
		}

		//�������
		void in_order()
		{
			_in_order(_root);
			cout << endl;
		}

		//�ж��Ƿ�Ϊ����
		bool empty()
		{
			if (_root != nullptr)
			{
				return false;
			}
			return true;
		}

		//��ȡ������
		K& root()
		{
			return _root->_key;
		}

		//��������
		~BST()
		{
			_destroy(_root);
		}
	};

}


namespace KV//��Key��ֵValue����
{
	template<class K,class V>
	struct BST_Node
	{
		K _key;
		V _value;
		BST_Node* _left;
		BST_Node* _right;

		BST_Node(const K& key,const V& value) :
			_key(key), _value(value),_left(nullptr), _right(nullptr) {}

	};


	template <class K,class V>
	class BST
	{
		typedef BST_Node<K,V> Node;

		Node* _root;

		//������������(����)
		void _destroy(Node*& root)
		{
			if (root == nullptr)
			{
				return;
			}

			_destroy(root->_left);
			_destroy(root->_right);
			delete root;
			root = nullptr;
		}

		//�����������캯��(ǰ��)
		Node* _copy(Node* root)
		{
			if (root == nullptr)
			{
				return nullptr;
			}

			Node* new_root = new Node(root->_key,root->_value);
			new_root->_left = _copy(root->_left);
			new_root->_right = _copy(root->_right);

			return new_root;

		}

		//��������Ӻ���
		void _in_order(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			_in_order(root->_left);
			cout << "key:" << root->_key << " value:" << root->_value << endl;
			_in_order(root->_right);
		}

	public:
		//�޲ι��캯��
		BST() = default;//C++11д��---ǿ������Ĭ�Ϲ��캯��

		//�������캯��
		BST(const BST<K,V>& t)
		{
			_root = _copy(t._root);
		}

		//��ֵ�����ִ�д��
		BST<K,V>& operator=(BST<K,V> t)
		{
			swap(_root, t._root);
			return *this;
		}

		//�������ֵ
		bool insert(const K& key,const V& value)
		{
			if (_root == nullptr)//��Ϊ��
			{
				_root = new Node(key,value);
				return true;
			}

			Node* parent = nullptr;//cur�ĸ��׽��
			Node* cur = _root;
			while (cur)
			{
				parent = cur;
				if (cur->_key < key)//key����cur�ļ�ֵ,ȥ������
				{
					cur = cur->_right;
				}
				else if (key < cur->_key)//keyС��cur�ļ�ֵ,ȥ������
				{
					cur = cur->_left;
				}
				else
				{
					return false;//�������������������ظ�ֵ
				}
			}

			if (key < parent->_key)//keyС��parent�ļ�ֵ,�����
			{
				parent->_left = new Node(key,value);
			}
			else//key����parent�ļ�ֵ,���ұ�
			{
				parent->_right = new Node(key,value);
			}
			return true;
		}

		//���Ҽ���ֵ
		Node* find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (key < cur->_key)//keyС��cur�ļ�ֵ,��������
				{
					cur = cur->_left;
				}
				else if (cur->_key < key)//key����cur�ļ�ֵ,��������
				{
					cur = cur->_right;
				}
				else
				{
					return cur;
				}
			}
			return nullptr;
		}

		//ɾ������ֵ
		bool erase(const K& key)
		{
			Node* parent = nullptr;//cur�ĸ����
			Node* cur = _root;
			int flag = 1;
			while (cur)
			{
				if (key < cur->_key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else
				{
					flag = 0;
					break;
				}
			}

			if (flag)//flagΪ1ʱ,��Ϊû�ڶ������������ҵ�key
			{
				return false;
			}

			if (cur->_left == nullptr)//��ɾ��ֵ��������Ϊ��
			{
				if (parent == nullptr)
				{
					_root = cur->_right;
				}
				else if (parent->_left == cur)
				{
					parent->_left = cur->_right;
				}
				else
				{
					parent->_right = cur->_right;
				}
				delete cur;
			}
			else if (cur->_right == nullptr)//��ɾ��ֵ��������Ϊ��
			{
				if (parent == nullptr)
				{
					_root = cur->_left;
				}
				else if (parent->_left == cur)
				{
					parent->_left = cur->_left;
				}
				else
				{
					parent->_right = cur->_left;
				}
				delete cur;
			}
			else
			{
				Node* max_in_left_parent = cur;//cur�������������ֵ�ĸ���
				Node* max_in_left = cur->_left;//cur�������������ֵ
				while (max_in_left->_right)
				{
					max_in_left_parent = max_in_left;
					max_in_left = max_in_left->_right;
				}
				//��ʱmax_in_left��������һ��Ϊ��,����������ȷ��

				swap(cur->_key, max_in_left->_key);

				if (max_in_left_parent->_left == max_in_left)
				{
					max_in_left_parent->_left = max_in_left->_left;
				}
				else
				{
					max_in_left_parent->_right = max_in_left->_left;
				}
				delete max_in_left;
			}
			return true;

		}

		//�ж��Ƿ�Ϊ����
		bool empty()
		{
			if (_root != nullptr)
			{
				return false;
			}
			return true;
		}

		//��ȡ�������
		Node* root()
		{
			return _root;
		}

		//�������
		void in_order()
		{
			_in_order(_root);
			cout << endl;
		}

		//��������
		~BST()
		{
			_destroy(_root);
		}
	};
}


