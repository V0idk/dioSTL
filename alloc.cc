#include "alloc.h"

#include <stdlib.h>

//SGI-STL
namespace mmm {	
char *alloc::start_free = 0;
char *alloc::end_free = 0;
size_t alloc::heap_size = 0;

//16*8 = 128
alloc::obj *alloc::free_list[alloc::kNumOfFreelist] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void* alloc::allocate(size_t bytes){
	if (bytes > kMaxbytes){
		return malloc(bytes);
	}
	size_t index = FREELIST_INDEX(bytes);
	obj *list = free_list[index];
	if (list){//此list还有空间给我们
		free_list[index] = list->next;
		return list;
	}
	else{//此list没有足够的空间，需要从内存池里面取空间
		return refill(ROUND_UP(bytes));
	}
}
void alloc::deallocate(void *ptr, size_t bytes){
	if (bytes > kMaxbytes){
		free(ptr);
	}
	else{
		size_t index = FREELIST_INDEX(bytes);
		obj *new_head = static_cast<obj*>(ptr);
		new_head->next = free_list[index];
		free_list[index] = new_head;
	}
}
void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
	deallocate(ptr, old_sz);
	ptr = allocate(new_sz);
	return ptr;
}

//refill填充空的freelist某节点并返回其中一个。
void *alloc::refill(size_t bytes){
	size_t nobjs = kNumOfOBJS;
	char *chunk = chunk_alloc(bytes, nobjs);	//从内存池里取出一块内存。
	if (nobjs == 1){//取出的空间只够一个对象使用
		return chunk;
	}
	else{
		obj **my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;
		my_free_list = free_list + FREELIST_INDEX(bytes);
		result = reinterpret_cast<obj *>(chunk);
		*my_free_list = next_obj = reinterpret_cast<obj *>(chunk + bytes);//从第二块开始，第一块用于返回
		//将取出的多余的空间加入到相应的free list里面去
		for (size_t i = 1;; ++i){
			current_obj = next_obj;
			next_obj = reinterpret_cast<obj*>(reinterpret_cast<char*> (next_obj) + bytes);
			if (nobjs - 1 == i){
				current_obj->next = 0;
				break;
			}
			else{
				current_obj->next = next_obj;
			}
		}
		return result;
	}
}

//获取一块内存：kNumOfOBJS*n，nobjs为值结果参数
char *alloc::chunk_alloc(size_t bytes, size_t& nobjs){
	char *result = 0;
	size_t need_bytes = bytes * nobjs;
	size_t bytes_left = end_free - start_free;

	if (bytes_left >= need_bytes){//剩余空间完全满足需要
		result = start_free;
		start_free = start_free + need_bytes;
		return result;
	}
	else if (bytes_left >= bytes){//剩余空间不能完全满足需要，返回剩余所有
		nobjs = bytes_left / bytes;//注意内存池和所需大小非倍数关系。需要取整。
		need_bytes = nobjs * bytes;
		result = start_free;
		start_free += need_bytes;
		return result;
	}
	else{//内存池剩余空间连一个区块的大小都无法提供
		//新分配两倍大小的
		size_t bytes_to_get = 2 * need_bytes + ROUND_UP(heap_size >> 4);
		//把剩余空间添加到freelist，因为需要新malloc。剩余空间必须被消耗
		if (bytes_left > 0){
			obj **head =  &free_list[FREELIST_INDEX(bytes_left)];
			reinterpret_cast<obj *>(start_free)->next = *head;
			*head = reinterpret_cast<obj *>(start_free);
		}
		//开始分配空间
		start_free = static_cast<char *>(malloc(bytes_to_get));
		if (!start_free){//如果malloc分配失败，则回收freelist中的所有空间空间。这里使用递归回收而非便利所有
			obj **my_free_list = 0, *p = 0;
			for (int i = 0; i <= kMaxbytes; i += kAlign){
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0){
					*my_free_list = p->next;
					start_free = (char *)p;
					end_free = start_free + i;
					return chunk_alloc(bytes, nobjs);
				}
			}
			end_free = 0;
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(bytes, nobjs);
	}
}
}//namespace mmm