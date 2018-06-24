#ifndef _UTILITY_H_
#define _UTILITY_H_


namespace mmm{
	//************ [swap] ***************
	template<class T>
	void swap(T& a, T& b){
		T temp = a;
		a = b;
		b = temp;
	}
}
#endif