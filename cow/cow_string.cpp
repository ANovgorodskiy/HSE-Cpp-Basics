#include "cow_string.h"
#include <iostream>
Proxy::Proxy(char* alpha, CowString* cow_string) : cow_inner_(cow_string), alpha_(alpha) {
}
Proxy::operator char() const {
    return *alpha_;
}
Proxy& Proxy::operator=(char other) {
    if (*alpha_ != other) {
        if (cow_inner_->inner_repr_->counter > 1) {
            InnerString* new_inner = new InnerString;
            new_inner->inner = new char[cow_inner_->inner_repr_->size_of_repr + 1];
            char tmp = *alpha_;
            *alpha_ = other;
            for (size_t i = 0; i < cow_inner_->inner_repr_->size_of_repr; ++i) {
                new_inner->inner[i] = cow_inner_->inner_repr_->inner[i];
            }
            new_inner->inner[cow_inner_->inner_repr_->size_of_repr] = '\0';
            *alpha_ = tmp;
            --cow_inner_->inner_repr_->counter;
            new_inner->counter = 1;
            new_inner->size_of_repr = cow_inner_->inner_repr_->size_of_repr;
            cow_inner_->inner_repr_ = new_inner;

        } else {
            *alpha_ = other;
        }
    }
    return *this;
}

InnerString::~InnerString() {
    delete[] inner;
}

InnerString::InnerString() = default;
CowIterator::CowIterator(CowString* ptr_to_cow, size_t counter) : ptr_to_cow_(ptr_to_cow), counter_(counter) {
}
CowIterator& CowIterator::operator++() {
    ++counter_;
    return *this;
}
CowIterator CowIterator::operator++(int) {
    CowIterator tmp = *this;
    ++*this;
    return tmp;
}

Proxy CowIterator::operator*() {
    return Proxy(ptr_to_cow_->GetData() + counter_, ptr_to_cow_);
}

char* CowIterator::operator->() {
    return ptr_to_cow_->GetData() + counter_;
}

bool CowIterator::operator!=(CowIterator& other) {
    return (this->ptr_to_cow_->GetData() + this->counter_ != other.ptr_to_cow_->GetData() + other.counter_);
}

bool CowIterator::operator==(CowIterator& other) {
    return !(*this != other);
}

ConstCowIterator::ConstCowIterator(CowString* ptr_to_cow, size_t counter) : ptr_to_cow_(ptr_to_cow), counter_(counter) {}

Proxy ConstCowIterator::operator*() const {
    Proxy const_P(ptr_to_cow_->GetData() + counter_, ptr_to_cow_);

    return const_P;
}

char* ConstCowIterator::operator->() const {
    return ptr_to_cow_->GetData() + counter_;
}

ConstCowIterator& ConstCowIterator::operator++() {
    ++counter_;
    return *this;
}

ConstCowIterator ConstCowIterator::operator++(int) {
    ConstCowIterator tmp = *this;
    ++*this;
    return tmp;
}

bool ConstCowIterator::operator==(const ConstCowIterator& other) const {
    return (this->ptr_to_cow_->GetData() + this->counter_ != other.ptr_to_cow_->GetData() + other.counter_);
}

bool ConstCowIterator::operator!=(const ConstCowIterator& other) const {
    return !(*this == other);
}

InnerString::InnerString(const std::string_view& initializer) {
    counter = 1;
    inner = new char[initializer.size() + 1];
    size_of_repr = initializer.size();
    for (size_t i = 0; i < initializer.size(); ++i) {
        inner[i] = initializer[i];
    }
    inner[size_of_repr] = '\0';
}
CowString::CowString(const std::string_view& initializer) {
    inner_repr_ = new InnerString(initializer);
}

CowString::CowString(const CowString& cow) {
    inner_repr_ = cow.inner_repr_;
    ++inner_repr_->counter;
}

CowString::CowString(CowString&& cow) {
    inner_repr_ = std::move(cow.inner_repr_);
}

