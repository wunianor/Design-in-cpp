#pragma once

#include <iostream>
#include <assert.h>
using namespace std;

#include "Reverse_iterator.h"

namespace VECTOR
{
	template<class T>
	class Vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		typedef Reverse_iterator<iterator, T&, T*> reverse_iterator;//反向迭代器
		typedef Reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;//反向const迭代器

		Vector():
			_start(nullptr),
			_finish(nullptr),
			_end_of_storage(nullptr)
		{}
		Vector(size_t n, const T& val=T())
		{
			_start = new T[n];
			_finish = _start;
			_end_of_storage = _start + n;
			while (_finish - _start < n)
			{
				*_finish = val;
				++_finish;
			}
		}
		Vector(int n, const T& val = T())//为了防止传参为(int,int)时,使用下面那个构造函数模板(函数模板与非模板函数匹配使用规则)
		{
			_start = new T[n];
			_finish = _start;
			_end_of_storage = _start + n;
			while (_finish - _start < n)
			{
				*_finish = val;
				++_finish;
			}
		}
		template <class InputIterator>
		Vector(InputIterator first, InputIterator last)
			:_start(nullptr),
			_finish(nullptr),
			_end_of_storage(nullptr)
		{
			while (first != last)
			{
				push_back(*first);
				first++;
			}
		}
		Vector(const Vector& v):
			_start(nullptr),//防止传值调用函数时,拷贝构造出问题
			_finish(nullptr),
			_end_of_storage(nullptr)
		{
			reserve(v.capacity());
			for (auto& e : v)
			{
				push_back(e);
			}
		}


		Vector<T>& operator=(Vector<T> tmp)
		{
			swap(tmp);
			return *this;
		}
		void swap(Vector<T>& v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}

		size_t capacity() const
		{
			return _end_of_storage - _start;
		}
		size_t size() const
		{
			return _finish - _start;
		}


		void resize(size_t n, const T& val = T())
		{
			if (n <= size())
			{
				_finish = _start + n;
			}
			else
			{
				reserve(n);
				while (n > size())
				{
					*_finish = val;
					++_finish;
				}
			}
		}
		void reserve(size_t n)
		{
			if (n > capacity())
			{
				size_t sz = size();
				size_t cc = capacity();
				T* tmp = new T[n];
				if (_start)
				{
					//memcpy(tmp, _start, sizeof(T) * sz);//会发生浅拷贝
					for (size_t i = 0; i < sz; i++)
					{
						tmp[i] = _start[i];//赋值重载
					}
					delete[] _start;
				}
				_start = tmp;
				_finish = _start + sz;
				_end_of_storage = _start + n;
			}
		}

		//头插
		void push_front(const T& val)
		{
			insert(begin(), val);
		}

		//尾插
		void push_back(const T& val)
		{
			if (_finish == _end_of_storage)
			{
				size_t cc = (capacity() == 0 ? 4 : capacity() * 2);
				reserve(cc);
			}
			*_finish = val;
			++_finish;

		}

		//插入
		void insert(iterator pos, const T& val)
		{
			assert(_start <= pos && pos <= _finish);
			if (size() + 1 > capacity())
			{
				size_t pos_index = pos - _start;
				reserve(size()+1);
				pos = _start + pos_index;
			}
			iterator end = _finish;
			while (end>pos)
			{
				*end = *(end-1);
				end--;
			}
			*pos = val;
			++_finish;
		}

		//头删
		void pop_front()
		{
			erase(begin());
		}

		//尾删
		void pop_back()
		{
			erase(end() - 1);
		}

		//删除
		void erase(iterator pos)
		{
			assert(_start<=pos && pos<_finish);
			
			iterator end = pos+1;
			while (end < _finish)
			{
				*(end-1) = *end;
				end++;
			}
			--_finish;
		}

		void clear()
		{
			_finish = _start;
		}

		T& operator[](size_t pos)
		{
			assert(pos < size());
			return _start[pos];
		}
		const T& operator[](size_t pos) const
		{
			assert(pos < size());
			return _start[pos];
		}

		//获取第一个元素
		T& front()
		{
			assert(size() > 0);
			return _start[0];
		}

		const T& front() const
		{
			assert(size() > 0);
			return _start[0];
		}

		//获取最后一个元素
		T& back()
		{
			assert(size() > 0);
			return _start[size() - 1];
		}

		const T& back() const
		{
			assert(size() > 0);
			return _start[size() - 1];
		}



		//迭代器
		iterator begin()
		{
			return _start;
		}
		iterator end()
		{
			return _finish;
		}
		const_iterator begin() const
		{
			return _start;
		}
		const_iterator end() const
		{
			return _finish;
		}
		const_iterator cbegin() const
		{
			return _start;
		}
		const_iterator cend() const
		{
			return _finish;
		}
		reverse_iterator rbegin()
		{
			return iterator(end());//隐式类型转换+内置类型可以认为有构造函数和匿名对象
		}
		reverse_iterator rend()
		{
			return iterator(begin());//隐式类型转换+内置类型可以认为有构造函数和匿名对象
		}
		const_reverse_iterator rbegin() const
		{
			return const_iterator(end());//隐式类型转换+内置类型可以认为有构造函数和匿名对象
		}
		const_reverse_iterator rend() const
		{
			return const_iterator(begin());//隐式类型转换+内置类型可以认为有构造函数和匿名对象
		}
		
		~Vector()
		{
			delete[] _start;
			_start = _finish = _end_of_storage = nullptr;
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storage;
	};
}
