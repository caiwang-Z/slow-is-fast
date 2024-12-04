// 1. explicit: In C++, if a constructor has only one argument and is not marked explicit, the compiler will allow
// implicit type conversion, which allows you to initialize the object directly with the value of the type of that
// argument without explicitly calling the constructor.This can sometimes lead to unexpected errors.
// The explicit keyword is used for constructors or conversion functions to prevent them from being called
// implicitly.Its purpose is to avoid implicit type conversions, reduce the number of automatic and potentially
// incorrect type conversion operations performed by the compiler, and increase the readability and safety of your
// code.You should use explicit when you want to be able to create objects only by explicitly calling the constructor.

// 2. std::flush

// 3. stoi