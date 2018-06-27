#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stddef.h>


//基于malloc的底层内存配置,其成员均为static
//allocate(bytes).
namespace mmm{
class alloc{
	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	private:

		//空间内存池: This for freelist, no malloc > 128kb
		static char *start_free; //内存池起始位置
		static char *end_free;   //内存池结束位置
		static size_t heap_size; // 已经在堆上分配的空间大小
		//freelist相关
		enum {
			kAlign = 8,    		//小型区块的上调边界
			kMaxbytes = 128, //小型区块的上限，超过的区块由malloc分配
			kNumOfFreelist= (kMaxbytes/ kAlign),//free-lists的个数
			kNumOfOBJS = 20//每次增加的节点数
		};

		union obj{
			union obj *next;
			char client[1];
		};
		static obj *free_list[kNumOfFreelist]; 

		//内部使用
		
		static size_t ROUND_UP(size_t bytes){       //将bytes增至8的倍数
			return ((bytes + kAlign - 1) & ~(kAlign - 1));
		}
		static size_t FREELIST_INDEX(size_t bytes){ //根据区块大小，算出free-list下标
			return ((bytes + kAlign-1)/ kAlign - 1);
		}
		static void *refill(size_t n); 	    	//如果没有可用的free list，准备重新填充free_list. 并返回其中一个
		static char *chunk_alloc(size_t size, size_t& nobjs); //配置一大块空间，建议性可容纳nobjs个大小为size的区块,
};

}//namespace mmm
#endif