//
//  queue.hpp
//  Stest
//
//  Created by 杨磊 on 2019/4/2.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef queue_hpp
#define queue_hpp

#include <stdio.h>
#include <iostream>
/*
 *********************************************
 定义抽象接口类 用于规定所有的队列的功能
 *********************************************
 */
template <class elemType>
class queue {
public:
    virtual bool isEmpty() const =0;
    virtual void enQueue(const elemType &x)=0;
    virtual elemType deQueue() = 0;
    virtual elemType getHead()const =0;
    virtual ~queue(){};
};
/*
 *********************************************
 使用数组实现的循环队列
 *********************************************
 */
template <class elemType>
class seqQueue:public queue<elemType> {
private:
    elemType *elem;
    int maxSize;
    int front,rear;
    void doubleSpace();
    
public:
    seqQueue(int size = 10);
    ~seqQueue();
    bool isEmpty()const;
    void enQueue(const elemType &x);
    elemType deQueue();
    elemType getHead() const;
};

template <class elemType>
seqQueue<elemType>::seqQueue(int size) {
    elem = new elemType[size];
    maxSize = size;
    front = rear = 0;
}

template <class elemType>
seqQueue<elemType>::~seqQueue() {
    delete [] elem;
}

template <class elemType>
bool seqQueue<elemType>::isEmpty()const {
    return front == rear;
    
}

template <class elemType>
elemType seqQueue<elemType>::deQueue() {
    front = (front + 1)%maxSize;
    return elem[front];
}

template <class elemType>
elemType seqQueue<elemType>::getHead()const {
    return elem[(front+1)%maxSize];
}

template <class elemType>
void seqQueue<elemType>::enQueue(const elemType &x) {
    if((rear+1)%maxSize == front)
        doubleSpace();
    rear = (rear + 1)%maxSize;
    elem[rear] = x;
}

template <class elemType>
void seqQueue<elemType>::doubleSpace() {
    elemType * tmp = elem;
    elem = new elemType[2 * maxSize];
    for(int i = 1;i<=maxSize;i++){
        elem[i] = tmp[(front+i)%maxSize];
    }
    front =0;
    rear = maxSize;
    maxSize *= 2;
    delete tmp;
}
/*
 *********************************************
 使用数组实现的循环队列
 使用保存队列长度的方法，算法比较简单
 *********************************************
 */
template <class elemType>
class AdSeqQueue:public queue<elemType>{
private:
    elemType *elem;
    int maxSize;
    int front;
    int length;
    
    void doubleSpace();
    
public:
    AdSeqQueue(int size = 10){
        elem = new elemType[size];
        maxSize = size;
        front = -1;
        length = 0;
        
    }
    ~AdSeqQueue(){
        delete [] elem;
    }
    bool isEmpty()const{
        return length == 0;
    }
    void enQueue(const elemType &x);
    elemType deQueue();
    
    elemType getHead()const{
        return elem[(front+1)%maxSize];
    }
    
};

template <class elemType>
void AdSeqQueue<elemType>::doubleSpace() {
    elemType * tmp = elem;
    
    elem = new elemType[2*maxSize];
    for(int i = 0;i < maxSize;i++){
        elem[i] = tmp[(front + i)%maxSize];
    }
    front = -1;
    maxSize *= 2;
    
    delete tmp;
}

template <class elemType>
void AdSeqQueue<elemType>::enQueue(const elemType &x) {
    if(length == maxSize)
        doubleSpace();
    int rear = (front+ length+1)%maxSize;
    elem[rear] = x;
    length++;
}

template <class elemType>
elemType AdSeqQueue<elemType>::deQueue(){
    front = (front + 1)%maxSize;
    length --;
    return elem[front];
}
/*
 *********************************************
 使用单链表实现的队列
 *********************************************
 */
template <class elemType>
class linkQueue:public queue<elemType> {
private:
    struct node{
        elemType data;
        node *next;
        
        node(const elemType &x,node *N = NULL){
            data = x;
            next = N;
        }
        node():next(NULL){}
        ~node(){}
    };
    node *front,*rear;
    
public:
    linkQueue();
    ~linkQueue();
    bool isEmpty() const;
    void enQueue(const elemType &x);
    elemType deQueue();
    elemType getHead()const;
    elemType getTail()const;
    
