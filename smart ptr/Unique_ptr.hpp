#pragma once

#include <iostream>
#include <functional>


/// <summary>
/// Unique_ptr 是一个智能指针类，
/// 负责独占管理动态分配的对象，
/// 并在对象生命周期结束时自动释放资源，
/// 防止内存泄漏。
/// </summary>
/// <typeparam name="T">被管理对象的类型。</typeparam>
template<class T>
class Unique_ptr
{
	/// <summary>
	/// 指向内存资源的指针
	/// </summary>
	T* _ptr;

	/// <summary>
	/// 自定义删除器,用于自定义资源释放方式
	/// </summary>
	std::function<void(T*)> _deleter = [](T* ptr) {delete ptr; };

private:
	////c++98解决禁止拷贝构造和复制重载的方案
	//// 1.只声明不实现
	//// 2.声明成私有成员
	////拷贝构造和复制重载的声明放到private内,
	////防止外部进行实现 ,
	////这种技术也叫防拷贝

	////作用:防止编译器生成默认拷贝构造
	//Unique_ptr(const Unique_ptr& other);

	////作用:防止编译器生成默认拷贝构造
	//Unique_ptr<T>& operator=(const Unique_ptr& other);

public:
	/// <summary>
	/// 创建一个Unique_ptr对象,并初始化为指定的指针---RAII思想
	/// </summary>
	/// <param name="ptr">用于初始化Unique_ptr的原始指针</param>
	explicit Unique_ptr(T* ptr = nullptr) :
		_ptr(ptr) 
	{
		std::cout << "构造:Unique_ptr" << std::endl;
	}

	/// <summary>
	/// 使用自定义删除器构造 Unique_ptr 智能指针
	/// </summary>
	/// <typeparam name="D">删除器的类型</typeparam>
	/// <param name="ptr">要管理的原始指针</param>
	/// <param name="deleter">用于释放指针资源的自定义删除器</param>
	template<class D>
	Unique_ptr(T* ptr, D deleter) :
		_ptr(ptr),
		_deleter(deleter)
	{
		std::cout << "构造:Unique_ptr(自定义删除器)" << std::endl;
	}

	/// <summary>
	/// 析构函数---RAII思想
	/// </summary>
	~Unique_ptr()
	{
		if (_ptr)
		{
			_deleter(_ptr);
		}

		std::cout << "析构:Unique_ptr" << std::endl;
	}

	/// <summary>
	/// 返回指针所指向的对象的引用
	/// </summary>
	/// <returns>指针所指向对象的引用</returns>
	T& operator*()
	{
		return *_ptr;
	}

	/// <summary>
	/// 返回指向对象的指针,以便通过箭头操作符访问成员
	/// </summary>
	/// <returns>指向对象的指针，可用于成员访问</returns>
	T* operator->()
	{
		return _ptr;
	}


	//c++11解决禁止拷贝构造和复制重载的方案
	Unique_ptr(const Unique_ptr<T>&) = delete;
	Unique_ptr<T>& operator=(const Unique_ptr<T>&) = delete;
};
