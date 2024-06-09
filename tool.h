#define _CRT_SECURE_NO_WARNINGS 1
#include<time.h>
#include<stdio.h>
//Cardnode* qureycard(char* name, Cardnode* pcardnodehead);
Card* cardIsExist(char* strNo, int* pCardIndex, Cardnode* const pCardNodehead);
void query(Cardnode* const  pcardnodehead);
void timetostring(time_t t, char* pBuff);
time_t stringtotime(char* ptime);

void display(Cardnode* pcardnodehead);
void lookup(Cardnode* pcardnodehead);

int getSize(char* p);

double getAmount(time_t tStart);