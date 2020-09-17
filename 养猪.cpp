#include<iostream>
#include<cstring>
#include<time.h>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<fstream> 

#define random(x) (rand()%x)            //便于随机数的生成

using namespace std;

struct  Pig {
    int number;                            //0~100号猪圈里，每头猪的编号（0~10）
    int colour;                            //猪圈里每头猪的颜色：1为黑猪，2为小花猪，3为大白花猪
    int life;                            //记录每个猪在猪圈的时间，当饲养超过1年时，要出圈
    double weight;                        //记录每个猪的体重，当体重大于150斤，要出圈
    bool sick;
    Pig* next;
    Pig(int a, int b, int c, double d,bool e)
    {
        number = a;
        colour = b;
        life = c;
        weight = d;
        sick = e;
        next = NULL;
    }
};

Pig* pigfarm[100];                    //总共有100个猪圈，pigfarm里面每个元素都存了一个指针，指向每个猪圈的第一头猪
int type[100] = { 0 };                        //记录每个猪圈的猪的种类，type==1时就不能有type==2||type==3
int numbers[100] = { 0 };                    //记录每个猪圈的猪的数量，便于使幼崽均匀分配到每个猪圈里

//判断猪圈中的猪的种类
bool judgeput(int x, int y)
{
    if (x == 1 && y >= 2) return false;            //如果出现黑猪（1），则不能有小花猪（2）或大花白猪（3）
    else if (x >= 2 && y == 1) return false;        // 如果有小花猪（2）或大花白猪（3），则不能有黑猪（1）
    else return true;
}

//找到链表中的最后一个元素，便于后面给新入圈的猪编号
Pig* findlast(Pig* t)
{
    while(t->next != nullptr) t=t->next ;   //t的下一个节点不为nullptr，则将p指向next，直到最后一个节点
    return t;
}

//把猪均匀放入猪圈的函数
bool putin(Pig* t)
{
    int min = 11;                            //min 表示猪最少的猪圈中猪的数量 （0~10）
    int minnumber = 0;                    //minnumber 表示猪最少的猪圈的编号（0~99）
    for (int i = 0; i <= 99; i++)
    {
        if (numbers[i] == 0)                //猪圈里没有猪
        {
            min = 0;                        //记录现在的最小值为0
            minnumber = i;                //记录最小值的猪圈编号为i
            break;                        //跳出循环
        }
        if (numbers[i] < min && judgeput(t->colour, type[i]))//如果每个猪圈都有猪 ，判断猪类型
        {
            min = numbers[i];                //记录现在的最小值为numbers[i]
            minnumber = i;                //记录最小值的猪圈编号为i
        }
    }
    if (min >= 10) return false;            //遍历一遍猪圈后，找不到能放新猪崽的猪圈
    if (min == 0)
    {
        t->number = 1;
        pigfarm[minnumber] = t;            //t指向i号猪圈的第一个位置                  //第一头猪放在i号猪圈的1号位置，猪的编号为1
        type[minnumber] = t->colour;    //猪圈的类型改为猪的类型
        numbers[minnumber]++;            //记录猪圈中猪的总数的numbers++
        return true;                    //这次putin结束，返回一个true，表示成功放入猪崽
    }
    else if (min < 10 && min>0)
    {
        Pig* temp = pigfarm[minnumber];    //temp指向第i个猪圈的第一个位置
        Pig* s;
        s = findlast(temp);                //用findlast，找到猪圈的最后一头猪
        s->next = t;                    //将新猪崽放入最后一头猪的后面位置
        t->number = s->number + 1;        //新猪崽的编号为原最后一头猪的编号+1
        numbers[minnumber]++;            //统计i号猪圈中猪的总数++
        return true;                    //这次putin结束，返回一个true，表示成功放入猪崽
    }
    return false;
}

//猪的体重增长和饲养时长增长函数
void gainweight()
{
    for (int i = 0; i <= 99; i++)
    {
        Pig* t = pigfarm[i];                    //t指向pigfarm的第一头猪
        while(t != nullptr)           //t是最后一头猪的next时跳出循环
        {
            t->weight += (random(12)) / 10.0;    //体重的增长用随机数生成
            t->life++;                        //饲养时长+1天
            t = t->next ;
        }
    }
}

