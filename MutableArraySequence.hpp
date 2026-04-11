#ifndef MUTABLEARRAYSEQUENCE_HPP
#define MUTABLEARRAYSEQUENCE_HPP

#include "ArraySequence.hpp"

template<typename T>
class MutableArraySequence : public ArraySequence<T>
{
protected:

    ArraySequence<T>* CreateEmpty() const override;
    ArraySequence<T>* CreateFromArray(const T* items, int count) const override;

    ArraySequence<T>* Clone() const override;
    ArraySequence<T>* Instance() override;

public:

    MutableArraySequence();
    MutableArraySequence(const T* items, int count);
    MutableArraySequence(const MutableArraySequence<T>& other);

};

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::CreateEmpty() const
{
    return new MutableArraySequence<T>();
}

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::CreateFromArray(const T* items, int count) const
{
    return new MutableArraySequence<T>(items, count);
}

template<typename T>
MutableArraySequence<T>::MutableArraySequence() : ArraySequence<T>() { }

template<typename T>
MutableArraySequence<T>::MutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) { }

template<typename T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) { }

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::Clone() const
{
    return new MutableArraySequence<T>(*this);
}

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::Instance()
{
    return this;
}

#endif
