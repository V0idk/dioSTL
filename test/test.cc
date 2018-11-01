

#include "../algorithm.h"
#include "../alloc.h"
#include "../allocator.h"
#include "../construct.h"
#include "../deque.h"
#include "../list.h"
#include "../map.h"
#include "../mycstring.h"
#include "../queue.h"
#include "../rbtree.h"
#include "../set.h"
#include "../stack.h"
#include "../uninitialized.h"
#include "../utility.h"
#include "../vector.h"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <vector>

namespace mmm {

// https://stackoverflow.com/questions/34052948/printing-any-stl-container
// template<typename T>
// std::ostream& print(std::ostream &out, T const &val) {
//   return (out << val << " ");
// }

// template<typename T1, typename T2>
// std::ostream& print(std::ostream &out, std::pair<T1, T2> const &val) {
//   return (out << "{" << val.first << " " << val.second << "} ");
// }

// template<template<typename, typename...> class TT, typename... Args>
// std::ostream& operator<<(std::ostream &out, TT<Args...> const &cont) {
//   for(auto&& elem : cont) print(out, elem);
//   return out;
// }

template <class Container1, class Container2>
bool container_equal(Container1 &&con1, Container2 &&con2) {
  auto first1 = mmm::begin(con1), last1 = mmm::end(con1);
  auto first2 = mmm::begin(con2), last2 = mmm::end(con2);
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 != *first2)
      return false;
  }
  return (first1 == last1 && first2 == last2);
}

template <class Container> void print_container(Container &&c) {
  auto first = mmm::begin(c);
  auto last = mmm::end(c);
  for (; first != last; ++first) {
    std::cout << *first << " ";
  }
  std::cout << std::endl;
}

class A {
public:
  int a;
  A(int a_) : a(a_) {}
};
bool operator==(A &a, A &b) { return a.a == b.a; }
bool operator!=(A &a, A &b) { return a.a != b.a; }

