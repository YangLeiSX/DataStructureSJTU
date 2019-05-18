//
//  tree.hpp
//  Stest
//
//  Created by 杨磊 on 2019/4/15.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef tree_hpp
#define tree_hpp

#include <iostream>
/*
 *********************************************
 定义抽象接口类 用于规定所有的树的功能
 *********************************************
 */
template <class elemType>
class tree {
public:
    virtual void clear() = 0;
    virtual bool isEmpty() const = 0;
    virtual elemType root(elemType flag) const = 0;
    virtual elemType parent(elemType value,elemType flag) const = 0;
    virtual elemType child(elemType value,int i,elemType flag)const = 0;
    virtual void remove(elemType value,int i) = 0;
    virtual void traverse() const  = 0;
};

/*
 *********************************************
 定义抽象接口类 用于规定所有的二叉树的功能
 *********************************************
 */
template <class elemType>
class bTree {
public:
    virtual void clear() = 0;
    virtual bool isEmpty() const = 0;
    virtual elemType Root(elemType flag) const = 0;
    virtual elemType parent(elemType value,elemType flag) const = 0;
    virtual elemType lchild(elemType value,elemType flag) const = 0;
    virtual elemType rchild(elemType value,elemType flag) const = 0;
    virtual void delLeft(elemType value) = 0;
    virtual void delRight(elemType value) = 0;
    virtual void preOrder() const = 0;
    virtual void midOrder() const = 0;
    virtual void postOrder() const = 0;
    virtual void levelOrder() const = 0;
};

/*
 *********************************************
 使用链表实现的二叉树
 *********************************************
 */
template <class elemType>
class binaryTree : public bTree<elemType> {
    
    template <class T>
    friend void printTree(const binaryTree<T> &t,T flag);
    
private:
    struct Node{
        Node *left,*right;
        elemType data;
        
        Node():left(NULL),right(NULL){}
        Node(elemType item,Node *l = NULL,Node *r = NULL):data(item),left(l),right(r){}
        ~Node(){}
    };
    struct STNode{//用于遍历的非递归实现
        Node * node;
        int timesPop;
        
        STNode(Node *n = NULL):node(n),timesPop(0){}
    };
    
    Node *root;
    
public:
    binaryTree():root(NULL){}
    binaryTree(elemType x){root = new Node (x);}
    ~binaryTree();
    void clear();
    bool isEmpty()const;
    elemType Root(elemType flag) const ;
    elemType lchild(elemType value,elemType flag) const ;
    elemType rchild(elemType value,elemType flag) const ;
    void delLeft(elemType value) ;
    void delRight(elemType value) ;
    //遍历的递归实现
    void preOrder() const ;
    void midOrder() const ;
    void postOrder() const ;
    //遍历的非递归实现
    void NpreOrder() const ;
    void NmidOrder() const ;
    void NpostOrder() const ;
    //层次遍历
    void levelOrder() const ;
    void createTree(elemType flag);
    elemType parent(elemType value,elemType flag) const {return flag;}//纯虚函数的无意义的实现
    int size()const;
    int height()const;
    void changeLR();//递归交换左右子树
    int countDegreeIsTwo();//统计度数为2的节点数
private:
    Node * find(elemType value,Node *t) const;
    void clear(Node *&t);
    void preOrder(Node *t)const;
    void midOrder(Node *t)const;
    void postOrder(Node *t)const;
    int size(Node *t)const;
    int height(Node *t)const;
    void changeLR(Node *t);
    int countDegreeIsTwo(Node *t);
};

template <class elemType>
bool binaryTree<elemType>::isEmpty()const {
    return root == NULL;
}

template <class elemType>
elemType binaryTree<elemType>::Root(elemType flag) const {
    if(root == NULL)
        return flag;
    else
        return root->data;
}

template <class elemType>
void binaryTree<elemType>::clear(binaryTree<elemType>::Node *&t) {
    if(t == NULL)
        return ;
    clear(t->left);
    clear(t->right);
    delete t;
    t = NULL;
}

template <class elemType>
void binaryTree<elemType>::clear() {
    clear(root);
}

template <class elemType>
binaryTree<elemType>::~binaryTree() {
    clear(root);
}

template <class elemType>
void binaryTree<elemType>::preOrder(binaryTree<elemType>::Node *t)const {
    if(t == NULL)
        return;
    std::cout << t->data << " ";
    preOrder(t->left);
    preOrder(t->right);
    
}

template <class elemType>
void binaryTree<elemType>::preOrder()const {
    std::cout << "\npreOrder:";
    preOrder(root);
}

