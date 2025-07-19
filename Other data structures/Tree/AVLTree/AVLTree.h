#pragma once
#include <utility>
#include <assert.h>
using namespace std;

template<class K,class V>
struct AVLTreeNode
{
	AVLTreeNode* _left;
	AVLTreeNode* _right;
	AVLTreeNode* _parent;
	pair<K, V> _kv;
	int _bf;   //Balance factor(平衡因子)=右子树高度-左子树高度

	AVLTreeNode(const pair<K, V>& kv) :
		_kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0){}

};



template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;

	Node* _root = nullptr;

	//左单旋
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

		//处理平衡因子
		subR->_bf = parent->_bf = 0;
	}

	//右单旋
	void RotateR(Node* parent)
	{
		Node* parentParent = parent->_parent;
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		//处理parent和subLR
		parent->_left = subLR;
		if(subLR)
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

		//处理平衡因子
		subL->_bf = parent->_bf = 0;
	}

	//右左双旋
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int subRL_bf = subRL->_bf;//旋转前subRL的平衡因子

		RotateR(parent->_right);//右单旋
		RotateL(parent);//左单旋

		//通过旋转前的subRL的平衡因子来判断旋转后的parent,subR,subRL的平衡因子
		if (subRL_bf == 0)//subRL本身是新插入结点
		{
			parent->_bf = subR->_bf = 0;
		}
		else if (subRL_bf == -1)//subRL的左子树新增
		{
			subRL->_bf = 0;
			parent->_bf = 0;
			subR->_bf = 1;
		}
		else if (subRL_bf == 1)//subRL的右子树新增
		{
			subRL->_bf = 0;
			parent->_bf = -1;
			subR->_bf = 0;
		}
		else //AVL树的实现方式有问题
		{
			assert(false);
		}
	}

	//左右双旋
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int subLR_bf = subLR->_bf;//旋转前subLR的平衡因子

		RotateL(subL);//左单旋
		RotateR(parent);//右单旋

		//通过旋转前的subLR的平衡因子来判断旋转后的parent,subL,subLR的平衡因子
		if (subLR_bf == 0)//subLR本身为新增
		{
			parent->_bf = subL->_bf = 0;
		}
		else if (subLR_bf == -1)//subLR左子树为新增
		{
			subLR->_bf = 0;
			subL->_bf = 0;
			parent->_bf = 1;
		}
		else if (subLR_bf == 1)//subLR右子树为新增
		{
			subLR->_bf = 0;
			subL->_bf = -1;
			parent->_bf = 0;
		}
		else //AVL树的实现方式有问题
		{
			assert(false);
		}

	}

	//前序遍历子函数
	void _prev_order(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		cout << root->_kv.first << ":" << root->_kv.second << "    ";
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
		cout << root->_kv.first << ":" << root->_kv.second << "    ";
		_in_order(root->_right);
	}

	//is_balance子函数,判断是否是合格的AVL树并返回高度
	pair<bool,int> _is_balance(Node* root)
	{
		if (root == nullptr)
		{
			return make_pair(true,0);
		}

		pair<bool,int> left = _is_balance(root->_left);
		pair<bool,int> right = _is_balance(root->_right);
		
		bool ret_first = abs(right.second - left.second) <= 1 && left.first && right.first;
		int ret_second = max(left.second, right.second) + 1;

		if (abs(root->_bf) > 1)
		{
			cout <<"第"<<ret_second<<"层 "<< root->_kv.first << ":" << root->_kv.second << "平衡因子异常->" <<root->_bf<< endl;
		}

		return make_pair(ret_first, ret_second);
	}

public:
	AVLTree():
		_root(nullptr){}

	//插入
	bool insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;

		while (cur)
		{
			if (kv.first <(cur->_kv).first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if ((cur->_kv).first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(kv);
		if (kv.first < (parent->_kv).first)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}


		//调整平衡因子并判断要不要旋转
		while (parent)
		{
			if (cur == parent->_left)//如果cur为parent的左孩子
			{
				--parent->_bf;
			}
			else
			{
				++parent->_bf;
			}

			if (parent->_bf == 0)//以parent为根的树高度不变,不会影响上一层的平衡因子
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)//以parent为根的树高度发生变化,会影响上一层的平衡因子
			{
				cur = parent;
				parent = cur->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				//旋转的目的:
				//	①.让子树平衡
				//	②.降低子树高度

				if (parent->_bf == 2 && cur->_bf == 1)//新节点插入较高右子树的右侧---走左单旋
				{
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == -1)//新节点插入较高左子树左侧---走右单旋
				{
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)//新节点插入较高右子树的左侧---走右左单旋
				{
					RotateRL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1)//新节点插入较高左子树的右侧---走左右双旋
				{
					RotateLR(parent);
				}

				//旋转后子树高度已降低,不会对上层结点的bf(平衡因子)产生影响
				break;
			}
			else //AVL树的实现方式有问题
			{
				assert(false);
			}
		}

		return true;
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

	//获取树的高度
	int height()
	{
		return _is_balance(_root).second;
	}

	//检查AVL树的实现是否正确
	bool is_balance()
	{
		return _is_balance(_root).first;
	}
	
};