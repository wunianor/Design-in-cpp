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
		int _front = 0;//ͷָ��
		int _rear = 0;//βָ��,ָ�����һ��Ԫ�ص���һ��λ��
		size_t _size = 0;//��С

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

		//���ض���
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

		//���ض�β
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


		//�Ƿ�Ϊ��
		bool empty() const 
		{
			return _size == 0;
		}

		//�Ƿ�Ϊ��
		bool full() const 
		{
			return _size == max_size;
		}

		//���ش�С
		size_t size() const
		{
			return _size;
		}
	};

}