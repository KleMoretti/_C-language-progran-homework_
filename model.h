#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<time.h>
typedef struct CARD {
	char cname[6];      //卡号
	char cpassword[10]; //卡密码
	int state;          //卡状态   0-未上机  1-正在上机 2-已注销
	double useofmoney;  //使用金额
	int usenum;         //使用次数
	double balance;     //余额
	time_t start;       //上机时间
	time_t end;         //下机时间
	time_t lasttime;    //最后使用时间
	int deletion;       //删除标记  0-未注销  1-注销
}Card;

typedef struct Cardnode {
	Card data;
	struct Cardnode* linkcard;
}Cardnode;

//计费结构体
typedef struct billing {
	char aCardname[8];   //卡号
	time_t tStart;        //上机时间
	time_t tEnd;          //下机时间
	double damount;       //消费金额
	int nstatus;          //消费状态  0-未结算  1-已结算
	int deletion;         //删除标志  0-未删除  1-已删除
}Billing;

typedef struct BillingNode {
	Billing data;
	struct BillingNode* nextlink;
}Billingnode;

//上机
typedef struct Logoinfo {
	char aCardName[8];
	time_t tLogon;
	float fBanlance;
}LogonInfo;

//下机
typedef struct Settleinformation {
	char aCardname[8];
	time_t tStart;
	time_t tEnd;
	double damount;
	double dbanlance;
}Settleinfo;

//充值与退费
typedef struct Money {
	char aCardname[8];    //卡号
	time_t Etime;         //最后是哦也能够时间
	double dMoney;        //充值退费金额
	int nstatus;          //消费状态  0-未结算  1-已结算
	int deletion;         //删除标志  0-未删除  1-已删除
}Money;

typedef struct MoneyInfo {
	char aCardName[8];    //卡号
	double dMoney;        //充值退费金额
	double dBalance;      //余额
}MoneyInfo;