#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<time.h>
typedef struct CARD {
	char cname[6];      //����
	char cpassword[10]; //������
	int state;          //��״̬   0-δ�ϻ�  1-�����ϻ� 2-��ע��
	double useofmoney;  //ʹ�ý��
	int usenum;         //ʹ�ô���
	double balance;     //���
	time_t start;       //�ϻ�ʱ��
	time_t end;         //�»�ʱ��
	time_t lasttime;    //���ʹ��ʱ��
	int deletion;       //ɾ�����  0-δע��  1-ע��
}Card;

typedef struct Cardnode {
	Card data;
	struct Cardnode* linkcard;
}Cardnode;

//�Ʒѽṹ��
typedef struct billing {
	char aCardname[8];   //����
	time_t tStart;        //�ϻ�ʱ��
	time_t tEnd;          //�»�ʱ��
	double damount;       //���ѽ��
	int nstatus;          //����״̬  0-δ����  1-�ѽ���
	int deletion;         //ɾ����־  0-δɾ��  1-��ɾ��
}Billing;

typedef struct BillingNode {
	Billing data;
	struct BillingNode* nextlink;
}Billingnode;

//�ϻ�
typedef struct Logoinfo {
	char aCardName[8];
	time_t tLogon;
	float fBanlance;
}LogonInfo;

//�»�
typedef struct Settleinformation {
	char aCardname[8];
	time_t tStart;
	time_t tEnd;
	double damount;
	double dbanlance;
}Settleinfo;

//��ֵ���˷�
typedef struct Money {
	char aCardname[8];    //����
	time_t Etime;         //�����ŶҲ�ܹ�ʱ��
	double dMoney;        //��ֵ�˷ѽ��
	int nstatus;          //����״̬  0-δ����  1-�ѽ���
	int deletion;         //ɾ����־  0-δɾ��  1-��ɾ��
}Money;

typedef struct MoneyInfo {
	char aCardName[8];    //����
	double dMoney;        //��ֵ�˷ѽ��
	double dBalance;      //���
}MoneyInfo;