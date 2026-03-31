#include <cassert>
#include <iostream>
#include <stdexcept>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "Bit.hpp"
#include "BitSequence.hpp"

template<typename Func>
bool expect_any_throw(Func f)
{
    try
    {
        f();
    }
    catch (...)
    {
        return true;
    }
    return false;
}

int square(int x)
{
    return x * x;
}

bool is_even(int x)
{
    return x % 2 == 0;
}

int sum_reduce(int value, int acc)
{
    return value + acc;
}

Bit invert_bit(Bit b)
{
    return ~b;
}

bool is_true_bit(Bit b)
{
    return b.GetValue();
}

Bit xor_reduce(Bit value, Bit acc)
{
    return value ^ acc;
}

void TestDynamicArray()
{
    int data[] = {1, 2, 3};

    DynamicArray<int> arr(data, 3);
    assert(arr.GetSize() == 3);
    assert(arr.Get(0) == 1);
    assert(arr.Get(2) == 3);

    arr.Set(1, 42);
    assert(arr.Get(1) == 42);

    arr.Resize(5);
    assert(arr.GetSize() == 5);
    assert(arr.Get(0) == 1);
    assert(arr.Get(1) == 42);
    assert(arr.Get(2) == 3);

    arr.Resize(2);
    assert(arr.GetSize() == 2);
    assert(arr.Get(0) == 1);
    assert(arr.Get(1) == 42);

    assert(expect_any_throw([&]() { arr.Get(-1); }));
    assert(expect_any_throw([&]() { arr.Get(10); }));
    assert(expect_any_throw([&]() { arr.Set(10, 5); }));
    assert(expect_any_throw([&]() { arr.Resize(-1); }));
}

void TestLinkedList()
{
    int data[] = {10, 20, 30};

    LinkedList<int> list(data, 3);
    assert(list.GetLength() == 3);
    assert(list.GetFirst() == 10);
    assert(list.GetLast() == 30);
    assert(list.Get(1) == 20);

    list.Append(40);
    assert(list.GetLength() == 4);
    assert(list.GetLast() == 40);

    list.Prepend(5);
    assert(list.GetLength() == 5);
    assert(list.GetFirst() == 5);

    // По твоей реализации InsertAt(index==length) запрещён
    list.InsertAt(15, 1);
    assert(list.Get(1) == 15);
    assert(list.GetLength() == 6);

    LinkedList<int>* sub = list.GetSubList(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 15);
    assert(sub->Get(1) == 10);
    assert(sub->Get(2) == 20);
    delete sub;

    int otherData[] = {100, 200};
    LinkedList<int> other(otherData, 2);
    LinkedList<int>* concat = list.Concat(&other);
    assert(concat->GetLength() == list.GetLength() + other.GetLength());
    assert(concat->Get(concat->GetLength() - 2) == 100);
    assert(concat->Get(concat->GetLength() - 1) == 200);
    delete concat;

    LinkedList<int> empty;
    assert(expect_any_throw([&]() { empty.GetFirst(); }));
    assert(expect_any_throw([&]() { empty.GetLast(); }));
    assert(expect_any_throw([&]() { empty.Get(0); }));
    assert(expect_any_throw([&]() { list.Get(-1); }));
    assert(expect_any_throw([&]() { list.Get(100); }));
    assert(expect_any_throw([&]() { list.InsertAt(77, -1); }));
    assert(expect_any_throw([&]() { list.InsertAt(77, list.GetLength()); }));
    assert(expect_any_throw([&]() { list.GetSubList(-1, 1); }));
    assert(expect_any_throw([&]() { list.GetSubList(2, 1); }));
}

void TestMutableArraySequence()
{
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 1);
    assert(seq.GetLast() == 3);
    assert(seq.Get(1) == 2);

    Sequence<int>* same1 = seq.Append(4);
    assert(same1 == &seq);
    assert(seq.GetLength() == 4);
    assert(seq.GetLast() == 4);

    Sequence<int>* same2 = seq.Prepend(0);
    assert(same2 == &seq);
    assert(seq.GetLength() == 5);
    assert(seq.GetFirst() == 0);

    Sequence<int>* same3 = seq.InsertAt(99, 2);
    assert(same3 == &seq);
    assert(seq.Get(2) == 99);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 1);
    assert(sub->Get(1) == 99);
    assert(sub->Get(2) == 2);
    delete sub;

    int otherData[] = {7, 8};
    MutableArraySequence<int> other(otherData, 2);
    Sequence<int>* concat = seq.Concat(&other);
    assert(concat->GetLength() == seq.GetLength() + other.GetLength());
    assert(concat->Get(concat->GetLength() - 2) == 7);
    assert(concat->Get(concat->GetLength() - 1) == 8);
    delete concat;

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
    int manualSum = 0;
    for (int i = 0; i < seq.GetLength(); ++i)
    {
        manualSum += seq.Get(i);
    }
    assert(reduced == manualSum);

    assert(expect_any_throw([&]() { seq.GetSubsequence(-1, 1); }));
    assert(expect_any_throw([&]() { seq.GetSubsequence(0, 100); }));
    assert(expect_any_throw([&]() { seq.InsertAt(5, -1); }));
    assert(expect_any_throw([&]() { seq.InsertAt(5, seq.GetLength()); }));
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

    Sequence<int>* prepended = seq.Prepend(0);
    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 1);
    assert(prepended->GetLength() == 4);
    assert(prepended->GetFirst() == 0);
    delete prepended;

    Sequence<int>* inserted = seq.InsertAt(99, 1);
    assert(seq.Get(1) == 2);
    assert(inserted->Get(1) == 99);
    delete inserted;

    Sequence<int>* mapped = seq.Map(square);
    assert(seq.Get(0) == 1);
    assert(mapped->Get(0) == 1);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 9);
    delete mapped;
}

