#ifndef _RB_TREE_BASE_
#define _RB_TREE_BASE_


#include "iterator.h"
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

    struct __rb_tree_iterator_base {
        typedef __rb_tree_node_base::base_link_type         base_link_type;
        typedef bidirectional_iterator_tag                  iterator_category;
        typedef ptrdiff_t                                   difference_type;

        base_link_type node;

        __rb_tree_iterator_base() {}

        __rb_tree_iterator_base(base_link_type x): node(x) {}

        void increment() {
            if (node -> parent -> parent == node && node -> color == __black && node -> parent -> right == node) {
                // root == rightmost
                //
                //     header
                //    /  |  \
                //   /   |  /
                //   \  root
                //    \ /  \
                //     o   NIL 
                node = node -> parent;
            } else if (node -> right != NULL) {
                node = __rb_tree_node_base::minimum(node -> right);
            } else {
                base_link_type y = node -> parent;
                while (node == y -> right) {
                    node = y;
                    y = y -> parent;
                }
                node = y;
            }
        }

        void decrement() {
            if (node -> parent -> parent == node && node -> color == __red) {   // header 节点
                node = node -> right;
            } else if (node -> left != NULL) {
                node = __rb_tree_node_base::maxinum(node -> left);
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

    void __rb_tree_insert_fixup(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
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
                    x -> parent -> parent -> color = __red;
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

    inline bool __rb_tree_is_black(__rb_tree_node_base* x) {
        return x == NULL || x -> color == __black;
    }

    void __rb_tree_erase_fixup(__rb_tree_node_base* x, __rb_tree_node_base* p, __rb_tree_node_base*& root) {
        __rb_tree_node_base* w;

        while (x != root && __rb_tree_is_black(x)) {
            if (x == p -> left) {
                w = p -> right;
                if (w -> color == __red) {
                    // case1 -> case2, case3, case4
                    w -> color = __black;
                    p -> color = __red;
                    __rb_tree_rotate_left(p);
                    w = p -> right;
                }
                if (__rb_tree_is_black(w -> left) && __rb_tree_is_black(w -> right)) {
                    // case2 -> case1, case2, case3, case4
                    w -> color = __red;
                    x = p;
                    p = p -> parent;
                } else { 
                    if (__rb_tree_is_black(w -> right)) {
                        // case3 -> case4
                        if (w -> left) {
                            w -> left -> color = __black;
                        }
                        w -> color = __red;
                        __rb_tree_rotate_right(w);
                        w = p -> right;
                    }
                    // case4
                    w -> color = p -> color; 
                    p -> color = __black;
                    w -> right -> color = __black;
                    __rb_tree_rotate_left(p);
                    break;
                }
            } else {
                w = p -> left;
                if (w -> color == __red) {
                    // case1 -> case2, case3, case4
                    w -> color = __black;
                    p -> color = __red;
                    __rb_tree_rotate_right(p);
                    w = p -> left;
                }
                if (__rb_tree_is_black(w -> left) && __rb_tree_is_black(w -> right)) {
                    // case2 -> case1, case2, case3, case4
                    w -> color = __red;
                    x = p;
                    p = p -> parent;
                } else {
                    if (__rb_tree_is_black(w -> left)) {
                        // case3 -> case4
                        if (w -> right) {
                            w -> right -> color = __black;
                        }
                        w -> color = __red;
                        __rb_tree_rotate_left(w);
                        w = p -> left;
                    }
                    // case4
                    w -> color = p -> color;
                    p -> color = __black;
                    w -> left -> color = __black;
                    __rb_tree_rotate_right(p);
                    break;
                }
            }
        }

        if (x != NULL) x -> color = __black;
    }

    void __rb_tree_insert(bool is_left, __rb_tree_node_base* z, __rb_tree_node_base* p, __rb_tree_node_base* header) {
        if (p == header) {
            p -> left = z;
            p -> right = z;
            p -> parent = z;
        } else if (is_left) {
            p -> left = z;
            if (p == header -> left) {
                header -> left = z;
            }
        } else {
            p -> right = z;
            if (p == header -> right) {
                header -> right = z;
            }
        }

        z -> parent = p;
        z -> left = NULL;
        z -> right = NULL;
        z -> color = __red;

        __rb_tree_insert_fixup(z, header -> parent);
    }

    __rb_tree_node_base* __rb_tree_erase(__rb_tree_node_base* z, __rb_tree_node_base* header) {
        __rb_tree_node_base* y = z -> left == NULL || z -> right == NULL ? z : __rb_tree_node_base::minimum(z -> right);
        __rb_tree_node_base* x = y -> left != NULL ? y -> left : y -> right;
        __rb_tree_node_base* p = NULL;  // 因为 x 可能为 NULL，所以需要 x 的父节点

        // 如果 y != z，则用 y 替换 z
        if (y != z) {
            // z 不可能是 leftmost 或者 rightmost
            // z 的左右孩子一定都不是 NULL，且 y 的左孩子一定是 NULL
            z -> left -> parent = y;
            y -> left = z -> left;

            if (y != z -> right) {
                //     z           y 
                //    / \         / \
                //   o   o   ->  o   o
                //      / \         / \
                //     y   o       x   o
                //    / \
                //   NIL x
                p = y -> parent;
                if (x != NULL) x -> parent = y -> parent;
                y -> parent -> left = x;    // y 一定是左孩子
                y -> right = z -> right;
                z -> right -> parent = y -> parent;
            } else {
                //     z
                //    / \
                //   o   y   ->   y
                //      / \      / \
                //     NIL x    o   x
                p = y;
            }

            if (header -> parent == z) {
                header -> parent = y;
            } else if (z -> parent -> left == z) {
                z -> parent -> left = y;
            } else {
                z -> parent -> right = y;
            }

            y -> parent = z -> parent;
            HxSTL::swap(y -> color, z -> color);
            y = z;  // 将 y 指向实际删除的节点
        } else {
            //    z(y)       z(y)       z(y)
            //   /    \     /    \     /    \
            //  x     NIL  NIL    x   NIL x(NIL)
            //      1          2          3
            p = y -> parent;
            if (x != NULL) x -> parent = y -> parent;

            if (header -> parent == z) {
                header -> parent = x;
            } else if (z -> parent -> left == z) {
                z -> parent -> left = x;
            } else {
                z -> parent -> right = x;
            }

            if (header -> left == z) {    // 左孩子一定是 NULL，见图 2 3
                if (x == NULL) {
                    header -> left = z -> parent;
                } else {
                    header -> left = x;
                }
            }

            if (header -> right == z) {   // 右孩子一定是 NULL，见图 1 3
                if (x == NULL) {
                    header -> right = z -> parent;
                } else {
                    header -> right = x;
                }
            }
        }

        if (y -> color == __black) {
            __rb_tree_erase_fixup(x, p, header -> parent);
        }

        return y;
    }

}


#endif
