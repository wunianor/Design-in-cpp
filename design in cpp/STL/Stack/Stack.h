#pragma once

#include <iostream>

using namespace std;


namespace STACK
{
	template<class T,class Container=deque<T>>
	class Stack
	{
		Container _con;

	public:
		void push(const T& val)
		{
			_con.push_back(val);
		}

		void pop()
		{
			_con.pop_back();
		}

		T& top()
		{
			return _con.back();
		}
		const T& top() const
		{
			return _con.back();
		}

		bool empty() const
		{
			return _con.empty();
		}

		size_t size() const
		{
			return _con.size();
		}


	};

}