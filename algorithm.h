
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "functional.h"
#include "iterator.h"
#include "mycstring.h"
#include "type_traits.h"
#include "utility.h"

namespace mmm {

/***********equal(begin1,end1,begin)****************/

template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2)) {
      return false;
    }
  }
  return true;
}

template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2,
           BinaryPredicate p) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!p(*first1, *first2)) {
      return false;
    }
  }
  return true;
}

/************max(a,b,[compare])********************/
template <class T> const T &max(const T &a, const T &b) {
  return (a < b) ? b : a;
}
template <class T, class Compare>
const T &max(const T &a, const T &b, Compare compare) {
  return (compare(a, b)) ? b : a;
}

/************lexicographical_compare********************/
template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
  for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first1))
      return false;
  }
  return (first1 == last1) && (first2 != last2);
}

template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
  return lexicographical_compare(
      first1, last1, first2, last2,
      mmm::less<typename iterator_traits<InputIt1>::value_type>());
}

// struct _Iter_less_iter
// {
//   template<typename _Iterator1, typename _Iterator2>
//     _GLIBCXX14_CONSTEXPR
//     bool
//     operator()(_Iterator1 __it1, _Iterator2 __it2) const
//     { return *__it1 < *__it2; }
// };

/*************fill_n(it,n,value)*********/
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &value) {
  for (; n > 0; --n, ++first)
    *first = value;
  return first; //返回最后赋值元素后一位置的迭代
}

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T &value) {
  for (; first != last; ++first)
    *first = value;
}

/***********copy(it1,it2,dest_it) 考虑平凡/非平凡类型构造****************/
//指向目标范围中最后复制元素的下个元素的输出迭代器。

template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last,
                    OutputIterator result) {
  while (first != last) {
    *result++ = *first++;
  }
  return result;
}

template <class InputIterator, class OutputIterator>
OutputIterator copy_backward(InputIterator first, InputIterator last,
                             OutputIterator result) {

  while (first != last) {
    *(--result) = *(--last);
  }
  return result;
}

/*********堆: this is for priorty queue****************/
//部分函数发生ADL查找，应限定命名空间
/*
is_heap    检查给定的区间是否为一个堆  O(N)
make_heap  根据区间内的元素创建出一个堆  
push_heap  将元素加入到堆  O(lgN) 。 插入位于位置 last-1 的元素到范围 [first, last-1) 所定义的最大堆中。
 pop_heap  将堆中的最大元素删除  O(lgN) sort_heap
将堆变成一个排好序的区间   O(N) 原地算法。
*/

template <class RandomIterator, class Compare>
static void up(RandomIterator which, RandomIterator first, Compare comp) {
  auto cur = which;
  while (first + (cur - first + 1) / 2 - 1 >= first) { // left<=last
    auto parent = first + (cur - first + 1) / 2 - 1; //考虑只含有左结点
    if (comp(*parent, *cur))
      mmm::swap(*parent, *cur); //注意顺序
    cur = parent;
  }
}

//违背性质的结点将下沉， 下沉which结点
template <class RandomIterator, class Compare>
static void down(RandomIterator which, RandomIterator first,
                 RandomIterator last, Compare comp) {
  auto cur = which;
  while (first + (cur - first + 1) * 2 - 1 <= last) { // left<=last
    auto left = first + (cur - first + 1) * 2 - 1;
    auto right = left + 1;
    auto swap_one = right <= last ? (comp(*left, *right) ? right : left)
                                  : left; //考虑只含有左结点
    if (comp(*cur, *swap_one))            //注意顺序
      mmm::swap(*cur, *swap_one);
    cur = swap_one;
  }
}

//构建堆：从最后一个非叶子节点开始，也就是最后一个节点的parent。 [first, last) 
template <class RandomIterator, class Compare>
void make_heap(RandomIterator first, RandomIterator last, Compare comp) {
  // 1 2 3 4 , 4 -1 =3 , 1+ 3/2 -1 = 1
  const auto range = last - first;
  for (auto cur = first + range / 2 - 1; cur >= first; --cur) {
    down(cur, first, last - 1, comp);
    if (cur == first)
      return;
  }
}
//默认大根堆
template <class RandomIterator>
void make_heap(RandomIterator first, RandomIterator last) {
  mmm::make_heap(first, last, less<iterator_value_type<RandomIterator>>());
}

//加入元素。元素位于尾部。上浮即可
template <class RandomIterator, class Compare>
void push_heap(RandomIterator first, RandomIterator last, Compare comp) {
  // up(first, last - 1, first, comp);
  up(last - 1, first, comp);
}

template <class RandomIterator>
void push_heap(RandomIterator first, RandomIterator last) {
  mmm::push_heap(first, last, less<iterator_value_type<RandomIterator>>());
}

//[first,last) 弹出顶点: 交换尾部，下沉顶点即可。
template <class RandomIterator, class Compare>
void pop_heap(RandomIterator first, RandomIterator last, Compare comp) {
  mmm::swap(*first, *(last - 1));
  if (last - first >= 2)
    down(first, first, last - 2, comp);
}
template <class RandomIterator>
void pop_heap(RandomIterator first, RandomIterator last) {
  mmm::pop_heap(first, last, less<iterator_value_type<RandomIterator>>());
}

//堆排序，原地算法。连续交换尾部
template <class RandomIterator,
          class Compare = less<iterator_value_type<RandomIterator>>>
void sort_heap(RandomIterator first, RandomIterator last,
               Compare comp = Compare()) {
  for (auto cur = last; cur != first; --cur) {
    mmm::pop_heap(first, cur, comp);
  }
}