CowString::~CowString() {
    if (inner_repr_->counter == 1) {
        delete inner_repr_;
    } else {
        --inner_repr_->counter;
    }
}

char* CowString::GetData() {
    return inner_repr_->inner;
}

char* CowString::GetData() const {
    return inner_repr_->inner;
}

Proxy CowString::operator[](size_t idx) {

    return Proxy(inner_repr_->inner + idx, this);
}

CowString& CowString::operator=(const CowString& other) {
    delete inner_repr_;
    inner_repr_ = other.inner_repr_;
    ++inner_repr_->counter;
    return *this;
}
CowString& CowString::operator=(CowString&& other) {
    inner_repr_->inner = std::move(other.inner_repr_->inner);
    return *this;
}

CowString::operator std::string_view() {
    return std::string_view(inner_repr_->inner);
}

char& CowString::At(size_t idx) const {
    return inner_repr_->inner[idx];
}

CowString& CowString::operator+(const std::string_view& other) const {
    std::string new_string;
    for (size_t i = 0; i < inner_repr_->size_of_repr + other.size() - 1; ++i) {
        if (i < inner_repr_->size_of_repr) {
            new_string += inner_repr_->inner[i];
        } else {
            new_string += other[i - inner_repr_->size_of_repr + 1];
        }
    }
    std::string_view new_string_view = new_string;
    CowString* new_cow = new CowString(new_string_view);
    return *new_cow;
}

CowString& CowString::operator+=(const std::string_view& other) {
    if (other == "") {
        return *this;
    }
    if (inner_repr_->counter > 1) {
        InnerString* new_inner = new InnerString;
        new_inner->inner = new char[inner_repr_->size_of_repr + other.size() + 1];
        new_inner->counter = 1;
        --inner_repr_->counter;
        for (size_t i = 0; i < inner_repr_->size_of_repr + other.size(); ++i) {
            if (i < inner_repr_->size_of_repr) {
                new_inner->inner[i] = inner_repr_->inner[i];
            } else {
                new_inner->inner[i] = other[i - inner_repr_->size_of_repr];
            }
        }
        new_inner->inner[inner_repr_->size_of_repr + other.size()] = '\0';
        new_inner->size_of_repr = inner_repr_->size_of_repr + other.size();
        inner_repr_ = new_inner;
    } else {
        char* tmp = inner_repr_->inner;
        inner_repr_->inner = new char[inner_repr_->size_of_repr + other.size() + 1];
        for (size_t i = 0; i < inner_repr_->size_of_repr + other.size(); ++i) {
            if (i < inner_repr_->size_of_repr) {
                inner_repr_->inner[i] = tmp[i];
            } else {
                inner_repr_->inner[i] = other[i - inner_repr_->size_of_repr];
            }
        }
        inner_repr_->size_of_repr = inner_repr_->size_of_repr + other.size();
        inner_repr_->inner[inner_repr_->size_of_repr + other.size()] = '\0';
    }
    return *this;
}

CowIterator CowString::begin() {

    return CowIterator(this, 0);
}

CowIterator CowString::end() {
    return CowIterator(this, inner_repr_->size_of_repr - 1);
}

bool CowString::operator==(const std::string_view& other) const {
    if (inner_repr_->size_of_repr != other.size()) {
        return false;
    }
    for (size_t i = 0; i < other.size(); ++i) {
        if (this->At(i) != other[i]) {
            return false;
        }
    }
    return true;
}

bool CowString::operator!=(const std::string_view& other) const {
    return !(*this == other);
}
ConstCowIterator CowString::begin() const {

    CowString* pointer = const_cast<CowString*>(this);

    return ConstCowIterator(pointer, 0);
}
ConstCowIterator CowString::end() const {
    CowString* pointer = const_cast<CowString*>(this);
    return ConstCowIterator(pointer, inner_repr_->size_of_repr - 1);
}
