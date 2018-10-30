#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "alloc.h"
#include "construct.h"
#include <new>
#include <stdlib.h>

namespace mmm{

	//alloc以字节为单位分配, allocator以对象为单位分配.
	template<class T>
	class allocator{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	public:
		static T* allocate(); //分配一个
		static T* allocate(size_t n);
		static void deallocate(T *ptr);
		static void deallocate(T *ptr, size_t n);
	};

	template<class T>
	T *allocator<T>::allocate(){
		// return static_cast<T *>(alloc::allocate(sizeof(T)));
		return (T*)malloc(sizeof(T));
	}
	template<class T>
	T *allocator<T>::allocate(size_t n){
		if (n == 0) return 0;
		// return static_cast<T *>(alloc::allocate(sizeof(T) * n));
		return (T*)malloc(sizeof(T) * n);
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr){
		// alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
		free(ptr);
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr, size_t n){
		if (n == 0) return;
		// alloc::deallocate(static_cast<void *>(ptr), sizeof(T)* n);
		free(ptr);
	}
}

#endif