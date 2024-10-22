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


