#ifndef _LIST_H_
#define _LIST_H_

#include "allocator.h"
#include "functional.h"
#include "iterator.h"
#include "type_traits.h"
#include "uninitialized.h"

namespace mmm {
namespace Detail {
// the class of node
template <class T> struct list_node {
  T data;
  list_node *prev;
  list_node *next;
  list_node(const T &d, list_node *p, list_node *n)
      : data(d), prev(p), next(n) {}
};
} // namespace Detail

// list迭代器,双向. list_node* ptr.
template <typename T, typename Ref, typename Ptr> 
struct list_iterator {
public:
  typedef ptrdiff_t difference_type;
  typedef mmm::bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef Detail::list_node<T> *node_ptr;
  typedef list_iterator<T,T&,T*> iterator;

  typedef list_iterator<T,Ref,Ptr> self;
  node_ptr p;

public:
  //提供隐式转换
  list_iterator(node_ptr ptr = nullptr) : p(ptr) {}

  list_iterator(const iterator &other) : p(other.p) {}

  // self &operator=(const self &x) {
  //   p = x.p;
  //   return *this;
  // }

  self &operator++() {
    p = p->next;
    return *this;
  }
  self operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }
  self &operator--() {
    p = p->prev;
    return *this;
  }
  self operator--(int) {
    auto res = *this;
    --*this;
    return res;
  }
  reference operator*() { return p->data; }
  pointer operator->() { return &(this->operator*()); }
	bool operator==(const self& right) const  { return p == right.p; }
	bool operator!=(const self& right) const  { return !(p == right.p); }
};

// the class of list
// list由双向循环链表实现,因此数据结构只需要一个指针.
template <class T, class Allocator = allocator<Detail::list_node<T>>>
class list {
public:
  typedef Allocator allocator_type;
  typedef Detail::list_node<T> *node_ptr;
  typedef T value_type;
  typedef list_iterator<T,T&,T*> iterator;
  typedef list_iterator<T,const T&,const T*>const_iterator;
  typedef mmm::reverse_iterator<iterator> reverse_iterator;
	typedef mmm::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef T &reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
  typedef size_t size_type;
  typedef ptrdiff_t					 difference_type;

private:
  //iterator is not for implement, we use raw data 
	// begin = end when init ,see reference
  node_ptr head;

public:
  list();

  explicit list(size_type n, const value_type &val = value_type()){
		listAux(n, val, is_integer<value_type>());
	}

  //stl not support this, same as copy(source.begin(), source.end(), destination);
  template <class InputIterator> list(InputIterator first, InputIterator last){
		listAux(first, last, is_integer<InputIterator>());
	}

  list(const list &other);
  list &operator=(const list &l);
  ~list(){
		erase(begin(), end());
		erase(end());
	}

  bool empty() const { return head->next == head; }
  size_type size() const;

  iterator begin() { //若容器为空，则返回的迭代器将等于end() 。
		return head->next;
	} 
  iterator end() { return head; }

  const_iterator begin() const { ;
		return const_iterator(head->next);
	}
	const_iterator cbegin(){
		return const_iterator(head->next);
	}
  const_iterator end() const { 
		return const_iterator(head);
	}
  reverse_iterator rbegin() { return reverse_iterator(head); }
  reverse_iterator rend() { return reverse_iterator(head->next); }

  //在空容器上对 front/back 的调用是未定义的(cpprefernce)。
	//程序的正确性应是使用者的逻辑正确性.而非库
  reference front() { return *begin(); }
  reference back() { return *(--end()); }

  void push_front(const value_type &val) { insert(begin(), val);}
  void pop_front() {erase(begin());}
  void push_back(const value_type &val) { insert(end(), val);}
  void pop_back() { erase(--end());}
  iterator insert(iterator position, const value_type &val);

  void insert(iterator position, size_type n, const value_type &val){
		insert_aux(position, n, val, is_integer<value_type>());
	}
	
