#pragma once

#include <utility>
#include <assert.h>

/// <summary>
/// 结点颜色枚举类型
/// </summary>
enum Color
{
	/// <summary>
	/// 红色
	/// </summary>
	RED,

	/// <summary>
	/// 黑色
	/// </summary>
	BLACK
};


/// <summary>
/// 红黑树结点类模板
/// </summary>
/// <typeparam name="T">数据的类型</typeparam>
/// <typeparam name="K">键值</typeparam>
template<class K,class T>
struct RBTreeNode
{
	/// <summary>
	/// 父亲结点
	/// </summary>
	RBTreeNode<K, T>* _parent; 

	/// <summary>
	/// 左孩子
	/// </summary>
	RBTreeNode<K, T>* _left;   

	/// <summary>
	/// 右孩子
	/// </summary>
	RBTreeNode<K, T>* _right; 
	
	/// <summary>
	/// 结点的数据
	/// </summary>
	T _data;            

	/// <summary>
	/// 结点颜色
	/// </summary>
	Color _color;              

	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="data">用于初始化结点的数据</param>
	RBTreeNode(const T& data):
		_data(data),
		_parent(nullptr),
		_left(nullptr),
		_right(nullptr),
		_color(RED)
	{}
};



template<class K,class T,class Ref,class Ptr>
struct __RBTree_Iterator
{
	typedef RBTreeNode<K, T> Node;
	typedef __RBTree_Iterator<K, T, Ref, Ptr> Self;
	
	/// <summary>
	/// 红黑树结点指针
	/// </summary>
	Node* _node;

	/// <summary>
	/// 迭代器所在树的根节点
	/// </summary>
	Node* _root;


	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="node">用于初始化迭代器的红黑树结点指针</param>
	__RBTree_Iterator(Node* const node,Node* const root): //const Node* 和 Node* const 不一样
		_node(node),
		_root(root)
	{}

	/// <summary>
	/// 寻找以root为根的树的键最大的结点
	/// </summary>
	/// <param name="root">树的根</param>
	/// <returns>返回树的键最大的结点()</returns>
	Node* max_node(Node* root)
	{
		Node* maxNodeInRoot = root;
		while (maxNodeInRoot && maxNodeInRoot->_right)
		{
			maxNodeInRoot = maxNodeInRoot->_right;
		}

		return maxNodeInRoot;
	}

	/// <summary>
	/// 寻找以root为根的树的键最小的结点
	/// </summary>
	/// <param name="root">树的根</param>
	/// <returns>返回树的键最小的结点</returns>
	Node* min_node(Node* root)
	{
		Node* minNodeInRoot = root;
		while (minNodeInRoot && minNodeInRoot->_left)
		{
			minNodeInRoot = minNodeInRoot->_left;
		}

		return minNodeInRoot;
	}


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
		return &_node->_data;
	}

	/// <summary>
	/// 重载!=
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator!=(const Self& other) const
	{
		return _node != other._node;
	}

	/// <summary>
	/// 重载==
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator==(const Self& other) const
	{
		return !(*this != other);
	}

	/// <summary>
	/// 重载前置++
	/// </summary>
	/// <returns>返回++后的迭代器</returns>
	Self& operator++()
	{
		
		if (_node->_right)
		{
			//如果当前结点的右子树不为空,就去找右子树的最左结点
			_node = min_node(_node->_right);
		}
		else 
		{ 
			//如果当前结点的右子树为空,就去找第一个大于该结点的祖先结点

			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && parent->_right == cur)
			{
				cur = parent;
				parent = cur->_parent;
			}

			_node = parent;
		}

		return *this;
	}

	/// <summary>
	/// 重载后置++
	/// </summary>
	/// <param name=""></param>
	/// <returns>返回++前的迭代器</returns>
	Self operator++(int)
	{
		Self tmp = *this;
	
		++*this;

		return tmp;
	}

	/// <summary>
	/// 重载前置--
	/// </summary>
	/// <returns>返回--后的迭代器</returns>
	Self& operator--()
	{
		if(_node == nullptr)
		{
			//如果是指向end()的迭代器,则去找整棵树的最右结点
			_node = max_node(_root);
		}
		else if (_node->_left)
		{
			//cur的左子树存在,则去找cur左子树的最右结点
			_node = max_node(_node->_left);
		}
		else
		{	
			//cur的左子树不存在,则去cur的祖父结点中按从下往上顺序找第一个键小于cur的祖先结点
			
			Node* cur = _node;
			Node* parent = cur->_parent;

			while (parent && parent->_left == cur)
			{
				cur = parent;
				parent = cur->_parent;
			}

			_node = parent;
		}

		return *this;
	}

	/// <summary>
	/// 重载后置--
	/// </summary>
	/// <param name=""></param>
	/// <returns>返回--前的迭代器</returns>
	Self operator--(int)
	{
		Self tmp = *this;

		--*this;

		return tmp;
	}

};


