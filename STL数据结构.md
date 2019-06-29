# 标准模板库STL（数据结构）

C++已经实现了常见的各种数据结构

每种数据结构的实现被称为容器，定义迭代器作为容器中对象的指针

迭代器被定义为公有内嵌类，类名为iterator或const_iterator

借助容器储存数据的容器称为容器适配器(栈和队列)

查找表容器通常称为关联容器

==建议：直接查看头文件中的函数声明，使用关键字检索==



## 线性表容器

vector：用动态数组实现的线性表\#include\<vector\>

list：用双链表实现的线性表\#include\<list\>

deque：经过优化的线性表，兼具两者特点，用于实现栈和队列

### 变量声明

```c++
std::vector<int> vc;
std::vector<int>::iterator itr;
std::list<int> ls;
std::list<int>::iterator its;
```

### 两者的共有操作：

```c++
int size() const;	//元素个数
void clear();        //清空元素
bool empty();		//判断是否为空
void push_back(const object &x);   	//添加到表尾
void pop_back();		//删除表尾元素
const object & front () const;		//第一个元素
const object & back() const;			//最后一个元素 
```

### 特有操作：

list可以在表头操作

```c++
void push_front(const object &x);	//表头添加元素
void pop_front();	//表头删除元素
```

vector可以实现类似数据的特征

```c++
object &opeator[](int idx);	//下标运算符重载，无检查
object &at(int idx);		//返回指定位置元素，有下标检查
int capacity();			//数组容量
void reseave(int newCapacity);	//指定数组容量
```

### 迭代器相关操作

```c++
iterator begin();		//表头位置
const_iterator begin();
iterator end();			//表尾位置
const_iterator end();
iterator insert(iterator pos,const object &x);	//插入元素
iterator erase(iterator pos);	//删除元素
iterator erase(iterator start,iterator end); //删除区间[start,end-1)
```

### 迭代器操作

```c++
itr ++;		//下一位置
* itr;		//取出元素
```

### 其他操作

```c++
int max_size();		//vector的最大范围
void resize(size);	//显式指定数组空间
void resize(size,fill);	//显示指定扩大数组空间，并填充
iterator rbegin() const; 	//反转后的表头（实际的表尾）
iterator rend()  const;		//反转后的表尾

void swap(vector &obj);	//与另一个vector交换数据
```



vector减少空间的方法被称为“收缩到合适（shrink to fit）”

使capacity收缩到size（capacity默认为2的指数）

```c++
vector<int>(vc).swap(vc);
//使用vector的拷贝构造函数，复制已有的数据，释放多余的空间 
```



## 栈容器

stack栈容器，#include\<stack\>

### 变量声明

```c++
stack<int,vector<int>> stack1;//借助vector储存数据
stack<int,list<int>> stack2;//借助list储存数据
```

### 基本操作

```c++
bool empty();		//检查栈是否为空
reference top();		//返回栈顶元素
void pop();			//出栈
void push(type &x);	//压栈
void push(const type &x);	//压栈
```



## 队列容器

queue队列容器，#include\<queue\>

### 变量声明

```c++
queue<int> queue1;			//使用deque实现的队列（默认）
queue<int,deque<int>> queue2;//使用deque实现的队列（默认）
queue<int,list<int>> queue3;//使用list实现的队列
```

### 基本操作

```c++
void push(type &x);			//入队
void push(const type &x);	//入队
void pop();			//出队
reference front();	//返回队头（下一出队元素）
reference back();	//返回队尾（上一入队元素）
bool empty() const ;	//是否为空
size_type size() const;	//队列规模
```



## 优先级队列容器

priority_queue优先级队列容器，#include\<queue\>

（内部使用二叉树实现）

### 变量声明

```c++
priority_queue<int,vector<int>,less<int>> pq1;
//默认使用vector实现，内部降序排列，最大值出队
priority_queue<int,vector<int>,greater<int>> pq2;//vector实现，升序排列，最小值出队
priority_queue<int,deque<int>,less<int>> pq3;//使用deque实现
```

第三个参数为仿函数，即使用一个类，实际上完成一个函数的作用

### 基本操作

```c++
void push(const type &x);	//入队
const type & top() const;	//返回队首值
void pop();			//出队
bool empty();		//检查是否为空
void clear();		//清空队列
```



## 查找表容器

set“集合”：数据只有一个字段键的查找表#include\<set\>

map“映射”：数据为键值对的查找表#include\<map\>

pair类型：键值对类型，map的元素#include\<utility\>

注：set和map中不允许key出现重复，需要出现重复的key时可以使用multiset和multimap

### 变量声明

```c++
set<int> s;
map<string,string> m;
pair<string,string> p;
```

### 基本操作

```c++
pair makr_pair(class1 obj1,class2 obj2);	//生成pair变量，元素类型自动推断
p.first;p.second;	//pair变量的内容

bool empty();		//判断是否为空
size_type size();	//容器规模
void clear();		//清空容器
size_type count(const key_type& key) const;		//计数键出现的次数（判断键是否存在）
iterator erase(iterator position);				//删除某位置元素
iterator erase(iterator begin,iterator end);	//删除一段元素
size_type erase(const key_type& key);			//删除键为某值的元素
iterator find(const key_type& key);			//查询键的位置（找不到时返回end）
iterator insert(value_type& value);			//插入（map中value_type为pair）
    
mapped_type at(const key_type& key);		//查找key对应的值
//map中可以直接使用下标运算，效果和at相同。但是如果目标不存在时会被创建并初始化

```

### 迭代器相关操作

```c++
iterator begin();		//返回头部
iterator end();			//返回尾部

iterator lower_bound(key_value& key);	//返回值不小于key的第一个迭代器
iterator upper_bound(key_value& key);	//返回大于key的第一个迭代器
pair<iterator,iterator> equal_range(key_value& key);	//返回等于key的范围
//找不到的时候返回end

```

特点：

1. map中的key是不可以修改的，但是每个key对应的value可以通过迭代器修改。

   set中只有key，因此不可以通过迭代器修改key的值

2. map中的元素默认按key升序排列


