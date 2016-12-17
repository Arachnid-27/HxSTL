#ifndef _PAIR_H_
#define _PAIR_H_


namespace HxSTL {

    template <class T1, class T2>
    struct pair {
        typedef T1      first_type;
        typedef T2      second_type;

        T1 first;
        T2 second;

        pair(): first(T1()), second(T2()) {}
        template <class U, class V>
        pair(const pair<U, V>& pr): first(pr.first), second(pr.second) {}
        pair(const first_type& a, const second_type& b): first(a), second(b) {}

        pair& operator=(const pair& pr) {
            first = pr.first;
            second = pr.second;
        }
    };

    template <class T1, class T2>
    bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) { 
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class T1, class T2>
    bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs == rhs); 
    }

    template <class T1, class T2>
    bool operator< (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) { 
        return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
    }

    template <class T1, class T2>
    bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(rhs < lhs);
    }

    template <class T1, class T2>
    bool operator> (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return rhs < lhs;
    }

    template <class T1, class T2>
    bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs < rhs);
    }

}


#endif
