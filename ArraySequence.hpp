#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"


template<typename T>
class ArraySequence : public Sequence<T>
{
private:

    DynamicArray<T> items;

protected:

    ArraySequence<T>* AppendInternal(T item);
    ArraySequence<T>* PrependInternal(T item);
    ArraySequence<T>* InsertAtInternal(T item, int index);

    virtual ArraySequence<T>* CreateEmpty() const = 0;
    virtual ArraySequence<T>* CreateFromArray(T* items, int count) const = 0;

public:

    ArraySequence(T* items, int count);
    ArraySequence();
    ArraySequence(const DynamicArray<T>& dynamicArray);
    ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence() = default;

    virtual ArraySequence<T>* Clone() const = 0;
    virtual ArraySequence<T>* Instance() = 0;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    int GetLength() const override;

    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, int index) override;
    Sequence<T>* Concat(Sequence<T>* list) const override;

    Sequence<T>* Map(T (*func)(T)) const override;
    Sequence<T>* Where(bool (*predicate)(T)) const override;
    T Reduce(T (*func)(T, T), T startValue) const override; 
};

template<typename T>
ArraySequence<T>::ArraySequence() : items(0) { }

template<typename T>
ArraySequence<T>::ArraySequence(T* items, int count) : items(items, count) { }

template<typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& dynamicArray) : items(dynamicArray) { }

template<typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) : items(other.items) { }

template<typename T>
T ArraySequence<T>::GetFirst() const 
{
    if(this->items.GetSize() == 0)
    {
        throw IndexOutOfRange("ArraySequence::GetFirst: sequence is empty");
    }

    return this->items.Get(0);
}

template<typename T>
T ArraySequence<T>::GetLast() const
{
    if(this->items.GetSize() == 0)
    {
        throw IndexOutOfRange("ArraySequence::GetLast: sequence is empty");
    }

    return this->items.Get(this->items.GetSize() - 1);
}

template<typename T>
T ArraySequence<T>::Get(int index) const
{
    return this->items.Get(index);
}

template<typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    if(startIndex < 0 || endIndex < 0 || startIndex >= this->items.GetSize() || endIndex >= this->items.GetSize() || startIndex > endIndex)
    {
        throw IndexOutOfRange("ArraySequence::GetSubsequence: index out of range");
    }

    int newSize = endIndex - startIndex + 1;
    T* data = new T[newSize];
    for(int i = 0; i < newSize; ++i)
    {
        data[i] = this->items.Get(i + startIndex);
    }
    Sequence<T>* newSequence = this->CreateFromArray(data, newSize); 
    delete[] data;
    return newSequence;
}

template<typename T>
int ArraySequence<T>::GetLength() const
{
    return this->items.GetSize();
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::AppendInternal(T item)
{
    int oldSize = this->items.GetSize();
    this->items.Resize(oldSize + 1);
    this->items.Set(oldSize, item);
    return this; // тут так как ArraySequence наследник Sequence то this возвращаем указатель на ArraySequence и неявно приводится к типу Sequence<T>* 
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::PrependInternal(T item)
{
    int oldSize = this->items.GetSize();
    this->items.Resize(oldSize + 1);
    for(int i = oldSize; i > 0; --i)
    {
        this->items.Set(i, this->items.Get(i - 1));
    }
    this->items.Set(0, item);
    return this;
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::InsertAtInternal(T item, int index)
{
    int oldSize = this->items.GetSize();

    if (index < 0 || index >= oldSize)
    {
        throw IndexOutOfRange("ArraySequence::InsertAt: index is out of range");
    }

    this->items.Resize(oldSize + 1);
    for(int i = oldSize; i > index; --i)
    {
        this->items.Set(i, this->items.Get(i - 1));
    }
    this->items.Set(index, item);
    return this;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* list) const
{
    if (list == nullptr)
{
    throw std::invalid_argument("ArraySequence::Concat: null list");
}

    int size1 = this->items.GetSize();
    int size2 = list->GetLength();

    T* data = new T[size1 + size2];

    for (int i = 0; i < size1; ++i)
    {
        data[i] = this->items.Get(i);
    }

    for (int i = 0; i < size2; ++i)
    {
        data[size1 + i] = list->Get(i);
    }

    Sequence<T>* result = this->CreateFromArray(data, size1 + size2);
    delete[] data;

    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Map(T (*func)(T)) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("ArraySequence::Map: null function");
    }

    int size = this->GetLength();
    T* data = new T[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = func(this->Get(i));
    }

    Sequence<T>* result = this->CreateFromArray(data, size);
    delete[] data;
    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Where(bool (*predicate)(T)) const
{
    if (predicate == nullptr)
    {
        throw std::invalid_argument("ArraySequence::Where: null predicate");
    }

    Sequence<T>* result = this->CreateEmpty();

    for (int i = 0; i < this->GetLength(); ++i)
    {
        T value = this->Get(i);
        if (predicate(value))
        {
            result->Append(value);
        }
    }

    return result;
}

template<typename T>
T ArraySequence<T>::Reduce(T (*func)(T, T), T startValue) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("ArraySequence::Reduce: null function");
    }

    T result = startValue;

    for (int i = 0; i < this->GetLength(); ++i)
    {
        result = func(this->Get(i), result);
    }

    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Append(T item)
{
    return Instance()->AppendInternal(item);
}

template<typename T>
Sequence<T>* ArraySequence<T>::Prepend(T item)
{
    return Instance()->PrependInternal(item);
}

template<typename T>
Sequence<T>* ArraySequence<T>::InsertAt(T item, int index)
{
    return Instance()->InsertAtInternal(item, index);
}
#endif
