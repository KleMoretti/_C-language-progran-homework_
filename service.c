#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<stdio.h>
#include<string.h>
#include"model.h"
#include<stdlib.h>
#include"global.h"
#include"tool.h"

int readCard(Cardnode** pcardnodehead, char* pPath) {
	Cardnode* pcardnode = NULL;
	Cardnode* pcardnodetail = NULL;
	*pcardnodehead = NULL;

	FILE* fp = NULL;

	//C 库函数 FILE *fopen(const char *filename, const char *mode) 使用给定的模式 mode 打开 filename 所指向的文件。
	//filename -- 字符串，表示要打开的文件名称。  mode -- 字符串，表示文件的访问模式
	//  使用方式	            作用 	             如果文件不存在
	//"rb+"（读写）	 为了读和写，打开一个二进制文件	     出错
	if ((fp = fopen(pPath, "rb+")) == NULL)
	{
		return 0;
	}
	while (!feof(fp))
		//feof()是检测流上的文件结束符的函数，如果文件结束，则返回非0值，否则返回0
	{
		pcardnode = malloc(sizeof(Cardnode));
		if (fread(&(pcardnode->data), sizeof(Card), 1, fp) != 0)
			//C 库函数 size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
			// 从给定流 stream 读取数据到 ptr 所指向的数组中。
		{
			if (*pcardnodehead == NULL) {
				*pcardnodehead = pcardnode;
				pcardnodetail = pcardnode;
				pcardnodetail->linkcard = NULL;
			}
			else {
				pcardnodetail->linkcard = pcardnode;
				pcardnodetail = pcardnodetail->linkcard;
				pcardnodetail->linkcard = NULL;
			}
		}
	}
	fclose(fp);
	return 1;
}

int readBilling(Billingnode** ppheadBillingnode, char* pPath) {
	Billingnode* pBillingnode = NULL, * ptailBillingnode = NULL;
	*ppheadBillingnode = NULL;

	FILE* fp = NULL;
	if ((fp = fopen(pPath, "ab+")) == NULL) {
		return 0;
	}
	while (!feof(fp)) {
		pBillingnode = malloc(sizeof(Billingnode));
		if (pBillingnode == NULL)return 0;
		if (fread(&(pBillingnode->data), sizeof(Billing), 1, fp) != 0) {
			if (*ppheadBillingnode == NULL) {
				*ppheadBillingnode = pBillingnode;
				ptailBillingnode = pBillingnode;
				ptailBillingnode->nextlink = NULL;
			}
			else {
				ptailBillingnode->nextlink = pBillingnode;
				ptailBillingnode = ptailBillingnode->nextlink;
				ptailBillingnode->nextlink = NULL;
			}
		}
	}
	fclose(fp);
	return 1;
}

int readMoney(char* pPath) {
	FILE* fp = NULL;
	char moneytime[TimeLength] = { 0 };
	Money* pMoney = (Money*)malloc(sizeof(Money));
	if ((fp = fopen(pPath, "ab+")) == NULL)
		return 0;
	while (!feof(fp)) {
		if (fread(pMoney, sizeof(Money), 1, fp) != 0) {
			timetostring(pMoney->Etime, moneytime);
			printf("卡号\t充值退费时间\t状态\t充值退费金额\t删除标志\n");
			printf("%s\t%s\t%d\t%0.1f\t%d\t\n", pMoney->aCardname, moneytime, pMoney->nstatus, pMoney->dMoney, pMoney->deletion);
		}
	}

	fclose(fp);
	return 1;
}

int saveCard(Card* pcardhead, char* pPath) {
	FILE* fp = NULL;
	if ((fp = fopen(pPath, "ab+")) == NULL) {
		return 0;
	}

	//C 库函数 size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
	//把 ptr 所指向的数组中的数据写入到给定流 stream 中。
	fwrite(pcardhead, sizeof(Card), 1, fp);

	fclose(fp);
	return 1;
}

