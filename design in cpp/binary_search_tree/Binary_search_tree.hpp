#include <iostream>
using namespace std;


namespace K//只有键Key,没有值Value
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


		//前序遍历子函数
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

		//中序遍历子函数
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


		//查找键---递归版子函数
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

		//插入键---递归版子函数
		bool _insertR(Node*& root, const K& key)//注意&的作用,分类讨论
		{
			if (root == nullptr)
			{
				//root是上层调用的别名,对root的改变会影响上层
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

		//删除键---递归版子函数
		bool _eraseR(Node*& root, const K& key)//root为上层的_left或_right的别名
		{
			if (root == nullptr)//没找到键为key的结点
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

			if (root->_left == nullptr)//待删除结点的左子树为空
			{
				root = root->_right;
				delete del;
			}
			else if (root->_right == nullptr)//待删除结点的右子树为空
			{
				root = root->_left;
				delete del;
			}
			else
			{
				//寻找右子树的最小键结点(右子树的最小键结点保证最多只有一个子结点)
				Node* min_in_right = root->_right;
				while (min_in_right->_left)
				{
					min_in_right = min_in_right->_left;
				}

				//交换键
				swap(root->_key, min_in_right->_key);

				//去右子树删除键为key的结点
				return _eraseR(root->_right, key);
			}
			return true;

		}

		//辅助析构函数(后序)
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

		//辅助拷贝构造函数(前序)
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
		//无参构造函数
		BST() = default;//C++11写法---强制生成默认构造函数

		//拷贝构造函数
		BST(const BST<K>& t)
		{
			_root = _copy(t._root);
		}

		//赋值重载现代写法
		BST<K>& operator=(BST<K> t)
		{
			swap(_root, t._root);
			return *this;
		}

		//插入键
		bool insert(const K& key)
		{
			if (_root == nullptr)//树为空
			{
				_root = new Node(key);
				return true;
			}

			Node* parent = nullptr;//cur的父亲结点
			Node* cur = _root;
			while (cur)
			{
				parent = cur;
				if (cur->_key < key)//key大于cur的键,去右子树
				{
					cur = cur->_right;
				}
				else if (key < cur->_key)//key小于cur的键,去左子树
				{
					cur = cur->_left;
				}
				else
				{
					return false;//二叉搜索树不允许有重复键
				}
			}

			if (key < parent->_key)//key小于parent的键,接左边
			{
				parent->_left = new Node(key);
			}
			else//key大于parent的键,接右边
			{
				parent->_right = new Node(key);
			}
			return true;
		}

		//查找键
		bool find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (key < cur->_key)//key小于cur的键,走左子树
				{
					cur = cur->_left;
				}
				else if (cur->_key < key)//key大于cur的键,走右子树
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

		//删除键
		bool erase(const K& key)
		{
			Node* parent = nullptr;//cur的父结点
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

			if (flag)//flag为1时,即为没在二叉搜索树中找到key
			{
				return false;
			}

			if (cur->_left == nullptr)//待删键的左子树为空
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
			else if (cur->_right == nullptr)//待删键的右子树为空
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
				Node* max_in_left_parent = cur;//cur的左子树的最大键结点的父亲
				Node* max_in_left = cur->_left;//cur的左子树的最大键结点
				while (max_in_left->_right)
				{
					max_in_left_parent = max_in_left;
					max_in_left = max_in_left->_right;
				}
				//此时max_in_left的右子树一定为空,但左子树不确定

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

		//插入键---递归版
		bool insertR(const K& key)
		{
			return _insertR(_root, key);
		}

		//查找键---递归版
		bool findR(const K& key)
		{
			return _findR(_root, key);
		}

		//删除键--递归版
		bool eraseR(const K& key)
		{
			return _eraseR(_root, key);
		}


		//前序遍历
		void prev_order()
		{
			_prev_order(_root);
			cout << endl;
		}

		//中序遍历
		void in_order()
		{
			_in_order(_root);
			cout << endl;
		}

		//判断是否为空树
		bool empty()
		{
			if (_root != nullptr)
			{
				return false;
			}
			return true;
		}

		//获取树顶键
		K& root()
		{
			return _root->_key;
		}

		//析构函数
		~BST()
		{
			_destroy(_root);
		}
	};

}


namespace KV//键Key和值Value都有
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

		//辅助析构函数(后序)
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

		//辅助拷贝构造函数(前序)
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

		//中序遍历子函数
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
		//无参构造函数
		BST() = default;//C++11写法---强制生成默认构造函数

		//拷贝构造函数
		BST(const BST<K,V>& t)
		{
			_root = _copy(t._root);
		}

		//赋值重载现代写法
		BST<K,V>& operator=(BST<K,V> t)
		{
			swap(_root, t._root);
			return *this;
		}

		//插入键和值
		bool insert(const K& key,const V& value)
		{
			if (_root == nullptr)//树为空
			{
				_root = new Node(key,value);
				return true;
			}

			Node* parent = nullptr;//cur的父亲结点
			Node* cur = _root;
			while (cur)
			{
				parent = cur;
				if (cur->_key < key)//key大于cur的键值,去右子树
				{
					cur = cur->_right;
				}
				else if (key < cur->_key)//key小于cur的键值,去左子树
				{
					cur = cur->_left;
				}
				else
				{
					return false;//二叉搜索树不允许有重复值
				}
			}

			if (key < parent->_key)//key小于parent的键值,接左边
			{
				parent->_left = new Node(key,value);
			}
			else//key大于parent的键值,接右边
			{
				parent->_right = new Node(key,value);
			}
			return true;
		}

		//查找键和值
		Node* find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (key < cur->_key)//key小于cur的键值,走左子树
				{
					cur = cur->_left;
				}
				else if (cur->_key < key)//key大于cur的键值,走右子树
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

		//删除键和值
		bool erase(const K& key)
		{
			Node* parent = nullptr;//cur的父结点
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

			if (flag)//flag为1时,即为没在二叉搜索树中找到key
			{
				return false;
			}

			if (cur->_left == nullptr)//待删键值的左子树为空
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
			else if (cur->_right == nullptr)//待删键值的右子树为空
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
				Node* max_in_left_parent = cur;//cur的左子树的最大值的父亲
				Node* max_in_left = cur->_left;//cur的左子树的最大值
				while (max_in_left->_right)
				{
					max_in_left_parent = max_in_left;
					max_in_left = max_in_left->_right;
				}
				//此时max_in_left的右子树一定为空,但左子树不确定

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

		//判断是否为空树
		bool empty()
		{
			if (_root != nullptr)
			{
				return false;
			}
			return true;
		}

		//获取树顶结点
		Node* root()
		{
			return _root;
		}

		//中序遍历
		void in_order()
		{
			_in_order(_root);
			cout << endl;
		}

		//析构函数
		~BST()
		{
			_destroy(_root);
		}
	};
}


