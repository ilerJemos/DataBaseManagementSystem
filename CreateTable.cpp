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
//������
bool CreateTable(int str_num)
{

    FILE* fp;
	INDEX ifile;
	Table t;	//��
//	int str_num = getString(ch,src,str);
	if(InitTable(&t,str_num))	//��ʼ��������
	{
		printf("initiate table successfully!\n");
	}
	else
	{
		printf("Exist errors in initiating table!\n");
		return false;
	}
	if(initIndex(&ifile,t.key_num,2))//???�����򲻵��ѸĶ�����ʼ������
	{
		printf("initiate index successfully!\n");
	}
	else
	{
		printf("Exist errors in initiating table!\n");
		return false;
	}

	

	fp = fopen(t.name,"wb");	//�򿪱���ļ���ÿ������һ���ļ�
	
	fwrite(&t,sizeof(char),sizeof(Table),fp);	//�����ڱ�������д���ļ���
	fclose(fp);
	
	char idfname[25];
	strcpy(idfname,t.name);
	strcat(idfname,"_id");
	fp = fopen(idfname,"wb");

	fwrite(&ifile,sizeof(char),sizeof(INDEX),fp);	//д��������
	fclose(fp);

	if(!RecordFile(t.name))
		return false;
	
	return true;
}




//�������Ҳû���ã�д��Ҳû�ù�
void showMess(char ch[][20],int len)
{
	for(int i=0;i<len;i++)
	{
		printf("%s\n",ch[i]);
	}
}

//���ļ��������޸ļ�¼д��r_filename�ļ���
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

//��ʾ����
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

//��ʾ�ļ������������ļ���_id���Ǳ������
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



//��ʼ����
bool InitTable(Table* t,int str_num)        //���� 
{
	bool keyf = false;	//��û������
	t->textline = 0;	//���¼
	t->key_num = 0;	//�����ԣ��У�
	
	if(SearchFile(ch[0]))	//������û������ļ�
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
		if(strcmp("char",ch[i])==0)		//���Ե�ֵ��char
		{
			i = i+1;
			t->field[j].datatype = 1;
			t->field[j].length = atoi(ch[i]);
			i = i+1;
		}
		else if(strcmp("int",ch[i])==0)		//���Ե�ֵ��int
		{
			i = i+1;
			t->field[j].datatype = 2;
			t->field[j].length = 4;

		}
		else if(strcmp("float",ch[i])==0)		//���Ե�ֵ��float
		{
			i = i+1;
			t->field[j].datatype = 3;
			t->field[j].length = 8;
		}

		if(strcmp("pkey",ch[i]) == 0)		//���Ե�ֵ��pkey��������
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
		printf("�����ı���û�������ؼ���,��һ������Ĭ��Ϊ�ؼ�������!\n");
		t->field[0].pkey = true;
	}
	t->field_num = j;
	t->fieldLen = CountRecordLen(t,t->field_num);
	t->stateS = 0;                     //���ڿ���״̬
	t->stateX = 0;
	return true;

}

//��¼�ĳ���
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
