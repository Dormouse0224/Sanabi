#pragma once

template <typename T>
class Ptr
{
private:
	T* m_Asset;

public:
	T* Get() { return m_Asset; }
	T** GetAdressOf() { return	&m_Asset; }

	bool operator == (T* _Asset)
	{
		if (m_Asset == _Asset)
			return true;
		return false;
	}

	bool operator == (const  Ptr<T>& _Other)
	{
		if (m_Asset == _Other.m_Asset)
			return true;
		return false;
	}

	bool operator != (T* _Asset)
	{
		if (m_Asset == _Asset)
			return false;
		return true;
	}

	bool operator != (const  Ptr<T>& _Other)
	{
		if (m_Asset == _Other.m_Asset)
			return false;
		return true;
	}






	Ptr& operator = (T* _Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();

		return *this;
	}

	Ptr& operator = (const Ptr<T>& _Other)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Other.m_Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();

		return *this;
	}

	T* operator ->()
	{
		return m_Asset;
	}


public:
	Ptr()
		: m_Asset(nullptr)
	{}

	Ptr(T* _Asset)
		: m_Asset(_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	Ptr(const Ptr<T>& _Other)
		: m_Asset(_Other.m_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	~Ptr()
	{
		if (nullptr != m_Asset)
			m_Asset->Release();
	}
};
