//
//  graph.hpp
//  DataStructure
//
//  Created by 杨磊 on 2019/5/31.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp
#include "set.hpp"
#include "queue.hpp"
/*
 *********************************************
 定义抽象接口类 用于规定所有的加权有向图的功能
 *********************************************
 */
template <class TypeOfVer, class TypeOfEdge> //节点vertax的类型，边Edge的权值类型
class graph {
public:
    virtual void insert(TypeOfVer x, TypeOfVer y, TypeOfEdge w) = 0;
    virtual void remove(TypeOfVer x, TypeOfVer y) = 0;
    virtual bool exist(TypeOfVer x, TypeOfVer y)const = 0;
    int numOfVer() const { return Vers;}
    int numOfEdge() const { return Edges;}
    
protected:
    int Vers,Edges;
};

/*
 *********************************************
 使用邻接矩阵实现的图
 适用于边数比较多的图，操作比较简单
 *********************************************
 */
template <class TypeOfVer, class TypeOfEdge>
class adjMatrixGraph:public graph<TypeOfVer,TypeOfEdge>{
public:
    adjMatrixGraph(int vSize, const TypeOfVer d[],const TypeOfEdge noEdgeFlag);//创建没有边的空图
    void insert(TypeOfVer x, TypeOfVer y, TypeOfEdge w);
    void remove(TypeOfVer x, TypeOfVer y);
    bool exist(TypeOfVer x, TypeOfVer y) const ;
    ~adjMatrixGraph();
    
private:
    TypeOfEdge **edge; // 邻接矩阵
    TypeOfVer *ver;
    TypeOfEdge noEdge;//没有边的标志
    
    int find(TypeOfVer v)const{//根据值找节点的编号
        for(int i = 0;i < this->Vers;i++){
            if(ver[i] == v)
                return i;
        }
        return NULL;//未找到
    }
};

template <class TypeOfVer, class TypeOfEdge>
adjMatrixGraph<TypeOfVer, TypeOfEdge>::adjMatrixGraph(int vSize, const TypeOfVer d[], const TypeOfEdge noEdgeFlag) {
    int i,j;
    
    this->Vers = vSize;
    this->Edges = 0;
    noEdge = noEdgeFlag;
    
    //节点数组初始化
    ver = new TypeOfVer[vSize];
    for(i = 0; i < vSize;i++){
        ver[i] = d[i];
    }
    
    //邻接矩阵初始化
    edge = new TypeOfEdge*[vSize];
    for(i = 0;i < vSize;i++){
        edge[i] = new TypeOfEdge[vSize];
        for(j = 0; j < vSize;j++){
            edge[i][j] = noEdge;
        }
        edge[i][j] = 0;
    }
}

template <class TypeOfVer, class TypeOfEdge>
adjMatrixGraph<TypeOfVer, TypeOfEdge>::~adjMatrixGraph() {
    delete [] ver;
    for(int i = 0; i < this->Vers;i++){
        delete [] edge[i];
    }
    delete [] edge;
}

template <class TypeOfVer, class TypeOfEdge>
void adjMatrixGraph<TypeOfVer, TypeOfEdge>::insert(TypeOfVer x, TypeOfVer y, TypeOfEdge w) {
    int u = find(x),v = find(y);
    edge[u][v] = w;
    this->Edges ++;
}

