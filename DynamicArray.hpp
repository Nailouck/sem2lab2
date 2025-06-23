#pragma once

#include <stdexcept>
#include "error.hpp"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& arr);
    ~DynamicArray();

    T Get(int index) const;
    int GetSize() const;

    void Remove(int index);

    void Set(int index, T value);
    void Resize(int newSize);
    DynamicArray<T>* GetSubArray(int startIndex, int endIndex) const;


    T& operator[](int index);
    const T& operator[](int index) const;
};

template <class T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    if (count < 0) throw Errors::NegativeSize();

    size = count;

    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = items[i];
}

template <class T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) throw Errors::NegativeSize();

    this->size = size;
    data = new T[size];
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& arr) {
    size = arr.size;
    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = arr.data[i];
}

template <class T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <class T>
T DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();

    return data[index];
}

template <class T>
int DynamicArray<T>::GetSize() const {

    return size;
}

template <class T>
void DynamicArray<T>::Remove(int index) {
    if (size == 0) return;

    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();

    T* newData = new T[size - 1];

    for (int i = 0; i < index; ++i)
        newData[i] = data[i];

    for (int i = index + 1; i < size; ++i)
        newData[i] = data[i];

    delete[] data;
    data = newData;

    size--;
}

template <class T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    data[index] = value;
}

template <class T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0) throw Errors::NegativeSize();

    T* newData = new T[newSize];

    for (int i = 0; i < newSize; ++i)
        newData[i] = data[i];

    delete[] data;
    data = newData;

    size = newSize;
}

template <class T>
DynamicArray<T>* DynamicArray<T>::GetSubArray(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw Errors::InvalidIndices();

    int count = endIndex - startIndex + 1;

    DynamicArray<T>* result = new DynamicArray<T>(count);
    for (int i = 0; i < count; ++i) {
        result->Set(i, data[startIndex + i]);
    }
    return result;

}


template <class T>
T& DynamicArray<T>::operator[](int index) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();

    return data[index];
}

template <class T>
const T& DynamicArray<T>::operator[](int index) const {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();

    return data[index];
}

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) return false;

    for (int i = 0; i < lhs.GetSize(); ++i)

        if (lhs.Get(i) != rhs.Get(i)) return false;

    return true;
}