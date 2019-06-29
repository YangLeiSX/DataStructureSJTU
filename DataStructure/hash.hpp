//
//  hash.hpp
//  数据结构学习 散列表部分
//
//  Created by 杨磊 on 2019/6/29.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef hash_hpp
#define hash_hpp

/*
 *********************************************
 使用线性探测法的闭散列表
 *********************************************
 */
template <class KEY,class OTHER>
class closeHashTable:public dynamicSearchTable<KEY, OTHER> {
private:
    struct Node{
        SET<KEY, OTHER> data;
        int state;
        
        Node(){state = 0;}
    };
    Node *array;
    int size;
    int (*key)(const KEY &x);
    static int defaultKey(const int &x){return x;}
    
public:
    closeHashTable(int length = 101,int (*f)(const KEY &x) = defaultKey);
    ~closeHashTable(){delete [] array;}
    SET<KEY, OTHER> *find(const KEY &x)const;
    void insert(const SET<KEY,OTHER> &x);
    void remove(const KEY &x);
    
};

template <class KEY,class OTHER>
closeHashTable<KEY, OTHER>::closeHashTable(int length,int (*f)(const KEY &x)) {
    size = length;
    array = new Node[length];
    key = f;
}

template <class KEY,class OTHER>
void closeHashTable<KEY, OTHER>::insert(const SET<KEY, OTHER> &x) {
    int initPos,pos;
    initPos = pos = key(x.key) % size;
    do{
        if(array[pos].state != 1){
            array[pos].data = x;
            array[pos].state = 1;
            return ;
        }
        pos = (pos + 1) % size;
    }while(pos != initPos);
}

template <class KEY,class OTHER>
void closeHashTable<KEY,OTHER>::remove(const KEY &x) {
    int initPos,pos;
    
    initPos = pos = key(x) % size;
    do{
        if(array[pos].state == 0){
            return ;
        }
        if(array[pos].state == 1 && array[pos].data.key == x){
            array[pos].state = 2;
            return ;
        }
        pos = (pos + 1) % size;
    }while(pos != initPos);
}

template <class KEY,class OTHER>
SET<KEY,OTHER>* closeHashTable<KEY,OTHER>::find(const KEY &x)const {
    int initPos,pos;
    
    initPos = pos = key(x) % size;
    do{
        if(array[pos].state == 0)
            return NULL;
        if(array[pos].state == 1 && array[pos].data.key == x)
            return (SET<KEY, OTHER> *) &array[pos];
        pos = (pos + 1) % size;
    }while(pos != initPos);
    return NULL;
}

/*
 *********************************************
 使用单链表的开散列表
 *********************************************
 */
template <class KEY,class OTHER>
class openHashTable:public dynamicSearchTable<KEY, OTHER> {
private:
    struct Node{
        SET<KEY,OTHER> data;
        Node *next;
        
        Node(const SET<KEY,OTHER> &d,Node *n = NULL){
            data = d;
            next = n;
        }
        Node(){
            next = NULL;
        }
    };
    
    Node **array;
    int size;
    int (*key)(const KEY &x);
    static int defaultKey(const int &x){return x;}
    
public:
    openHashTable(int length = 101,int(*f)(const KEY &x) = defaultKey);
    ~openHashTable();
    SET<KEY, OTHER>* find(const KEY &x)const;
    void insert(const SET<KEY, OTHER> &x);
    void remove(const KEY &x);
};

template <class KEY,class OTHER>
openHashTable<KEY, OTHER>::openHashTable(int length,int (*f)(const KEY &x)) {
    size = length;
    array = new Node*[size];
    key = f;
    for(int i = 0; i < size;i++){
        array[i] = NULL;
    }
}

template <class KEY,class OTHER>
openHashTable<KEY, OTHER>::~openHashTable() {
    Node *q,*p;
    
    for(int i = 0;i < size;i++){
        p = array[i];
        while(p != NULL){
            q = p->next;
            delete p;
            p = q;
        }
    }
    delete [] array;
}

template <class KEY,class OTHER>
void openHashTable<KEY,OTHER>::insert(const SET<KEY, OTHER> &x) {
    int pos;
    
    pos = key(x.key) % size;
    array[pos] = new Node(x,array[pos]);
}

template <class KEY,class OTHER>
void openHashTable<KEY,OTHER>::remove(const KEY &x) {
    int pos;
    Node *p,*q;
    
    pos = key(x) % size;
    if(array[pos] == NULL)
        return ;
    p = array[pos];
    if(array[pos]->data.key == x){
        array[pos] = p->next;
        delete p;
        return ;
    }
    
    while(p->next != NULL && !(p->next->data.key == x))
        p = p->next;
    if(p->next != NULL){
        q = p->next;
        p->next = q->next;
        delete q;
    }
}

template <class KEY,class OTHER>
SET<KEY,OTHER> *openHashTable<KEY,OTHER>::find(const KEY &x)const {
    int pos;
    Node *p;
    
    pos = key(x) % size;
    p = array[pos];
    while(p != NULL && !(p->data.key == x))
        p = p->next;
    if(p == NULL)
        return NULL;
    else
        return (SET<KEY,OTHER> *) p;
}

#endif /* hash_hpp */
