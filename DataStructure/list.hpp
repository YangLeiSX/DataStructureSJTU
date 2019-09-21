//
//  list.hpp
//  数据结构学习，线性表部分
//
//  Created by 杨磊 on 2019/2/12.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef list_hpp
#define list_hpp

#include <stdio.h>
#include <iostream>

/*
 *********************************************
 定义抽象接口类 用于规定所有的线性表的功能
 *********************************************
 */
template <class elemType>
class list{//线性表的接口对应的抽象类
public:
    virtual void clear() = 0;// =0 表示纯虚函数
    virtual int length() const = 0;
    virtual void insert(int i,const elemType &x) = 0;
    virtual void remove(int i) = 0;
    virtual int search(const elemType &x)const = 0;
    virtual elemType visit(int i)const = 0;
    virtual void traverse() const = 0;
    virtual ~list(){};//用于防止内存泄露
    
};

class outOfBound{};//异常类 数组下标越界检查
class illegalSize{};//异常类 初始规模h检查

/*
 *********************************************
 使用数组实现的线性表
 适用于相对稳定 较少执行插入和删除操作
 *********************************************
 */
template <class elemType>
class seqList: public list<elemType> {//使用数组实现的顺序表
private:
    elemType * data;//数组地址
    int currentLength;//当前的表长
    int maxSize;//数组的长度
    
    void doubleSpace();//辅助函数：自动扩容
    
    
public:
    seqList(int initSize = 10);
    seqList(const seqList &obj);
    ~seqList(){delete [] data;}//动态数组注意防止内存泄露
    void clear(){currentLength = 0;}
    int length() const {return currentLength;}
    void insert(int i, const elemType &x);
    void remove(int i);
    int search(const elemType &x) const;
    elemType visit(int i)const;
    void traverse()const;
    
    seqList& operator=(const seqList &obj);
    seqList operator+(const seqList &obj) const ;
};

template <class elemType>
void seqList<elemType>::doubleSpace(){//空间倍增
    elemType * tmp = data;
    
    maxSize *= 2;
    data = new elemType[maxSize];//申请新的空间
    
    for(int i = 0;i <currentLength;i++){//复制原有数据
        data[i] = tmp[i];
    }
    delete tmp;
    
}

template <class elemType>
seqList<elemType>::seqList(int initSize) {//初始化
    if(initSize <= 0) throw outOfBound();
    data = new elemType [initSize];//申请数组空间
    maxSize = initSize;
    currentLength = 0;
    
}

template <class elemType>
seqList<elemType>::seqList(const seqList<elemType> &obj) {
    data = new elemType [obj.maxSize];
    maxSize = obj.maxSize;
    currentLength = obj.currentLength;
    for(int i = 0;i < obj.length();i++){
        this->insert(i,obj.visit(i));
    }
}

template <class elemType>
void seqList<elemType>::insert(int i,const elemType &x) {//插入数据
    if(i < 0 || i > currentLength) throw outOfBound();
    if(currentLength == maxSize) doubleSpace();
    for(int j = currentLength;j > i;j--){//插入点后的数据全部移位
        data[j] = data[j-1];
    }
    data[i] = x;
    currentLength ++ ;
}

template <class elemType>
void seqList<elemType>::remove(int i) {//移除数据
    if(i < 0 || i > currentLength-1) throw outOfBound();
    for(int j = i ; j < currentLength -1 ;j++){//插入点后的数据全部移位
        data[j] = data[j+1];
    }
    currentLength -- ;
}

template <class elemType>
int seqList<elemType>::search(const elemType & x) const {//查找数据位置
    int i;
    for(i = 0;i < currentLength && data[i]!= x ;i++);
    if(i == currentLength)
        return -1;//没有找到
    else
        return i;
}

template <class elemType>
elemType seqList<elemType>::visit(int i) const{//读取数据
    if(i < 0 || i > currentLength - 1) throw outOfBound();
    return data[i];
}

template <class elemType>
void seqList<elemType>::traverse() const {//遍历数据
    std::cout << std::endl;
    for(int i = 0;i < currentLength;i++){
        std::cout << data[i] << " ";
    }
}

