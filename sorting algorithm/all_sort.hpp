#include <iostream>
#include <vector>
#include "heap.h"
#include <stack>
#include <map>

using namespace std;

//使用迭代器的都是左闭右开,
//使用下标的都是左闭右闭

//检查是否排序正确
template<class Compare>
bool check(vector<int>& v, Compare com)
{
	for (int i = 0; i < v.size() - 2; ++i)
	{
		if (com(v[i + 1], v[i]))
		{
			return false;
		}
	}
	return true;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//插入排序---时间:o(n^2)
template<class Compare>
void insert_sort(vector<int>& v, Compare com)
{
	for (int i = 1; i < v.size(); ++i)
	{
		int tmp = v[i];
		int end = i - 1;//指向已排序数组的末尾
		while (end >= 0)
		{
			if (com(tmp, v[end]))
			{
				v[end + 1] = v[end];
				--end;
			}
			else
			{
				break;
			}
		}
		v[end + 1] = tmp;
	}
}




//插入排序(可选择排序的范围)[left,right]
template<class T ,class Compare>
void insert_sort(vector<T>& v,int left, int right,Compare com)
{
	for (int i = left + 1; i <= right; ++i)
	{
		T tmp = v[i];
		int end = i - 1;//指向已排序数组的末尾
		while (end >= left)
		{
			if (com(tmp, v[end]))
			{
				v[end + 1] = v[end];
				end--;
			}
			else
			{
				break;
			}
		}
		v[end + 1] = tmp;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//希尔排序---时间:o(n^1.3)
template<class Compare>
void shell_sort(vector<int>& v, Compare com)
{
	int gap = v.size();//设定gap的初始值
	while (gap > 1)//最外层循环时间复杂度为o(logN)
	{
		gap = gap / 3 + 1;// 更新gap再排序,+1可以保证最后一次gap一定等于1,(gap/3+1)比(gap/2)更常用
		for (int i = 0; i < v.size() - gap; ++i)//这两层循环时间复杂度可以简单当作是o(n)
		{
			int end = i;//有序部分的最后一个元素
			int tmp = v[end + gap];//待排序的数
			while (end >= 0)
			{
				if (com(tmp, v[end]))//违反从小到大或者从大到小的性质
				{
					v[end + gap] = v[end];//挪动元素
					end -= gap;//end向前移动gap位
				}
				else break;
			}
			v[end + gap] = tmp;//插入tmp
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//选择排序---时间:o(n^2)
template<class Compare>
void select_sort(vector<int>& v,Compare com)
{
	int begin = 0, end = v.size() - 1;
	while (begin < end)//用begin和end来遍历整个数组
	{
		//找到下标区间[begin,end]内的最大值和最小值的下标,并分别与v[begin],v[end]进行交换(less<int>()),
		//greater<int>()与之相反
		int maxi = begin, mini = begin;//maxi与mini的意义在less<int>()的情况下为最大值的下标和最小值的下标,greater<int>()时则相反
		for (int i = begin; i <= end; ++i)
		{
			if (com(v[i],v[mini]))
			{
				mini = i;
			}
			if (com(v[maxi] ,v[i]))
			{
				maxi = i;
			}
		}
		swap(v[begin], v[mini]);
		if (begin == maxi)//处理最大值(less<int>()时)的下标为begin的情况
		{
			maxi = mini;
		}
		swap(v[end], v[maxi]);
		begin++;//begin向后移动
		end--;//end向前移动
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//堆排序---时间:o(nlogn)
template<class T, class Compare>
void heap_sort(vector<T>& v, Compare com)
{
	//升序---建大堆
	//降序---建小堆

	for (int i = (v.size() - 1 - 1) / 2; i >= 0; i--)//向下调整建堆--时间复杂度o(n)
	{
		//adjust_up(i,v,com);//向上调整--建堆(i初始等于1)
		adjust_down(i, v, v.size(), com);//向下调整--建堆(i初始等于(size()-1-1)/2)
	}

	for (int end = v.size() - 1; end > 0; end--)//模拟堆的删除---时间复杂度o(nlogn)
	{
		swap(v[0], v[end]);
		adjust_down(0, v, end, com);//向下调整算法
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//快速排序---时间:o(nlogn) 空间:o(logn)
//如果key是中位数,效果就会很好(二分了)
//如果数组有序,效果最差,时间复杂度为:o(n^2)---解决办法:1.随机数选key 2.三数取中(取中间值)(针对有序情况的优化)

//三数取中(下标)
int get_mid_number_index(vector<int>& v, int left, int right)//[left,right]
{
	int mid = left + (right - left) / 2;
	if (v[left] < v[mid])
	{
		if (v[mid] < v[right]) return mid;
		else if (v[right] < v[left]) return left;
		else return right;
	}
	else //v[left] >= v[mid]
	{
		if (v[mid] > v[right]) return mid;
		else if (v[right] > v[left]) return left;
		else return right;
	}
}

//快速排序---递归下标版
template<class Compare>
void quick_sort(vector<int>& v, int left, int right, Compare com)//[left,right]
{
	//前后指针法
	if (left < right)//递归出口
	{
		////优化:三数取中
		int midi = get_mid_number_index(v, left, right);
		swap(v[left], v[midi]);

		int keyi = left;
		int slow = left;//慢指针
		int fast = left;//快指针
		while (fast <= right)
		{
			if (com(v[fast], v[keyi]) && (++slow != fast))
			{	//如果v[fast]<v[keyi](less<int>())(或者v[fast]>v[keyi](greater<int>())), 
				//那么此时[slow+1,fast-1]区间的值一定>=v[keyi](或者一定<=v[keyi])
				
				//slow++,然后再交换(slow=cur时,不必交换)
				swap(v[slow], v[fast]);
			}
			++fast;
		}
		swap(v[keyi], v[slow]);//此时v[slow]一定<v[keyi],交换即可
		quick_sort(v, left, slow - 1, com);//去slow左边的区间
		quick_sort(v, slow + 1, right, com);//去slow右边的区间
	}



	////挖坑法
	//if (left < right)//递归出口
	//{
	//	int hole = v[left];//挖坑
	//	int l = left;
	//	int r = right;
	//	while (l < r)
	//	{
	//		//从右往左 找比hole小的值的下标(less<int>()) or 找比hole大的值的下标(greater<int>())
	//		while (l < r && (v[r] == hole || com(hole, v[r])))
	//		{
	//			--r;
	//		}
	//		v[l] = v[r];//填旧坑并挖新坑
	//		//从左往右 找比hole大的值的下标(less<int>()) or 找比hole小的值的下标(greater<int>())
	//		while (l < r && (v[l] == hole || com(v[l], hole)))
	//		{
	//			++l;
	//		}
	//		v[r] = v[l];//填旧坑并挖新坑
	//	}
	//	v[l] = hole;//相遇的位置一定是坑,覆盖即可
	//	quick_sort(v, left, l - 1, com);//去l左边的区间
	//	quick_sort(v, l + 1, right, com);//去l右边的区间
	//}







	////hoare写法
	//if (left < right)//递归出口
	//{
	//	int keyi = left;//keyi选左边,那么r先走,保证l与r相遇位置的值一定<=v[keyi]
	//	int l = left;
	//	int r = right;
	//	while (l < r)
	//	{
	//		//从右往左 找比v[keyi]小的值的下标(less<int>()) or 找比v[keyi]大的值的下标(greater<int>())
	//		while (l < r && (v[r] == v[keyi] || com(v[keyi], v[r])))
	//		{
	//			--r;
	//		}
	//		//从左往右 找比v[keyi]大的值的下标(less<int>()) or 找比v[keyi]小的值的下标(greater<int>())
	//		while (l < r && (v[l] == v[keyi] || com(v[l], v[keyi])))
	//		{
	//			++l;
	//		}
	//		swap(v[l], v[r]);//交换两个数
	//	}
	//	swap(v[keyi], v[l]);//l与r相遇的位置,就是v[keyi]的位置,进行交换
	//	quick_sort(v, left, l - 1, com);//去l左边的区间
	//	quick_sort(v, l + 1, right, com);//去l右边的区间
	//}









}

//////////////////////////////////////////////////////////////////////

//三数取中(迭代器)
template<class RandomAccessIterator>
RandomAccessIterator get_mid_number_index(RandomAccessIterator begin, RandomAccessIterator end)//[begin,end)
{
	RandomAccessIterator mid = begin + (end - 1 - begin) / 2;
	if (*begin < *mid)
	{
		if (*mid < *(end - 1))
		{
			return mid;
		}
		else if (*begin < *(end - 1))
		{
			return end - 1;
		}
		else
		{
			return begin;
		}
	}
	else
	{
		if (*mid > *(end - 1))
		{
			return mid;
		}
		else if (*begin > *(end - 1))
		{
			return end - 1;
		}
		else
		{
			return begin;
		}
	}
}

//快速排序---非递归版(迭代器)
template<class RandomAccessIterator,class Compare>
void quick_sort_NonR(RandomAccessIterator begin, RandomAccessIterator end,Compare com)//[begin,end)
{
	stack<pair<RandomAccessIterator, RandomAccessIterator>> st;
	st.push({begin, end});

	while (!st.empty())
	{
		pair<RandomAccessIterator, RandomAccessIterator> top = st.top();
		st.pop();
		RandomAccessIterator mid = get_mid_number_index(top.first, top.second);//三数取中
		swap(*(top.first), *mid);

		RandomAccessIterator keyi = top.first;
		RandomAccessIterator slow = top.first;//慢指针
		RandomAccessIterator fast = top.first;//快指针
		while (fast != top.second)
		{
			if (com(*fast, *keyi))
			{
				++slow;
				swap(*fast, *slow);
			}
			++fast;
		}
		swap(*keyi, *slow);
		if (slow + 1 < top.second - 1)//push右区间
		{
			st.push({slow + 1, top.second});//[slow+1,top.second)
		}
		if (top.first + 1 < slow)//push左区间
		{
			st.push({top.first, slow});//[top.first,slow)
		}
		
	}
}




//快速排序---三路划分(优化大量重复值的情况)+三数取中(优化有序情况)写法
template<class Compare>
void quick_sort(vector<int>& v,Compare com,int left, int right)
{
	if (left >= right)//区间不存在或者只有一个元素
	{
		return;
	}

	//三数取中
	int midi = get_mid_number_index(v,left,right);
	swap(v[left],v[midi]);

	//设定key=v[l],在排序的过程中v[l]的值一定一直等于key
	//[l,cur)之间的数一定等于key(v[l])
	//一趟排序走完后,[l,r]内的值一定等于key(v[l])
	int l = left;
	int key = v[l];
	int r = right;
	int cur = left+1;

	while (cur <= r)
	{
		if (com(v[cur] ,key))//当v[cur]小于key(v[l])时,交换v[cur]和v[l],然后cur++,l++
		{					   
			swap(v[cur], v[l]);
			cur++;
			l++;
		}
		else if (com(key,v[cur]))//当v[cur]大于key(v[l])时,交换v[cur]和v[r],然后r--,cur不动
		{						  //cur不动是因为不确定交换后v[cur]的值和key(v[l])的关系,需要继续比较
			swap(v[cur], v[r]);
			r--;
		}
		else//当v[cur]等于key(v[l])时,cur++
		{
			cur++;
		}
	}

	quick_sort(v, com, left, l - 1);//去左区间[left,l-1]排序
	quick_sort(v, com, r + 1, right);//去右区间[r+1,right]排序
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//冒泡排序---时间:o(n^2)
template<class Compare>
void bubble_sort(vector<int>& v,Compare com)
{
	for (int i = 0; i < v.size() - 1; ++i)
	{
		int flag = 1;
		for (int j = 0; j < v.size() - 1 - i; ++j)
		{
			if (com(v[j + 1], v[j]))
			{
				swap(v[j+1],v[j]);
				flag = 0;
			}
		}
		if (flag)
		{
			break;
		}
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//归并排序子排序
template<class T,class Compare>
void _merge_sort(vector<T>& v, int begin, int end, Compare com, vector<T>& tmp)//[begin,end]
{
	if (begin >= end)
	{
		return;
	}

	if (end - begin + 1 < 10)//小区间优化(可以减少约87.5%的递归开销),锦上添花
	{
		insert_sort(v, begin, end, com);
		return;
	}

	//递归使左右区间有序
	int mid = begin + (end - begin) / 2;
	_merge_sort(v, begin, mid, com, tmp);
	_merge_sort(v,mid + 1, end, com, tmp);

	//将左右两个有序区间进行合并到tmp数组
	int begin1 = begin, end1 = mid;
	int begin2 = mid + 1, end2 = end;
	int i = begin;
	while (begin1 <= end1 && begin2 <= end2)
	{
		if (com(v[begin1], v[begin2]) || v[begin1] == v[begin2])//等于保证排序的稳定性
		{
			tmp[i++] = v[begin1++];
		}
		else
		{
			tmp[i++] = v[begin2++];
		}
	}

	while (begin1 <= end1)
	{
		tmp[i++] = v[begin1++];
	}
	while (begin2 <= end2)
	{
		tmp[i++] = v[begin2++];
	}


	//将合并结果从tmp数组拷贝到原数组
	i = begin;
	while (i <= end)
	{
		v[i] = tmp[i];
		++i;
	}
}

//归并排序---时间:o(nlogn) 空间:o(n)
template<class T,class Compare>
void merge_sort(vector<T>& v,Compare com)
{
	vector<T> tmp = v;
	_merge_sort(v, 0, v.size() - 1, com, tmp);
}





//归并排序非递归
template<class T,class Compare>
void merge_sort_NonR(vector<T>& v,Compare com)
{
	vector<int> tmp = v;
	int sz = v.size();
	int gap = 1;//需要合并的两个区间每个区间的元素个数
	while (gap < sz)
	{
		for (int i = 0; i < sz; i += 2 * gap)
		{
			//左右两个区间的四种情况:
			//1.左右两个区间都不越界
			//2.右区间部分越界,即end2越界
			//3.右区间完全越界,即begin2越界
			//4.左区间部分越界+右区间完全越界,即end1越界

			int begin1 = i, end1 = i + gap - 1;//左区间的开始和结尾
			int begin2 = i + gap, end2 = i + 2 * gap - 1;//右区间的开始和结尾

			if (end1 >= sz || begin2 >= sz)
			{	//end1越界,即左区间部分存在,右区间完全不存在
				//begin2越界,即左区间完全存在,右区间完全不存在
				break;
			}
			if (end2 >= sz)//需要合并的第二个区间存在,但是end2不存在
			{
				end2 = sz - 1;
			}

			//将左右两个有序区间合并到tmp数组
			int j = i;
			while (begin1 <= end1 && begin2 <= end2)
			{
				if (com(v[begin1], v[begin2]) || v[begin1] == v[begin2])//等于保证排序的稳定性
				{
					tmp[j++] = v[begin1++];
				}
				else
				{
					tmp[j++] = v[begin2++];
				}
			}

			while (begin1 <= end1)
			{
				tmp[j++] = v[begin1++];
			}
			while (begin2 <= end2)
			{
				tmp[j++] = v[begin2++];
			}


			//将合并结果从tmp数组拷贝到原数组
			j = i;
			while (j <= end2)
			{
				v[j] = tmp[j];
				++j;
			}
		}
		gap *= 2;//合并之后gap *= 2
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//计数排序---时间复杂度:o(n+range),空间复杂度:o(range)
template<class Compare>
void count_sort(vector<int>& v, Compare com,int way)//way输入0使用map,输入1使用vector
{
	if (way == 0)//使用map
	{
		map<int, int> hash;
		for (auto i : v)
		{
			hash[i]++;
		}

		int i = 0;
		for (map<int, int>::iterator it = hash.begin(); it != hash.end(); ++it)
		{
			int count = it->second;
			while (count--)
			{
				v[i++] = it->first;
			}
		}
	}
	else//使用vector
	{
		int maxx = INT_MIN;//存储数组的最大值
		int minn = INT_MAX;//存储数组的最小值
		for (auto i : v)
		{
			maxx = max(maxx, i);
			minn = min(minn, i);
		}

		vector<int> hash(maxx - minn + 1);
		for (auto i : v)//统计元素出现的次数
		{
			hash[i - minn]++;
		}

		int index = 0;
		for (int i = 0; i < hash.size(); ++i)//根据出现次数写入覆盖原数组
		{
			int count = hash[i];
			while (count--)
			{
				v[index++] = i + minn;
			}
		}
	}
}