  template <class InputIterator> //声明为模板使得能以其他容器赋值如vector
  void insert(iterator position, InputIterator first, InputIterator last){
		insert_aux(position, first, last, is_integer<InputIterator>());
	}
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void swap(list &x);
  void clear(){
		erase(begin(), end());
	}
  void splice(const_iterator position, list &x);
  void splice(const_iterator position, list &x, const_iterator i);
  void splice(const_iterator position, list &x, const_iterator first, const_iterator last);
  void remove(const value_type &val);
  template <class UnaryPredicate>
  void remove_if(UnaryPredicate pred);
  void unique() { unique(mmm::equal_to<T>()); }

  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred);
  void merge(list &x) { merge(x, less<T>()); }
  template <class Compare> void merge(list &x, Compare comp);
  void sort(){ sort(mmm::less<T>());}
  template <class Compare> void sort(Compare comp);
  void reverse();
  void transfer(const_iterator position, const_iterator first, const_iterator last);

private:
  void listAux(size_type n, const value_type &val, true_type);
  template <class InputIterator>
  void listAux(InputIterator first, InputIterator last, false_type);
  void insert_aux(iterator position, size_type n, const T &val, true_type);
  template <class InputIterator>
  void insert_aux(iterator position, InputIterator first, InputIterator last,
                  false_type);
  node_ptr create_node(const T &val = T());
  void delete_node(node_ptr p);
  const_iterator changeIteratorToConstIterator(iterator &it) const;

public:
  template <class T1, class Allocator1>
  friend void swap(list<T1, Allocator> &x, list<T1, Allocator> &y) {x.swap(y);} 
}; // end of List

//比较操作符
template <class T, class Allocator>
bool operator==(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
  auto begin1 = lhs.begin(), begin2 = rhs.begin();
  auto end1 = lhs.end(), end2 = rhs.end();
  for (; begin1 != end1 && begin2 != end2; ++begin1, ++begin2) {
    if (*begin1 != *begin2)
      return false;
  }
  return (begin1 == end1 && begin2 == end2);
}
template <class T, class Allocator>
bool operator!=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
  return !(lhs == rhs);
}

template <class T, class Allocator>
typename list<T, Allocator>::node_ptr list<T, Allocator>::create_node(const T &val) {
  node_ptr tmp = allocator_type::allocate();
  mmm::construct(&tmp->data, val);
	tmp->next = tmp->prev = nullptr;
  return tmp;
}

//释放结点
template <class T, class Allocator>
void list<T, Allocator>::delete_node(node_ptr p) {
  mmm::destroy(&p->data);
  allocator_type::deallocate(p);
}

//size
template <class T, class Allocator>
typename list<T, Allocator>::size_type list<T, Allocator>::size() const {
  size_type length = 0;
  for (auto h = head->next; h != head; h = h->next)
    ++length;
  return length;
}

//构造函数,析构函数

template <class T, class Allocator> list<T, Allocator>::list() {
  head = create_node(); //dummy
  head->next = head;
  head->prev = head;
}

template <class T, class Allocator>
void list<T, Allocator>::listAux(size_type n, const value_type &val,
                                 true_type) {
  head = create_node();
  head->next = head;
  head->prev = head;
  while (n--)
    push_back(val);
}
template <class T, class Allocator>
template <class InputIterator>
void list<T, Allocator>::listAux(InputIterator first, InputIterator last,
                                 false_type) {
  head = create_node();
  head->next = head;
  head->prev = head;
  auto tmp = first;
  for (; tmp != last; ++tmp)
    push_back(*tmp);
}
template <class T, class Allocator>
list<T, Allocator>::list(const list &other) { //直接初始化
  head = create_node();
  head->next = head;
  head->prev = head;
  for (auto cur = other.begin(); cur != other.end(); ++cur)
    push_back(*cur);
}

// insert
template <class T, class Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::insert(iterator position, const value_type &val) {
  //修改四个指针
  auto tmp = create_node(val);
  tmp->next = position.p;
  tmp->prev = position.p->prev;
  position.p->prev->next = tmp;
  position.p->prev = tmp;
  return tmp;
}

template <class T, class Allocator>
void list<T, Allocator>::insert_aux(iterator position, size_type n, const T &val, true_type) {
  for (auto i = n; i != 0; --i) {
    position = insert(position, val);
  }
}
template <class T, class Allocator>
template <class InputIterator>
void list<T, Allocator>::insert_aux(iterator position, InputIterator first, InputIterator last, false_type) {
  for (--last; first != last; --last) {
    position = insert(position, *last);
  }
  insert(position, *last);
}

