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
    };

}


#endif
