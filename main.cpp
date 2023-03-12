#include <iostream>

#include "SkipList.hpp"
int main(int argc, char const *argv[]) {
    std::cout << "hello" << std::endl;
    microkv::SkipList<int, int> sl;
    sl.Insert(1, 1);
    sl.Insert(2, 100);
    sl.Show();
    int value;
    if (sl.Find(1, value)) {
        std::cout << "find " << 1 << ' ' <<value << std::endl;
    } else {
        std::cout << "error1" << std::endl;
    }

    if (sl.Find(2, value)) {
        std::cout << "find " << 2 << ' ' <<value << std::endl;
    } else {
        std::cout << "error2" << std::endl;
    }

    sl.Erase(2);
    sl.Show();
    return 0;
}
