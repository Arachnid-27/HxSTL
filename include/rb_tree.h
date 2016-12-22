#ifndef _RB_TREE_
#define _RB_TREE_


#include "allocator.h"
#include "rb_tree_base.h"


namespace HxSTL {

    template <class T>
    struct __rb_tree_node: public __rb_tree_node_base {
        T value;
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
            return it;
        }

        __rb_tree_iterator& operator--() {
            decrement();
            return *this;
        }

        __rb_tree_iterator operator--(int) {
            __rb_tree_iterator it = *this;
            decrement();
            return it;
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

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    bool operator==(const __rb_tree_iterator<T, RefL, PtrL>& lhs, const __rb_tree_iterator<T, RefR, PtrR>& rhs) {
        return lhs.node == rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    bool operator!=(const __rb_tree_iterator<T, RefL, PtrL>& lhs, const __rb_tree_iterator<T, RefR, PtrR>& rhs) {
        return lhs.node != rhs.node;
    }

    template <class K, class V, class KOV, class Compare, class Alloc = allocator<__rb_tree_node<V>>>
    class rb_tree {
    public:
        typedef Alloc                                                           allocator_type;
        typedef K                                                               key_type;
        typedef V                                                               value_type;
        typedef V*                                                              pointer;
        typedef const V*                                                        const_pointer;
        typedef V&                                                              reference;
        typedef const V&                                                        const_reference;
        typedef size_t                                                          size_type;
        typedef ptrdiff_t                                                       difference_type;
        typedef __rb_tree_iterator<V, V&, V*>                                   iterator;
        typedef __rb_tree_iterator<V, const V&, const V*>                       const_iterator;
        typedef typename iterator::link_type                                    link_type;
        typedef typename iterator::base_link_type                               base_link_type;
        typedef __rb_tree_node_base::color_type                                 color_type;
        typedef typename Alloc::template rebind<__rb_tree_node<V>>::other       node_allocator_type;
    protected:
        size_type _count;
        link_type _header;
        Compare _compare;
        Alloc _alloc;
        node_allocator_type _node_alloc;
    protected:
        template <class T>
        link_type create_node(T&& value);
        link_type clone_node(link_type node);
        void destroy_node(link_type node);

        link_type root() const { return (link_type) (_header -> parent); }
        link_type leftmost() const { return (link_type) (_header -> left); }
        link_type rightmost() const { return (link_type) (_header -> right); }

        static reference VALUE(link_type node) { return node -> value; }
        static K KEY(link_type node) { return KOV()(node -> value); }

        static reference VALUE(base_link_type node) { return ((link_type) node) -> value; }
        static K KEY(base_link_type node) { return KOV()(((link_type) node) -> value); }

        iterator copy_aux(link_type x, link_type p);
        template <class T>
        iterator insert_aux(link_type y, T&& value);
        template <class T>
        iterator insert_aux(bool is_left, link_type y, T&& value);
        iterator lower_bound_aux(link_type x, link_type y, const K& key);
        iterator upper_bound_aux(link_type x, link_type y, const K& key);
    public:
        explicit rb_tree(const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            :_count(0), _compare(comp), _alloc(alloc), _node_alloc(node_allocator_type()) {
                _header = _node_alloc.allocate(1);
                _header -> color = __red;
                _header -> parent = NULL;
                _header -> left = _header;
                _header -> right = _header;
            }

        rb_tree(const rb_tree& other):_count(other._count), _compare(other._compare), 
            _alloc(other._alloc), _node_alloc(other._node_alloc) {
                copy_aux(other.root(), _header);
            }

        rb_tree(const rb_tree& other, const Alloc& alloc):_count(other._count), 
            _compare(other._compare), _alloc(alloc), _node_alloc(allocator_type()) {
                copy_aux(other.root(), _header);
            }

        rb_tree(rb_tree&& other):_count(other._count), _header(other._header),
            _compare(HxSTL::move(other._compare)), _alloc(HxSTL::move(other._alloc)), 
            _node_alloc(HxSTL::move(other._node_alloc)) {
                other._header = NULL;
            }

        rb_tree(rb_tree&& other, const Alloc& alloc):_count(other._count), _header(other._header),
            _compare(HxSTL::move(other._compare)), _alloc(alloc), _node_alloc(allocator_type()) {
                other._header = NULL;
            }

        ~rb_tree() {}
    public:
        Alloc get_allocator() const { return _alloc; }

        Compare get_compare() const { return _compare; }

        iterator begin() const { return leftmost(); }

        iterator end() const { return _header; }
        
        bool empty() const { return _count == 0; }
    
        size_type size() const { return _count; }

        size_type max_size() const { return _alloc.max_size(); }

        template <class T>
        HxSTL::pair<iterator, bool> insert_unique(T&& value);

        template <class T>
        iterator insert_unique(iterator hint, T&& value);

        template <class T>
        iterator insert_equal(T&& value);
        
        template <class T>
        iterator insert_equal(iterator hint, T&& value);

        void clear();

        iterator erase(const_iterator pos);

        size_type erase(const K& key);

        iterator erase(const_iterator first, const_iterator last);

        HxSTL::pair<iterator, iterator> equal_range(const K& key);

        HxSTL::pair<const_iterator, const_iterator> equal_range(const K& key) const;

        iterator lower_bound(const K& key) { return lower_bound_aux(root(), _header, key); }

        iterator upper_bound(const K& key) { return upper_bound_aux(root(), _header, key); }
    };

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::link_type
    rb_tree<K, V, KOV, Compare, Alloc>::create_node(T&& value) {
        link_type node = _node_alloc.allocate(1);
        _node_alloc.construct(&(node -> value), HxSTL::forward<T>(value));
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
        _node_alloc.destroy(&(node -> value));
        _node_alloc.deallocate(node, 1);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::copy_aux(link_type x, link_type p) {
        // 类似 sort 中的写法，左子树迭代右子树递归，减少递归次数
        link_type top = clone_node(x);
        top -> parent = p;

        if (x -> right != NULL) {
            top -> right = copy_aux(x -> right, p);
        }

        p = top;
        x = x -> left;

        while (x != NULL) {
            link_type y = clone_node(x);
            p -> left = y;
            y -> parent = p;

            if (x -> right != NULL) {
                y -> right = copy_aux(x -> right, p);
            }

            p = y;
            x = x -> left;
        }

        return top;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::insert_aux(link_type p, T&& value) {
        bool is_left = p == _header || _compare(KOV()(value), KEY(p));
        return insert_aux(is_left, p, HxSTL::forward<T>(value));
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::lower_bound_aux(link_type x, link_type y, const K& key) {
        while (x != NULL) {
            if (!_compare(KEY(x), key)) {
                y = x;
                x = (link_type) (x -> left);
            } else {
                x = (link_type) (x -> right);
            }
        }

        return y;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::upper_bound_aux(link_type x, link_type y, const K& key) {
        while (x != NULL) {
            if (_compare(key, KEY(x))) {
                y = x;
                x = (link_type) (x -> left);
            } else {
                x = (link_type) (x -> right);
            }
        }

        return y;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::insert_aux(bool is_left, link_type p, T&& value) {
        link_type z = create_node(HxSTL::forward<T>(value));

        __rb_tree_insert(is_left, z, p, _header);
        ++_count;

        return z;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::erase(const_iterator pos)  {
        iterator result = pos;
        ++result;

        link_type node = (link_type) __rb_tree_erase(pos.node, _header);
        destroy_node(node);
        --_count;

        return iterator(pos.node);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::erase(const_iterator first, const_iterator last) {
        if (first == begin() && last == end()) {
            clear();
        } else {
            while (first != last) {
                erase(first++);
            }
        }
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    typename rb_tree<K, V, KOV, Compare, Alloc>::size_type
    rb_tree<K, V, KOV, Compare, Alloc>::erase(const K& key)  {
        size_type old_count = _count;
        HxSTL::pair<iterator, iterator> pr = equal_range(key);
        erase(pr.first, pr.second);
        return old_count - _count;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::insert_equal(T&& value) {
        link_type y = _header;
        link_type x = root();

        while (x != NULL) {
            y = x;
            x = (link_type) (_compare(KOV()(value), KEY(y)) ? x -> left : x -> right);
        }

        return insert_aux(y, HxSTL::forward<T>(value));
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::insert_equal(iterator hint, T&& value) {
        key_type k_value = KOV()(value);

        if (hint.node == _header && 
                (_count == 0 || _compare(KEY(rightmost()), k_value))) { // end
            return insert_aux(false, rightmost(), HxSTL::forward<T>(value));
        } else if (!_compare(KEY(hint.node), k_value)) { // before
            iterator before = hint;

            if (hint.node == leftmost()) {
                return insert_aux(true, leftmost(), HxSTL::forward<T>(value));
            } else if (!_compare(k_value, KEY((--before).node))) {
                if (before.node -> right == NULL) {
                    return insert_aux(false, before.node, HxSTL::forward<T>(value));
                } else {
                    return insert_aux(true, hint.node, HxSTL::forward<T>(value));
                }
            }
        } else { // after
            iterator after = hint;

            if (hint.node == rightmost()) {
                return insert_aux(false, rightmost(), HxSTL::forward<T>(value));
            } else if (!_compare(KEY((++after).node)), k_value) {
                if (after.node -> left == NULL) {
                    return insert_aux(true, after.node, HxSTL::forward<T>(value));
                } else {
                    return insert_aux(false, hint.node, HxSTL::forward<T>(value));
                }
            }
        }

        return insert_equal(HxSTL::forward<T>(value));
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    HxSTL::pair<typename rb_tree<K, V, KOV, Compare, Alloc>::iterator, bool>
    rb_tree<K, V, KOV, Compare, Alloc>::insert_unique(T&& value) {
        link_type y = _header;
        link_type x = root();
        bool comp = true;

        while (x != NULL) {
            y = x;
            comp = _compare(KOV()(value), KEY(x));
            x = (link_type) (comp ? x -> left : x -> right);
        }

        iterator it = iterator(y);

        if (comp) {
            if (it == begin()) {
                return HxSTL::pair<iterator, bool>(insert_aux(y, HxSTL::forward<T>(value)), true); 
            }
            --it;
        }

        if (_compare(KEY(it.node), KOV()(value))) {
            return HxSTL::pair<iterator, bool>(insert_aux(y, HxSTL::forward<T>(value)), true);
        }

        return HxSTL::pair<iterator, bool>(it, false);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    template <class T>
    typename rb_tree<K, V, KOV, Compare, Alloc>::iterator
    rb_tree<K, V, KOV, Compare, Alloc>::insert_unique(iterator hint, T&& value) {
        key_type k_value = KOV()(value);

        if (hint.node == _header && 
                (_count == 0 || _compare(KEY(rightmost()), k_value))) { // end
            return insert_aux(false, rightmost(), HxSTL::forward<T>(value));
        } else {
            if (_compare(k_value, KEY(hint.node))) { // before
                iterator before = hint;

                if (hint.node == leftmost()) {
                    return insert_aux(true, leftmost(), HxSTL::forward<T>(value));
                } else if (_compare(KEY((--before).node), k_value)) {
                    //        hint          hint          before
                    //        /  \          /  \           /  \
                    //     before o        o    o         o    o    
                    //      /  \          / \                 / \
                    //     o   NIL       o before           hint o
                    //                      /  \            / \
                    //                     o   NIL        NIL  o
                    if (before.node -> right == NULL) {
                        return insert_aux(false, before.node, HxSTL::forward<T>(value));
                    } else {
                        return insert_aux(true, hint.node, HxSTL::forward<T>(value));
                    }
                }
            } else if (_compare(KEY(hint.node), k_value)) { // after
                iterator after = hint;

                if (hint.node == rightmost()) {
                    return insert_aux(false, rightmost(), HxSTL::forward<T>(value));
                } else if (_compare(k_value, KEY((++after).node))) {
                    //       hint          hint            after
                    //       /  \          /  \            /  \
                    //      o  after      o    o          o    o    
                    //          / \           / \        / \
                    //        NIL  o       after o      o hint
                    //                     /  \           / \
                    //                   NIL   o         o  NIL 
                    if (after.node -> left == NULL) {
                        return insert_aux(true, after.node, HxSTL::forward<T>(value));
                    } else {
                        return insert_aux(false, hint.node, HxSTL::forward<T>(value));
                    }
                }
            } else { // equal
                return hint;
            }
        }

        return insert_unique(HxSTL::forward<T>(value)).first;
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    HxSTL::pair<typename rb_tree<K, V, KOV, Compare, Alloc>::iterator, 
        typename rb_tree<K, V, KOV, Compare, Alloc>::iterator> 
    rb_tree<K, V, KOV, Compare, Alloc>::equal_range(const K& key) {
        //          8(y)
        //        /   \
        //       6
        //     /   \
        //          7(x)
        //        /   \
        //       7     7
        //     /   \ /   \
        //    7           7
        link_type x = root();
        link_type y = _header;

        while (x != NULL) {
            if (_compare(KEY(x), key)) {
                x = x -> right;
            } else if (_compare(key, KEY(x))) {
                y = x;
                x = x -> left;
            } else {
                return HxSTL::pair<iterator, iterator>(lower_bound_aux(x -> left, x, key), 
                        upper_bound_aux(x -> right, y, key));
            }
        }

        return HxSTL::pair<iterator, iterator>(y, y);
    }

    template <class K, class V, class KOV, class Compare, class Alloc>
    HxSTL::pair<typename rb_tree<K, V, KOV, Compare, Alloc>::const_iterator, 
        typename rb_tree<K, V, KOV, Compare, Alloc>::const_iterator> 
    rb_tree<K, V, KOV, Compare, Alloc>::equal_range(const K& key) const {
        link_type x = root();
        link_type y = _header;

        while (x != NULL) {
            if (_compare(KEY(x), key)) {
                x = x -> right;
            } else if (_compare(key, KEY(x))) {
                y = x;
                x = x -> left;
            } else {
                return HxSTL::pair<const_iterator, const_iterator>(lower_bound_aux(x -> left, x, key), 
                        upper_bound_aux(x -> right, y, key));
            }
        }

        return HxSTL::pair<const_iterator, const_iterator>(y, y);
    }

}


#endif
