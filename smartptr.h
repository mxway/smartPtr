#ifndef SMARTPTR_INCLUDE_H
#define SMARTPTR_INCLUDE_H
#include <iostream>
using namespace std;
namespace commutil
{
	template<typename T>
	class DefaultFunc;
	template<typename T,typename Func=DefaultFunc<T> >
	class SmartPtr
	{
	public:
		SmartPtr(T *p = 0);
		SmartPtr(const SmartPtr &);
		SmartPtr &operator=(const SmartPtr &);
		~SmartPtr();
		T &operator *();
		T *operator ->();
	private:
		void decreaseRef();
		T *m_p;
		int *m_useCnt;
		DefaultFunc<T> m_func;
		
	};
	template<typename T,typename Func>
	SmartPtr<typename T,typename Func>::SmartPtr(T *p=0):m_p(p)
	{
		//调用此构造函数时，默认引用数为1
		m_useCnt = new int(1);
	}

	template<typename T,typename Func>
	SmartPtr<typename T,typename Func>::SmartPtr(const SmartPtr &rhs)
	{
		//使用复制构造函数，创建新的对象，引用数加1
		this->m_p = rhs.m_p;
		this->m_useCnt = rhs.m_useCnt;
		(*m_useCnt)++;
	}

	template<typename T,typename Func>
	SmartPtr<typename T,typename Func>::~SmartPtr()
	{
		decreaseRef();
	}

	template<typename T,typename Func>
	void SmartPtr<typename T,typename Func>::decreaseRef()
	{
		if(--(*m_useCnt)==0)
		{//当引用数为0时，释放heap内存
			delete m_useCnt;
			m_func(m_p);
		}
	}

	template<typename T,typename Func>
	T &SmartPtr<typename T,typename Func>::operator *()
	{
		//重载解引用符
		return *m_p;
	}

	template<typename T,typename Func>
	T *SmartPtr<typename T,typename Func>::operator ->()
	{
		//重载 ->运算符，返回真实的对象指针
		return m_p;
	}

	template<typename T,typename Func>
	SmartPtr<typename T,typename Func> &SmartPtr<typename T,typename Func>::operator=(const SmartPtr &anotherPtr)
	{
		if(this==&anotherPtr)
		{//防止自赋值
			return *this;
		}
		//使用赋值运算符，原来所指对象的引用数减1。
		decreaseRef();
		m_p = anotherPtr.m_p;
		m_useCnt = anotherPtr.m_useCnt;
		++*m_useCnt;//指向新的对象，所指对象引用数加1
		return *this;
	}
	/****
	*
	* 增加定义删除指针的类。
	* 重载类的()运算符，()运算符参数传入T *p
	*
	****/
	template<typename T>
	class DefaultFunc
	{
	public:
		void operator ()(const T *p);
	};
	template<typename T>
	void DefaultFunc<T>::operator ()(const T *p)
	{
		delete p;
	}
}
#endif