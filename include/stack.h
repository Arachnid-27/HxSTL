#ifndef _STACK_H_
#define _STACK_H_


#include "deque.h"


namespace HxSTL {

    template <class T, class Container = HxSTL::deque<T>>
    class stack {
    public:
        typedef Container                               container_type;
        typedef typename Container::value_type          value_type;
        typedef typename Container::size_type           size_type;
        typedef typename Container::reference           reference;
        typedef typename Container::const_reference     const_reference;
    protected:
        Container _cont;
    public:
        explicit stack(const Container& cont): _cont(cont)  {}

        explicit stack(Container&& cont = Container()): _cont(HxSTL::move(cont)) {}

        reference top() { return _cont.back(); }

        const_reference top() const { return _cont.back(); }

        bool empty() const { return _cont.empty(); }
        
        size_type size() const { return _cont.size(); }

        void push(const value_type& value) { _cont.push_back(value); }

        void push(value_type&& value) { _cont.push_back(HxSTL::move(value)); }

        template <class... Args>
        void emplace(Args... args) { _cont.emplace_back(HxSTL::forward<Args>(args)...); }

        void pop() { _cont.pop_back(); }

        void swap(stack& other) {
            using HxSTL::swap;
            swap(_cont, other._cont);
        }
    };

}


#endif
