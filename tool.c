#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include"model.h"
#include"tool.h"
#include<string.h>
#include"global.h"

void query(Cardnode* const pcardnodehead) {
	int index;
	printf("准确查询----1\n");
	printf("全局查询----2\n");
	printf("请输入查询方式：\n");
	(void)scanf("%d", &index);
	switch (index) {
	case 1:
	{
		display(pcardnodehead);
		break;
	}
	case 2:
	{
		lookup(pcardnodehead);
		break;
	}
	}
	return;
}

//精确查找
void display(Cardnode* pcardnodehead) {
	Card* ppnum;

	char nname[6] = { 0 };
	char timeinnow[TimeLength] = { 0 };

	printf("输入想要查询的卡号：");
	(void)scanf("%6s", &nname);

	int nCardIndex = 0;
	if (cardIsExist(nname, &nCardIndex, pcardnodehead) != NULL) {
		printf("已查询到卡号\n");

		ppnum = cardIsExist(nname, &nCardIndex, pcardnodehead);

		printf("\n%6s%10s%8s%8s%15s%15s\n", "卡号", "密码", "状态", "余额", "使用次数", "开卡时间");
		printf("-------------------------------------------------------------\n");
		printf("%6s", ppnum->cname);
		printf("%10s", ppnum->cpassword);
		if (ppnum->state == Invalid) {
			printf("已注销\n");
			return;
		}
		printf("%10s", ppnum->state == Unusing ? "未上机" : "已上机");
		printf("%8.2f", ppnum->balance);
		printf("%6d", ppnum->usenum);
		timetostring(ppnum->start, timeinnow);
		printf("%25s\n", timeinnow);
	}
	else printf("卡号不存在\n");
}

// 模糊查找函数
void lookup(Cardnode* pcardnodehead) {
	printf("\n%6s%10s%8s%8s%15s%15s\n", "卡号", "密码", "状态", "余额", "使用次数", "开卡时间");
	printf("-------------------------------------------------------------\n");

	char timeinnoww[TimeLength] = { 0 };

	Cardnode* pnext = pcardnodehead;

	while (pnext != NULL) {
		printf("%6s", pnext->data.cname);
		printf("%10s", pnext->data.cpassword);
		if (pnext->data.state == Invalid) {
			printf("    已注销\n");
		}
		else {
			printf("%10s", pnext->data.state == Unusing ? "未上机" : "已上机");
			printf("%8.2f", pnext->data.balance);
			printf("%6d", pnext->data.usenum);
			timetostring(pnext->data.start, timeinnoww);
			printf("%25s\n", timeinnoww);
		}
		pnext = pnext->linkcard;
	}
	printf("\n");
}

void timetostring(time_t t, char* pBuff) {
	struct tm* timeof;
	timeof = localtime(&t);
	//C 库函数 size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr)
	//根据 format 中定义的格式化规则，格式化结构 timeptr 表示的时间，并把它存储在 str 中。
	strftime(pBuff, 25, "%Y-%m-%d %H:%M %a", timeof);
}

time_t stringtotime(char* ptime) {
	struct tm qtime;
	time_t qqtime;
	//tm结构体中数据均为int,且tm_wday指一星期中的第几天，从星期日开始0-6.
	(void)sscanf(ptime, "%d-%d-%d %d:%d", &qtime.tm_year, &qtime.tm_mon, &qtime.tm_mday, &qtime.tm_hour, &qtime.tm_min);

	qtime.tm_year -= 1900;
	qtime.tm_mon -= 1;

	qtime.tm_sec = 0;
	qtime.tm_isdst = -1;
	qqtime = mktime(&qtime);
	return qqtime;
}

int getSize(char* p) {
	int cnt = 0;
	while (*p != '\0') {
		cnt++;
		p++;
	}
	return cnt;
}

double getAmount(time_t tStart) {
	double damount = 0;
	int ncount = 0; //上机单位数
	int nsec = 0;  //秒
	int nmin = 0;   //分钟
	time_t tEnd = time(NULL);
	nsec = (int)difftime(tEnd, tStart);
	nmin = nsec / 60;
	if (nmin % Unit == 0) {
		ncount = nmin / Unit;
	}
	else {
		ncount = nmin / Unit + 1;
	}
	if (ncount == 0)
		ncount = 1;

	damount = ncount * Charge;
	return damount;
}