    void traverse()const{
        node * tmp = front;
        while(tmp != rear){
            std::cout << tmp->data << " ";
            tmp = tmp->next;
        }
        std::cout << tmp->data << " ";
        std::cout << "\n";
        
    }
    int length()const{
        int counter = 0;
        node* tmp = front;
        while(tmp != rear){
            counter ++;
            tmp = tmp->next;
        }
        return counter+1;
    }
};

template <class elemType>
linkQueue<elemType>::linkQueue() {
    front = rear = NULL;
}

template <class elemType>
linkQueue<elemType>::~linkQueue() {
    node *tmp;
    while(front != NULL){
        tmp = front ;
        front = front->next;
        delete tmp;
    }
}

template <class elemType>
bool linkQueue<elemType>::isEmpty()const {
    return front == NULL;
}

template <class elemType>
elemType linkQueue<elemType>::getHead()const  {
    return front->data;
}

template <class elemType>
elemType linkQueue<elemType>::getTail()const {
    return rear->data;
}

template <class elemType>
void linkQueue<elemType>::enQueue(const elemType &x) {
    if(rear == NULL){
        front = rear = new node(x);
    }
    else{
        rear = rear -> next = new node(x);
    }
}

template <class elemType>
elemType linkQueue<elemType>::deQueue() {
    node *tmp = front;
    elemType value = front -> data;
    front = front -> next;
    if(front == NULL){
        rear = NULL;
    }
    delete tmp;
    return value;
}

/*
 *********************************************
 使用单循环链表实现的队列
 *********************************************
 */
template <class elemType>
class cyLinkQueue:public queue<elemType>  {
private:
    struct node{
        elemType data;
        node *next;
        
        node(const elemType &x,node *N = NULL){
            data = x;
            next = N;
        }
        node():next(NULL){}
        ~node(){}
    };
    
    node *rear;
    
public:
    cyLinkQueue(){
        rear = NULL;
    }
    ~cyLinkQueue();
    bool isEmpty()const{
        return rear == NULL;
    }
    void enQueue(const elemType &x);
    elemType deQueue();
    elemType getHead()const{
        return  rear -> data;
    }
};

template <class elemType>
cyLinkQueue<elemType>::~cyLinkQueue() {
    node *tmp ,*delp;
    
    if(rear == NULL)
        return;
    delp = rear -> next;
    rear -> next = NULL;
    while(delp != NULL){
        tmp = delp;
        delp = delp->next;
        delete tmp;
    }
}
template <class elemType>
void cyLinkQueue<elemType>::enQueue(const elemType &x) {
    if(rear == NULL){
        rear = new node(x);
        rear -> next = rear;
    }
    else{
        
        node *tmp = new node(x,rear->next);
        rear->next = tmp;
        rear = tmp;
    }
}

template <class elemType>
elemType cyLinkQueue<elemType>::deQueue() {
    node *tmp = rear->next;
    elemType value = tmp->data;
    if(rear == tmp)
        rear = NULL;
    else
        rear->next = tmp -> next;
    delete tmp;
    return value;
}


/*
 *********************************************
 使用队列解决火车车厢重排问题
 in表示车厢序列 n表示车厢数 k表示缓冲轨道数
 *********************************************
 */

bool putBuffer(linkQueue<int> *buffer,int size,int in);
void checkBuffer(linkQueue<int> *buffer,int size,int &last);

void arrange(int in[],int n,int k){
    linkQueue<int> *buffer = new linkQueue<int>[k];//k条缓冲轨道
    int last = 0;//出轨上的火车车厢的编号
    
    for(int i = 0;i < n;i++){//处理入轨上的车厢
        if(!putBuffer(buffer,k,in[i]))//
            return ;//没有可用的轨道
        checkBuffer(buffer,k,last);//出轨
    }

}

