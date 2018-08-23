#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <cstddef> //ptrdiff_t

namespace mmm {
//五种迭代器
struct input_iterator_tag{}; //只读
struct output_iterator_tag{}; //只写
struct forward_iterator_tag : public input_iterator_tag {}; // 单向移动,仅支持operator++
struct bidirectional_iterator_tag : public forward_iterator_tag {}; //双向移动,支持operator++,operator--
struct random_access_iterator_tag : public bidirectional_iterator_tag {}; //涵盖所有指针的算数能力，支持p+n,p-n,p[n],p1-p2,p1

//迭代器的规范化实现, 不直接使用. 指明自定义的迭代器应有的别名. 
template<class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator {
	typedef Category	iterator_category;
	typedef T			value_type;
	typedef Distance	difference_type;
	typedef Pointer		pointer;
	typedef Reference	reference;
};
	
//类型萃取
//初等模板
template<class Iterator>
struct iterator_traits {
	typedef typename Iterator::iterator_category	iterator_category;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference 			reference;
};

//T*
template<class T>
struct iterator_traits<T*> {
	typedef random_access_iterator_tag 	iterator_category;
	typedef T 							value_type;
	typedef ptrdiff_t 			difference_type;
	typedef T*							pointer;
	typedef T& 							reference;
};
	
//const T*
template<class T>
struct iterator_traits<const T*> {
	typedef random_access_iterator_tag 	iterator_category;
	typedef T 							value_type;
	typedef ptrdiff_t 					difference_type;
	typedef const T*					pointer;
	typedef const T& 					reference;
};


//萃取迭代器所指类型
template<typename Iterator>
using iterator_value_type = typename iterator_traits<Iterator>::value_type;

//萃取迭代器所属分类
template<class Iterator>
using iterator_category = typename iterator_traits<Iterator>::iterator_category;

//萃取迭代器所属距离
template<class Iterator>
using iterator_difference_type = typename iterator_traits<Iterator>::difference_type;

/****************反向迭代器***************/
//正向迭代器的封装(适配器), example: ++ -> --
//ex: reverse_iterator<string::iterator>

template<class Iterator>
class reverse_iterator
{
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;
private:
	Iterator current;
public:
	reverse_iterator() : current(){}
	//cppreference: 关系 &*r == &*(i-1) 从而构造自末尾后一位置的迭代器的逆向迭代器解引用到序列的最后元素。
	explicit reverse_iterator(const Iterator& it) :current(it){
		auto temp = it;//不要修改原it
		current = --temp;
	}

	//操作符重载
	Iterator base() const { return current; }
	reference operator*(){ return (*current); }
	reference operator*()const{ return(*current); }
	// operator-> 重载返回指向T的pointer
	pointer operator->(){ return &(operator*()); }
	pointer operator->()const{ return &(operator*()); }
	reverse_iterator& operator ++(){
		--current;
		return *this;
	}
	reverse_iterator& operator ++(int){
		reverse_iterator temp = *this;
		++(*this);
		return temp;
	}
	reverse_iterator& operator--(){
		++current;
		return *this;
	}
	reverse_iterator  operator--(int){
		reverse_iterator temp = *this;
		--(*this);
		return temp;
	}
	reference operator[] (difference_type n){
		return base()[-n - 1];
	}

	//注意二元操作符重载,通常为了两个位置可以互换,会重载一个为成员,一个为friend.
	reverse_iterator operator + (difference_type n)const{
		return reverse_iterator(current + n);
	}
	reverse_iterator& operator += (difference_type n){
		current -= n;
		return *this;
	}
	reverse_iterator operator - (difference_type n) const {
		return reverse_iterator(current - n);
	}
	reverse_iterator& operator -= (difference_type n){
		current += n;
		return *this;
	}
public:
	//若友元声明为模板,可以在类外定义. 否则必须在类内定义, 因为你没办法知道 Iterator在类外是什么.
	friend bool operator == (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{return lhs.current == rhs.current;};
	friend bool operator != (const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs) 
	{ return !(lhs == rhs);}
	friend bool operator < (const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
	{ return lhs.current < rhs.current;}
	friend bool operator <= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{return !(lhs > rhs);}
	friend bool operator > (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{return lhs.current > rhs.current;}
	friend bool operator >= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{return !(lhs < rhs);}

	friend reverse_iterator<Iterator> 
	operator+ ( typename reverse_iterator<Iterator>::difference_type n, 
							const reverse_iterator<Iterator>& rev_it )
	{ return rev_it + n; }

	friend typename reverse_iterator<Iterator>::difference_type 
	operator- ( const reverse_iterator<Iterator>& lhs,
							const reverse_iterator<Iterator>& rhs )
	{ return lhs.current - rhs.current; }

};// end of reverse_iterator

//----------------算法----------------

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

//advance
template<class ForwardIt>
ForwardIt next(ForwardIt it,
               typename iterator_traits<ForwardIt>::difference_type n = 1)
{
    mmm::advance(it, n);
    return it;
}


} //namespace mmm

#endif