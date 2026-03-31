#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"

template<typename T>
class ListSequence : public Sequence<T>
{
private:
    LinkedList<T> items;

public:
    ListSequence(T* items, int count);
    ListSequence();
    ListSequence(const LinkedList<T>& list);
    ListSequence(const ListSequence<T>& other);
    ~ListSequence() override = default;

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
ListSequence<T>::ListSequence() : items() { }

template<typename T>
ListSequence<T>::ListSequence(T* items, int count) : items(items, count) { }

template<typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& list) : items(list) { }

template<typename T>
ListSequence<T>::ListSequence(const ListSequence<T>& other) : items(other.items) { }

template<typename T>
T ListSequence<T>::GetFirst() const
{
    return this->items.GetFirst();
}

template<typename T>
T ListSequence<T>::GetLast() const
{
    return this->items.GetLast();
}

template<typename T>
T ListSequence<T>::Get(int index) const
{
    return this->items.Get(index);
}

template<typename T>
int ListSequence<T>::GetLength() const
{
    return this->items.GetLength();
}

template<typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    LinkedList<T>* subList = this->items.GetSubList(startIndex, endIndex);
    Sequence<T>* result = new ListSequence<T>(*subList);
    delete subList;
    return result;
}

template<typename T>
Sequence<T>* ListSequence<T>::Append(T item)
{
    this->items.Append(item);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::Prepend(T item)
{
    this->items.Prepend(item);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::InsertAt(T item, int index)
{
    this->items.InsertAt(item, index);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* list) const
{
    if (list == nullptr)
    {
        throw std::invalid_argument("ListSequence::Concat: null list");
    }
    Sequence<T>* newList = new ListSequence<T>(this->items);
    for (int i = 0; i < list->GetLength(); ++i)
    {
        newList->Append(list->Get(i));
    }

    return newList;
}

template<typename T>
Sequence<T>* ListSequence<T>::Map(T (*func)(T)) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("ListSequence::Map: null function");
    }

    int size = this->GetLength();
    T* data = new T[size];

    for(int i = 0; i < size; ++i)
    {
        data[i] = func(this->Get(i));
    }

    Sequence<T>* newSequence = new ListSequence<T>(data, size);
    delete[] data;
    return newSequence;
}

template<typename T>
Sequence<T>* ListSequence<T>::Where(bool (*predicate)(T)) const
{
    if (predicate == nullptr)
    {
        throw std::invalid_argument("ListSequence::Where: null predicate");
    }

    int size = this->GetLength();
    Sequence<T>* newSequence = new ListSequence<T>();

    for(int i = 0; i < size; ++i)
    {
        T value = this->Get(i);
        if(predicate(value))
        {
            newSequence->Append(value);
        }
    }

    return newSequence;
}

template<typename T>
T ListSequence<T>::Reduce(T (*func)(T, T), T startValue) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("ListSequence::Reduce: null function");
    }

    T result = startValue;

    for (int i = 0; i < this->GetLength(); ++i)
    {
        result = func(this->Get(i), result);
    }

    return result;
}

#endif