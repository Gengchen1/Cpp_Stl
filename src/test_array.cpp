#include <iostream>
#include "STL_Array.hpp"

extern "C" void cfunc(int *a) {}

template <class T, size_t N>
void iota(Array<T, N> &a) noexcept {
  a.swap(a);
  T count = 0;
  for (auto &ai : a) {
    ai = count++;
  }
}
int main() {
  auto a = Array{2, 1, 0};
  for (auto &ai : a) {
    std::cout << ai << std::endl;
  }
  // cfunc(a.data());  // 兼容C语言函数
  iota(a);
  auto b = Array{1, 2, 3};
  std::swap(a, b);
  // a.fill(42);
  std::cout << "Front: " << a.front() << std::endl;
  std::cout << "Back: " << a.back() << std::endl;
  std::cout << "a[1]: " << a[1] << std::endl;
  // 会去查找对应的begin和end.
  // 相当于 for(auto it = a.begin(), eit = a.end(); it != eit; ++it)
  for (auto &ai : a) {
    std::cout << ai << std::endl;
  }
  return 0;
}