template <class elemType>
seqList<elemType>& seqList<elemType>::operator=(const seqList<elemType> &obj) {
    data = new elemType [obj.maxSize];
    maxSize = obj.maxSize;
    currentLength = obj.currentLength;
    for(int i = 0; i < obj.length() ;i++){
        this->insert(i,obj.visit(i));
    }
    return *this;
}

template<class elemType>
seqList<elemType> seqList<elemType>::operator+(const seqList<elemType> &obj) const {
    int size = this->length() + obj.length();
    seqList<elemType> tmp;
    
    for(int i = 0;i < size;i++){
        if(i < this->currentLength){
            tmp.insert(i, this->visit(i));
        }
        else{
            tmp.insert(i, obj.visit(i - this->currentLength));
        }
    }
    return tmp;
}

/*
 *********************************************
 使用单链表实现的线性表
 插入和移除数据更容易，节省空间
 *********************************************
 */

template <class elemType>
class slinkList:public list<elemType> {
private:
    struct node{
        elemType data;
        node * next;
        
        node(const elemType &x,node *n = NULL){
            data = x;next = n;
        }
        node():next(NULL){};
        ~node(){};
    };
    
    node * head;
    int currentLength;
    
    node * move(int i)const;
public:
    slinkList();
    ~slinkList(){clear();delete head;}
    
    void clear();
    int length()const {return currentLength;}
    void insert(int i,const elemType &x);
    void remove(int i);
    int search(const elemType &x)const;
    elemType visit(int i)const;
    void traverse() const ;
    void reverse();
};

template <class elemType>
typename slinkList<elemType>::node * slinkList<elemType>::move(int i) const{
    node *p = head;
    while(i-->=0)p = p->next;
    return p;
}

template <class elemType>
slinkList<elemType>::slinkList() {
    head = new node;
    currentLength = 0;
}

template <class elemType>
void slinkList<elemType>::clear() {
    node *p = head->next,*q;
    head->next = NULL;
    while(p != NULL){
        q = p -> next;
        delete p;
        p = q;
    }
    currentLength = 0;
}

template <class elemType>
void slinkList<elemType>::insert(int i, const elemType &x) {
    node *pos;
    pos = move(i-1);
    
    pos->next = new node(x,pos->next);
    ++currentLength;
}

template <class elemType>
void slinkList<elemType>::remove(int i) {
    node *pos,*delp;
    
    pos = move(i-1);
    delp = pos->next;
    pos->next = delp->next;
    delete delp;
    --currentLength;
}

template <class elemType>
int  slinkList<elemType>::search(const elemType &x) const{
    node *p = head->next;
    int i=0;
    
    while(p != NULL && p->data != x){
        p = p->next;i++;
    }
    if(p == NULL) return -1;
    else return i;
}

template <class elemType>
elemType slinkList<elemType>::visit(int i)const {
    return move(i)->data;
}

template <class elemType>
void slinkList<elemType>::traverse()const {
    node *p = head->next;
    std::cout << std::endl;
    while(p != NULL){
        std::cout << p ->data << " ";
        p = p->next;
    }
    std::cout << std::endl;
}

template <class elemType>
void slinkList<elemType>::reverse() {
    //交换数据的顺序
    slinkList<elemType> tmp;
    int size = this->length();
    for(int i = 0;i < size;i++){
        tmp.insert(0, this->head->next->data);//每次访问第一个，时间复杂度为O(1)
        this->remove(0);//每次访问第一个，时间复杂度为O(1)
    }
    //交换头指针的值
    node * tt = this->head;
    this->head = tmp.head;
    tmp.head = tt;
    
}

/*
 *********************************************
 使用双链表实现的线性表
 插入和移除数据更容易，节省空间
 *********************************************
 */

template <class elemType>
class linkList:public list<elemType> {//使用双链表实现的线性表
private:
    struct node{//定义内嵌的节点类
        elemType data;//节点数据
        node *prev,*next;//双链表具有前后两个指针
        
        node(const elemType &x,node *p = NULL,node *n = NULL){
            data = x;prev = p;next = n;
        }
        node():next(NULL),prev(NULL){}
        ~node(){}
    };
    node *head ,*tail;//线性表的q首尾节点
    int currentLength;//表长
    
