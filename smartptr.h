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
		//���ô˹��캯��ʱ��Ĭ��������Ϊ1
		m_useCnt = new int(1);
	}

	template<typename T,typename Func>
	SmartPtr<typename T,typename Func>::SmartPtr(const SmartPtr &rhs)
	{
		//ʹ�ø��ƹ��캯���������µĶ�����������1
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
		{//��������Ϊ0ʱ���ͷ�heap�ڴ�
			delete m_useCnt;
			m_func(m_p);
		}
	}

	template<typename T,typename Func>
	T &SmartPtr<typename T,typename Func>::operator *()
	{
		//���ؽ����÷�
		return *m_p;
	}

	template<typename T,typename Func>
	T *SmartPtr<typename T,typename Func>::operator ->()
	{
		//���� ->�������������ʵ�Ķ���ָ��
		return m_p;
	}

	template<typename T,typename Func>
	SmartPtr<typename T,typename Func> &SmartPtr<typename T,typename Func>::operator=(const SmartPtr &anotherPtr)
	{
		if(this==&anotherPtr)
		{//��ֹ�Ը�ֵ
			return *this;
		}
		//ʹ�ø�ֵ�������ԭ����ָ�������������1��
		decreaseRef();
		m_p = anotherPtr.m_p;
		m_useCnt = anotherPtr.m_useCnt;
		++*m_useCnt;//ָ���µĶ�����ָ������������1
		return *this;
	}
	/****
	*
	* ���Ӷ���ɾ��ָ����ࡣ
	* �������()�������()�������������T *p
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