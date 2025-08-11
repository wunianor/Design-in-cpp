#pragma once

#include <iostream>
#include <functional>

/// <summary>
/// Shared_ptr ��һ������ָ���࣬
/// �����Զ�����̬����Ķ�����������ڣ�
/// ͨ�����ü���ʵ����Դ���Զ��ͷţ�RAII����
/// ֧���Զ���ɾ�������������졢��ֵ���������ü�����ѯ�ȡ�
/// </summary>
/// <typeparam name="T">Ҫ����Ķ�������͡�</typeparam>
template<class T>
class Shared_ptr
{
	/// <summary>
	/// ָ���ڴ���Դ��ָ��
	/// </summary>
	T* _ptr;

	/// <summary>
	/// ���ü���,��¼���ٸ�ָ��ָ�������Դ
	/// </summary>
	int* _pCount;

	/// <summary>
	/// �Զ���ɾ����,�����Զ�����Դ�ͷŷ�ʽ
	/// </summary>
	std::function<void(T*)> _deleter = [](T* ptr) {delete ptr; };

private:

	/// <summary>
	/// ���ü���-1,
	/// ������ü���Ϊ0,���ͷ���Դ
	/// </summary>
	void release()
	{
		if (--(*_pCount) == 0)
		{
			_deleter(_ptr);	// ʹ���Զ���ɾ�����ͷ���Դ
			delete _pCount;
		}
	}


public:

	/// <summary>
	/// ����һ���������ָ���Share_ptr����,
	/// ����ʼ�����ü���
	/// (RAII)
	/// </summary>
	/// <param name="ptr">Ҫ��Share_ptr��������ԭʼָ��</param>
	explicit Shared_ptr(T* ptr=nullptr) :
		_ptr(ptr),
		_pCount(new int(1))
	{
		std::cout << "����:Share_ptr" << std::endl;
	}

	/// <summary>
	/// ʹ���Զ���ɾ��������һ�� Shared_ptr ����ָ�����
	/// (RAII)
	/// </summary>
	/// <typeparam name="D">ɾ���������͡�</typeparam>
	/// <param name="ptr">Ҫ�����ԭʼָ�롣</param>
	/// <param name="deleter">�����ͷ�ָ����Դ���Զ���ɾ������</param>
	template<class D>
	Shared_ptr(T* ptr, D deleter) :
		_ptr(ptr),
		_pCount(new int(1)),
		_deleter(deleter)
	{
		std::cout << "����:Share_ptr(�Զ���ɾ����)" << std::endl;
	}

	/// <summary>
	/// �������캯��,
	/// ���ڴ���һ���µ� Shared_ptr ʵ��,
	/// ��ʹ��ָ������һ�� Shared_ptr ��ͬ�Ķ���
	/// </summary>
	/// <param name="other">Ҫ������ Shared_ptr ����</param>
	Shared_ptr(const Shared_ptr<T>& other) :
		_ptr(other._ptr),
		_pCount(other._pCount)
	{
		++(*_pCount);

		std::cout << "��������:Share_ptr" << std::endl;
	}

	/// <summary>
	/// ����һ�� Shared_ptr<T> ����Դ�����ü�����ֵ����ǰ����ָ�룬ʵ�ָ�ֵ���������ء�
	/// </summary>
	/// <param name="other">Ҫ��ֵ����һ�� Shared_ptr<T> ����ָ�롣</param>
	/// <returns>���ص�ǰ��ֵ��� Shared_ptr<T> ���á�</returns>
	Shared_ptr<T>& operator=(const Shared_ptr<T>& other)
	{
		//����ǹ���ͬһ����Դ������ָ��,
		//ֱ�ӷ���(���Խ���Լ����Լ���ֵ������:a1=a1)
		if (_ptr != other._ptr)
		{
			release();

			//���¸�����ָ��������Դ�������ü���
			_ptr = other._ptr;
			_pCount = other._pCount;
			++(*_pCount);
		}

		return *this;
	}

