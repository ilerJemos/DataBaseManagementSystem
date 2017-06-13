#ifndef _INDEX_H_
#define _INDEX_H_

#define KEY_NUM   2
#define INDEX_NUM 20

//#include "CreateTable.h"


typedef struct
{
	char keyWord[KEY_NUM][20];	//�ؼ��֣����2����ÿ������20���ַ���
	int  rec_num;		//��¼������
}INODE;	//�����ڵ�

typedef struct 
{
	INODE node[INDEX_NUM];	//�����ڵ��������20����
    int  key_num;	//�У����ԣ�������
	int  trecord;     //�ܹ���¼
	int  ktype[KEY_NUM];
}INDEX;	//����


bool initIndex(INDEX* id,int knum,int keytype);

bool addInode(INDEX* id,int rn,char key[]);

int bSearch(INDEX* id,char key[],int start_no);

bool delARecord(INDEX* id,int delno);


#endif