namespace algorithmTest {
void testMax() {
  assert(mmm::max(1, 2) == 2);
  assert(mmm::max(2, 1) == 2);
  assert(mmm::max('a', 'z') == 'z');
  assert(mmm::max(3.14, 2.73) == 3.14);
}
void testFillN() {
  std::vector<int> v1(8, 10), v2(8, 10);
  std::fill_n(v1.begin(), 4, 20);
  std::fill_n(v1.begin() + 3, 3, 33);
  mmm::fill_n(v2.begin(), 4, 20);
  mmm::fill_n(v2.begin() + 3, 3, 33);
  assert(mmm::container_equal(v1, v2));
}
void testDistance() {
  mmm::list<int> l(10, 0);
  mmm::vector<int> v(10, 0);

  auto lit = l.begin();
  mmm::advance(lit, 5);
  auto vit = v.begin();
  mmm::advance(vit, 5);

  assert(mmm::distance(l.begin(), lit) == 5);
  assert(mmm::distance(v.begin(), vit) == 5);
}
void testAdvance() {
  mmm::vector<int> v;
  mmm::list<int> l;
  for (auto i = 0; i != 10; ++i) {
    v.push_back(i);
    l.push_back(i);
  }
  auto vit = v.begin();
  auto lit = l.begin();

  mmm::advance(vit, 5);
  mmm::advance(lit, 5);

  mmm::advance(vit, -5);
  mmm::advance(lit, -5);
  assert(*vit == 0 && *lit == 0);
}

void testCopy() {
  char arr1[] = "hello", res1[6] = {0};
  mmm::copy(std::begin(arr1), std::end(arr1), res1);
  assert(mmm::container_equal(arr1, res1));

  wchar_t arr2[] = L"hello", res2[6] = {0};
  mmm::copy(std::begin(arr2), std::end(arr2), res2);
  assert(mmm::container_equal(arr2, res2));

  int arr3[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, res3[10] = {0};
  mmm::copy(std::begin(arr3), std::end(arr3), res3);
  assert(mmm::container_equal(arr3, res3));

  std::string arr4[3] = {"1", "2", "3"}, res4[3];
  mmm::copy(std::begin(arr4), std::end(arr4), res4);
  assert(mmm::container_equal(arr4, res4));
}

void testHeap() {
  int myints[] = {10, 20, 30, 5, 15, 21, 43, 53, 2, 5, 332, 34, 23};
  std::vector<int> v1(myints, myints + 13);
  std::vector<int> v2(myints, myints + 13);
  assert(std::is_heap(v1.begin(), v1.end()) ==
         mmm::is_heap(v1.begin(), v1.end()));

  std::make_heap(v1.begin(), v1.end());
  mmm::make_heap(v2.begin(), v2.end());
  assert(mmm::container_equal(v1, v2));
  assert(std::is_heap(v1.begin(), v1.end()) ==
         mmm::is_heap(v1.begin(), v1.end()));
  std::pop_heap(v1.begin(), v1.end());
  v1.pop_back();
  mmm::pop_heap(v2.begin(), v2.end());
  v2.pop_back();
  assert(mmm::container_equal(v1, v2));
  assert(std::is_heap(v1.begin(), v1.end()) ==
         mmm::is_heap(v1.begin(), v1.end()));
  v1.push_back(99);
  std::push_heap(v1.begin(), v1.end());
  v2.push_back(99);
  mmm::push_heap(v2.begin(), v2.end());
  assert(mmm::container_equal(v1, v2));
  assert(std::is_heap(v1.begin(), v1.end()) ==
         mmm::is_heap(v1.begin(), v1.end()));
  std::sort_heap(v1.begin(), v1.end());
  mmm::sort_heap(v2.begin(), v2.end());
  assert(mmm::container_equal(v1, v2));
}

void testQuicksort() {
  int myints[] = {10, 20, 30, 5, 15, 21, 43, 53, 2, 5, 332, 34, 23};
  mmm::vector<int> v1(myints, myints + 13);
  mmm::vector<int> v2(myints, myints + 13);
  std::sort(v2.begin(), v2.end());
  mmm::quicksort(v1.begin(), v1.end());
  assert(mmm::container_equal(v1, v2));
}

void testbubblesort() {
  int myints[] = {10, 20, 30, 5, 15, 21, 43, 53, 2, 5, 332, 34, 23};
  mmm::vector<int> v1(myints, myints + 13);
  mmm::vector<int> v2(myints, myints + 13);
  std::sort(v2.begin(), v2.end(), std::greater<int>());
  mmm::bubblesort(v1.begin(), v1.end(), mmm::greater<int>());
  assert(mmm::container_equal(v1, v2));
}

void testselectionsort() {
  int myints[] = {10, 20, 30, 5, 15, 21, 43, 53, 2, 5, 332, 34, 23};
  mmm::vector<int> v1(myints, myints + 13);
  mmm::vector<int> v2(myints, myints + 13);
  std::sort(v2.begin(), v2.end(), std::greater<int>());
  mmm::selectionsort(v1.begin(), v1.end(), mmm::greater<int>());
  assert(mmm::container_equal(v1, v2));
}

void testinsertionsort() {
  int myints[] = {10, 20, 30, 5, 15, 21, 43, 53, 2, 5, 332, 34, 23};
  mmm::vector<int> v1(myints, myints + 13);
  mmm::vector<int> v2(myints, myints + 13);
  mmm::insertionsort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  assert(mmm::container_equal(v1, v2));
}

void testAll() {
  testMax();
  testFillN();
  testDistance();
  testAdvance();
  testCopy();
  testHeap();
  testQuicksort();
  testbubblesort();
  testselectionsort();
  testinsertionsort();
}

} // namespace algorithmTest
namespace DequeTest {

void testCase1() {
  std::deque<int> dq1(10, 0);
  mmm::deque<int> dq2(10, 0);
  assert(mmm::container_equal(dq1, dq2));

  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::deque<int> dq3(std::begin(arr), std::end(arr));
  mmm::deque<int> dq4(std::begin(arr), std::end(arr));
  assert(mmm::container_equal(dq3, dq4));

  auto dq5(dq1);
  auto dq6(dq2);
  assert(mmm::container_equal(dq5, dq6));

  auto dq7 = dq3;
  auto dq8 = dq4;
  assert(mmm::container_equal(dq7, dq8));

  auto dq9 = std::move(dq7);
  auto dq10 = std::move(dq8);
  assert(mmm::container_equal(dq9, dq10));

  // test <
  assert((dq1 < dq5) == (dq2 < dq6));
}
void testCase2() {
  mmm::deque<int> dq1;
  assert(dq1.empty());
  assert(dq1.size() == 0);

  mmm::deque<int> dq2(10, 0);
  assert(!dq2.empty());
  assert(dq2.size() == 10);
}
void testCase3() {
  std::deque<std::string> dq1(10, "10");
  mmm::deque<std::string> dq2(10, "10");

  dq1[0] = "0";
  dq1[9] = "9";
  dq2[0] = "0";
  dq2[9] = "9";

  assert(dq1.front() == dq2.front());
  assert(dq1.back() == dq2.back());
}
void testCase4() {
  std::deque<int> dq1;
  mmm::deque<int> dq2;

  for (auto i = 0; i != 10; ++i) {
    dq1.push_back(i);
    dq2.push_back(i);
  }
  assert(mmm::container_equal(dq1, dq2));

  for (auto i = 10; i != 20; ++i) {
    dq1.push_front(i);
    dq2.push_front(i);
  }
  assert(mmm::container_equal(dq1, dq2));

  for (auto i = 0; i != 5; ++i) {
    dq1.pop_back();
    dq2.pop_back();
  }
  assert(mmm::container_equal(dq1, dq2));

  for (auto i = 0; i != 5; ++i) {
    dq1.pop_front();
    dq2.pop_front();
  }
  assert(mmm::container_equal(dq1, dq2));
}
void testCase5() {
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  mmm::deque<int> foo(arr, arr + 3), bar(arr + 3, arr + 10);

  assert(foo.size() == 3 && bar.size() == 7);
  foo.swap(bar);
  assert(foo.size() == 7 && bar.size() == 3);
  mmm::swap(foo, bar);
  assert(foo.size() == 3 && bar.size() == 7);
}
void testCase6() {
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  mmm::deque<int> foo1(arr, arr + 3), bar(arr + 3, arr + 10);

  assert(foo1 != bar);
  auto foo2 = bar;
  assert(foo2 == bar);
}

void testAll() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  testCase6();
}
} // namespace DequeTest
namespace ListTest {
void testCase1() {
  std::list<int> l1(10, 0);
  mmm::list<int> l2(10, 0);
  assert(mmm::container_equal(l1, l2));

  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::list<int> l3(std::begin(arr), std::end(arr));
  mmm::list<int> l4(std::begin(arr), std::end(arr));
  assert(mmm::container_equal(l3, l4));

  auto l5(l1);
  auto l6(l2);
  assert(mmm::container_equal(l5, l6));

  auto l7 = l1;
  auto l8 = l2;
  assert(mmm::container_equal(l7, l8));

  // test const_iteration
  auto tmp = l2.begin();
  *tmp = 2;
  mmm::list<int> l9;
  l9.push_back(1);
  auto temp1 = l9.cbegin();
  temp1++;
  // *temp1 = 2;

  auto tmp3 = l1.cbegin();
  tmp3++;

  // mmm::list<int const> la;
}
void testCase2() {
  mmm::list<std::string> l1;
  assert(l1.empty());
  assert(l1.size() == 0);

  std::string arr[] = {"1", "2", "3"};
  mmm::list<std::string> l2(std::begin(arr), std::end(arr));
  assert(!l2.empty());
  assert(l2.size() == 3);
}
void testCase3() {
  std::string arr[] = {"1", "2", "3"};
  mmm::list<std::string> l(std::begin(arr), std::end(arr));
  assert(l.back() == arr[2]);
  assert(l.front() == arr[0]);

  l.front() = "front";
  l.back() = "back";
  assert(l.back() == "back");
  assert(l.front() == "front");
}
void testCase4() {
  std::list<int> l1;
  mmm::list<int> l2;
  for (auto i = 0; i != 10; ++i) {
    l1.push_front(i);
    l2.push_front(i);
  }
  assert(mmm::container_equal(l1, l2));
  for (auto i = 0; i != 10; ++i) {
    l1.push_back(i);
    l2.push_back(i);
  }
  assert(mmm::container_equal(l1, l2));
  for (auto i = 0; i != 5; ++i) {
    l1.pop_back();
    l2.pop_back();
  }
  assert(mmm::container_equal(l1, l2));
  for (auto i = 0; i != 5; ++i) {
    l1.pop_front();
    l2.pop_front();
  }
  assert(mmm::container_equal(l1, l2));
}
void testCase5() {
  std::list<int> l1;
  mmm::list<int> l2;

  for (auto i = 0; i != 10; ++i) {
    l1.push_back(i);
    l2.push_back(i);
  }
  auto rit1 = l1.rbegin();
  auto rit2 = l2.rbegin();
  for (; rit1 != l1.rend() && rit2 != l2.rend(); ++rit1, ++rit2) {
    assert(*rit1 == *rit2);
  }
  assert(rit1 == l1.rend() && rit2 == l2.rend());
}
void testCase6() {
  std::list<int> l1;
  mmm::list<int> l2;

  l1.insert(l1.end(), 10, -1);
  l2.insert(l2.end(), 10, -1);
  assert(mmm::container_equal(l1, l2));

  auto it1 = l1.begin();
  auto it2 = l2.begin();
  for (auto i = l1.size() / 2 + 1; i != 0; --i) {
    ++it1;
    ++it2;
  }
  l1.insert(it1, 1);
  l2.insert(it2, 1);
  assert(mmm::container_equal(l1, l2));

  int arr[] = {1, 2, 3, 4, 5};
  it1 = l1.end();
  it2 = l2.end();
  l1.insert(it1, std::begin(arr), std::end(arr));
  l2.insert(it2, std::begin(arr), std::end(arr));
  assert(mmm::container_equal(l1, l2));
}
void testCase7() {
  std::list<int> l1;
  mmm::list<int> l2;
  for (auto i = 0; i != 100; ++i) {
    l1.push_back(i);
    l2.push_back(i);
  }

  l1.erase(l1.begin());
  l1.erase(--l1.end());
  l2.erase(l2.begin());
  l2.erase(--l2.end());
  assert(mmm::container_equal(l1, l2));

  l1.erase(l1.begin(), l1.end());
  l2.erase(l2.begin(), l2.end());
  assert(mmm::container_equal(l1, l2));
}
void testCase8() {
  mmm::list<int> l1, l2;
  l1.push_back(1);
  l1.push_back(1);
  l1.push_back(1);
  l2.push_back(2);
  l2.push_back(2);

  l1.swap(l2);
  assert(l1.size() == 2 && l2.size() == 3);
  mmm::swap(l1, l2);
  assert(l1.size() == 3 && l2.size() == 2);
}

void testCase9() {
  std::random_device rd;
  std::list<int> l1;
  mmm::list<int> l2;
  for (auto i = 0; i != 5; ++i) {
    auto ret = rd() % 65536;
    l1.push_back(ret);
    l2.push_back(ret);
  }

  l1.sort();
  l2.sort();
  assert(mmm::container_equal(l1, l2));

  l1.sort(mmm::greater<int>());
  l2.sort(mmm::greater<int>());
  assert(mmm::container_equal(l1, l2));
}
void testCase10() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::list<int> l1(std::begin(arr), std::end(arr));
  mmm::list<int> l2(std::begin(arr), std::end(arr));
  l1.reverse();
  l2.reverse();
  assert(mmm::container_equal(l1, l2));
}
void testCase11() {
  int arr1[] = {0, 1, 3, 5, 9}, arr2[] = {2, 4, 6, 7, 8};
  std::list<double> first1(std::begin(arr1), std::end(arr1)),
      second1(std::begin(arr2), std::end(arr2));
  mmm::list<double> first2(std::begin(arr1), std::end(arr1)),
      second2(std::begin(arr2), std::end(arr2));

  first1.merge(second1);
  first2.merge(second2);
  assert(mmm::container_equal(first1, first2));
}
void testCase12() {
  int arr[] = {0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 11};
  std::list<int> l1(std::begin(arr), std::end(arr));
  mmm::list<int> l2(std::begin(arr), std::end(arr));

  l1.unique();
  l2.unique();
  assert(mmm::container_equal(l1, l2));
}
void testCase13() {
  int arr[] = {17, 89, 7, 14, 89, 0, 1, 4};
  std::list<int> l1(std::begin(arr), std::end(arr));
  mmm::list<int> l2(std::begin(arr), std::end(arr));

  l1.remove(89);
  l2.remove(89);
  assert(mmm::container_equal(l1, l2));

  auto func = [](int n) { return n % 2 == 0; };
  l1.remove_if(func);
  l2.remove_if(func);
  assert(mmm::container_equal(l1, l2));
}
void testCase14() {
  std::list<int> l1(10, 0), l3(10, 1);
  mmm::list<int> l2(10, 0), l4(10, 1);

  l1.splice(l1.begin(), l3);
  l2.splice(l2.begin(), l4);
  assert(mmm::container_equal(l1, l2));

  auto l5 = l1;
  auto l6 = l2;
  l1.splice(l1.end(), l5, l5.begin());
  l2.splice(l2.end(), l6, l6.begin());
  assert(mmm::container_equal(l1, l2));

  auto it1 = l1.begin();
  auto it2 = l2.begin();
  for (size_t i = 0; i != l1.size() / 2; ++i) {
    ++it1;
    ++it2;
  }
  l1.splice(it1, l5, l5.begin(), l5.end());
  l2.splice(it2, l6, l6.begin(), l6.end());
  assert(mmm::container_equal(l1, l2));
}

