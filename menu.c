#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include"model.h"  //�ṹ��
#include"menu.h"
#include"global.h"
#include"tool.h"
#include"service.h"
/*�˵��������*/
void outputmenu() {
	printf("---------�˵�---------\n");
	printf("1.��ӿ�\n");
	printf("2.��ѯ��\n");
	printf("3.�ϻ�\n");
	printf("4.�»�\n");
	printf("5.��ֵ\n");
	printf("6.�˷�\n");
	printf("7.��ѯͳ��\n");
	printf("8.ע����\n");
	printf("9.���\n");
	printf("0.�˳�\n");
	printf("��ѡ��˵���ţ�0~9��:");
}

void outputmenu7() {
	printf("--------��ѯ�˵�---------\n");
	printf("1.��ѯ�ܿ�\n");
	printf("2.��ѯ��ֵ�˷�\n");
	printf("3.ͳ������\n");
	printf("4.��ѯ����\n");
	printf("����-- 0 --���˳�����\n");
	printf("��ѡ��˵���ţ�1~4��:");
}

/*��ӿ�����*/
void addcard(Cardnode** pcardnodehead) {
	char mname[15] = { 0 };
	char mpassword[10] = { 0 };
	Cardnode* pcardnext = (Cardnode*)malloc(sizeof(Cardnode));
	if (pcardnext == NULL)return;
	//�½��ṹ�����¿�������ƽ��ṹ����

	printf("----------��ӿ�-----------\t\n");
	printf("�����뿨�ţ�����Ϊ5����");
	(void)scanf("%6s", mname);

	if (strlen(mname) != 5) {
		printf("���ų��Ȳ�����\n");
		return;
	}
	int nCardIndex = 0;
	if (cardIsExist(mname, &nCardIndex, *pcardnodehead) != NULL)
	{
		printf("���š�%s���Ѿ�����!\n", mname);
		return;
	}

	strcpy(pcardnext->data.cname, mname);

	printf("�����뿨�����루����1-8����");
	(void)scanf("%9s", mpassword);
	fflush(stdin);

	if (strlen(mpassword) >= 8) {
		printf("������������������¿�����\n");
		return;
	}

	strcpy(pcardnext->data.cpassword, mpassword);

	printf("�����뿪����RMB����");
	(void)scanf("%lf", &pcardnext->data.balance);

	if (pcardnext->data.balance < 0) {
		printf("����Ŀ�������С��0��\n");
		return;
	}

	if (pcardnext->data.balance != (int)pcardnext->data.balance) {
		printf("������������\n");
		return;
	}

	pcardnext->data.useofmoney = pcardnext->data.balance;
	pcardnext->data.deletion = 0;
	pcardnext->data.state = Unusing;
	pcardnext->data.usenum = 0;
	pcardnext->data.start = time(NULL);  //��ȡϵͳʱ�䣬��λΪ��;
	pcardnext->data.end = time(NULL);
	pcardnext->data.lasttime = time(NULL);
	//C �⺯�� time_t time(time_t *seconds) �����Լ�Ԫ Epoch��1970-01-01 00:00:00 UTC���𾭹���ʱ�䣬����Ϊ��λ��
	// ��� seconds ��Ϊ�գ��򷵻�ֵҲ�洢�ڱ��� seconds �С�

	struct tm* endoftime;
	struct tm* starttime;
	struct tm* lasttime;
	starttime = localtime(&pcardnext->data.start);
	endoftime = localtime(&pcardnext->data.end);
	lasttime = localtime(&pcardnext->data.lasttime);

	//C �⺯�� struct tm *localtime(const time_t *timer) ʹ�� timer ��ֵ����� tm �ṹ��
	// timer ��ֵ���ֽ�Ϊ tm �ṹ�����ñ���ʱ����ʾ��

	endoftime->tm_year = endoftime->tm_year + 2;//ÿ�ſ���Ч��2��
	pcardnext->data.end = mktime(endoftime);

	Cardnode* pcardnodetail = *pcardnodehead;

	if (*pcardnodehead == NULL) {
		*pcardnodehead = pcardnext;
		pcardnodetail = pcardnext;
		pcardnodetail->linkcard = NULL;
	}
	else
	{
		while (pcardnodetail->linkcard != NULL) {
			pcardnodetail = pcardnodetail->linkcard;
		}

		pcardnodetail->linkcard = pcardnext;
		pcardnodetail = pcardnext;
		pcardnodetail->linkcard = NULL;
	}

	saveCard(&pcardnext->data, CardPath);

	printf("��ӿ�״̬�ɹ�!\n");
}