bool putBuffer(linkQueue<int> *buffer,int size,int in){
    int avail = -1,max = 0;//avail为合适的缓冲轨道号，max为该轨道上的最后一个元素
    
    for(int j = 0;j < size;j++){//检查队列
        if(buffer[j].isEmpty()){//如果有空队列
            if(avail == -1)//如果尚未找到合适的轨道
                avail = j;
        }
        else if (buffer[j].getTail() < in && buffer[j].getTail() > max){
            //轨道上最后一个编号小于进入的编号 同时选择队尾元素最大的轨道
            avail = j;
            max = buffer[j].getTail();
        }
    }
    
    if(avail != -1){//找到了合适的轨道
        buffer[avail].enQueue(in);//入队
        std::cout << in << " enter the buffer #" << avail << std::endl;
        return true;
    }
    else{
        std::cout << " no available plan" << std::endl;
        return false;
    }
}

void checkBuffer(linkQueue<int> *buffer,int size,int &last){
    int j;
    int flag = true;//有元素出队
    
    while(flag){
        flag = false;
        for(j = 0;j < size;j++){//检查队列
            if(!buffer[j].isEmpty() && buffer[j].getHead() == last + 1){
                //如果队列有空位 队首元素符合出队的顺序
                std::cout << "move " <<  buffer[j].deQueue() << " from #" << j << " to  output" << std::endl;
                last ++;
                flag = true;//有元素出队
                break;//再次扫描轨道
            }
        }
    }
}

/*
 *********************************************
 使用队列实现单服务台排队系统模拟问题
 *********************************************
 */

class simulator{
    int arrivalLow;
    int arrivalHigh;
    int serviceTimeLow;
    int serviceTimeHigh;
    int customNum;
    
public:
    simulator();
    int avgWaitTime() const;
};

simulator::simulator(){
    std::cout << "please enter arrivalLow and arrivalHigh:";
    std::cin >> arrivalLow >> arrivalHigh;
    
    std::cout << "please enter serivceTimeLow and serviceTimeHigh:";
    std::cin >> serviceTimeLow >> serviceTimeHigh;
    
    std::cout << "please enter the number of custom:";
    std::cin >> customNum;
    
    srand((unsigned int)time(NULL));

}

int simulator::avgWaitTime()const{
    int currentTime = 0;
    int totalWaitTime = 0;
    int eventTime;
    linkQueue<int> customerQueue;
    int i;
    
    //生成事件
    for(i = 0;i < customNum;i++){
        currentTime += arrivalLow + (arrivalHigh - arrivalLow + 1)*rand()/((long)RAND_MAX + 1);
        customerQueue.enQueue(currentTime);
    }
    //处理事件
    currentTime = 0;
    while(!customerQueue.isEmpty()){
        eventTime = customerQueue.deQueue();
        if(eventTime < currentTime)
            totalWaitTime += currentTime - eventTime;
        else
            currentTime = eventTime;
        currentTime += serviceTimeLow + (serviceTimeHigh - serviceTimeLow + 1) * rand()/((long)RAND_MAX + 1);
    }
    
    return totalWaitTime/customNum;
}


/*
 *********************************************
 使用队列实现过江渡口模拟问题
 要求：渡船能载十个车，十分钟一班，客车优先于货车
      要求最多四辆客车要上一辆货车，得到平均等待时间
 前提：在linkQueue中加入了length()函数计算队列长度
 *********************************************
 */


class ferrySim{
    int arrivalLow;
    int arrivalHigh;
    int vehicalNum;
    
public:
    ferrySim();
    void avgWaitTime() const;
    
};

ferrySim::ferrySim(){
    std::cout << "please enter arrivalLow and arrivalHigh:";
    std::cin >> arrivalLow >> arrivalHigh;
    
    std::cout << "please enter the vehicalNum:";
    std::cin >> vehicalNum;
    
    srand((unsigned int)time(NULL));
}

