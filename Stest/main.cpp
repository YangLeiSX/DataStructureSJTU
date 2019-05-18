//
//  main.cpp
//  Stest
//  用于数据结构的学习和代码实现
//
//  Created by 杨磊 on 2019/1/14.
//  Copyright © 2019 杨磊. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

#include "set.hpp"

int main(int argc, const char * argv[]) {
    int size = 15;
    node<int> *test = dataGen<int>(size);
    std::cout << "origin data:\n";
    printData(test);
    mergeSort(test, 1, size);
    std::cout << "after sort:\n";
    printData(test);
    return 0;
}
