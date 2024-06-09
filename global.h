#pragma once

#define CardPath "card.dat"
#define BillingPath "billing.dat"
#define MoneyPath "money.dat"

#define TimeLength 25

#define Success 0         //成功
#define FindCard 1        //找到卡
#define NotfindCard 2     //未找到卡
#define NoMatch 3         //匹配失败
#define NoEnoughMoney 4   //余额不足
#define NoFare 5          //没有找到计费信息
#define Using 6           //使用
#define Unusing 7         //未使用
#define Invalid 8         //不可用

#define NosettleMent 0    //未下机
#define YessettleMent 1

#define Unit 60    //最小收费单位（分钟）
#define Charge 5   //每个计费单元收费（元）