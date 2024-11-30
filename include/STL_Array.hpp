#pragma once

#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <iterator> // std::reverse_iterator

// C++标准规定：单下划线+大写字母(_Identifier) 或 双下划线+小写字母(__identifier)的标识符是保留字，理论上用户不得使用。

#if defined(_MSC_VER)
#define _LIBZGCCXX_UNREACHABLE() __assume(0)
#elif defined(__clang__)
#define _LIBZGCCXX_UNREACHABLE() __builtin_unreachable()
#elif defined(__GNUC__)
#define _LIBZGCCXX_UNREACHABLE() __builtin_unreachable()
#else
#define _LIBZGCCXX_UNREACHABLE() \
  do {                            \
  } while (1)
#endif

template <class _Tp, size_t _N>
struct Array {
  using __value_type = _Tp;
  using iterator = _Tp *;
  using const_iterator = _Tp const *;
  using difference_type = std::ptrdiff_t;
  using pointer = _Tp *;
  using const_pointer = _Tp const &;
  using reference = _Tp &;
  using const_reference = _Tp const &;
  using reverse_iterator = std::reverse_iterator<_Tp *>;
  using const_reverse_iterator = std::reverse_iterator<_Tp const *>;

  _Tp _M_elements[_N];
  // size_t 类型可以根据操作系统不同来改变大小，即节省空间又防止空间不够.
  // 加 constexpr 使其编译期求出
  static constexpr size_t size() { return _N; }
  static constexpr size_t max_size() { return _N; }
  // 这里的&引用是为了给数组元素赋值.
  _Tp &operator[](size_t __i) noexcept {
    // 如果传int , 会多发生一步：movzx %eax, %rax ,然后再 lea %rax(%rsp)
    return _M_elements[__i];
  }
  // 要保证 const对象不改变元素的值，这里返回值应该是const.
  // 这里的引用&是为了减少拷贝的开销.
  const _Tp &operator[](size_t __i) const noexcept { return _M_elements[__i]; }

  void swap(Array &__that) noexcept(
      std::is_nothrow_copy_assignable<_Tp>::value) {
    for (size_t __i = 0; __i < _N; __i++) {
      std::swap(_M_elements[__i], __that._M_elements[__i]);
    }
  }

  // noexcept(noexcept(_M_elements[0] = __val)) 表示如果_Tp类型写入无异常,
  // 里层noexcept就返回True, 则整个fill也无异常。
  // 以上写法相当于 noexcpet(std::is_nothrow_copy_assignable<_Tp>)
  void fill(const _Tp &__val) noexcept(
      std::is_nothrow_copy_assignable<_Tp>::value) {
    for (size_t __i = 0; __i < _N; ++__i) {
      _M_elements[__i] = __val;
    }
  }

  _Tp &front() noexcept { return _M_elements[0]; }
  const _Tp &front() const noexcept { return _M_elements[0]; }

  _Tp &back() noexcept { return _M_elements[_N - 1]; }
  const _Tp &back() const noexcept { return _M_elements[_N - 1]; }

  _Tp &at(size_t __i) {
    // 小概率发生的条件表达式，使用 [[unlikely]]修饰，让编译器去优化。
    if (__i >= _N) [[__unlikely__]] {
      throw std::out_of_range("out of range! i = " + std::to_string(__i));
    }
    return _M_elements[__i];
  }
   
  const _Tp &at(size_t __i) const {
    if (__i >= _N) [[__unlikely__]] {
      // throw卸载这里会占用L1i, 指令缓存.[[unlikely]] 会把这些转移到.clod区间。
      throw std::out_of_range("out of range! i = " + std::to_string(__i));
    }
    return _M_elements[__i];
  }
  _Tp const *data() const noexcept { return _M_elements; }
  _Tp *data() noexcept { return _M_elements; }
  _Tp const *cdata() const noexcept { return _M_elements; }

  _Tp *begin() noexcept { return _M_elements; }
  _Tp *end() noexcept { return _M_elements + _N; }

  const _Tp *begin() const noexcept { return _M_elements; }
  const _Tp *end() const noexcept { return _M_elements + _N; }
  const _Tp *cbegin() const noexcept { return _M_elements; }
  const _Tp *cend() const noexcept { return _M_elements + _N; }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }

};


// 0长数组偏特化
template <class _Tp>
struct Array<_Tp, 0> {
  using __value_type = _Tp;
  using iterator = _Tp *;
  using const_iterator = _Tp const *;
  using pointer = _Tp *;
  using const_pointer = _Tp const *;
  using reference = _Tp &;
  using const_reference = _Tp const &;
  using reverse_iterator = _Tp *;
  using const_reverse_iterator = _Tp const *;

  _Tp _M_elements[0];
  static constexpr size_t size() { return 0; }
  static constexpr size_t max_size() { return 0; }
  _Tp &operator[](size_t __i) noexcept { _LIBZGCCXX_UNREACHABLE(); }
  const _Tp &operator[](size_t __i) const noexcept { _LIBZGCCXX_UNREACHABLE(); }

  _Tp &front() noexcept { _LIBZGCCXX_UNREACHABLE(); }
  const _Tp &front() const noexcept { _LIBZGCCXX_UNREACHABLE(); }

  _Tp &back() noexcept { _LIBZGCCXX_UNREACHABLE(); }
  const _Tp &back() const noexcept { _LIBZGCCXX_UNREACHABLE(); }

  _Tp &at(size_t __i) {
    throw std::out_of_range("out of range! __i = " + std::to_string(__i));
  }
  const _Tp &at(size_t __i) const {
    throw std::out_of_range("out of range! __i = " + std::to_string(__i));
  }

  _Tp const *data() const noexcept { return nullptr; }
  _Tp *data() noexcept { return nullptr; }
  _Tp const *cdata() const noexcept { return nullptr; }

  _Tp *begin() noexcept { return nullptr; }
  _Tp *end() noexcept { return nullptr; }
  const _Tp *cbegin() const noexcept { return nullptr; }
  const _Tp *cend() const noexcept { return nullptr; }

  const _Tp *begin() const noexcept { return nullptr; }
  const _Tp *end() const noexcept { return nullptr; }

  void fill(const _Tp &) noexcept {}
  void swap(Array &__that) noexcept {}
};

// 实现 a{1, 2, 3} 来创建数组
template <class _Tp, class ..._Ts>
Array(_Tp, _Ts...) -> Array<_Tp, 1 + sizeof...(_Ts)>;
