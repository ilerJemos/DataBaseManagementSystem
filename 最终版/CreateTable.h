
#ifndef _CREATETABLE_H
#define _CREATETABLE_H

#define PRO_NUM  10
#define MAX_INDEX 20

#include "Index.h"


typedef struct 
{
	char name[20];	//名称
	int  datatype;//数据类型 1char 2int 3float
	unsigned length;	//数据长度 例如int4 float8
	bool pkey;	//主键
}Field;	//表的每个属性


typedef struct
{
	char name[20];	//表名字
	Field field[PRO_NUM];	//表的属性
	unsigned int field_num;	//属性数量
	unsigned int key_num;	//主键数量
	unsigned int textline;	//记录有多少
	int fieldLen;	//属性长度
	char buf[1024];
	INDEX* pif;
	int  stateS;	//共享锁状态
	bool stateX;	//排它锁状态
} Table;	//表结构





bool CreateTable(int str_num);
bool ShowFileIndex(char *fname);
//bool InsertNode(slink* sl,int app);
//int GetNode(slink *sl);
int CountRecordLen(Table *t,int end_no);
//bool initIndex(slink *sl,int key_num);
#endif