	/// <summary>
	/// ���� Shared_ptr ����
	/// �������ü���Ϊ��ʱ�ͷ����������Դ��
	/// (RAII)
	/// </summary>
	~Shared_ptr()
	{
		release();

		std::cout << "����:Share_ptr" << std::endl;
	}



	/// <summary>
	/// ����ָ����ָ��Ķ��������
	/// </summary>
	/// <returns>ָ����ָ����������</returns>
	T& operator*()
	{
		return *_ptr;
	}

	/// <summary>
	/// ����ָ������ָ��,�Ա�ͨ����ͷ���������ʳ�Ա
	/// </summary>
	/// <returns>ָ������ָ�룬�����ڳ�Ա����</returns>
	T* operator->()
	{
		return _ptr;
	}



	/// <summary>
	/// ���ص�ǰ���ü�����ֵ��
	/// </summary>
	/// <returns>ָ�� _pCount ��ָ�������ֵ����ʾ���ü�����</returns>
	int use_count() const noexcept
	{
		return *_pCount;
	}

	/// <summary>
	/// �����ڲ�ָ���ֵ
	/// </summary>
	/// <returns>�ڲ��洢��ָ��_ptr</returns>
	T* get() const noexcept
	{
		return _ptr;
	}
};

/// <summary>
/// Weak_ptr ��һ��ģ���࣬
/// ���ڳ�������Ϊ T �Ķ����������ָ�롣
/// ������ͨ�� Shared_ptr<T> ������ Weak_ptr<T> ���й���͸�ֵ��
/// ����Ӱ�������������ڹ���
/// </summary>
/// <typeparam name="T">Ҫ���������õĶ������͡�</typeparam>
template<class T>
class Weak_ptr
{
	/// <summary>
	/// ָ������Ϊ T �Ķ����ָ��
	/// </summary>
	T* _ptr;


public:
	/// <summary>
	/// Weak_ptr��Ĭ�Ϲ��캯��
	/// </summary>
	Weak_ptr() : 
		_ptr(nullptr) 
	{
		std::cout << "����:Weak_ptr(Ĭ�Ϲ���)" << std::endl;
	}

	/// <summary>
	/// ʹ��Shared_ptr ������һ�� Weak_ptr ����
	/// </summary>
	/// <param name="shared_ptr">���ڳ�ʼ�� Weak_ptr �� Shared_ptr ����</param>
	Weak_ptr(const Shared_ptr<T>& shared_ptr):
		_ptr(shared_ptr.get()) 
	{
		std::cout << "����:Weak_ptr(shared_ptr����)" << std::endl;
	}

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="other">Ҫ������ Weak_ptr<T> ����</param>
	Weak_ptr(const Weak_ptr<T>& other) : 
		_ptr(other._ptr) 
	{
		std::cout << "����:Weak_ptr(��������)" << std::endl;
	}

	/// <summary>
	/// ��һ�� Shared_ptr<T> ��ָ�븳ֵ����ǰ Weak_ptr<T> ʵ��
	/// </summary>
	/// <param name="shared_ptr">Ҫ��ֵ�� Shared_ptr<T> ʵ��</param>
	/// <returns>���ص�ǰ��ֵ��� Weak_ptr<T> ʵ��������</returns>
	Weak_ptr<T>& operator=(const Shared_ptr<T>& shared_ptr)
	{
		_ptr= shared_ptr.get();
		std::cout << "��ֵ:Weak_ptr(shared_ptr��ֵ)" << std::endl;
		return *this;
	}

	/// <summary>
	/// ������ֵ
	/// </summary>
	/// <param name="other">Ҫ��ֵ����һ�� Weak_ptr<T> ����</param>
	/// <returns>���ص�ǰ��������ã�*this��</returns>
	Weak_ptr<T>& operator=(const Weak_ptr<T>& other)
	{
		_ptr = other._ptr;
		std::cout << "��ֵ:Weak_ptr(������ֵ)" << std::endl;
		return *this;
	}

	/// <summary>
	/// ��������
	/// </summary>
	~Weak_ptr()
	{
		std::cout << "����:Weak_ptr" << std::endl;
	}
};