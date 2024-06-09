#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include"model.h"  //结构体
#include"menu.h"
#include"global.h"
#include"tool.h"
#include"service.h"
/*菜单输出函数*/
void outputmenu() {
	printf("---------菜单---------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("9.激活卡\n");
	printf("0.退出\n");
	printf("请选择菜单编号（0~9）:");
}

void outputmenu7() {
	printf("--------查询菜单---------\n");
	printf("1.查询总卡\n");
	printf("2.查询充值退费\n");
	printf("3.统计消费\n");
	printf("4.查询销卡\n");
	printf("输入-- 0 --以退出程序\n");
	printf("请选择菜单编号（1~4）:");
}

/*添加卡函数*/
void addcard(Cardnode** pcardnodehead) {
	char mname[15] = { 0 };
	char mpassword[10] = { 0 };
	Cardnode* pcardnext = (Cardnode*)malloc(sizeof(Cardnode));
	if (pcardnext == NULL)return;
	//新建结构体存放新卡，随后复制进结构体中

	printf("----------添加卡-----------\t\n");
	printf("请输入卡号（长度为5）：");
	(void)scanf("%6s", mname);

	if (strlen(mname) != 5) {
		printf("卡号长度不符！\n");
		return;
	}
	int nCardIndex = 0;
	if (cardIsExist(mname, &nCardIndex, *pcardnodehead) != NULL)
	{
		printf("卡号【%s】已经存在!\n", mname);
		return;
	}

	strcpy(pcardnext->data.cname, mname);

	printf("请输入卡号密码（长度1-8）：");
	(void)scanf("%9s", mpassword);
	fflush(stdin);

	if (strlen(mpassword) >= 8) {
		printf("卡号密码过长，请重新开卡！\n");
		return;
	}

	strcpy(pcardnext->data.cpassword, mpassword);

	printf("请输入开卡金额（RMB）：");
	(void)scanf("%lf", &pcardnext->data.balance);

	if (pcardnext->data.balance < 0) {
		printf("输入的开卡金额不能小于0！\n");
		return;
	}

	if (pcardnext->data.balance != (int)pcardnext->data.balance) {
		printf("请输入整数金额！\n");
		return;
	}

	pcardnext->data.useofmoney = pcardnext->data.balance;
	pcardnext->data.deletion = 0;
	pcardnext->data.state = Unusing;
	pcardnext->data.usenum = 0;
	pcardnext->data.start = time(NULL);  //获取系统时间，单位为秒;
	pcardnext->data.end = time(NULL);
	pcardnext->data.lasttime = time(NULL);
	//C 库函数 time_t time(time_t *seconds) 返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位。
	// 如果 seconds 不为空，则返回值也存储在变量 seconds 中。

	struct tm* endoftime;
	struct tm* starttime;
	struct tm* lasttime;
	starttime = localtime(&pcardnext->data.start);
	endoftime = localtime(&pcardnext->data.end);
	lasttime = localtime(&pcardnext->data.lasttime);

	//C 库函数 struct tm *localtime(const time_t *timer) 使用 timer 的值来填充 tm 结构。
	// timer 的值被分解为 tm 结构，并用本地时区表示。

	endoftime->tm_year = endoftime->tm_year + 2;//每张卡有效期2年
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

	printf("添加卡状态成功!\n");
}

void shangji(Cardnode* pCardnodehead, Billingnode** ppheadBillingnode) {
	char aName[10] = { 0 };
	char aPassword[10] = { 0 };

	printf("请保证输入卡号不要过长以引起程序崩溃!\n");
	printf("请输入卡号（长度为5）:");
	(void)scanf("%6s", aName);
	fflush(stdin);

	if (getSize(aName) != 5) {
		printf("卡号长度必须为5!\n");
		return;
	}
	printf("请输入密码（长度为1~8）:");
	(void)scanf("%9s", aPassword);
	fflush(stdin);

	if (getSize(aPassword) >= 8) {
		printf("输入的密码长度超过最大值！\n");
		return;
	}

	printf("---------上机信息----------\n");

	LogonInfo* pInfo = malloc(sizeof(LogonInfo));
	if (pInfo == NULL)return;

	char atime[TimeLength] = { 0 };  //防止越界
	int nresult = logon(aName, aPassword, pInfo, pCardnodehead, ppheadBillingnode);

	switch (nresult) {
	case NotfindCard:
	case NoMatch: {
		printf("卡号不存在或卡信息不对，不能上机！\n");
		break;
	}
	case Using: {
		printf("该卡正在上机，请勿重复上机！\n");
		break;
	}
	case Success: {
		if (pInfo->fBanlance < 15)
			printf("卡余额过低，请及时充值！\n\n");
		timetostring(pInfo->tLogon, atime);
		printf("卡号：%s\n", aName);
		printf("余额：%.2f\n", pInfo->fBanlance);
		printf("上机时间：%s\n", atime);
		printf("该卡【%s】已上机成功！\n", aName);
		break;
	}
	case Invalid: {
		printf("该卡已注销，不能上机！\n");
		break;
	}
	case NoEnoughMoney: {
		printf("卡余额为0，不能上机！\n");
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

	printf("请输入卡号（长度为5）:");
	(void)scanf("%6s", aName);

	if (getSize(aName) != 5) {
		printf("卡号长度必须为5!\n");
		return;
	}
	printf("请输入密码（长度为1~8）:");
	(void)scanf("%9s", aPassword);

	if (getSize(aPassword) >= 8) {
		printf("输入的密码长度超过最大值！\n");
		return;
	}

	char astarttime[TimeLength] = { 0 };
	char aendtime[TimeLength] = { 0 };
	Settleinfo* pInfo = malloc(sizeof(Settleinfo));
	if (pInfo == NULL)return;

	printf("---------下机信息----------\n");
	int nresult = settle(aName, aPassword, pInfo, pcardnodehead, pheadBillingnode);

	switch (nresult) {
	case NotfindCard:
	case NoMatch: {
		printf("卡号不存在或卡信息不对，不能下机！\n");
		break;
	}
	case Success: {
		timetostring(pInfo->tStart, astarttime);
		timetostring(pInfo->tEnd, aendtime);
		printf("消费：%.2f\n", pInfo->damount);
		printf("上机时间：%s\n", astarttime);
		printf("下机时间：%s\n", aendtime);
		break;
	}
	case Unusing: {
		printf("该卡没有上机！\n");
		break;
	}
	case NoEnoughMoney: {
		printf("卡余额不足，请先充值在下机！\n");
		break;
	}
	case Invalid: {
		printf("该卡已注销!\n");
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
	//输入卡号、密码
	printf("请输入要充值的卡号:\n");
	(void)scanf("%s", aName);
	printf("请输入充值卡密码:\n");
	(void)scanf("%s", aPwd);
	//调用daAddMoney函数
	int result = doAddMoney(aName, aPwd, pMoneyInfo, pCardNodehead);
	if (result != Success || result == Invalid)
	{
		printf("充值失败!\n");
	}
	else if (result == Success)
	{
		printf("充值成功!\n");
		//显示充值信息
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%0.1f\t\t%0.1f\n", pMoneyInfo->aCardName, pMoneyInfo->dMoney, pMoneyInfo->dBalance);
	}
}

void refundMoney(Cardnode* const pCardNodehead)
{
	char aName[10] = { 0 };
	char aPwd[10] = { 0 };
	MoneyInfo* pMoneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//输入退费卡号和密码
	printf("请输入要退费的卡号:\n");
	(void)scanf("%s", aName);
	printf("请输入卡的密码:\n");
	(void)scanf("%s", aPwd);

	//调用doRefundMoney()函数
	if (doRefundMoney(aName, aPwd, pMoneyInfo, pCardNodehead) != 1)
	{
		printf("退费失败!\n");
	}
	else
	{
		printf("退费成功!\n");
		//显示退费信息
		printf("卡号\t退费金额\t余额\n");
		printf("%s\t%0.1f\t\t%0.1f\n", pMoneyInfo->aCardName, pMoneyInfo->dMoney, pMoneyInfo->dBalance);
	}
}

void statNewCard(Cardnode* pCardnodehead) {
	unsigned yearStart, monthStart, dayStart;
	unsigned yearEnd, monthEnd, dayEnd;

	printf("请输入统计的开始时间(年-月-日):");
	(void)scanf("%u-%u-%u", &yearStart, &monthStart, &dayStart);
	if (Islegal(yearStart, monthStart, dayStart) == 0) {
		printf("统计的时间不合法\n");
		return;
	}

	printf("请输入统计的结束时间(年-月-日):");
	(void)scanf("%u-%u-%u", &yearEnd, &monthEnd, &dayEnd);
	if (Islegal(yearEnd, monthEnd, dayEnd) == 0) {
		printf("统计的时间不合法\n");
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

	fprintf(fp, "%s", "\t\t\t开卡统计数据报表\n");
	fprintf(fp, "\t%s--------%s\n", timeStart, timeEnd);
	fprintf(fp, "%s", "-------------------------\n");
	fprintf(fp, "%s-------%s----------------%s\n", "卡号", "时间", "余额");

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
	fprintf(fp, "\t\t开卡总数:%d\n", cardCnt);

	fclose(fp);
	printf("共开了%d张新卡，报表已生成！\n", cardCnt);
	return;
}

int Islegal(int year, int mon, int day) {
	//大：1 3 5 7 8 10 12
	//小：4 6 9 11
	//平：2
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

	printf("请输入卡号（长度为5）:");
	(void)scanf("%6s", lastname);

	if (getSize(lastname) != 5) {
		printf("卡号长度必须为5!\n");
		return;
	}
	strcpy(pCard->cname, lastname);

	printf("请输入密码（长度为1~8）:");
	(void)scanf("%9s", lastpwd);

	if (getSize(lastpwd) >= 8) {
		printf("输入的密码长度超过最大值！\n");
		return;
	}
	strcpy(pCard->cpassword, lastpwd);

	int result = 0;

	result = zhuxiaoCard(lastname, lastpwd, pCard, pCardNodehead);

	switch (result) {
	case NotfindCard:
	case NoMatch:
	{
		printf("卡号不存在或卡信息不对，不能注销！\n");
		break;
	}
	case Success: {
		printf("注销成功！\n");
		pCard->lasttime = time(NULL);
		timetostring(pCard->lasttime, lasttime);
		printf("卡号\t注销时间\t注销状态\n");
		printf("%s\t%s\t已注销\n", pCard->cname, lasttime);
		break;
	}
	case Using: {
		printf("正在上机，不能注销\n");
		break;
	}
	case Invalid: {
		printf("该卡已经失效！\n");
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
		printf("请先办理退费业务！\n");
		return -1;
	}
	if (ppCard->deletion == 1)return Invalid;

	//更改卡信息
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

	printf("请输入统计的开始时间(年-月-日):");
	(void)scanf("%u-%u-%u", &yearStart, &monthStart, &dayStart);
	if (Islegal(yearStart, monthStart, dayStart) == 0) {
		printf("统计的时间不合法\n");
		return;
	}

	printf("请输入统计的结束时间(年-月-日):");
	(void)scanf("%u-%u-%u", &yearEnd, &monthEnd, &dayEnd);
	if (Islegal(yearEnd, monthEnd, dayEnd) == 0) {
		printf("统计的时间不合法\n");
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
				printf("\n\n卡号\t\t充值退费时间\t状态\t充值退费金额\t删除标志\n");
				printf("%s\t%s\t%d\t%0.1f\t\t%d\n", pMoney->aCardname, mtime, pMoney->nstatus, pMoney->dMoney, pMoney->deletion);
				sum += pMoney->dMoney;
			}
		}
	}
	printf("\n总充值退费金额为:%0.1f\n", sum);
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
				printf("卡号\t删除标志\n");
				printf("%s\t已注销\n", pCard->cname);
				cnt++;
			}
		}
	}

	if (cnt == 0)printf("\n暂时没有卡注销\n");
	else printf("\n总共注销%d张卡\n", cnt);
	fclose(fp);
}

void tongjibilling(char* pPath) {
	unsigned yearStart, monthStart, dayStart;
	unsigned yearEnd, monthEnd, dayEnd;

	printf("请输入统计的开始时间(年-月-日):");
	(void)scanf("%u-%u-%u", &yearStart, &monthStart, &dayStart);
	if (Islegal(yearStart, monthStart, dayStart) == 0) {
		printf("统计的时间不合法\n");
		return;
	}

	printf("请输入统计的结束时间(年-月-日):");
	(void)scanf("%u-%u-%u", &yearEnd, &monthEnd, &dayEnd);
	if (Islegal(yearEnd, monthEnd, dayEnd) == 0) {
		printf("统计的时间不合法\n");
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
				printf("\n\n卡号\t   最后使用时间\t\t消费金额\n");
				printf("%s\t%s\t%.1f\n", pBilling->aCardname, pptime, pBilling->damount);
				sum = sum + pBilling->damount;
			}
		}
	}
	if (sum == 0)
		printf("\n暂无消费记录\n");
	else printf("\n总消费金额为%0.1f元\n", sum);

	fclose(fp);
	return;
}

