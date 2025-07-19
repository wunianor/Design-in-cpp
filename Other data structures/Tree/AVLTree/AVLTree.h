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
	int _bf;   //Balance factor(ƽ������)=�������߶�-�������߶�

	AVLTreeNode(const pair<K, V>& kv) :
		_kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0){}

};



template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;

	Node* _root = nullptr;

	//����
	void RotateL(Node* parent)
	{
		Node* parentParent = parent->_parent;
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		//����parent��subRL
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		//����subR��parent
		subR->_left = parent;
		parent->_parent = subR;


		//����parentParent��subR
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

		//����ƽ������
		subR->_bf = parent->_bf = 0;
	}

	//�ҵ���
	void RotateR(Node* parent)
	{
		Node* parentParent = parent->_parent;
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		//����parent��subLR
		parent->_left = subLR;
		if(subLR)
			subLR->_parent = parent;

		//����subL��parent
		subL->_right = parent;
		parent->_parent = subL;

		//����parentParent��subL
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

		//����ƽ������
		subL->_bf = parent->_bf = 0;
	}

	//����˫��
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int subRL_bf = subRL->_bf;//��תǰsubRL��ƽ������

		RotateR(parent->_right);//�ҵ���
		RotateL(parent);//����

		//ͨ����תǰ��subRL��ƽ���������ж���ת���parent,subR,subRL��ƽ������
		if (subRL_bf == 0)//subRL�������²�����
		{
			parent->_bf = subR->_bf = 0;
		}
		else if (subRL_bf == -1)//subRL������������
		{
			subRL->_bf = 0;
			parent->_bf = 0;
			subR->_bf = 1;
		}
		else if (subRL_bf == 1)//subRL������������
		{
			subRL->_bf = 0;
			parent->_bf = -1;
			subR->_bf = 0;
		}
		else //AVL����ʵ�ַ�ʽ������
		{
			assert(false);
		}
	}

	//����˫��
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int subLR_bf = subLR->_bf;//��תǰsubLR��ƽ������

		RotateL(subL);//����
		RotateR(parent);//�ҵ���

		//ͨ����תǰ��subLR��ƽ���������ж���ת���parent,subL,subLR��ƽ������
		if (subLR_bf == 0)//subLR����Ϊ����
		{
			parent->_bf = subL->_bf = 0;
		}
		else if (subLR_bf == -1)//subLR������Ϊ����
		{
			subLR->_bf = 0;
			subL->_bf = 0;
			parent->_bf = 1;
		}
		else if (subLR_bf == 1)//subLR������Ϊ����
		{
			subLR->_bf = 0;
			subL->_bf = -1;
			parent->_bf = 0;
		}
		else //AVL����ʵ�ַ�ʽ������
		{
			assert(false);
		}

	}

	//ǰ������Ӻ���
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

	//��������Ӻ���
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

	//is_balance�Ӻ���,�ж��Ƿ��Ǻϸ��AVL�������ظ߶�
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
			cout <<"��"<<ret_second<<"�� "<< root->_kv.first << ":" << root->_kv.second << "ƽ�������쳣->" <<root->_bf<< endl;
		}

		return make_pair(ret_first, ret_second);
	}

public:
	AVLTree():
		_root(nullptr){}

	//����
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


		//����ƽ�����Ӳ��ж�Ҫ��Ҫ��ת
		while (parent)
		{
			if (cur == parent->_left)//���curΪparent������
			{
				--parent->_bf;
			}
			else
			{
				++parent->_bf;
			}

			if (parent->_bf == 0)//��parentΪ�������߶Ȳ���,����Ӱ����һ���ƽ������
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)//��parentΪ�������߶ȷ����仯,��Ӱ����һ���ƽ������
			{
				cur = parent;
				parent = cur->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				//��ת��Ŀ��:
				//	��.������ƽ��
				//	��.���������߶�

				if (parent->_bf == 2 && cur->_bf == 1)//�½ڵ����ϸ����������Ҳ�---������
				{
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == -1)//�½ڵ����ϸ����������---���ҵ���
				{
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)//�½ڵ����ϸ������������---��������
				{
					RotateRL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1)//�½ڵ����ϸ����������Ҳ�---������˫��
				{
					RotateLR(parent);
				}

				//��ת�������߶��ѽ���,������ϲ����bf(ƽ������)����Ӱ��
				break;
			}
			else //AVL����ʵ�ַ�ʽ������
			{
				assert(false);
			}
		}

		return true;
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

	//��ȡ���ĸ߶�
	int height()
	{
		return _is_balance(_root).second;
	}

	//���AVL����ʵ���Ƿ���ȷ
	bool is_balance()
	{
		return _is_balance(_root).first;
	}
	
};