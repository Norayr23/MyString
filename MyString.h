#ifndef MYSTRING_H
#define MYSTRING_H

#include <cstddef>

class MyString {
public:
   MyString();
   explicit MyString(const char*);
   explicit MyString(const size_t);
   MyString(const MyString&);
   MyString (MyString&&); 
   MyString& operator=(const MyString&);
   MyString& operator=(MyString&&);
   virtual size_t size() const;
   virtual size_t capacity() const;
   virtual void push_back(const char);
   virtual void pop_back();
   operator char*();
   operator const char*() const;
   virtual const char* c_str() const;
   char& operator[](const size_t);
   const char& operator[](const size_t) const;
   virtual ~MyString();
private:
   size_t m_size; 
   static const size_t m_initCap = 16;
   bool m_isInStack;
   static  size_t initCap();
   virtual bool isInStack() const;
   struct DynStr {
     size_t m_cap;
     char* m_ptr;
   };
   union {
     char onstack[m_initCap];
     DynStr str; 
   } m_str;   
};

#endif
