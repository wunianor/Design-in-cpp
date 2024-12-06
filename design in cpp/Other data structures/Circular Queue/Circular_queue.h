#pragma once

#include <deque>
#include <assert.h>
using namespace std;

namespace CIRCULAR_QUEUE
{

	template<class T,size_t max_size ,class Contain=deque<T>>
	class c_queue
	{
		Contain _data = Contain(max_size);
		int _front = 0;//头指针
		int _rear = 0;//尾指针,指向最后一个元素的下一个位置
		size_t _size = 0;//大小

	public:

		bool push(const T& val)
		{
			if (full())
			{
				return false;
			}

			_data[_rear] = val;
			_rear = (_rear + 1) % max_size;
			++_size;
			
			return true;
		}

		bool pop()
		{
			if (empty())
			{
				return false;
			}
			
			_front = (_front + 1) % max_size;
			--_size;
			return true;
		}

		//返回队首
		T& front()
		{
			assert(_size > 0);
			return _data[_front];
		}

		const T& front() const
		{
			assert(_size > 0);
			return _data[_front];
		}

		//返回队尾
		T& back()
		{
			assert(_size > 0);
			size_t index_back = _rear == 0 ? max_size - 1 : _rear - 1;
			return _data[index_back];
		}

		const T& back() const 
		{
			assert(_size > 0);
			size_t index_back = _rear == 0 ? max_size - 1 : _rear - 1;
			return _data[index_back];
		}


		//是否为空
		bool empty() const 
		{
			return _size == 0;
		}

		//是否为满
		bool full() const 
		{
			return _size == max_size;
		}

		//返回大小
		size_t size() const
		{
			return _size;
		}
	};

}