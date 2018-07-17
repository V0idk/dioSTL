#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include "type_traits.h"

//对象构造工具

namespace mmm{

//construct
template<class T1, class T2>
inline void construct(T1 *ptr1, const T2& value){
    new(ptr1) T1(value);
}

template<class T1>
inline void construct(T1 *ptr1){
    new(ptr1) T1();
}

//destroy

template<class T>
inline void destroy(T *ptr){
    ptr->~T();
}

template<class ForwardIterator> inline void _destroy(ForwardIterator first, ForwardIterator last, true_type){
	//do nothing
}

template<class ForwardIterator> inline void _destroy(ForwardIterator first, ForwardIterator last, false_type){
	for (; first != last; ++first){
		destroy(&(*first));
	}
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last){
	_destroy(first, last, is_pod<ForwardIterator>());
}

}//namespace mmm
#endif