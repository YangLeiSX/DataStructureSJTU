//
//  stack.hpp
//  数据结构学习 栈部分
//
//  Created by 杨磊 on 2019/2/15.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef stack_hpp
#define stack_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>

/*
 *********************************************
 定义抽象接口类 用于规定所有的栈的功能
 *********************************************
 */
template <class elemType>
class stack {
public:
    virtual bool isEmpty() const = 0;
    virtual void push(const elemType &x) = 0;
    virtual elemType pop() = 0;
    virtual elemType top() const = 0;
    virtual ~stack(){}
};
/*
 *********************************************
 使用数组实现的栈
 *********************************************
 */
template <class elemType>
class seqStack:public stack<elemType> {
private:
    elemType *elem;
    int top_p;
    int maxSize;
    void doubleSpace();

public:
    seqStack(int initSize = 10);
    ~seqStack();
    bool isEmpty() const;
    void push(const elemType &x);
    elemType pop();
    elemType top() const;
};

template <class elemType>
seqStack<elemType>::seqStack(int initSize) {
    elem = new elemType[initSize];
    maxSize = initSize;
    top_p = -1;
}

template <class elemType>
seqStack<elemType>::~seqStack() {
    delete [] elem;
}

template <class elemType>
bool seqStack<elemType>::isEmpty()const {
    return top_p == -1;
}

template <class elemType>
void seqStack<elemType>::push(const elemType &x) {
    if(top_p == maxSize - 1)
        doubleSpace();
    elem[++top_p] = x;
}

template <class elemType>
elemType seqStack<elemType>::pop() {
    return elem[top_p--];
}

template <class elemType>
elemType seqStack<elemType>::top()const {
    return elem[top_p];
}

template <class elemType>
void seqStack<elemType>::doubleSpace() {
    elemType *tmp = elem;
    
    elem = new elemType[2* maxSize];
    for(int i = 0;i < maxSize;i++){
        elem[i] = tmp[i];
    }
    maxSize *= 2;
    delete [] tmp;
}
/*
 *********************************************
 使用链表实现的栈
 *********************************************
 */
template <class elemType>
class linkStack:public stack<elemType> {
private:
    struct node{
        elemType data;
        node * next;
        node(const elemType &x,node *N = NULL){
            data = x;
            next = N;
        }
        node():next(NULL){}
        ~node(){}
    };
    
    node *top_p;
public:
    linkStack();
    ~linkStack();
    bool isEmpty() const;
    void push(const elemType &x);
    elemType pop();
    elemType top()const;
};

template <class elemType>
linkStack<elemType>::linkStack() {
    top_p = NULL;
}

template <class elemType>
linkStack<elemType>::~linkStack() {
    node *tmp;
    while(top_p != NULL){
        tmp = top_p;
        top_p = top_p -> next;
        delete tmp;
    }
}

template <class elemType>
bool linkStack<elemType>::isEmpty() const{
    return top_p == NULL;
}

template <class elemType>
void linkStack<elemType>::push(const elemType &x) {
    top_p = new node(x,top_p);
}

template <class elemType>
elemType linkStack<elemType>::pop() {
    node *tmp = top_p;
    elemType x = tmp -> data;
    top_p = top_p -> next ;
    delete tmp;
    return x;
}

template <class elemType>
elemType linkStack<elemType>::top() const {
    return top_p -> data;
}


/*
 *********************************************
 使用栈实现的C++源码括号配对
 *********************************************
 */
class balance{
private:
    std::ifstream fin;
    int currentLine;
    int Errors;
    
    struct Symbol{
        char Token;
        int TheLine;
    };
    enum CommentType{SlashSlash,SlashStar};
    
    bool CheckMatch(char Symb1,char Symb2,int Line1,int Line2);
    char GetNextSymbol();
    void PutBackChar(char ch);
    void SkipComment(enum CommentType type);
    void SkipQuote(char type);
    char NextChar();
    
public:
    balance(const char *s);
    int CheckBalance();
    
};
class noFile{};

balance::balance(const char *s){
    fin.open(s);
    if(!fin) throw noFile();
    
    currentLine = 1;
    Errors = 0;
}

int balance::CheckBalance(){
    struct Symbol node;
    seqStack<Symbol> st;
    char LastChar,Match;
    
    while((LastChar = GetNextSymbol())){
        switch (LastChar) {
            case '(': case '[': case '{':
                node.Token = LastChar;
                node.TheLine = currentLine;
                st.push(node);
                break;
            case ')': case ']': case '}':
                if(st.isEmpty()){
                    Errors ++;
                    std::cout << "在第" << currentLine << "行有一个多余的" << LastChar << std::endl;
                }
                else{
                    node = st.pop();
                    Match = node.Token;
                    if(!CheckMatch(Match, LastChar, node.TheLine, currentLine))
                        Errors++;
                }
                break;
        }
    }
    while(!st.isEmpty()){
        Errors ++;
        node = st.pop();
        std::cout << "在第" << node.TheLine << "行的符号" << node.Token << "不匹配\n";
        
    }
    return Errors;
}

bool balance::CheckMatch(char Symb1, char Symb2, int Line1, int Line2){
    if((Symb1 == '(' && Symb2 != ')') || (Symb1 == '[' && Symb2 != ']') || (Symb1 == '{' && Symb2 != '}') ){
        std::cout << "发现第" << Line2 << "行的符号" << Symb2 << "与第" << Line1 << "行的符号" << Symb1 << "不匹配\n";
        return false;
    }
    else
        return true;
}

