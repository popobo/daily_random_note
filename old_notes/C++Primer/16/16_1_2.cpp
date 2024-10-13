#include <memory>
#include <vector>
#include <string>
#include <iostream>

template <typename T> class BlobPtr; //forward declaration should be BlobPtr<T>
template <typename T> class Blob;
template <typename T> bool operator==(const Blob<T>& x, const Blob<T>& y);

template <typename T>
class Blob {
    public:
        typedef T value_type;
        typedef typename std::vector<T>::size_type size_type;

        Blob();
        Blob(std::initializer_list<T> il);
        
        size_type size() const { return data->size(); }
        bool empty() const { return data->empty(); }
        
        void push_back(const T& t) { data->push_back(t); }
        void push_back(T&& t) { data->push_back(std::move(t)); }
        
        T& back() {
            return data->back();
        }

        T& front();

        T& operator[](size_type i);

        friend class BlobPtr<T>;
        friend bool operator==<T>(const Blob<T>& x, const Blob<T>& y);

    private:
        std::shared_ptr<std::vector<T>> data = nullptr;
        void check(size_type i, const std::string &msg) const {
            if (i >= data->size())
                throw std::out_of_range(msg);
        }
};

template <typename T>
Blob<T>::Blob(): data{std::make_shared<std::vector<T>>()} {}

template <typename T>
Blob<T>::Blob(std::initializer_list<T> il): data{std::make_shared<std::vector<T>>(il)} {}

template <typename T>
T& Blob<T>::front() {
    return data->front();
}

template <typename T>
T& Blob<T>::operator[] (size_type i) {
    check(i, "[]");
    return (*data)[i];
}

template <typename T>
class BlobPtr {
    public:
        BlobPtr(): curr{0} {}
        BlobPtr(Blob<T> &a, std::size_t sz = 0): wptr{a.data}, curr(sz) {}
        
        T& operator*() const {
            auto p = check(curr, "deference past end");
            return (*p)[curr];
        }

        BlobPtr<T>& operator++(); // return BlobPtr&, not BlobPtr<T>&, they are the same in the scope of the class
        BlobPtr<T>& operator--();
        BlobPtr<T> operator++(int);

    private:
        std::shared_ptr<std::vector<T>> check(std::size_t, const std::string& msg) const;
        std::weak_ptr<std::vector<T>> wptr;
        std::size_t curr;
};


template <typename T>
BlobPtr<T>& BlobPtr<T>::operator++()
{
    curr++;
    return *this;
}

template <typename T>
BlobPtr<T> &BlobPtr<T>::operator--()
{
    curr--;
    return *this;
}

template <typename T>
bool operator==<T>(const Blob<T> &x, const Blob<T> &y)
{
    return false;
}
