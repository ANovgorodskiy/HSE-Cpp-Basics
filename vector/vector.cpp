#include "vector.h"
#include <utility>
#include <tuple>
Vector::Vector(size_t size) : size_(size), capacity_(size), data_(new int[size]) {
    for (size_t i = 0; i < size; ++i) {
        data_[i] = 0;
    }
}

Vector::Vector() : size_(0), capacity_(0), data_(nullptr) {
}

Vector::Vector(std::initializer_list<ValueType> list) : Vector(list.size()) {
    size_t index = 0;
    for (const auto& element : list) {
        data_[index] = element;
        ++index;
    }
}

Vector::Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(new int[other.size_]) {
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

Vector::~Vector() {
    delete[] data_;
}

Vector::ValueType& Vector::operator[](size_t position) {
    return data_[position];
}

Vector::ValueType Vector::operator[](size_t position) const {
    return data_[position];
}

Vector::SizeType Vector::Capacity() const {
    return capacity_;
}

Vector::SizeType Vector::Size() const {
    return size_;
}
void Vector::Clear() {
    size_ = 0;
}

void Vector::PopBack() {
    --size_;
}

const Vector::ValueType* Vector::Data() const {
    return data_;
}

void Vector::PushBack(const ValueType& new_element) {
    if (size_ < capacity_) {
        data_[size_] = new_element;
    } else {
        if (size_ != 0) {
            this->Reserve(capacity_ * 2);
            data_[size_] = new_element;
        } else {
            data_ = new int[1];
            data_[0] = new_element;
            capacity_ = 1;
        }
    }
    ++size_;
}

void Vector::Reserve(SizeType new_capacity) {
    if (capacity_ < new_capacity) {
        capacity_ = new_capacity;
        int* new_data = new int[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = std::move(new_data);
    }
}

bool Vector::operator==(const Vector& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }
    return true;
}

bool Vector::operator!=(const Vector& other) const {
    return !(other == *this);
}

std::strong_ordering Vector::operator<=>(const Vector& other) const {
    for (size_t t = 0; t < std::min(size_, other.size_); ++t) {
        if (data_[t] != other.data_[t]) {
            return data_[t] <=> other.data_[t];
        }
    }
    return size_ <=> other.size_;
}

Vector& Vector::operator=(const Vector& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    delete[] data_;
    data_ = new int[other.size_];
    for (size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
    delete[] other.data_;
    return *this;
}

void Vector::Swap(Vector& other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

Vector::Iterator::Iterator() : it_(nullptr) {
}

Vector::Iterator::Iterator(ValueType* pointer) : it_(pointer) {
}

Vector::Iterator& Vector::Iterator::operator=(Iterator other) {
    delete it_;
    it_ = other.it_;
    return *this;
}

Vector::ValueType& Vector::Iterator::operator*() const {
    return *it_;
}

Vector::ValueType* Vector::Iterator::operator->() const {
    return it_;
}

bool Vector::Iterator::operator==(const Iterator& other) const {
    return it_ == other.it_;
}

bool Vector::Iterator::operator!=(const Iterator& other) const {
    return !(it_ == other.it_);
}

Vector::Iterator& Vector::Iterator::operator++() {
    ++it_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    Vector::Iterator old(*this);
    ++it_;
    return old;
}

Vector::DifferenceType Vector::Iterator::operator-(Iterator other) {
    return it_ - other.it_;
}

Vector::Iterator Vector::Iterator::operator+(DifferenceType shift) {
    return Vector::Iterator(it_ + shift);
}

Vector::Iterator& Vector::Iterator::operator+=(DifferenceType shift) {
    it_ += shift;
    return *this;
}

Vector::Iterator& Vector::Iterator::operator-=(DifferenceType shift) {
    it_ -= shift;
    return *this;
}

std::strong_ordering Vector::Iterator::operator<=>(const Iterator& other) const {
    return it_ <=> other.it_;
}

Vector::Iterator& Vector::Iterator::operator--() {
    --it_;
    return *this;
}
Vector::Iterator Vector::Iterator::operator--(int) {
    Vector::Iterator old(*this);
    --it_;
    return old;
}

Vector::Iterator Vector::begin() {
    return Iterator(data_);
}

Vector::Iterator Vector::end() {
    return Iterator(data_ + size_);
}

Vector::Iterator Vector::Begin() {
    return this->begin();
}

Vector::Iterator Vector::End() {
    return this->end();
}