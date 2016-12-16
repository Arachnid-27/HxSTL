#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_


#include "vector.h"
#include "functional.h"


namespace HxSTL {

    template <class T, class Container = HxSTL::vector<T>, 
             class Compare = HxSTL::less<typename Container::value_type>>
    class priority_queue {
    public:
        typedef Container                               container_type;
        typedef Compare                                 value_compare;
        typedef typename Container::value_type          value_type;
        typedef typename Container::size_type           size_type;
        typedef typename Container::reference           reference;
        typedef typename Container::const_reference     const_reference;
    protected:
        Compare _comp; 
        Container _cont;
    public:
        priority_queue(const Compare& comp, const Container& cont)
            : _comp(comp), _cont(cont) {
                HxSTL::make_heap(_cont.begin(), _cont.end(), _comp);
            }

        explicit priority_queue(const Compare& comp = Compare(), Container&& cont = Container())
            : _comp(comp), _cont(HxSTL::move(cont)) {
                HxSTL::make_heap(_cont.begin(), _cont.end(), _comp);
            }

        template <class InputIt>
        priority_queue(InputIt first, InputIt last, const Compare& comp, const Container& cont)
            : _comp(comp), _cont(cont) {
                _cont.insert(_cont.end(), first, last);
                HxSTL::make_heap(_cont.begin(), _cont.end(), _comp);
            }

        template <class InputIt>
        priority_queue(InputIt first, InputIt last, const Compare& comp = Compare(), Container&& cont = Container())
            // 为什么要移动
            : _comp(HxSTL::move(comp)), _cont(HxSTL::move(cont)) {
                _cont.insert(_cont.end(), first, last);
                HxSTL::make_heap(_cont.begin(), _cont.end(), _comp);
            }

        const_reference top() const { return _cont.front(); }

        bool empty() const { return _cont.empty(); }

        size_type size() const { return _cont.size(); }

        void push(const value_type& value) {
            _cont.push_back(value);
            HxSTL::push_heap(_cont.begin(), _cont.end(), _comp);
        }

        void push(value_type&& value) {
            _cont.push_back(HxSTL::move(value));
            HxSTL::push_heap(_cont.begin(), _cont.end(), _comp);
        }

        template <class... Args>
        void emplace(Args&&... args) {
            _cont.emplace_back(HxSTL::forward<Args>(args)...);
            HxSTL::push_heap(_cont.begin(), _cont.end(), _comp);
        }

        void pop() {
            HxSTL::pop_heap(_cont.begin(), _cont.end());
            _cont.pop_back();
        }

        void swap(priority_queue& other) {
            using HxSTL::swap;
            swap(_comp, other._comp);
            swap(_cont, other._cont);
        }
    };

}


#endif
