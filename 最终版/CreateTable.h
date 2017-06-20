
#ifndef _CREATETABLE_H
#define _CREATETABLE_H

#define PRO_NUM  10
#define MAX_INDEX 20

#include "Index.h"


typedef struct 
{
	char name[20];	//����
	int  datatype;//�������� 1char 2int 3float
	unsigned length;	//���ݳ��� ����int4 float8
	bool pkey;	//����
}Field;	//���ÿ������


typedef struct
{
	char name[20];	//������
	Field field[PRO_NUM];	//�������
	unsigned int field_num;	//��������
	unsigned int key_num;	//��������
	unsigned int textline;	//��¼�ж���
	int fieldLen;	//���Գ���
	char buf[1024];
	INDEX* pif;
	int  stateS;	//������״̬
	bool stateX;	//������״̬
} Table;	//��ṹ





bool CreateTable(int str_num);
bool ShowFileIndex(char *fname);
//bool InsertNode(slink* sl,int app);
//int GetNode(slink *sl);
int CountRecordLen(Table *t,int end_no);
//bool initIndex(slink *sl,int key_num);
#endif