#pragma once

#include <iostream>
#include <deque>
using namespace std;

namespace QUEUE
{
	template<class T,class Container=deque<T>>
	class Queue
	{
		Container _con;

	public:
		void push(const T& val) 
		{
			_con.push_back(val);
		}

		void pop()
		{
			_con.pop_front();
		}

		T& front()
		{
			return _con.front();
		}
		const T& front() const
		{
			return _con.front();
		}

		T& back()
		{
			return _con.back();
		}
		const T& back() const
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

};