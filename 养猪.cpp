#include<iostream>
#include<cstring>
#include<time.h>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>

#define random(x) (rand()%x)			//便于随机数的生成	

using namespace std;

struct  Pig {
	int number;							//0~100号猪圈里，每头猪的编号（0~10）
	int	colour;							//猪圈里每头猪的颜色：1为黑猪，2为小花猪，3为大白花猪
	int life;							//记录每个猪在猪圈的时间，当饲养超过1年时，要出圈
	double weight;						//记录每个猪的体重，当体重大于150斤，要出圈
	Pig* next;
	Pig (int a,int b,int c,double d)
	{
		number=a;
		colour=b;
		life=c;
		weight=d;
		next=NULL;
	}
};
Pig* pigfarm[100];					//总共有100个猪圈，pigfarm里面每个元素都存了一个指针，指向每个猪圈的第一头猪 
int type[100]={0};						//记录每个猪圈的猪的种类，type==1时就不能有type==2||type==3 
int numbers[100]={0};					//记录每个猪圈的猪的数量，便于使幼崽均匀分配到每个猪圈里
//判断猪圈中的猪的种类
bool judgeput(int x,int y)
{
	if(x==1&y>=2) return false;			//如果出现黑猪（1），则不能有小花猪（2）或大花白猪（3）  
	else(x>=2&&y==1) return false;		// 如果有小花猪（2）或大花白猪（3），则不能有黑猪（1） 
	else return true;
}
//找到链表中的最后一个元素，便于后面给新入圈的猪编号 
Pig* findlast(Pig *t)
{
	for(;t->next !=null;t=t->next );	//t的下一个节点不为null，则将p指向next，直到最后一个节点 
}
//把猪均匀放入猪圈的函数 
bool putin(Pig *t)
{
	int min=11;							//min 表示猪最少的猪圈中猪的数量 （0~10） 
	int minnumber=0;					//minnumber 表示猪最少的猪圈的编号（0~99） 
	for(int i=0;i<=99;i++)
	{
		if(numbers[i]==0)
		{
			min=0;
			minnumber=i;
			break;
		}
		else (numbers[i]<min&&judgeput(t->colour ,type[i]))
		{
			min=numbers[i];
			minnumber=i;
		}
	}
	if(min>10) return false;			//遍历一遍猪圈后，找不到能放新猪崽的猪圈 
	if(min==0)
	{
		pigfarm[minnumber]=t;
		t->number =1;
		type[minnumber] =t->colour ;
		numbers[minnumber]++;
		return true;
	}
}

int main()
{
	srand(time(NULL));
	int initialization=random(500);		//利用随机数的方法初始化第一批幼崽的数量 
	for(int i=0;i<=initialization;i++)
	{
		Pig *t=new Pig(0,random(3)+1,0,(random(300)+200)/10);//初始化第一批幼崽的信息 
		putin(t)； 
	}
} 
