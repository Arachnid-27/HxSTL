#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_


#include "memory.h"
#include "type_info.h"


namespace HxSTL {

    template <class R, class... Args>
    class callable_base {
    public:
        virtual ~callable_base() {}
        virtual R invoke(Args... args) = 0;
        virtual void* get() = 0;
        virtual const type_info& target_type() = 0;
    };

    template <class T, class R, class... Args>
    class callable: public callable_base<R, Args...> {
    public:
        callable(const T& t): _t(t) {}

        R invoke(Args... args) { return _t(args...); }

        void* get() { return static_cast<void*>(&_t); }

        const type_info& target_type() { return typeid(T); }
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
        shared_ptr<callable_base<R, Args...>> _ptr;
    protected:
        void assignment_aux(const function& other, true_type) { function(other).swap(*this); }
        template <class F>
        void assignment_aux(F&& f, false_type) { function(HxSTL::move(f)).swap(*this); }
    public:
        function(): _ptr(nullptr) {}

        function(nullptr_t): _ptr(nullptr) {}

        function(const function& other): _ptr(other._ptr) {}

        function(function&& other): _ptr(other._ptr) { other._ptr = nullptr; }

        template <class F>
        function(F f): _ptr(shared_ptr<callable<F, R, Args...>>(new callable<F, R, Args...>(f))) {}

        ~function() {}

        function& operator=(const function& other) {
            assignment_aux(other);
            return *this;
        }

        function& operator=(function&& other) {
            function(HxSTL::move(other)).swap(*this);
            return *this;
        }

        function& operator=(nullptr_t) {
            function(nullptr).swap(*this);
            return *this;
        }

        template <class F>
        function& operator=(F&& f) {
            assignment_aux(f, typename is_same<typename remove_const<F>::type, function<R(Args...)>&>::type());
            return *this;
        }

        void swap(function& other) { HxSTL::swap(_ptr, other._ptr); }

        explicit operator bool() const { return _ptr != nullptr; }

        R operator()(Args... args) const { return _ptr -> invoke(args...); }

        const type_info& target_type() const { return _ptr ? _ptr -> target_type() : typeid(void); }

        template <class F>
        F* target() { return target_type() == typeid(F) ? reinterpret_cast<F*>(_ptr -> get()) : nullptr; }

        template <class F>
        const F* target() const { return target_type() == typeid(F) ? static_cast<F*>(_ptr -> get()) : nullptr; }
    };

    template <class R, class... Args>
    void swap(function<R(Args...)> &lhs, function<R(Args...)> &rhs) {
        lhs.swap(rhs);
    }

    template<class R, class... ArgTypes>
    bool operator==(const function<R(ArgTypes...)>& f, nullptr_t) {
        return !f;
    }

    template<class R, class... ArgTypes>
    bool operator==(nullptr_t, const function<R(ArgTypes...)>& f) {
        return !f;
    }

    template<class R, class... ArgTypes>
    bool operator!=(const function<R(ArgTypes...)>& f, nullptr_t) {
        return (bool) f;
    }

    template<class R, class... ArgTypes>
    bool operator!=(nullptr_t, const function<R(ArgTypes...)>& f) {
        return (bool) f;
    }

    template <class T>
    struct less {
        bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
    };

    template <class T>
    struct greater {
        bool operator()(const T& lhs, const T& rhs) const { return lhs > rhs; }
    };

}


#endif
