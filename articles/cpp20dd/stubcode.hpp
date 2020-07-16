#include <utility>

#if __has_include(<concepts>)
#include <concepts>
using std::convertible_to;
#else
template <class From, class To>
concept convertible_to =
  std::is_convertible_v<From, To> &&
  requires(std::add_rvalue_reference_t<From> (&f)()) {
    static_cast<To>(f());
  };
#endif