template <class elemType>
void binaryTree<elemType>::midOrder(binaryTree<elemType>::Node *t)const {
    if(t == NULL)
        return ;
    midOrder(t->left);
    std::cout << t->data << " ";
    midOrder(t->right);
}

template <class elemType>
void binaryTree<elemType>::midOrder()const {
    std::cout << "\nmidOrder:" ;
    midOrder(root);
}

template <class elemType>
void binaryTree<elemType>::postOrder(binaryTree<elemType>::Node *t)const {
    if(t == NULL)
        return ;
    postOrder(t->left);
    postOrder(t->right);
    std::cout << t->data << " ";
}

template <class elemType>
void binaryTree<elemType>::postOrder()const {
    std::cout << "\npostOrder:";
    postOrder(root);
}

template <class elemType>
void binaryTree<elemType>::levelOrder()const {
    linkQueue<Node *> que;
    Node *tmp;
    
    std::cout << "\nlevelOrder:" ;
    que.enQueue(root);
    
    while(!que.isEmpty()){
        tmp = que.deQueue();
        std::cout << tmp->data << " ";
        if(tmp->left)
            que.enQueue(tmp->left);
        if(tmp->right)
            que.enQueue(tmp->right);
    }
}

template <class elemType>
typename binaryTree<elemType>::Node *  binaryTree<elemType>::find(elemType value,binaryTree<elemType>::Node *t)const {
    Node *tmp;
    if(t == NULL)
        return NULL;
    if(t -> data == value)
        return t;
    if((tmp = find(value,t->left)) != NULL)
        return tmp;
    else
        return find(value,t->right);
    
}

template <class elemType>
void binaryTree<elemType>::delLeft(elemType value) {
    Node *tmp = find(value,root);
    if(tmp == NULL)
        return;
    clear(tmp->left);
}

template <class elemType>
void binaryTree<elemType>::delRight(elemType value) {
    Node *tmp = find(value,root);
    if(tmp == NULL)
        return;
    clear(tmp->right);
}

template <class elemType>
elemType binaryTree<elemType>::lchild(elemType value,elemType flag)const {
    Node *tmp = find(value,root);
    if(tmp == NULL || tmp->left == NULL)
        return flag;
    
    return tmp->left->data;
}

template <class elemType>
elemType binaryTree<elemType>::rchild(elemType value, elemType flag)const {
    Node *tmp = find(value,root);
    if(tmp == NULL || tmp->right == NULL)
        return flag;
    
    return tmp->right->data;
}

template <class elemType>
void binaryTree<elemType>::createTree(elemType flag) {
    linkQueue<Node *> que;
    Node * tmp;
    elemType x,ldata,rdata;
    
    std::cout << "Please enter the root data:";
    std::cin >> x;
    
    root = new Node(x);
    que.enQueue(root);
    
    while(!que.isEmpty()){
        tmp = que.deQueue();
        std::cout << "please enter two children of "<< tmp->data << ":(" << flag << ") means NULL)";
        std::cin >> ldata >> rdata;
        if(ldata != flag)
            que.enQueue(tmp->left = new Node(ldata));
        if(rdata != flag)
            que.enQueue(tmp->right = new Node(rdata));
    }
    
    std::cout << "\nfinish create!\n";
    
}

template <class T>
void printTree(const binaryTree<T> &t,T flag) {
    linkQueue<T> que;
    que.enQueue(t.root->data);
    std::cout << std::endl;
    while(!que.isEmpty()){
        T p,l,r;
        p = que.deQueue();
        l = t.lchild(p, flag);
        r = t.rchild(p, flag);
        std::cout << p << " " << l << " " << r << std::endl;
        if(l != flag)
            que.enQueue(l);
        if(r != flag)
            que.enQueue(r);
    }
}

template <class elemType>
int binaryTree<elemType>::size(binaryTree<elemType>::Node *t)const {
    if(t == NULL)
        return 0;
    return 1 + size(t->left) + size(t->right);
}

template <class elemType>
int binaryTree<elemType>::size()const {
    return size(root);
}

template <class elemType>
int binaryTree<elemType>::height(binaryTree<elemType>::Node *t)const {
    if(t == NULL)
        return 0;
    else{
        int lt = height(t->left),rt = height(t->right);
        return 1 + ((lt > rt)?lt:rt);
    }
}

template <class elemType>
void binaryTree<elemType>::NpreOrder()const {
    linkStack<Node *> stack;
    Node *current;
    
    std::cout << "\npreOrder:";
    stack.push(root);
    
    while(!stack.isEmpty()){
        current = stack.pop();
        std::cout << current->data;
        if(current -> right != NULL)
            stack.push(current->right);
        if(current -> left != NULL)
            stack.push(current->left);
    }
}

