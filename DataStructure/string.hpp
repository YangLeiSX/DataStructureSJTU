//
//  string.hpp
//  数据结构学习 字符串部分
//
//  Created by 杨磊 on 2019/6/29.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef string_hpp
#define string_hpp

#include <iostream>
/*
 *********************************************
 使用顺序实现的字符串
 *********************************************
 */
class seqString{
    //使用运算符重载实现的字符串运算
    friend seqString operator+(const seqString &s1, const seqString &s2);
    friend bool operator==(const seqString &s1, const seqString &s2);
    friend bool operator!=(const seqString &s1, const seqString &s2);
    friend bool operator>(const seqString &s1, const seqString &s2);
    friend bool operator>=(const seqString &s1, const seqString &s2);
    friend bool operator<(const seqString &s1, const seqString &s2);
    friend bool operator<=(const seqString &s1, const seqString &s2);
    friend std::ostream &operator<<(std::ostream &os, const seqString &x);
    
    //数据成员
    char *data;
    int len;

public:
    seqString(const char *s="");
    seqString(const seqString &other);
    ~seqString();
    int length() const;
    seqString &operator=(const seqString &other);
    seqString subStr(int start, int num) const;
    void insert(int start,const seqString &s);
    void remove(int start, int num);
    
    //模式匹配(查找子串)
    int find(const seqString &sub)const;
};

seqString::seqString(const char *s){
    for(len = 0;s[len] != '\0';len++);
    data = new char[len+1];
    for(len = 0;s[len] != '\0';len++)
        data[len] = s[len];
    data[len] = '\0';
}

seqString::seqString(const seqString &other){
    data = new char[other.len + 1];
    for(len = 0;len <= other.len ;len++){
        data[len] = other.data[len];
    }
}

seqString::~seqString(){
    delete data;
}

int seqString::length() const{
    return len;
}

seqString &seqString::operator=(const seqString &other){
    if(this == &other)
        return *this;
    delete data;
    data = new char[other.len+1];
    for(len = 0;len <= other.len;len++)
        data[len] = other.data[len];
    return *this;
}

seqString seqString::subStr(int start, int num)const{
    if(start >= len-1 || start < 0)
        return "";
    seqString tmp;
    //子串的实际长度
    tmp.len = (start+num > len) ? len - start : num;
    delete tmp.data;
    tmp.data = new char[tmp.len +1];
    for(int i = 0; i < tmp.len;i++){
        tmp.data[i] = data[start+i];
    }
    tmp.data[tmp.len] = '\0';
    return tmp;
}

void seqString::insert(int start, const seqString &s){
    char *tmp = data;
    int i;
    
    if(start > len || start < 0)
        return ;
    len += s.len;
    data = new char [len+1];
    for(i = 0;i < start;i++)
        data[i] = tmp[i];
    for(i = 0; i < s.len;i++)
        data[start+i] = s.data[i];
    for(i = start;tmp[i] != '\0';i++)
        data[i+s.len] = tmp[i];
    data[i+s.len] = '\0';
    delete tmp;
}

void seqString::remove(int start, int num){
    if(start >= len-2 || start < 0)
        return ;
    if(start + num >= len){
        data[start] = '\0';
        len = start;
    }else{
        for(len = start; data[len + num] != '\0';len++)
            data[len] = data[len + num];
        data[len] = '\0';
    }
}

std::ostream &operator<<(std::ostream &os, const seqString &s){
    os << s.data;
    return os;
}

seqString operator+(const seqString &s1, const seqString &s2){
    seqString tmp;
    int i;
    
    tmp.len = s1.len + s2.len;
    delete tmp.data;
    tmp.data = new char[tmp.len+1];
    for(i = 0; i < s1.len;i++)
        tmp.data[i] = s1.data[i];
    for(i = 0; i < s2.len;i++)
        tmp.data[s1.len +i] = s2.data[i];
    tmp.data[s1.len + s2.len] = '\0';
    return tmp;
}

bool operator==(const seqString &s1, const seqString &s2){
    if(s1.len != s2.len)
        return false;
    for(int i = 0;i < s1.len;i++){
        if(s1.data[i] != s2.data[i])
            return false;
    }
    return false;
}

bool operator!=(const seqString &s1, const seqString &s2){
    return !(s1 == s2);
}

bool operator>(const seqString &s1, const seqString &s2){
    for(int i = 0; i < s1.len;i++){
        if(s1.data[i] > s2.data[i])
            return true;
        else if(s1.data[i] < s2.data[i])
            return false;
    }
    return false;
}

bool operator>=(const seqString &s1, const seqString &s2){
    return (s1 == s2 || s1 > s2);
}

