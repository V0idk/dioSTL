#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_

//functioal: 包含一些函数对象(把类当函数用),通过重载operator()实现
namespace mmm{

	//一元(参数类型的个数),元代表函数对象的一种分类方法.
	template <class Arg, class Result>
	struct unary_function {
		typedef Arg argumant_type;
		typedef Result result_type;
	};

	//二元
	template <class Arg1, class Arg2, class Result>
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};


	/*******************************************************************************/

	/*******identity： 返回自身的 抽象对象*************/
	template <class T>
	struct identity : public unary_function<T, T> {
		const T& operator()(const T& x) const {return x;}  	//assert( (double)1.1 == mmm::identity<double>()(1.1));
	};


	/*************less greater equal*****************/
	template <class T>
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x < y;}
	};

	template <class T>
	struct greater : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x > y;}
	};

	template<class T>
	struct equal_to : public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x == y;}

	};

	/******************return pair.first**********************/
	template <class _Pair>
	struct select1st : public unary_function<_Pair, typename _Pair::first_type> {
		const typename _Pair::first_type& operator()(const _Pair& x) const {return x.first;}
	};

}// namespace mmm
#endif