void shangji(Cardnode* pCardnodehead, Billingnode** ppheadBillingnode) {
	char aName[10] = { 0 };
	char aPassword[10] = { 0 };

	printf("�뱣֤���뿨�Ų�Ҫ����������������!\n");
	printf("�����뿨�ţ�����Ϊ5��:");
	(void)scanf("%6s", aName);
	fflush(stdin);

	if (getSize(aName) != 5) {
		printf("���ų��ȱ���Ϊ5!\n");
		return;
	}
	printf("���������루����Ϊ1~8��:");
	(void)scanf("%9s", aPassword);
	fflush(stdin);

	if (getSize(aPassword) >= 8) {
		printf("��������볤�ȳ������ֵ��\n");
		return;
	}

	printf("---------�ϻ���Ϣ----------\n");

	LogonInfo* pInfo = malloc(sizeof(LogonInfo));
	if (pInfo == NULL)return;

	char atime[TimeLength] = { 0 };  //��ֹԽ��
	int nresult = logon(aName, aPassword, pInfo, pCardnodehead, ppheadBillingnode);

	switch (nresult) {
	case NotfindCard:
	case NoMatch: {
		printf("���Ų����ڻ���Ϣ���ԣ������ϻ���\n");
		break;
	}
	case Using: {
		printf("�ÿ������ϻ��������ظ��ϻ���\n");
		break;
	}
	case Success: {
		if (pInfo->fBanlance < 15)
			printf("�������ͣ��뼰ʱ��ֵ��\n\n");
		timetostring(pInfo->tLogon, atime);
		printf("���ţ�%s\n", aName);
		printf("��%.2f\n", pInfo->fBanlance);
		printf("�ϻ�ʱ�䣺%s\n", atime);
		printf("�ÿ���%s�����ϻ��ɹ���\n", aName);
		break;
	}
	case Invalid: {
		printf("�ÿ���ע���������ϻ���\n");
		break;
	}
	case NoEnoughMoney: {
		printf("�����Ϊ0�������ϻ���\n");
		break;
	}
	default: {
		break;
	}
	}
	free(pInfo);
}

void xiaji(Cardnode* pcardnodehead, Billingnode* pheadBillingnode) {
	char aName[10] = { 0 };
	char aPassword[10] = { 0 };

	printf("�����뿨�ţ�����Ϊ5��:");
	(void)scanf("%6s", aName);

	if (getSize(aName) != 5) {
		printf("���ų��ȱ���Ϊ5!\n");
		return;
	}
	printf("���������루����Ϊ1~8��:");
	(void)scanf("%9s", aPassword);

	if (getSize(aPassword) >= 8) {
		printf("��������볤�ȳ������ֵ��\n");
		return;
	}

	char astarttime[TimeLength] = { 0 };
	char aendtime[TimeLength] = { 0 };
	Settleinfo* pInfo = malloc(sizeof(Settleinfo));
	if (pInfo == NULL)return;

	printf("---------�»���Ϣ----------\n");
	int nresult = settle(aName, aPassword, pInfo, pcardnodehead, pheadBillingnode);

	switch (nresult) {
	case NotfindCard:
	case NoMatch: {
		printf("���Ų����ڻ���Ϣ���ԣ������»���\n");
		break;
	}
	case Success: {
		timetostring(pInfo->tStart, astarttime);
		timetostring(pInfo->tEnd, aendtime);
		printf("���ѣ�%.2f\n", pInfo->damount);
		printf("�ϻ�ʱ�䣺%s\n", astarttime);
		printf("�»�ʱ�䣺%s\n", aendtime);
		break;
	}
	case Unusing: {
		printf("�ÿ�û���ϻ���\n");
		break;
	}
	case NoEnoughMoney: {
		printf("�����㣬���ȳ�ֵ���»���\n");
		break;
	}
	case Invalid: {
		printf("�ÿ���ע��!\n");
		break;
	}
	default: {
		break;
	}
	}
	free(pInfo);
}