void jihuo(Cardnode* const pCardNodehead) {
	Card* pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)return;
	char lastname[10];
	char lastpwd[10];
	char lasttime[TimeLength] = { 0 };

	printf("请输入卡号（长度为5）:");
	(void)scanf("%6s", lastname);

	if (getSize(lastname) != 5) {
		printf("卡号长度必须为5!\n");
		return;
	}
	strcpy(pCard->cname, lastname);

	printf("请输入密码（长度为1~8）:");
	(void)scanf("%9s", lastpwd);

	if (getSize(lastpwd) >= 8) {
		printf("输入的密码长度超过最大值！\n");
		return;
	}
	strcpy(pCard->cpassword, lastpwd);

	int result = -1;

	result = jihuoCard(lastname, lastpwd, pCard, pCardNodehead);

	switch (result) {
	case NotfindCard:
	case NoMatch:
	{
		printf("卡号不存在或卡信息不对，不能激活！\n");
		break;
	}
	case Success: {
		printf("激活成功！\n");
		pCard->lasttime = time(NULL);
		timetostring(pCard->lasttime, lasttime);
		printf("卡号\t激活时间\t状态\n");
		printf("%s\t%s\t未上机\n", pCard->cname, lasttime);
		break;
	}
	case Using: {
		printf("正在上机，无法激活\n");
		break;
	}
	case Unusing: {
		printf("该卡可以使用！\n");
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

	//更改卡信息
	ppCard->state = Unusing;
	ppCard->lasttime = time(NULL);
	ppCard->deletion = 0;
	ppCard->balance = 0;

	Card** pppCard = &pCard;
	pppCard = &ppCard;

	updateCard(ppCard, CardPath, pCardIndex);

	return Success;
}