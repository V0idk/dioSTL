#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "allocator.h"
#include "iterator.h"
#include "uninitialized.h"
#include "utility.h"
#include "algorithm.h"
#include  <initializer_list>
#include "exception.h"
namespace mmm{
/********* vector *************/
template<class T, class Allocator = allocator<T>>
class vector{
 public:
	typedef T												value_type;
	typedef Allocator 							allocator_type;
	typedef value_type*							        iterator;
	typedef const value_type*								const_iterator;

	typedef mmm::reverse_iterator<iterator>			reverse_iterator;
	typedef mmm::reverse_iterator<const_iterator>		const_reverse_iterator;
	typedef typename Allocator::pointer							pointer;
	typedef value_type&								   	reference;
	typedef const value_type&							const_reference;
	typedef size_t								size_type;
	typedef ptrdiff_t	            difference_type;
 private:
	pointer start_;
	pointer finish_;
	pointer end_of_storage_;
 public:
	//构造，复制，析构相关函数
	vector() : start_(0), finish_(0), end_of_storage_(0){}
	explicit vector(const size_type n){	//构造n个默认T
		fill_initialize(n, value_type());
	}
	vector(const size_type n, const value_type& value){//指定value
		fill_initialize(n, value);
	}
	//this is template so all container support iterator could communicate each other 
	template<class InputIterator> vector(InputIterator first, InputIterator last){
		//处理指针和数字间的区别的函数,因为上面的接口如果value_type为整数那么显然应该调用上面的vector<int> a(10,10)。
		vector_aux(first, last,  is_integer<InputIterator>());
	}

	vector(const vector& other){
		range_initialize(other.start_, other.finish_);
	}
	vector(vector&& other){
		start_ = other.start_;
		finish_ = other.finish_;
		end_of_storage_ = other.end_of_storage_;
		other.start_ = other.finish_ = other.end_of_storage_ = 0;
	}
	vector( std::initializer_list<T> init){
		range_initialize(init.begin(),init.end());
	}
	vector& operator = (const vector& other){
		if (this != &other){
			range_initialize(other.start_, other.finish_);
		}
		return *this;
	}
	vector& operator = (vector&& other){
		if (this != &other){
			release_vector();
			start_ = other.start_;
			finish_ = other.finish_;
			end_of_storage_ = other.end_of_storage_;
			other.start_ = other.finish_ = other.end_of_storage_ = 0;
		}
		return *this;
	}
	~vector(){
		release_vector();
	}

	//迭代器
	iterator begin(){ return (start_); }
	const_iterator begin()const{ return (start_); }
	const_iterator cbegin()const{ return (start_); }
	iterator end(){ return (finish_); }
	const_iterator end()const{ return (finish_); }
	const_iterator cend()const{ return (finish_); }
	reverse_iterator rbegin(){ return reverse_iterator(finish_); }
	const_reverse_iterator crbegin()const{ return const_reverse_iterator(finish_); }
	reverse_iterator rend(){ return reverse_iterator(start_); }
	const_reverse_iterator crend()const{ return const_reverse_iterator(start_); }

	//容量
	size_type size()const{ return finish_ - start_; }
	size_type capacity()const{ return end_of_storage_ - start_; }
	bool empty()const{ return start_ == finish_; }
	void resize(size_type n, value_type val = value_type());
	void reserve(size_type n);
	void shrink_to_fit(){
		T* t = (T*)allocator_type::allocate(size());
		finish_ = mmm::uninitialized_copy(start_, finish_, t);
		allocator_type::deallocate(start_, capacity());
		start_ = t;
		end_of_storage_ = finish_;
	}

	//元素访问
	reference operator[](const difference_type i){ return *(begin() + i); }
	const_reference operator[](const difference_type i)const{ return *(cbegin() + i); }
	reference front(){ return *(begin()); }
	reference back(){ return *(end() - 1); }
	pointer data(){ return start_; }

	reference at(size_type n) {
		range_check(n);
		return (*this)[n];
	}


	const_reference
	at(size_type n) const {
		range_check(n);
		return (*this)[n];
	}

	//修改器
	void clear(){
		mmm::destroy(start_, finish_);
		finish_ = start_;
	}
	void swap(vector& v){
		if (this != &v){
			mmm::swap(start_, v.start_);
			mmm::swap(finish_, v.finish_);
			mmm::swap(end_of_storage_, v.end_of_storage_);
		}
	}
	void push_back(const value_type& value){
		insert(end(), value);
	}
	void pop_back(){
		--finish_;
		mmm::destroy(finish_);
	}
	iterator insert(iterator position, const value_type& val){
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}
	void insert(iterator position, const size_type& n, const value_type& val){
		insert_aux(position, n, val, true_type());
	}
	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last){
		insert_aux(position, first, last,  is_integer<InputIterator>());
	}
	iterator erase(iterator position){
		return erase(position, position + 1);
	}
	iterator erase(iterator first, iterator last);

