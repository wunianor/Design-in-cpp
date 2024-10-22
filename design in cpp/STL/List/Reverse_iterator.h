#pragma once



//·´Ïòµü´úÆ÷ÊÊÅäÆ÷
template<class iterator,class Ref,class Ptr>
class Reverse_iterator
{
	iterator _it;

	typedef Reverse_iterator<iterator,Ref,Ptr> self;

public:
	Reverse_iterator(iterator it):
		_it(it){}


	Ref operator*()
	{
		iterator tmp = _it;
		return *(--tmp);
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	self& operator++()
	{
		--_it;
		return *this;
	}
	self operator++(int)
	{
		iterator tmp = _it;
		--_it;
		return tmp;
	}

	self& operator--()
	{
		++_it;
		return *this;
	}
	self operator--(int)
	{
		iterator tmp = _it;
		++_it;
		return tmp;
	}

	bool operator!=(const self& s)
	{
		return _it != s._it;
	}
	bool operator==(const self& s)
	{
		return _it == s._it;
	}


};