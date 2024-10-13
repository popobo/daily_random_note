#include <iostream>
#include "Blob.h"
#include "BlobPtr.h"

void testStrBlob(Blob<std::string> &sb) {
    try {
        sb.push_back("abc");
        std::cout << "front: " << sb.front() << " back: " << sb.back() << std::endl;
        sb.pop_back();
        std::cout << "front: " << sb.front() << " back: " << sb.back() << std::endl;
        *sb.begin() = "Change";
        for (auto p = sb.begin(); ; ++p)
            std::cout << "deref: " << *p << std::endl;
    } catch(const std::out_of_range& err) {
        std::cerr << err.what() << " out of range" << std::endl;
    } catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
    }
}

int main() {
    Blob<std::string> sb1;
    testStrBlob(sb1);
    std::cout << std::endl;

    Blob<std::string> sb2{"Hello", "World"};
    testStrBlob(sb2);
    std::cout << std::endl;
    std::cout << sb2[1] << std::endl;
    std::cout << sb2.at(1) << std::endl;
    try {
        std::cout << sb2[100] << std::endl;
    } catch (std::out_of_range err) {
        std::cerr << err.what() << " out of range" << std::endl;
    } catch (std::exception err) {
        std::cerr << err.what() << std::endl;
    }
    try {
        std::cout << sb2.at(100) << std::endl;
    } catch (std::out_of_range err) {
        std::cerr << err.what() << " out of range" << std::endl;
    } catch (std::exception err) {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}