int saveBilling(Billing* pBilling, char* pPath) {
	FILE* fp = NULL;
	if ((fp = fopen(pPath, "ab+")) == NULL) {
		return 0;
	}
	fwrite(pBilling, sizeof(Billing), 1, fp);

	fclose(fp);
	return 1;
}

int saveMoney(Money* pMoney, char* pPath) {
	FILE* fp = NULL;
	if ((fp = fopen(pPath, "ab+")) == NULL) {
		return 0;
	}
	fwrite(pMoney, sizeof(Money), 1, fp);

	fclose(fp);
	return 1;
}

void releaseCard(Cardnode* pcardnodehead) {
	Cardnode* pcardnode = pcardnodehead;
	while (pcardnodehead != NULL) {
		pcardnode = pcardnodehead;
		pcardnodehead = pcardnodehead->linkcard;
		free(pcardnode);
	}
	pcardnodehead = NULL;
	return;
}

void releaseBilling(Billingnode* pheadBillingnode) {
	Billingnode* pBillingnode = pheadBillingnode;
	while (pheadBillingnode != NULL) {
		pBillingnode = pheadBillingnode;
		pheadBillingnode = pheadBillingnode->nextlink;
		free(pBillingnode);
	}
	pheadBillingnode = NULL;
	return;
}

/*void releaseMoney(MoneyInfo* psheadMoney) {
	MoneyInfo* pMoneyInfo = psheadMoney;
	while (psheadMoney != NULL) {
		pMoneyInfo = psheadMoney;
		psheadMoney = psheadMoney->nextlink;
		free(pMoneyInfo);
	}
	psheadMoney = NULL;
	return;
}*/

Card* cardIsExist(char* strNo, int* pCardIndex, Cardnode* const pCardNodehead) {
	Cardnode* pcardnode = pCardNodehead;
	*pCardIndex = 0;
	while (pcardnode != NULL) {
		if (strcmp(pcardnode->data.cname, strNo) == 0) {
			return &(pcardnode->data);
		}
		pcardnode = pcardnode->linkcard;
		(*pCardIndex)++;
	}
	return NULL;
}

int updateCard(const Card* pcard, char* pPath, int nCardIndex) {
	FILE* fp = NULL;
	if ((fp = fopen(pPath, "rb+")) == NULL)
		return 0;

	//C 库函数 int fseek(FILE *stream, long int offset, int whence)
	//设置流 stream 的文件位置为给定的偏移 offset，参数 offset 意味着从给定的 whence 位置查找的字节数。
	fseek(fp, sizeof(Card) * nCardIndex, SEEK_SET);
	fwrite((char*)pcard, sizeof(Card), 1, fp);

	fclose(fp);
	return 1;
}

//判断卡的计费信息是否存在，并返回计费在链表中的节点
Billing* billingIsExist(char* strNo, int* pBillingIndex, Billingnode* const pheadBillingnode) {
	Billingnode* pBillingnode = pheadBillingnode;

	*pBillingIndex = 0;
	while (pBillingnode != NULL) {
		if (strcmp(pBillingnode->data.aCardname, strNo) == 0 && pBillingnode->data.nstatus == NosettleMent) {
			return &(pBillingnode->data);
		}
		pBillingnode = pBillingnode->nextlink;
		(*pBillingIndex)++;
	}
	return NULL;
}

int updateBilling(const Billing* pbilling, char* pPath, int nBillingIndex) {
	FILE* fp = NULL;
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		return 0;
	}

	fseek(fp, sizeof(Billing) * nBillingIndex, SEEK_SET);

	fwrite((char*)pbilling, sizeof(Billing), 1, fp);
	fclose(fp);

	return 1;
}