/// <summary>
/// 红黑树类模板
/// </summary>
/// <typeparam name="K">键值</typeparam>
/// <typeparam name="T">数据的类型</typeparam>
/// <typeparam name="KeyOfT">仿函数---取T类型数据的Key</typeparam>
template<class K,class T,class KeyOfT>
class RBTree
{
	typedef RBTreeNode<K, T> Node;

public:

	/// <summary>
	/// 红黑树普通迭代器
	/// </summary>
	typedef __RBTree_Iterator<K, T, T&, T*> Iterator; 

	/// <summary>
	/// 红黑树const迭代器
	/// </summary>
	typedef __RBTree_Iterator<K, T, const T&, const T*> ConstIterator;


	/// <summary>
	/// 无参构造函数
	/// </summary>
	RBTree():
		_root(nullptr),
		_size(0)
	{}

	/// <summary>
	/// 插入数据
	/// </summary>
	/// <param name="data">待插入的数据</param>
	/// <returns>
	/// 插入成功,返回{新插入数据的迭代器,true};
	/// 插入失败,返回{已经存在的键的迭代器,false}
	/// </returns>
	pair<Node*,bool> insert(const T& data)
	{
		pair<Node*,bool> insertResult = _insert(data);

		//根据插入成功/失败情况,更新树的大小
		_size += insertResult.second;

		return insertResult;
	}



	/// <summary>
	/// 返回树的大小
	/// </summary>
	/// <returns>返回树的大小</returns>
	size_t size()
	{
		return _size;
	}

	/// <summary>
	/// 返回树的高度
	/// </summary>
	/// <returns>返回树的高度</returns>
	size_t height()
	{
		return _height(_root);
	}



	/// <summary>
	/// 返回begin迭代器
	/// </summary>
	/// <returns>返回begin迭代器</returns>
	Iterator begin()
	{

		//找到树中的最左结点
		Node* cur = _root;

		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		//构造迭代器并返回
		return Iterator(cur, _root);
	}

	/// <summary>
	/// 返回const begin迭代器
	/// </summary>
	/// <returns>返回const begin迭代器</returns>
	ConstIterator begin() const
	{
		//找到树中的最左结点
		Node* cur = _root;

		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		//构造迭代器并返回
		return ConstIterator(cur, _root);
	}

	/// <summary>
	/// 返回end迭代器
	/// </summary>
	/// <returns>返回end迭代器</returns>
	Iterator end()
	{
		return Iterator(nullptr, _root);
	}

	/// <summary>
	/// 返回const end迭代器
	/// </summary>
	/// <returns>返回const end迭代器</returns>
	ConstIterator end() const
	{
		return ConstIterator(nullptr, _root);
	}



	/// <summary>
	/// 返回树根的指针
	/// </summary>
	/// <returns></returns>
	Node* getRoot()
	{
		return _root;
	}

	/// <summary>
	/// 清空红黑树
	/// </summary>
	void clear()
	{
		//如果大小为0,直接返回
		if (_size == 0)
		{
			return;
		}

		_clear(_root);
		_size = 0;
	}

	/// <summary>
	/// 检查红黑树的实现是否正确
	/// </summary>
	/// <returns>正确返回true,错误返回false</returns>
	bool check()
	{
		//如果根存在并且颜色为红色
		if (_root && _root->_color == RED)
		{
			return false;
		}

		int BlackCount = -1;//一条路径上黑色结点的个数
		return _check(_root, BlackCount, 0);
	}



	/// <summary>
	/// 析构函数
	/// </summary>
	~RBTree()
	{
		clear();
	}

private:
	/// <summary>
	/// 树的根节点
	/// </summary>
	Node* _root = nullptr;

	/// <summary>
	/// 树的大小
	/// </summary>
	size_t _size = 0;



