#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Exceptions.h"

template<typename T>
class DynamicArray
{
private:

    T* items;
    int size;

public:

    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    ~DynamicArray();

    T Get(int index) const;
    int GetSize() const;
    void Set(int index, T value);
    void Resize(int newSize);
};

template<typename T>
DynamicArray<T>::DynamicArray(T* items, int count) 
{
    if (count < 0)
    {
        throw IndexOutOfRange("DynamicArray constructor: count cannot be negative");
    }

    if (items == nullptr && count > 0)
    {
        throw std::invalid_argument("DynamicArray constructor: null items with positive count");
    }

    this->size = count;
    this->items = new T[this->size];
    for(int i = 0; i < this->size; ++i)
    {
        this->items[i] = items[i];
    }
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
    if (this == &other)
    {
        return *this;
    }

    T* newItems = new T[other.size];
    for (int i = 0; i < other.size; ++i)
    {
        newItems[i] = other.items[i];
    }

    delete[] this->items;
    this->items = newItems;
    this->size = other.size;

    return *this;
}

template<typename T>
DynamicArray<T>::DynamicArray(int size)
{
    if (size < 0)
    {
        throw IndexOutOfRange("DynamicArray constructor: size cannot be negative");
    }

    this->size = size;
    this->items = new T[this->size];
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) 
{
    this->size = other.size;
    this->items = new T[this->size];

    for(int i = 0; i < this->size; ++i)
    {
        this->items[i] = other.items[i];
    }
}

template<typename T>
DynamicArray<T>::~DynamicArray() 
{
    delete[] items;
}

template<typename T>
T DynamicArray<T>::Get(int index) const
{
    if (index < 0 || index >= this->size)
    {
        throw IndexOutOfRange("Get: index out of range");
    }

    return this->items[index];
}

template<typename T>
int DynamicArray<T>::GetSize() const
{
    return this->size;
}

template<typename T>
void DynamicArray<T>::Set(int index, T value)
{
    if (index < 0 || index >= this->size)
    {
        throw IndexOutOfRange("Set: index out of range");
    }

    this->items[index] = value;
}

template<typename T>
void DynamicArray<T>::Resize(int newSize) // capasity 
{
    if (newSize < 0)
    {
        throw IndexOutOfRange("Resize: new size cannot be negative");
    }
    
    if(newSize >= this->size)
    {
        T* temp = new T [newSize];

        for(int i = 0; i < this->size; ++i)
        {
            temp[i] = this->items[i];
        }

        this->size = newSize;
        delete[] this->items;
        this->items = temp;
    }
    else
    {
        T* temp = new T [newSize];

        for(int i = 0; i < newSize; ++i)
        {
            temp[i] = this->items[i];
        }

        this->size = newSize;
        delete[] this->items;
        this->items = temp;
    }

}
 
 

#endif 