//从非叶结点开始判断。
template <class RandomIterator, class Compare>
bool is_heap(RandomIterator first, RandomIterator last, Compare comp) {
  const auto range = last - first;
  auto index = range / 2 - 1;
  for (auto cur = first + range / 2 - 1; cur >= first; --cur, --index) {
    // auto parent = first + (cur - first + 1)/2 - 1;
    auto left = first + (cur - first + 1) * 2 - 1;
    auto right = left + 1;
    if (comp(*cur, *left) ||
        (right < last &&
         comp(*cur, *right))) //可优化尾部的right<last，为了清晰暂不
      return false;
  }
  return true;
}
template <class RandomIterator>
bool is_heap(RandomIterator first, RandomIterator last) {
  return mmm::is_heap(first, last,
                      mmm::less<iterator_value_type<RandomIterator>>());
}

// find.find_if,find_if_not

/**********find(it1,it2,val)*******************/

//指向首个满足条件的迭代器，或若找不到这种元素则为 last 。
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &val) {
  for (; first != last; ++first) {
    if (*first == val)
      return first;
  }
  return last;
}

template <class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
  for (; first != last; ++first) {
    if (p(*first)) {
      return first;
    }
  }
  return last;
}

template <class InputIt, class UnaryPredicate>
InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q) {
  for (; first != last; ++first) {
    if (!q(*first)) {
      return first;
    }
  }
  return last;
}

//交换给定的迭代器所指向的元素的值。
template <class ForwardIt1, class ForwardIt2>
void iter_swap(ForwardIt1 a, ForwardIt2 b) {
  mmm::swap(*a, *b);
}

// partition
// 重排序范围 [first, last) 中的元素，使得谓词 p 对其返回 true 的元素在false的前面.
// 
//
//
//
//
//返回交界处(不满足)
template <class ForwardIt, class UnaryPredicate>
ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p) {
  first = mmm::find_if_not(first, last, p); //找到第一个不满足
  if (first == last)
    return first;
  //找到不满足后面的满足, 交换该位置
  for (ForwardIt i = mmm::next(first); i != last; ++i) {
    if (p(*i)) {
      mmm::iter_swap(i, first);
      ++first;
    }
  }
  return first;
}

// https://en.cppreference.com/w/Talk:cpp/algorithm/partition
// https://stackoverflow.com/questions/35963436/stdpartition-called-twice-for-quick-sort
/*
initial array:  {3, 4, 5, 3, 4, 6, 4, 1, 4, 8, 8, 1, 6}
                                   ↑
                                   pivot

after 1st part: {3, 3, 1, 1, 4, 5, 4, 6, 4, 4, 8, 8, 6}
                             ↑
                             middle1

after 2nd part: {3, 3, 1, 1, 4, 4, 4, 4, 5, 6, 8, 8, 6}
                 [-----------)           [------------)
                    recurse  ↑           ↑   recurse
                             ↑           ↑
                             middle1     middle2
*/
template <class ForwardIterator,
          typename Comparator = mmm::less<iterator_value_type<ForwardIterator>>>
void quicksort(ForwardIterator first, ForwardIterator last,
               Comparator cmp = Comparator()) {
  if (first == last)
    return;
  auto pivot = *mmm::next(first, mmm::distance(first, last) / 2); //中间位置的值
  //两次, 性能考虑
  ForwardIterator middle1 = mmm::partition(
      first, last,
      [pivot, cmp](const iterator_value_type<ForwardIterator> &em) {
        return cmp(em, pivot);
      }); // const auto &em c++14
  ForwardIterator middle2 = mmm::partition(
      middle1, last,
      [pivot, cmp](const iterator_value_type<ForwardIterator> &em) {
        return !cmp(pivot, em);
      }); // em >= pivot 使用<. 因为可能没有实现其他操作.
  quicksort(first, middle1);
  quicksort(middle2, last);
}

// https://stackoverflow.com/questions/2447458/default-template-arguments-for-function-templates
template <typename ForwardIterator,
          typename Comparator = mmm::less<iterator_value_type<ForwardIterator>>>
void bubblesort(ForwardIterator begin, ForwardIterator end,
                Comparator cmp = Comparator()) {
  for (auto j = end; j != begin; --j) {
    for (auto i = mmm::next(begin); i != j; ++i) {
      auto &val0 = *i;
      auto &val1 = *mmm::prev(i);
      if (cmp(val0, val1)) {
        mmm::swap(val1, val0);
      }
    }
  }
}

template <class ForwardIterator,
          typename Comparator = mmm::less<iterator_value_type<ForwardIterator>>>
void selectionsort(ForwardIterator first, ForwardIterator last,
                   Comparator cmp = Comparator()) {
  //原理：从头开始扫描最大或者最小的数字，扫描完后放到前面相应位置
  for (auto i = first; i != last; ++i) {
    auto max_ = i;
    for (auto j = i; j != last; ++j) {
      auto &val0 = *max_;
      auto &val1 = *j;
      if (cmp(val1, val0))
        max_ = j;
    }
    mmm::swap(*max_, *i);
  }
}

template <typename ForwardIterator,
          typename Comparator = mmm::less<iterator_value_type<ForwardIterator>>>
void insertionsort(ForwardIterator begin, ForwardIterator end,
                   Comparator cmp = Comparator()) {
  for (auto i = mmm::next(begin); i != end; ++i) {
    auto key = *i;
    auto j = mmm::prev(i);
    while (j >= begin and cmp(key, *j)) {
      *mmm::next(j) = *j;
      --j;
    }
    *mmm::next(j) = key;
  }
}
} // namespace mmm

#endif
