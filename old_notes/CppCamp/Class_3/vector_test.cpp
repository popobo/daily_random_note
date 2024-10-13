#include <vector>
#include <iostream>


int main() {
    std::vector<int> v = {1, 2, 3, 4};
    std::vector<int>&& refv = std::move(v);
    std::vector<int> v1{std::move(v)};

    auto dump{
        [](auto&& vec){
            std::cout << "----" << std::endl;
            for (const auto& i: vec) {
                std::cout << i << std::endl;
            }
            std::cout << "****" << std::endl;
        }
    };

    dump(v);
    //dump(v1);
    std::cout << v1.empty() << std::endl;
}