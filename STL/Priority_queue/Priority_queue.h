#pragma once

#include <iostream>
#include <vector>
using namespace std;


namespace P_Q
{
	template<class T,class Container=vector<T>,class Compare=less<T>>
	class Priority_queue
	{
		Container _con;

		void adjust_up(int child)//���ϵ���
		{
			Compare com;
			int parent = (child - 1) / 2;//����child���±�Ѱ����parent���±�Ĺ�ʽ
			while (child > 0)
			{
				if (com(_con[parent], _con[child]))//���Υ���ѵ�����
				{
					swap(_con[child], _con[parent]);

					//��������Ѱ����parent
					child = parent;
					parent = (child - 1) / 2;
				}
				else
				{
					break;
				}
			}
		}

		void adjust_down(int parent)//���µ���
		{
			Compare com;
			int child = parent * 2 + 1;//����parent���±�Ѱ��child���±�Ĺ�ʽ
			while (child < _con.size())
			{
				if (child + 1 < _con.size() &&
					com(_con[child], _con[child + 1]))//Ѱ��ֵ�ϴ���Ǹ�child
				{
					++child;
				}


				if (com(_con[parent], _con[child]))//���Υ���ѵ�����
				{
					swap(_con[child], _con[parent]);

					//��������Ѱ����child
					parent = child;
					child = parent * 2 + 1;
				}
				else
				{
					break;
				}
			}

		}

	public:

		Priority_queue(){}

		template<class InputIterator>
		Priority_queue(InputIterator first, InputIterator last) :
			_con(first,last)
		{
			//����
			for (int i = (_con.size() - 2) / 2; i >= 0; i--)
			{
				adjust_down(i);
			}
		}
	
		void push(const T& val)
		{
			_con.push_back(val);
			adjust_up(_con.size()-1);
		}
		void pop()
		{
			swap(_con[0],_con[_con.size()-1]);
			_con.pop_back();
			adjust_down(0);
		}

		T& top()
		{
			return _con[0];
		}
		const T& top() const
		{
			return _con[0];
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