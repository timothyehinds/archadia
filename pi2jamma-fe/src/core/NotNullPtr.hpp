#pragma once

template<typename T>
class NotNullPtr
{
public:
    NotNullPtr(T& t)
        : m_pT{&t}
    {
    }

    T& get() const
    {
        return *m_pT;
    }
private:
    T* m_pT;
};