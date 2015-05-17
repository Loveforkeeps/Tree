//
//  main.cpp
//  Tree
//
//  Created by EMo‘s mac  on 5/13/15.
//  Copyright (c) 2015 EMo‘s mac . All rights reserved.
//
// Tree.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include<stdlib.h>
#include<time.h>
#define N 100 //节点数

//顺序表表示的树
struct table_element {
    int name;
    int parent_name;
};
//用双亲表示法表示的树
table_element tree_nodes[N];

//生成一个随机的多叉树
void genenrat(table_element* table,int count){
    srand((unsigned)time(NULL));
    tree_nodes[0].name=0;
    tree_nodes[0].parent_name=NULL;
    for(int i=1;i < N;i++){
        tree_nodes[i].name=i;
        tree_nodes[i].parent_name=rand()%i;
    }
}

//2叉树节点，用链式结构表示的二叉树
struct node {
    int name;
    node * left;
    node * right;
};

//根据name从二叉树中找相应节点
node * findParentNode(node * root,int name){
    node * ret=NULL;
    if(root==NULL)return ret;
    if(root->name==name) return root;
    ret=findParentNode(root->left,name);
    if(ret!=NULL) return ret;
    return findParentNode(root->right,name);
}

//将一棵随机生成的树用孩子-兄弟法表示法转换成2叉树
//兄弟节点的顺序按照 节点属性name的顺序
node * create(table_element * table,int count){
    node * root=NULL;
    
    if(count==0) return root;
    
    root =(node *)malloc(sizeof(node));
    root->name=0;
    root->left=NULL;
    root->right=NULL;
    
    node * tmp_node,*tmp_parent_node;
    
    for(int i=1;i<count;i++){
        tmp_node=(node *)malloc(sizeof(node));
        tmp_node->name=table[i].name;
        tmp_node->left=NULL;
        tmp_node->right=NULL;
        tmp_parent_node=findParentNode(root,table[i].parent_name);
        
        //left指向第一个孩子，right指向下一个兄弟
        if(tmp_parent_node->left==NULL)
            tmp_parent_node->left=tmp_node;
        else{
            tmp_parent_node=tmp_parent_node->left;
            //找到当前最后一个兄弟
            while(tmp_parent_node->right!=NULL)
                tmp_parent_node=tmp_parent_node->right;
            tmp_parent_node->right=tmp_node;
        }
    }
    return root;
}

//打印二叉树
void printTree(node * root,int level ){
    if(root==NULL) return;
    for(int i=0;i<level;i++)
        printf("—");
    printf("%d\n",root->name);
    printTree(root->left, level+1 );
    printTree(root->right, level+1 );
}


//***********************非递归方式实现中，前，后序遍历***********************************

typedef node* bintree;
// 用栈来存储
typedef struct seqstack{
    bintree data[N];
    int tag[N];   //为后续遍历准备的
    int top;     //top为数组的下标
}seqstack;

void push( seqstack *s,bintree t){  //将二叉树入栈
    
    if(s->top == N){
        printf("the stack is full\n");
    }else{
        s->top++;
        s->data[s->top]=t;
    }
}

bintree pop(seqstack *s){    //出栈
    if(s->top == -1){ //如果栈为空
        return NULL;
    }else{
        s->top--;
        return s->data[s->top+1];
    }
}
void midorder(bintree t){    //中序遍历
    seqstack s;
    s.top = -1;//定义初始化一个栈空间
    if(!t){
        printf("the tree is empty!\n");
    }
    else{
        while(t ||s.top != -1){
            while(t){       //只要结点不为空就应该入栈保存，与其左右结点无关
                push(&s,t);
                t= t->left;
            }
            t=pop(&s);
            printf("%d ",t->name);
            t=t->right;
        }
    }
}
void preorder_dev(bintree t){  //前序遍历
    seqstack s;
    s.top = -1;
    if(!t){
        printf("the tree is empty\n");
    }else{
        while(t || s.top != -1){
            while(t){
                printf("%d ",t->name);
                push(&s,t);
                t= t->left;
            }
        t=pop(&s);
        t=t->right;
            }
         }
}
void postorder_dev(bintree t){  //后续遍历
    seqstack s;
    s.top = -1;
    if(!t){
        printf("the tree is empty!\n");
    }else{
        while(t || s.top != -1){    //栈空了的同时t也为空。
            while(t){
                push(&s,t);
                s.tag[s.top] = 0;   //设置访问标记，0为第一次访问，1为第二次访问
                t= t->left;
            }
            if(s.tag[s.top] == 0){  //第一次访问时，转向同层右结点
                t= s.data[s.top];   //左走到底时t是为空的，必须有这步！
                s.tag[s.top]=1;
                t=t->right;
            }else {
                while (s.tag[s.top] == 1){ //找到栈中下一个第一次访问的结点，退出循环时并没有pop所以为其左子结点
                    t = pop(&s);
                    printf("%d ",t->name);
                }
                t = NULL; //必须将t置空。跳过向左走，直接向右走
            }
        }
    }
}


//********************************递归方式实现中，前，后序遍历****************************

//  中序遍历
void ZhongOrder(bintree t){
    if (t) {
        ZhongOrder(t->left);
        printf("%d ",t->name);
        ZhongOrder(t->right);
    }
}
// 前序遍历
void QianOder(bintree t){
    if (t) {
        printf("%d ",t->name);
        QianOder(t->left);
        QianOder(t->right);
    }
}
//后序遍历
void Houorder(bintree t){
    if (t) {
        Houorder(t->left);
        Houorder(t->right);
        printf("%d ",t->name);
    }
}
int main(int argc,const char * argv[]){
    
    genenrat(tree_nodes,N);		//生成一个随机的树
    
    node * root=create(tree_nodes,N);	//根据树生成2叉树
    
    printTree(root,0); //打印
    
    clock_t start,end;
    
    start = clock();
    //中序遍历
    printf("中序遍历：");
    midorder(root);
    //前序遍历
    printf("\n前序遍历：");
    preorder_dev(root);
    //后续遍历
    printf("\n后序遍历：");
    postorder_dev(root);
    end = clock();
    printf("\n非递归遍历总用时：%lu",end - start);
    
    
    printf("\n递归遍历算法：\n");
    start = clock();
    //中序遍历
    printf("中序遍历：");
    ZhongOrder(root);
    //前序遍历
    printf("\n前序遍历：");
    QianOder(root);
    //后续遍历
    printf("\n后序遍历：");
    Houorder(root);
    end = clock();
    printf("\n递归遍历算法总耗时：%lu",end - start);
    return 0;
}