bool operator<(const seqString &s1, const seqString &s2){
    return !(s1 >= s2);
}

bool operator<=(const seqString &s1, const seqString &s2){
    return !(s1 > s2);
}

//使用KMP算法实现子串匹配和查找
int seqString::find(const seqString &sub)const{
    int *p = new int[sub.len];
    int i,j;
    
    //生成失效函数,表示一次匹配失败后下标j的变化
    p[0] = -1;
    for(i = 1;i < sub.len;i++){
        j = i - 1;
        while(j >= 0 && sub.data[p[j]+1] != sub.data[i])
            j = p[j];
        if(j < 0)
            p[i] = -1;
        else
            p[i] = p[j] + 1;
    }
    
    //开始查找
    i = j = 0;
    while(i < len && j < sub.len){
        if(sub.data[j] == data[i]){
            i++;
            j++;
        }else if(j == 0){
            i++;
        }else{
            j = p[j-1] + 1;
        }
    }
    
    delete []p;
    
    if(j == sub.len)
        return i-j;//返回找到的位置
    else
        return -1;//没找到
}

/*
 *********************************************
 使用链接实现的字符串
 *********************************************
 */
class linkString{
    friend linkString operator+(const linkString &s1, const linkString &s2);
    friend bool operator==(const linkString &s1, const linkString &s2);
    friend bool operator!=(const linkString &s1, const linkString &s2);
    friend bool operator>(const linkString &s1, const linkString &s2);
    friend bool operator>=(const linkString &s1, const linkString &s2);
    friend bool operator<(const linkString &s1, const linkString &s2);
    friend bool operator<=(const linkString &s1, const linkString &s2);
    friend std::ostream &operator<<(std::ostream &os, const linkString &s);
    
    struct node{
        int size;
        char *data;
        node *next;
        node(int s = 1, node *n = NULL){
            data = new char[s];
            size = 0;
            next = n;
        }
    };
    
    node *head;
    int len;//总的字符串长度
    int nodeSize;//每个节点的规模
    
    void clear();
    void findPos(int start, int &pos,node *&p)const;
    void split(node *p, int pos);
    void merge(node *p);
    
public:
    linkString(const char *s="");
    linkString(const linkString &other);
    ~linkString();
    int length()const;
    linkString &operator=(const linkString&other);
    linkString subStr(int start, int num)const;
    void insert(int start, const linkString &s);
    void remove(int start, int num);
};

linkString::linkString(const char *s){
    node *p;
    //确定字符串长度和块的大小
    for(len = 0;s[len] != '\0';len++);
    nodeSize = sqrt(len);
    //头节点
    p = head = new node(1);
    while(*s){
        p = p->next = new node(nodeSize);
        for(;p->size < nodeSize && *s;p->size++,s++){
            p->data[p->size] = *s;
        }
    }
}

linkString::linkString(const linkString &other){
    node *p, *otherp = other.head->next;
    p = head = new node(1);
    len = other.len;
    nodeSize = other.nodeSize;
    while(otherp){
        p = p->next = new node[nodeSize];
        //新节点的size为0，通过循环复制节点
        for(;p->size < otherp->size;p->size++){
            p->data[p->size] = otherp->data[p->size];
        }
        otherp = otherp->next;
    }
}

void linkString::clear(){
    node *p = head->next, *nextp;
    while(p){
        nextp = p->next;
        delete p;
        p = nextp;
    }
}

linkString::~linkString(){
    clear();
    delete head;
}

int linkString::length()const{
    return len;
}

linkString &linkString::operator=(const linkString &other){
    if(this == &other) return *this;
    
    node *p = head, *otherp = other.head->next;
    
    clear();
    len = other.len;
    nodeSize = other.nodeSize;
    //复制块状链表
    while(otherp){
        p = p->next = new node[nodeSize];
        for(;p->size < otherp->size; p->size++){
            p->data[p->size] = otherp->data[p->size];
        }
        otherp = otherp->next;
    }
    return *this;
}

void linkString::findPos(int start, int &pos, node *&p)const{
    int count = 0;
    p = head->next;
    
    while(count < start){
        if(count + p->size < start){
            count += p->size;
            p = p->next;
        }else{
            pos = start - count;
            return ;
        }
    }
}

linkString linkString::subStr(int start, int num)const{
    linkString tmp;
    int pos;
    node *p,*tp = tmp.head;
    
    if(start < 0 || start >= len - 1)
        return tmp;
    
    num = (start + num > len) ? len-start : num;
    tmp.len = num;
    tmp.nodeSize = sqrt(num);
    
    findPos(start, pos, p);
    
    for(int i = 0; i< tmp.len;){
        tp = tp->next = new node(tmp.nodeSize);
        for(;tp->size < tmp.nodeSize && i < tmp.len;tp->size++,i++){
            if(pos == p->size){
                p = p->next;
                pos = 0;
            }
            tp->data[tp->size] = p->data[pos++];
        }
    }
    
    return tmp;
}

