#include<iostream>
#include<cstring>
#include<time.h>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<fstream> 

#define random(x) (rand()%x)            //���������������

using namespace std;

struct  Pig {
    int number;                            //0~100����Ȧ�ÿͷ��ı�ţ�0~10��
    int colour;                            //��Ȧ��ÿͷ�����ɫ��1Ϊ����2ΪС����3Ϊ��׻���
    int life;                            //��¼ÿ��������Ȧ��ʱ�䣬����������1��ʱ��Ҫ��Ȧ
    double weight;                        //��¼ÿ��������أ������ش���150�Ҫ��Ȧ
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

Pig* pigfarm[100];                    //�ܹ���100����Ȧ��pigfarm����ÿ��Ԫ�ض�����һ��ָ�룬ָ��ÿ����Ȧ�ĵ�һͷ��
int type[100] = { 0 };                        //��¼ÿ����Ȧ��������࣬type==1ʱ�Ͳ�����type==2||type==3
int numbers[100] = { 0 };                    //��¼ÿ����Ȧ���������������ʹ���̾��ȷ��䵽ÿ����Ȧ��

//�ж���Ȧ�е��������
bool judgeput(int x, int y)
{
    if (x == 1 && y >= 2) return false;            //������ֺ���1����������С����2����󻨰���3��
    else if (x >= 2 && y == 1) return false;        // �����С����2����󻨰���3���������к���1��
    else return true;
}

//�ҵ������е����һ��Ԫ�أ����ں��������Ȧ������
Pig* findlast(Pig* t)
{
    while(t->next != nullptr) t=t->next ;   //t����һ���ڵ㲻Ϊnullptr����pָ��next��ֱ�����һ���ڵ�
    return t;
}

//������ȷ�����Ȧ�ĺ���
bool putin(Pig* t)
{
    int min = 11;                            //min ��ʾ�����ٵ���Ȧ��������� ��0~10��
    int minnumber = 0;                    //minnumber ��ʾ�����ٵ���Ȧ�ı�ţ�0~99��
    for (int i = 0; i <= 99; i++)
    {
        if (numbers[i] == 0)                //��Ȧ��û����
        {
            min = 0;                        //��¼���ڵ���СֵΪ0
            minnumber = i;                //��¼��Сֵ����Ȧ���Ϊi
            break;                        //����ѭ��
        }
        if (numbers[i] < min && judgeput(t->colour, type[i]))//���ÿ����Ȧ������ ���ж�������
        {
            min = numbers[i];                //��¼���ڵ���СֵΪnumbers[i]
            minnumber = i;                //��¼��Сֵ����Ȧ���Ϊi
        }
    }
    if (min >= 10) return false;            //����һ����Ȧ���Ҳ����ܷ������̵���Ȧ
    if (min == 0)
    {
        t->number = 1;
        pigfarm[minnumber] = t;            //tָ��i����Ȧ�ĵ�һ��λ��                  //��һͷ�����i����Ȧ��1��λ�ã���ı��Ϊ1
        type[minnumber] = t->colour;    //��Ȧ�����͸�Ϊ�������
        numbers[minnumber]++;            //��¼��Ȧ�����������numbers++
        return true;                    //���putin����������һ��true����ʾ�ɹ���������
    }
    else if (min < 10 && min>0)
    {
        Pig* temp = pigfarm[minnumber];    //tempָ���i����Ȧ�ĵ�һ��λ��
        Pig* s;
        s = findlast(temp);                //��findlast���ҵ���Ȧ�����һͷ��
        s->next = t;                    //�������̷������һͷ��ĺ���λ��
        t->number = s->number + 1;        //�����̵ı��Ϊԭ���һͷ��ı��+1
        numbers[minnumber]++;            //ͳ��i����Ȧ���������++
        return true;                    //���putin����������һ��true����ʾ�ɹ���������
    }
    return false;
}

//�����������������ʱ����������
void gainweight()
{
    for (int i = 0; i <= 99; i++)
    {
        Pig* t = pigfarm[i];                    //tָ��pigfarm�ĵ�һͷ��
        while(t != nullptr)           //t�����һͷ���nextʱ����ѭ��
        {
            t->weight += (random(12)) / 10.0;    //���ص����������������
            t->life++;                        //����ʱ��+1��
            t = t->next ;
        }
    }
}

//�ж�������ػ�����ʱ����������Ҫ���ܲ��ܳ�Ȧ
bool sellable(Pig* t)
{
    if (t->weight > 75 || t->life > 365) return true;//��������90������س���150kg����Գ���
    return false;
}

//������������ļ�Ǯprice
double price(Pig* t)
{
    double sum_price = 0;                            //��ʼ��sum_priceΪ0
    if (t->colour == 1) sum_price += t->weight * 30;//��tָ����Ǻ���ʱ��sum_price=����*����
    if (t->colour == 2) sum_price += t->weight * 14;//��tָ�����С����ʱ��sum_price=����*����
    if (t->colour == 3) sum_price += t->weight * 12;//��tָ����Ǵ�׻���ʱ��sum_price=����*����
    return sum_price;
}


double  sellout(void)
{
    double sum = 0;                                        //sum��¼���γ�Ȧ���ܼ۸�
    for (int i = 0; i <= 99; i++)                        //������Ȧ
    {
        Pig* prev, * t;                                    //��������Pigָ�룬���ں�����ĳ�Ȧ���ڵ�ɾ����
        if (numbers[i] == 0) continue;                        //��i����Ȧû��continue����һ����Ȧ
        while (pigfarm[i] != nullptr && sellable(pigfarm[i]))                           
		//��i����Ȧ���������£��жϵ�һͷ��������������������׼������
        {
            sum += price(pigfarm[i]);                        //���������������������ĵ���
            Pig* temp = pigfarm[i];
            pigfarm[i] = pigfarm[i]->next;
            delete temp;
            temp = nullptr;
            numbers[i]--;                            //��Ȧ���������-1
        }

        if (pigfarm[i] == nullptr) {
            numbers[i] = 0;
            continue;
        }                        //��Ȧ��ֻʣ����һͷ�޷���������continue����һ����Ȧ
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
    srand((unsigned)time(NULL));                    //��time�����ı���������ӣ�ʵ�������
    int initialization_1 = random(500);        //����������ķ�����ʼ����һ�����̵�����
    for (int i = 0; i <= initialization_1; i++)
    {
        Pig* t = new Pig(0, random(3) + 1, 0, ((double)random(300) + 200.0) / 10.0, false);
        //��ʼ����һ�����̵���Ϣ ����Ŷ�������ʱ������ʼ��Ϊ0������ĺ������ź͸ı�����ʱ�������Ʒ�ֺ����ض������������
        putin(t);
    }
    int days = 0;
    while (days != 181)					//���� 
    {
        days++;
        gainweight();
        //ÿ�춼Ҫ�����������غ�����ʱ��
        if (days % 90 == 0)                    //90���Ȧһ����
        {
            cout << "���γ�Ȧ���������ۼ�" << sellout() << endl;
            int initialization_2 = random(500);//�������������µ�����
        	cout<<endl;
            for (int i = 0; i <= initialization_2; i++)
            {
                Pig* t = new Pig(0, random(3) + 1, 0, ((double)random(300) + 200.0) / 10.0, false);//��ʼ�����̵���Ϣ
                if (putin(t) == false) break;    //ִ���жϺ���putin���������̷���
            }
            cout<<"�������̺����Ȧ�����������"<<endl; 
			for(int i=0;i<=99;i++) cout<<numbers[i]<<" ";
			cout<<endl<<endl; 
        }
    }
		
	//��ѯ�ض���Ȧ��������������
    Pig* t;
    int x;
    int a[3]={0};
    cout <<endl<< "������Ҫ��ѯ����Ȧ���:";
    cin >> x;                                    //xΪ��Ҫ��ѯ����Ȧ���
    t = pigfarm[x];
    for (; t != nullptr; t = t->next)
    {
		if(t->colour ==1) a[0]++;
		if(t->colour ==2) a[1]++;
		if(t->colour ==3) a[2]++;
    }
    t=nullptr;
    cout << "��"<<x<<"����Ȧ���������  ���� : "<<a[0]<<" ���� ��"<<a[1]<<" ��׻��� : "<<a[2]; 
    
    // ��ѯ��ǰĳһ����Ȧ��ĳͷ������ 
    cout<<endl<<"������Ҫ��ѯ����ı�ţ�";
    int no_pig;
	cin>>no_pig;
	t=pigfarm[x];
	if(no_pig>numbers[x]) cout<<"��Ȧ��û����ͷ��"<<endl;
	else
	{
		int flag(1);
		for(;t!=nullptr;t=t->next )
		{
			if(flag==no_pig)
			{
				if(t->colour ==1) cout<<"�������Ϊ����"<<endl;
				else if(t->colour ==2) cout<<"�������Ϊ����"<<endl;
				else if(t->colour ==3) cout<<"�������Ϊ��׻���"<<endl;
				cout<<"�������ʱ��Ϊ"<<t->life<<"��"<<endl;
				cout<<"�������Ϊ"<<t->weight<<"ǧ��"<<endl;; 
			}
			flag++;
		}
		t=nullptr;
	}
	
	//ͳ�Ƶ�ǰ��Ȧÿ������������������أ�����ʱ��ֲ����
	int number_pigs[3]={0};
	int sum_pigs=0;
	double weight_pigs[3]={0};
	int life_pigs1[3]={0};				//0~90�����
	int life_pigs3[3]={0};				//90���ϵ��� 
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
	cout<<"�����������Ϊ��"<<number_pigs[0]<<"ͷ"<<endl;
	cout<<"�����������Ϊ��"<<number_pigs[1]<<"ͷ"<<endl;
	cout<<"��׻����������Ϊ��"<<number_pigs[2]<<"ͷ"<<endl;
	cout<<"�����������Ϊ��"<<weight_pigs[0]<<"kg"<<endl;
	cout<<" �����������Ϊ��"<<weight_pigs[1]<<"kg"<<endl;
	cout<<"��׻����������Ϊ��"<<weight_pigs[2]<<"kg"<<endl;
	cout<<"��������ʱ��0~90���У�"<<life_pigs1[0]<<"ͷ"<<endl;
	cout<<"��������ʱ��90�������У�"<<life_pigs3[0]<<"ͷ"<<endl;
	cout<<"��������ʱ��0~90���У�"<<life_pigs1[1]<<"ͷ"<<endl;
	cout<<"��������ʱ��90�������У�"<<life_pigs3[1]<<"ͷ"<<endl;
	cout<<"��׻���������ʱ��0~90���У�"<<life_pigs1[2]<<"ͷ"<<endl;
	cout<<"��׻�������ʱ��90�������У�"<<life_pigs3[2]<<"ͷ"<<endl;
	
	//ģ������ĳһֻ���������
	bool pigfarm_sick[100];
	memset(pigfarm_sick,false,sizeof(bool)*100);	//bool����ÿ������false����ʾ���ڻ�û�������� 
	int randill_1=random(100);
	while(numbers[ranill_1]==0) randill_1=random(100);
	int randill_2=random(numbers[randil_1]),sickday=0;
	//�������һ����������Ȧ,���Ϊrandill_1,�������һ������������������Ȧ�е�randill_2ͷ��
	pigfarm_sick[randill]=true;
	int flag_sick=0;
	int sum_sickpigs=0;
	
	//���Ϊrandill_2�ŵ����Ⱦ������ 
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
	
	//��ʼ�������� 
	int sickfarm_no[100];//���ڼ�¼�������ߴ��ڵ���Ȧ������֮����� 
	memset(sickfarm_no,-1,sizeof(int)*100);
	sickfarm_n0[0]=randill_1;//randill_1�Ѿ�����������
	while(sum_sickpigs!=sum_pigs)
	{
		days++; 
		//����sickfarm_no��ֱ����Ȧ�����е������������� 
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
	cout<<"�������ȡ�����ʩ,"<<days<<"����ȫ������"; 

	//�����ȡ�����ʩ
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
