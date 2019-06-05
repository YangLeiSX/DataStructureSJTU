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

#include "review.h"

int main(int argc, const char * argv[]) {
    binarySearchTree<int, int> t;
    for(int i = 1;i <= 4;i++){
        t.insert(SET<int,int>(i,i));
    }
    std::cout << t.isAVL();
    return 0;
}
