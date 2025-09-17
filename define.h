#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"CSocketLx.h"
enum {SEVER_PORT=8080};
enum {
	WK_PRINT = 0x1234,
	WK_INPUT,
	WK_DELETE,
	WK_MODIFY,
	WK_FIND_NUMB,
	WK_FIND_NAME,
	WK_FIND_SALARY,
	WK_FIND_DATE,

	AD_LOGIN = 0x2345,
	AD_PRINT,
	AD_INPUT,
	AD_DELETE,
	AD_CHHPASS,
	AD_CHECK,
};
struct  sAdmin
{
	char sName[20];
	char sPass[20];
	int nPrior;
};
struct SInfo
{
	int nNumb;
	char sName[20];
	float fsala;
	char sDate[20];

};
