#ifndef _MEMORY1_H_
#define _MEMORY1_H_

namespace mmm {
    //由于对象可以重载&. 这个函数忽略重载并取真正内存地址.
    // https://blog.csdn.net/qmickecs/article/details/44180525
    template <class T> T *addressof(T &arg) {
      return reinterpret_cast<T *>(
          //第一个reinterpret_cast是为了保证类型转换成功。 
          &const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
    }

} // namespace mmm

#endif