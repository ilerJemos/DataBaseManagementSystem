#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Gloab.h"
#include "CreateTable.h"
#include "index.h"



void showMess(char ch[][20],int len);
bool RecordFile(char* fname);
bool InitTable(Table* t,int str_num);

//----------------------------------------------------------------
//创建表
bool CreateTable(int str_num)
{

    FILE* fp;
	INDEX ifile;
	Table t;	//表
//	int str_num = getString(ch,src,str);
	if(InitTable(&t,str_num))	//初始化表？？？
	{
		printf("initiate table successfully!\n");
	}
	else
	{
		printf("Exist errors in initiating table!\n");
		return false;
	}
	if(initIndex(&ifile,t.key_num,2))//???已有万不得已改动。初始化索引
	{
		printf("initiate index successfully!\n");
	}
	else
	{
		printf("Exist errors in initiating table!\n");
		return false;
	}

	

	fp = fopen(t.name,"wb");	//打开表的文件，每个表是一个文件
	
	fwrite(&t,sizeof(char),sizeof(Table),fp);	//把现在表里内容写到文件里
	fclose(fp);
	
	char idfname[25];
	strcpy(idfname,t.name);
	strcat(idfname,"_id");
	fp = fopen(idfname,"wb");

	fwrite(&ifile,sizeof(char),sizeof(INDEX),fp);	//写到索引里
	fclose(fp);

	if(!RecordFile(t.name))
		return false;
	
	return true;
}




//这个东西也没卵用，写了也没用过
void showMess(char ch[][20],int len)
{
	for(int i=0;i<len;i++)
	{
		printf("%s\n",ch[i]);
	}
}

//把文件（表）的修改记录写到r_filename文件里
bool RecordFile(char* fname)
{
    FILE* fp = fopen("r_filename","a");
	if(fp == NULL)
		return false;
	else
	{
		fputs(fname,fp);
		fputs("\n",fp);
	}
	fclose(fp);
	return true;
}

//显示索引
bool ShowIndex(INDEX *pif)
{
	int i = 0;
	printf("\tindex\n");

	printf("keyword num:%d\t\n",pif->key_num);

	if(pif->key_num == 1)
	{
		for(;i<pif->trecord;i++)
		{
			printf("%s  %d\n",pif->node[i].keyWord[0],pif->node[i].rec_num);
		}
	}
	else
	{
		for(;i<pif->trecord;i++)
		{
			printf("%s  ",pif->node[i].keyWord[0]);
			printf("%s  %d\n",pif->node[i].keyWord[1],pif->node[i].rec_num);
		}
	}
	printf("\n");

	return true;
	
}

//显示文件（表）索引，文件名_id就是表的索引
bool ShowFileIndex(char *fname)
{
	FILE* fp;
	INDEX ifile;
	char *ch = "_id";
	strcat(fname,ch);
	fp = fopen(fname,"rb");
	if(fp == NULL)
	{
		printf("%s file can't be open!\n",fname);
		return false;
	}
	
	fread(&ifile,sizeof(char),sizeof(INDEX),fp);
	ShowIndex(&ifile);
	fclose(fp);
	return true;
}



//初始化表
bool InitTable(Table* t,int str_num)        //不好 
{
	bool keyf = false;	//有没有主键
	t->textline = 0;	//表记录
	t->key_num = 0;	//表属性（列）
	
	if(SearchFile(ch[0]))	//看看有没有这个文件
	{
		printf("this file has alreadly exists!\n");
		return false;
	}

	if(str_num < 1)
		return  false;
	strcpy(t->name,ch[0]);
	int i = 1;
	int j = 0; 

	while(i < str_num)
	{
		strcpy(t->field[j].name,ch[i]);
		i = i+1;
		if(strcmp("char",ch[i])==0)		//属性的值是char
		{
			i = i+1;
			t->field[j].datatype = 1;
			t->field[j].length = atoi(ch[i]);
			i = i+1;
		}
		else if(strcmp("int",ch[i])==0)		//属性的值是int
		{
			i = i+1;
			t->field[j].datatype = 2;
			t->field[j].length = 4;

		}
		else if(strcmp("float",ch[i])==0)		//属性的值是float
		{
			i = i+1;
			t->field[j].datatype = 3;
			t->field[j].length = 8;
		}

		if(strcmp("pkey",ch[i]) == 0)		//属性的值是pkey（主键）
		{
			i++;
			t->field[j].pkey = true;
			t->key_num++;
			keyf  = true;
		}
		else
			t->field[j].pkey = false;
		
		j++;	
	}
	if(keyf == false)
	{
		printf("创建的表中没有申明关键字,第一个将被默认为关键子属性!\n");
		t->field[0].pkey = true;
	}
	t->field_num = j;
	t->fieldLen = CountRecordLen(t,t->field_num);
	t->stateS = 0;                     //处于空闲状态
	t->stateX = 0;
	return true;

}

//记录的长度
int CountRecordLen(Table *t,int end_no)
{
	int i = 0;
	int len = 0;
	for(i=0;i<end_no;i++)
	{
		if(t->field[i].datatype == 1)
			len += t->field[i].length;
		else if(t->field[i].datatype == 2)
			len += 4;
		else 
			len += 8;
	}

	return len;
}	