// push/pop
template <class T, class Allocator>
list<T, Allocator> &list<T, Allocator>::operator=(const list &l) {
  if (this != &l) {
    list(l).swap(*this); //见友元
  }
  return *this;
}
//返回被删除的的迭代器后面的迭代器
//注意,不负责检查参数的合法性.因为如果erase一个空链表,属于使用者的逻辑错误,而非STL本身.
//没有必要隐藏这种错误. 与cppference一致
template <class T, class Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::erase(iterator position) {
  auto ret = position.p->next;
  position.p->next->prev = position.p->prev;
  position.p->prev->next = position.p->next;
  delete_node(position.p);
  return ret;
}
template <class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::erase(iterator first,
                                                                iterator last) {
  typename list<T, Allocator>::iterator ret;
  for (; first != last;) {
    auto temp = first++;//微妙的,删除之前先递进
    ret = erase(temp);
  }
  return ret;
}
template <class T, class Allocator>
void list<T, Allocator>::remove(const value_type &val) {
  for (auto it = begin(); it != end();) {
    if (*it == val)
      it = erase(it);
    else
      ++it;
  }
}
template <class T, class Allocator>
template <class UnaryPredicate>
void list<T, Allocator>::remove_if(UnaryPredicate pred) {
  for (auto it = begin(); it != end();) {
    if (pred(*it))
      it = erase(it);
    else
      ++it;
  }
}
template <class T, class Allocator> void list<T, Allocator>::swap(list &x) {
  node_ptr tmp = x.head;
  x.head = this->head;
  this->head = tmp;
}

//将[first,last)之间的元素移动到position之前
template <class T, class Allocator>
void list<T, Allocator>::transfer(const_iterator position, const_iterator first,
                                  const_iterator last) {
  if (last == position)
    return;
  // 前 P 后
  auto last_node = last.p->prev;

  first.p->prev->next = last.p;
  last.p->prev = first.p->prev;

  auto prev_node = position.p->prev;
  prev_node->next = first.p;
  first.p->prev = prev_node;

  last_node->next = position.p;
  position.p->prev = last_node;
}

// splice
template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &x) {
  if (x.empty())
    return;
  transfer(position, x.begin(), x.end());
}


//从另一个list中移动元素 
template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &x, const_iterator i) {
  const_iterator j = i;
  transfer(position, i, ++j);
}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &x, const_iterator first,
                                const_iterator last) {
  if (position == last)
    return;
  transfer(position, first, last);
}

// reverse: 利用transfer逐个插到头部
template <class T, class Allocator> void list<T, Allocator>::reverse() {
  if (size() == 0 || size() == 1)
    return;
  auto start = begin();
  auto stop = end();
  start++;
  while (start != stop) {
    //不支持+操作符,虽然可实现
    auto tmp = start;
    auto next = ++tmp;
		transfer(begin(), start, next);
    start = next;
  }
}

// unique: 从容器移除所有相继的重复元素只保留一个

template <class T, class Allocator>
template <class BinaryPredicate>
void list<T, Allocator>::unique(BinaryPredicate binary_pred) {
  auto first = begin();
  auto last = end();
  if (empty())
    return; //空链表
  iterator next = first;
  while (++next != last) {
    if (binary_pred(*first, *next))
      erase(next);
    else
      first = next;
    next = first;
  }
}
// merge 将x与*this合并，两个list必须经过递增排序. 抽扑克牌
template <class T, class Allocator>
template <class Compare>
void list<T, Allocator>::merge(list &x, Compare comp) {
  auto it1 = begin(), it2 = x.begin();
  while (it1 != end() && it2 != x.end()) {
    if (comp(*it2, *it1)) {
      auto temp = it2;
			transfer(it1, temp, ++it2);
    } else
      ++it1;
  }
  if (it1 == end()) {
    splice(it1, x, it2, x.end());
  }
}

//构造一个临时链表. 从头遍历依次插入
template <class T, class Allocator>
template <class Compare>
void list<T, Allocator>::sort(Compare comp) {
  if (empty() || size() == 1)
    return;
  list<T, Allocator> tmp;
  auto q = begin();
  while (!empty()) {
    auto p = tmp.begin();
    while (p != tmp.end() && comp(*p, *q))
      p++;
    tmp.splice(p, *this, q);
    q = begin();
  }
  //将tmp赋给本list
  swap(tmp);
}

} // namespace mmm

#endif