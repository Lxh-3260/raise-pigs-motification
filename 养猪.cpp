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
	if(x==1&&y>=2) return false;			//如果出现黑猪（1），则不能有小花猪（2）或大花白猪（3）  
	else if(x>=2&&y==1) return false;		// 如果有小花猪（2）或大花白猪（3），则不能有黑猪（1） 
	else return true;
}

//找到链表中的最后一个元素，便于后面给新入圈的猪编号 
Pig* findlast(Pig *t)
{
	for(;t->next !=nullptr;t=t->next );	//t的下一个节点不为nullptr，则将p指向next，直到最后一个节点 
	return t;
}

//把猪均匀放入猪圈的函数 
bool putin(Pig *t)
{
	int min=11;							//min 表示猪最少的猪圈中猪的数量 （0~10） 
	int minnumber=0;					//minnumber 表示猪最少的猪圈的编号（0~99） 
	for(int i=0;i<=99;i++)
	{
		if(numbers[i]==0)				//猪圈里没有猪 
		{
			min=0;						//记录现在的最小值为0 
			minnumber=i;				//记录最小值的猪圈编号为i 
			break;						//跳出循环 
		}
		else if(numbers[i]<min&&judgeput(t->colour ,type[i]))//如果每个猪圈都有猪 ，判断猪类型 
		{
			min=numbers[i];				//记录现在的最小值为0 
			minnumber=i;				//记录最小值的猪圈编号为i 
		}
	}
	if(min>10) return false;			//遍历一遍猪圈后，找不到能放新猪崽的猪圈 
	if(min==0)
	{
		pigfarm[minnumber]=t;			//t指向i号猪圈的第一个位置 
		t->number =1;					//第一头猪放在i号猪圈的1号位置，猪的编号为1 
		type[minnumber] =t->colour ;	//猪圈的类型改为猪的类型 
		numbers[minnumber]++;			//记录猪圈中猪的总数的numbers++ 
		return true;					//这次putin结束，返回一个true，表示成功放入猪崽 
	}
	else
	{
		Pig *temp=pigfarm[minnumber];	//temp指向第i个猪圈的第一个位置 
		temp=findlast(temp);			//用findlast，找到猪圈的最后一头猪
		t=temp->next;					//将新猪崽放入最后一头猪的后面位置 
		t->number =temp->number ++;		//新猪崽的编号为原最后一头猪的编号++ 
		numbers[minnumber]++;			//统计i号猪圈中猪的总数++ 
		return true;					//这次putin结束，返回一个true，表示成功放入猪崽
	} 
}

//猪的体重增长和饲养时长增长函数 
void gainweight()
{
	for(int i=0;i<=99;i++)
	{
		Pig *t=pigfarm [i];					//t指向pigfarm的第一头猪 
		for(;t!=nullptr;t=t->next )			//t是最后一头猪的next时跳出循环 
		{
			t->weight +=(random(12))/10.0;	//体重的增长用随机数生成 
			t->life ++;						//饲养时长+1天 
		}
	}
} 

//判断猪的体重或饲养时长满不满足要求，能不能出圈
bool sellable(Pig *t)
{
	if(t->weight >75||t->life >365) return true;//饲养超过90天或体重超过150kg便可以出售 
	else return false;
} 

//计算卖出的猪的价钱price
double price(Pig *t)
{
	double sum_price=0;
	if(t->colour ==1) sum_price+=t->weight *30;
	if(t->colour ==2) sum_price+=t->weight *14;
	if(t->colour ==3) sum_price+=t->weight *12;
	return sum_price;
} 
 
//卖出猪崽 ，返回值为本次出圈的总价 
double  sellout(void)
{
	double sum=0;
	Pig *prev,*t;
	for(int i=0;i<=99;i++)
	{
		t=pigfarm[i];
		if(pigfarm[i]==nullptr) continue;
		if(sellable(t)&&t->next !=nullptr)
		{
			sum+=price(t);
			prev=t;
			t=t->next ;
			delete prev;
			prev=nullptr;
			numbers[i]--;
		}
		t->number =1;
		if(t->next ==nullptr) continue;
		else
		{
			while(t!=nullptr)
			{
				prev=t;
				t=t->next ;
				if(sellable(t))
				{
					sum+=price(t);
					prev->next=t->next;
					delete t;
					t=prev->next;
					if(t!=nullptr) t->number=prev->number+1;
					numbers[i]--;	
				}	
			}
		}
	}
	return sum;
}

int main()
{
	srand(time(NULL));					//用time函数改变随机数种子，实现真随机 
	int initialization_1=random(500);		//利用随机数的方法初始化第一批幼崽的数量 
	for(int i=0;i<=initialization_1;i++)
	{
		Pig *t=new Pig(0,random(3)+1,0,(random(300)+200)/10.0);
		//初始化第一批幼崽的信息 ，编号都和饲养时长都初始化为0，后面的函数会编号和改变饲养时长，猪的品种和体重都用随机数生成 
		putin(t); 
	}
	int days=0;
	while(days!=365)
	{
		days++;
		gainweight();					//每天都要给猪增长体重和饲养时长 
		if(days%90==0)					//90天出圈一批猪 
		{
			cout<<"本次出圈的猪总体售价"<<sellout()<<endl;
			int initialization_2=random(300);
			for(int i=0;i<=initialization_2;i++)
			{
				Pig *t=new Pig(0,random(3)+1,0,(random(300)+200)/10.0);
				if(putin(t)==false) break;	//执行判断函数putin，并将猪崽放入 
			}
		}
	}
	
} 
