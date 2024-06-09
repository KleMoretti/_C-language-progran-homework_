#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
void outputmenu();
void outputmenu7();
void addcard(Cardnode** pcardnodehead);
void shangji(Cardnode* pCardnodehead, Billingnode** ppheadBillingnode);
void xiaji(Cardnode* pcardnodehead, Billingnode* pheadBillingnode);
//void chongzhi(Cardnode* pcardnodehead, sMoney* pheadsmoney);
void addMoney(Cardnode* const pCardNodehead);
void refundMoney(Cardnode* const pCardNodehead);
void statNewCard(Cardnode* pCardnodehead);
int Islegal(int year, int mon, int day);
int IsLeapyear(int year);
void zhuxiao(Cardnode* const pCardNodehead);
int zhuxiaoCard(char* strNo, char* strSwd, Card* pCard, Cardnode* const pCardNodehead);
int allmoney(char* pPath);
void findzhuxiao(char* pPath);
void tongjibilling(char* pPath);
void jihuo(Cardnode* const pCardNodehead);
int jihuoCard(char* strNo, char* strSwd, Card* pCard, Cardnode* const pCardNodehead);
