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
 使用红黑树实现的动态查找表
 *********************************************
 */
template <class KEY, class OTHER>
class RedBlackTree:public dynamicSearchTable<KEY, OTHER> {
    enum colorT{RED, BLACK};
    struct RedBlackNode{
        SET<KEY, OTHER> data;
        RedBlackNode *left;
        RedBlackNode *right;
        colorT colour;
        
        RedBlackNode(const SET<KEY, OTHER> &element, RedBlackNode *lt = NULL, RedBlackNode *rt = NULL, colorT h = RED)
        :data(element),left(lt),right(rt),colour(h){}
    };
    
    RedBlackNode *root;
    
public:
    RedBlackTree(RedBlackNode *t = NULL){
        root = t;
    }
    ~RedBlackTree(){makeEmpty(root);}
    SET<KEY, OTHER> *find(const KEY &x)const;
    void insert(const SET<KEY, OTHER> &x);
    void remove(const KEY &x);
    
private:
    void makeEmpty(RedBlackNode *&t);
    void insertAdjust(RedBlackNode *gp, RedBlackNode *p, RedBlackNode *t);
    void removeAdjust(RedBlackNode *&p, RedBlackNode *&c, RedBlackNode *&t, KEY del);
    void LL(RedBlackNode *gp);
    void RR(RedBlackNode *gp);
    void LR(RedBlackNode *gp);
    void RL(RedBlackNode *gp);
};