//判断猪的体重或饲养时长满不满足要求，能不能出圈
bool sellable(Pig* t)
{
    if (t->weight > 75 || t->life > 365) return true;//饲养超过90天或体重超过150kg便可以出售
    return false;
}

//计算卖出的猪的价钱price
double price(Pig* t)
{
    double sum_price = 0;                            //初始化sum_price为0
    if (t->colour == 1) sum_price += t->weight * 30;//当t指向的是黑猪时，sum_price=体重*单价
    if (t->colour == 2) sum_price += t->weight * 14;//当t指向的是小花猪时，sum_price=体重*单价
    if (t->colour == 3) sum_price += t->weight * 12;//当t指向的是大白花猪时，sum_price=体重*单价
    return sum_price;
}


double  sellout(void)
{
    double sum = 0;                                        //sum记录本次出圈的总价格
    for (int i = 0; i <= 99; i++)                        //遍历猪圈
    {
        Pig* prev, * t;                                    //定义两个Pig指针，用于后面猪的出圈（节点删除）
        if (numbers[i] == 0) continue;                        //第i个猪圈没猪，continue，下一个猪圈
        while (pigfarm[i] != nullptr && sellable(pigfarm[i]))                           
		//第i个猪圈有猪的情况下，判断第一头猪满不满足卖出条件，准备卖出
        {
            sum += price(pigfarm[i]);                        //满足卖出条件，计算该猪的单价
            Pig* temp = pigfarm[i];
            pigfarm[i] = pigfarm[i]->next;
            delete temp;
            temp = nullptr;
            numbers[i]--;                            //猪圈中猪的总数-1
        }

        if (pigfarm[i] == nullptr) {
            numbers[i] = 0;
            continue;
        }                        //猪圈中只剩下了一头无法卖出的猪，continue，下一个猪圈
        pigfarm[i]->number = 1;
        if (pigfarm[i]->next == nullptr) continue;
        t = pigfarm[i];
        prev = t;
        t = t->next;
        t->number = prev->number + 1;
        if (t == nullptr) continue;
        while (t->next != nullptr)
        {
            if (sellable(t))
            {
                prev->next = t->next;
                sum += price(t);
                delete t;
                t = prev->next;
                t->number = prev->number + 1;
            }
            else
            {
                prev = t;
                t = t->next;
                t->number = prev->number + 1;
            }
            if (t->next == nullptr && sellable(t))
            {
                sum += price(t);
                prev->next = nullptr;
                delete t;
                numbers[i]--;
            }
        }
    }
    return sum;
}

