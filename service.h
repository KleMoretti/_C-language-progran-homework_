#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include"model.h"

int readCard(Cardnode** pcardnodehead, char* pPath);
int readBilling(Billingnode** ppheadBillingnode, char* pPath);
//int readMoney(sMoney** ppheadMoney, char* pPath);

int saveCard(Cardnode* pcardnodehead, char* pPath);
int saveBilling(Billing* pBilling, char* pPath);
//int saveMoney(Money* pMoney, char* pPath);

void releaseCard(Cardnode* pcardnodehead);
void releaseBilling(Billingnode* pheadBillingnode);
//void releaseMoney(sMoney* psheadMoney);

Card* cardIsExist(char* strNo, int* pCardIndex, Cardnode* const pCardNodehead);
Billing* billingIsExist(char* strNo, int* pBillingIndex, Billingnode* const pheadBillingnode);
//Money* MoneyIsExist(char* strNo, int* pMoneyIndex, sMoney* const pheadMoney);

int updateCard(const Card* pcard, char* pPath, int nCardIndex);
int updateBilling(const Billing* pbilling, char* pPath, int nBillingIndex);
//int updateMoney(const Money* pMoney, char* pPath, int nMoneyIndex);

int logon(char* strNo, char* strpwd, LogonInfo* pInfo, Cardnode* pcardnodehead, Billingnode** ppheadBillingnode);
int settle(char* strNo, char* strpwd, Settleinfo* pInfo, Cardnode* const pcardnodehead, Billingnode* const pheadBillingnode);
//int charge(char* strNo, char* strpwd, sMoney* pInfo, Cardnode* pcardnodehead, Money* pheadMoney);
//int discharge(char* strNo, char* strpwd, sMoney* pInfo, Cardnode* pcardnodehead, Money* pheadMoney);

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo, Cardnode* const pCardNodehead);
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo, Cardnode* const pCardNodehead);
