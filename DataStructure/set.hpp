//
//  set.hpp
//  数据结构学习 集合部分
//
//  Created by 杨磊 on 2019/5/6.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef set_hpp
#define set_hpp

/*
 *********************************************
 定义集合中的元素
 *********************************************
 */
template <class KEY,class OTHER>
struct SET {
    KEY key;
    OTHER other;
    
    SET(){}
    SET(KEY theKey,OTHER theOther){
        key = theKey;
        other = theOther;
    }
};

/*
 *********************************************
 静态查找表的查找函数实现
 *********************************************
 */
template <class KEY,class OTHER>//无序表的顺序查找
int seqSearch(SET<KEY,OTHER> data[],int size, const KEY &x) {
    //要求集合的第一个元素（下标为0)不存放数据
    data[0].key = x;
    int i = 0;
    for(i = size;x != data[i].key;i--);
    return i;
}

template <class KEY,class OTHER>//有序表的顺序查找
int oSeqSearch(SET<KEY,OTHER> data[],int size,const KEY &x) {
    //要求集合的第一个元素（下标为0)不存放数据
    data[0].key = x;
    int i = 0;
    for(i = size;x < data[i].key;i--);
    if(x == data[i].key)
        return i;
    else
        return 0;
}

template <class KEY,class OTHER>//有序表的二分查找
int binarySearch(SET<KEY,OTHER> data[],int size,const KEY &x) {
    int low = 1,high = size,mid;
    while(low <= high){
        mid = (high + low) / 2;
        if(x == data[mid].key)
            return mid;
        if(x < data[mid].key)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;
}

template <class KEY,class OTHER>//有序查找链表
class linkSet {
    //链表的节点内嵌类
    struct Node{
        SET<KEY,OTHER> data;
        Node *next;
        //使用SET构造节点
        Node(SET<KEY,OTHER> d,Node *n = NULL){
            data = d;
            next = n;
        }
    };
    
    Node *head;

public:
    linkSet(){}
    //使用SET数组构造链表
    linkSet(SET<KEY,OTHER> data[],int size){
        head = NULL;
        //使用头插法
        for(int i = size - 1;i >= 0;i--){
            head = new Node(data[i],head);
        }
    }
    //析构函数
    ~linkSet(){
        Node *tmp = head;
        //删除动态变量
        while(tmp != NULL){
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
        delete head;
    }
    //顺序查找函数的实现
    int seqSearch(const KEY &x){
        Node *tmp = head;
        int i = 1;
        while(tmp != NULL){
            if(tmp->data.key == x)
                return i;
            i++;
            tmp = tmp->next;
        }
        return 0;
    }
};


/*
 *********************************************
 动态查找表的抽象类
 *********************************************
 */
template <class KEY,class OTHER>
class dynamicSearchTable {
public:
    virtual SET<KEY,OTHER> * find(const KEY &x)const  = 0;
    virtual void insert(const SET<KEY,OTHER> &x) = 0;
    virtual void remove(const KEY &x) = 0;
    virtual ~dynamicSearchTable(){};
};

/*
 *********************************************
 使用二叉查找树实现的动态查找表
 *********************************************
 */
template <class KEY,class OTHER>
class binarySearchTree:public dynamicSearchTable<KEY, OTHER> {
    struct binaryNode{//链表实现
        SET<KEY,OTHER> data;
        binaryNode *left;
        binaryNode *right;
        
        binaryNode(const SET<KEY, OTHER> &thedata,binaryNode *lt = NULL,binaryNode *rt = NULL):data(thedata),left(lt),right(rt){}
    };
    binaryNode *root;
    
public:
    binarySearchTree();
    ~binarySearchTree();
    //基本操作：
    SET<KEY,OTHER> *find(const KEY &x)const;
    void insert(const SET<KEY,OTHER> &x);
    void remove(const KEY &x);
    //区间删除：
    void removeBigger(const KEY &x);
    void removeSmaller(const KEY &x);
    void removeBetween(const KEY &low,const KEY &high);
    //非递归实现：
    SET<KEY,OTHER> *NRfind(const KEY &x)const;
    void NRinsert(const SET<KEY,OTHER> &x);
    void NRremove(const KEY &x);
    
    bool isAVL()const;    
private:
    void insert(const SET<KEY,OTHER> &x,binaryNode* &t);
    void remove(const KEY &x,binaryNode* &t);
    SET<KEY,OTHER> *find(const KEY &x,binaryNode * t)const;
    void makeEmpty(binaryNode *t);
    
    void removeBigger(const KEY &x,binaryNode* &t);
    void removeSmaller(const KEY &x,binaryNode* &t);
    void removeBetween(const KEY &low,const KEY &high,binaryNode* &t);
    
    void NRinsert(const SET<KEY,OTHER> &x,binaryNode* &t);
    void NRremove(const KEY &x,binaryNode* &t);
    
    bool isAVL(const binaryNode *t)const;
    int height(const binaryNode *t)const;
};
//基本操作的实现
template <class KEY,class OTHER >
SET<KEY,OTHER>* binarySearchTree<KEY,OTHER>::find(const KEY &x)const {
    return find(x,root);
}

template <class KEY,class OTHER>
SET<KEY,OTHER>* binarySearchTree<KEY,OTHER>::find(const KEY &x,binaryNode *t) const{
    if(t == NULL || t->data.key == x){
        return (SET<KEY, OTHER> *)t;
    }
    if(x < t->data.key)
        return find(x,t->left);
    else
        return find(x,t->right);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::insert(const SET<KEY, OTHER> &x) {
    insert(x,root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::insert(const SET<KEY, OTHER> &x, binaryNode *&t) {
    if(t == NULL)
        t = new binaryNode(x,NULL,NULL);
    else if(x.key < t->data.key)
        insert(x,t->left);
    else
        insert(x,t->right);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::remove(const KEY &x) {
    remove(x,root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::remove(const KEY &x,binaryNode *&t) {
    if(t == NULL)return;
    if(x < t->data.key)remove(x,t->left);
    else if (x > t->data.key)remove(x,t->right);
    else if(t->left != NULL && t->right != NULL){
        binaryNode *tmp = t->right;
        while(tmp->left != NULL)
            tmp = tmp->left;
        t->data = tmp->data;
        remove(t->data.key,t->right);
    }else{
        binaryNode *oldNode = t;
        t = (t->left != NULL)?t->left:t->right;
        delete oldNode;
    }
}
//构造函数
template <class KEY,class OTHER>
binarySearchTree<KEY, OTHER>::binarySearchTree() {
    root = NULL;
}
//清除节点
template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::makeEmpty(binaryNode *t) {
    if(t == NULL)
        return ;
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
    
}
//析构函数
template <class KEY,class OTHER>
binarySearchTree<KEY, OTHER>::~binarySearchTree() {
    makeEmpty(root);
}

//区间删除
template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::removeBigger(const KEY &x) {
    removeBigger(x,root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::removeBigger(const KEY &x, binaryNode *&t){
    if(t == NULL)
        return;
    if(t->data.key == x){
        makeEmpty(t->right);
        t->right = NULL;
    }else if(t->data.key < x){
        removeBigger(x,t->right);
    }else{
        binaryNode *tmp = t;
        t = t->left;
        makeEmpty(tmp->right);
        tmp->right = NULL;
        removeBigger(x,t);
        delete tmp;
    }
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::removeSmaller(const KEY &x) {
    removeSmaller(x, root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::removeSmaller(const KEY &x,binaryNode *&t){
    if(t == NULL)
        return ;
    if(t->data.key == x){
        makeEmpty(t->left);
        t->left = NULL;
    }else if(t->data.key > x){
        removeSmaller(x, t->left);
    }else if(t->data.key < x){
        binaryNode *tmp = t;
        t = t->right;
        makeEmpty(tmp->left);
        tmp->left = NULL;
        removeSmaller(x,t);
        delete tmp;
    }
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::removeBetween(const KEY &low, const KEY &high) {
    removeBetween(low, high, root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::removeBetween(const KEY &low, const KEY &high,binaryNode *&t) {
    if(t == NULL)
        return ;
    
    if(t->data.key <= low){
        removeBetween(low, high, t->right);
    }else if(t->data.key >= high){
        removeBetween(low, high, t->left);
    }else{
        removeBetween(low,high,t->left);
        removeBetween(low,high,t->right);
        remove(t->data.key, t);
    }
}

//查询的非递归实现
template <class KEY,class OTHER>
SET<KEY,OTHER> * binarySearchTree<KEY,OTHER>::NRfind(const KEY &x)const{
    binaryNode *t = root;
    while(t != NULL && t->data.key != x){
        if(t->data.key > x)
            t = t->left;
        else
            t = t->right;
    }
    
    if(t == NULL)
        return NULL;
    else
        return (SET<KEY,OTHER> *)t;
}

//插入的非递归实现
template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::NRinsert(const SET<KEY,OTHER> &x){
    NRinsert(x,root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::NRinsert(const SET<KEY,OTHER> &x,binaryNode* &t){
    
    if(t == NULL){
        t = new binaryNode(x,NULL,NULL);
        return ;
    }
    
    binaryNode *tmp = t;
    
    while(t != NULL && t->data.key != x.key){
        if(t->data.key > x.key && t->left != NULL)
            t = t->left;
        else if(t->data.key < x.key && t->right != NULL)
            t = t->right;
        else
            break;
    }
    
    if(t ->data.key > x.key){
        t->left = new binaryNode(x,NULL,NULL);
    }else if(t->data.key < x.key){
        t->right = new binaryNode(x,NULL,NULL);
    }

    t = tmp;
}

//删除的非递归实现
template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::NRremove(const KEY &x){
    NRremove(x,root);
}

template <class KEY,class OTHER>
void binarySearchTree<KEY,OTHER>::NRremove(const KEY &x,binaryNode* &t){
    binaryNode *backup = t;
    
    if(!NRfind(x))
        return ;
    
    if(t == NULL)
        return ;
    
    while(t->data.key != x && t != NULL){
        if(t->data.key > x)
            t = t->left;
        else
            t = t->right;
       
    }
    
    if(t->left != NULL && t->right != NULL){
        binaryNode *tmp = t->right;
        while(tmp -> left != NULL && tmp->left->left != NULL){
            tmp = tmp -> left;
        }
        if(tmp -> left == NULL){
            t->data = tmp->data;
            t->right = tmp->right;
            delete tmp;
        }else{
            binaryNode *tt = tmp->left;
            tmp->left = tt->right;
            delete tt;
        }
    }else{
        binaryNode *ff = backup;
        while(ff -> left != t && ff->right != t){
            if(ff->data.key > x)
                ff = ff->left;
            else
                ff = ff->right;
        }
        if(ff -> left == t)
            ff->left = (t->left != NULL)? t->left:t->right;
        else
            ff->right = (t->left != NULL)? t->left:t->right;
        delete t;
    }
    
    t = backup;
}


/*
 *********************************************
 使用AVL平衡树实现的动态查找表
 *********************************************
 */
template <class KEY,class OTHER>
class AVLTree: public dynamicSearchTable<KEY, OTHER> {
    struct AVLNode{
        SET<KEY,OTHER> data;
        AVLNode *left;
        AVLNode *right;
        int height;
        
        AVLNode(const SET<KEY,OTHER> &element,AVLNode *lt,AVLNode *rt,int h = 1):data(element),left(lt),right(rt),height(h){}
        
    };
    
    AVLNode *root;
    
public:
    AVLTree(){root = NULL;}
    ~AVLTree(){makeEmpty(root);}
    SET<KEY,OTHER> *find(const KEY &x)const;
    void insert(const SET<KEY,OTHER> &x);
    void remove(const KEY &x);
    
private:
    void insert(const SET<KEY,OTHER> &x,AVLNode *&t);
    bool remove(const KEY &x,AVLNode *&t);
    void makeEmpty(AVLNode *t);
    int height(AVLNode *t)const{
        return t==NULL?0:t->height;
    }
    //节点的调整
    void LL(AVLNode *&t);
    void LR(AVLNode *&t);
    void RL(AVLNode *&t);
    void RR(AVLNode *&t);
    int max(int a,int b){return (a>b)?a:b;}
    bool adjust(AVLNode *&t,int subTree);
};

template <class KEY,class OTHER>
SET<KEY, OTHER> * AVLTree<KEY,OTHER>::find(const KEY &x)const {
    AVLNode *t = root;
    
    while(t != NULL && t->data.key != x){
        if(t->data.key > x)
            t = t->left;
        else
            t = t->right;
    }
    
    if(t == NULL)
        return NULL;
    else
        return (SET<KEY, OTHER> *)t;
}

//节点的调整
template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::LL(AVLNode *&t) {
    AVLNode *tl = t->left;
    t->left = tl->right;
    tl->right = t;
    t->height = max(height(t->left),height(t->right))+1;
    tl->height = max(height(tl->left),height(t))+1;
    t = tl;
}

template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::RR(AVLNode *&t) {
    AVLNode *tl = t->right;
    t->right = tl->left;
    tl->left = t;
    t->height = max(height(t->left),height(t->right))+1;
    tl->height = max(height(tl->right),height(t))+1;
    t = tl;
}

template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::LR(AVLNode *&t) {
    RR(t->left);
    LL(t);
}

template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::RL(AVLNode *&t) {
    LL(t->right);
    RR(t);
}

//基本操作：
template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::insert(const SET<KEY, OTHER> &x) {
    insert(x,root);
}

template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::insert(const SET<KEY, OTHER> &x, AVLNode *&t) {
    if(t == NULL){
        t = new AVLNode(x,NULL,NULL);
    }else if(x.key < t->data.key){
        insert(x,t->left);
        if(height(t->left) - height(t->right) == 2){
            if(x.key < t->left->data.key)
                LL(t);
            else
                LR(t);
        }
    }else if(x.key > t->data.key){
        insert(x,t->right);
        if(height(t->right) - height(t->left) == 2){
            if(x.key > t->right->data.key)
                RR(t);
            else
                RL(t);
        }
    }
    
    t->height = max(height(t->left),height(t->right)) + 1;
}

template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::remove(const KEY &x) {
    remove(x,root);
}

template <class KEY,class OTHER>
bool AVLTree<KEY,OTHER>::remove(const KEY &x, AVLNode *&t) {
    if(t == NULL)
        return true;
    if(x == t->data.key){
        if(t->left == NULL || t->right == NULL){
            AVLNode *oldNode = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete oldNode;
            return false;
        }else{
            AVLNode *tmp = t->right;
            while(tmp->left != NULL)
                tmp = tmp->left;
            t->data = tmp->data;
            if(remove(tmp->data.key,t->right))
                return true;
            return adjust(t,1);
        }
    }
    
    if(x < t->data.key){
        if(remove(x,t->left))
            return true;
        return adjust(t,0);
    }else{
        if(remove(x,t->right))
            return true;
        return adjust(t,1);
    }
}

//节点的调整
template <class KEY,class OTHER>
bool AVLTree<KEY,OTHER>::adjust(AVLNode *&t, int subTree) {
    if(subTree){
        if(height(t->left) - height(t->right) == 1)
            return true;
        if(height(t->right) == height(t->left)){
            t->height -- ;
            return false;
        }
        if(height(t->left->right) > height(t->left->left)){
            LR(t);
            return false;
        }
        LL(t);
        if(height(t->right) == height(t->left))
            return false;
        else
            return true;
    }else{
        if(height(t->right) - height(t->left) == 1)
            return true;
        if(height(t->right) == height(t->left)){
            t->height --;
            return false;
        }
        if(height(t->right->left) > height(t->right->right)){
            RL(t);
            return false;
        }
        RR(t);
        if(height(t->right) == height(t->left))
            return false;
        else
            return true;
    }
}

template <class KEY,class OTHER>
void AVLTree<KEY,OTHER>::makeEmpty(AVLNode *t) {
    if(t == NULL)
        exit(0);
    else
        makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
}

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


/*
 *********************************************
 使用数组实现的不相交集
 *********************************************
 */
class DisjointSet{
private:
    int size;//数组的规模
    int *parent;
    
public:
    DisjointSet(int n);
    ~DisjointSet(){delete [] parent;}
    void Union(int root1, int root2);
    int Find(int x);
};

DisjointSet::DisjointSet(int n){
    size = n;
    parent = new int [size];
    for(int i = 0; i < size;i++){
        parent[i] = -1;//所有的都是规模为1的根节点
    }
}

int DisjointSet::Find(int x){
    if(parent[x] < 0)//找到根节点
        return x;
    //将路径上所有的点直接与根节点相连：路径压缩
    return parent[x] = Find(parent[x]);
}

void DisjointSet::Union(int root1, int root2){
    if(root1 == root2)
        return ;
    if(parent[root1] > parent[root2]){//root1规模比较小
        parent[root2] += parent[root1];//root1归并到root2上
        parent[root1] = root2;
    }else{
        parent[root1] += parent[root2];//root2归并到root1上
        parent[root2] = root1;
    }
}
#endif /* set_hpp */
