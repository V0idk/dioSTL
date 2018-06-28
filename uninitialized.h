#ifndef _UNINITIALIZED__H_
#define _UNINITIALIZED__H_

#include "type_traits.h"
#include "iterator.h"
#include "algorithm.h"
#include "mycstring.h"


namespace mmm{

	/*** uninitialized_copy: [first,last) to result .考虑平凡类型 *****/
	//返回指向最后复制的元素后一元素的迭代器
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest, true_type){
		memcpy(dest, first, (last - first) * sizeof(*first));
		return dest + (last - first);
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest, false_type){
		int i = 0;
		while(first != last){
			construct((dest + i), *first);
			++first;
			++i;
		}
		return (dest + i);
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest){
			return _uninitialized_copy(first, last, dest, is_pod<iterator_value_type<InputIterator>>());
	}

	/****uninitialized_fill: [first,last) with value 考虑平凡类型****/


	template<class ForwardIterator, class T>
	void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, true_type){
		 fill(first, last, value);
	}
	template<class ForwardIterator, class T>
	void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, false_type){
		for (; first != last; ++first){
			construct(first, value);
		}
	}
	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
		_uninitialized_fill(first, last, value, is_pod<T>());
	}


	/****uninitialized_fill_n: first n value***********/

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x, true_type){
		return fill_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x, false_type){
		Size i = 0;
		while(i!=n){
			construct((T*)(first + i), x);
			++i;
		}
		return (first + i);
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first,	Size n, const T& x){
		return _uninitialized_fill_n(first, n, x,  is_pod<T>());
	}
}

#endif
