//
//  review.h
//  数据结构学习 复习题
//
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

/*
 ***********************************************
 无头节点单链表实现的整数的保存
 要求：第一个节点保存最低位
 请实现输出函数
 ***********************************************
 */
class ListInt{
private:
    struct intNode{
        int data;
        intNode *next;
        
        intNode(){data = 0;next = NULL;}
        intNode(int d,intNode *n = NULL){
            data = d;
            next = n;
        }
    };
    intNode *head;
    
    void print(intNode *n)const;
    
public:
    ListInt(int num);
    ~ListInt();
    void print()const;
};

ListInt::ListInt(int num){
    head = new intNode();
    intNode *tmp = head;
    
    while(num > 0){
        tmp->data = num % 10;
        if(num > 10)
            tmp = tmp->next = new intNode();
        num /= 10;
    }
}

ListInt::~ListInt(){
    intNode *tmp;
    while(head != NULL){
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

void ListInt::print()const{
    print(head);
}

void ListInt::print(intNode *n)const{
    if(n->next == NULL){
        std::cout << n->data;
        return ;
    }
    print(n->next);
    std::cout << n->data;
    return ;
}


/*
 ***********************************************
 判断一个二叉树是不是AVL树
 要求：参数为根节点，返回值为bool类型
 请实现该函数
 ***********************************************
 */
#include "set.hpp"

template <class KEY, class OTHER>
bool binarySearchTree<KEY,OTHER>::isAVL()const {//包裹函数
    return isAVL(root);
}

template <class KEY, class OTHER>
int binarySearchTree<KEY, OTHER>::height(const binaryNode *t)const {
    if(t == NULL) return 0;
    return 1 + (height(t->left) > height(t->right) ? height(t->left) : height(t->right));
}

template <class KEY, class OTHER>
bool binarySearchTree<KEY,OTHER>::isAVL(const binaryNode *t)const {
    
    int hl = height(t->left),hr = height(t->right);//储存高度
    binaryNode *l = t->left, *r = t->right;//左右子树
    
    //特殊情况的考虑
    if(t == NULL)
        return true;
    
    if(l == NULL && r == NULL)
        return true;//叶子结点
    
    //比较子树高度
    if(abs(hl - hr) > 1)
        return false;//不满足定义
    
    if(!isAVL(t->left))
        return false;//左子树不满足定义
    
    if(!isAVL(t->right))
        return false;//右子树不满足定义
    
    return true;
}


/*
 ***********************************************
 通过前序遍历和中序遍历确定二叉树和后序遍历
 要求：输入字符串得到前序遍历和中序遍历
 请实现该函数
 ***********************************************
 */
const int MAX = 100;
//C语言风格的定义
typedef struct Nodex{
    char data;
    struct Nodex *left, *right;
} BinNode;

BinNode *builtBinTree(char *ppos, char *ipos, int n){
    BinNode *ptr = new BinNode;
    char *pos;
    int k;
    if(n <= 0) return NULL;
    //根结点为前序遍历的第一个值
    ptr->data = *ppos;
    //在中序遍历中搜索根结点
    for(pos = ipos; pos < ipos+n; pos++){
        if(*pos == *ppos)
            break;
    }
    //确定根结点的下标，得到子树的规模
    k = int(pos - ipos);
    //递归调用
    ptr->left = builtBinTree(ppos+1, ipos, k);
    ptr->right = builtBinTree(ppos+k+1, pos+1, n-1-k);
    return ptr;
}

void postOrder(BinNode *ptr){//后序遍历输出
    if(ptr == NULL)
        return ;
    postOrder(ptr->left);
    postOrder(ptr->right);
    std::cout << ptr->data;
}

void getPostOrderFromPI(){
    BinNode *root;
    char preSeq[MAX], inSeq[MAX];
    //获得前序遍历和中序遍历
    std::cout << "presequence:\t";
    std::cin >> preSeq;
    std::cout << "insequence:\t";
    std::cin >> inSeq;
    //建立二叉树
    root = builtBinTree(preSeq, inSeq, int(strlen(preSeq)));
    //结果输出
    std::cout << "post sequence:\t";
    postOrder(root);
    std::cout << "\n";
}
#endif /* review_h */
