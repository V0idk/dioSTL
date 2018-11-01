#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "type_traits.h"

namespace mmm{
	//************ [swap] ***************
	template<class T>
	void swap(T& a, T& b) noexcept {
		T temp = a;
		a = b;
		b = temp;
	}
	template< class T >
	T&& forward( typename mmm::remove_reference<T>::type& t ){ 
		return static_cast<T&&>(t); 
	}  
	template <class T> 
	typename remove_reference<T>::type&& move(T&& t)//我们知道这种形式是T可以推导出引用的唯一情况
	{
		return static_cast<typename mmm::remove_reference<T>::type&&>(t);
	}

	struct piecewise_construct_t
	{
		explicit piecewise_construct_t() = default;
	};

	struct pair_first_construct_t {

	};
}

#endif