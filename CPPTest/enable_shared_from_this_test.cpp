#include <memory>
#include <iostream>

class A: public std::enable_shared_from_this<A> {

    public:
        virtual void print() {
            std::cout << "A" << std::endl;
        }

        std::shared_ptr<A> getThis() {
            return shared_from_this();
        }
};

class B: public A {
    public:
        void print() override {
            std::cout << "B" << std::endl;
        }

        std::shared_ptr<B> getThis() {
            return std::dynamic_pointer_cast<B>(shared_from_this());
        }
};


int main() {
    auto pa = std::make_shared<A>();

    auto pb = std::make_shared<B>();

    pa->print();
    pa->getThis()->print();

    pb->print();
    pb->getThis()->print();

    pa = pb;
    
    pa->print();
}