int updateMoney(const Money* pMoney, char* pPath, int nMoneyIndex) {
	FILE* fp = NULL;
	Money* nMoney = (Money*)malloc(sizeof(Money));
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		return 0;
	}
	fseek(fp, sizeof(Money) * nMoneyIndex, SEEK_SET);

	fwrite((char*)pMoney, sizeof(Money), 1, fp);
	fclose(fp);
	return 1;
}

int logon(char* strNo, char* strpwd, LogonInfo* pInfo, Cardnode* pcardnodehead, Billingnode** ppheadBillingnode) {
	Billing billing;
	int nCardIndex = 0;
	Card* pCard = cardIsExist(strNo, &nCardIndex, pcardnodehead);
	if (pCard == NULL)
		return NotfindCard;
	if (strcmp(pCard->cpassword, strpwd) != 0)
		return NoMatch;
	if (pCard->state == Using)
		return Using;
	if (pCard->state == Invalid)
		return Invalid;
	if (pCard->balance <= 0)
		return NoEnoughMoney;

	//如果可以上机，更新信息
	pCard->state = Using;
	pCard->usenum++;        //使用次数+1
	pCard->lasttime = time(NULL);

	updateCard(pCard, CardPath, nCardIndex);

	strcpy(billing.aCardname, strNo);
	billing.tStart = time(NULL);
	billing.tEnd = 0;
	billing.nstatus = NosettleMent;
	billing.damount = 0;

	//先将计费信息保存到文件中
	saveBilling(&billing, BillingPath);

	//在计费链表中增加一条计费信息
	Billingnode* pBillingnode = malloc(sizeof(Billingnode));
	if (pBillingnode == NULL)return 0;
	//ppheadBillingnode->nextlink->nextlink可能不存在
	Billingnode* ppBillingtail = malloc(sizeof(Billingnode));
	if (ppBillingtail == NULL)return 0;

	pBillingnode->data = billing;

	Billingnode* pBillingnodetail = *ppheadBillingnode;

	if (*ppheadBillingnode == NULL) {
		*ppheadBillingnode = pBillingnode;
		pBillingnodetail = pBillingnode;
	}
	else {
		while (pBillingnodetail->nextlink != NULL)
			pBillingnodetail = pBillingnodetail->nextlink;
		pBillingnodetail->nextlink = pBillingnode;
		pBillingnodetail = pBillingnode;
	}
	pBillingnodetail->nextlink = NULL;

	strcpy(pInfo->aCardName, strNo);
	pInfo->fBanlance = pCard->balance;
	pInfo->tLogon = billing.tStart;
	return Success;
}

