#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include <stdio.h>
#include<stdlib.h>
#include "model.h"
//定义卡的结构体
#include"menu.h"
//菜单栏：添加卡，查询卡，上机，下机，充值，退费，查询统计，注销函数
#include"tool.h"
#include"global.h"
#include"service.h"
#pragma once
//时间工具

int main(void) {
	Cardnode* pcardnodehead = NULL;

	Billingnode* pheadBillingnode = NULL;

	MoneyInfo* pheadmoneyinfo = NULL;

	if (readCard(&pcardnodehead, CardPath) == 0) {
		printf("数据错误1!\n");
		return 0;
	}

	if (readBilling(&pheadBillingnode, BillingPath) == 0) {
		printf("数据错误2！\n");
		return 0;
	}

	int serial = -1;  //定义用户输入的序号

	printf("欢迎进入计费管理系统\n");
	do {
		outputmenu();

		serial = -1;

		(void)scanf("%d", &serial);
		char c;
		while (c = getchar() != '\n');
		switch (serial) {
		case 1: {
			addcard(&pcardnodehead);
			break;
		}
		case 2: {
			query(pcardnodehead);
			break;
		}
		case 3: {
			shangji(pcardnodehead, &pheadBillingnode);
			break;
		}
		case 4: {
			xiaji(pcardnodehead, pheadBillingnode);
			break;
		}
		case 5: {
			addMoney(pcardnodehead);
			//充值 recharge
			break;
		}
		case 6: {
			refundMoney(pcardnodehead);
			//退费 refund
			break;
		}
		case 7: {
			int cntt = -1;
			do {
				outputmenu7();
				(void)scanf("%d", &cntt);
				switch (cntt)
				{
				case 1: {
					statNewCard(pcardnodehead);
					break;
				}
				case 2: {
					allmoney(MoneyPath);
					break;
				}
				case 3: {
					tongjibilling(BillingPath);
					break;
				}
				case 4: {
					findzhuxiao(CardPath);
					break;
				}

				default:
					break;
				}
			} while (cntt != 0);
			break;
		}
		case 8: {
			printf("注销卡\n");
			zhuxiao(pcardnodehead);
			//注销卡 logout
			break;
		}
		case 9: {
			printf("激活卡\n");
			jihuo(pcardnodehead);
			break;
		}
		case 0: {
			releaseCard(pcardnodehead);
			releaseBilling(pheadBillingnode);
			break;
		}
		default: {
			system("cls");
			printf("输入菜单编号无效！\n");
			break;
		}
		}
	} while (serial != 0);
	return 0;
}