template <class KEY, class OTHER>
SET<KEY, OTHER> *RedBlackTree<KEY, OTHER>::find(const KEY &x)const {
    RedBlackNode *t = root;
    while(t != NULL && t->data.key != x)
        if(t->data.key > x)
            t = t->left;
        else
            t = t->right;
    
    if(t == NULL)
        return NULL;
    else
        return (SET<KEY, OTHER> *)t;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::makeEmpty(RedBlackNode *&t) {
    if(t != NULL){
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

//调整函数
template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::LL(RedBlackNode *gp) {
    RedBlackNode *p = gp->left, *t = p->left;
    RedBlackNode tmp = *gp;
    
    gp->data = p->data;
    gp->left = t;
    gp->right = p;
    p->data = tmp.data;
    p->left = p->right;
    p->right = tmp.right;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::LR(RedBlackNode *gp) {
    RedBlackNode *p = gp->left, *t= p->right;
    RedBlackNode tmp = *gp;
    
    gp->data = t->data;
    gp->right = t;
    p->right = t->left;
    t->data = tmp.data;
    t->left = t->right;
    t->right = tmp.right;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::RR(RedBlackNode *gp) {
    RedBlackNode *p = gp->right, *t = p->right;
    RedBlackNode tmp = *gp;
    
    gp->data = p->data;
    gp->right = t;
    gp->left = p;
    p->data = tmp.data;
    p->right = p->left;
    p->left = tmp.left;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::RL(RedBlackNode *gp) {
    RedBlackNode *p = gp->right, *t = p->left;
    RedBlackNode tmp = *gp;
    
    gp->data = t->data;
    gp->left = t;
    p->left = t->right;
    t->data = tmp.data;
    t->right = t->left;
    t->left = tmp.left;
}

//插入过程
//没有注意节点已经存在的情况
template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::insert(const SET<KEY, OTHER> &x) {
    RedBlackNode *t, *parent, *grandP;
    
    if(root == NULL){
        root = new RedBlackNode(x,NULL,NULL,BLACK);
        return ;
    }
    
    parent = grandP = t = root;
    while(true){
        if(t){
            if(t->left && t->left->colour == RED && t->right && t->right->colour == RED){
                t->left->colour = t->right->colour = BLACK;
                t->colour = RED;
                insertAdjust(grandP, parent, t);
            }
            grandP = parent;
            parent = t;
            t = (t->data.key > x.key ? t->left : t->right);
        }else{
            t = new RedBlackNode(x);
            if(x.key < parent->data.key)
                parent->left = t;
            else
                parent->right = t;
            insertAdjust(grandP, parent, t);
            root->colour = BLACK;
            return ;
        }
    }
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::insertAdjust(RedBlackNode *gp, RedBlackNode *p, RedBlackNode *t) {
    if(p->colour)//没有破坏性质
        return ;
    if(p == root){//根结点直接变色，不会影响
        p->colour = BLACK;
        return ;
    }
    if(gp->left == p)
        if(p->left == t)
            LL(gp);
        else
            LR(gp);
    else
        if(p->right == t)
            RR(gp);
        else
            RL(gp);
}

//删除过程
template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::remove(const KEY &x) {
    KEY del = x;
    RedBlackNode *p, *t, *c;
    
    if(root == NULL)
        return ;
    if(root->data.key == x && root->left == NULL && root->right == NULL){
        delete root;
        root = NULL;
        return ;
    }
    
    p = c = t = NULL;
    while(true){
        removeAdjust(p, c, t, del);
        if(c->data.key == del && c->left && c->right){
            RedBlackNode *tmp = c->right;
            while(tmp ->left)
                tmp = tmp->left;
            c->data = tmp->data;
            del = tmp->data.key;
            p = c;
            c = c->right;
            t = p->left;
            continue;
        }
        
        if(c->data.key == del){
            delete c;
            if(p->left == c)
                p->left = NULL;
            else
                p->right = NULL;
            root->colour = BLACK;
            return ;
        }
        
        p = c;
        c = (del < p->data.key ? p->left : p->right);
        t = ( c == p->left ? p->right : p->left);
    }
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::removeAdjust(RedBlackNode *&p, RedBlackNode *&c, RedBlackNode *&t, KEY del) {
    if(c->colour == RED)
        return ;
    if(c == root)
        if(c->left && c->right && c->right->colour == c->left->colour){
            c->colour = RED;
            c->left->colour = c->right->colour = BLACK;
            return ;
        }
    
    if(((c->left && c->left->colour) || c->left == NULL) && ((c->right && c->right->colour) || c->right == NULL))
        if(((t->left && t->left->colour) || t->left == NULL) && ((t->right && t->right->colour) || t->right == NULL)){
            p->colour = BLACK;
            t->colour = c->colour = RED;
        }else{
            if(p->left == t)
                if(t->left && t->right->colour == RED){
                    t->left->colour = BLACK;
                    LL(p);
                    p = t;
                }else{
                    LR(p);
                    p = p->right;
                    p->colour = BLACK;
                }
            else if(t->right && t->right->colour == RED){
                t->right->colour = BLACK;
                RR(p);
                p = t;
            }else{
                RL(p);
                p = p->left;
                p->colour = BLACK;
            }
            c->colour = RED;
        }
        else{
            if(c ->data.key == del){
                if(c ->left && c->right){
                    if(c->right->colour == BLACK){
                        LL(c);
                        c = c->right;
                    }
                    return ;
                }
                if(c->left){
                    LL(c);
                    p = c;
                    c = c->right;
                }else{
                    RR(c);
                    p = c;
                    c = c->left;
                }
            }else{
                p = c;
                c = (del < p->data.key ? p->left : p->right);
                t = ( c == p->left? p->right : p->left);
                if(c->colour == BLACK){
                    if(t == p->right)
                        RR(p);
                    else
                        LL(p);
                    p = t;
                    t = ( c == p->left ? p->right : p->left);
                    removeAdjust(p, c, t, del);
                }
            }
        }
}


/*
 *********************************************
 使用AA树实现的动态查找表
 *********************************************
 */
template <class KEY, class OTHER>
class AATree:public dynamicSearchTable<KEY, OTHER> {
    struct AANode{
        SET<KEY, OTHER> data;
        AANode *left;
        AANode *right;
        int level;
        
        AANode(const SET<KEY, OTHER> &x, AANode *lt, AANode *rt, int lv = 1):data{x},left(lt),right(rt),level(lv){}
    };
    AANode *root;
    
public:
    AATree(){root = NULL;}
    ~AATree(){makeEmpty(root);}
    SET<KEY, OTHER> *find(const KEY &x)const;
    void insert(const SET<KEY, OTHER> &x){insert(x, root);}
    void remove(const KEY &x){remove(x, root);}
    
private:
    void insert(const SET<KEY, OTHER> &x,AANode *&t);
    void remove(const KEY &x, AANode *&t);
    void makeEmpty(AANode *&t);
    void LL(AANode *&t);
    void RR(AANode *&t);
    int min(int a, int b){return a < b ? a : b;}
};

template <class KEY, class OTHER>
SET<KEY, OTHER> *AATree<KEY, OTHER>::find(const KEY &x)const {
    AANode *t = root;
    while( t != NULL && t->data.key != x)
        if(t->data.key > x)
            t = t->left;
        else
            t = t->right;
    if(t == NULL)
        return NULL;
    else
        return (SET<KEY, OTHER> *)t;
}

template <class KEY, class OTHER>
void AATree<KEY, OTHER>::makeEmpty(AANode *&t) {
    if(t != NULL){
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
}

//辅助函数
template <class KEY, class OTHER>
void AATree<KEY, OTHER>::LL(AANode *&t) {
    if(t->left != NULL && t->left->level == t->level){
        AANode *tmp = t->left;
        t->left = tmp->right;
        tmp->right = t;
        t = tmp;
    }
}

template <class KEY, class OTHER>
void AATree<KEY, OTHER>::RR(AANode *&t) {
    if(t->right != NULL && t->right->right != NULL && t->right->right->level == t->level){
        AANode *tmp = t->right;
        t->right = tmp->left;
        tmp->left = t;
        t = tmp;
        t->level ++;
    }
}

//插入操作
template <class KEY, class OTHER>
void AATree<KEY, OTHER>::insert(const SET<KEY, OTHER> &x, AANode *&t) {
    if(t == NULL)
        t = new AANode(x, NULL, NULL);
    else if(x.key < t->data.key )
        insert(x, t->left);
    else if(t->data.key < x.key)
        insert(x, t->right);
    else
        return ;
    
    LL(t);
    RR(t);
}

//删除操作
template <class KEY, class OTHER>
void AATree<KEY, OTHER>::remove(const KEY &x, AANode *&t) {
    if(t == NULL)
        return ;
    if(x < t->data.key)
        remove(x, t->left);
    else if(t->data < x)
        remove(x, t->right);
    else if(t->left != NULL && t->right != NULL){
        AANode *tmp = t->right;
        while(tmp->left != NULL)
            tmp = tmp -> left;
        t->data = tmp->data;
        remove(t->data.key, t->right);
    }
    else{
        AANode *oldNode = t;
        t = (t -> left != NULL)? t->left : t->right;
        delete oldNode;
        return ;
    }
    
    //修改节点层次
    if(t ->left == NULL || t->right == NULL)
        t->level = 1;
    else
        t->level = min(t->left->level, t->right->level) + 1;
    if(t->right != NULL && t->right->level > t->level)
        t->right->level = t->level;
    LL(t);
    if(t->right != NULL)
        LL(t->right);
    if(t->right != NULL && t->right->right != NULL)
        LL(t->right->right);
    RR(t);
    if(t->right != NULL )
        RR(t->right);
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
    int num;// 不相交元素的个数
public:
    DisjointSet(int n);
    ~DisjointSet(){delete [] parent;}
    void Union(int root1, int root2);
    int Find(int x);
    int Num();
};

DisjointSet::DisjointSet(int n){
    size = n;
    parent = new int [size];
    for(int i = 0; i < size;i++){
        parent[i] = -1;//所有的都是规模为1的根节点
    }
    num = n;
}

int DisjointSet::Find(int x){
    if(parent[x] < 0)//找到根节点
        return x;
    //将路径上所有的点直接与根节点相连：路径压缩
    return parent[x] = Find(parent[x]);
}

void DisjointSet::Union(int root1, int root2){
    root1 = Find(root1);
    root2 = Find(root2);
    if(root1 == root2)
        return ;
    if(parent[root1] > parent[root2]){//root1规模比较小
        parent[root2] += parent[root1];//root1归并到root2上
        parent[root1] = root2;
    }else{
        parent[root1] += parent[root2];//root2归并到root1上
        parent[root2] = root1;
    }
    num --;
}

int DisjointSet::Num(){
    for(int i = 0;i < size;i++){
        std::cout << i << "\t" << parent[i] << std::endl;
    }
    return this->num;
}

#endif /* set_hpp */
