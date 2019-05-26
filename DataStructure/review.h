//
//  review.h
//  DataStructure
//
//  部分期末复习题的代码实现
//  Created by 杨磊 on 2019/5/26.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef review_h
#define review_h

/*
 ***********************************************
 循环单链表实现的优先级队列
 要求：只有一个指向队尾的指针，没有头指针，元素入队时在队尾
 请实现出队程序
 ***********************************************
 */
template <class type>
class Queue {
    struct node{
        type data;
        int priority;
        node *next;
        node(){}
        node(type x,node *n = NULL):data(x),next(n){}
    };
    node *rear;
    
public:
    Queue();
    bool isEmpty();
    void enQueue(type x,int priority);
    type deQueue();
    void printElem();//辅助函数
    ~Queue();
};

template <class type>
Queue<type>::Queue() {
    rear = new node();
    rear->next = rear;
    rear->priority = -1;//队列为空
}

template <class type>
bool Queue<type>::isEmpty() {
    //区分队列为空和队列只有一个元素
    return (rear->next == rear && rear->priority == -1);
}

template <class type>
void Queue<type>::enQueue(type x, int priority) {
    if(isEmpty()){//原本队列为空
        rear->data = x;
        rear->priority = priority;
    }else{//在尾部插入新的节点
        node *tmp = new node(x,rear->next);
        tmp->priority = priority;
        rear = rear->next = tmp;
        
    }
}
template <class type>
type Queue<type>::deQueue() {
    //队列为空
    if(isEmpty())return NULL;
    //队列只有一个元素
    if(rear -> next == rear){
        rear ->priority = -1;
        return rear->data;
    }
    
    node *max = rear->next;
    node *tmp = rear->next;
    //找到最大值
    while(tmp != rear){
        if(tmp->priority > max->priority)
            max = tmp;
        tmp = tmp->next;
    }
    if(rear ->priority > max->priority)
        max = rear;
    //找到最大值的前一个节点
    tmp = rear->next;
    while(tmp->next != max){
        tmp = tmp->next;
    }
    //删除节点 考虑删除尾节点的情况
    if(rear != max){
        tmp->next = max ->next;
        delete max;
    }else{
        rear = tmp;
        tmp->next = max ->next;
        delete max;
    }
    //返回优先级最大的值
    return max->data;
}

//显示队列内容的辅助函数
template <class type>
void Queue<type>::printElem() {
    node *tmp = rear->next;
    if(isEmpty())return ;
    if(rear->next == rear)
        std::cout << "(" << rear->data << "," << rear->priority << ")\n";
    else{
        while(tmp != rear){
            std::cout << "(" << tmp->data << "," << tmp->priority << ")\n";
            tmp = tmp->next;
        }
        std::cout << "(" << rear->data << "," << rear->priority << ")\n";
    }
}

template <class type>
Queue<type>::~Queue() {
    node *tmp;
    //清空其他节点
    while(rear->next != rear){
        tmp = rear -> next;;
        rear->next = tmp->next;
        delete tmp;
    }
    //删除尾节点
    delete rear;
}
#endif /* review_h */
