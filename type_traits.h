#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

namespace mmm{

/*类+值的萃取,struct integral_constant {static T t;}
*	 integral_constant wraps a static constant of specified type.
*	 see libc++
* 编译期已计算: constexpr 
*/
template<typename _Tp, _Tp __v>
struct integral_constant{
		static constexpr _Tp                  value = __v;
		typedef _Tp                           value_type;
		typedef integral_constant<_Tp, __v>   type; //当前类型.
		constexpr operator value_type() const noexcept { return value; } //integral_constant<T, v>() 隐式转换为value
		constexpr value_type operator()() const noexcept { return value; }
};


typedef integral_constant<bool, true>     true_type;
typedef integral_constant<bool, false>    false_type;



/*
** 萃取传入的 T类型的类型特性, 
** 迭代器可能是原始指针, 因此需要新加一层萃取逻辑
*/
template<class T>
struct _type_traits
{
	typedef false_type		has_trivial_default_constructor;
	typedef false_type		has_trivial_copy_constructor;
	typedef false_type		has_trivial_assignment_operator;
	typedef false_type		has_trivial_destructor;
	typedef false_type		is_POD_type;
	typedef false_type    is_integer_type;
};


//平凡类型全特化
template<>
struct _type_traits<bool>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<char>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<unsigned char>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<signed char>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<wchar_t>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<short>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type   is_integer_type;
};
template<>
struct _type_traits<unsigned short>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;

	typedef true_type  is_integer_type;
};
template<>
struct _type_traits<int>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type  is_integer_type;
};
template<>
struct _type_traits<unsigned int>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type   is_integer_type;
};
template<>
struct _type_traits<long>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type   is_integer_type;
};
template<>
struct _type_traits<unsigned long>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type   is_integer_type;
};
template<>
struct _type_traits<long long>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type   is_integer_type;
};
template<>
struct _type_traits<unsigned long long>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef true_type   is_integer_type;
};
template<>
struct _type_traits<float>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type   is_integer_type;
};
template<>
struct _type_traits<double>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type   is_integer_type;
};
template<>
struct _type_traits<long double>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type   is_integer_type;
};

template<class T>
struct _type_traits<T*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<class T>
struct _type_traits<const T*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<char*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<unsigned char*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<signed char*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<const char*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<const unsigned char*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};
template<>
struct _type_traits<const signed char*>
{
	typedef true_type		has_trivial_default_constructor;
	typedef true_type		has_trivial_copy_constructor;
	typedef true_type		has_trivial_assignment_operator;
	typedef true_type		has_trivial_destructor;
	typedef true_type		is_POD_type;
	typedef false_type  is_integer_type;
};


/***************************接口******************************/

/*
template<typename _Tp>
struct is_pod : public integral_constant<bool, __is_pod(_Tp)>{ }; //__is_pod是编译期内置的,我们手动实现
*/

//__is_pod是编译期内置的,我们手动实现
//ex : is_pod<int>::type() 或 is_pod<int>()() [不推荐]; return true_type/false_type

// template<typename _Tp>
// struct is_pod : public _type_traits<_Tp>::is_POD_type{ };

template<typename _Tp>
using is_pod = typename _type_traits<_Tp>::is_POD_type::type;

// 整数
// template<typename _Tp>
// struct is_integer : public _type_traits<_Tp>::is_integer_type{ };

template<typename _Tp>
using is_integer = typename _type_traits<_Tp>::is_integer_type::type;
}//namespace mmm


#endif