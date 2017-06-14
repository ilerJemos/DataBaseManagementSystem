
#include "CreateTable.h"
#include "Index.h"
#define SH_NUM    10   //shadow number.

typedef struct
{
	int tNum;// table number
	int fNum;// field number
	int dis; // 偏移量
	char cons[20];// const
	char datatype;
}fieldImf;

typedef struct
{
	fieldImf  sha[SH_NUM];
	int len;
	int num;
}Shadows;

typedef struct
{
	Table* t[3];//
	//char *buf[3];//
	int num;
	//INDEX* pid[3];
	//int rNum;
	//int len;//   表头的长度
}tableImf;

typedef struct
{
	fieldImf para[2];
	//fieldImf para2;
	int operatortype;
}aCondition;	//每个条件

typedef struct
{
	aCondition content[10];//最多的条件个数
	int relation;
	int num;
}Conditions;	//所有条件

typedef struct
{
	int record[200][2];// 2:table number;
	int dimesion;      // 维数
	int num;           //record number;
}Result;


bool SelectTable(int strlen,int* authority);

int SelectTableByIndex(INDEX* pif,Table* t,char ch[],int datatype,int operatortype,
						int start_no);


int SelectTableByFile(INDEX* pif,Table* t,char ch[],int datatype,int operatortype,int field_no,
					 char* buf,int start_no);

int SelectFieldType(char ch[]);

int GetFileNo(Table* t,char ch[]);

void ShowRecord(char buf[],Table* pt);

bool getTableImf(tableImf* ptf,char ch[][20],int strlen);

bool isIncludeTable(char* tName,tableImf* ptf);

bool getConditionImf(tableImf* ptf,Conditions* pc,char ch[][20],int strlen);

int getFieldDis(int fieldNumth,Table* pt);//属性在一个记录中的偏移量！