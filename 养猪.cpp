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
	int number;                            //0~100����Ȧ�ÿͷ��ı�ţ�1~10��
	int colour;                            //��Ȧ��ÿͷ�����ɫ��1Ϊ����2ΪС����3Ϊ��׻���
	int life;                            //��¼ÿ��������Ȧ��ʱ�䣬����������1��ʱ��Ҫ��Ȧ
	double weight;                        //��¼ÿ��������أ������ش���150�Ҫ��Ȧ
	bool sick;
	Pig* next;
	Pig(int a, int b, int c, double d, bool e)
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
	if (x == 1 && y == 2) return false;            //������ֺ���1����������С����2����󻨰���3
	if (x == 1 && y == 3) return false;
	if (x = 2 && y == 1) return false;        // �����С����2����󻨰���3���������к���1��
	if (x = 3 && y == 1) return false;
	return true;
}

//�ҵ������е����һ��Ԫ�أ����ں��������Ȧ������
Pig* findlast(Pig* t)
{
	while (t->next != nullptr) t = t->next;   //t����һ���ڵ㲻Ϊnullptr����pָ��next��ֱ�����һ���ڵ�
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
		while (t != nullptr)           //t�����һͷ���nextʱ����ѭ��
		{
			t->weight += (random(12)) / 10.0;    //���ص����������������
			t->life++;                        //����ʱ��+1��
			t = t->next;
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
		if (nullptr == prev->next) continue;
		t = prev->next;
		t->number = prev->number + 1;
		while ((t != nullptr) && (nullptr != t->next))
		{
			if (sellable(t))
			{
				prev->next = t->next;
				sum += price(t);
				delete t;
				t = prev->next;
				t->number = prev->number + 1;
				numbers[i]--;
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
				t = nullptr;
				numbers[i]--;
			}
		}
	}
	return sum;
}
//�ж���Ȧ�е�����û������ 
bool alldie()
{
	for (int i = 0; i <= 99; i++)
	{
		for (Pig* s = pigfarm[i]; s != nullptr; s = s->next)
		{
			if (s->sick == false) return false;
		}
	}
	return true;
}