char balance::GetNextSymbol(){
    char ch;
    while((ch = NextChar())){
        if(ch == '/'){
            ch = NextChar();
            if(ch == '*')
                SkipComment(SlashStar);
            else if(ch == '/')
                SkipComment(SlashSlash);
            else
                PutBackChar(ch);
        }
        else if(ch == '\'' || ch == '"')
                SkipQuote(ch);
        else if(ch == '(' || ch == '[' || ch == '{' || ch == ')' || ch == ']' || ch == '}')
            return ch;
    }
    return NULL;
}

char balance::NextChar(){
    char ch;
    if((ch = fin.get()) == EOF)
        return NULL;
    if(ch == '\n')
        currentLine ++;
    return ch;
}

void balance::PutBackChar(char ch){
    fin.putback(ch);
    if(ch == '\n')
        currentLine--;
}

void balance::SkipQuote(char type){
    char ch;
    while((ch = NextChar())){
        if(ch == type)
            return ;
        else if(ch == '\n'){
            Errors ++;
            std::cout << currentLine << "行内引号缺失" << std::endl;
            return ;
        }
        else if(ch == '\\')
            ch = NextChar();
    }
}

void balance::SkipComment(enum CommentType type){
    char ch,flag;
    
    if(type == SlashSlash){
        while((ch = NextChar())&&(ch != '\n')) continue;
            return ;
    }
    
    flag =' ';
    while((ch = NextChar()) != char(NULL)){
        if(flag == '*' && ch == '/')
            return ;
        flag = ch;
        
    }
    Errors ++;
    std::cout << "注释没有结束" << std::endl;
    
}


/*
 *********************************************
 使用栈实现的简单计算器
 (抄的课本代码跑不通 getOp函数的返回值有问题）
 *********************************************
 */
class calc{
    char * expression;
    
    enum token{OPAREN,ADD,SUB,MULTI,DIV,EXP,CPAREN,VALUE,EOL};
    void BinaryOp(token op,seqStack<int> &dataStack);
    token getOp(int &value);
    
public:
    calc(char *s){
        expression = new char[strlen(s)+1];
        strcpy(expression,s);
    }
    ~calc(){
        delete expression;
    }
    
    int result();
};

int calc::result(){
    token lastOp,topOp;
    int result_value,CurrentValue;
    seqStack<token> opStack;
    seqStack<int> dataStack;
    char *str = expression;
    while((lastOp = getOp(CurrentValue))!= EOL){
        switch (lastOp) {
            case VALUE:
                dataStack.push(CurrentValue);
                break;
            case CPAREN:
                while((topOp = opStack.pop()) != OPAREN && !opStack.isEmpty() )
                    BinaryOp(topOp, dataStack);
                if(topOp != OPAREN)
                    std::cerr << "缺开括号" << std::endl;
                break;
            case OPAREN:
                opStack.push(OPAREN);
                break;
            case EXP:
                opStack.push(EXP);
                break;
            case MULTI:case DIV:
                while(!opStack.isEmpty() && opStack.top() >= MULTI)
                    BinaryOp(opStack.pop(),dataStack);
                opStack.push(lastOp);
                break;
            case ADD:case SUB:
                while(!opStack.isEmpty() && opStack.top() != OPAREN)
                    BinaryOp(opStack.pop(), dataStack);
                opStack.push(lastOp);
            default:
                ;
        }
    }
    while(!opStack.isEmpty())
        BinaryOp(opStack.pop(), dataStack);
    if(dataStack.isEmpty()){
        std::cout << "无结果" << std::endl;
        return 0;
    }
    result_value = dataStack.pop();
    if(!dataStack.isEmpty()){
        std::cout << "缺操作符" << std::endl;
        return 0;
    }
    
    expression = str;
    return result_value;
}

void calc::BinaryOp(token op, seqStack<int> &dataStack){
    int num1,num2;
    
    if(dataStack.isEmpty()){
        std::cerr << "缺右运算数" ;
        exit(1);
    }
    else
        num2 = dataStack.pop();
    
    if(dataStack.isEmpty()){
        std::cerr << "缺左运算数" ;
        exit(1);
    }
    else
        num1 = dataStack.pop();
    
    switch (op) {
        case ADD:dataStack.push(num1+num2);break;
        case SUB:dataStack.push(num1-num2);break;
        case MULTI:dataStack.push(num1*num2);break;
        case DIV:dataStack.push(num1/num2);break;
        case EXP:dataStack.push(pow(num1,num2));break;
        default:
            ;
    }
}

calc::token calc::getOp(int &value){
    while(*expression && *expression == ' ')
        expression++;
    
    if(*expression) return EOL;
    
    if(*expression <= '9' && *expression >= '0'){
        value = 0;
        while(*expression >= '0' && *expression <= '9'){
            value = value * 10 + (*expression - '0');
            expression++;
        }
        return  VALUE;
    }
    
    switch (*expression) {
        case '(':expression++;return OPAREN;break;
        case ')':expression++;return CPAREN;break;
        case '+':expression++;return ADD;break;
        case '-':expression++;return SUB;break;
        case '*':expression++;return MULTI;break;
        case '/':expression++;return DIV;break;
        case '^':expression++;return EXP;break;
    }
    
    return EOL;
}


#endif /* stack_hpp */
