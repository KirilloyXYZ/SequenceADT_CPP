#ifndef BITSEQUENCE_HPP
#define BITSEQUENCE_HPP

#include "Sequence.hpp"
#include "Bit.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.h"

class BitSequence : public Sequence<Bit>
{
private:
    MutableArraySequence<Bit> bits;

public:
    BitSequence();
    BitSequence(Bit* items, int count);
    BitSequence(const MutableArraySequence<Bit>& bits);
    BitSequence(const BitSequence& other);
    ~BitSequence();

    Bit GetFirst() const override;
    Bit GetLast() const override;
    Bit Get(int index) const override;
    Sequence<Bit>* GetSubsequence(int startIndex, int endIndex) const override;
    int GetLength() const override;

    Sequence<Bit>* Append(Bit item) override;
    Sequence<Bit>* Prepend(Bit item) override;
    Sequence<Bit>* InsertAt(Bit item, int index) override;
    Sequence<Bit>* Concat(Sequence<Bit>* list) const override;

    Sequence<Bit>* Map(Bit (*func)(Bit)) const override;
    Sequence<Bit>* Where(bool (*predicate)(Bit)) const override;
    Bit Reduce(Bit (*func)(Bit, Bit), Bit startValue) const override;

    BitSequence* And(const BitSequence& other) const;
    BitSequence* Or(const BitSequence& other) const;
    BitSequence* Xor(const BitSequence& other) const;
    BitSequence* Not() const;
};


BitSequence::BitSequence() : bits() { }

BitSequence::BitSequence(Bit* items, int count) : bits(items, count) { }

BitSequence::BitSequence(const MutableArraySequence<Bit>& bits) : bits(bits) { }

BitSequence::BitSequence(const BitSequence& other) : bits(other.bits) { }

BitSequence::~BitSequence() = default;

Bit BitSequence::GetFirst() const
{
    return this->bits.GetFirst();
}

Bit BitSequence::GetLast() const
{
    return this->bits.GetLast();
}

Bit BitSequence::Get(int index) const
{
    return this->bits.Get(index);
}

Sequence<Bit>* BitSequence::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex < 0 ||
        startIndex >= this->GetLength() ||
        endIndex >= this->GetLength() ||
        startIndex > endIndex)
    {
        throw IndexOutOfRange("BitSequence::GetSubsequence: index out of range");
    }

    int newSize = endIndex - startIndex + 1;
    Bit* data = new Bit[newSize];

    for (int i = 0; i < newSize; ++i)
    {
        data[i] = this->Get(startIndex + i);
    }

    Sequence<Bit>* result = new BitSequence(data, newSize);
    delete[] data;
    return result;
}

int BitSequence::GetLength() const
{
    return this->bits.GetLength();
}

Sequence<Bit>* BitSequence::Append(Bit item)
{
    this->bits.Append(item);
    return this;
}

Sequence<Bit>* BitSequence::Prepend(Bit item)
{
    this->bits.Prepend(item);
    return this;
}

Sequence<Bit>* BitSequence::InsertAt(Bit item, int index)
{
    this->bits.InsertAt(item, index);
    return this;
}

Sequence<Bit>* BitSequence::Concat(Sequence<Bit>* list) const
{
    if (list == nullptr)
    {
        throw std::invalid_argument("BitSequence::Concat: null list");
    }

    int size1 = this->GetLength();
    int size2 = list->GetLength();

    Bit* data = new Bit[size1 + size2];

    for (int i = 0; i < size1; ++i)
    {
        data[i] = this->Get(i);
    }

    for (int i = 0; i < size2; ++i)
    {
        data[size1 + i] = list->Get(i);
    }

    Sequence<Bit>* result = new BitSequence(data, size1 + size2);
    delete[] data;
    return result;
}

Sequence<Bit>* BitSequence::Map(Bit (*func)(Bit)) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("BitSequence::Map: null function");
    }

    int size = this->GetLength();
    Bit* data = new Bit[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = func(this->Get(i));
    }

    Sequence<Bit>* result = new BitSequence(data, size);
    delete[] data;
    return result;
}

Sequence<Bit>* BitSequence::Where(bool (*predicate)(Bit)) const
{
    if (predicate == nullptr)
    {
        throw std::invalid_argument("BitSequence::Where: null predicate");
    }

    Sequence<Bit>* result = new BitSequence();

    for (int i = 0; i < this->GetLength(); ++i)
    {
        Bit value = this->Get(i);
        if (predicate(value))
        {
            result->Append(value);
        }
    }

    return result;
}

Bit BitSequence::Reduce(Bit (*func)(Bit, Bit), Bit startValue) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("BitSequence::Reduce: null function");
    }

    Bit result = startValue;

    for (int i = 0; i < this->GetLength(); ++i)
    {
        result = func(this->Get(i), result);
    }

    return result;
}

BitSequence* BitSequence::And(const BitSequence& other) const
{
    if (this->GetLength() != other.GetLength())
    {
        throw std::invalid_argument("BitSequence::And: sequences must have equal length");
    }

    int size = this->GetLength();
    Bit* data = new Bit[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = this->Get(i) & other.Get(i);
    }

    BitSequence* result = new BitSequence(data, size);
    delete[] data;
    return result;
}

BitSequence* BitSequence::Or(const BitSequence& other) const
{
    if (this->GetLength() != other.GetLength())
    {
        throw std::invalid_argument("BitSequence::Or: sequences must have equal length");
    }

    int size = this->GetLength();
    Bit* data = new Bit[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = this->Get(i) | other.Get(i);
    }

    BitSequence* result = new BitSequence(data, size);
    delete[] data;
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence& other) const
{
    if (this->GetLength() != other.GetLength())
    {
        throw std::invalid_argument("BitSequence::Xor: sequences must have equal length");
    }

    int size = this->GetLength();
    Bit* data = new Bit[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = this->Get(i) ^ other.Get(i);
    }

    BitSequence* result = new BitSequence(data, size);
    delete[] data;
    return result;
}

BitSequence* BitSequence::Not() const
{
    int size = this->GetLength();
    Bit* data = new Bit[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = ~this->Get(i);
    }

    BitSequence* result = new BitSequence(data, size);
    delete[] data;
    return result;
}

#endif