int main()
{
    srand((unsigned)time(NULL));                    //用time函数改变随机数种子，实现真随机
    int initialization_1 = random(500);        //利用随机数的方法初始化第一批幼崽的数量
    for (int i = 0; i <= initialization_1; i++)
    {
        Pig* t = new Pig(0, random(3) + 1, 0, ((double)random(300) + 200.0) / 10.0, false);
        //初始化第一批幼崽的信息 ，编号都和饲养时长都初始化为0，后面的函数会编号和改变饲养时长，猪的品种和体重都用随机数生成
        putin(t);
    }
    int days = 0;
    while (days != 181)					//五年 
    {
        days++;
        gainweight();
        //每天都要给猪增长体重和饲养时长
        if (days % 90 == 0)                    //90天出圈一批猪
        {
            cout << "本次出圈的猪总体售价" << sellout() << endl;
            int initialization_2 = random(500);//卖完后随机生成新的猪崽
        	cout<<endl;
            for (int i = 0; i <= initialization_2; i++)
            {
                Pig* t = new Pig(0, random(3) + 1, 0, ((double)random(300) + 200.0) / 10.0, false);//初始化猪崽的信息
                if (putin(t) == false) break;    //执行判断函数putin，并将猪崽放入
            }
            cout<<"放入猪崽后各猪圈中猪的数量："<<endl; 
			for(int i=0;i<=99;i++) cout<<numbers[i]<<" ";
			cout<<endl<<endl; 
        }
    }
		
	//查询特定猪圈的猪的种类和数量
    Pig* t;
    int x;
    int a[3]={0};
    cout <<endl<< "输入你要查询的猪圈编号:";
    cin >> x;                                    //x为你要查询的猪圈编号
    t = pigfarm[x];
    for (; t != nullptr; t = t->next)
    {
		if(t->colour ==1) a[0]++;
		if(t->colour ==2) a[1]++;
		if(t->colour ==3) a[2]++;
    }
    t=nullptr;
    cout << "第"<<x<<"个猪圈中猪的种类  黑猪 : "<<a[0]<<" 花猪 ："<<a[1]<<" 大白花猪 : "<<a[2]; 
    
    // 查询当前某一个猪圈中某头猪的情况 
    cout<<endl<<"输入你要查询的猪的编号：";
    int no_pig;
	cin>>no_pig;
	t=pigfarm[x];
	if(no_pig>numbers[x]) cout<<"猪圈内没有这头猪"<<endl;
	else
	{
		int flag(1);
		for(;t!=nullptr;t=t->next )
		{
			if(flag==no_pig)
			{
				if(t->colour ==1) cout<<"猪的种类为黑猪"<<endl;
				else if(t->colour ==2) cout<<"猪的种类为白猪"<<endl;
				else if(t->colour ==3) cout<<"猪的种类为大白花猪"<<endl;
				cout<<"猪的饲养时长为"<<t->life<<"天"<<endl;
				cout<<"猪的体重为"<<t->weight<<"千克"<<endl;; 
			}
			flag++;
		}
		t=nullptr;
	}
	
	//统计当前猪圈每个种类猪的数量，体重，饲养时间分布情况
	int number_pigs[3]={0};
	int sum_pigs=0;
	double weight_pigs[3]={0};
	int life_pigs1[3]={0};				//0~90天的猪
	int life_pigs3[3]={0};				//90以上的猪 
	for(int i=0;i<=99;i++)
	{
		t=pigfarm[i];
		for(;t!=nullptr;t=t->next )
		{
			if(t->colour ==1)
			{
				number_pigs[0]++;
				weight_pigs[0]+=t->weight ;
				if(t->life <90) life_pigs1[0]++;
				else if(t->life>=90) life_pigs3[0]++;
			}
			else if(t->colour ==2)
			{
				number_pigs[1]++;
				weight_pigs[1]+=t->weight ;
				if(t->life <90) life_pigs1[1]++;
				else if(t->life>=90) life_pigs3[1]++;
			}
			else if(t->colour ==3)
			{
				number_pigs[2]++;
				weight_pigs[2]+=t->weight ;
				if(t->life <90) life_pigs1[2]++;
				else if(t->life>=90) life_pigs3[2]++;
			}
		}
	}
	t=nullptr;
	sum_pigs=number_pigs[0]+number_pigs[1]+number_pigs[2];
	cout<<"黑猪的总数量为："<<number_pigs[0]<<"头"<<endl;
	cout<<"花猪的总数量为："<<number_pigs[1]<<"头"<<endl;
	cout<<"大白花猪的总数量为："<<number_pigs[2]<<"头"<<endl;
	cout<<"黑猪的总体重为："<<weight_pigs[0]<<"kg"<<endl;
	cout<<" 花猪的总体重为："<<weight_pigs[1]<<"kg"<<endl;
	cout<<"大白花猪的总体重为："<<weight_pigs[2]<<"kg"<<endl;
	cout<<"黑猪饲养时长0~90天有："<<life_pigs1[0]<<"头"<<endl;
	cout<<"黑猪饲养时长90天以上有："<<life_pigs3[0]<<"头"<<endl;
	cout<<"花猪饲养时长0~90天有："<<life_pigs1[1]<<"头"<<endl;
	cout<<"花猪饲养时长90天以上有："<<life_pigs3[1]<<"头"<<endl;
	cout<<"大白花猪猪饲养时长0~90天有："<<life_pigs1[2]<<"头"<<endl;
	cout<<"大白花猪饲养时长90天以上有："<<life_pigs3[2]<<"头"<<endl;
	
	//模拟养猪场某一只猪得了猪瘟
	bool pigfarm_sick[100];
	memset(pigfarm_sick,false,sizeof(bool)*100);	//bool数组每个都置false，表示现在还没有猪生病 
	int randill_1=random(100);
	while(numbers[ranill_1]==0) randill_1=random(100);
	int randill_2=random(numbers[randil_1]),sickday=0;
	//随机生成一个生病的猪圈,编号为randill_1,随机生成一个生病的猪，该猪是猪圈中第randill_2头猪
	pigfarm_sick[randill]=true;
	int flag_sick=0;
	int sum_sickpigs=0;
	
	//编号为randill_2号的猪感染上猪瘟 
	for(t=pigfarm[randill_1];t!=nullptr;t=t->next )
	{
		if(randill_2==0)
		{
			t->sick =true;
			sum_sickpigs++;
			break;
		}
		else
		{
			t=t->next;
			flag_sick++;
			if(flag_sick==randill_2)
			{
				t->sick =true;
				sum_sickpigs++;
				break;
			}
		}
	}
	t=nullptr;
	
	//开始传播瘟疫 
	int sickfarm_no[100];//用于记录下有瘟疫存在的猪圈，方便之后遍历 
	memset(sickfarm_no,-1,sizeof(int)*100);
	sickfarm_n0[0]=randill_1;//randill_1已经出现了猪瘟
	while(sum_sickpigs!=sum_pigs)
	{
		days++; 
		//遍历sickfarm_no，直到猪圈中所有的猪都得上了猪瘟 
		for(int i=0;sickfarm_no[i]!=-1;i++)
		{
			if(sickfarm_no[i]==1)
			{
				for(t=pigfarm[2];t!=nullptr;t=t->next )
				{
					if(random(15)==1&&t->sick ==false)
					{
						t->sick =true;
						sum_sickpigs++;
						if(pigfarm_sick[2]=false)
						{
							pigfarm_sick[2]=true;
							int j;
							for(j=0;sickfarm_no[j]!=-1;j++);
							if(j<=99) sickfarm_no[j]=2;
						}
					}
				}
			}
			if(sickfarm_no[i]==99)
			{
				for(t=pigfarm[98];t!=nullptr;t=t->next )
				{
					if(random(15)==1&&t->sick ==false)
					{
						t->sick =true;
						sum_sickpigs++;
						if(pigfarm_sick[98]=false)
						{
							pigfarm_sick[98]=true;
							int j;
							for(j=0;sickfarm_no[j]!=-1;j++);
							if(j<=99) sickfarm_no[j]=98;
						}
					}
				}
			}
			else if
			{
				for(t=pigfarm[sickfarm_no[i-1]];t!=nullptr;t=t->next )
				{
					if(random(15)==1&&t->sick ==false)
					{
						t->sick =true;
						sum_sickpigs++;
						if(pigfarm_sick[sickfarm_no[i-1]]=false)
						{
							pigfarm_sick[sickfarm_no[i-1]]=true;
							int j;
							for(j=0;sickfarm_no[j]!=-1;j++);
							if(j<=99) sickfarm_no[j]=sickfarm_no[i-1];
						}
					}
				}
				for(t=pigfarm[sickfarm_no[i-1]];t!=nullptr;t=t->next )
				{
					if(random(15)==1&&t->sick ==false)
					{
						t->sick =true;
						sum_sickpigs++;
						if(pigfarm_sick[sickfarm_no[i+1]]=false)
						{
							pigfarm_sick[sickfarm_no[i+1]]=true;
							int j;
							for(j=0;sickfarm_no[j]!=-1;j++);
							if(j<=99) sickfarm_no[j]=sickfarm_no[i+1];
						}
					}
				}
			}
		}
		
	}
	cout<<"如果不采取隔离措施,"<<days<<"天猪全部死光"; 

	//如果采取隔离措施
	int randill_isolate1=random(100) ;
	while(randill_isolate==0) randill_isolate=random(100) ;
	int randill_isolate2=random(numbers[randill_isolate1]);
	for(t=pigfarm[randill_isolate1];t!=nullptr;t=t->next )
	{
		if(numbers[randill_isolate1]==1)
		{
			delete t;
			t=nullptr;
			numbers[randill_isolate1]=0;
			break;
		}
		else
		{
			if(randill_isolate2==0)
			{
				pigfarm[randill_isolate1]=
			}
		}
	}
}