void addMoney(Cardnode* const pCardNodehead) {
	char aName[10] = { 0 };
	char aPwd[10] = { 0 };
	MoneyInfo* pMoneyInfo;
	pMoneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//���뿨�š�����
	printf("������Ҫ��ֵ�Ŀ���:\n");
	(void)scanf("%s", aName);
	printf("�������ֵ������:\n");
	(void)scanf("%s", aPwd);
	//����daAddMoney����
	int result = doAddMoney(aName, aPwd, pMoneyInfo, pCardNodehead);
	if (result != Success || result == Invalid)
	{
		printf("��ֵʧ��!\n");
	}
	else if (result == Success)
	{
		printf("��ֵ�ɹ�!\n");
		//��ʾ��ֵ��Ϣ
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%0.1f\t\t%0.1f\n", pMoneyInfo->aCardName, pMoneyInfo->dMoney, pMoneyInfo->dBalance);
	}
}

void refundMoney(Cardnode* const pCardNodehead)
{
	char aName[10] = { 0 };
	char aPwd[10] = { 0 };
	MoneyInfo* pMoneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//�����˷ѿ��ź�����
	printf("������Ҫ�˷ѵĿ���:\n");
	(void)scanf("%s", aName);
	printf("�����뿨������:\n");
	(void)scanf("%s", aPwd);

	//����doRefundMoney()����
	if (doRefundMoney(aName, aPwd, pMoneyInfo, pCardNodehead) != 1)
	{
		printf("�˷�ʧ��!\n");
	}
	else
	{
		printf("�˷ѳɹ�!\n");
		//��ʾ�˷���Ϣ
		printf("����\t�˷ѽ��\t���\n");
		printf("%s\t%0.1f\t\t%0.1f\n", pMoneyInfo->aCardName, pMoneyInfo->dMoney, pMoneyInfo->dBalance);
	}
}

void statNewCard(Cardnode* pCardnodehead) {
	unsigned yearStart, monthStart, dayStart;
	unsigned yearEnd, monthEnd, dayEnd;

	printf("������ͳ�ƵĿ�ʼʱ��(��-��-��):");
	(void)scanf("%u-%u-%u", &yearStart, &monthStart, &dayStart);
	if (Islegal(yearStart, monthStart, dayStart) == 0) {
		printf("ͳ�Ƶ�ʱ�䲻�Ϸ�\n");
		return;
	}

	printf("������ͳ�ƵĽ���ʱ��(��-��-��):");
	(void)scanf("%u-%u-%u", &yearEnd, &monthEnd, &dayEnd);
	if (Islegal(yearEnd, monthEnd, dayEnd) == 0) {
		printf("ͳ�Ƶ�ʱ�䲻�Ϸ�\n");
		return;
	}

	char timeStart[TimeLength] = { 0 }, timeEnd[TimeLength] = { 0 };

	sprintf(timeStart, "%d-%d-%d 0:0", yearStart, monthStart, dayStart);
	sprintf(timeEnd, "%d-%d-%d 23:59", yearEnd, monthEnd, dayEnd);

	time_t sTime, eTime;
	sTime = stringtotime(timeStart);
	eTime = stringtotime(timeEnd);

	FILE* fp = NULL;
	if ((fp = fopen("card.txt", "w")) == NULL) {
		return;
	}

	//*(strchr(timeStart, ' ')) = '\0';
	//*(strchr(timeEnd, ' ')) = '\0';

	fprintf(fp, "%s", "\t\t\t����ͳ�����ݱ���\n");
	fprintf(fp, "\t%s--------%s\n", timeStart, timeEnd);
	fprintf(fp, "%s", "-------------------------\n");
	fprintf(fp, "%s-------%s----------------%s\n", "����", "ʱ��", "���");

	Cardnode* pcardnode = pCardnodehead;
	int cardCnt = 0;
	while (pcardnode != NULL) {
		if (pcardnode->data.start >= sTime && pcardnode->data.lasttime <= eTime) {
			char timeTmp[TimeLength] = { 0 };
			timetostring(pcardnode->data.start, timeTmp);
			fprintf(fp, "%s\t%s\t%.2f\n", pcardnode->data.cname, timeTmp, pcardnode->data.balance);
			cardCnt++;
		}
		pcardnode = pcardnode->linkcard;
	}
	fprintf(fp, "%s", "-------------------------------\n");
	fprintf(fp, "\t\t��������:%d\n", cardCnt);

	fclose(fp);
	printf("������%d���¿������������ɣ�\n", cardCnt);
	return;
}