int settle(char* strNo, char* strpwd, Settleinfo* pInfo, Cardnode* const pcardnodehead, Billingnode* const pheadBillingnode) {
	int nCardIndex = 0;
	Card* pCard = cardIsExist(strNo, &nCardIndex, pcardnodehead);
	if (pCard == NULL)
		return NotfindCard;
	if (strcmp(pCard->cpassword, strpwd) != 0)
		return NoMatch;
	if (pCard->state != Using)
		return Unusing;
	if (pCard->state == Invalid)
		return Invalid;

	int nBillingIndex = 0;

	Billing* pBilling = billingIsExist(strNo, &nBillingIndex, pheadBillingnode);
	if (pBilling == NULL) {
		printf("计费信息为空！\n");
		return Unusing;
	}

	double ddamount = getAmount(pBilling->tStart);

	float fbalance = pCard->balance - (float)ddamount;
	if (fbalance < 0) {
		return NoEnoughMoney;
	}

	//更新卡信息
	pCard->balance = fbalance;
	pCard->state = Unusing;
	pCard->lasttime = time(NULL);

	//更新文件中的卡信息
	updateCard(pCard, CardPath, nCardIndex);

	//更新计费信息
	pBilling->damount = ddamount;
	pBilling->nstatus = YessettleMent;
	pBilling->tEnd = time(NULL);
	//更新文件中的计费信息
	updateBilling(pBilling, BillingPath, nBillingIndex);

	strcpy(pInfo->aCardname, strNo);
	pInfo->damount = ddamount;
	pInfo->dbanlance = fbalance;
	pInfo->tStart = pBilling->tStart;
	pInfo->tEnd = time(NULL);

	return Success;
}

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo, Cardnode* const pCardNodehead) {
	int Index = 0;
	double AddMoney = 0;
	Money* pMoney = NULL;
	Card* pCard = NULL;
	pMoney = (Money*)malloc(sizeof(Money));

	//查询充值卡
	pCard = cardIsExist(pName, &Index, pCardNodehead);
	if (pCard == NULL)
	{
		printf("没有查询到该卡信息!\n");
		return Invalid;
	}
	if (pCard->deletion == 1)return Invalid;
	if (pCard->state == Invalid)return Invalid;
	if (pCard->state == Unusing || pCard->state == Using)
	{
		printf("请输入充值金额:\n");
		(void)scanf("%lf", &AddMoney);
		if (AddMoney <= 0) {
			printf("充值金额非负，请重新充值！\n");
			return Invalid;;
		}
		fflush(stdin);
		pCard->balance += AddMoney;
	}
	else
	{
		return Invalid;
	}
	//为充值信息赋值
	strcpy(pMoney->aCardname, pName);
	pMoney->dMoney = AddMoney;
	pMoney->deletion = 0;
	pMoney->nstatus = 0;
	pMoney->Etime = time(NULL);

	//为显示充值信息赋值
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->dBalance = pCard->balance;
	pMoneyInfo->dMoney = AddMoney;

	//更新文件中的信息
	if (!updateCard(pCard, CardPath, Index))
	{
		printf("更新文件信息失败!");
		return 0;
	}

	//保存充值信息
	if (!saveMoney(pMoney, MoneyPath))
	{
		printf("保存充值信息失败!");
		return 0;
	}
	free(pMoney);
	return Success;
}

int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo, Cardnode* const pcardnodehead) {
	Card* pCard = NULL;
	int Index = 0;
	float money = 0;
	Money* pMoney = NULL;
	int nFalg = 0;
	int n = 1;
	pMoney = (Money*)malloc(sizeof(Money));
	//查询退费卡
	pCard = cardIsExist(pName, &Index, pcardnodehead);
	if (pCard == NULL)
	{
		printf("没有查询到该卡信息!\n");
		return 0;
	}
	if (pCard->deletion == 0)
	{
		if (pCard->state == Unusing)
		{
			if (pCard->balance > 0)
			{
				printf("你的余额为:%0.1f\n", pCard->balance);
				do
				{
					if (nFalg != 0)
					{
						printf("你输入金额错误!请重新输入!\n");
					}
					printf("输入你想要退费的金额:");
					(void)scanf("%f", &money);
					fflush(stdin);
					nFalg++;
				} while (money > pCard->balance || money <= 0);

				pCard->balance -= money;
			}
			else if (pCard->balance <= 0)
			{
				printf("该卡余额小于0，不能退费!\n");
				return 0;
			}
		}
		else if (pCard->state == Using)
		{
			printf("该卡正在上机!\n");
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		printf("该卡已经不存在!\n");
		return 0;
	}
	//为充值信息赋值
	strcpy(pMoney->aCardname, pName);
	pMoney->dMoney = -money;
	pMoney->deletion = 0;
	pMoney->nstatus = 1;
	pMoney->Etime = time(NULL);
	//为显示退费信息赋值
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->dBalance = pCard->balance;
	pMoneyInfo->dMoney = money;
	//更新文件中的信息
	if (updateCard(pCard, CardPath, Index) != 1)
	{
		printf("更新文件信息失败!");
		return 0;
	}
	//保存退费信息
	if (saveMoney(pMoney, MoneyPath) != 1)
	{
		printf("保存退费信息失败!");
		return 0;
	}
	free(pMoney);
	return 1;
}