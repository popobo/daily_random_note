#pragma once

#include <memory>
#include <vector>
#include <string>

template <typename T> class BlobPtr;
template <typename T> bool operator==(const BlobPtr<T> &, const BlobPtr<T> &);
template <typename T> bool operator<(const BlobPtr<T> &, const BlobPtr<T> &);
template <typename T> bool operator-(const BlobPtr<T> &, const BlobPtr<T> &);
template <typename T> class Blob;

template <typename T>
class BlobPtr {
    friend bool operator==<T>(const BlobPtr<T> &, const BlobPtr<T> &);
    friend bool operator< <T>(const BlobPtr<T> &, const BlobPtr<T> &);
    friend bool operator-<T>(const BlobPtr<T> &, const BlobPtr<T> &);

    public:
        using size_type = typename std::vector<T>::size_type;
        
        BlobPtr(): wptr{}, curr{0} {}

        BlobPtr(const Blob<T>& blob, size_type pos = 0): wptr{blob.data}, curr{0} {
            check(pos, "BlobPtr initial position out of range");
            curr = pos; // begin() curr = 0, end() curr = size()
        }
        
        T& operator*() {
            return (*(check(curr + 1, "* out of range")))[curr];
        }
        
        T* operator->() const {
            return &**this; // delegate by operator*
        }

        BlobPtr& operator++() {
            check(curr + 1, "BlobPtr increasement pass the end");
            ++curr;
            return *this;
        }

        BlobPtr operator++(int) {
            BlobPtr ret = *this;
            ++*this;
            return ret;
        }

        BlobPtr& operator--() {
            check(curr - 1, "BlobPtr increasement pass the end");
            --curr;
            return *this;
        }

        BlobPtr& operator--(int) {
            BlobPtr ret = *this;
            --*this;
            return ret;
        }

        
        BlobPtr &operator+=(int n) {
            check(curr + n, "compound assignment out of range");
            curr += n;
            return *this;
        }

        BlobPtr &operator-=(int n) {
            return *this + -n;
        }

        BlobPtr operator+(int n) const {
            BlobPtr ret = *this;
            return ret += n;
        }

        BlobPtr operator-(int n) const {
            BlobPtr ret = *this;
            return ret += -n;
        }


    private:
        std::weak_ptr<std::vector<T>> wptr;    
        size_type curr; //curr + 1 is size;

        std::shared_ptr<std::vector<T>> check(size_type pos, const std::string& msg) {
            std::shared_ptr<std::vector<T>> ret = wptr.lock();
            if (!ret)
                throw std::runtime_error("unbound BlobPtr");
            else if (pos > ret->size())
                throw std::out_of_range(msg);
            return ret;
        }
};

template <typename T>
int operator-(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr - rhs.curr;
}

template <typename T>
bool operator==(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  // compare identity
  return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr == rhs.curr;
}

template <typename T>
bool operator!=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator<(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  // compare identity
  return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr < rhs.curr;
}

template <typename T>
bool operator>(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return rhs < lhs;
}

template <typename T>
bool operator<=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return !(lhs > rhs);
}

template <typename T>
bool operator>=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return !(lhs < rhs);
}