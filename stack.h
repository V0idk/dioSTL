#ifndef _STACK_H_
#define _STACK_H_

#include "deque.h"

namespace mmm {
// class of stack
template <class T, class Container = mmm::deque<T>> class stack {
public:
  typedef typename Container::value_type value_type;
  typedef typename Container::reference reference;
  typedef typename Container::size_type size_type;
  typedef typename Container::const_reference const_reference;

  typedef Container container_type;

private:
  container_type container_;

public:
  explicit stack(const container_type &ctnr = container_type())
      : container_(ctnr) {}

  bool empty() const { return container_.empty(); }
  size_type size() const { return container_.size(); }
  reference top() { return (container_.back()); }
  const_reference top() const { return (container_.back()); }
  void push(const value_type &val) { container_.push_back(val); }
  void pop() { container_.pop_back(); }
  void swap(stack &other) { mmm::swap(container_, other.container_); }

  template <typename T1, typename Container1>
  friend bool operator==(const stack<T1, Container1> &,
                         const stack<T1, Container1> &);

  template <typename T1, typename Container1>
  friend bool operator<(const stack<T1, Container1> &,
                        const stack<T1, Container1> &);
};
template <class T, class Container>
bool operator==(const stack<T, Container> &lhs,
                const stack<T, Container> &rhs) {
  return lhs.container_ == rhs.container_;
}
template <class T, class Container>
bool operator!=(const stack<T, Container> &lhs,
                const stack<T, Container> &rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Container>
inline bool operator<(const stack<T, Container> &x,
                      const stack<T, Container> &y) {
  return x.c < y.c;
}

template <typename T, typename Container>
inline bool operator>(const stack<T, Container> &x,
                      const stack<T, Container> &y) {
  return y < x;
}

template <typename T, typename Container>
inline bool operator<=(const stack<T, Container> &x,
                       const stack<T, Container> &y) {
  return !(y < x);
}

template <typename T, typename Container>
inline bool operator>=(const stack<T, Container> &x,
                       const stack<T, Container> &y) {
  return !(x < y);
}

template <class T, class Container>
void swap(stack<T, Container> &x, stack<T, Container> &y) {
  x.swap(y);
}

} // namespace mmm
#endif