int Islegal(int year, int mon, int day) {
	//��1 3 5 7 8 10 12
	//С��4 6 9 11
	//ƽ��2
	if (year < 0 || mon <= 0 || mon>12 || day <= 0 || day > 31)
		return 0;
	if (1 == mon || 3 == mon || 5 == mon || 7 == mon || 8 == mon || 10 == mon || 12 == mon) {
		return 1;
	}
	if (IsLeapyear(year)) {
		if (2 == mon && (28 == day || 30 == day || 31 == day))
			return 0;
		return 1;
	}
	else {
		if (2 == mon && (29 == day || 30 == day || 31 == day))
			return 0;
		return 1;
	}
}

int IsLeapyear(int year) {
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
		return 1;
	return 0;
}

void zhuxiao(Cardnode* const pCardNodehead) {
	Card* pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)return;
	char lastname[10];
	char lastpwd[10];
	char lasttime[TimeLength] = { 0 };

	printf("�����뿨�ţ�����Ϊ5��:");
	(void)scanf("%6s", lastname);

	if (getSize(lastname) != 5) {
		printf("���ų��ȱ���Ϊ5!\n");
		return;
	}
	strcpy(pCard->cname, lastname);

	printf("���������루����Ϊ1~8��:");
	(void)scanf("%9s", lastpwd);

	if (getSize(lastpwd) >= 8) {
		printf("��������볤�ȳ������ֵ��\n");
		return;
	}
	strcpy(pCard->cpassword, lastpwd);

	int result = 0;

	result = zhuxiaoCard(lastname, lastpwd, pCard, pCardNodehead);

	switch (result) {
	case NotfindCard:
	case NoMatch:
	{
		printf("���Ų����ڻ���Ϣ���ԣ�����ע����\n");
		break;
	}
	case Success: {
		printf("ע���ɹ���\n");
		pCard->lasttime = time(NULL);
		timetostring(pCard->lasttime, lasttime);
		printf("����\tע��ʱ��\tע��״̬\n");
		printf("%s\t%s\t��ע��\n", pCard->cname, lasttime);
		break;
	}
	case Using: {
		printf("�����ϻ�������ע��\n");
		break;
	}
	case Invalid: {
		printf("�ÿ��Ѿ�ʧЧ��\n");
		break;
	}
	default:break;
	}

	free(pCard);
}

int zhuxiaoCard(char* strNo, char* strSwd, Card* pCard, Cardnode* const pCardNodehead) {
	int pCardIndex = 0;
	Card* ppCard = NULL;
	ppCard = cardIsExist(strNo, &pCardIndex, pCardNodehead);
	if (ppCard == NULL)
		return NotfindCard;
	if (strcmp(ppCard->cname, strNo) != 0)return NoMatch;
	if (ppCard->state == Using)return Using;
	if (ppCard->state == Invalid)return Invalid;
	if (ppCard->balance > 0) {
		printf("���Ȱ����˷�ҵ��\n");
		return -1;
	}
	if (ppCard->deletion == 1)return Invalid;

	//���Ŀ���Ϣ
	ppCard->state = Invalid;
	ppCard->lasttime = time(NULL);
	ppCard->deletion = 1;
	ppCard->balance = 0;

	Card** pppCard = &pCard;
	pppCard = &ppCard;

	updateCard(ppCard, CardPath, pCardIndex);

	return Success;
}

