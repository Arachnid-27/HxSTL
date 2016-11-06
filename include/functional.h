#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_


namespace HxSTL {

    template <class R, class... Args>
    class base_callable {
    public:
        virtual R operator()(Args... args) = 0;
        virtual ~base_callable() {};
    };

    template <class T, class R, class... Args>
    class callable: public base_callable<R, Args...> {
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
        base_callable<R, Args...>* _callable;
    public:
        function(): _callable(0) {}

        function(const function& other): _callable(other._callable) {}

        template <class F>
        function(F f): _callable(new callable<F, R, Args...>(f)) {}

        R operator()(Args... args) const { return _callable(args...); }
    };

}


#endif
