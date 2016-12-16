#ifndef _RB_TREE_
#define _RB_TREE_


#include "iterator.h"
#include "allocator.h"


namespace HxSTL {

    typedef bool __rb_tree_color_type;
    enum __rb_tree_color { __red = false, __black = true };

    struct __rb_tree_node_base {
        typedef __rb_tree_color_type    color_type;
        typedef __rb_tree_node_base*    link_type;

        color_type color;
        link_type parent;
        link_type left;
        link_type right;

        static link_type minimum(link_type p) {
            while (p -> left != NULL) p = p -> left;
            return p;
        }

        static link_type maxinum(link_type p) {
            while (p -> right != NULL) p = p -> right;
            return p;
        }
    };

    template <class T>
    struct __rb_tree_node: public __rb_tree_node_base {
        T value;
    };

    struct __rb_tree_iterator_base {
        typedef __rb_tree_node_base::link_type      base_link_type;
        typedef bidirectional_iterator_tag          iterator_category;
        typedef ptrdiff_t                           difference_type;

        base_link_type node;

        __rb_tree_iterator_base() {}

        __rb_tree_iterator_base(base_link_type x): node(x) {}

        void increment() {
            if (node -> right != NULL) {
                node = node -> right;
                while (node -> left != NULL) {
                    node = node -> left;
                }
            } else if (node -> color == __red && node -> parent -> parent == node) {
                node = node -> parent;
            } else {
                base_link_type y = node -> parent;
                if (node == y -> right) {
                    node = y;
                    y = y -> parent;
                }
                node = y;
            }
        }

        void decrement() {
            if (node -> color == __red && node -> parent -> parent == node) {   // header 节点
                node = node -> right;
            } else if (node -> left != NULL) {
                node = node -> left;
                while (node -> right != NULL) {
                    node = node -> right;
                }
            } else {
                base_link_type y = node -> parent;
                while (node == y -> left) {
                    node = y;
                    y = y -> parent;
                }
                node = y;
            }
        }
    };

    template <class T, class Ref, class Ptr>
    struct __rb_tree_iterator: public __rb_tree_iterator_base {
        typedef T                       value_type;
        typedef Ref                     reference;
        typedef Ptr                     pointer;
        typedef __rb_tree_node<T>*      link_type;

        __rb_tree_iterator() {}

        __rb_tree_iterator(link_type x): __rb_tree_node_base(x) {}

        __rb_tree_iterator(const __rb_tree_iterator<T, T&, T*> other): __rb_tree_node_base(other.node) {}

        reference operator*() const { return link_type(node) -> value; }

        pointer operator->() const { return &(operator*()); }

        __rb_tree_iterator& operator++() {
            increment();
            return *this;
        }

        __rb_tree_iterator operator++(int) {
            __rb_tree_iterator it = *this;
            increment();
            return *this;
        }

        __rb_tree_iterator& operator--() {
            decrement();
            return *this;
        }

        __rb_tree_iterator operator--(int) {
            __rb_tree_iterator it = *this;
            decrement();
            return *this;
        }
    };

    template <class K, class V, class KOV, class Compare, class Alloc = allocator<__rb_tree_node<V>>>
    class rb_tree {
    public:
        typedef Alloc                                           allocator_type;
        typedef K                                               key_type;
        typedef V                                               value_type;
        typedef V*                                              pointer;
        typedef const V*                                        const_pointer;
        typedef V&                                              reference;
        typedef const V&                                        const_reference;
        typedef size_t                                          size_type;
        typedef ptrdiff_t                                       difference_type;
        typedef __rb_tree_iterator<V, V&, V*>                   iterator;
        typedef __rb_tree_iterator<V, const V&, const V*>       const_iterator;
        typedef typename iterator::link_type                    link_type;
        typedef __rb_tree_node_base::color_type                 color_type;
    protected:
        allocator_type _alloc;
        size_type _count;
        link_type _header;
        Compare _compare;
    protected:
        template <class T>
        link_type create_node(T&& value);
        link_type clone_node(link_type node);
        void destroy_node(link_type node);
        link_type& root() const { return (link_type&) (_header -> parent); }
        link_type& leftmost() const { return (link_type&) (_header -> left); }
        link_type& rightmost() const { return (link_type&) (_header -> right); }

        static link_type& LEFT(link_type node) { return (link_type&) (node -> left); }
        static link_type& RIGHT(link_type node) { return (link_type&) (node -> right); }
        static link_type& PARENT(link_type node) { return (link_type&) (node -> parent); }
        static reference VALUE(link_type node) { return node -> value; }
        static const K& KEY(link_type node) { return KOV()(node -> value); }
        static color_type& COLOR(link_type node) { return node -> color; }
    public:
        rb_tree(const Compare& comp = Compare())
            :_count(0), _compare(comp) {
                _header = _alloc.allocate(1);
                COLOR(_header) = __red;
                root() = NULL;
                leftmost() = _header;
                rightmost() = _header;
            }
    
    };

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::link_type
    rb_tree<K, V, KOV, Compare, Alloc>::create_node(T&& value) {
        link_type node = _alloc.allocate(1);
        _alloc.construct(&*node, HxSTL::forward(value));
        return node;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::link_type
    rb_tree<K, V, KOV, Compare, Alloc>::clone_node(link_type node) {
        link_type new_node = create_node(*node);
        new_node -> color = node -> color;
        new_node -> left = NULL;
        new_node -> right = NULL;
        return new_node;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    void rb_tree<K, V, KOV, Compare, Alloc>::destroy_node(link_type node) {
        _alloc.destroy(&*node, 1);
        _alloc.deallocate(node);
    }

}


#endif
