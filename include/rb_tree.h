#ifndef _RB_TREE_
#define _RB_TREE_


#include "iterator.h"
#include "allocator.h"
#include "utility.h"


namespace HxSTL {

    enum __rb_tree_color { __red = false, __black = true };

    struct __rb_tree_node_base {
        typedef __rb_tree_color         color_type;
        typedef __rb_tree_node_base*    base_link_type;

        color_type color;
        base_link_type parent;
        base_link_type left;
        base_link_type right;

        static base_link_type minimum(base_link_type p) {
            while (p -> left != NULL) p = p -> left;
            return p;
        }

        static base_link_type maxinum(base_link_type p) {
            while (p -> right != NULL) p = p -> right;
            return p;
        }
    };

    template <class T>
    struct __rb_tree_node: public __rb_tree_node_base {
        T value;
    };

    struct __rb_tree_iterator_base {
        typedef __rb_tree_node_base::base_link_type         base_link_type;
        typedef bidirectional_iterator_tag                  iterator_category;
        typedef ptrdiff_t                                   difference_type;

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

        __rb_tree_iterator(link_type x): __rb_tree_iterator_base(x) {}

        __rb_tree_iterator(const __rb_tree_iterator<T, T&, T*>& other): __rb_tree_iterator_base(other.node) {}

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

    template <class T, class Ref, class Ptr>
    bool operator==(const __rb_tree_iterator<T, Ref, Ptr>& lhs, const __rb_tree_iterator<T, Ref, Ptr>& rhs) {
        return lhs.node == rhs.node;
    }

    template <class T, class Ref, class Ptr>
    bool operator!=(const __rb_tree_iterator<T, Ref, Ptr>& lhs, const __rb_tree_iterator<T, Ref, Ptr>& rhs) {
        return lhs.node != rhs.node;
    }

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
        typedef typename iterator::base_link_type               base_link_type;
        typedef __rb_tree_node_base::color_type                 color_type;
    protected:
        size_type _count;
        link_type _header;
        Compare _compare;
        allocator_type _alloc;
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
        static K KEY(link_type node) { return KOV()(node -> value); }
        static color_type& COLOR(link_type node) { return node -> color; }

        static link_type& LEFT(base_link_type node) { return (link_type&) (node -> left); }
        static link_type& RIGHT(base_link_type node) { return (link_type&) (node -> right); }
        static link_type& PARENT(base_link_type node) { return (link_type&) (node -> parent); }
        static reference VALUE(base_link_type node) { return ((link_type) node) -> value; }
        static K KEY(base_link_type node) { return KOV()(((link_type) node) -> value); }
        static color_type& COLOR(base_link_type node) { return node -> color; }

        static link_type MIN(link_type node) { return (link_type) __rb_tree_node_base::minimum(node); }
        static link_type max(link_type node) { return (link_type) __rb_tree_node_base::maxinum(node); }
    private:
        iterator __insert(link_type x, link_type y, const V& value);
    public:
        rb_tree(const Compare& comp = Compare())
            :_count(0), _compare(comp), _alloc(Alloc()) {
                _header = _alloc.allocate(1);
                COLOR(_header) = __red;
                root() = NULL;
                leftmost() = _header;
                rightmost() = _header;
            }

        ~rb_tree() {}
    public:
        Compare get_compare() const { return _compare; }

        iterator begin() const { return leftmost(); }

        iterator end() const { return _header; }
        
        bool empty() const { return _count == 0; }
    
        size_type size() const { return _count; }

        size_type max_size() const { return _alloc.max_size(); }

        HxSTL::pair<iterator, bool> insert_unique(const V& value);

        iterator insert_equal(const V& value);
    };

    void __rb_tree_rotate_left(__rb_tree_node_base* x) {
        __rb_tree_node_base* y = x -> right;

        x -> right = y -> left;
        if (y -> left != NULL) {
            y -> left -> parent = x;
        }

        y -> parent = x -> parent;

        if (x == x -> parent -> parent) {
            x -> parent -> parent = y;
        } else if (x == x -> parent -> left) {
            x -> parent -> left = y;
        } else {
            x -> parent -> right = y;
        }

        y -> left = x;
        x -> parent = y;
    }

