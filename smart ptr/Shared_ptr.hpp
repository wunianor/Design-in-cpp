#pragma once

#include <iostream>
#include <functional>

/// <summary>
/// Shared_ptr 是一个智能指针类，
/// 用于自动管理动态分配的对象的生命周期，
/// 通过引用计数实现资源的自动释放（RAII）。
/// 支持自定义删除器、拷贝构造、赋值操作和引用计数查询等。
/// </summary>
/// <typeparam name="T">要管理的对象的类型。</typeparam>
template<class T>
class Shared_ptr
{
	/// <summary>
	/// 指向内存资源的指针
	/// </summary>
	T* _ptr;

	/// <summary>
	/// 引用计数,记录多少个指针指向这份资源
	/// </summary>
	int* _pCount;

	/// <summary>
	/// 自定义删除器,用于自定义资源释放方式
	/// </summary>
	std::function<void(T*)> _deleter = [](T* ptr) {delete ptr; };

private:

	/// <summary>
	/// 引用计数-1,
	/// 如果引用计数为0,则释放资源
	/// </summary>
	void release()
	{
		if (--(*_pCount) == 0)
		{
			_deleter(_ptr);	// 使用自定义删除器释放资源
			delete _pCount;
		}
	}


public:

	/// <summary>
	/// 创建一个管理给定指针的Share_ptr对象,
	/// 并初始化引用计数
	/// (RAII)
	/// </summary>
	/// <param name="ptr">要由Share_ptr对象管理的原始指针</param>
	explicit Shared_ptr(T* ptr=nullptr) :
		_ptr(ptr),
		_pCount(new int(1))
	{
		std::cout << "构造:Share_ptr" << std::endl;
	}

	/// <summary>
	/// 使用自定义删除器构造一个 Shared_ptr 智能指针对象。
	/// (RAII)
	/// </summary>
	/// <typeparam name="D">删除器的类型。</typeparam>
	/// <param name="ptr">要管理的原始指针。</param>
	/// <param name="deleter">用于释放指针资源的自定义删除器。</param>
	template<class D>
	Shared_ptr(T* ptr, D deleter) :
		_ptr(ptr),
		_pCount(new int(1)),
		_deleter(deleter)
	{
		std::cout << "构造:Share_ptr(自定义删除器)" << std::endl;
	}

	/// <summary>
	/// 拷贝构造函数,
	/// 用于创建一个新的 Shared_ptr 实例,
	/// 并使其指向与另一个 Shared_ptr 相同的对象
	/// </summary>
	/// <param name="other">要拷贝的 Shared_ptr 对象</param>
	Shared_ptr(const Shared_ptr<T>& other) :
		_ptr(other._ptr),
		_pCount(other._pCount)
	{
		++(*_pCount);

		std::cout << "拷贝构造:Share_ptr" << std::endl;
	}

	/// <summary>
	/// 将另一个 Shared_ptr<T> 的资源和引用计数赋值给当前智能指针，实现赋值操作符重载。
	/// </summary>
	/// <param name="other">要赋值的另一个 Shared_ptr<T> 智能指针。</param>
	/// <returns>返回当前赋值后的 Shared_ptr<T> 引用。</returns>
	Shared_ptr<T>& operator=(const Shared_ptr<T>& other)
	{
		//如果是管理同一份资源的智能指针,
		//直接返回(可以解决自己给自己赋值的问题:a1=a1)
		if (_ptr != other._ptr)
		{
			release();

			//更新该智能指针管理的资源及其引用计数
			_ptr = other._ptr;
			_pCount = other._pCount;
			++(*_pCount);
		}

		return *this;
	}

	/// <summary>
	/// 销毁 Shared_ptr 对象，
	/// 并在引用计数为零时释放所管理的资源。
	/// (RAII)
	/// </summary>
	~Shared_ptr()
	{
		release();

		std::cout << "析构:Share_ptr" << std::endl;
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



	/// <summary>
	/// 返回当前引用计数的值。
	/// </summary>
	/// <returns>指针 _pCount 所指向的整数值，表示引用计数。</returns>
	int use_count() const noexcept
	{
		return *_pCount;
	}

	/// <summary>
	/// 返回内部指针的值
	/// </summary>
	/// <returns>内部存储的指针_ptr</returns>
	T* get() const noexcept
	{
		return _ptr;
	}
};

/// <summary>
/// Weak_ptr 是一个模板类，
/// 用于持有类型为 T 的对象的弱引用指针。
/// 它可以通过 Shared_ptr<T> 或其他 Weak_ptr<T> 进行构造和赋值，
/// 不会影响对象的生命周期管理。
/// </summary>
/// <typeparam name="T">要持有弱引用的对象类型。</typeparam>
template<class T>
class Weak_ptr
{
	/// <summary>
	/// 指向类型为 T 的对象的指针
	/// </summary>
	T* _ptr;


public:
	/// <summary>
	/// Weak_ptr的默认构造函数
	/// </summary>
	Weak_ptr() : 
		_ptr(nullptr) 
	{
		std::cout << "构造:Weak_ptr(默认构造)" << std::endl;
	}

	/// <summary>
	/// 使用Shared_ptr 对象构造一个 Weak_ptr 对象
	/// </summary>
	/// <param name="shared_ptr">用于初始化 Weak_ptr 的 Shared_ptr 对象</param>
	Weak_ptr(const Shared_ptr<T>& shared_ptr):
		_ptr(shared_ptr.get()) 
	{
		std::cout << "构造:Weak_ptr(shared_ptr构造)" << std::endl;
	}

	/// <summary>
	/// 拷贝构造
	/// </summary>
	/// <param name="other">要拷贝的 Weak_ptr<T> 对象。</param>
	Weak_ptr(const Weak_ptr<T>& other) : 
		_ptr(other._ptr) 
	{
		std::cout << "构造:Weak_ptr(拷贝构造)" << std::endl;
	}

	/// <summary>
	/// 将一个 Shared_ptr<T> 的指针赋值给当前 Weak_ptr<T> 实例
	/// </summary>
	/// <param name="shared_ptr">要赋值的 Shared_ptr<T> 实例</param>
	/// <returns>返回当前赋值后的 Weak_ptr<T> 实例的引用</returns>
	Weak_ptr<T>& operator=(const Shared_ptr<T>& shared_ptr)
	{
		_ptr= shared_ptr.get();
		std::cout << "赋值:Weak_ptr(shared_ptr赋值)" << std::endl;
		return *this;
	}

	/// <summary>
	/// 拷贝赋值
	/// </summary>
	/// <param name="other">要赋值的另一个 Weak_ptr<T> 对象</param>
	/// <returns>返回当前对象的引用（*this）</returns>
	Weak_ptr<T>& operator=(const Weak_ptr<T>& other)
	{
		_ptr = other._ptr;
		std::cout << "赋值:Weak_ptr(拷贝赋值)" << std::endl;
		return *this;
	}

	/// <summary>
	/// 析构函数
	/// </summary>
	~Weak_ptr()
	{
		std::cout << "析构:Weak_ptr" << std::endl;
	}
};