#pragma once

#include <utility>
#include <assert.h>

/// <summary>
/// �����ɫö������
/// </summary>
enum Color
{
	/// <summary>
	/// ��ɫ
	/// </summary>
	RED,

	/// <summary>
	/// ��ɫ
	/// </summary>
	BLACK
};


/// <summary>
/// ����������ģ��
/// </summary>
/// <typeparam name="T">���ݵ�����</typeparam>
/// <typeparam name="K">��ֵ</typeparam>
template<class K,class T>
struct RBTreeNode
{
	/// <summary>
	/// ���׽��
	/// </summary>
	RBTreeNode<K, T>* _parent; 

	/// <summary>
	/// ����
	/// </summary>
	RBTreeNode<K, T>* _left;   

	/// <summary>
	/// �Һ���
	/// </summary>
	RBTreeNode<K, T>* _right; 
	
	/// <summary>
	/// ��������
	/// </summary>
	T _data;            

	/// <summary>
	/// �����ɫ
	/// </summary>
	Color _color;              

	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="data">���ڳ�ʼ����������</param>
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
	/// ��������ָ��
	/// </summary>
	Node* _node;

	/// <summary>
	/// �������������ĸ��ڵ�
	/// </summary>
	Node* _root;


	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="node">���ڳ�ʼ���������ĺ�������ָ��</param>
	__RBTree_Iterator(Node* const node,Node* const root): //const Node* �� Node* const ��һ��
		_node(node),
		_root(root)
	{}

