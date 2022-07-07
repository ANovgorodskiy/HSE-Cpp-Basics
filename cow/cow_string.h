#include <string>
class CowString;

struct InnerString {
    char* inner;
    size_t counter = 0;
    size_t size_of_repr = 0;
    InnerString(const std::string_view& string_into);
    InnerString();
    ~InnerString();
};
class Proxy {
public:
    Proxy(char* alpha, CowString* cow_inner);
    operator char() const;
    Proxy& operator=(char other);

private:
    CowString* cow_inner_;
    char* alpha_;
};
struct CowIterator {

    CowIterator(CowString* ptr_to_cow, size_t counter);
    Proxy operator*();
    char* operator->();
    CowIterator& operator++();
    CowIterator operator++(int);

    bool operator==(CowIterator& other);
    bool operator!=(CowIterator& other);

private:
    CowString* ptr_to_cow_;
    size_t counter_ = 0;
};
struct ConstCowIterator {
    ConstCowIterator(CowString* ptr_to_cow, size_t counter);

    Proxy operator*() const;
    char* operator->() const;
    ConstCowIterator& operator++();
    ConstCowIterator operator++(int);
    bool operator==(const ConstCowIterator& other) const;
    bool operator!=(const ConstCowIterator& other) const;

private:
    CowString* ptr_to_cow_;
    size_t counter_ = 0;
};
class CowString {
public:
    friend Proxy;

    CowString(const std::string_view& initializer);
    CowString(const CowString& cow);
    CowString(CowString&& cow);
    CowString& operator=(const CowString& other);
    CowString& operator=(CowString&& other);
    ~CowString();
    operator std::string_view();
    char* GetData();
    char* GetData() const;
    Proxy operator[](size_t idx);
    char& At(size_t idx) const;
    CowString& operator+(const std::string_view& other) const;
    CowString& operator+=(const std::string_view& other);
    bool operator==(const std::string_view& other) const;
    bool operator!=(const std::string_view& other) const;

    CowIterator begin();             // NOLINT
    CowIterator end();               // NOLINT
    ConstCowIterator begin() const;  // NOLINT
    ConstCowIterator end() const;    // NOLINT

private:
    InnerString* inner_repr_;
};
