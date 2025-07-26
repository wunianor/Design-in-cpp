

#include <vector>
using namespace std;

/// <summary>
/// 位图类
/// </summary>
/// <typeparam name="N">需要的比特位的位数</typeparam>
template<size_t N>
class Bitset
{
	/// <summary>
	/// 位图
	/// </summary>
	vector<int> _bitset;

	/// <summary>
	/// 统计置为1的位的个数
	/// </summary>
	size_t _count = 0;

public:
	Bitset() :
		_bitset(N / 32 + 1, 0),
		_count(0)
	{}

	/// <summary>
	/// 将 第pos位 置为1
	/// </summary>
	/// <param name="pos">需要置为1的位</param>
	void set(size_t pos)
	{
		size_t i = pos / 32;
		size_t j = pos % 32;

		if (test(pos) == 0)
		{
			++_count;
		}

		_bitset[i] |= (1 << j);

		
	}

	/// <summary>
	/// 将 第pos位 置为0
	/// </summary>
	/// <param name="pos">需要置为0的位</param>
	void reset(size_t pos)
	{
		size_t i = pos / 32;
		size_t j = pos % 32;


		if (test(pos) == 1)
		{
			--_count;
		}

		_bitset[i] &= ~(1 << j);
	}

	/// <summary>
	/// 检查第pos位是否为1
	/// </summary>
	/// <param name="pos">需要检查的位</param>
	/// <returns>第pos位是1返回true,否则返回false</returns>
	bool test(size_t pos)
	{
		size_t i = pos / 32;
		size_t j = pos % 32;

		return _bitset[i] & (1 << j);
	}

	/// <summary>
	/// 重载[],获取第pos位的值
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool operator[](size_t pos)
	{
		return test(pos);
	}

	/// <summary>
	/// 翻转第pos位
	/// </summary>
	/// <param name="pos">待翻转的位</param>
	void filp(size_t pos)
	{
		if (test(pos))
		{
			reset(pos);
		}
		else
		{
			set(pos);
		}
	}
};