void testCase15() {
  mmm::list<int> l1(5, 0), l2(5, 1), l3(5, 0);
  assert(mmm::container_equal(l1, l3));
  assert(l1 == l3);
  assert(l1 != l2);

  auto aaa = l1.cbegin();
  auto bbb = l1.begin();
  auto ccc = l1.cbegin();
  auto eee = l1.begin();
  aaa = bbb;
  *bbb = 1;
  aaa = ccc;
  bbb = eee;
  auto fff(bbb);
  // *aaa = 1;
  // bbb = aaa;
}

void testAll() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  testCase6();
  testCase7();
  testCase8();

  testCase9();
  testCase10();
  testCase11();
  testCase12();
  testCase13();
  testCase14();

  testCase15();
}
} // namespace ListTest

namespace PriorityQueueTest {

void testCase1() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, -1, -2, -3};
  std::priority_queue<int> pq1(std::begin(arr), std::end(arr));
  mmm::priority_queue<int> pq2(std::begin(arr), std::end(arr));

  while (!pq1.empty() && !pq2.empty()) {
    assert(pq1.top() == pq2.top());
    pq1.pop();
    pq2.pop();
  }
  assert(pq1.empty() && pq2.empty());
}
void testCase2() {
  mmm::priority_queue<std::string> pq;
  assert(pq.empty());

  pq.push("zzz");
  assert(!pq.empty());
}
void testCase3() {
  mmm::priority_queue<int> pq;
  size_t i = 1;
  for (; i != 10; ++i) {
    pq.push(i);
    assert(pq.size() == i);
  }
  for (i = pq.size(); i != 0; --i) {
    pq.pop();
    assert(pq.size() == (i - 1));
  }
}
void testCase4() {
  std::priority_queue<int> pq1;
  mmm::priority_queue<int> pq2;

  pq1.push(30);
  pq1.push(100);
  pq1.push(25);
  pq1.push(40);

  pq2.push(30);
  pq2.push(100);
  pq2.push(25);
  pq2.push(40);

  while (!pq1.empty() && !pq2.empty()) {
    assert(pq1.top() == pq2.top());
    pq1.pop();
    pq2.pop();
  }
}
void testCase5() {
  mmm::priority_queue<int> foo, bar;
  foo.push(15);
  foo.push(30);
  foo.push(10);
  bar.push(101);
  bar.push(202);
  assert(foo.size() == 3 && bar.size() == 2);

  foo.swap(bar);
  assert(foo.size() == 2 && bar.size() == 3);

  mmm::swap(foo, bar);
  assert(foo.size() == 3 && bar.size() == 2);
}

