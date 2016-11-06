#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_


#include "memory.h"


namespace HxSTL {

    template <class R, class... Args>
    class callable_base {
    public:
        virtual R operator()(Args... args) = 0;
        virtual ~callable_base() {};
    };

    template <class T, class R, class... Args>
    class callable: public callable_base<R, Args...> {
    public:
        callable(const T& t): _t(t) {}
        R operator()(Args... args) { return _t(args...); }
    private:
        T _t;
    };
    
    template <class T>
    class function; /* undefined */

    template <class R, class... Args>
    class function<R(Args...)> {
    public:
        typedef R       result_type;
    protected:
        unique_ptr<callable_base<R, Args...>> _ptr;
    public:
        function(): _ptr(unique_ptr<callable_base<R, Args...>>()) {}

        function(const function& other): _ptr(other._ptr) {}

        template <class F>
        function(F f): _ptr(unique_ptr<callable<F, R, Args...>>(new callable<F, R, Args...>(f))) {}

        R operator()(Args... args) const { return _ptr(args...); }
    };

}


#endif