int allmoney(char* pPath) {
	unsigned yearStart, monthStart, dayStart;
	unsigned yearEnd, monthEnd, dayEnd;

	printf("������ͳ�ƵĿ�ʼʱ��(��-��-��):");
	(void)scanf("%u-%u-%u", &yearStart, &monthStart, &dayStart);
	if (Islegal(yearStart, monthStart, dayStart) == 0) {
		printf("ͳ�Ƶ�ʱ�䲻�Ϸ�\n");
		return;
	}

	printf("������ͳ�ƵĽ���ʱ��(��-��-��):");
	(void)scanf("%u-%u-%u", &yearEnd, &monthEnd, &dayEnd);
	if (Islegal(yearEnd, monthEnd, dayEnd) == 0) {
		printf("ͳ�Ƶ�ʱ�䲻�Ϸ�\n");
		return;
	}

	char timeStart[TimeLength] = { 0 }, timeEnd[TimeLength] = { 0 }, mtime[TimeLength] = { 0 };

	sprintf(timeStart, "%d-%d-%d 0:0", yearStart, monthStart, dayStart);
	sprintf(timeEnd, "%d-%d-%d 23:59", yearEnd, monthEnd, dayEnd);

	time_t sTime, eTime;
	sTime = stringtotime(timeStart);
	eTime = stringtotime(timeEnd);

	FILE* fp = NULL;

	Money* pMoney = (Money*)malloc(sizeof(Money));
	if (pMoney == NULL)return 0;

	fp = fopen(pPath, "rb");
	if (fp == NULL)return 0;
	double sum = 0;

	while (!feof(fp)) {
		if (fread(pMoney, sizeof(Money), 1, fp) != 0) {
			timetostring(pMoney->Etime, mtime);
			if (sTime <= pMoney->Etime && pMoney->Etime <= eTime)
			{
				printf("\n\n����\t\t��ֵ�˷�ʱ��\t״̬\t��ֵ�˷ѽ��\tɾ����־\n");
				printf("%s\t%s\t%d\t%0.1f\t\t%d\n", pMoney->aCardname, mtime, pMoney->nstatus, pMoney->dMoney, pMoney->deletion);
				sum += pMoney->dMoney;
			}
		}
	}
	printf("\n�ܳ�ֵ�˷ѽ��Ϊ:%0.1f\n", sum);
	fclose(fp);
	return 1;
}

void findzhuxiao(char* pPath) {
	FILE* fp = NULL;
	int cnt = 0;
	char pptime[TimeLength] = { 0 };
	Card* pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)return;
	fp = fopen(pPath, "rb");
	if (fp == NULL)return 0;
	while (!feof(fp)) {
		if (fread(pCard, sizeof(Card), 1, fp) != 0) {
			if (pCard->deletion == 1)
			{
				printf("����\tɾ����־\n");
				printf("%s\t��ע��\n", pCard->cname);
				cnt++;
			}
		}
	}

	if (cnt == 0)printf("\n��ʱû�п�ע��\n");
	else printf("\n�ܹ�ע��%d�ſ�\n", cnt);
	fclose(fp);
}

