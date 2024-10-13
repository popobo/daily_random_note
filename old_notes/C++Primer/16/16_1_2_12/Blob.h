#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>

template <typename T> class BlobPtr;
template <typename T> class ConstBlobPtr;

template <typename T> class Blob;
template <typename T> bool operator==(const Blob<T>&, const Blob<T>&);
template <typename T> bool operator<(const Blob<T>&, const Blob<T>&);

template <typename T>
class Blob {
    friend class BlobPtr<T>;
    friend class ConstBlobPtr<T>;
    
    friend bool operator==<T>(const Blob<T>&, const Blob<T> &);
    friend bool operator< <T>(const Blob<T>&, const Blob<T> &);

    public:
        using value_type = T;
        using size_type = typename std::vector<T>::size_type;
        
        using iterator = BlobPtr<T>;
        using const_iterator = ConstBlobPtr<T>;
        using data_type = std::shared_ptr<std::vector<T>>;

        Blob(): data{std::make_shared<std::vector<T>>()} {}

        Blob(const std::initializer_list<T>& il): data{std::make_shared<std::vector<T>>(il)} {}

        Blob(const Blob& other): data{std::make_shared<std::vector<T>>(*other.data)} {}
        
        Blob& operator=(const Blob& other) {
            data = std::make_shared<std::vector<T>>(*other.data);
            return *this;
        }

        size_type size() const { return data->size(); }
        
        bool empty() const { return data->empty(); }

        void push_back(const T& t) {
            data->push_back(t);
        }

        void push_back(const T&& t) {
            data->push_back(std::move(t));
        }

        void pop_back() {
            check(0, "pop on empty Blob");
            data->pop_back();
        }

        const T& front() const {
            check(0, "front on empty Blob");
            return data->front();
        }

        T& front() {
            check(0, "front on empty Blob");
            return data->front();
        }

        T& back() {
            check(0, "back on empty Blob");
            return data->back();
        }

        T& at(size_type pos) {
            check(pos, "at on wrong pos");
            return (*data)[pos];
        }

        T& operator[](size_type pos) {
            check(pos, "at on wrong pos");
            return at(pos);
        }

        const_iterator begin() const { return const_iterator(*this); }
        
        iterator begin() { return iterator(*this); }
        
        const_iterator end() const { return const_iterator(*this, size()); }

        iterator end() { return iterator(*this, size()); }

    private:
        std::shared_ptr<std::vector<T>> data;
        
        void check(size_type pos, const std::string & msg) const {
            if (pos > size())
                throw std::out_of_range("msg");
        }
};

template <typename T>
bool operator==(const Blob<T>& lhs, const Blob<T>& rhs) {
    return *lhs.data == *rhs.data; // compare value
}

template <typename T>
bool operator!=(const Blob<T>& lhs, const Blob<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const Blob<T>& lhs, const Blob<T>& rhs) {
    return *lhs.data < *rhs.data;
}

template <typename T>
bool operator>(const Blob<T>& lhs, const Blob<T>& rhs) {
    return rhs < lhs;
}

template <typename T>
bool operator<=(const Blob<T>& lhs, const Blob<T>& rhs) {
    return !(lhs > rhs);
}

template <typename T>
bool operator>=(const Blob<T>& lhs, const Blob<T>& rhs) {
    return !(lhs < rhs);
}