	/// <summary>
	/// 左单旋
	/// </summary>
	/// <param name="parent"></param>
	void RotateL(Node* parent)
	{
		Node* parentParent = parent->_parent;
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		//处理parent和subRL
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		//处理subR和parent
		subR->_left = parent;
		parent->_parent = subR;


		//处理parentParent和subR
		subR->_parent = parentParent;
		if (parentParent)
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subR;
			}
			else
			{
				parentParent->_right = subR;
			}
		}
		else
		{
			_root = subR;
		}


	}

	/// <summary>
	/// 右单旋
	/// </summary>
	/// <param name="parent"></param>
	void RotateR(Node* parent)
	{
		Node* parentParent = parent->_parent;
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		//处理parent和subLR
		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		//处理subL和parent
		subL->_right = parent;
		parent->_parent = subL;

		//处理parentParent和subL
		subL->_parent = parentParent;
		if (parentParent)
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subL;
			}
			else
			{
				parentParent->_right = subL;
			}
		}
		else
		{
			_root = subL;
		}


	}

	/// <summary>
	/// 右左双旋
	/// </summary>
	/// <param name="parent"></param>
	void RotateRL(Node* parent)
	{

		RotateR(parent->_right);//右单旋
		RotateL(parent);//左单旋

	}

	/// <summary>
	/// 左右双旋
	/// </summary>
	/// <param name="parent"></param>
	void RotateLR(Node* parent)
	{
		RotateL(parent->_left);//左单旋
		RotateR(parent);//右单旋
	}



	/// <summary>
	/// insert()子函数---向树中插入data
	/// </summary>
	/// <param name="data">待插入的数据</param>
	/// <returns>
	/// 插入成功,返回{新插入数据的迭代器,true};
	/// 插入失败,返回{已经存在的键的迭代器,false}
	/// </returns>
	pair<Node*, bool> _insert(const T& data)
	{
		//如果根结点为空
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_color = BLACK;

			return { _root,true };
		}

		//寻找能插入data的地方
		Node* parent = _root->_parent;
		Node* cur = _root;
		KeyOfT keyOfT;

		while (cur)
		{
			if (keyOfT(data) < keyOfT(cur->_data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (keyOfT(cur->_data) < keyOfT(data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else  //如果data中的键已在树中存在,则插入失败
			{
				return { cur, false };
			}
		}

		//插入data
		Node* newNode = new Node(data);
		newNode->_parent = parent;
		if (keyOfT(newNode->_data) < keyOfT(parent->_data))
		{
			parent->_left = newNode;
		}
		else
		{
			parent->_right = newNode;
		}

		cur = newNode;

		//进行颜色检查并修正
		while (parent && parent->_color == RED)
		{
			Node* gParent = parent->_parent; //cur的祖父结点
			Node* uncle = gParent->_left == parent ? gParent->_right : gParent->_left;//cur的叔叔结点


			//如果叔叔存在且颜色为红
			if (uncle && uncle->_color == RED)
			{
				parent->_color = BLACK;
				uncle->_color = BLACK;
				gParent->_color = RED;

				cur = gParent;
				parent = cur->_parent;
			}
			else //如果叔叔不存在 or 颜色为黑
			{

				if (gParent->_left == parent && parent->_left == cur)
				{
					//如果是下图这种情况
					//         gParent
					//    parent      uncle
					// cur
					//

					//右单旋
					RotateR(gParent);

					//旋转后变成下图
					//       parent
					//   cur        gParent
					//                   uncle
					//

					//更新颜色
					gParent->_color = RED;
					parent->_color = BLACK;
				}
				else if (gParent->_left == parent && parent->_right == cur)
				{
					//如果是下图这种情况
					//             gParent
					//    parent            uncle
					//          cur
					//

					//左右双旋
					RotateLR(gParent);

					//旋转后变成下图
					//          cur
					//   parent     gParent
					//                      uncle
					//

					//更新颜色
					gParent->_color = RED;
					cur->_color = BLACK;
				}
				else if (gParent->_right == parent && parent->_left == cur)
				{
					//如果是下图这种情况
					//         gParent
					//  uncle               parent
					//                 cur
					//

					//右左双旋
					RotateRL(gParent);

					//旋转后变成下图
					//            cur
					//     gParent    parent
					//uncle

					gParent->_color = RED;
					cur->_color = BLACK;
				}
				else
				{
					//如果是下图这种情况
					//         gParent
					//  uncle         parent
					//                      cur
					//

					//左单旋
					RotateL(gParent);

					//旋转后变成下图
					//            parent
					//     gParent     cur
					//uncle

					//更新颜色
					gParent->_color = RED;
					parent->_color = BLACK;
				}
				return { newNode,true };
			}
		}

		_root->_color = BLACK;

		return { newNode,true };
	}



	/// <summary>
	/// check()子函数---检查树是否符合红黑树的规则
	/// </summary>
	/// <param name="root">树的根结点</param>
	/// <param name="BlackCount">树上一条路径上的黑色结点的个数</param>
	/// <param name="curBlackCount">当前路径已经计算的黑色结点的个数</param>
	/// <returns>树的实现符合红黑树的规则,返回true;否则返回false</returns>
	bool _check(Node* root, int& BlackCount, int curBlackCount)
	{
		if (root == nullptr)
		{
			if (BlackCount == -1)
			{
				BlackCount = curBlackCount;
			}
			else if (curBlackCount != BlackCount)
			{
				return false;
			}

			return true;
		}

		if (root->_color == RED)
		{
			if (root->_left && root->_left->_color == RED) return false;
			if (root->_right && root->_right->_color == RED) return false;
		}
		else ++curBlackCount;

		return _check(root->_left, BlackCount, curBlackCount) &&
			_check(root->_right, BlackCount, curBlackCount);

	}

	/// <summary>
	/// height()子函数---求树的高度
	/// </summary>
	/// <param name="root">树的根结点</param>
	/// <returns>返回树的高度</returns>
	size_t _height(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		size_t leftHeight = _height(root->_left);   //左子树高度
		size_t rightHeight = _height(root->_right); //右子树高度

		return max(leftHeight, rightHeight) + 1;
	}

	/// <summary>
	/// clear()子函数---采用后序遍历的方式,delete掉所有结点
	/// </summary>
	/// <param name="root">待清空的树的根</param>
	void _clear(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_clear(root->_left);
		_clear(root->_right);

		delete root;
		root = nullptr;
	}
};

