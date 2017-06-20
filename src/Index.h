#ifndef _INDEX_H_
#define _INDEX_H_

#define KEY_NUM   2
#define INDEX_NUM 20

//#include "CreateTable.h"


typedef struct
{
	char keyWord[KEY_NUM][20];	//关键字（最多2个，每个名字20个字符）
	int  rec_num;		//记录的数量
}INODE;	//索引节点

typedef struct 
{
	INODE node[INDEX_NUM];	//索引节点数（最多20个）
    int  key_num;	//列（属性）的数量
	int  trecord;     //总共记录
	int  ktype[KEY_NUM];
}INDEX;	//索引


bool initIndex(INDEX* id,int knum,int keytype);

bool addInode(INDEX* id,int rn,char key[]);

int bSearch(INDEX* id,char key[],int start_no);

bool delARecord(INDEX* id,int delno);


#endif



