
#include <bitset>
using namespace std;

/// <summary>
/// BKDRHash算法(字符串哈希)
/// </summary>
struct BKDRHash
{
	size_t operator()(const string& s)
	{
		size_t hashValue = 0;
		for (auto ch : s)
		{
			hashValue = hashValue * 131 + ch;
		}

		return hashValue;
	}

};

/// <summary>
/// APHash算法(字符串哈希)
/// </summary>
struct APHash
{
	size_t operator()(const string& str)
	{
		size_t hash = 0;
		for (long i = 0; i < str.size(); i++)
		{
			size_t ch = str[i];
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
			}
		}
		return hash;
	}
};

/// <summary>
/// DJBhash算法(字符串哈希)
/// </summary>
struct DJBHash
{

	size_t operator()(const string& str)
	{
		if (str.size() == 0)
		{
			return 0;
		}
		size_t hash = 5381;
		for (auto ch : str)
		{
			hash += (hash << 5) + ch;
		}
		return hash;
	}
};

/// <summary>
/// 布隆过滤器
/// </summary>
/// <typeparam name="K">键的类型</typeparam>
/// <typeparam name="Hash1">哈希函数1</typeparam>
/// <typeparam name="Hash2">哈希函数2</typeparam>
/// <typeparam name="Hash3">哈希函数3</typeparam>
/// <typeparam name="N">需要的比特位的位数</typeparam>
template<
	size_t N,
	class K,
	class Hash1=BKDRHash,
	class Hash2=APHash,
	class Hash3=DJBHash
>
class BloomFilter
{
	/// <summary>
	/// 位图
	/// </summary>
	bitset<N> _bitset;


public:
	
	/// <summary>
	/// 插入键
	/// </summary>
	/// <param name="key">待插入的键</param>
	void set(const K& key)
	{
		size_t index1 = Hash1()(key) % N;
		size_t index2 = Hash2()(key) % N;
		size_t index3 = Hash3()(key) % N;

		_bitset.set(index1);
		_bitset.set(index2);
		_bitset.set(index3);
	}

	/// <summary>
	/// 判断键是否在布隆过滤器内
	/// </summary>
	/// <param name="key">待判断的键</param>
	/// <returns>
	/// 不在,返回false(一定准确);
	/// 在,返回true(可能会误判)
	/// </returns>
	bool test(const K& key)
	{
		//判断不在是准确的
		size_t index1 = Hash1()(key) % N;
		if (_bitset.test(index1) == false) return false;

		size_t index2 = Hash2()(key) % N;
		if (_bitset.test(index2) == false) return false;

		size_t index3 = Hash3()(key) % N;
		if (_bitset.test(index3) == false) return false;

		
		return true;//判断在可能是误判
	}
};