	//容器的空间配置器相关
	allocator_type get_allocator(){ return allocator_type(); }
 private:
	void release_vector(){
		if (capacity() != 0){
			mmm::destroy(start_, finish_);
			allocator_type::deallocate(start_, capacity());
		}
	}
	void fill_initialize(const size_type n, const value_type& value){
		start_ = allocator_type::allocate(n);
		mmm::uninitialized_fill_n(start_, n, value);
		finish_ = end_of_storage_ = start_ + n;
	}
	template<class InputIterator>
	void range_initialize(InputIterator first, InputIterator last){
		start_ = allocator_type::allocate(last - first);
		finish_ = mmm::uninitialized_copy(first, last, start_);
		end_of_storage_ = finish_;
	}

	template<class InputIterator>
	void vector_aux(InputIterator first, InputIterator last, false_type){
		range_initialize(first, last);
	}
	template<class Integer>
	void vector_aux(Integer n, const value_type& value, true_type){
		fill_initialize(n, value);//等价于vector(n,value)
	}

	void range_check(size_type n) const {
		if (n >= size())	throw mmm::out_of_range("Out Of Range");
  }
	template<class InputIterator>
	void insert_aux(iterator position, InputIterator first, InputIterator last, false_type);

	void insert_aux(iterator position, size_type n, const value_type& value, true_type);
	template<class InputIterator>
	void realloc_insert(iterator position, InputIterator first, InputIterator last);
	//若需要的容量不大于当前capacity，则仅仅2*当前，否则当前+需要的. 这样不至于过大。
	size_type get_new_capacity(size_type need) const {
		//std::ptrdiff_t is signed. std::size_t is unsigned.
		size_type cur = end_of_storage_ - start_;
		if(cur == 0)
			return need;
		else if(need > cur)
			return cur + need;
		else
			return cur + cur;
	}
};// end of class vector


//resize change size but not capacity.
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n, value_type val){ //默认参数在定义时不写
	if (n < size()){
		mmm::destroy(start_ + n, finish_);
		finish_ = start_ + n;
	}else if (n > size() && n <= capacity()){
		auto new_add_num = n - size();
		finish_ = mmm::uninitialized_fill_n(finish_, new_add_num, val);
	}else if (n > capacity()){
		auto new_add_num = n - size();
		pointer new_start = allocator_type::allocate(get_new_capacity(new_add_num));
		pointer new_finish = mmm::uninitialized_copy(begin(), end(), new_start);
		new_finish = mmm::uninitialized_fill_n(new_finish, new_add_num, val);
		release_vector();
		start_ = new_start;
		finish_ = new_finish;
		end_of_storage_ = start_ + n;
	}
}

//reserve : base on capacity.
template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n){
	if (n <= capacity())
		return;
	pointer new_start = allocator_type::allocate(n);
	pointer new_finish = mmm::uninitialized_copy(begin(), end(), new_start);
	release_vector();
	start_ = new_start;
	finish_ = new_finish;
	end_of_storage_ = start_ + n;
}
// erase : move item
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last){
	difference_type last_to_end = end() - last;
	difference_type remove_item_num  = last - first;
	finish_ = finish_ - remove_item_num;
	mmm::destroy(first,last); //dont forget
	for(auto cur = first;last_to_end != 0;++cur,++last,--last_to_end){
		*cur = std::move(*last);//just for speed
	}
	return first;
}

// insert
// 由于强类型(无法运行时类型推导)这两个函数暂时没办法合并重用.
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, false_type) {
	difference_type need = distance(first, last);
	if (end_of_storage_ - finish_ >= need){
		mmm::copy_backward(position,finish_ ,finish_+need);
		mmm::copy(first, last, position);
		finish_ += need;
	}
	else{
		difference_type newCapacity = get_new_capacity(last - first);
		auto new_start = allocator_type::allocate(newCapacity);
		auto new_end_of_storage = new_start + newCapacity;
		auto new_finish = mmm::uninitialized_copy(begin(), position, new_start);
		new_finish = mmm::uninitialized_copy(first, last, new_finish);
		new_finish = mmm::uninitialized_copy(position, end(), new_finish);
		release_vector();
		start_ = new_start;
		finish_ = new_finish;
		end_of_storage_ = new_end_of_storage;
	}
}
template<class T, class Alloc>

void vector<T, Alloc>::insert_aux(iterator position, size_type n, const value_type& value, true_type){
	difference_type need = n;
	//enough
	if (end_of_storage_ - finish_ > need){
		//后移
    mmm::copy_backward(position,finish_ ,finish_+ need);
		//填充
		mmm::uninitialized_fill_n(position, need, value);
		finish_ += need;
	}
	else{
		// not enough
		//复制到新内存
		difference_type newCapacity = get_new_capacity(n);
		auto new_start = allocator_type::allocate(newCapacity);
		auto new_end_of_storage = new_start + newCapacity;
		auto new_finish = mmm::uninitialized_copy(begin(), position, new_start);
		new_finish = mmm::uninitialized_fill_n(new_finish, n, value);
		new_finish = mmm::uninitialized_copy(position, end(), new_finish);
		//释放原来的内存
		release_vector();
		start_ = new_start;
		finish_ = new_finish;
		end_of_storage_ = new_end_of_storage;
	}
}


//***********比较操作: 非成员.*******************
template<class T, class Alloc>
bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2){
	return v1.size() == v2.size() && equal(v1.begin(),v1.end(),v2.begin());
}
template<class T, class Alloc>
bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2){
	return !(v1 == v2);
}

}//namespace mmm

#endif