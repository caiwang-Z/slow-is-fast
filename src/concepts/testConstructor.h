#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace TestCopyConstructor {
class String {
  private:
  char*        m_buffer;
  unsigned int m_size;

  public:
  // default constructor
  // String(){};

  // custom constructor
  String(const char* string) {
    m_size   = strlen(string);
    m_buffer = new char[m_size + 1];  // null termination
    memcpy(m_buffer, string, m_size);
    m_buffer[m_size] = 0;
  }

  // copy constructor
  String(const String& other) : m_size(other.m_size) {
    m_buffer = new char[m_size + 1];
    memcpy(m_buffer, other.m_buffer, m_size + 1);
  }
};

class Box {
  public:
  // default constructor
  Box() { std::cout << "default" << std::endl; }

  // custom constructor
  Box(int width, int height, int length) : m_width(width), m_height(height), m_length(length) {
    std::cout << "int,int,int" << std::endl;
  }

  // copy constructor
  Box(Box& other) : m_width(other.m_width), m_height(other.m_height), m_length(other.m_length) {
    std::cout << "copy" << std::endl;
  }

  // move constructor
  Box(Box&& other) : m_width(other.m_width), m_height(other.m_height), m_length(other.m_length) {
    m_contents = std::move(other.m_contents);
    std::cout << "move" << std::endl;
  }
  int  Volume() { return m_width * m_height * m_length; }
  void Add_Item(std::string item) { m_contents.push_back(item); }
  void Print_Contents() {
    for (const auto& item : m_contents) {
      std::cout << item << " ";
    }
  }

  private:
  int                      m_width{0};
  int                      m_height{0};
  int                      m_length{0};
  std::vector<std::string> m_contents;
};

Box get_Box() {
  Box b(5, 10, 18);  // "int,int,int"
  b.Add_Item("Toupee");
  b.Add_Item("Megaphone");
  b.Add_Item("Suit");

  return std::move(b);  // -fno-elide-constructors stop optimization
}

void test() {
  Box b;              // "default"
  Box b1(b);          // "copy"
  Box b2(get_Box());  // "move"
  std::cout << "b2 contents: ";
  b2.Print_Contents();  // Prove that we have all the values
}

}  // namespace TestCopyConstructor

void test() {
  TestCopyConstructor::test();
}