void tongjibilling(char* pPath) {
	unsigned yearStart, monthStart, dayStart;
	unsigned yearEnd, monthEnd, dayEnd;

	printf("������ͳ�ƵĿ�ʼʱ��(��-��-��):");
	(void)scanf("%u-%u-%u", &yearStart, &monthStart, &dayStart);
	if (Islegal(yearStart, monthStart, dayStart) == 0) {
		printf("ͳ�Ƶ�ʱ�䲻�Ϸ�\n");
		return;
	}

	printf("������ͳ�ƵĽ���ʱ��(��-��-��):");
	(void)scanf("%u-%u-%u", &yearEnd, &monthEnd, &dayEnd);
	if (Islegal(yearEnd, monthEnd, dayEnd) == 0) {
		printf("ͳ�Ƶ�ʱ�䲻�Ϸ�\n");
		return;
	}

	char timeStart[TimeLength] = { 0 }, timeEnd[TimeLength] = { 0 }, mtime[TimeLength] = { 0 };

	sprintf(timeStart, "%d-%d-%d 0:0", yearStart, monthStart, dayStart);
	sprintf(timeEnd, "%d-%d-%d 23:59", yearEnd, monthEnd, dayEnd);

	time_t sTime, eTime;
	sTime = stringtotime(timeStart);
	eTime = stringtotime(timeEnd);

	FILE* fp = NULL;
	double sum = 0;
	char pptime[TimeLength] = { 0 };
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));

	if (pBilling == NULL)return;
	fp = fopen(pPath, "rb");
	if (fp == NULL)return 0;

	while (!feof(fp)) {
		if (fread(pBilling, sizeof(Billing), 1, fp) != 0) {
			timetostring(pBilling->tEnd, pptime);
			if (sTime <= pBilling->tStart && pBilling->tEnd <= eTime)
			{
				printf("\n\n����\t   ���ʹ��ʱ��\t\t���ѽ��\n");
				printf("%s\t%s\t%.1f\n", pBilling->aCardname, pptime, pBilling->damount);
				sum = sum + pBilling->damount;
			}
		}
	}
	if (sum == 0)
		printf("\n�������Ѽ�¼\n");
	else printf("\n�����ѽ��Ϊ%0.1fԪ\n", sum);

	fclose(fp);
	return;
}

void jihuo(Cardnode* const pCardNodehead) {
	Card* pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)return;
	char lastname[10];
	char lastpwd[10];
	char lasttime[TimeLength] = { 0 };

	printf("�����뿨�ţ�����Ϊ5��:");
	(void)scanf("%6s", lastname);

	if (getSize(lastname) != 5) {
		printf("���ų��ȱ���Ϊ5!\n");
		return;
	}
	strcpy(pCard->cname, lastname);

	printf("���������루����Ϊ1~8��:");
	(void)scanf("%9s", lastpwd);

	if (getSize(lastpwd) >= 8) {
		printf("��������볤�ȳ������ֵ��\n");
		return;
	}
	strcpy(pCard->cpassword, lastpwd);

	int result = -1;

	result = jihuoCard(lastname, lastpwd, pCard, pCardNodehead);

	switch (result) {
	case NotfindCard:
	case NoMatch:
	{
		printf("���Ų����ڻ���Ϣ���ԣ����ܼ��\n");
		break;
	}
	case Success: {
		printf("����ɹ���\n");
		pCard->lasttime = time(NULL);
		timetostring(pCard->lasttime, lasttime);
		printf("����\t����ʱ��\t״̬\n");
		printf("%s\t%s\tδ�ϻ�\n", pCard->cname, lasttime);
		break;
	}
	case Using: {
		printf("�����ϻ����޷�����\n");
		break;
	}
	case Unusing: {
		printf("�ÿ�����ʹ�ã�\n");
		break;
	}
	default:break;
	}

	free(pCard);
}

int jihuoCard(char* strNo, char* strSwd, Card* pCard, Cardnode* const pCardNodehead) {
	int pCardIndex = 0;
	Card* ppCard = NULL;
	ppCard = cardIsExist(strNo, &pCardIndex, pCardNodehead);
	if (ppCard == NULL)
		return NotfindCard;
	if (strcmp(ppCard->cname, strNo) != 0)return NoMatch;
	if (ppCard->state == Using)return Using;
	if (ppCard->state == Unusing)return Unusing;
	if (ppCard->deletion == 0)return Unusing;

	//���Ŀ���Ϣ
	ppCard->state = Unusing;
	ppCard->lasttime = time(NULL);
	ppCard->deletion = 0;
	ppCard->balance = 0;

	Card** pppCard = &pCard;
	pppCard = &ppCard;

	updateCard(ppCard, CardPath, pCardIndex);

	return Success;
}