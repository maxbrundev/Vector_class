#ifndef VECTOR_LIST_PROJECT_VECTOR_H
#define VECTOR_LIST_PROJECT_VECTOR_H

#include <cstdlib>

template <typename T, class allocator>
class Vector {

private:
    size_t _capacity;
    size_t _size;
    T* _data;
    allocator alloc;

public:
    Vector();
    ~Vector();

    Vector(const Vector &other);
    Vector& operator=(const Vector &other);

    size_t capacity()const;
    size_t size()const;

    void push_back(const T &value);
    void reserve(size_t newCapacity);
    void clear();
    void resize(size_t value);

    T& operator[](size_t value) const { return this->_data[value]; }
    int& operator*=(size_t value) const { return  this->_data[value]; }
};

template <typename T, class allocator>
Vector<T, allocator>::Vector() : _capacity {0}, _size {0}, _data {nullptr} {

}

template <typename T, class allocator>
Vector<T,allocator>::~Vector() {
    for (size_t i = 0; i < this->_size; i++)
        alloc.destroy(_data + i);

    alloc.deallocate(_data, _capacity);
}

template <typename T, class allocator>
Vector<T, allocator>::Vector(const Vector &other) {
    this->_data = alloc.allocate(other.capacity());
    this->_size = other.size();
    this->_capacity = other.capacity();

    for (size_t i = 0; i < _size; i++)
        alloc.construct(this->_data + i, *(other._data+i));
}

template <typename T, class allocator>
Vector<T, allocator> &Vector<T, allocator>::operator=(const Vector &other) {
    if(this->_size > 0)
    {
        this->clear();
        alloc.deallocate(_data, _capacity);
    }

    this->_data = alloc.allocate(other.capacity());
    this->_size = other.size();
    this->_capacity = other.capacity();

    for (size_t i = 0; i < _size; i++)
        alloc.construct(this->_data + i, *(other._data+i));

    return *this;
}

template <typename T, class allocator>
size_t Vector<T, allocator>::capacity() const {
    return this->_capacity;
}

template <typename T, class allocator>
size_t Vector<T, allocator>::size() const {
    return this->_size;
}

template <typename T, class allocator>
void Vector<T, allocator>::push_back(const T &value) {
    size_t newCapacity = 0;
    if (this->_size == this->_capacity)
    {
        if (this->_capacity == 0) {
            newCapacity = 1;
        }
        else
        {
            newCapacity = 2* this->_size;
        }
        T *temp = this->alloc.allocate(newCapacity);
        this->alloc.construct(temp + this->_size, value);
        if (this->_capacity > 0) {
            for (size_t i = 0; i < this->_size; i++)
            {
                this->alloc.construct(temp + i, this->_data[i]);
            }
            for (size_t i = 0; i < this->_size; i++)
            {
                this->alloc.destroy(this->_data + i);
            }
            this->alloc.deallocate(this->_data, this->_capacity);
        }
        ++this->_size;
        this->_data = temp;
        this->_capacity = newCapacity;
    }
    else
    {
        this->alloc.construct(this->_data + this->_size, value);
        ++this->_size;
    }
}

template <typename T, class allocator>
void Vector<T, allocator>::reserve(size_t newCapacity) {
    if (newCapacity > this->_capacity)
    {
        T *temp = this->alloc.allocate(newCapacity);
        this->_data = temp;
        this->_capacity = newCapacity;
    }
}

template <typename T, class allocator>
void Vector<T, allocator>::clear() {
    for (size_t i = 0; i < this->_size; i++)
        alloc.destroy(this->_data + i);

    this->_size = 0;
}

template <typename T, class allocator>
void Vector<T, allocator>::resize(size_t value) {

    if (value > this->_capacity)
    {
        this->reserve(value);
        for (size_t i = this->_size; i < value; i++)
        {
            this->alloc.construct(this->_data + i);
        }
        this->_size += (value - this->_size);
    }
    if(value < this->_capacity && value > this->_size)
    {
        for (size_t i = this->_size; i < value; i++)
        {
            this->alloc.construct(this->_data + i);
        }
        this->_size += (value - this->_size);
    }
    if(value < this->_size)
    {
        T *temp = this->alloc.allocate(value);
        if (this->_size != 0)
        {
            for (size_t i = 0; i < value; i++)
            {
                this->alloc.construct(temp + i, this->_data[i]);
            }
            for (size_t i = 0; i < this->_size; i++)
            {
                this->alloc.destroy(this->_data + i);
            }
            this->alloc.deallocate(this->_data, this->_capacity);
        }
        this->_size = value;
        this->_capacity = value;
        this->_data = temp;
    }
}

#endif //VECTOR_LIST_PROJECT_VECTOR_H