void ferrySim::avgWaitTime()const{
    int currentTime = 0;
    int carWaitTime = 0;
    int truckWaitTIme = 0;
    int carNum;
    int truckNum;
    linkQueue<int> carQueue;
    linkQueue<int> truckQueue;
    int i,j;
    
    
    //生成车辆队列
    
    for(i = 0 ;i < vehicalNum;i++){
        currentTime += arrivalLow + rand() % (arrivalHigh - arrivalLow );
        if(rand() % 5 <= 2){
            carQueue.enQueue(currentTime);
        }
        else
            truckQueue.enQueue(currentTime);
    }
    
//    std::cout << "car:" << std::endl;
//    carQueue.traverse();
//    std::cout << "truck:" << std::endl;
//    truckQueue.traverse();

    carNum = carQueue.length();
    truckNum = truckQueue.length();
    //处理事件
    currentTime = 10;
    i = 0;
    while(!carQueue.isEmpty() && !truckQueue.isEmpty()){
        //上船
        while(!carQueue.isEmpty() && carQueue.getHead() < currentTime){//先检查有没有客车
            if(i < 4){//最多连续四辆客车
                carWaitTime += currentTime - carQueue.deQueue();
                i ++;
            }
            else
                break;
        }
        if(i != 4){//客车不满四辆
            while(!truckQueue.isEmpty() && truckQueue.getHead() < currentTime){//检查有没有货车
                if(i < 10){//最多一次十辆车
                    truckWaitTIme += currentTime - truckQueue.deQueue();
                    i ++;
                }
                else
                    break;
            }
            
        }
        else{//上了四辆客车
            if(!truckQueue.isEmpty() && truckQueue.getHead() < currentTime){
                
                truckWaitTIme += currentTime -truckQueue.deQueue();//上一辆货车
                i++;
            }
            j = 0;
            while(!carQueue.isEmpty() && carQueue.getHead() < currentTime){//再次检查客车
                if(j < 4){//最多再上四辆客车
                    carWaitTime += currentTime - carQueue.deQueue();
                    i++;j++;
                }
                else
                    break;
                
            }
            j = 0;
            while(!truckQueue.isEmpty() && truckQueue.getHead() < currentTime){//再次上货车
                truckWaitTIme += currentTime - truckQueue.deQueue();
                i ++;
          
            }
            
        }
        //一波船上满了
        currentTime += 10;
        i = 0;
    }
    carWaitTime = carWaitTime / carNum;
    truckWaitTIme = truckWaitTIme / truckNum;
    std::cout << "客车的平均等待时间为： " << carWaitTime << std::endl;
    std::cout << "货车的平均等待时间为： " << truckWaitTIme << std::endl;
}


/*
 *********************************************
 使用二叉堆实现的优先级队列
 二叉堆使用了二叉树的顺序实现（数组）
 *********************************************
 */
template <class elemType>
class priorityQueue:public queue<elemType> {
public:
    priorityQueue(int capacity = 100);
    priorityQueue(const elemType data [],int size);
    ~priorityQueue();
    bool isEmpty()const;
    void enQueue(const elemType &x);
    elemType deQueue();
    elemType getHead() const;
    int findMin(elemType x)const;
    void decreaseKey(int i,elemType v);
    //void printElem();//调试辅助函数

private:
    int currentSize;
    elemType *array;
    int maxSize;
    
    void doubleSpace();
    void buildHeap();
    void percolateDown(int hole);
};

template <class elemType>
priorityQueue<elemType>::priorityQueue(int capacity) {
    array = new elemType[capacity];
    maxSize = capacity;
    currentSize = 0;
}

template <class elemType>
priorityQueue<elemType>::~priorityQueue() {
    delete [] array;
}

template <class elemType>
bool priorityQueue<elemType>::isEmpty()const {
    return currentSize == 0;
}

template <class elemType>
elemType priorityQueue<elemType>::getHead()const {
    return array[1];//下标为0的元素作为空元素
}

template <class elemType>
void priorityQueue<elemType>::enQueue(const elemType &x) {
    if(currentSize == maxSize - 1)
        doubleSpace();
    
    int hole = ++currentSize;
    for(;hole>1 && (x < array[hole/2]);hole/=2){
        array[hole] = array[hole/2];
    }
    array[hole] = x;
}

template <class elemType>
elemType priorityQueue<elemType>::deQueue() {
    elemType minItem;
    minItem = array[1];
    array[1] = array[currentSize --];
    percolateDown(1);
    return minItem;
}

template <class elemType>
void priorityQueue<elemType>::percolateDown(int hole) {
    int child;
    elemType tmp = array[hole];
    
    for(;hole * 2 <= currentSize;hole = child){
        child = hole * 2;
        if(child != currentSize && array[child + 1] < array[child])
            child ++;
        if(array[child] < tmp)
            array[hole] = array[child];
        else
            break;
    }
    array[hole] = tmp;
}

