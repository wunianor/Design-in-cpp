#pragma once

#include <iostream>
#include <functional>


/// <summary>
/// Unique_ptr ��һ������ָ���࣬
/// �����ռ����̬����Ķ���
/// ���ڶ����������ڽ���ʱ�Զ��ͷ���Դ��
/// ��ֹ�ڴ�й©��
/// </summary>
/// <typeparam name="T">�������������͡�</typeparam>
template<class T>
class Unique_ptr
{
	/// <summary>
	/// ָ���ڴ���Դ��ָ��
	/// </summary>
	T* _ptr;

	/// <summary>
	/// �Զ���ɾ����,�����Զ�����Դ�ͷŷ�ʽ
	/// </summary>
	std::function<void(T*)> _deleter = [](T* ptr) {delete ptr; };

private:
	////c++98�����ֹ��������͸������صķ���
	//// 1.ֻ������ʵ��
	//// 2.������˽�г�Ա
	////��������͸������ص������ŵ�private��,
	////��ֹ�ⲿ����ʵ�� ,
	////���ּ���Ҳ�з�����

	////����:��ֹ����������Ĭ�Ͽ�������
	//Unique_ptr(const Unique_ptr& other);

	////����:��ֹ����������Ĭ�Ͽ�������
	//Unique_ptr<T>& operator=(const Unique_ptr& other);

public:
	/// <summary>
	/// ����һ��Unique_ptr����,����ʼ��Ϊָ����ָ��---RAII˼��
	/// </summary>
	/// <param name="ptr">���ڳ�ʼ��Unique_ptr��ԭʼָ��</param>
	explicit Unique_ptr(T* ptr = nullptr) :
		_ptr(ptr) 
	{
		std::cout << "����:Unique_ptr" << std::endl;
	}

	/// <summary>
	/// ʹ���Զ���ɾ�������� Unique_ptr ����ָ��
	/// </summary>
	/// <typeparam name="D">ɾ����������</typeparam>
	/// <param name="ptr">Ҫ�����ԭʼָ��</param>
	/// <param name="deleter">�����ͷ�ָ����Դ���Զ���ɾ����</param>
	template<class D>
	Unique_ptr(T* ptr, D deleter) :
		_ptr(ptr),
		_deleter(deleter)
	{
		std::cout << "����:Unique_ptr(�Զ���ɾ����)" << std::endl;
	}

	/// <summary>
	/// ��������---RAII˼��
	/// </summary>
	~Unique_ptr()
	{
		if (_ptr)
		{
			_deleter(_ptr);
		}

		std::cout << "����:Unique_ptr" << std::endl;
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


	//c++11�����ֹ��������͸������صķ���
	Unique_ptr(const Unique_ptr<T>&) = delete;
	Unique_ptr<T>& operator=(const Unique_ptr<T>&) = delete;
};