    node * move(int i) const;//辅助函数：移动到指定位置
    
public:
    linkList();
    ~linkList(){clear(),delete head,delete tail;}//防止内存泄漏，所有的节点都要清楚
    void clear();
    int length() const{return currentLength;}
    void insert(int i,const elemType &x);
    void remove(int i);
    int search(const elemType &x) const;
    elemType visit(int i) const;
    void traverse() const;
    
};

template <class elemType>
typename linkList<elemType>::node* linkList<elemType>::move(int i)const {//移动到指定位置
    node * p = head->next;
    if(i < 0 || i > currentLength) throw outOfBound();
    while(i--) p = p->next;//移动i位
    
    return p;
}

template <class elemType>
linkList<elemType>::linkList() {//构造空表
    head  = new node;
    tail = new node;
    head->next = tail;
    tail->prev = head;
    currentLength = 0;
}

template <class elemType>
void linkList<elemType>::clear() {//清除数据
    node *p,*q;
    p = head->next;//存放所有数据
    
    head->next = tail;//构建空表
    tail->prev = head;
    
    while(p != tail){//遍历删除
        q = p->next;
        delete p;
        p = q;
    }
    currentLength = 0;
    
}

template <class elemType>
void linkList<elemType>::insert(int i,const elemType &x) {//插入数据
    node *pos,*tmp;
    
    pos = move(i);
    tmp = new node(x,pos->prev,pos);
    pos->prev->next = tmp;
    pos->prev = tmp;
    
    currentLength ++ ;
}

template <class elemType>
void  linkList<elemType>::remove(int i) {//删除数据
    node *pos;
    pos = move(i);
    
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    
    delete pos;
    currentLength --;
}

template <class elemType>
int linkList<elemType>::search(const elemType &x) const {//查找数据
    node *p = head->next;
    int i = 0;
    while(p!= tail && p->data != x){
        p = p->next;
        i++;
    }
    if(p == tail)
        return -1;//没有找到
    else
        return i;
}

template <class elemType>
elemType linkList<elemType>::visit(int i)const {//读取数据
    node *p = move(i);
    return p->data;
    
}

template <class elemType>
void  linkList<elemType>::traverse()const {//遍历数据
    node *p = head->next;
    std::cout << std::endl;
    while (p!= tail) {
        std::cout << p->data << "\t";
        p = p->next;
    }
    std::cout << std::endl;
}


/*
 *********************************************
 使用双循环链表实现的线性表
 同时定义迭代器类，可以实现取值，前移/后移，赋值，比较
 （迭代器的实现存在逻辑问题，需要进一步优化）
 *********************************************
 */
template<class elemType>
class dclinkList:public list<elemType>{
private:
    //定义节点类
    struct node{
        elemType data;//数据
        node * prev;
        node * next;
        
        node(const elemType &x,node *p = NULL,node *n = NULL){
            data = x;prev = p;next = n;
        }
        node():data(NULL),prev(NULL),next(NULL){}
        ~node(){};
    };
    node * head;//头指针
    int currentLength;//表长
    
    node * move(int i) const;//辅助函数
    
public:
    //定义迭代器
    struct iterator{
        node * target;//迭代器指向一个节点
        
        iterator(const dclinkList<elemType> &x){
            target = x.head;
        }
        iterator(const iterator &itr){//复制构造函数
            target = itr.target;
        }
        iterator(node *n = NULL):target(n){};
        ~iterator(){};
        
        iterator &operator=(const int pos);//赋值
        iterator &operator=(const iterator &itr);//赋值
        elemType operator*() const ;//取值
        
        //++itr
        iterator &operator++();
        iterator &operator--();
        //itr++
        iterator operator++(int);
        iterator operator--(int);
        
        bool operator==(const iterator &obj);//比较是否相等
        bool operator!=(const iterator &obj);
    };
    