	/// <summary>
	/// Ѱ����rootΪ�������ļ����Ľ��
	/// </summary>
	/// <param name="root">���ĸ�</param>
	/// <returns>�������ļ����Ľ��()</returns>
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
	/// Ѱ����rootΪ�������ļ���С�Ľ��
	/// </summary>
	/// <param name="root">���ĸ�</param>
	/// <returns>�������ļ���С�Ľ��</returns>
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
		return &_node->_data;
	}

	/// <summary>
	/// ����!=
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator!=(const Self& other) const
	{
		return _node != other._node;
	}

	/// <summary>
	/// ����==
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator==(const Self& other) const
	{
		return !(*this != other);
	}

	/// <summary>
	/// ����ǰ��++
	/// </summary>
	/// <returns>����++��ĵ�����</returns>
	Self& operator++()
	{
		
		if (_node->_right)
		{
			//�����ǰ������������Ϊ��,��ȥ����������������
			_node = min_node(_node->_right);
		}
		else 
		{ 
			//�����ǰ����������Ϊ��,��ȥ�ҵ�һ�����ڸý������Ƚ��

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
	/// ���غ���++
	/// </summary>
	/// <param name=""></param>
	/// <returns>����++ǰ�ĵ�����</returns>
	Self operator++(int)
	{
		Self tmp = *this;
	
		++*this;

		return tmp;
	}

	/// <summary>
	/// ����ǰ��--
	/// </summary>
	/// <returns>����--��ĵ�����</returns>
	Self& operator--()
	{
		if(_node == nullptr)
		{
			//�����ָ��end()�ĵ�����,��ȥ�������������ҽ��
			_node = max_node(_root);
		}
		else if (_node->_left)
		{
			//cur������������,��ȥ��cur�����������ҽ��
			_node = max_node(_node->_left);
		}
		else
		{	
			//cur��������������,��ȥcur���游����а���������˳���ҵ�һ����С��cur�����Ƚ��
			
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
	/// ���غ���--
	/// </summary>
	/// <param name=""></param>
	/// <returns>����--ǰ�ĵ�����</returns>
	Self operator--(int)
	{
		Self tmp = *this;

		--*this;

		return tmp;
	}

};


/// <summary>
/// �������ģ��
/// </summary>
/// <typeparam name="K">��ֵ</typeparam>
/// <typeparam name="T">���ݵ�����</typeparam>
/// <typeparam name="KeyOfT">�º���---ȡT�������ݵ�Key</typeparam>
template<class K,class T,class KeyOfT>
class RBTree
{
	typedef RBTreeNode<K, T> Node;

public:

	/// <summary>
	/// �������ͨ������
	/// </summary>
	typedef __RBTree_Iterator<K, T, T&, T*> Iterator; 

	/// <summary>
	/// �����const������
	/// </summary>
	typedef __RBTree_Iterator<K, T, const T&, const T*> ConstIterator;


	/// <summary>
	/// �޲ι��캯��
	/// </summary>
	RBTree():
		_root(nullptr),
		_size(0)
	{}

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="data">�����������</param>
	/// <returns>
	/// ����ɹ�,����{�²������ݵĵ�����,true};
	/// ����ʧ��,����{�Ѿ����ڵļ��ĵ�����,false}
	/// </returns>
	pair<Node*,bool> insert(const T& data)
	{
		pair<Node*,bool> insertResult = _insert(data);

		//���ݲ���ɹ�/ʧ�����,�������Ĵ�С
		_size += insertResult.second;

		return insertResult;
	}



	/// <summary>
	/// �������Ĵ�С
	/// </summary>
	/// <returns>�������Ĵ�С</returns>
	size_t size()
	{
		return _size;
	}

	/// <summary>
	/// �������ĸ߶�
	/// </summary>
	/// <returns>�������ĸ߶�</returns>
	size_t height()
	{
		return _height(_root);
	}



	/// <summary>
	/// ����begin������
	/// </summary>
	/// <returns>����begin������</returns>
	Iterator begin()
	{

		//�ҵ����е�������
		Node* cur = _root;

		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		//���������������
		return Iterator(cur, _root);
	}

	/// <summary>
	/// ����const begin������
	/// </summary>
	/// <returns>����const begin������</returns>
	ConstIterator begin() const
	{
		//�ҵ����е�������
		Node* cur = _root;

		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		//���������������
		return ConstIterator(cur, _root);
	}

	/// <summary>
	/// ����end������
	/// </summary>
	/// <returns>����end������</returns>
	Iterator end()
	{
		return Iterator(nullptr, _root);
	}

	/// <summary>
	/// ����const end������
	/// </summary>
	/// <returns>����const end������</returns>
	ConstIterator end() const
	{
		return ConstIterator(nullptr, _root);
	}



	/// <summary>
	/// ����������ָ��
	/// </summary>
	/// <returns></returns>
	Node* getRoot()
	{
		return _root;
	}

	/// <summary>
	/// ��պ����
	/// </summary>
	void clear()
	{
		//�����СΪ0,ֱ�ӷ���
		if (_size == 0)
		{
			return;
		}

		_clear(_root);
		_size = 0;
	}

	/// <summary>
	/// ���������ʵ���Ƿ���ȷ
	/// </summary>
	/// <returns>��ȷ����true,���󷵻�false</returns>
	bool check()
	{
		//��������ڲ�����ɫΪ��ɫ
		if (_root && _root->_color == RED)
		{
			return false;
		}

		int BlackCount = -1;//һ��·���Ϻ�ɫ���ĸ���
		return _check(_root, BlackCount, 0);
	}



	/// <summary>
	/// ��������
	/// </summary>
	~RBTree()
	{
		clear();
	}

private:
	/// <summary>
	/// ���ĸ��ڵ�
	/// </summary>
	Node* _root = nullptr;

	/// <summary>
	/// ���Ĵ�С
	/// </summary>
	size_t _size = 0;



	/// <summary>
	/// ����
	/// </summary>
	/// <param name="parent"></param>
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


	}

	/// <summary>
	/// �ҵ���
	/// </summary>
	/// <param name="parent"></param>
	void RotateR(Node* parent)
	{
		Node* parentParent = parent->_parent;
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		//����parent��subLR
		parent->_left = subLR;
		if (subLR)
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


	}

	/// <summary>
	/// ����˫��
	/// </summary>
	/// <param name="parent"></param>
	void RotateRL(Node* parent)
	{

		RotateR(parent->_right);//�ҵ���
		RotateL(parent);//����

	}

	/// <summary>
	/// ����˫��
	/// </summary>
	/// <param name="parent"></param>
	void RotateLR(Node* parent)
	{
		RotateL(parent->_left);//����
		RotateR(parent);//�ҵ���
	}



	/// <summary>
	/// insert()�Ӻ���---�����в���data
	/// </summary>
	/// <param name="data">�����������</param>
	/// <returns>
	/// ����ɹ�,����{�²������ݵĵ�����,true};
	/// ����ʧ��,����{�Ѿ����ڵļ��ĵ�����,false}
	/// </returns>
	pair<Node*, bool> _insert(const T& data)
	{
		//��������Ϊ��
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_color = BLACK;

			return { _root,true };
		}

		//Ѱ���ܲ���data�ĵط�
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
			else  //���data�еļ��������д���,�����ʧ��
			{
				return { cur, false };
			}
		}

		//����data
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

		//������ɫ��鲢����
		while (parent && parent->_color == RED)
		{
			Node* gParent = parent->_parent; //cur���游���
			Node* uncle = gParent->_left == parent ? gParent->_right : gParent->_left;//cur��������


			//��������������ɫΪ��
			if (uncle && uncle->_color == RED)
			{
				parent->_color = BLACK;
				uncle->_color = BLACK;
				gParent->_color = RED;

				cur = gParent;
				parent = cur->_parent;
			}
			else //������岻���� or ��ɫΪ��
			{

				if (gParent->_left == parent && parent->_left == cur)
				{
					//�������ͼ�������
					//         gParent
					//    parent      uncle
					// cur
					//

					//�ҵ���
					RotateR(gParent);

					//��ת������ͼ
					//       parent
					//   cur        gParent
					//                   uncle
					//

					//������ɫ
					gParent->_color = RED;
					parent->_color = BLACK;
				}
				else if (gParent->_left == parent && parent->_right == cur)
				{
					//�������ͼ�������
					//             gParent
					//    parent            uncle
					//          cur
					//

					//����˫��
					RotateLR(gParent);

					//��ת������ͼ
					//          cur
					//   parent     gParent
					//                      uncle
					//

					//������ɫ
					gParent->_color = RED;
					cur->_color = BLACK;
				}
				else if (gParent->_right == parent && parent->_left == cur)
				{
					//�������ͼ�������
					//         gParent
					//  uncle               parent
					//                 cur
					//

					//����˫��
					RotateRL(gParent);

					//��ת������ͼ
					//            cur
					//     gParent    parent
					//uncle

					gParent->_color = RED;
					cur->_color = BLACK;
				}
				else
				{
					//�������ͼ�������
					//         gParent
					//  uncle         parent
					//                      cur
					//

					//����
					RotateL(gParent);

					//��ת������ͼ
					//            parent
					//     gParent     cur
					//uncle

					//������ɫ
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
	/// check()�Ӻ���---������Ƿ���Ϻ�����Ĺ���
	/// </summary>
	/// <param name="root">���ĸ����</param>
	/// <param name="BlackCount">����һ��·���ϵĺ�ɫ���ĸ���</param>
	/// <param name="curBlackCount">��ǰ·���Ѿ�����ĺ�ɫ���ĸ���</param>
	/// <returns>����ʵ�ַ��Ϻ�����Ĺ���,����true;���򷵻�false</returns>
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
	/// height()�Ӻ���---�����ĸ߶�
	/// </summary>
	/// <param name="root">���ĸ����</param>
	/// <returns>�������ĸ߶�</returns>
	size_t _height(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		size_t leftHeight = _height(root->_left);   //�������߶�
		size_t rightHeight = _height(root->_right); //�������߶�

		return max(leftHeight, rightHeight) + 1;
	}

	/// <summary>
	/// clear()�Ӻ���---���ú�������ķ�ʽ,delete�����н��
	/// </summary>
	/// <param name="root">����յ����ĸ�</param>
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

