#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include <stdio.h>
#include<stdlib.h>
#include "model.h"
//���忨�Ľṹ��
#include"menu.h"
//�˵�������ӿ�����ѯ�����ϻ����»�����ֵ���˷ѣ���ѯͳ�ƣ�ע������
#include"tool.h"
#include"global.h"
#include"service.h"
#pragma once
//ʱ�乤��

int main(void) {
	Cardnode* pcardnodehead = NULL;

	Billingnode* pheadBillingnode = NULL;

	MoneyInfo* pheadmoneyinfo = NULL;

	if (readCard(&pcardnodehead, CardPath) == 0) {
		printf("���ݴ���1!\n");
		return 0;
	}

	if (readBilling(&pheadBillingnode, BillingPath) == 0) {
		printf("���ݴ���2��\n");
		return 0;
	}

	int serial = -1;  //�����û���������

	printf("��ӭ����Ʒѹ���ϵͳ\n");
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
			//��ֵ recharge
			break;
		}
		case 6: {
			refundMoney(pcardnodehead);
			//�˷� refund
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
			printf("ע����\n");
			zhuxiao(pcardnodehead);
			//ע���� logout
			break;
		}
		case 9: {
			printf("���\n");
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
			printf("����˵������Ч��\n");
			break;
		}
		}
	} while (serial != 0);
	return 0;
}