    dclinkList();
    ~dclinkList(){clear(),delete head;}//防止内存泄漏，所有的节点都要清除
    void clear();
    int length() const{return currentLength;}
    void insert(int i,const elemType &x);
    void remove(int i);
    void remove(const iterator &itr);
    int search(const elemType &x) const;
    elemType visit(int i) const;
    void traverse() const;

};
//函数的实现
template <class elemType>
typename dclinkList<elemType>::node* dclinkList<elemType>::move(int i)const {//移动到指定位置
    node * p = head->next;
    while(i--) p = p->next;//移动i位
    
    return p;
}

template <class elemType>
dclinkList<elemType>::dclinkList() {//构造空表
    head  = NULL;
    currentLength = 0;
}

template <class elemType>
void dclinkList<elemType>::clear() {//清除数据
    node *p,*q;
    p = head->next;//存放所有数据
    
    head = NULL;//构建空表
   
    
    while(p != head){//遍历删除
        q = p->next;
        delete p;
        p = q;
    }
    currentLength = 0;
    
}

template <class elemType>
void dclinkList<elemType>::insert(int i,const elemType &x) {//插入数据
    if(currentLength == 0){
        head = new node(x,NULL,NULL);
        head->next = head;
        head->prev = head;
        currentLength ++;
        return ;
    }
    node *pos,*tmp;
    
    pos = move(i);
    tmp = new node(x,pos->prev,pos);
    pos->prev->next = tmp;
    pos->prev = tmp;
    if(i == 0){
        head = tmp;
    }
    currentLength ++ ;
}

template <class elemType>
void  dclinkList<elemType>::remove(int i) {//删除数据
    node *pos;
    pos = move(i);
    
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    
    delete pos;
    currentLength --;
}

template <class elemType>
void  dclinkList<elemType>::remove(const iterator &itr) {//删除数据
    node *pos;
    pos = itr.target;

    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    
    delete pos;
    currentLength --;
}

template <class elemType>
int dclinkList<elemType>::search(const elemType &x) const {//查找数据
    node *p = head->next;
    int i = 0;
    while(p!= head && p->data != x){
        p = p->next;
        i++;
    }
    if(p == head)
        return -1;//没有找到
    else
        return i;
}

template <class elemType>
elemType dclinkList<elemType>::visit(int i)const {//读取数据
    node *p = move(i);
    return p->data;
    
}

template <class elemType>
void  dclinkList<elemType>::traverse()const {//遍历数据
    node *p = head;
    std::cout << std::endl;
    while (p!= head) {
        std::cout << p->data << "\t";
        p = p->next;
    }
    std::cout << std::endl;
}

//前置运算符 ++itr
template<class elemType>
typename dclinkList<elemType>::iterator & dclinkList<elemType>::iterator::operator++(){//后移
    this->target = this->target->next;
    return *this;
}

template<class elemType>
typename dclinkList<elemType>::iterator & dclinkList<elemType>::iterator::operator--(){//前移
    this->target = this->target->next;
    return *this;
}

//后置运算符 itr++
template<class elemType>
typename dclinkList<elemType>::iterator  dclinkList<elemType>::iterator::operator++(int){//后移
    dclinkList<elemType>::iterator tmp;
    tmp = *this;
    this->target = this->target->next;
    return tmp;
}

template<class elemType>
typename dclinkList<elemType>::iterator  dclinkList<elemType>::iterator::operator--(int){//前移
    dclinkList<elemType>::iterator tmp;
    tmp = *this;
    this->target = this->target->prev;
    return tmp;
}

template<class elemType>
elemType dclinkList<elemType>::iterator::operator*() const {//取值
    return this->target->data;
    
}

template<class elemType>
typename dclinkList<elemType>::iterator & dclinkList<elemType>::iterator::operator=(const int pos){
    int ppos = pos;
    while(ppos--){
        target = target->next;
        
    }
    return *this;
}

template <class elemType>
typename dclinkList<elemType>::iterator & dclinkList<elemType>::iterator::operator=(const iterator &itr) {
    this->target = itr.target;
    return *this;
}

template<class elemType>
bool dclinkList<elemType>::iterator::operator==(const typename dclinkList<elemType>::iterator &obj){//相等
    
    return &(*this->target) == obj.target;
}

template<class elemType>
bool dclinkList<elemType>::iterator::operator!=(const typename dclinkList<elemType>::iterator &obj){
    
    return !(*this == obj);
}
#endif /* list_hpp */