void TestListSequence()
{
    int data[] = {3, 4, 5};
    ListSequence<int> seq(data, 3);

    assert(seq.GetLength() == 3);
    assert(seq.GetFirst() == 3);
    assert(seq.GetLast() == 5);
    assert(seq.Get(1) == 4);

    seq.Append(6);
    assert(seq.GetLast() == 6);

    seq.Prepend(2);
    assert(seq.GetFirst() == 2);

    seq.InsertAt(99, 2);
    assert(seq.Get(2) == 99);

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
    int manualSum = 0;
    for (int i = 0; i < seq.GetLength(); ++i)
    {
        manualSum += seq.Get(i);
    }
    assert(reduced == manualSum);

    assert(expect_any_throw([&]() { seq.InsertAt(1, -1); }));
    assert(expect_any_throw([&]() { seq.InsertAt(1, seq.GetLength()); }));
}

void TestBit()
{
    Bit zero(false);
    Bit one(true);

    assert((zero & zero).GetValue() == false);
    assert((zero & one).GetValue() == false);
    assert((one & one).GetValue() == true);

    assert((zero | zero).GetValue() == false);
    assert((zero | one).GetValue() == true);
    assert((one | one).GetValue() == true);

    assert((zero ^ zero).GetValue() == false);
    assert((zero ^ one).GetValue() == true);
    assert((one ^ one).GetValue() == false);

    assert((~zero).GetValue() == true);
    assert((~one).GetValue() == false);
}

void TestBitSequence()
{
    Bit data1[] = {Bit(true), Bit(false), Bit(true)};
    Bit data2[] = {Bit(true), Bit(true), Bit(false)};

    BitSequence a(data1, 3);
    BitSequence b(data2, 3);

    assert(a.GetLength() == 3);
    assert(a.GetFirst().GetValue() == true);
    assert(a.GetLast().GetValue() == true);

    a.Append(Bit(false));
    assert(a.GetLength() == 4);
    assert(a.GetLast().GetValue() == false);

    a.Prepend(Bit(true));
    assert(a.GetLength() == 5);
    assert(a.GetFirst().GetValue() == true);

    a.InsertAt(Bit(false), 1);
    assert(a.Get(1).GetValue() == false);

    BitSequence* andSeq = b.And(BitSequence(data1, 3));
    assert(andSeq->Get(0).GetValue() == true);
    assert(andSeq->Get(1).GetValue() == false);
    assert(andSeq->Get(2).GetValue() == false);
    delete andSeq;

    BitSequence* orSeq = b.Or(BitSequence(data1, 3));
    assert(orSeq->Get(0).GetValue() == true);
    assert(orSeq->Get(1).GetValue() == true);
    assert(orSeq->Get(2).GetValue() == true);
    delete orSeq;

    BitSequence* xorSeq = b.Xor(BitSequence(data1, 3));
    assert(xorSeq->Get(0).GetValue() == false);
    assert(xorSeq->Get(1).GetValue() == true);
    assert(xorSeq->Get(2).GetValue() == true);
    delete xorSeq;

    BitSequence* notSeq = b.Not();
    assert(notSeq->Get(0).GetValue() == false);
    assert(notSeq->Get(1).GetValue() == false);
    assert(notSeq->Get(2).GetValue() == true);
    delete notSeq;

    Sequence<Bit>* mapped = b.Map(invert_bit);
    assert(mapped->Get(0).GetValue() == false);
    assert(mapped->Get(1).GetValue() == false);
    assert(mapped->Get(2).GetValue() == true);
    delete mapped;

    Sequence<Bit>* filtered = b.Where(is_true_bit);
    for (int i = 0; i < filtered->GetLength(); ++i)
    {
        assert(filtered->Get(i).GetValue() == true);
    }
    delete filtered;

    Bit reduced = b.Reduce(xor_reduce, Bit(false));
    bool manual = false;
    for (int i = 0; i < b.GetLength(); ++i)
    {
        manual = b.Get(i).GetValue() != manual;
    }
    assert(reduced.GetValue() == manual);

    Bit shortData[] = {Bit(true), Bit(false)};
    BitSequence shortSeq(shortData, 2);
    assert(expect_any_throw([&]() { b.And(shortSeq); }));
    assert(expect_any_throw([&]() { b.Or(shortSeq); }));
    assert(expect_any_throw([&]() { b.Xor(shortSeq); }));
}

    void TestDynamicArrayAssignment()
    {

        int aData[] = {1, 2, 3};
        int bData[] = {9, 8};

        DynamicArray<int> a(aData, 3);
        DynamicArray<int> b(bData, 2);

        b = a;

        assert(b.GetSize() == 3);
        assert(b.Get(0) == 1);
        assert(b.Get(1) == 2);
        assert(b.Get(2) == 3);

        a.Set(0, 100);
        assert(b.Get(0) == 1);
    }

    void TestLinkedListAssignment()
    {

        int aData[] = {1, 2, 3};
        int bData[] = {9, 8};

        LinkedList<int> a(aData, 3);
        LinkedList<int> b(bData, 2);

        b = a;

        assert(b.GetLength() == 3);
        assert(b.Get(0) == 1);
        assert(b.Get(1) == 2);
        assert(b.Get(2) == 3);

        a.Append(4);
        assert(b.GetLength() == 3);

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
    TestDynamicArrayAssignment();
    TestLinkedListAssignment();

    std::cout << "All assert tests passed.\n";
    return 0;
}