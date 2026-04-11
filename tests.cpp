#include <cassert>
#include <cstring>
#include <iostream>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "Bit.hpp"
#include "BitSequence.hpp"

int square(const int& x)
{
    return x * x;
}

bool is_even(const int& x)
{
    return x % 2 == 0;
}

int sum_reduce(const int& value, const int& acc)
{
    return value + acc;
}

Bit invert_bit(const Bit& b)
{
    return ~b;
}

Bit xor_reduce(const Bit& value, const Bit& acc)
{
    return value ^ acc;
}

BitSequence* MakeBitSequence(const char* bits)
{
    int n = static_cast<int>(std::strlen(bits));
    Bit* data = new Bit[n];

    for (int i = 0; i < n; ++i)
    {
        data[i] = Bit(bits[i] == '1');
    }

    BitSequence* seq = new BitSequence(data, n);
    delete[] data;
    return seq;
}

void AssertBits(const Sequence<Bit>& seq, const char* expected)
{
    int n = static_cast<int>(std::strlen(expected));
    assert(seq.GetLength() == n);

    for (int i = 0; i < n; ++i)
    {
        assert(seq.Get(i).GetValue() == (expected[i] == '1'));
    }
}

void TestDynamicArray()
{
    int data[] = {1, 2, 3};
    DynamicArray<int> arr(data, 3);

    assert(arr.GetSize() == 3);
    assert(arr.Get(0) == 1);
    assert(arr.Get(2) == 3);

    arr.Set(1, 10);
    assert(arr.Get(1) == 10);

    arr.Resize(5);
    assert(arr.GetSize() == 5);

    bool thrown = false;
    try
    {
        arr.Get(10);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

void TestLinkedList()
{
    int data[] = {10, 20, 30};
    LinkedList<int> list(data, 3);

    assert(list.GetLength() == 3);
    assert(list.GetFirst() == 10);
    assert(list.GetLast() == 30);

    list.Append(40);
    list.Prepend(5);
    list.InsertAt(15, 1);

    assert(list.GetLength() == 6);
    assert(list.GetFirst() == 5);
    assert(list.Get(1) == 15);
    assert(list.GetLast() == 40);

    bool thrown = false;
    LinkedList<int> empty;
    try
    {
        empty.GetFirst();
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

void TestMutableArraySequence()
{
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int>* same = seq.Append(4);
    assert(same == &seq);
    assert(seq.GetLength() == 4);
    assert(seq.GetLast() == 4);

    seq.Prepend(0);
    seq.InsertAt(99, 2);

    assert(seq.GetFirst() == 0);
    assert(seq.Get(2) == 99);

    int otherData[] = {7, 8};
    MutableArraySequence<int> other(otherData, 2);
    Sequence<int>* concat = seq.Concat(&other);

    assert(concat->GetLength() == seq.GetLength() + other.GetLength());
    assert(concat->Get(concat->GetLength() - 2) == 7);
    assert(concat->Get(concat->GetLength() - 1) == 8);

    delete concat;
}

void TestImmutableArraySequence()
{
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    Sequence<int>* appended = seq.Append(4);

    assert(seq.GetLength() == 3);
    assert(seq.GetLast() == 3);

    assert(appended->GetLength() == 4);
    assert(appended->GetLast() == 4);

    delete appended;
}

void TestListSequence()
{
    int data[] = {3, 4, 5};
    ListSequence<int> seq(data, 3);

    seq.Append(6);
    seq.Prepend(2);
    seq.InsertAt(99, 2);

    assert(seq.GetFirst() == 2);
    assert(seq.Get(2) == 99);
    assert(seq.GetLast() == 6);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    delete sub;

    Sequence<int>* mapped = seq.Map(square);
    assert(mapped->Get(0) == seq.Get(0) * seq.Get(0));
    delete mapped;

    Sequence<int>* filtered = seq.Where(is_even);
    for (int i = 0; i < filtered->GetLength(); ++i)
    {
        assert(filtered->Get(i) % 2 == 0);
    }
    delete filtered;

    int reduced = seq.Reduce(sum_reduce, 0);
    assert(reduced == 119);
}

void TestBit()
{
    Bit zero(false);
    Bit one(true);

    assert((zero & one).GetValue() == false);
    assert((zero | one).GetValue() == true);
    assert((one ^ one).GetValue() == false);
    assert((~zero).GetValue() == true);
}

void TestBitSequence()
{
    BitSequence* a = MakeBitSequence("10110010");
    AssertBits(*a, "10110010");

    a->Append(Bit(true));
    AssertBits(*a, "101100101");
    assert(a->GetLength() == 9);
    assert(a->Get(8).GetValue() == true);

    BitSequence* b = MakeBitSequence("110000111");

    BitSequence* andSeq = a->And(*b);
    AssertBits(*andSeq, "100000101");
    delete andSeq;

    BitSequence* orSeq = a->Or(*b);
    AssertBits(*orSeq, "111100111");
    delete orSeq;

    BitSequence* xorSeq = a->Xor(*b);
    AssertBits(*xorSeq, "011100010");
    delete xorSeq;

    BitSequence* notSeq = a->Not();
    AssertBits(*notSeq, "010011010");
    delete notSeq;

    Bit parity = a->Reduce(xor_reduce, Bit(false));
    assert(parity.GetValue() == true);

    bool thrown = false;
    BitSequence* shortSeq = MakeBitSequence("10");
    try
    {
        a->And(*shortSeq);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    delete shortSeq;
    delete b;
    delete a;
}

int main()
{
    TestDynamicArray();
    TestLinkedList();
    TestMutableArraySequence();
    TestImmutableArraySequence();
    TestListSequence();
    TestBit();
    TestBitSequence();

    std::cout << "All tests passed!\n";
    return 0;
}