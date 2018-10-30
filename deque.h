#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "algorithm.h"
#include "allocator.h"
#include "uninitialized.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"
namespace mmm {

#define _MMM_DEQUE_BUF_SIZE 512

// template <typename T, typename Ref, typename Ptr> 兼容const_iterator.
// ex: iterator<int,int&,int*> 对应 const_iterator<int,const int&,const int*> hav
//  same ::iterator. so constructor is same.
template <typename T, typename Ref, typename Ptr> struct deque_iterator {

  typedef deque_iterator<T, T &, T *> iterator;
  typedef deque_iterator<T, const T &, const T *> const_iterator;
  typedef T value_type;
  typedef mmm::random_access_iterator_tag iterator_category;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef deque_iterator self;
  typedef T **map_pointer;

  pointer cur_;
  pointer first_;
  pointer last_;
  map_pointer map_;

  static inline size_t deque_buf_len() {
    return (sizeof(T) < _MMM_DEQUE_BUF_SIZE
                ? size_t(_MMM_DEQUE_BUF_SIZE / sizeof(T))
                : size_t(1));
  }
  deque_iterator()
      : cur_(nullptr), first_(nullptr), last_(nullptr), map_(nullptr) {}

  deque_iterator(pointer cur, map_pointer m)
      : cur_(cur), first_(*m), last_(*m + deque_buf_len()), map_(m) {}

  deque_iterator(const iterator &other)
      : cur_(other.cur_), first_(other.first_), last_(other.last_),
        map_(other.map_) {}
  reference operator*() const { return *cur_; }

  pointer operator->() const { return cur_; }

  self &operator++() {
    ++cur_;
    if (cur_ == last_) {
      set_map(map_ + 1);
      cur_ = first_;
    }
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self &operator--() {
    if (cur_ == first_) {
      set_map(map_ - 1);
      cur_ = last_;
    }
    --cur_;
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

  self &operator+=(difference_type n) {
    const difference_type offset = n + (cur_ - first_);
    if (offset >= 0 && offset < difference_type(deque_buf_len()))
      cur_ += n;
    else {
      //超过尾部
      const difference_type node_offset =
          offset > 0 ? offset / difference_type(deque_buf_len())
                     : -difference_type((-offset - 1) / deque_buf_len()) -
                           1; // map + i
      set_map(map_ + node_offset);
      cur_ = first_ + (offset - node_offset * difference_type(deque_buf_len()));
    }
    return *this;
  }

  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;
  }

  self &operator-=(difference_type n) { return *this += -n; }

  self operator-(difference_type n) const {
    self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  void set_map(map_pointer new_node) {
    map_ = new_node;
    first_ = *new_node;
    last_ = first_ + difference_type(deque_buf_len());
  }

  void swap(iterator &other) {
    mmm::swap(cur_, other.cur_);
    mmm::swap(first_, other.first_);
    mmm::swap(last_, other.last_);
    mmm::swap(map_, other.map_);
  }
};

template <typename T, typename Ref, typename Ptr>
inline typename deque_iterator<T, Ref, Ptr>::difference_type
operator-(const deque_iterator<T, Ref, Ptr> &x,
          const deque_iterator<T, Ref, Ptr> &y) {
  return typename deque_iterator<T, Ref, Ptr>::difference_type(
             deque_iterator<T, Ref, Ptr>::deque_buf_len()) *
             (x.map_ - y.map_ - 1) +
         (x.cur_ - x.first_) + (y.last_ - y.cur_);
}

template <typename T, typename Ref, typename Ptr>
inline bool operator==(const deque_iterator<T, Ref, Ptr> &x,
                       const deque_iterator<T, Ref, Ptr> &y) {
  return x.cur_ == y.cur_;
}

template <typename T, typename Ref, typename Ptr>
inline bool operator!=(const deque_iterator<T, Ref, Ptr> &x,
                       const deque_iterator<T, Ref, Ptr> &y) {
  return !(x == y);
}

// -------------------------------------------------------------------------
// deque
template <class T, class Alloc = allocator<T>> class deque {
public:
  typedef T value_type;
  typedef deque_iterator<T, T &, T *> iterator;
  typedef deque_iterator<T, const T &, const T *> const_iterator;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Alloc allocator_type;

private:
  typedef Alloc dataAllocator;

private:
  iterator begin_;
  iterator end_;
  size_type map_len;
  T **map_;

public:
  deque();
  explicit deque(size_type n, const value_type &val = value_type()) : deque() {
    deque_aux(n, val, is_integer<size_type>());
  }
  template <class InputIterator>
  deque(InputIterator first, InputIterator last) : deque() {
    deque_aux(first, last, is_integer<InputIterator>());
  }
  deque(const deque &x);

  ~deque();

  iterator begin() { return begin_; }
  iterator end() { return end_; }
  const_iterator begin() const { return begin_; }
  const_iterator end() const { return end_; }
  const_iterator cbegin() const { return begin_; }
  const_iterator cend() const { return end_; }

public:
  size_type size() const { return end() - begin(); }
  bool empty() const { return begin() == end(); }

  reference operator[](size_type n) { return *(begin() + n); }
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }

  reference back() { return *(end() - 1); }

  const_reference back() const { return *(end() - 1); }

  reference operator[](size_type n) const { return *(begin() + n); }

  void push_back(const value_type &val);
  void push_front(const value_type &val);
  void pop_back() {
    --end_;
    mmm::destroy(end_.cur_);
  }
  void pop_front() {
    mmm::destroy(begin_.cur_);
    ++begin_;
  }
  void swap(deque &x);
  void clear();

private:
  static inline size_t deque_buf_len() {
    return (sizeof(T) < _MMM_DEQUE_BUF_SIZE
                ? size_t(_MMM_DEQUE_BUF_SIZE / sizeof(T))
                : size_t(1));
  }

  T **get_new_map(const size_t size);
  size_t get_new_map_size() { return (map_len == 0 ? 2 : map_len * 2); }
  bool is_reach_map_tail() const;
  bool is_reach_map_head() const;
  void deque_aux(size_t n, const value_type &val, true_type);
  template <class Iterator>
  void deque_aux(Iterator first, Iterator last, false_type);
  void enlarge_map();

	void release_map(){
    for (size_t i = 0; i != map_len; ++i) 
        dataAllocator::deallocate(map_[i], deque_buf_len());
    allocator<T*>::deallocate(map_,map_len);
	}
}; // end of deque

template <class T, class Alloc>
T **deque<T, Alloc>::get_new_map(const size_t size) {
  // T **map = new T *[size];
  T **map = allocator<T*>::allocate(size);
  for (size_t i = 0; i != size; ++i)
    map[i] = dataAllocator::allocate(deque_buf_len());
  return map;
}

template <class T, class Alloc> deque<T, Alloc>::deque() : map_len(0), map_(0) {
  map_len = 2; //先申请两个段,便于中间(push_back,push_front) ---中----
  map_ = get_new_map(map_len);
  begin_.set_map(&map_[map_len - 1]);
  end_.set_map(&map_[map_len - 1]);
  begin_.cur_ = end_.cur_ = map_[map_len - 1];
}

template <class T, class Alloc>
deque<T, Alloc>::deque(const deque &x) : map_len(0), map_(0) {
  map_len = x.map_len;
  map_ = get_new_map(map_len);
  auto offset = typename deque::iterator::difference_type(
                    deque::iterator::deque_buf_len()) *
                    (x.begin().map_ - x.map_ - 1) +
                (x.begin().cur_ - x.begin().first_) +
                (x.begin().last_ - x.begin().cur_);

  begin_.set_map(&map_[map_len - 1]);
  end_.set_map(&map_[map_len - 1]);
  begin_.cur_ = end_.cur_ = map_[map_len - 1];
  begin_ = begin_ + offset;
  end_ = begin_;
  for (auto cur = x.begin(); cur != x.end(); ++cur)
    push_back(*cur);
}

template <class T, class Alloc> void deque<T, Alloc>::enlarge_map() {
  auto newMapSize = get_new_map_size();
  T **newMap = get_new_map(newMapSize);
  size_t startIndex = newMapSize / 4;

  iterator dst;
  dst.set_map(&newMap[startIndex]);

  for (auto cur = begin(); cur != end(); ++cur) 
    *dst = *cur;
  
  auto size = this->size();

  // clear();
  release_map();
  map_len = newMapSize;
  map_ = newMap;
  begin_.set_map(&map_[startIndex]);
  begin_.cur_ = map_[startIndex];
  end_ = begin_ + size;
}
template <class T, class Alloc>
bool deque<T, Alloc>::is_reach_map_tail() const {
  // return map_[map_len] == end().cur_;
  return &map_[map_len] == end().map_;
}
template <class T, class Alloc>
bool deque<T, Alloc>::is_reach_map_head() const {
  return map_[0] == begin().cur_;
}
template <class T, class Alloc>
void deque<T, Alloc>::push_front(const value_type &val) {
  if (is_reach_map_head())
    enlarge_map();
  --begin_;
  mmm::construct(begin_.cur_, val);
}

template <class T, class Alloc>
void deque<T, Alloc>::push_back(const value_type &val) {
  if (is_reach_map_tail()) {
    enlarge_map();
  }
  mmm::construct(end_.cur_, val);
  ++end_;
}

//以迭代器/n个构造deque
template <class T, class Alloc>
void deque<T, Alloc>::deque_aux(size_t n, const value_type &val, true_type) {
  size_t i = 0;
  for (; i != n / 2; ++i)
    push_front(val);
  for (; i != n; ++i)
    push_back(val);
}

template <class T, class Alloc>
template <class Iterator>
void deque<T, Alloc>::deque_aux(Iterator first, Iterator last, false_type) {
  difference_type mid = (last - first) / 2;
  for (auto it = first + mid; it != first - 1; --it)
    push_front(*it);
  for (auto it = first + mid + 1; it != last; ++it)
    push_back(*it);
}
template <class T, class Alloc> void deque<T, Alloc>::clear() {
  for (auto cur = begin(); cur != end(); ++cur) 
    mmm::destroy(cur.cur_);

  // for (size_t i = 0; i != map_len; ++i) {
  //   for (auto p = map_[i]; !p && p != map_[i] + deque_buf_len(); ++p)
  //     mmm::destroy(p);
  // }

  //指向中间
  begin_.set_map(&map_[map_len / 2]);
  end_.set_map(&map_[map_len / 2]);
  begin_.cur_ = end_.cur_ = map_[map_len / 2];
}

template <class T, class Alloc> deque<T, Alloc>::~deque() {
  clear();
  for (size_t i = 0; i != map_len; ++i)
    dataAllocator::deallocate(map_[i], deque_buf_len());

  // for (size_t i = 0; i != map_len; ++i) {
  //   for (auto p = map_[i] + 0; !p && p != map_[i] + deque_buf_len(); ++p)
  //     mmm::destroy(p);
  //   if (map_[i]){
  //     dataAllocator::deallocate(map_[i], deque_buf_len());
  //   }
  // }
  //delete[] map_;
  allocator<T*>::deallocate(map_,map_len);
}

template <class T, class Alloc> void deque<T, Alloc>::swap(deque<T, Alloc> &x) {
  mmm::swap(map_len, x.map_len);
  mmm::swap(map_, x.map_);
  begin_.swap(x.begin_);
  end_.swap(x.end_);
}

template <class T, class Alloc>
bool operator==(const deque<T, Alloc> &x, const deque<T, Alloc> &y) {
  auto cit1 = x.begin(), cit2 = y.begin();
  for (; cit1 != x.end() && cit2 != y.end(); ++cit1, ++cit2) {
    if (*cit1 != *cit2)
      return false;
  }
  if (cit1 == x.end() && cit2 == y.end())
    return true;
  return false;
}
template <class T, class Alloc>
inline bool operator!=(const deque<T, Alloc> &x, const deque<T, Alloc> &y) {
  return !(x == y);
}
template <class T, class Alloc>
inline void swap(deque<T, Alloc> &x, deque<T, Alloc> &y) {
  x.swap(y);
}

template <typename T, typename Alloc>
inline bool operator<(const deque<T, Alloc> &x, const deque<T, Alloc> &y) {
  return mmm::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename Alloc>
inline bool operator>(const deque<T, Alloc> &x, const deque<T, Alloc> &y) {
  return y < x;
}

template <typename T, typename Alloc>
inline bool operator<=(const deque<T, Alloc> &x, const deque<T, Alloc> &y) {
  return !(y < x);
}

template <typename T, typename Alloc>
inline bool operator>=(const deque<T, Alloc> &x, const deque<T, Alloc> &y) {
  return !(x < y);
}

} // namespace mmm

#endif