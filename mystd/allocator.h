#ifndef _ALLOC_
#define _ALLOC_
#include <new>
#include <cstddef>
#include <cstdlib>
#include <iostream>
// simple allocator

namespace mystd
{
    template <class T> inline T* _allocate(ptrdiff_t size, T*){
            T* tmp = (T *)(::operator new ( (size_t) (size * sizeof(T))));
            if (tmp == 0){
                std::cerr << "out of memory" << std::endl;
                exit(1);
            }
            return tmp;
    }

    template <class T> inline void _deallocate(T* buffer){
        ::operator delete(buffer);
    }

    template <class T1, class T2> inline void construct(T1* p, const T2& value) {
        new (p) T1(value);
    }

    template <class T> inline void _destroy(T* ptr){
        ptr->~T();
    }

    template <class T> inline void _destroy(T* first, T* last){
        for(; first < last; ++first)
            _destroy(first);
    }

    template <class T> class allocator {
        public:
            typedef T           value_type;
            typedef T*          pointer;
            typedef T&          reference;
            typedef const T&    const_reference;
            typedef size_t      size_type;
            typedef ptrdiff_t   difference_type;

            static pointer allocate(size_type n){
                return _allocate( (ptrdiff_t) n, (pointer) 0);
            }

            static void deallocate(pointer p, size_type n){
                _deallocate(p);
            }

            static void deallocate(pointer p){
                _deallocate(p);
            }

            static void destroy(pointer p){
                _destroy(p);
            }

            static void destroy(pointer first, pointer last){
                _destroy(first, last);
            }

            static pointer copy(pointer first, pointer last, pointer result){
                pointer cur = result;
                for( ; first != last; ++first, ++cur)
                    construct(&*cur, *first);
                return cur;
            }

            size_type max_size() const {
                return std::max(size_type(1), size_type(UINT_MAX/sizeof(T)));
            }

    };
}
#endif
