#pragma once

#include <iostream>
#include <assert.h>
using namespace std;
namespace STRING
{
	class String
	{
		char* _str;
		size_t _size;
		size_t _capacity;
		const static size_t npos;
	public:
		typedef char* iterator;
		typedef const char* const_iterator;


		String(const char* str = "") :
			_size(strlen(str)),
			_capacity(_size+4)
		{
			_str = new char[_capacity + 1];
			strcpy(_str, str);
			//cout << "构造" << endl;
		}
		
		//传统拷贝构造和赋值重载的写法
		/*String(const String& s1)
		{
			_size = s1._size;
			_capacity = s1._capacity;
			_str = new char[_capacity + 1];
			strcpy(_str, s1._str);
			//cout << "拷贝构造" << endl;
		}
		String& operator=(const String& s1)
		{
			if (this != &s1)
			{
				_size = s1._size;
				_capacity = s1._capacity;
				char* tmp= new char[_capacity + 1];
				strcpy(tmp, s1._str);
				delete[] _str;
				_str = tmp;
			}
			return *this;
		}*/
		//现代拷贝构造和赋值重载的写法
		String(const String& s1):
			_str(nullptr),
			_size(0),
			_capacity(0)
		{
			String tmp(s1._str);
			swap(tmp);
		}
		String& operator=(String s1)
		{
			swap(s1);
			return *this;
		}
		void swap(String& s1)
		{
			std::swap(_str, s1._str);
			std::swap(_size, s1._size);
			std::swap(_capacity, s1._capacity);
		}


		size_t size() const
		{
			return _size;
		}
		size_t capacity() const
		{
			return _capacity;
		}


		char& operator[](size_t pos)//可读可写
		{
			assert(pos < _size);
			return _str[pos];
		}
		const char& operator[](size_t pos) const//只读
		{
			assert(pos<_size);
			return _str[pos];
		}

		void reserve(size_t n)
		{
			if (n > _capacity)
			{
				char* tmp = new char[n+1];
				strcpy(tmp,_str);
				delete[] _str;
				_str = tmp;
				_capacity = n;
				//cout << "扩容" << endl;
			}
		}

		void push_back(char ch)
		{
			if (_size == _capacity)
			{
				reserve(_capacity*2);
			}
			_str[_size++] = ch;
			_str[_size] = '\0';
		}
		void append(const char* str)
		{
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(_size+len);
			}
			strcpy(_str+_size,str);
			_size += len;
		}
		String& operator+=(char ch)
		{
			push_back(ch);
			return *this;
		}
		String& operator+=(const char* str)
		{
			append(str);
			return *this;
		}
		String& operator+=(String& s1)
		{
			const char* tmp=s1.c_str();
			append(tmp);
			return *this;
		}

		void insert(size_t pos,char ch)
		{
			assert(pos<=_size);
			if (_size == _capacity)
			{
				reserve(_capacity*2);
			}
			for (size_t i = _size+1; i > pos; i--)
			{
				_str[i] = _str[i-1];
			}
			_str[pos] = ch;
			_size++;
		}
		void insert(size_t pos, const char* str)
		{
			assert(pos<=_size);
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(_size+len);
			}
			for (size_t i = _size + len; i > pos+len-1; i--)
			{
				_str[i] = _str[i - len];
			}
			strncpy(_str+pos,str,len);
			_size += len;
		}
		void erase(size_t pos, size_t len = String::npos)
		{
			assert(pos <= _size);
			if (len == npos || pos + len >= _size)
			{
				_str[pos] = '\0';
				_size = pos;
			}
			else
			{
				for (size_t i = pos + len; i <= _size; i++)
				{
					_str[i - len] = _str[i];
				}
				_size -= len;
			}
		}

		void resize(size_t n, char ch = '\0')
		{
			if (n > _capacity)
			{
				reserve(n);
			}
			if (n > _size)
			{
				for (size_t i = _size; i < n; i++)
				{
					_str[i] = ch;
				}
			}
			_str[n] = '\0';
			_size = n;
		}

		String substr(size_t pos = 0, size_t len= String::npos) const
		{
			assert(pos<=_size);
			String tmp;
			len = len > _size ? _size : len;
			for (size_t i = pos; i <pos + len && i<_size; i++)
			{
				tmp += _str[i];
			}
			return tmp;
		}


		//find系列
		size_t find(const String& s1, size_t pos = 0) const
		{
			return find(s1._str,pos);
		}
		size_t find(const char* str, size_t pos = 0) const
		{
			char* p_pos = strstr(_str+pos, str);
			if (p_pos == nullptr)
			{
				return npos;
			}
			return p_pos-_str;
		}
		size_t find(char ch, size_t pos = 0) const
		{
			for (size_t i = pos; i < _size; i++)
			{
				if (_str[i] == ch)
				{
					return i;
				}
			}
			return npos;
		}




		//比较
		bool operator<(const String& s1) const
		{
			return strcmp(_str, s1._str) < 0;
		}
		bool operator==(const String& s1) const
		{
			return strcmp(_str, s1._str) == 0;
		}
		bool operator<=(const String& s1) const
		{
			return *this < s1 || *this == s1;
		}
		bool operator>(const String& s1) const
		{
			return !(*this <= s1);
		}
		bool operator>=(const String& s1) const
		{
			return !(*this < s1);
		}
		bool operator!=(const String& s1) const
		{
			return !(*this == s1);
		}




		//迭代器
		iterator begin()
		{
			return _str;
		}
		const_iterator begin() const
		{
			return _str;
		}
		iterator end()
		{
			return _str + _size;
		}
		const_iterator end() const
		{
			return _str + _size;
		}



		const char* c_str() const
		{
			return _str;
		}
		void clear()
		{
			_str[0] = '\0';
			_size = 0;
		}
		~String()
		{
			delete[] _str;
			_str = nullptr;
			_size = 0;
			_capacity = 0;
			//cout << "析构" << endl;
		}
	};
	const size_t String::npos = -1;


	istream& operator>>(istream& in, String& s)
	{
		s.clear();
		char buff[129] = { 0 };//对流提取的优化-减少扩容次数
		size_t i = 0;
		char ch;
		ch = in.get();
		while (ch!=' '&&ch!='\n')
		{
			buff[i++] = ch;
			if (i == 128)
			{
				s += buff;
				i = 0;
			}
			ch = in.get();
		}
		if (i != 0)
		{
			buff[i] = '\0';
			s += buff;
		}
		return in;
	}
	ostream& operator<<(ostream& out,const String& s)
	{
		for (auto ch : s)
		{
			out << ch;
		}
		return out;
	}
}
