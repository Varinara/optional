#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <iostream>

template<typename T>
struct optional {
    optional() {
        empty = true;
    }

    optional(T const &x) {
        empty = false;
        new(reinterpret_cast<T*>(&data)) T(x);
    }

    optional(optional const &other) : empty(other.empty) {
        if (!empty) {
            new(reinterpret_cast<T*>(&data)) T(*other);
        }
    }

    optional &operator=(optional const &x) {
        optional to_swap = x;
        swap(to_swap);
        return *this;
    }

    ~optional() {
        clear();
    }

    explicit operator bool() const {
        return !empty;
    }

    void clear() {
        if (!empty) {
            reinterpret_cast<T*>(&data)->~T();
            empty = true;
        }
    }

    void swap(optional &other) {
        if (!empty && !other.empty) {
            std::swap(*reinterpret_cast<T*>(&data), *reinterpret_cast<T*>(&other.data));
            std::swap(empty, other.empty);
        }
        else if (!empty){
            new(reinterpret_cast<T*>(&other.data)) T(*(*this));
            this -> clear();
            other.empty = 0;
        }
        else if (!other.empty){
            new(reinterpret_cast<T*>(&data)) T(*other);
            other.clear();
            empty = 0;
        }
    }


    T &operator*() {
        if (!empty)
            return *reinterpret_cast<T*>(&data);
    }

    T const &operator* () const{
        if (!empty)
            return *reinterpret_cast<T const*>(&data);
    }

    T *operator->() {
        if (!empty)
            return (reinterpret_cast<T*>(&data));
    }

    T const *operator->() const{
        if (!empty)
            return (reinterpret_cast<T const*>(&data));
    }

    friend bool check(optional &x, optional &y) {
        if (x.empty && y.empty){
            return false;
        }

        if (x.empty) {
            return true;
        }

        if (y.empty){
            return false;
        }
    }

    friend bool operator<(optional &x, optional &y) {
        if (x.empty || y.empty)
            return check(x, y);

        if (*x < *y)
            return true;
        return false;
    };
    friend bool operator==(optional &x, optional  &y){
        if (x.empty && y.empty)
            return true;
        else
            if (x.empty || y.empty)
                return false;
        if (*x == *y)
            return true;
        return false;
    };
    friend bool operator>(optional  &x, optional  &y){
        return y < x;
    };
    friend bool operator>=(optional  &x, optional  &y) {
        return !(x < y);
    };
    friend bool operator<=(optional  &x, optional  &y){
        return !(x > y);
    };
    friend bool operator!=(optional  &x, optional  &y){
        return !(x == y);
    };
private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data;
    bool empty;
};

template<typename T>
void swap(optional<T> &a, optional<T> &b) {
    a.swap(b);
}

#endif //OPTIONAL_H