    void __rb_tree_rotate_right(__rb_tree_node_base* x) {
        __rb_tree_node_base* y = x -> left;

        x -> left = y -> right;
        if (y -> right != NULL) {
            y -> right -> parent = x;
        }

        y -> parent = x -> parent;

        if (x == x -> parent -> parent) {
            x -> parent -> parent = y;
        } else if (x == x -> parent -> left) {
            x -> parent -> left = y;
        } else {
            x -> parent -> right = y;
        }

        y -> right = x;
        x -> parent = y;
    }

    void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
        while (x != root && x -> parent -> color == __red) {
            if (x -> parent == x -> parent -> parent -> left) {
                // 父节点在左边
                __rb_tree_node_base* y = x -> parent -> parent -> right;
                if (y && y -> color == __red) {
                    // 伯父节点为红
                    x -> parent -> color = __black;
                    y -> color = __black;
                    x -> parent -> parent -> color = __red;
                    x = x -> parent -> parent;
                } else {
                    if (x == x -> parent -> right) {
                        // 插入点为右 需要旋转两次
                        x = x -> parent;
                        __rb_tree_rotate_left(x); 
                    }
                    x -> parent -> color = __black;
                    x -> parent -> parent -> color = __red;
                    __rb_tree_rotate_right(x -> parent -> parent);
                }
            } else {
                // 父节点在右边
                __rb_tree_node_base* y = x -> parent -> parent -> left;
                if (y && y -> color == __red) {
                    x -> parent -> color = __black;
                    y -> color = __black;
                    y -> parent -> parent -> color = __red;
                    x = x -> parent -> parent;
                } else {
                    if (x == x -> parent -> left) {
                        // 插入点为左 需要旋转两次
                        x = x -> parent;
                        __rb_tree_rotate_right(x);
                    }
                    x -> parent -> color = __black;
                    x -> parent -> parent -> color = __red;
                    __rb_tree_rotate_left(x -> parent -> parent);
                }
            }
        }
        root -> color = __black;
    }


    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::link_type
    rb_tree<K, V, KOV, Compare, Alloc>::create_node(T&& value) {
        link_type node = _alloc.allocate(1);
        _alloc.construct(&(node -> value), HxSTL::forward<T>(value));
        return node;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::link_type
    rb_tree<K, V, KOV, Compare, Alloc>::clone_node(link_type node) {
        link_type new_node = create_node(node -> value);
        new_node -> color = node -> color;
        new_node -> left = NULL;
        new_node -> right = NULL;
        return new_node;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    void rb_tree<K, V, KOV, Compare, Alloc>::destroy_node(link_type node) {
        _alloc.destroy(&(node -> value), 1);
        _alloc.deallocate(node);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::__insert(link_type x, link_type y, const V& value) {
        link_type node = create_node(value);

        if (y == _header) {
            LEFT(y) = node;
            RIGHT(y) = node;
            PARENT(y) = node;
        } else if (x != NULL || _compare(KOV()(value), KEY(y))) {
            LEFT(y) = node;
            if (y == leftmost()) {
                leftmost() = node;
            }
        } else {
            RIGHT(y) = node;
            if (y == rightmost()) {
                rightmost() = node;
            }
        }

        PARENT(node) = y;
        LEFT(node) = NULL;
        RIGHT(node) = NULL;
        COLOR(node) = __red;

        __rb_tree_rebalance(node, _header -> parent);
        ++_count;

        return iterator(node);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::insert_equal(const V& value) {
        link_type y = _header;
        link_type x = root();

        while (x != NULL) {
            y = x;
            x = _compare(KOV()(value), KEY(y)) ? LEFT(x) : RIGHT(x);
        }

        return __insert(x, y, value);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    HxSTL::pair<typename rb_tree<K, V, KOV, Compare, Alloc>::iterator, bool>
    rb_tree<K, V, KOV, Compare, Alloc>::insert_unique(const V& value) {
        link_type y = _header;
        link_type x = root();
        bool comp = true;

        while (x != NULL) {
            y = x;
            comp = _compare(KOV()(value), KEY(x));
            x = comp ? LEFT(x) : RIGHT(x);
        }

        iterator it = iterator(y);

        if (comp) {
            if (it == begin()) {
                return HxSTL::pair<iterator, bool>(__insert(x, y, value), true); 
            }
            it.node = it.node -> parent;
        }

        if (_compare(KEY(it.node), KOV()(value))) {
            return HxSTL::pair<iterator, bool>(__insert(x, y, value), true);
        }

        return HxSTL::pair<iterator, bool>(it, false);
    }

}


#endif