void testAll() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
}
} // namespace PriorityQueueTest
namespace QueueTest {
void testCase1() {
  std::queue<int> q1;
  mmm::queue<int> q2;

  for (auto i = 0; i != 10; ++i) {
    q1.push(i);
    q2.push(i);
  }
  for (auto i = 0; i != 10; ++i) {
    assert(q1.front() == q2.front());
    q1.pop();
    q2.pop();
  }
}

void testCase3() {
  mmm::queue<int> q;
  assert(q.empty());
  assert(q.size() == 0);

  q.push(10);
  q.push(11);
  assert(!q.empty());
  assert(q.size() == 2);
}
void testCase4() {
  mmm::queue<std::string> q;
  q.push("front");
  q.push("back");

  assert(q.front() == "front");
  assert(q.back() == "back");
}
void testCase5() {
  mmm::queue<int> q1, q2;

  q1.push(1);
  q1.push(2);
  q1.push(3);
  q2.push(1);
  q2.push(2);

  assert(q1.size() == 3 && q2.size() == 2);
  q1.swap(q2);
  assert(q1.size() == 2 && q2.size() == 3);
  mmm::swap(q1, q2);
  assert(q1.size() == 3 && q2.size() == 2);
}

void testAll() {
  testCase1();

  testCase3();
  testCase4();
  testCase5();
}
} // namespace QueueTest
namespace StackTest {
void testCase1() {
  std::stack<int> st1;
  mmm::stack<int> st2;

  for (auto i = 0; i != 10; ++i) {
    st1.push(i);
    st2.push(i);
  }
  for (auto i = 0; i != 10; ++i) {
    assert(st1.top() == st2.top());
    st1.pop();
    st2.pop();
  }
}
void testCase2() {
  mmm::stack<std::string> st;
  assert(st.empty());
  assert(st.size() == 0);

  st.push("one");
  st.push("two");
  assert(!st.empty());
  assert(st.size() == 2);
}
void testCase4() {
  mmm::stack<int> st1, st2;
  st1.push(1);
  st1.push(2);
  st1.push(3);
  st2.push(1);
  st2.push(2);
  assert(st1.size() == 3 && st2.size() == 2);
  st1.swap(st2);
  assert(st1.size() == 2 && st2.size() == 3);
  mmm::swap(st1, st2);
  assert(st1.size() == 3 && st2.size() == 2);
}

void testAll() {
  testCase1();
  testCase2();
  testCase4();
}
} // namespace StackTest
namespace VectorTest {
void testCase1() {
  std::vector<std::string> v1(10, "zzz");
  mmm::vector<std::string> v2(10, "zzz");
  assert(mmm::container_equal(v1, v2));

  std::vector<std::string> v3(10);
  mmm::vector<std::string> v4(10);
  assert(mmm::container_equal(v3, v4));

  std::array<std::string, 3> arr = {{"abc", "def", "ghi"}};
  std::vector<std::string> v5(std::begin(arr), std::end(arr));
  mmm::vector<std::string> v6(std::begin(arr), std::end(arr));
  assert(mmm::container_equal(v5, v6));

  std::vector<int> v7 = {1, 2, 3, 4, 5, 6};
  mmm::vector<int> v8 = {1, 2, 3, 4, 5, 6};
  assert(mmm::container_equal(v7, v8));

  // mmm::vector<const int> v9; not allow
}
void testCase2() {
  std::vector<int> temp1(10, 0);
  mmm::vector<int> temp2(10, 0);

  auto v1(temp1);
  auto v2(temp2);
  assert(mmm::container_equal(v1, v2));

  auto v3(std::move(temp1));
  auto v4(std::move(temp2));
  assert(mmm::container_equal(v3, v4));

  auto v5 = v1;
  auto v6 = v2;
  assert(mmm::container_equal(v5, v6));

  auto v7 = std::move(v3);
  auto v8 = std::move(v4);
  assert(mmm::container_equal(v7, v8));
}
void testCase3() {
  mmm::vector<int> v1, v2;
  for (int i = 0; i != 1000; ++i) {
    v1.push_back(i);
    v2.push_back(i);
  }

  assert(v1 == v2);
  assert(!(v1 != v2));
}
void testCase4() {
  mmm::vector<int> myvector;
  for (int i = 1; i <= 5; i++)
    myvector.push_back(i);

  auto i = 1;
  for (mmm::vector<int>::iterator it = myvector.begin(); it != myvector.end();
       ++it, ++i) {
    assert(*it == i);
  }

  i = 1;
  for (mmm::vector<int>::const_iterator it = myvector.cbegin();
       it != myvector.cend(); ++it, ++i) {
    assert(*it == i);
  }
}
void testCase5() {
  mmm::vector<int> myvector(5);
  int i = 0;
  mmm::vector<int>::reverse_iterator rit = myvector.rbegin();
  for (; rit != myvector.rend(); ++rit)
    *rit = ++i;

  i = 5;
  for (mmm::vector<int>::iterator it = myvector.begin(); it != myvector.end();
       ++it, --i) {
    assert(*it == i);
  }

  i = 1;
  for (mmm::vector<int>::reverse_iterator it = myvector.rbegin();
       it != myvector.rend(); ++it, ++i) {
    assert(*it == i);
  }
}
void testCase6() {
  mmm::vector<int> v(11, 0);
  assert(v.size() == 11);

  v.resize(5);
  assert(v.size() == 5);

  v.resize(20);
  assert(v.size() == 20);
}
void testCase7() {
  mmm::vector<int> v;
  v.reserve(20);
  assert(v.capacity() == 20);
}
void testCase8() {
  std::vector<int> v1(10);
  mmm::vector<int> v2(10);
  for (unsigned i = 0; i < 10; i++) {
    v1[i] = i;
    v2[i] = i;
  }
  assert(mmm::container_equal(v1, v2));

  v1.front() = 99;
  v2.front() = 99;
  v1.back() = 100;
  v2.back() = 100;

  assert(mmm::container_equal(v1, v2));
}
void testCase9() {
  std::vector<int> v1(5);
  mmm::vector<int> v2(5);

  auto p1 = v1.data();
  auto p2 = v2.data();
  *p1 = 10;
  ++p1;
  *p1 = 20;
  p1[2] = 100;
  *p2 = 10;
  ++p2;
  *p2 = 20;
  p2[2] = 100;

  assert(mmm::container_equal(v1, v2));
}
void testCase10() {
  mmm::vector<int> foo(3, 100);
  mmm::vector<int> bar(2, 200);

  std::vector<int> a{100, 100, 100};
  std::vector<int> b{200, 200};
  assert(mmm::container_equal(foo, a));
  assert(mmm::container_equal(bar, b));

  foo.swap(bar);
  std::vector<int> c{100, 100, 100};
  std::vector<int> d{200, 200};
  assert(mmm::container_equal(bar, c));
  assert(mmm::container_equal(foo, d));
}
void testCase11() {
  std::vector<std::string> v1;
  mmm::vector<std::string> v2;

  v1.push_back("hello ");
  v1.push_back("world");
  v2.push_back("hello ");
  v2.push_back("world");
  assert(mmm::container_equal(v1, v2));

  v1.pop_back();
  v2.pop_back();
  assert(mmm::container_equal(v1, v2));
}
void testCase12() {
  std::vector<int> v1;
  mmm::vector<int> v2;

  v1.insert(v1.begin(), 0);
  v2.insert(v2.begin(), 0);
  assert(mmm::container_equal(v1, v2));

  v1.insert(v1.end(), 1);
  v2.insert(v2.end(), 1);
  assert(mmm::container_equal(v1, v2));

  v1.insert(v1.begin() + v1.size() / 2, 10, 0);
  v2.insert(v2.begin() + v2.size() / 2, 10, 0);
  assert(mmm::container_equal(v1, v2));

  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  v1.insert(v1.end(), std::begin(arr), std::end(arr));
  v2.insert(v2.end(), std::begin(arr), std::end(arr));
  assert(mmm::container_equal(v1, v2));

  v1.insert(v1.begin() + 7, 55);
  v2.insert(v2.begin() + 7, 55);
  assert(mmm::container_equal(v1, v2));
  v1.insert(v1.begin() + 5, 55);
  v2.insert(v2.begin() + 5, 55);
  assert(mmm::container_equal(v1, v2));

  std::vector<A> v3;
  mmm::vector<A> v4;
  v3.insert(v3.begin(), A(0));
  v4.insert(v4.begin(), A(0));
  assert(mmm::container_equal(v3, v4));

  v3.insert(v3.begin(), A(1));
  v4.insert(v4.begin(), A(1));
  assert(mmm::container_equal(v3, v4));

  v3.insert(v3.end(), A(2));
  v4.insert(v4.end(), A(2));
  assert(mmm::container_equal(v3, v4));

  v3.insert(v3.end(), A(5));
  v4.insert(v4.end(), A(5));
  assert(mmm::container_equal(v3, v4));

  v3.insert(v3.begin() + 2, A(44));
  v4.insert(v4.begin() + 2, A(44));
  assert(mmm::container_equal(v3, v4));

  A arr1[] = {A(0), A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9)};
  v3.insert(v3.begin() + 5, std::begin(arr), std::end(arr));
  v4.insert(v4.begin() + 5, std::begin(arr), std::end(arr));
  assert(mmm::container_equal(v1, v2));
}
void testCase13() {
  std::vector<int> v1;
  mmm::vector<int> v2;
  for (int i = 1; i <= 10; i++) {
    v1.push_back(i);
    v2.push_back(i);
  }
  v1.erase(v1.begin() + 5);
  v2.erase(v2.begin() + 5);
  assert(mmm::container_equal(v1, v2));

  v1.erase(v1.begin(), v1.begin() + 3);
  v2.erase(v2.begin(), v2.begin() + 3);
  assert(mmm::container_equal(v1, v2));
}
void testCase14() {
  mmm::vector<int> foo(3, 100);
  mmm::vector<int> bar(2, 200);

  assert(!(foo == bar));
  assert(foo != bar);
}

