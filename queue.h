#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "deque.h"
#include "functional.h"
#include "vector.h"

namespace mmm {
// class of queue
template <class T, class Container = mmm::deque<T>> class queue {
public:
  typedef Container container_type;
  typedef typename Container::value_type value_type;
  typedef typename Container::reference reference;
  typedef typename Container::reference const_reference;
  typedef typename Container::size_type size_type;

private:
  Container container_;

public:
  queue() {}
  explicit queue(const container_type &ctnr) : container_(ctnr) {}

  bool empty() const { return container_.empty(); }
  size_type size() const { return container_.size(); }
  reference &front() { return container_.front(); }
  const_reference &front() const { return container_.front(); }
  reference &back() { return container_.back(); }
  const_reference &back() const { return container_.back(); }
  void push(const value_type &val) { container_.push_back(val); }
  void pop() { container_.pop_front(); }
  void swap(queue &x) { container_.swap(x.container_); }

public:
  template <class T1, class Container1>
  friend bool operator==(const queue<T1, Container1> &lhs,
                         const queue<T1, Container1> &rhs);
  template <typename T1, typename Container1>
  friend bool operator<(const queue<T1, Container1> &,
                        const queue<T1, Container1> &);
};

template <class T, class Container>
bool operator==(const queue<T, Container> &lhs,
                const queue<T, Container> &rhs) {
  return lhs.container_ == rhs.container_;
}
template <class T, class Container>
bool operator!=(const queue<T, Container> &lhs,
                const queue<T, Container> &rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Container>
inline bool operator<(const queue<T, Container> &x,
                      const queue<T, Container> &y) {
  return x.c < y.c;
}

template <typename T, typename Container>
inline bool operator>(const queue<T, Container> &x,
                      const queue<T, Container> &y) {
  return y < x;
}

template <typename T, typename Container>
inline bool operator<=(const queue<T, Container> &x,
                       const queue<T, Container> &y) {
  return !(y < x);
}

template <typename T, typename Container>
inline bool operator>=(const queue<T, Container> &x,
                       const queue<T, Container> &y) {
  return !(x < y);
}

template <class T, class Container>
void swap(queue<T, Container> &x, queue<T, Container> &y) {
  x.swap(y);
}

/**************class of priority_queue************************/
//不过是push/pop时进行堆维护而已
//注意默认容器为vector.因为堆算法函数用的是swap,而非修改指针.
template <class T, class Container = mmm::vector<T>,
          class Compare = mmm::less<typename Container::value_type>>
class priority_queue {
public:
  typedef typename Container::value_type value_type;
  typedef Container container_type;
  typedef typename Container::reference reference;
  typedef typename Container::const_reference const_reference;
  typedef typename Container::size_type size_type;

private:
  container_type container_;
  Compare compare_;

public:
  explicit priority_queue(const Compare &comp = Compare(),
                          const Container &ctnr = Container())
      : container_(ctnr), compare_(comp) {}
  template <class InputIterator>
  priority_queue(InputIterator first, InputIterator last,
                 const Compare &comp = Compare(),
                 const Container &ctnr = Container())
      : container_(ctnr), compare_(comp) {
    container_.insert(container_.end(), first, last);
    mmm::make_heap(container_.begin(), container_.end());
  }
  bool empty() const { return container_.empty(); }
  size_type size() const { return container_.size(); }
  reference top() { return container_.front(); }

  void push(const value_type &val) {
    container_.push_back(val);
    mmm::push_heap(container_.begin(), container_.end(), compare_);
  }
  void pop() {
    mmm::pop_heap(container_.begin(), container_.end(), compare_);
    container_.pop_back();
  }
  void swap(priority_queue &x) {
    mmm::swap(container_, x.container_);
    mmm::swap(compare_, x.compare_);
  }
};
template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare> &x,
          priority_queue<T, Container, Compare> &y) {
  x.swap(y);
}
} // namespace mmm

#endif