// template <typename elemType> class ListItem;
// template <typename elemType> class List {
//     public:
//         List<elemType>() {

//         }

//         List<elemType>(const List<elemType> &) {

//         }

//         List<elemType>& operator=(const List<elemType> &) {
//             return *this;
//         }
        
//         ~List() {

//         }
//         void insert(ListItem *ptr, elemType value) {

//         }
//     private:
//         ListItem *front, *end;
// };

// template <typename elemType> class ListItem {
//     private:
//         elemType ele;
// };

template <typename elemType> class ListItem;
template <typename elemType> class List {
    public:
        List<elemType>() {

        }

        List<elemType>(const List<elemType> &) {

        }

        List<elemType>& operator=(const List<elemType> &) {
            return *this;
        }
        
        ~List() {

        }
        void insert(ListItem<elemType> *ptr, elemType value) { // should use ListItem<elemType> instead of ListItem, or ListItem can not identify its template argument

        }
    private:
        ListItem<elemType> *front, *end;
};

template <typename elemType> class ListItem {
    private:
        elemType ele;
};

int main() {
    List<int> a;
    return 0;
}