

#include <vector>
using namespace std;

/// <summary>
/// λͼ��
/// </summary>
/// <typeparam name="N">��Ҫ�ı���λ��λ��</typeparam>
template<size_t N>
class Bitset
{
	/// <summary>
	/// λͼ
	/// </summary>
	vector<int> _bitset;

	/// <summary>
	/// ͳ����Ϊ1��λ�ĸ���
	/// </summary>
	size_t _count = 0;

public:
	Bitset() :
		_bitset(N / 32 + 1, 0),
		_count(0)
	{}

	/// <summary>
	/// �� ��posλ ��Ϊ1
	/// </summary>
	/// <param name="pos">��Ҫ��Ϊ1��λ</param>
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
	/// �� ��posλ ��Ϊ0
	/// </summary>
	/// <param name="pos">��Ҫ��Ϊ0��λ</param>
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
	/// ����posλ�Ƿ�Ϊ1
	/// </summary>
	/// <param name="pos">��Ҫ����λ</param>
	/// <returns>��posλ��1����true,���򷵻�false</returns>
	bool test(size_t pos)
	{
		size_t i = pos / 32;
		size_t j = pos % 32;

		return _bitset[i] & (1 << j);
	}

	/// <summary>
	/// ����[],��ȡ��posλ��ֵ
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool operator[](size_t pos)
	{
		return test(pos);
	}

	/// <summary>
	/// ��ת��posλ
	/// </summary>
	/// <param name="pos">����ת��λ</param>
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