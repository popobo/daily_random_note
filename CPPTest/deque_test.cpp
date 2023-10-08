#include <deque>
#include <iostream>
#include <chrono>
#include <limits>

using namespace std;

int main() {
    int a = 0b1111;
    deque<char> queue(1000);
    cout << queue.size() << endl;
    deque<char> queue2;
    cout << queue2.size() << endl;
    queue2.resize(666);
    cout << queue2.size() << endl;

    std::string str(numeric_limits<short>::max(), 'a');
    queue.resize(numeric_limits<short>::max());
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < str.size(); ++i) {
        
    }
}