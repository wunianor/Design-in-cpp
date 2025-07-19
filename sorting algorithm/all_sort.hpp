#include <iostream>
#include <vector>
#include "heap.h"
#include <stack>
#include <map>

using namespace std;

//ʹ�õ������Ķ�������ҿ�,
//ʹ���±�Ķ�������ұ�

//����Ƿ�������ȷ
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
//��������---ʱ��:o(n^2)
template<class Compare>
void insert_sort(vector<int>& v, Compare com)
{
	for (int i = 1; i < v.size(); ++i)
	{
		int tmp = v[i];
		int end = i - 1;//ָ�������������ĩβ
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




//��������(��ѡ������ķ�Χ)[left,right]
template<class T ,class Compare>
void insert_sort(vector<T>& v,int left, int right,Compare com)
{
	for (int i = left + 1; i <= right; ++i)
	{
		T tmp = v[i];
		int end = i - 1;//ָ�������������ĩβ
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


//ϣ������---ʱ��:o(n^1.3)
template<class Compare>
void shell_sort(vector<int>& v, Compare com)
{
	int gap = v.size();//�趨gap�ĳ�ʼֵ
	while (gap > 1)//�����ѭ��ʱ�临�Ӷ�Ϊo(logN)
	{
		gap = gap / 3 + 1;// ����gap������,+1���Ա�֤���һ��gapһ������1,(gap/3+1)��(gap/2)������
		for (int i = 0; i < v.size() - gap; ++i)//������ѭ��ʱ�临�Ӷȿ��Լ򵥵�����o(n)
		{
			int end = i;//���򲿷ֵ����һ��Ԫ��
			int tmp = v[end + gap];//���������
			while (end >= 0)
			{
				if (com(tmp, v[end]))//Υ����С������ߴӴ�С������
				{
					v[end + gap] = v[end];//Ų��Ԫ��
					end -= gap;//end��ǰ�ƶ�gapλ
				}
				else break;
			}
			v[end + gap] = tmp;//����tmp
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//ѡ������---ʱ��:o(n^2)
template<class Compare>
void select_sort(vector<int>& v,Compare com)
{
	int begin = 0, end = v.size() - 1;
	while (begin < end)//��begin��end��������������
	{
		//�ҵ��±�����[begin,end]�ڵ����ֵ����Сֵ���±�,���ֱ���v[begin],v[end]���н���(less<int>()),
		//greater<int>()��֮�෴
		int maxi = begin, mini = begin;//maxi��mini��������less<int>()�������Ϊ���ֵ���±����Сֵ���±�,greater<int>()ʱ���෴
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
		if (begin == maxi)//�������ֵ(less<int>()ʱ)���±�Ϊbegin�����
		{
			maxi = mini;
		}
		swap(v[end], v[maxi]);
		begin++;//begin����ƶ�
		end--;//end��ǰ�ƶ�
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//������---ʱ��:o(nlogn)
template<class T, class Compare>
void heap_sort(vector<T>& v, Compare com)
{
	//����---�����
	//����---��С��

	for (int i = (v.size() - 1 - 1) / 2; i >= 0; i--)//���µ�������--ʱ�临�Ӷ�o(n)
	{
		//adjust_up(i,v,com);//���ϵ���--����(i��ʼ����1)
		adjust_down(i, v, v.size(), com);//���µ���--����(i��ʼ����(size()-1-1)/2)
	}

	for (int end = v.size() - 1; end > 0; end--)//ģ��ѵ�ɾ��---ʱ�临�Ӷ�o(nlogn)
	{
		swap(v[0], v[end]);
		adjust_down(0, v, end, com);//���µ����㷨
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//��������---ʱ��:o(nlogn) �ռ�:o(logn)
//���key����λ��,Ч���ͻ�ܺ�(������)
//�����������,Ч�����,ʱ�临�Ӷ�Ϊ:o(n^2)---����취:1.�����ѡkey 2.����ȡ��(ȡ�м�ֵ)(�������������Ż�)

//����ȡ��(�±�)
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

//��������---�ݹ��±��
template<class Compare>
void quick_sort(vector<int>& v, int left, int right, Compare com)//[left,right]
{
	//ǰ��ָ�뷨
	if (left < right)//�ݹ����
	{
		////�Ż�:����ȡ��
		int midi = get_mid_number_index(v, left, right);
		swap(v[left], v[midi]);

		int keyi = left;
		int slow = left;//��ָ��
		int fast = left;//��ָ��
		while (fast <= right)
		{
			if (com(v[fast], v[keyi]) && (++slow != fast))
			{	//���v[fast]<v[keyi](less<int>())(����v[fast]>v[keyi](greater<int>())), 
				//��ô��ʱ[slow+1,fast-1]�����ֵһ��>=v[keyi](����һ��<=v[keyi])
				
				//slow++,Ȼ���ٽ���(slow=curʱ,���ؽ���)
				swap(v[slow], v[fast]);
			}
			++fast;
		}
		swap(v[keyi], v[slow]);//��ʱv[slow]һ��<v[keyi],��������
		quick_sort(v, left, slow - 1, com);//ȥslow��ߵ�����
		quick_sort(v, slow + 1, right, com);//ȥslow�ұߵ�����
	}



	////�ڿӷ�
	//if (left < right)//�ݹ����
	//{
	//	int hole = v[left];//�ڿ�
	//	int l = left;
	//	int r = right;
	//	while (l < r)
	//	{
	//		//�������� �ұ�holeС��ֵ���±�(less<int>()) or �ұ�hole���ֵ���±�(greater<int>())
	//		while (l < r && (v[r] == hole || com(hole, v[r])))
	//		{
	//			--r;
	//		}
	//		v[l] = v[r];//��ɿӲ����¿�
	//		//�������� �ұ�hole���ֵ���±�(less<int>()) or �ұ�holeС��ֵ���±�(greater<int>())
	//		while (l < r && (v[l] == hole || com(v[l], hole)))
	//		{
	//			++l;
	//		}
	//		v[r] = v[l];//��ɿӲ����¿�
	//	}
	//	v[l] = hole;//������λ��һ���ǿ�,���Ǽ���
	//	quick_sort(v, left, l - 1, com);//ȥl��ߵ�����
	//	quick_sort(v, l + 1, right, com);//ȥl�ұߵ�����
	//}







	////hoareд��
	//if (left < right)//�ݹ����
	//{
	//	int keyi = left;//keyiѡ���,��ôr����,��֤l��r����λ�õ�ֵһ��<=v[keyi]
	//	int l = left;
	//	int r = right;
	//	while (l < r)
	//	{
	//		//�������� �ұ�v[keyi]С��ֵ���±�(less<int>()) or �ұ�v[keyi]���ֵ���±�(greater<int>())
	//		while (l < r && (v[r] == v[keyi] || com(v[keyi], v[r])))
	//		{
	//			--r;
	//		}
	//		//�������� �ұ�v[keyi]���ֵ���±�(less<int>()) or �ұ�v[keyi]С��ֵ���±�(greater<int>())
	//		while (l < r && (v[l] == v[keyi] || com(v[l], v[keyi])))
	//		{
	//			++l;
	//		}
	//		swap(v[l], v[r]);//����������
	//	}
	//	swap(v[keyi], v[l]);//l��r������λ��,����v[keyi]��λ��,���н���
	//	quick_sort(v, left, l - 1, com);//ȥl��ߵ�����
	//	quick_sort(v, l + 1, right, com);//ȥl�ұߵ�����
	//}









}

//////////////////////////////////////////////////////////////////////

//����ȡ��(������)
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

//��������---�ǵݹ��(������)
template<class RandomAccessIterator,class Compare>
void quick_sort_NonR(RandomAccessIterator begin, RandomAccessIterator end,Compare com)//[begin,end)
{
	stack<pair<RandomAccessIterator, RandomAccessIterator>> st;
	st.push({begin, end});

	while (!st.empty())
	{
		pair<RandomAccessIterator, RandomAccessIterator> top = st.top();
		st.pop();
		RandomAccessIterator mid = get_mid_number_index(top.first, top.second);//����ȡ��
		swap(*(top.first), *mid);

		RandomAccessIterator keyi = top.first;
		RandomAccessIterator slow = top.first;//��ָ��
		RandomAccessIterator fast = top.first;//��ָ��
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
		if (slow + 1 < top.second - 1)//push������
		{
			st.push({slow + 1, top.second});//[slow+1,top.second)
		}
		if (top.first + 1 < slow)//push������
		{
			st.push({top.first, slow});//[top.first,slow)
		}
		
	}
}




//��������---��·����(�Ż������ظ�ֵ�����)+����ȡ��(�Ż��������)д��
template<class Compare>
void quick_sort(vector<int>& v,Compare com,int left, int right)
{
	if (left >= right)//���䲻���ڻ���ֻ��һ��Ԫ��
	{
		return;
	}

	//����ȡ��
	int midi = get_mid_number_index(v,left,right);
	swap(v[left],v[midi]);

	//�趨key=v[l],������Ĺ�����v[l]��ֵһ��һֱ����key
	//[l,cur)֮�����һ������key(v[l])
	//һ�����������,[l,r]�ڵ�ֵһ������key(v[l])
	int l = left;
	int key = v[l];
	int r = right;
	int cur = left+1;

	while (cur <= r)
	{
		if (com(v[cur] ,key))//��v[cur]С��key(v[l])ʱ,����v[cur]��v[l],Ȼ��cur++,l++
		{					   
			swap(v[cur], v[l]);
			cur++;
			l++;
		}
		else if (com(key,v[cur]))//��v[cur]����key(v[l])ʱ,����v[cur]��v[r],Ȼ��r--,cur����
		{						  //cur��������Ϊ��ȷ��������v[cur]��ֵ��key(v[l])�Ĺ�ϵ,��Ҫ�����Ƚ�
			swap(v[cur], v[r]);
			r--;
		}
		else//��v[cur]����key(v[l])ʱ,cur++
		{
			cur++;
		}
	}

	quick_sort(v, com, left, l - 1);//ȥ������[left,l-1]����
	quick_sort(v, com, r + 1, right);//ȥ������[r+1,right]����
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//ð������---ʱ��:o(n^2)
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




//�鲢����������
template<class T,class Compare>
void _merge_sort(vector<T>& v, int begin, int end, Compare com, vector<T>& tmp)//[begin,end]
{
	if (begin >= end)
	{
		return;
	}

	if (end - begin + 1 < 10)//С�����Ż�(���Լ���Լ87.5%�ĵݹ鿪��),������
	{
		insert_sort(v, begin, end, com);
		return;
	}

	//�ݹ�ʹ������������
	int mid = begin + (end - begin) / 2;
	_merge_sort(v, begin, mid, com, tmp);
	_merge_sort(v,mid + 1, end, com, tmp);

	//��������������������кϲ���tmp����
	int begin1 = begin, end1 = mid;
	int begin2 = mid + 1, end2 = end;
	int i = begin;
	while (begin1 <= end1 && begin2 <= end2)
	{
		if (com(v[begin1], v[begin2]) || v[begin1] == v[begin2])//���ڱ�֤������ȶ���
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


	//���ϲ������tmp���鿽����ԭ����
	i = begin;
	while (i <= end)
	{
		v[i] = tmp[i];
		++i;
	}
}

//�鲢����---ʱ��:o(nlogn) �ռ�:o(n)
template<class T,class Compare>
void merge_sort(vector<T>& v,Compare com)
{
	vector<T> tmp = v;
	_merge_sort(v, 0, v.size() - 1, com, tmp);
}





//�鲢����ǵݹ�
template<class T,class Compare>
void merge_sort_NonR(vector<T>& v,Compare com)
{
	vector<int> tmp = v;
	int sz = v.size();
	int gap = 1;//��Ҫ�ϲ�����������ÿ�������Ԫ�ظ���
	while (gap < sz)
	{
		for (int i = 0; i < sz; i += 2 * gap)
		{
			//��������������������:
			//1.�����������䶼��Խ��
			//2.�����䲿��Խ��,��end2Խ��
			//3.��������ȫԽ��,��begin2Խ��
			//4.�����䲿��Խ��+��������ȫԽ��,��end1Խ��

			int begin1 = i, end1 = i + gap - 1;//������Ŀ�ʼ�ͽ�β
			int begin2 = i + gap, end2 = i + 2 * gap - 1;//������Ŀ�ʼ�ͽ�β

			if (end1 >= sz || begin2 >= sz)
			{	//end1Խ��,�������䲿�ִ���,��������ȫ������
				//begin2Խ��,����������ȫ����,��������ȫ������
				break;
			}
			if (end2 >= sz)//��Ҫ�ϲ��ĵڶ����������,����end2������
			{
				end2 = sz - 1;
			}

			//������������������ϲ���tmp����
			int j = i;
			while (begin1 <= end1 && begin2 <= end2)
			{
				if (com(v[begin1], v[begin2]) || v[begin1] == v[begin2])//���ڱ�֤������ȶ���
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


			//���ϲ������tmp���鿽����ԭ����
			j = i;
			while (j <= end2)
			{
				v[j] = tmp[j];
				++j;
			}
		}
		gap *= 2;//�ϲ�֮��gap *= 2
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//��������---ʱ�临�Ӷ�:o(n+range),�ռ临�Ӷ�:o(range)
template<class Compare>
void count_sort(vector<int>& v, Compare com,int way)//way����0ʹ��map,����1ʹ��vector
{
	if (way == 0)//ʹ��map
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
	else//ʹ��vector
	{
		int maxx = INT_MIN;//�洢��������ֵ
		int minn = INT_MAX;//�洢�������Сֵ
		for (auto i : v)
		{
			maxx = max(maxx, i);
			minn = min(minn, i);
		}

		vector<int> hash(maxx - minn + 1);
		for (auto i : v)//ͳ��Ԫ�س��ֵĴ���
		{
			hash[i - minn]++;
		}

		int index = 0;
		for (int i = 0; i < hash.size(); ++i)//���ݳ��ִ���д�븲��ԭ����
		{
			int count = hash[i];
			while (count--)
			{
				v[index++] = i + minn;
			}
		}
	}
}