int main()
{
	srand((unsigned)time(NULL));                    //��time�����ı���������ӣ�ʵ�������
	int initialization_1 = random(500);        //����������ķ�����ʼ����һ�����̵�����

	ofstream s_save;
	s_save.open("buyrecord.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ������� 
	ofstream t_save;
	t_save.open("sellrecord.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ������� 
	ofstream s;
	s.open("pig_information.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ������� 

	for (int i = 0; i <= 80; i++)cout << "=";
	cout << endl << endl;
	for (int i = 0; i <= 32; i++)cout << " ";
	cout << "��ӭ�����ҵ�����"<<endl<<endl;
	for (int i = 0; i <= 80; i++)cout << "=";
	cout << endl << endl;
	cout << "����1�������������ģ�Ⲣ�洢���ݵ�txt�ļ�"<<endl<<endl;
	int control0;
	cin >> control0;
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	int hei = 0, hua = 0, bai = 0;
	for (int i = 0; i <= 900; i++)
	{
		Pig* t = new Pig(0, random(3) + 1, 0, ((double)random(300) + 200.0) / 10.0, false);
		//��ʼ����һ�����̵���Ϣ ����Ŷ�������ʱ������ʼ��Ϊ0������ĺ������ź͸ı�����ʱ�������Ʒ�ֺ����ض������������
		if (t->colour == 1) hei++;
		if (t->colour == 2) hua++;
		if (t->colour == 3) bai++;
		putin(t);
	}
	s_save.open("buyrecord.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ�������
	s_save << "��ʼ��Ϸ��һ�ι���:" << hei << "ͷ���� " << hua << "ͷС���� " << bai << "ͷ��׻���" << endl;
	s_save.close();//ÿ�δ浵֮��Ҫ�ر� 

	int days = 0;
	while (days != 1850)					//���� 
	{
		days++;
		gainweight();
		//ÿ�춼Ҫ�����������غ�����ʱ��
		if (days % 90 == 0)                    //90���Ȧһ����
		{
			double sell_1 = 0;
			sell_1 = sellout();
			t_save.open("sellrecord.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ�������
			t_save << "���γ�Ȧ���������ۼ�" << sell_1 << endl;
			t_save.close();

			cout << "���γ�Ȧ���������ۼ�" << sell_1 << endl;
			int initialization_2 = random(500);//�������������µ�����
			cout << endl;
			hei = 0, hua = 0, bai = 0;
			for (int i = 0; i <= initialization_2; i++)
			{
				Pig* t = new Pig(0, random(3) + 1, 0, ((double)random(300) + 200.0) / 10.0, false);//��ʼ�����̵���Ϣ
				if (t->colour == 1) hei++;
				if (t->colour == 2) hua++;
				if (t->colour == 3) bai++;
				if (putin(t) == false) break;    //ִ���жϺ���putin���������̷���
			}
			
			s_save.open("buyrecord.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ������� 
			s_save << "���ι���:" << hei << "ͷ���� " << hua << "ͷС���� " << bai << "ͷ��׻���" << endl;
			s_save.close();//ÿ�δ浵֮��Ҫ�ر� 

			
			cout << "�������̺����Ȧ�����������" << endl;
			for (int i = 0; i <= 99; i++) cout << numbers[i] << " ";
			cout << endl << endl;
		}
		
	}
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	//���浱ǰ��������Ϣ
	for (int i = 0; i <= 99; i++)
	{
		s.open("pig_information.txt", ofstream::app);//ofstream����app��Ϊ�˷�ֹ��������ݸ���ǰ������� 
		s << "��" << i << "����������Ϣ��" << endl;
		for (Pig* t = pigfarm[i]; t != nullptr; t = t->next)
		{
			s << "��ı����:" << t->number << "	" << "���������" << t->weight << "	" << "�������ʱ����" << t->life << "��" << endl;
		}
		s.close();//ÿ�δ浵֮��Ҫ�ر�
		
	}


	//��ѯ�ض���Ȧ��������������
	Pig* t;
	int x;
	int a[3] = { 0 };
	cout << endl << endl << "������Ҫ��ѯ����Ȧ���:";
	cin >> x;                                    //xΪ��Ҫ��ѯ����Ȧ���
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	t = pigfarm[x];
	for (; t != nullptr; t = t->next)
	{
		if (t->colour == 1) a[0]++;
		if (t->colour == 2) a[1]++;
		if (t->colour == 3) a[2]++;
	}
	t = nullptr;
	cout << "��" << x << "����Ȧ���������  ���� : " << a[0] << " ���� ��" << a[1] << " ��׻��� : " << a[2];

	// ��ѯ��ǰĳһ����Ȧ��ĳͷ������ 
	cout << endl << "������Ҫ��ѯ����ı�ţ�";
	int no_pig;
	cin >> no_pig;
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	t = pigfarm[x];
	if (no_pig > numbers[x] || no_pig <= 0) cout << "��Ȧ��û����ͷ��" << endl << endl;
	else
	{
		int flag(1);
		for (; t != nullptr; t = t->next)
		{
			if (flag == no_pig)
			{
				if (t->colour == 1) cout << "�������Ϊ����" << endl << endl;
				else if (t->colour == 2) cout << "�������Ϊ����" << endl << endl;
				else if (t->colour == 3) cout << "�������Ϊ��׻���" << endl << endl;
				cout << "�������ʱ��Ϊ" << t->life << "��" << endl << endl;
				cout << "�������Ϊ" << t->weight << "ǧ��" << endl << endl;
			}
			flag++;
		}
		t = nullptr;
	}
	
	//ͳ�Ƶ�ǰ��Ȧÿ������������������أ�����ʱ��ֲ����
	int number_pigs[3] = { 0 };
	//number_pigs[0]Ϊ����������number_pigs[1]ΪС����������number_pigs[2]Ϊ��׻�������
	int sum_pigs = 0;
	double weight_pigs[3] = { 0 };
	//weight_pigs[0]Ϊ�������أ�weight_pigs[1]ΪС����weight_pigs[2]Ϊ��׻�������
	int life_pigs1[3] = { 0 };				//0~90���������
	int life_pigs3[3] = { 0 };				//90���ϵ������� 
	//����100����Ȧ��ͳ��
	for (int i = 0; i <= 99; i++)
	{
		t = pigfarm[i];
		for (; t != nullptr; t = t->next)
		{
			if (t->colour == 1)
			{
				number_pigs[0]++;
				weight_pigs[0] += t->weight;
				if (t->life < 90) life_pigs1[0]++;
				else if (t->life >= 90) life_pigs3[0]++;
			}
			else if (t->colour == 2)
			{
				number_pigs[1]++;
				weight_pigs[1] += t->weight;
				if (t->life < 90) life_pigs1[1]++;
				else if (t->life >= 90) life_pigs3[1]++;
			}
			else if (t->colour == 3)
			{
				number_pigs[2]++;
				weight_pigs[2] += t->weight;
				if (t->life < 90) life_pigs1[2]++;
				else if (t->life >= 90) life_pigs3[2]++;
			}
		}
	}
	t = nullptr;
	sum_pigs = number_pigs[0] + number_pigs[1] + number_pigs[2];
	cout << "�����������Ϊ��" << number_pigs[0] << "ͷ" << endl << endl;
	cout << "�����������Ϊ��" << number_pigs[1] << "ͷ" << endl << endl;
	cout << "��׻����������Ϊ��" << number_pigs[2] << "ͷ" << endl << endl;
	cout << "�����������Ϊ��" << weight_pigs[0] << "kg" << endl << endl;
	cout << " �����������Ϊ��" << weight_pigs[1] << "kg" << endl << endl;
	cout << "��׻����������Ϊ��" << weight_pigs[2] << "kg" << endl << endl;
	cout << "��������ʱ��0~90���У�" << life_pigs1[0] << "ͷ" << endl << endl;
	cout << "��������ʱ��90�������У�" << life_pigs3[0] << "ͷ" << endl << endl;
	cout << "��������ʱ��0~90���У�" << life_pigs1[1] << "ͷ" << endl << endl;
	cout << "��������ʱ��90�������У�" << life_pigs3[1] << "ͷ" << endl << endl;
	cout << "��׻���������ʱ��0~90���У�" << life_pigs1[2] << "ͷ" << endl << endl;
	cout << "��׻�������ʱ��90�������У�" << life_pigs3[2] << "ͷ" << endl << endl;

	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;

	cout << "�Ƿ��������ģ��,���Ҫ������1����Ҫ������0" << endl;
	int control1;
	cin >> control1;
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	if (control1 == true)
	{
		//ģ������ĳһֻ���������
		bool pigfarm_sick[100];
		memset(pigfarm_sick, false, sizeof(bool) * 100);	//bool����ÿ������false����ʾ���ڻ�û�������� 
		int randill_1 = random(100);
		while (numbers[randill_1] == 0) randill_1 = random(100);
		int randill_2 = random(numbers[randill_1]);
		//�������һ����������Ȧ,���Ϊrandill_1,�������һ������������������Ȧ�е�randill_2ͷ��
		pigfarm_sick[randill_1] = true;
		int flag_sick = 0;
		int sum_sickpigs = 0;
		//���Ϊrandill_2�ŵ����Ⱦ������ 
		for (t = pigfarm[randill_1]; t != nullptr; t = t->next)
		{
			if (randill_2 == 0)
			{
				t->sick = true;
				sum_sickpigs++;
				break;
			}
			else
			{
				t = t->next;
				flag_sick++;
				if (flag_sick == randill_2)
				{
					t->sick = true;
					sum_sickpigs++;
					break;
				}
			}
		}
		t = nullptr;
		//��ʼ��������
		int sickfarm_no[100];//���ڼ�¼�������ߴ��ڵ���Ȧ�ı�ţ�����֮����� 
		memset(sickfarm_no, -1, sizeof(int) * 100);
		sickfarm_no[0] = randill_1;//randill_1�Ѿ�����������
		int sickdays = 0;
		sum_sickpigs = 10;
		while (sum_sickpigs < sum_pigs)
		{
			sickdays++;
			//����sickfarm_no��ֱ����Ȧ�����е������������� 
			for (int i = 0; sickfarm_no[i] != -1; i++)
			{
				//����sickfarm_no[i]����Ȧ��50%��������
				for (t = pigfarm[sickfarm_no[i]]; t != nullptr; t = t->next)
				{
					if (t->sick == false && random(2) == 1)
					{
						t->sick = true;
						sum_sickpigs++;
					}
				}
				//��sickfarm_no[i]==0,��ֻ��Ⱦ2����Ȧ
				if (sickfarm_no[i] == 0)
				{
					for (t = pigfarm[1]; t != nullptr; t = t->next)
					{
						if (t->sick == false && (random(20) == 1 || random(20) == 2 || random(20) == 3))
						{
							t->sick = true;
							sum_sickpigs++;
							if (pigfarm_sick[1] == false)
							{
								pigfarm_sick[1] = true;
								int j;
								for (j = 0; sickfarm_no[j] != -1; j++);
								if (j <= 99) sickfarm_no[j] = 1;
							}
						}
					}
				}
				//��sickfarm_no[i]==99,��ֻ��Ⱦ98����Ȧ
				else if (sickfarm_no[i] == 99)
				{
					for (t = pigfarm[98]; t != nullptr; t = t->next)
					{
						if (t->sick == false && (random(20) == 1 || random(20) == 2 || random(20) == 3))
						{
							t->sick = true;
							sum_sickpigs++;
							if (pigfarm_sick[98] == false)
							{
								pigfarm_sick[98] = true;
								int j;
								for (j = 0; sickfarm_no[j] != -1; j++);
								if (j <= 99) sickfarm_no[j] = 98;
							}
						}
					}
				}
				//sickfarm_no[i]==1~98����Ⱦ������Ȧ
				else
				{
					for (t = pigfarm[sickfarm_no[i] - 1]; t != nullptr; t = t->next)
					{
						if (t->sick == false && (random(20) == 1 || random(20) == 2 || random(20) == 3))
						{
							t->sick = true;
							sum_sickpigs++;
							if (pigfarm_sick[sickfarm_no[i] - 1] == false)
							{
								pigfarm_sick[sickfarm_no[i] - 1] = true;
								int j;
								for (j = 0; sickfarm_no[j] != -1; j++);
								if (j <= 99) sickfarm_no[j] = sickfarm_no[i] - 1;
							}
						}
					}
					for (t = pigfarm[sickfarm_no[i] + 1]; t != nullptr; t = t->next)
					{
						if (t->sick == false && (random(20) == 1 || random(20) == 2 || random(20) == 3))
						{
							t->sick = true;
							sum_sickpigs++;
							if (pigfarm_sick[sickfarm_no[i] + 1] == false)
							{
								pigfarm_sick[sickfarm_no[i] + 1] = true;
								int j;
								for (j = 0; sickfarm_no[j] != -1; j++);
								if (j <= 99) sickfarm_no[j] = sickfarm_no[i] + 1;
							}
						}
					}
				}
			}
		}
		cout << "�������ȡ�����ʩ,�ܼ�" << sickdays << "����ȫ������" << endl << endl;
	}
	if (control1 == 1)
	{
		cout << "�Ƿ��ȡ�����ʩ�������ȡ����1������ȡ����0" << endl << endl;
		int control2;
		cin >> control2;
		for (int i = 0; i <= 80; i++)cout << "#";
		cout << endl << endl;
		if (control2 == 1)
		{
			//�����ȡ�����ʩ
			int randill_isolate1 = random(100);
			Pig* previous;
			while (numbers[randill_isolate1] == 0) randill_isolate1 = random(100);//randill_isolate1����Ȧ�ı��
			int randill_isolate2 = random(numbers[randill_isolate1]);//randill_isolate2�ǵ�����������ñ��
			int flag = 0;
			t = pigfarm[randill_isolate1];
			previous = t;
			//��Ȧ��ֻ��һͷ����ͷ��������� ������ɾ��
			if (t->next == nullptr)
			{
				delete t;
				t = nullptr;
				numbers[randill_isolate1] = 0;
			}
			else//��Ȧ������ͷ�����ϵ���
			{
				//��һͷ������
				if (randill_isolate2 == 0)
				{
					pigfarm[randill_isolate1] = t->next;
					delete t;
					t = nullptr;
					numbers[randill_isolate1]--;
				}
				//��randill_isolate2ͷ������
				else if (randill_isolate2 < numbers[randill_isolate1])
				{
					if (flag == randill_isolate2)
					{
						previous->next = t->next;
						delete t;
						t = previous->next;
					}
					flag++;
					previous = t;
					t = t->next;
				}
			}
		}
	}
	cout << "�Ƿ�ɾ��txt�ļ������ģ��Ĵ浵����������1������������2" << endl << endl;
	int control5;
	cin >> control5;
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	if (control5 == 1)
	{
		s_save.open("buyrecord.txt");
		s_save << " ";//����֮ǰ�浵
		t_save.open("sellrecord.txt");
		t_save << " ";//����֮ǰ�浵
		s.open("pig_information.txt");
		s << " ";//����֮ǰ�浵
	}
	cout << "���ֵ�������������ټ�~" << endl << endl;
	for (int i = 0; i <= 80; i++)cout << "#";
	cout << endl << endl;
	return 0;
}
