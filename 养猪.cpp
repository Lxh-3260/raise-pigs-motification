#include<iostream>
#include<cstring>
#include<time.h>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>

#define random(x) (rand()%x)			//���������������	

using namespace std;

struct  Pig {
	int number;							//0~100����Ȧ�ÿͷ��ı�ţ�0~10��
	int	colour;							//��Ȧ��ÿͷ�����ɫ��1Ϊ����2ΪС����3Ϊ��׻���
	int life;							//��¼ÿ��������Ȧ��ʱ�䣬����������1��ʱ��Ҫ��Ȧ
	double weight;						//��¼ÿ��������أ������ش���150�Ҫ��Ȧ
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

Pig* pigfarm[100];					//�ܹ���100����Ȧ��pigfarm����ÿ��Ԫ�ض�����һ��ָ�룬ָ��ÿ����Ȧ�ĵ�һͷ�� 
int type[100]={0};						//��¼ÿ����Ȧ��������࣬type==1ʱ�Ͳ�����type==2||type==3 
int numbers[100]={0};					//��¼ÿ����Ȧ���������������ʹ���̾��ȷ��䵽ÿ����Ȧ��

//�ж���Ȧ�е��������
bool judgeput(int x,int y)
{
	if(x==1&&y>=2) return false;			//������ֺ���1����������С����2����󻨰���3��  
	else if(x>=2&&y==1) return false;		// �����С����2����󻨰���3���������к���1�� 
	else return true;
}

//�ҵ������е����һ��Ԫ�أ����ں��������Ȧ������ 
Pig* findlast(Pig *t)
{
	for(;t->next !=nullptr;t=t->next );	//t����һ���ڵ㲻Ϊnullptr����pָ��next��ֱ�����һ���ڵ� 
	return t;
}

//������ȷ�����Ȧ�ĺ��� 
bool putin(Pig *t)
{
	int min=11;							//min ��ʾ�����ٵ���Ȧ��������� ��0~10�� 
	int minnumber=0;					//minnumber ��ʾ�����ٵ���Ȧ�ı�ţ�0~99�� 
	for(int i=0;i<=99;i++)
	{
		if(numbers[i]==0)				//��Ȧ��û���� 
		{
			min=0;						//��¼���ڵ���СֵΪ0 
			minnumber=i;				//��¼��Сֵ����Ȧ���Ϊi 
			break;						//����ѭ�� 
		}
		else if(numbers[i]<min&&judgeput(t->colour ,type[i]))//���ÿ����Ȧ������ ���ж������� 
		{
			min=numbers[i];				//��¼���ڵ���СֵΪ0 
			minnumber=i;				//��¼��Сֵ����Ȧ���Ϊi 
		}
	}
	if(min>10) return false;			//����һ����Ȧ���Ҳ����ܷ������̵���Ȧ 
	if(min==0)
	{
		pigfarm[minnumber]=t;			//tָ��i����Ȧ�ĵ�һ��λ�� 
		t->number =1;					//��һͷ�����i����Ȧ��1��λ�ã���ı��Ϊ1 
		type[minnumber] =t->colour ;	//��Ȧ�����͸�Ϊ������� 
		numbers[minnumber]++;			//��¼��Ȧ�����������numbers++ 
		return true;					//���putin����������һ��true����ʾ�ɹ��������� 
	}
	else
	{
		Pig *temp=pigfarm[minnumber];	//tempָ���i����Ȧ�ĵ�һ��λ�� 
		temp=findlast(temp);			//��findlast���ҵ���Ȧ�����һͷ��
		t=temp->next;					//�������̷������һͷ��ĺ���λ�� 
		t->number =temp->number ++;		//�����̵ı��Ϊԭ���һͷ��ı��++ 
		numbers[minnumber]++;			//ͳ��i����Ȧ���������++ 
		return true;					//���putin����������һ��true����ʾ�ɹ���������
	} 
}

//�����������������ʱ���������� 
void gainweight()
{
	for(int i=0;i<=99;i++)
	{
		Pig *t=pigfarm [i];					//tָ��pigfarm�ĵ�һͷ�� 
		for(;t!=nullptr;t=t->next )			//t�����һͷ���nextʱ����ѭ�� 
		{
			t->weight +=(random(12))/10.0;	//���ص���������������� 
			t->life ++;						//����ʱ��+1�� 
		}
	}
} 

//�ж�������ػ�����ʱ����������Ҫ���ܲ��ܳ�Ȧ
bool sellable(Pig *t)
{
	if(t->weight >75||t->life >365) return true;//��������90������س���150kg����Գ��� 
	else return false;
} 

//������������ļ�Ǯprice
double price(Pig *t)
{
	double sum_price=0;
	if(t->colour ==1) sum_price+=t->weight *30;
	if(t->colour ==2) sum_price+=t->weight *14;
	if(t->colour ==3) sum_price+=t->weight *12;
	return sum_price;
} 
 
//�������� ������ֵΪ���γ�Ȧ���ܼ� 
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
	srand(time(NULL));					//��time�����ı���������ӣ�ʵ������� 
	int initialization_1=random(500);		//����������ķ�����ʼ����һ�����̵����� 
	for(int i=0;i<=initialization_1;i++)
	{
		Pig *t=new Pig(0,random(3)+1,0,(random(300)+200)/10.0);
		//��ʼ����һ�����̵���Ϣ ����Ŷ�������ʱ������ʼ��Ϊ0������ĺ������ź͸ı�����ʱ�������Ʒ�ֺ����ض������������ 
		putin(t); 
	}
	int days=0;
	while(days!=365)
	{
		days++;
		gainweight();					//ÿ�춼Ҫ�����������غ�����ʱ�� 
		if(days%90==0)					//90���Ȧһ���� 
		{
			cout<<"���γ�Ȧ���������ۼ�"<<sellout()<<endl;
			int initialization_2=random(300);
			for(int i=0;i<=initialization_2;i++)
			{
				Pig *t=new Pig(0,random(3)+1,0,(random(300)+200)/10.0);
				if(putin(t)==false) break;	//ִ���жϺ���putin���������̷��� 
			}
		}
	}
	
} 