void testAll() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  testCase6();
  testCase7();
  testCase8();
  testCase9();
  testCase10();
  testCase11();
  testCase12();
  testCase13();
  testCase14();
}
} // namespace VectorTest

namespace RbtreeTest {
void testCase1() {
  mmm::less<int> comp;
  // rb_tree<int, int, identity<int>, mmm::less<int>> r1(comp);
  rbtree<int, int, mmm::less<int>, mmm::allocator<rbtree_node<int>>,
         mmm::identity<int>, false, true>
      r1;
  std::set<int, mmm::less<int>> s1(comp);

  std::random_device rd;
  for (auto i = 0; i != 10000; ++i) {
    int ret = rd() % 999999;
    r1.insert(ret);
    s1.insert(ret);
  }
  r1.insert(99);
  s1.insert(99);

  for (auto i = 0; i != 100; ++i) {
    int ret = rd() % 999999;
    if (r1.find(ret) != r1.end())
      r1.erase(r1.find(ret));
    if (s1.find(ret) != s1.end())
      s1.erase(s1.find(ret));
  }

  assert(mmm::container_equal(r1, s1));
}
void testAll() { testCase1(); }

} // namespace RbtreeTest

namespace SetTest {
void testCase1() {
  int ia[5] = {0, 1, 3, 6, 2};
  mmm::set<int> myset(ia, ia + 5);
  std::set<int> stdset(ia, ia + 5);
  myset.insert(4);
  myset.insert(10);
  stdset.insert(4);
  stdset.insert(10);
  assert(mmm::container_equal(myset, stdset));
  auto it1 = myset.find(3);
  auto it2 = stdset.find(3);
  assert(*it1 == *it2);

  std::random_device rd;
  for (auto i = 0; i != 10000; ++i) {
    int ret = rd() % 999999;
    myset.insert(ret);
    stdset.insert(ret);
  }
  myset.insert(99);
  stdset.insert(99);

  for (auto i = 0; i != 100; ++i) {
    int ret = rd() % 999999;
    if (myset.find(ret) != myset.end())
      myset.erase(myset.find(ret));
    if (stdset.find(ret) != stdset.end())
      stdset.erase(stdset.find(ret));
  }
}
void testAll() { testCase1(); }
} // namespace SetTest

namespace MapTest {
using string = std::string;
void testCase1() {
  mmm::map<string, int> mymap;
  mymap["aaa"] = 1;
  mymap["bbb"] = 2;
  mymap["ccc"] = 3;
  const mmm::pair<const string, int> value1(string("ddd"), 4);
  mymap.insert(value1);
  assert(mymap["ddd"] == 4);
  auto iter = mymap.find("ddd");
  assert(*iter == value1);
}
void testAll() { testCase1(); }
} // namespace MapTest

} // namespace mmm

int main() {
  std::cout << "start test" << std::endl;
  mmm::VectorTest::testAll();
  mmm::DequeTest::testAll();
  mmm::PriorityQueueTest::testAll();
  mmm::QueueTest::testAll();
  mmm::StackTest::testAll();
  mmm::ListTest::testAll();
  mmm::algorithmTest::testAll();
  mmm::RbtreeTest::testAll();

  mmm::SetTest::testAll();
  mmm::MapTest::testAll();

  std::cout << "finish test" << std::endl;
}