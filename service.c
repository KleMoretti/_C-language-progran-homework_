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

	//C �⺯�� FILE *fopen(const char *filename, const char *mode) ʹ�ø�����ģʽ mode �� filename ��ָ����ļ���
	//filename -- �ַ�������ʾҪ�򿪵��ļ����ơ�  mode -- �ַ�������ʾ�ļ��ķ���ģʽ
	//  ʹ�÷�ʽ	            ���� 	             ����ļ�������
	//"rb+"����д��	 Ϊ�˶���д����һ���������ļ�	     ����
	if ((fp = fopen(pPath, "rb+")) == NULL)
	{
		return 0;
	}
	while (!feof(fp))
		//feof()�Ǽ�����ϵ��ļ��������ĺ���������ļ��������򷵻ط�0ֵ�����򷵻�0
	{
		pcardnode = malloc(sizeof(Cardnode));
		if (fread(&(pcardnode->data), sizeof(Card), 1, fp) != 0)
			//C �⺯�� size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
			// �Ӹ����� stream ��ȡ���ݵ� ptr ��ָ��������С�
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
			printf("����\t��ֵ�˷�ʱ��\t״̬\t��ֵ�˷ѽ��\tɾ����־\n");
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

	//C �⺯�� size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
	//�� ptr ��ָ��������е�����д�뵽������ stream �С�
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

	//C �⺯�� int fseek(FILE *stream, long int offset, int whence)
	//������ stream ���ļ�λ��Ϊ������ƫ�� offset������ offset ��ζ�ŴӸ����� whence λ�ò��ҵ��ֽ�����
	fseek(fp, sizeof(Card) * nCardIndex, SEEK_SET);
	fwrite((char*)pcard, sizeof(Card), 1, fp);

	fclose(fp);
	return 1;
}

//�жϿ��ļƷ���Ϣ�Ƿ���ڣ������ؼƷ��������еĽڵ�
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

	//��������ϻ���������Ϣ
	pCard->state = Using;
	pCard->usenum++;        //ʹ�ô���+1
	pCard->lasttime = time(NULL);

	updateCard(pCard, CardPath, nCardIndex);

	strcpy(billing.aCardname, strNo);
	billing.tStart = time(NULL);
	billing.tEnd = 0;
	billing.nstatus = NosettleMent;
	billing.damount = 0;

	//�Ƚ��Ʒ���Ϣ���浽�ļ���
	saveBilling(&billing, BillingPath);

	//�ڼƷ�����������һ���Ʒ���Ϣ
	Billingnode* pBillingnode = malloc(sizeof(Billingnode));
	if (pBillingnode == NULL)return 0;
	//ppheadBillingnode->nextlink->nextlink���ܲ�����
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
		printf("�Ʒ���ϢΪ�գ�\n");
		return Unusing;
	}

	double ddamount = getAmount(pBilling->tStart);

	float fbalance = pCard->balance - (float)ddamount;
	if (fbalance < 0) {
		return NoEnoughMoney;
	}

	//���¿���Ϣ
	pCard->balance = fbalance;
	pCard->state = Unusing;
	pCard->lasttime = time(NULL);

	//�����ļ��еĿ���Ϣ
	updateCard(pCard, CardPath, nCardIndex);

	//���¼Ʒ���Ϣ
	pBilling->damount = ddamount;
	pBilling->nstatus = YessettleMent;
	pBilling->tEnd = time(NULL);
	//�����ļ��еļƷ���Ϣ
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

	//��ѯ��ֵ��
	pCard = cardIsExist(pName, &Index, pCardNodehead);
	if (pCard == NULL)
	{
		printf("û�в�ѯ���ÿ���Ϣ!\n");
		return Invalid;
	}
	if (pCard->deletion == 1)return Invalid;
	if (pCard->state == Invalid)return Invalid;
	if (pCard->state == Unusing || pCard->state == Using)
	{
		printf("�������ֵ���:\n");
		(void)scanf("%lf", &AddMoney);
		if (AddMoney <= 0) {
			printf("��ֵ���Ǹ��������³�ֵ��\n");
			return Invalid;;
		}
		fflush(stdin);
		pCard->balance += AddMoney;
	}
	else
	{
		return Invalid;
	}
	//Ϊ��ֵ��Ϣ��ֵ
	strcpy(pMoney->aCardname, pName);
	pMoney->dMoney = AddMoney;
	pMoney->deletion = 0;
	pMoney->nstatus = 0;
	pMoney->Etime = time(NULL);

	//Ϊ��ʾ��ֵ��Ϣ��ֵ
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->dBalance = pCard->balance;
	pMoneyInfo->dMoney = AddMoney;

	//�����ļ��е���Ϣ
	if (!updateCard(pCard, CardPath, Index))
	{
		printf("�����ļ���Ϣʧ��!");
		return 0;
	}

	//�����ֵ��Ϣ
	if (!saveMoney(pMoney, MoneyPath))
	{
		printf("�����ֵ��Ϣʧ��!");
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
	//��ѯ�˷ѿ�
	pCard = cardIsExist(pName, &Index, pcardnodehead);
	if (pCard == NULL)
	{
		printf("û�в�ѯ���ÿ���Ϣ!\n");
		return 0;
	}
	if (pCard->deletion == 0)
	{
		if (pCard->state == Unusing)
		{
			if (pCard->balance > 0)
			{
				printf("������Ϊ:%0.1f\n", pCard->balance);
				do
				{
					if (nFalg != 0)
					{
						printf("�����������!����������!\n");
					}
					printf("��������Ҫ�˷ѵĽ��:");
					(void)scanf("%f", &money);
					fflush(stdin);
					nFalg++;
				} while (money > pCard->balance || money <= 0);

				pCard->balance -= money;
			}
			else if (pCard->balance <= 0)
			{
				printf("�ÿ����С��0�������˷�!\n");
				return 0;
			}
		}
		else if (pCard->state == Using)
		{
			printf("�ÿ������ϻ�!\n");
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		printf("�ÿ��Ѿ�������!\n");
		return 0;
	}
	//Ϊ��ֵ��Ϣ��ֵ
	strcpy(pMoney->aCardname, pName);
	pMoney->dMoney = -money;
	pMoney->deletion = 0;
	pMoney->nstatus = 1;
	pMoney->Etime = time(NULL);
	//Ϊ��ʾ�˷���Ϣ��ֵ
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->dBalance = pCard->balance;
	pMoneyInfo->dMoney = money;
	//�����ļ��е���Ϣ
	if (updateCard(pCard, CardPath, Index) != 1)
	{
		printf("�����ļ���Ϣʧ��!");
		return 0;
	}
	//�����˷���Ϣ
	if (saveMoney(pMoney, MoneyPath) != 1)
	{
		printf("�����˷���Ϣʧ��!");
		return 0;
	}
	free(pMoney);
	return 1;
}