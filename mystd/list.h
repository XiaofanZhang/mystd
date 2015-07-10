#include "allocator.h"

using namespace mystd;

namespace mystd{
    template <class T>  struct __list_node {
        typedef void* void_pointer;
        void_pointer prev;
        void_pointer next;
        T data;
    };

    template <class T, class Ref, class Ptr> struct __list_iterator {
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, Ref, Ptr> self;

        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef __list_node<T>* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        
        link_type node;

        // constructor
        __list_iterator(link_type x) : node(x) {}
        __list_iterator() {}
        __list_iterator(const iterator& x) : node(x.node) {}


        bool operator==(const self& x) const { return node == x.node; }
        bool operator!=(const self& x) const { return node != x.node; }
        reference operator*() const  { return (*node).data; }
        pointer operator->() const { return &(operator*());}

        self& operator++(){
            node = (link_type)((*node).next);
            return *this;
        }

        self& operator--(){
            node = (link_type)((*node).prev);
            return *this;
        }
    };

    template <class T, class Alloc = allocator<T> > class list {
        protected:
            typedef __list_node<T> list_node;
            typedef allocator<list_node> list_node_alloc;
            typedef __list_iterator<T,T&,T*> iterator;
            typedef T value_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
        public:
            typedef list_node* link_type;
        protected:
            link_type node;
            link_type get_node() {
                return list_node_alloc::allocate(1);
            }
            void put_node(link_type p) {
                list_node_alloc::deallocate(p);
            }
            link_type create_node(const T& x){
                link_type p = get_node();
                construct(&p->data, x);
                return p;
            }

            void destroy_node(link_type p){
                Alloc::destroy(&p->data);
                put_node(p);
            }

        public:
            iterator begin() { return (link_type)((*node).next);}
            iterator end()   { return node;}
            bool empty()     { return node->next == node;}

            reference front() { return *begin(); }
            reference back()  { return *(--end());}
            list() { 
                node = get_node();
                node->prev = node;
                node->next = node;
            }

            ~list() {
                link_type start = link_type(node->next);
                while( start != node){
                    link_type cur = start;
                    start = link_type(start->next);
                    destroy_node(cur);
                }
                put_node(node);
            }

            void push_front(const T& x) { insert(begin(), x); }

            void push_back(const T& x) { insert(end(), x); }

            void pop_front() { erase(begin()); }

            void pop_back() { erase(--end()); }

            iterator erase(iterator position){
                link_type prev = link_type(position.node->prev);
                link_type next = link_type(position.node->next);
                prev->next = next;
                next->prev = prev;
                destroy_node(position.node);
                return iterator(next);
            }

            iterator insert(iterator position, const T& x){
                link_type tmp = create_node(x);
                tmp->next = position.node;
                tmp->prev = position.node->prev;

                link_type(position.node->prev)->next = tmp;
                position.node->prev = tmp;
                return tmp;
            }

            void clear() {
                link_type start = link_type(node->next);
                while(start != node){
                    link_type cur = start;
                    start = link_type(start->next);
                    destroy_node(cur);
                }
                node->prev = node;
                node->next = node;
            }
    };
}
