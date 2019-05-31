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

#include "graph.hpp"

int main(int argc, const char * argv[]) {
    adjListGraph<char, int> g(7,"1234567");
    g.insert('1','2',1);
    g.insert('1','3',3);
    g.insert('1','4',5);
    g.insert('2','3',1);
    g.insert('2','5',2);
    g.insert('3','4',1);
    g.insert('3','6',2);
    g.insert('4','5',1);
    g.insert('4','6',2);
    g.insert('5','6',2);
    
    g.criticalPath();
    return 0;
}