template <class elemType>
void binaryTree<elemType>::NmidOrder()const {
    linkStack<STNode > stack;
    STNode current(root);
    
    std::cout << "\nmidOrder:";
    stack.push(current);
    while(!stack.isEmpty()){
        current = stack.pop();
        if(++current.timesPop == 2){
            std::cout << current.node->data;
            if(current.node->right != NULL)
                stack.push(STNode(current.node->right));
            
        }
        else{
            stack.push(current);
            if(current.node->left != NULL)
                stack.push(STNode(current.node->left));
        }
    }
}

template <class elemType>
void binaryTree<elemType>::NpostOrder()const {
    linkStack<STNode > stack;
    STNode current(root);
    
    std::cout << "\npostOrder:";
    stack.push(current);
    
    while(!stack.isEmpty()){
        current = stack.pop();
        if((++current.timesPop) == 3){
            std::cout << current.node->data;
            continue;
        }
        stack.push(current);
        if(current.timesPop == 1){
            if(current.node->left != NULL){
                stack.push(STNode(current.node->left));
            }
        }
        else{
            if(current.node->right != NULL){
                stack.push(STNode(current.node->right));
            }
        }
    }
}

template <class elemType>
void binaryTree<elemType>::changeLR(binaryTree<elemType>::Node *t) {
    if(t == NULL)
        return ;
    Node * tmp;
    
    tmp = t->left;
    t->left = t->right;
    t->right = tmp;
    
    changeLR(t->left);
    changeLR(t->right);
}

template <class elemType>
void binaryTree<elemType>::changeLR() {
    changeLR(root);
}

template <class elemType>
int binaryTree<elemType>::countDegreeIsTwo(binaryTree<elemType>::Node *t) {
    if(t == NULL){
        return 0;
    }
    if(t->left != NULL && t->right != NULL){
        return 1 + countDegreeIsTwo(t->left) + countDegreeIsTwo(t->right);
    }else if (t->left == NULL || t->right == NULL){
        return countDegreeIsTwo(t->left) + countDegreeIsTwo(t->right);
    }else
        return 0;
}

template <class elemType>
int binaryTree<elemType>::countDegreeIsTwo() {
    return countDegreeIsTwo(root);
}
/*
 *********************************************
 使用数组实现的哈夫曼树，生成哈夫曼编码
 *********************************************
 */
template <class elemType>
class hfTree {
private:
    struct Node{
        elemType data;
        int weight;
        int parent,left,right;
    };
    
    Node *elem;
    int length;
    
public:
    struct hfCode{
        elemType data;
        std::string code;
    };
    
    hfTree(const elemType *v,const int *w,int size);
    void getCode(hfCode result []);
    ~hfTree(){delete [] elem;}
};

template <class elemType>
hfTree<elemType>::hfTree(const elemType *v,const int *w,int size) {
    const int MAX_INT = 32767;
    int min1,min2;
    int x,y;
    
    length = 2 * size;
    elem = new Node[length];
    
    for(int i = size;i < length;i++){
        elem[i].weight = w[i-size];
        elem[i].data = v[i-size];
        elem[i].parent = elem[i].left = elem[i].right = 0;
    }
    
    
    for(int i = size - 1;i > 0 ;i--){
        min1 = min2 = MAX_INT;
        x = y = 0;
        for(int j = i+1;j < length;j++){
            if(elem[j].parent == 0){
                if(elem[j].weight <= min1){
                    min2 = min1;
                    min1 = elem[j].weight;
                    x = y;
                    y = j;
                }else if(elem[j].weight < min2){
                    min2 = elem[j].weight;
                    x = j;
                }
            }
        }
        elem[i].weight = min1+min2;
        elem[i].left = x;
        elem[i].right = y;
        elem[i].parent = 0;
        elem[x].parent = i;
        elem[y].parent = i;
    }
}

template <class elemType>
void hfTree<elemType>::getCode(hfCode result []) {
    int size = length / 2;
    int p,s;
    
    for(int i = size;i < length;i++){
        result[i-size].data = elem[i].data;
        result[i-size].code = "";
        p = elem[i].parent;
        s = i;
        while(p){
            if(elem[p].left == s){
                result[i-size].code = '0' + result[i-size].code;
            }else{
                result[i-size].code = '1' + result[i-size].code;
            }
            s = p;
            p = elem[p].parent;
        }
        
    }
}
#endif /* tree_hpp */
