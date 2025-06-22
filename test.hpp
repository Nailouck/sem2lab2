#pragma once

#include <assert.h>
#include <iostream>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"

void DynamicArrayTest() {
    std::cout << "DynamicArray tests: ";
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);

    DynamicArray<int> expected1(3);
    expected1.Set(0, 10);
    expected1.Set(1, 20);
    expected1.Set(2, 30);
    assert(arr == expected1);

    arr.Resize(5);
    assert(arr.GetSize() == 5);
    assert(arr.GetCapacity() == 5);

    arr.Resize(2);
    DynamicArray<int> expected2(2);
    expected2.Set(0, 10);
    expected2.Set(1, 20);
    assert(arr == expected2);

    arr.Remove(1);
    assert(arr.GetSize() == 1);

    std::cout << "all tests were completed successfully.\n";
}

void LinkedListTest() {
    std::cout << "LinkedList tests: ";
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Prepend(0);

    assert(list.GetLength() == 3);
    assert(list.GetFirst() == 0);
    assert(list.GetLast() == 2);
    assert(list.Get(1) == 1);


    list.InsertAt(5, 1);
    assert(list.Get(1) == 5);
    assert(list.Get(2) == 1);

    LinkedList<int>* sub = list.GetSubList(1, 2);
    assert(sub->GetLength() == 2);
    assert(sub->Get(0) == 5);
    assert(sub->Get(1) == 1);

    std::cout << "all tests were completed successfully.\n";
}

void ArraySequenceTest() {
    std::cout << "ArraySequence tests: ";
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Prepend(0);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 0);
    assert(seq.Get(2) == 2);

    seq.InsertAt(5, 1);
    assert(seq.Get(1) == 5);
    assert(seq.Get(2) == 1);

    auto sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 5);

    std::cout << "all tests were completed successfully.\n";
}

void ListSequenceTest() {
    std::cout << "ListSequence tests: ";

    MutableListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Prepend(0);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 0);
    assert(seq.Get(2) == 2);

    seq.InsertAt(5, 1);
    assert(seq.Get(1) == 5);
    assert(seq.Get(2) == 1);

    auto sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 5);

    std::cout << "all tests were completed successfully.\n";
}

void AllTests() {

    DynamicArrayTest();
    LinkedListTest();

    ArraySequenceTest();
    ListSequenceTest();
}