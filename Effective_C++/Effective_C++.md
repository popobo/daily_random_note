# Introduction
# Item 1: View C++ as a federation of languages
- C: blocks, statements, preprocessor, build-in data types, arrays, pointers
- Object-Oriented C++, C with Classes, constructors, destructors, encapsulation, inheritance, polymorphism, virtual functions(dynamic binding)
- Template C++
- The STL containers, iterators, algorithms, function objects
- Rules for effective C++ programming vary, depending on the part of C++ you are using
# Item 2: Prefer const, enums, and inline to #define
- prefer the compiler to the preprocessor
- #define A 1
  const int B = 1
  A will not be in symbol table, and B is in symbol table
  B uses small code because of no copy
- first special case, defining constant pointers
  const char *const str = "str"; better than const std::string str("str");
- second special concerns class-specific constant.
- #define do not respect scope