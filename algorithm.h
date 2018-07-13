
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "utility.h"
#include "iterator.h"
#include "type_traits.h"
#include "functional.h"
#include "mycstring.h"

namespace mmm{

/***********equal(begin1,end1,begin)****************/

template<class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, 
           InputIt2 first2)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!(*first1 == *first2)) {
            return false;
        }
    }
    return true;
}

template<class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, 
           InputIt2 first2, BinaryPredicate p)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!p(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

/************max(a,b,[compare])********************/
template <class T> 
const T& max(const T& a, const T& b){
	return (a < b) ? b : a;
}
template <class T, class Compare>
const T& max(const T& a, const T& b, Compare compare){
	return (compare(a, b)) ? b : a;
}

/*************fill_n(it,n,value)*********/
template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first)
		*first = value;
	return first;//返回最后赋值元素后一位置的迭代
}


	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; ++first)
			*first = value;
	}

/**********distance(it1,it2) 考虑random和forward迭代器 ***********/
template<class InputIterator>
iterator_difference_type<InputIterator>
_distance(InputIterator first, InputIterator last, forward_iterator_tag){
	iterator_difference_type<InputIterator> dist = 0;
	while (first++ != last){
		++dist;
	}
	return dist;
}
template<class RandomIterator>
iterator_difference_type<RandomIterator>
_distance(RandomIterator first, RandomIterator last, random_access_iterator_tag){
	return last - first;
}

template<class Iterator>
iterator_difference_type<Iterator>
distance(Iterator first, Iterator last){
	return _distance(first, last, iterator_category<Iterator>()); 
}

/*****advance(it,distance) 考虑双向(负数距离)，单向， random*********/

template<class InputIterator, class Distance>
void _advance(InputIterator& it, Distance n, input_iterator_tag){
	assert(n >= 0);
	while (n--){
		++it;
	}
}
template<class BidirectionIterator, class Distance>
void _advance(BidirectionIterator& it, Distance n, bidirectional_iterator_tag){
	if (n < 0){
		while (n++){
			--it;
		}
	}else{
		while (n--){
			++it;
		}
	}
}
template<class RandomIterator, class Distance>
void _advance(RandomIterator& it, Distance n, random_access_iterator_tag){
	if (n < 0){
		it -= (-n);
	}else{
		it += n;
	}
}

template <class InputIterator, class Distance> 
void advance(InputIterator& it, Distance n){
	_advance(it, n, iterator_category<InputIterator>());
}


/***********copy(it1,it2,dest_it) 考虑平凡/非平凡类型构造****************/
//指向目标范围中最后复制元素的下个元素的输出迭代器。

template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result){
	while (first != last) {
			*result++ = *first++;
	}
	return result;
}

template <class InputIterator, class OutputIterator>
OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result){

	while (first != last) {
			*(--result) = *(--last);
	}
	return result;
}

/**********find(it1,it2,val)*******************/
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& val){
	for (; first != last; ++first){
		if (*first == val)
			break;
	}
	return first;
}

/*********堆: this is for priorty queue****************/
//部分函数发生ADL查找，应限定命名空间
/*
is_heap    检查给定的区间是否为一个堆  O(N)
make_heap  根据区间内的元素创建出一个堆  
push_heap  将元素加入到堆  O(lgN) 。 插入位于位置 last-1 的元素到范围 [first, last-1) 所定义的最大堆中。
pop_heap  将堆中的最大元素删除  O(lgN)
sort_heap 将堆变成一个排好序的区间   O(N) 原地算法。
*/

template<class RandomIterator, class Compare>
static void up(RandomIterator which, RandomIterator first, Compare comp){
  auto cur = which;
	while(first + (cur - first + 1)/2 - 1 >= first ){//left<=last
		auto parent = first + (cur - first + 1)/2 - 1 ;//考虑只含有左结点
		if (comp(*parent,*cur))
			mmm::swap(*parent,*cur);//注意顺序
		cur = parent;
	}
}

//违背性质的结点将下沉， 下沉which结点
template<class RandomIterator, class Compare>
static void down(RandomIterator which, RandomIterator first, RandomIterator last, Compare comp){
	auto cur = which;
	while(first + (cur - first + 1)*2 - 1 <= last ){//left<=last
		auto left = first + (cur - first + 1)*2 - 1;
		auto right = left+1;
		auto swap_one = right<=last ? (comp(*left,*right) ?right :left) : left;//考虑只含有左结点
		if (comp(*cur, *swap_one))//注意顺序
			mmm::swap(*cur, *swap_one);
		cur = swap_one;
	}
}

//构建堆：从最后一个非叶子节点开始，也就是最后一个节点的parent。 [first, last) 
template <class RandomIterator, class Compare>
void make_heap(RandomIterator first, RandomIterator last, Compare comp){
	// 1 2 3 4 , 4 -1 =3 , 1+ 3/2 -1 = 1
	const auto range = last - first;
	for (auto cur = first + range / 2 - 1; cur >= first; --cur){
		down(cur,  first, last - 1, comp);
		if (cur == first) return;
	}
}
//默认大根堆
template <class RandomIterator>
void make_heap(RandomIterator first, RandomIterator last){
	mmm::make_heap(first, last, less<iterator_value_type<RandomIterator>>());
}

//加入元素。元素位于尾部。上浮即可
template <class RandomIterator, class Compare>
void push_heap(RandomIterator first, RandomIterator last, Compare comp){
	// up(first, last - 1, first, comp);
	up(last - 1, first, comp);
}

template <class RandomIterator>
void push_heap(RandomIterator first, RandomIterator last){
	mmm::push_heap(first, last, less<iterator_value_type<RandomIterator>>());
}


//[first,last) 弹出顶点: 交换尾部，下沉顶点即可。
template <class RandomIterator, class Compare>
void pop_heap(RandomIterator first, RandomIterator last, Compare comp){
	mmm::swap(*first, *(last - 1));
	if (last - first >= 2)
		down(first, first,last - 2, comp);
}
template <class RandomIterator>
void pop_heap(RandomIterator first, RandomIterator last){
	mmm::pop_heap(first, last, less<iterator_value_type<RandomIterator>>());
}


//堆排序，原地算法。连续交换尾部
template <class RandomIterator, class Compare>
void sort_heap(RandomIterator first, RandomIterator last, Compare comp){
	for (auto cur = last; cur != first; --cur){
		mmm::pop_heap(first, cur, comp);
	}
}
template <class RandomIterator>
void sort_heap(RandomIterator first, RandomIterator last){
	return mmm::sort_heap(first, last, less<iterator_value_type<RandomIterator>>());
}	


//从非叶结点开始判断。
template <class RandomIterator, class Compare>
bool is_heap(RandomIterator first, RandomIterator last, Compare comp){
	const auto range = last - first;
	auto index = range / 2 - 1;
	for (auto cur = first + range / 2 - 1; cur >= first; --cur, --index){
		auto parent = first + (cur - first + 1)/2 - 1;
		auto left = first + (cur - first + 1)*2 - 1;
		auto right = left+1;
		if(comp(*cur,*left) || ( right<last && comp(*cur,*right)))//可优化尾部的right<last，为了清晰暂不
			return false;
	}
	return true;
}
template <class RandomIterator>
bool is_heap(RandomIterator first, RandomIterator last){
	return mmm::is_heap(first, last, mmm::less<iterator_value_type<RandomIterator>>());
}

} // namespace mmm


#endif