template <class TypeOfVer, class TypeOfEdge>
void adjMatrixGraph<TypeOfVer, TypeOfEdge>::remove(TypeOfVer x, TypeOfVer y) {
    int u = find(x),v = find(y);
    edge[u][v] = noEdge;
    this->Edges --;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjMatrixGraph<TypeOfVer, TypeOfEdge>::exist(TypeOfVer x, TypeOfVer y)const {
    int u = find(x), v = find(y);
    if(edge[u][v] == noEdge)
        return false;
    else
        return true;
}


/*
 *********************************************
 使用邻接表实现的图
 适用于稀疏图，节省空间
 *********************************************
 */
template <class TypeOfVer, class TypeOfEdge>
class adjListGraph:public graph<TypeOfVer, TypeOfEdge> {
public:
    adjListGraph(int vSize,const TypeOfVer d[]);
    //插入和删除有向边
    void insert(TypeOfVer x, TypeOfVer y, TypeOfEdge w);
    void remove(TypeOfVer x, TypeOfVer y);
    void remove(int x, int y);
    //插入和删除一正一反两条有向边，表示无向图
    void NDinsert(TypeOfVer x, TypeOfVer y, TypeOfEdge w);
    void NDremove(TypeOfVer x, TypeOfVer y);
    void NDremove(int x, int y);
    bool exist(TypeOfVer x, TypeOfVer y)const;
    ~adjListGraph();
    
    void dfs() const;//深度优先搜索
    void bfs()const;//广度优先搜索
    void EulerCircuit(TypeOfVer start);//欧拉回路
    void topSort()const;//拓扑序列
    void criticalPath() const;//关键路径
    
    void kruskal() const;//最小生成树kruskal算法
    void prim(TypeOfEdge noEdge) const;//最小生成树prim算法
    
    void unweightedShortDistance(TypeOfVer start,TypeOfEdge noEdge) const;//无权图单源最短路径
    void dijkstra(TypeOfVer start, TypeOfEdge noEdge) const;//dijkstra算法 加权图最短路径
    
private:
    struct edgeNode{//储存边的结点
        int end;//指向的目标
        TypeOfEdge weight;
        edgeNode* next;
        
        edgeNode(int e, TypeOfEdge w,edgeNode *n = NULL){
            end = e;
            weight = w;
            next = n;
        }
    };
    
    struct verNode{//储存节点的结点
        TypeOfVer ver;
        edgeNode *head;//储存所有边
        
        verNode(edgeNode *h = NULL){
            head = h;
        }
    };
    
    struct EulerNode{//储存欧拉回路的结点
        int NodeNum;
        EulerNode *next;
        
        EulerNode(int ver){
            NodeNum = ver;
            next = NULL;
        }
    };
    
    verNode *verList;//节点列表
    
    //辅助函数
    int find(TypeOfVer v)const{
        for(int i = 0;i < this->Vers;i++){
            if(verList[i].ver == v)
                return i;
        }
        return NULL;//未找到
    }
    //深度优先搜索辅助函数
    void dfs(int start, bool visited[]) const;//得到一颗深度优先树
    //欧拉回路辅助函数
    adjListGraph<TypeOfVer,TypeOfEdge>::verNode* clone()const;
    void EulerCircuit(int start, EulerNode *&beg,EulerNode *&en);
    
    //kruskal算法辅助结构
    struct edge{
        int beg,end;
        TypeOfEdge w;
        bool operator<(const edge &rp)const{
            return w < rp.w;
        }
    };
    //单源最短路径显示辅助函数
    void printPath(int start, int end, int prev[]) const{
        if(start == end){
            std::cout << verList[start].ver;
            return ;
        }
        printPath(start, prev[end], prev);
        std::cout << "-" << verList[end].ver;
    }
};

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::adjListGraph(int vSize, const TypeOfVer d[]) {
    this->Vers = vSize;
    this->Edges = 0;
    
    verList = new verNode[vSize];
    for(int i = 0;i < vSize;i++){
        verList[i].ver = d[i];
    }
}

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::~adjListGraph() {
    int i;
    edgeNode *p;
    
    for(i = 0;i < this->Vers;i++){
        while((p = verList[i].head) != NULL){
            verList[i].head = p->next;
            delete p;
        }
    }
    delete [] verList;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::insert(TypeOfVer x, TypeOfVer y, TypeOfEdge w) {
    int u = find(x),v = find(y);
    verList[u].head = new edgeNode(v,w,verList[u].head);
    this->Edges ++;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::NDinsert(TypeOfVer x, TypeOfVer y, TypeOfEdge w) {
    insert(x, y, w);
    insert(y, x, w);
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::remove(TypeOfVer x, TypeOfVer y) {
    int u = find(x), v = find(y);
    edgeNode *p = verList[u].head, *q;
    
    if(p == NULL)
        return ;
    
    if(p->end == v){
        verList[u].head = p->next;
        delete p;
        this->Edges --;
        return ;
    }
    while(p->next != NULL && p->next->end != v)
        p = p->next;
    if(p->next != NULL){
        q = p->next;
        p->next = q->next;
        delete q;
        this->Edges --;
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::NDremove(TypeOfVer x, TypeOfVer y) {
    remove(x, y);
    remove(y, x);
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::remove(int x, int y) {
    int u = x, v = y;
    edgeNode *p = verList[u].head, *q;
    
    if(p == NULL)
        return ;
    
    if(p->end == v){
        verList[u].head = p->next;
        delete p;
        this->Edges --;
        return ;
    }
    while(p->next != NULL && p->next->end != v)
        p = p->next;
    if(p->next != NULL){
        q = p->next;
        p->next = q->next;
        delete q;
        this->Edges --;
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::NDremove(int x, int y) {
    remove(x, y);
    remove(y, x);
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::exist(TypeOfVer x, TypeOfVer y)const {
    int u = find(x), v = find(y);
    edgeNode *p = verList[u].head;

    while(p != NULL && p->end != v)
        p = p->next;
    if(p == NULL)
        return false;
    else
        return true;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dfs()const {
    bool *visited = new bool[this->Vers];
    
    for(int i = 0;i < this->Vers;i++){
        visited[i] = false;
    }
    
    std::cout << "Depth First Search:" << std::endl;
    
    for(int i = 0 ; i < this->Vers;i++){
        if(visited[i] == true)//检查未访问节点
            continue;
        dfs(i, visited);//得到一颗深度优先树
        std::cout << std::endl;
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dfs(int start, bool visited[])const {
    edgeNode *p = verList[start].head;
    
    std::cout << verList[start].ver <<"\t";
    visited[start] = true;
    
    while(p != NULL ){
        if(visited[p->end] == false)
            dfs(p->end, visited);//递归调用
        p = p->next;
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer,TypeOfEdge>::bfs()const {
    bool *visited = new bool[this->Vers];
    int currentNode;
    linkQueue<int> q;
    edgeNode *p;
    
    for(int i = 0;i < this->Vers;i++){
        visited[i] = false;
    }
    
    std::cout << "Breadth First Search:" << std::endl;
    
    for(int i = 0; i < this->Vers;i++){
        if(visited[i] == true)
            continue;
        q.enQueue(i);
        while(!q.isEmpty()){
            currentNode = q.deQueue();
            if(visited[currentNode] == true)
                continue;
            std::cout << verList[currentNode].ver << "\t";
            visited[currentNode] = true;
            p = verList[currentNode].head;
            while( p != NULL){
                if(visited[p->end] == false)
                    q.enQueue(p->end);
                p = p->next;
            }
        }
        std::cout << std::endl;
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::EulerCircuit(TypeOfVer start) {
    EulerNode *beg,*en,*p,*q,*tb,*te;//beg和en为起点和终点
    int numOfDegree,i;
    edgeNode *r;
    verNode *tmp;
    
    if(this->Edges == 0){
        std::cout << "No Euler Circuit!\n";
        return ;
    }
    for(i = 0; i < this->Vers;i++){
        numOfDegree = 0;
        r = verList[i].head;
        while(r != 0){
            numOfDegree ++;
            r = r->next;
        }
        if(numOfDegree % 2){
            std::cout << "No Euler Circuit!\n";
            return ;
        }
    }
    
    i = find(start);
    tmp = clone();
    
    EulerCircuit(i, beg, en);
    while(true){
        p = beg;
        while(p->next != NULL){
            if(verList[p->next->NodeNum].head != NULL)
                break;
            else
                p = p->next;
        }
        if(p->next == NULL)
            break;
        q = p->next;
        EulerCircuit(q->NodeNum,tb,te);
        te->next = q->next;
        p->next = tb;
        delete q;
    }
    
    delete [] verList;
    verList = tmp;
    
    std::cout << "Euler Circuit is as follow:\n";
    while(beg != NULL){
        std::cout << verList[beg->NodeNum].ver << "\t";
        p = beg;
        beg = beg->next;
        delete p;
    }
    std::cout << "\n";
}

template <class TypeOfVer, class TypeOfEdge>
typename adjListGraph<TypeOfVer, TypeOfEdge>::verNode* adjListGraph<TypeOfVer,TypeOfEdge>::clone()const {
    verNode *tmp = new verNode[this->Vers];
    edgeNode *p;
    
    for(int i = 0 ; i < this->Vers;i++){
        tmp[i].ver = verList[i].ver;
        p = verList[i].head;
        while( p != NULL){
            tmp[i].head = new edgeNode(p->end,p->weight,tmp[i].head);
            p = p->next;
        }
    }
    return tmp;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer,TypeOfEdge>::EulerCircuit(int start, EulerNode *&beg, EulerNode *&en) {
    int nextNode;
    
    beg = en = new EulerNode(start);
    while(verList[start].head != NULL){
        nextNode= verList[start].head->end;
        remove(start,nextNode);
        remove(nextNode,start);
        start = nextNode;
        en->next = new EulerNode(start);
        en = en->next;
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer,TypeOfEdge>::topSort()const {
    linkQueue<int> q;
    edgeNode *p;
    int current;
    int *inDegree = new int[this->Vers];
    
    for(int i = 0; i < this->Vers;i++){
        inDegree[i] = 0;
        for( p = verList[i].head;p != NULL;p = p->next){
            inDegree[p->end]++;
        }
    }
    
    for(int i = 0; i< this->Vers;i++){
        if(inDegree[i] == 0)
            q.enQueue(i);
    }
    
    std::cout << "Topology Sort is:\n";
    while(!q.isEmpty()){
        current = q.deQueue();
        std::cout << verList[current].ver << "\t";
        for( p = verList[current].head;p != NULL;p = p->next)
            if(--inDegree[p->end] == 0)
                q.enQueue(p->end);
    }
    std::cout << std::endl;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer,TypeOfEdge>::criticalPath()const {//关键路径问题
    TypeOfEdge *ee = new TypeOfEdge[this->Vers];//ee储存最早时间
    TypeOfEdge *le = new TypeOfEdge[this->Vers];//le储存最晚时间
    int *top = new int[this->Vers];//储存拓扑序列
    int *inDegree = new int[this->Vers];//储存入度
    linkQueue<int> q;
    int i;
    edgeNode *p;
    
    //计算拓扑序列
    for(i = 0;i < this->Vers;i++){//赋初值
        top[i] = 0;
        inDegree[i] = 0;
    }
    for(i = 0;i < this->Vers;i++){
        for(p = verList[i].head; p != NULL; p = p->next)
            ++inDegree[p->end];//计算入度
    }
    for(i = 0;i < this->Vers;i++){
        if(inDegree[i] == 0)
            q.enQueue(i);
    }
    
    i = 0;
    while(!q.isEmpty()){
        top[i] = q.deQueue();//输出入度为0的
        for(p = verList[top[i]].head; p != NULL; p = p->next){
            if((--inDegree[p->end]) == 0)//删去对应的边（减小入度）
                q.enQueue(p->end);
        }
        i++;
    }
    
    //计算最早时间
    for(i = 0; i < this->Vers;i++){
        ee[i] = 0;//赋初值
    }
    for(i = 0; i < this->Vers;i++){
        for(p = verList[top[i]].head; p != NULL ; p = p->next)
            if(ee[p->end] < ee[top[i]] + p->weight)//更新目标节点的最早时间
                ee[p->end] = ee[top[i]] + p->weight;
    }
    
    //计算最晚时间
    for(i = 0; i < this->Vers;i++){
        le[i] = ee[this->Vers - 1];//赋初值为最大时间
    }
    for(i = this->Vers - 1; i >= 0;i--){//按拓扑序列逆序更新时间
        for(p = verList[top[i]].head; p!= NULL; p = p->next)
            if(le[p->end] - p->weight < le[top[i]])
                le[top[i]] = le[p->end] - p->weight;
    }
    
    //输出关键路径
    for(i = 0; i < this->Vers;i++){
        if(le[top[i]] == ee[top[i]])
            std::cout << "(" << verList[top[i]].ver << "," << ee[top[i]] << ")";
    }
    
    std::cout << std::endl;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::kruskal()const {
    int edgesAccepted = 0, u, v;
    edgeNode *p;
    edge e;
    DisjointSet ds(this->Vers);
    priorityQueue<edge> pq;
    
    
    for(int i = 0; i< this->Vers;i++){
        for(p = verList[i].head; p!= NULL; p = p->next)
            if(i < p->end){
                e.beg = i;
                e.end = p->end;
                e.w = p->weight;
                pq.enQueue(e);
            }
    }
    
    while(edgesAccepted < this->Vers - 1){
        e = pq.deQueue();
        u = ds.Find(e.beg);
        v = ds.Find(e.end);
        if( u != v){
            edgesAccepted ++;
            ds.Union(u, v);
            std::cout << "(" << verList[e.beg].ver << "," << verList[e.end].ver << ")\n";
        }
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::prim(TypeOfEdge noEdge)const {
    bool *flag = new bool[this->Vers];
    TypeOfEdge *lowCost = new TypeOfEdge[this->Vers];
    int *startNode = new int[this->Vers];
    
    edgeNode *p;
    TypeOfEdge min;
    int start,i,j;
    
    for(i = 0;i < this->Vers;i++){
        flag[i] = false;
        lowCost[i] = noEdge;
    }
    
    start = 0;
    for(i = 1;i < this->Vers;i++){
        for(p = verList[start].head; p != NULL; p = p->next)
            if(!flag[p->end] && lowCost[p->end] > p->weight){
                lowCost[p->end] = p->weight;
                startNode[p->end] = start;
            }
        flag[start] = true;
        min = noEdge;
        for(j = 0; j < this->Vers;j++)
            if(lowCost[j] < min){
                min = lowCost[j];
                start = j;
            }
        std::cout << "(" << verList[startNode[start]].ver << "," << verList[start].ver << ")\n";
        lowCost[start] = noEdge;
    }
    
    delete [] flag;
    delete [] startNode;
    delete [] lowCost;
}


template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::unweightedShortDistance(TypeOfVer start, TypeOfEdge noEdge)const {
    linkQueue<int> q;
    TypeOfEdge *distance = new TypeOfEdge[this->Vers];
    int *prev = new int[this->Vers];
    int u,sNo;
    edgeNode *p;
    
    for(int i = 0; i < this->Vers;i++){
        distance[i] = noEdge;
    }
    
    sNo = find(start);
    
    distance[sNo] = 0;
    prev[sNo] = sNo;
    q.enQueue(sNo);
    
    while(!q.isEmpty()){
        u = q.deQueue();
        for(p = verList[u].head;p != NULL;p=p->next)
            if(distance[p->end] == noEdge){
                distance[p->end] = distance[u]+1;
                prev[p->end] = u;
                q.enQueue(p->end);
            }
    }
    
    for(int i = 0;i < this->Vers ;i++){
        std::cout << "path from " << start << " to " << verList[i].ver << " is :\t" << std::endl;
        printPath(sNo, i, prev);
        std::cout << "\n";
    }
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dijkstra(TypeOfVer start, TypeOfEdge noEdge)const {
    TypeOfEdge *distance = new TypeOfEdge[this->Vers];
    int *prev = new int[this->Vers];
    bool *known = new bool[this->Vers];
    
    int u = 0,sNo,i,j;
    edgeNode *p;
    TypeOfEdge min;
    
    for(i = 0; i< this->Vers;i++){
        known[i] = false;
        distance[i] = noEdge;
    }
    
    sNo = find(start);
    distance[sNo] = 0;
    prev[sNo] = sNo;
    
    for(i = 1;i < this->Vers;i++){
        min = noEdge;
        for(j = 0;j < this->Vers;j++){
            if(!known[j] && distance[j] < min){
                min = distance[j];
                u = j;
            }
        }
        
        known[u] = true;
        for(p = verList[u].head ; p!= NULL; p=p->next)
            if(!known[p->end] && distance[p->end] > min + p->weight){
                distance[p->end] = min + p->weight;
                prev[p->end] = u;
            }
    }
    
    for(i = 0 ; i< this->Vers;i++){
        std::cout << "path from " << start << " to " << verList[i].ver << " is:\n";
        printPath(sNo, i, prev);
        std::cout << "\t and the length is: " << distance[i] << std::endl;
    }
}
#endif /* graph_hpp */
