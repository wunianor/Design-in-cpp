
#include <bitset>
using namespace std;

/// <summary>
/// BKDRHash�㷨(�ַ�����ϣ)
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
/// APHash�㷨(�ַ�����ϣ)
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
/// DJBhash�㷨(�ַ�����ϣ)
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
/// ��¡������
/// </summary>
/// <typeparam name="K">��������</typeparam>
/// <typeparam name="Hash1">��ϣ����1</typeparam>
/// <typeparam name="Hash2">��ϣ����2</typeparam>
/// <typeparam name="Hash3">��ϣ����3</typeparam>
/// <typeparam name="N">��Ҫ�ı���λ��λ��</typeparam>
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
	/// λͼ
	/// </summary>
	bitset<N> _bitset;


public:
	
	/// <summary>
	/// �����
	/// </summary>
	/// <param name="key">������ļ�</param>
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
	/// �жϼ��Ƿ��ڲ�¡��������
	/// </summary>
	/// <param name="key">���жϵļ�</param>
	/// <returns>
	/// ����,����false(һ��׼ȷ);
	/// ��,����true(���ܻ�����)
	/// </returns>
	bool test(const K& key)
	{
		//�жϲ�����׼ȷ��
		size_t index1 = Hash1()(key) % N;
		if (_bitset.test(index1) == false) return false;

		size_t index2 = Hash2()(key) % N;
		if (_bitset.test(index2) == false) return false;

		size_t index3 = Hash3()(key) % N;
		if (_bitset.test(index3) == false) return false;

		
		return true;//�ж��ڿ���������
	}
};