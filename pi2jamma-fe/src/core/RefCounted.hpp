#pragma once

#include "core/cast.hpp"
#include "debug.hpp"
#include <memory>

class RefCounted
{
public:

	void AddRef();
	void Release();

protected:

	RefCounted() = default;
	virtual ~RefCounted() = default;

private:

	unsigned int mRefCount = 0;

};

inline void RefCounted::AddRef()
{
	mRefCount++;
}

inline void RefCounted::Release()
{
	ASSERT(mRefCount > 0);
	
	mRefCount--;
	
	if(mRefCount > 0) {
		return;
	}

	delete this;
}

template< typename T>
class ref
{
	public:
		ref(T* pT = nullptr);
		ref(const ref<T>& rhs);		

		template<typename RhsType>
		ref(ref<RhsType>&& rhs);

		template<typename RhsType>
		ref(const ref<RhsType>& rhs);

		~ref();
		T* operator->() const;
		T* get() const;

		void abandon() {
			m_pT = nullptr;
		}

		ref& operator=(const ref& rhs);
		ref& operator=(ref&& rhs);

		template<typename NewType>
		ref<NewType> downCast() const;

		explicit operator bool() const
		{
			return nullptr != m_pT;
		}

		T& operator*() const
		{
			ASSERT(m_pT);
			return *m_pT;
		}

	private:

		T* m_pT;
};

template<typename T, typename ...Params>
ref<T> make_ref(Params&&... params) {
	return ref<T>(
		new T(std::forward<Params>(params)...));
}


template<typename T >
ref<T>::ref(T* pT)
	: m_pT(pT)
{
	if(nullptr != m_pT) {
		m_pT->AddRef();
	}
}

template<typename T >
ref<T>::ref(const ref<T>& rhs)
	: m_pT(rhs.m_pT)
{
	if(nullptr != m_pT) {
		m_pT->AddRef();
	}
}

template<typename T >
template<typename RhsType>
ref<T>::ref(ref<RhsType>&& rhs)
	: m_pT(rhs.get())
{
	rhs.abandon();
}

template<typename T >
template<typename RhsType>
ref<T>::ref(const ref<RhsType>& rhs)
	: m_pT(rhs.get())
{
	if(nullptr != m_pT) {
		m_pT->AddRef();
	}
}

template<typename T >
ref<T>::~ref()
{
	if(nullptr != m_pT)
	{
		m_pT->Release();
	}
}

template<typename T>
T* ref<T>::operator->() const
{
	ASSERT(nullptr != m_pT);
	return m_pT;
}

template<typename T>
T* ref<T>::get() const
{
	return m_pT;
}

template<typename T>
ref<T>& ref<T>::operator=(const ref& rhs)
{
	if(nullptr != m_pT)
	{
		m_pT->Release();
	}

	m_pT = rhs.m_pT;

	if(nullptr != m_pT)
	{
		m_pT->AddRef();
	}

	return *this;
}

template<typename T>
ref<T>& ref<T>::operator=(ref&& rhs)
{
	if(nullptr != m_pT)
	{
		m_pT->Release();
	}

	m_pT = rhs.m_pT;
	rhs.m_pT = nullptr;

	return *this;
}

template<typename T>
template<typename NewType>
ref<NewType> ref<T>::downCast() const {
	return ::downCast<NewType*, T*>(m_pT);
}