template <class elemType>
void priorityQueue<elemType>::buildHeap() {
    for(int i = currentSize/2;i > 0 ;i++){
        percolateDown(i);
    }
}

template <class elemType>
priorityQueue<elemType>::priorityQueue(const elemType *items,int size):maxSize(size+10),currentSize(size) {
    array = new elemType [maxSize];
    for(int i = 0;i < size;i++){
        array[i+1] = items[i];
    }
    buildHeap();
}

template <class elemType>
void priorityQueue<elemType>::doubleSpace() {
    elemType *tmp = array;
    maxSize *= 2;
    array = new elemType[maxSize];
    for(int i = 0;i < currentSize;i++){
        array[i] = tmp[i];
    }
    delete [] tmp;
}

template <class elemType>
int priorityQueue<elemType>::findMin(elemType x)const  {
    int min = INT_MAX;
    int minPos = 0;
    for(int i = 0;i < currentSize;i++){
        if(array[i] >= x && array[i] <= min){
            min = array[i];
            minPos = i;
        }
    }
    return minPos;
}

template <class elemType>
void priorityQueue<elemType>::decreaseKey(int i, elemType v) {
    array[i] -= v;
    int hole = i;
    int x = array[i];
    
    for(;hole>1 && (x < array[hole/2]);hole/=2){
        array[hole] = array[hole/2];
    }
    array[hole] = x;
}
/*
 *********************************************
 使用优先级队列实现多服务台排队系统模拟问题
 *********************************************
 */
class mSimulator{
    int noOfServer;
    int arrivalLow;
    int arrivalHigh;
    int serviceTimeLow;
    int serviceTimeHigh;
    int customNum;
    
    struct eventT{
        int time;
        int type;
        bool operator<(const eventT &e)const{
            return time < e.time;
        }
    };
public:
    mSimulator();
    int avgWaitTime();
};

mSimulator::mSimulator(){
    std::cout << "Please enter the num of servers:" ;
    std::cin >> noOfServer;
    std::cout << "Please enter the range of arrival time span(low-high):";
    std::cin >> arrivalLow >> arrivalHigh;
    std::cout << "Please enter the range of service time (low-high):";
    std::cin >> serviceTimeLow >> serviceTimeHigh;
    std::cout << "please enter the num of custom:";
    std::cin >> customNum;
    
    srand((unsigned int)time(NULL));
}

int mSimulator::avgWaitTime(){
    int serverBusy = 0;
    int currentTime;
    int totalWaitTime = 0;
    linkQueue<eventT> waitQueue;
    priorityQueue<eventT> eventQueue;
    
    eventT currentEvent;
    
    int i;
    currentEvent.time = 0;
    currentEvent.type = 0;
    for(i = 0;i < customNum;i++){
        currentEvent.time += arrivalLow + (arrivalHigh - arrivalLow + 1)*rand()/((long)RAND_MAX + 1);
        eventQueue.enQueue(currentEvent);
    }
    
    while(!eventQueue.isEmpty()){
        currentEvent = eventQueue.deQueue();
        currentTime = currentEvent.time;
        switch (currentEvent.type) {
            case 0:
                if(serverBusy != noOfServer){
                    serverBusy++;
                    currentEvent.time += serviceTimeLow + (serviceTimeHigh - serviceTimeLow + 1)*rand()/((long)RAND_MAX + 1);
                    currentEvent.type = 1;
                    eventQueue.enQueue(currentEvent);
                }else{
                    waitQueue.enQueue(currentEvent);
                }
                break;
            case 1:
                if(!waitQueue.isEmpty()){
                    currentEvent = waitQueue.deQueue();
                    totalWaitTime += currentTime - currentEvent.time;
                    currentEvent.time = currentTime + serviceTimeLow + (serviceTimeHigh - serviceTimeLow + 1)*rand()/((long)RAND_MAX + 1);
                    currentEvent.type = 1;
                    eventQueue.enQueue(currentEvent);
                }else{
                    serverBusy --;
                }
        }
    }
    return totalWaitTime/customNum;
}
#endif /* queue_hpp */
