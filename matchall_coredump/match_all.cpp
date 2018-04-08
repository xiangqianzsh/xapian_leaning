#include <xapian.h>

#include <thread>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;


std::shared_ptr<Xapian::Query> constructQuery() {
    Xapian::Query black_list("BLt1");
    return std::make_shared<Xapian::Query>(Xapian::Query::OP_AND_NOT, Xapian::Query::MatchAll, black_list);
}

void func(int i) {
    std::cout << "In thread " << i << std::endl;
    auto p = constructQuery();
    // Do some search in the database.
}

int main(int argc, char** argv) {
    std::thread th1(func, 1);
    std::thread th2(func, 2);
    std::thread th3(func, 3);
    std::thread th4(func, 4);
    std::thread th5(func, 5);
    std::thread th6(func, 6);
    std::thread th7(func, 7);
    std::thread th8(func, 8);
    std::thread th9(func, 9);
    std::thread th10(func, 10);
    std::thread th11(func, 11);
    std::thread th12(func, 12);
    std::thread th13(func, 13);
    std::thread th14(func, 14);
    std::thread th15(func, 15);


    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();
    th11.join();
    th12.join();
    th13.join();
    th14.join();
    th15.join();
    cout << "here is main" << std::endl;
}

// g++ match_all.cpp  -lxapian -std=c++14