void linkString::split(node *p, int pos){
    p->next = new node(nodeSize, p->next);
    for(int i = pos;i < p->size;i++){
        //p->next->data[i-pos] = p->data[pos];//这里有问题
        p->next->data[i-pos] = p->data[i];
    }
    p->next->size = p->size - pos;
    p->size = pos;
}

void linkString::merge(node *p){
    node *nextp = p->next;
    if(p->size + nextp->size <= nodeSize){
        for(int pos = 0;pos < nextp->size;pos++,p->size++){
            p->data[p->size] = nextp->data[pos];
        }
        p->next = nextp->next;
        delete nextp;
    }
}

void linkString::insert(int start, const linkString &s){
    node *p,*nextp,*tmp;
    int pos;
    
    if(start < 0 || start > len)return ;
    findPos(start, pos, p);
    split(p, pos);
    nextp = p->next;
    tmp = s.head->next;
    while(tmp){
        for(pos = 0; pos < tmp->size;pos++){
            if(p->size == nodeSize)
                p = p->next = new node(nodeSize);
            p->data[p->size] = tmp->data[pos];
            p->size++;
        }
        tmp = tmp->next;
    }
    len += s.len;
    p->next = nextp;
    merge(p);
}

void linkString::remove(int start, int num){
    if(start < 0 || start >= len - 1)
        return ;
    node *startp;
    int pos;
    findPos(start, pos, startp);
    split(startp,pos);
    if(start + num >= len){
        num = len - start;
        len = start;
    }
    else
        len -= num;
    
    while(true){
        node *nextp = startp->next;
        if(num > nextp->size){
            num -= nextp->size;
            startp->next = nextp->next;
            delete nextp;
        }else{
            split(nextp, num);
            startp->next = nextp->next;
            delete nextp;
            break;
        }
    }
    merge(startp);
}

std::ostream &operator<<(std::ostream &os, const linkString &s){
    linkString::node *p = s.head->next;
    int pos = 0;
    
    while(p){
        for(pos = 0;pos < p->size;pos++){
            os << p->data[pos];
        }
        p = p->next;
        //os << "_";//用于split和remove的测试
    }
    return os;
}

linkString operator+(const linkString &s1, const linkString &s2){
    char *tmp = new char[s1.len + s2.len + 1];
    linkString::node *p;
    int count = 0,i;
    
    for(p = s1.head->next;p!= NULL;p = p->next){
        for(i = 0; i < p->size;i++)
            tmp[count++] = p->data[i];
    }
    for(p = s2.head->next;p!=NULL;p = p->next){
        for(i = 0;i < p->size;i++)
            tmp[count++] = p->data[i];
    }
    tmp[count]= '\0';
    linkString returnValue(tmp);
    delete[] tmp;
    return returnValue;
}

bool operator==(const linkString &s1, const linkString &s2){
    linkString::node *p1 = s1.head->next, *p2 = s2.head->next;
    int pos1 = 0, pos2 = 0;
    
    if(s1.len != s2.len)
        return false;
    while (p1 && p2) {
        if(p1->data[pos1] != p2->data[pos2])
            return false;
        if(++pos1 == p1->size){
            p1 = p1->next;
            pos1 = 0;
        }
        if(++pos2 == p2->size){
            p2 = p2->next;
            pos2 = 0;
        }
    }
    return true;
}

bool operator!=(const linkString &s1, const linkString &s2){
    return !(s1 == s2);
}

bool operator>(const linkString &s1, const linkString &s2){
    linkString::node *p1 = s1.head->next, *p2 = s2.head->next;
    int pos1 = 0, pos2 = 0;
    
    while(p1){
        if(p2 == NULL)  return true;
        if(p1->data[pos1] > p2->data[pos2]) return true;
        if(p1->data[pos1] < p2->data[pos2]) return false;
        if(++pos1 == p1->size){
            p1 = p1->next;
            pos1 = 0;
        }
        if(++pos2 == p2->size){
            p2 = p2->next;
            pos2 = 0;
        }
    }
    return false;
}

bool operator>=(const linkString &s1, const linkString &s2){
    return (s1 == s2 || s1 > s2);
}

bool operator<(const linkString &s1, const linkString &s2){
    return !(s1 >= s2);
}
bool operator<=(const linkString &s1, const linkString &s2){
    return !(s1 > s2);
}


#endif /* string_hpp */
