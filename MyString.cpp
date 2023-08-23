#include "MyString.h"

#include <cstring>
#include <utility>

MyString::MyString() : m_size(0), m_isInStack(true) {
    m_str.onstack[0] = '\0';
}
MyString::MyString(const char* str) : m_size(strlen(str)) {
    if (size() < initCap()) {
        m_isInStack = true;
        strcpy(m_str.onstack, str);
    }
    else {
    m_isInStack = false;    
    m_str.str.m_cap = size();
    m_str.str.m_ptr = new char[capacity() + 1];
    strcpy(m_str.str.m_ptr, str);
    }
}
MyString::MyString(const size_t s) : m_size(s) {
    if (size() >= initCap()) {
        m_isInStack = false;
        m_str.str.m_cap = size();
        m_str.str.m_ptr  = new char[capacity() + 1];
    }
    else { m_isInStack = true; }
}
MyString::MyString(const MyString& rhs) : MyString(rhs.size()) {
    if (isInStack()) { 
        strcpy(m_str.onstack, rhs.c_str());
    }
    else { strcpy(m_str.str.m_ptr, rhs.c_str()); }
}
MyString::MyString(MyString&& rhs) : m_size(rhs.size()), m_isInStack(rhs.isInStack()) {
    if (isInStack()) {
        strcpy(m_str.onstack, rhs.c_str());
    }
    else {
        m_str.str.m_cap = rhs.capacity();
        m_str.str.m_ptr = rhs.m_str.str.m_ptr;
        rhs.m_str.str.m_ptr = nullptr;
    }
}
size_t MyString::size() const { return m_size; }
size_t MyString::capacity() const {
    return (isInStack() ? initCap() - 1 : m_str.str.m_cap);
}
size_t MyString::initCap()  { return m_initCap; }
bool MyString::isInStack() const { return m_isInStack; }
void MyString::push_back(const char c) {
    if (size() == capacity() && isInStack()) {
        m_isInStack = false;
        char* tmp = new char[capacity() * 2];
        strcpy(tmp, m_str.onstack);
        tmp[m_size++] = c;
        tmp[size()] = '\0';
        m_str.str.m_cap = initCap() * 2;
        m_str.str.m_ptr = tmp;
    }
    else if (size() == capacity()) {
        m_str.str.m_cap *= 2;
        char* tmp = new char[capacity()];
        strcpy(tmp, m_str.str.m_ptr);
        tmp[m_size++] = c;
        tmp[size()] = '\0';
        delete [] m_str.str.m_ptr;
        m_str.str.m_ptr = tmp;   
    }
    else if (isInStack()) {
        m_str.onstack[m_size++] = c;
        m_str.onstack[size()] = '\0';
    }
    else {
        m_str.str.m_ptr[m_size++] = c;
        m_str.str.m_ptr[size()] = '\0';
    }    
}
void MyString::pop_back() {
     if (isInStack()) {
        m_str.onstack[size() - 1] = '\0';
        --m_size;
     }
     else {
        m_str.str.m_ptr[size() - 1] = '\0';
        --m_size;
     }
}
const char* MyString::c_str() const {
    return isInStack() ? m_str.onstack : m_str.str.m_ptr;
}
MyString::operator char*() {
    return (char*)static_cast<const MyString&>(*this).operator const char*();
}
MyString::operator const char*() const {
    if (isInStack()) { return m_str.onstack; }
    return m_str.str.m_ptr;
}
const char& MyString::operator[](const size_t i) const {
    return isInStack() ? m_str.onstack[i] : m_str.str.m_ptr[i];
}
char& MyString::operator[](const size_t i) {
    return isInStack() ? m_str.onstack[i] : m_str.str.m_ptr[i];
}
MyString& MyString::operator=(const MyString& rhs) {
    if (this == &rhs) { return *this; }
    m_size = rhs.size();
    if (isInStack() && rhs.isInStack()) {
        strcpy(m_str.onstack, rhs.c_str());
    }
    else if ((isInStack() && !rhs.isInStack()) || (!isInStack() && size() < rhs.size())) {
        if (!isInStack()) { delete [] m_str.str.m_ptr; }
         m_isInStack = false;
         m_str.str.m_cap = rhs.capacity();
         m_str.str.m_ptr = new char[capacity() + 1];
        strcpy(m_str.str.m_ptr, rhs.c_str());
    }
    else {
        strcpy(m_str.str.m_ptr, rhs.c_str());
    }
    return *this;
}
MyString& MyString::operator=(MyString&& rhs) {
    if (this == &rhs) { return *this; }
    m_size = rhs.size();
    if (isInStack() && rhs.isInStack()) {
        strcpy(m_str.onstack, rhs.c_str());
    }
    else if ((isInStack() && !rhs.isInStack()) || (!isInStack() && size() < rhs.size())) {
        if (!isInStack()) { delete [] m_str.str.m_ptr; }
         m_isInStack = false;
         m_str.str.m_cap = rhs.capacity();
         m_str.str.m_ptr = std::exchange(rhs.m_str.str.m_ptr, nullptr);
    }
    else {
        m_str.str.m_cap = rhs.capacity();
        m_str.str.m_ptr = std::exchange(rhs.m_str.str.m_ptr, nullptr);
    }
    return *this;
}
MyString::~MyString() {
    if (!isInStack()) { delete [] m_str.str.m_ptr; }
}
