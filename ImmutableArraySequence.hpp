#ifndef IMMUTABLEARRAYSEQUENCE_HPP
#define IMMUTABLEARRAYSEQUENCE_HPP

#include "ArraySequence.hpp"

template<typename T>
class ImmutableArraySequence : public ArraySequence<T>
{
protected:

    ArraySequence<T>* CreateEmpty() const override;
    ArraySequence<T>* CreateFromArray(T* items, int count) const override;

public:
    ImmutableArraySequence();
    ImmutableArraySequence(T* items, int count);
    ImmutableArraySequence(const DynamicArray<T>& dynamicArray);
    ImmutableArraySequence(const ImmutableArraySequence<T>& other);

    ArraySequence<T>* Clone() const override;
    ArraySequence<T>* Instance() override;
};

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::CreateEmpty() const
{
    return new ImmutableArraySequence<T>();
}

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::CreateFromArray(T* items, int count) const
{
    return new ImmutableArraySequence<T>(items, count);
}

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() { }

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) { }

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const DynamicArray<T>& dynamicArray) : ArraySequence<T>(dynamicArray) { }

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) { }

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::Clone() const
{
    return new ImmutableArraySequence<T>(*this);
}

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::Instance()
{
    return this->Clone();
}

#endif