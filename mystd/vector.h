#include "allocator.h"
// std::copy
#include <iostream>
#include <cstddef>
namespace mystd{
    template <class T, class Alloc = allocator<T> >
        class vector {
            public:
                typedef T value_type;
                typedef value_type*  pointer;
                typedef value_type*  iterator;
                typedef const value_type* const_iterator;
                typedef value_type&  reference;
                typedef size_t      size_type;
                typedef ptrdiff_t   difference_type;

            protected:
                iterator start;
                iterator finish;
                iterator pos;

            public:
                iterator begin() {  return start; }
                iterator end()   {  return finish; }

                const_iterator begin() const { return start; }
                const_iterator end() const { return finish; }

                bool empty() const {  return begin() == end(); }
                size_type size() const { return size_type(end() - begin()); }
                size_type capacity() const { return size_type(pos - begin()); }

                reference operator[] (size_type n) { return *(begin() + n); }
                reference front() { return *begin(); }
                reference back() { return *(end() - 1); }

                void fill_init(size_type n, const T& value){
                    start = Alloc::allocate(n);
                    iterator cur = start;
                    for(int i = 0; i < n; i++, cur++) {
                        mystd::construct(cur, value);
                    }
                    finish = start + n;
                    pos = finish;
                }

                vector() : start(0), finish(0), pos(0) {}
                vector(int n, const T& value)   { fill_init(n, value); }
                explicit vector(size_type n)     { fill_init(n, T()); }

                ~vector() {
                    Alloc::destroy(start, finish);
                    Alloc::deallocate(start);
                }

                void insert_aux(iterator position, const T& v){
                    if ( finish != pos ){
                        construct(finish, *(finish - 1));
                        ++ finish;
                        T x_copy = v;
                        std::copy_backward(position, finish - 2, finish - 1);
                        *position = x_copy;
                    } else {
                        // resize
                        const size_type old = size();
                        const size_type len = old != 0 ? 2 * old : 1;

                        iterator newStart = Alloc::allocate(len);
                        iterator newEnd = newStart;
                        try {
                            newEnd = Alloc::copy(start, finish, newStart);
                            construct(newEnd, v);
                            ++newEnd;
                        } catch (...){
                            throw;
                        }

                        Alloc::destroy(begin(), end());
                        Alloc::deallocate(start);

                        start = newStart;
                        finish = newEnd;
                        pos = newStart + len;
                    }
                }

                void push_back(const T& x){
                    if (finish != pos){
                        construct(finish, x);
                        ++finish;
                    } else
                        insert_aux(end(), x);
                }

                void pop_back(){
                    --finish;
                    Alloc::destroy(finish);
                }

                iterator erase(iterator position){
                    if (position + 1 != finish)
                        std::copy(position + 1, finish, position);
                    --finish;
                    Alloc::destroy(finish);
                    return position;
                }
        };
}
