
#include <stdio.h>
#include <string.h>
#include  "CreateTable.h"
#include  "Index.h"
#include  "Gloab.h"
#include  "SelectTable.h"
#include  "stdlib.h"

bool setDif(INDEX* pif1,INDEX* pif2,Table* pt,char buf1[],char buf2[]);
bool setCross(INDEX* pif1,INDEX* pif2,Table* pt,char buf1[],char buf2[]);
bool setMerge(INDEX* pif1,INDEX* pif2,Table* pt,char buf1[],char buf2[]);

//set操作
bool setOperate(int strlen)
{
	FILE* fp1;
	FILE* fp2;
	FILE* ifp1;
	FILE* ifp2;
	INDEX ifile1;
	INDEX ifile2;
	Table t1;
	Table t2;
	char ifname1[30];
	char ifname2[30];
	char buf1[1024];
	char buf2[1024];
	if(strlen != 3)
	{
		printf("Inputing data is error!\n");
		return false;
	}
	fp1 = fopen(ch[1],"rb");
	if(fp1 == NULL)
	{
		printf("%s file can't be opened!\n",ch[1]);
		return false;
	}
	fp2 = fopen(ch[2],"rb");
	if(fp2 == NULL)
	{
		printf("%s file can't be opened!\n",ch[2]);
		return false;
	}

	strcpy(ifname1,ch[1]);
	strcat(ifname1,"_id");
	strcpy(ifname2,ch[2]);
	strcat(ifname2,"_id");
	ifp1 = fopen(ifname1,"rb");

	if(ifp1 == NULL)
	{
		printf("%s file can't be opened!\n",ch[1]);
		return false;	
	}
	ifp2 = fopen(ifname2,"rb");
	if(ifp2 == NULL)
	{
		printf("%s file can't be opened!\n",ch[2]);
		return false;		
	}
	fread(&t1,sizeof(char),sizeof(Table),fp1);
	fread(&t2,sizeof(char),sizeof(Table),fp2);
	fread(&ifile1,sizeof(char),sizeof(INDEX),ifp1);
	fread(&ifile2,sizeof(char),sizeof(INDEX),ifp2);
	fread(buf1,sizeof(char),1024,fp1);
	fread(buf2,sizeof(char),1024,fp2);
	
	int fieldlen = CountRecordLen(&t1,t1.field_num);
	int rlen = fieldlen + t1.field_num;
	if( strcmp(ch[0],"dif") ==0)
		setDif(&ifile1,&ifile2,&t1,buf1,buf2);
	else if(strcmp(ch[0],"cross") == 0)
		setCross(&ifile1,&ifile2,&t1,buf1,buf2);
	else if(strcmp(ch[0],"merge") == 0)
		setMerge(&ifile1,&ifile2,&t1,buf1,buf2);
	else 
	{
		printf(" command error!\n");
		return false;
	}

	fclose(fp1);
	fclose(fp2);
	fclose(ifp1);
	fclose(ifp2);
	return true;
}

//设置数据交换
bool setDif(INDEX* pif1,INDEX* pif2,Table* pt,char buf1[],char buf2[])
{
	int i1 = 0;
	int i2 = 0;
	int fieldlen = CountRecordLen(pt,pt->field_num);
	if(pif1->ktype[0] == 1)
	{
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			if(strcmp(pif1->node[i1].keyWord[0],pif2->node[i2].keyWord[0])==0)
			{
				i1++;
				i2++;
			}
			else if(strcmp(pif1->node[i1].keyWord[0],pif2->node[i2].keyWord[0])== -1)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
			}
			else
			{
				i2++;
			}
		}
	}

	else if(pif1->ktype[0] == 2)
	{
		int t1 = 0;
		int t2 = 0;
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			t1 = atoi(pif1->node[i1].keyWord[0]);
			t2 = atoi(pif2->node[i2].keyWord[0]);
			if(t1 == t2)
			{
				i1++;
				i2++;
			}
			else if(t1 < t2)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
			}
			else
			{
				i2++;
			}
		}
	
	}
	else
	{
		int f1 = 0;
		int f2 = 0;
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			f1 = atoi(pif1->node[i1].keyWord[0]);
			f2 = atoi(pif2->node[i2].keyWord[0]);
			if(f1 == f2)
			{
				i1++;
				i2++;
			}
			else if(f1 < f2)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
			}
			else
			{
				i2++;
			}
		}
	
	}
	if(i1 < pif1->trecord)
	{
		for(;i1<pif1->trecord;i1++)
			ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
	}
	return true;
}

//设置连接
bool setCross(INDEX* pif1,INDEX* pif2,Table* pt,char buf1[],char buf2[])
{
	
	int i1 = 0;
	int i2 = 0;
	int fieldlen = CountRecordLen(pt,pt->field_num);
	if(pif1->ktype[0] == 1)
	{
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			if(strcmp(pif1->node[i1].keyWord[0],pif2->node[i2].keyWord[0])==0)
			{
				
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
				i2++;
			}
			else if(strcmp(pif1->node[i1].keyWord[0],pif2->node[i2].keyWord[0])== -1)
			{
				
				i1++;
			}
			else
			{
				i2++;
			}
		}
	}

	else if(pif1->ktype[0] == 2)
	{
		int t1 = 0;
		int t2 = 0;
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			t1 = atoi(pif1->node[i1].keyWord[0]);
			t2 = atoi(pif2->node[i2].keyWord[0]);
			if(t1 == t2)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
				i2++;
			}
			else if(t1 < t2)
			{		
				i1++;
			}
			else
			{
				i2++;
			}
		}
	
	}
	else
	{
		int f1 = 0;
		int f2 = 0;
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			f1 = atoi(pif1->node[i1].keyWord[0]);
			f2 = atoi(pif2->node[i2].keyWord[0]);
			if(f1 == f2)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
				i2++;
				
			}
			else if(f1 < f2)
			{	
				i1++;
			}
			else
			{
				i2++;
			}
		}
	
	}
	return true;
}

//merge语句
bool setMerge(INDEX* pif1,INDEX* pif2,Table* pt,char buf1[],char buf2[])
{
	int i1 = 0;
	int i2 = 0;
	int fieldlen = CountRecordLen(pt,pt->field_num);
	if(pif1->ktype[0] == 1)
	{
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			if(strcmp(pif1->node[i1].keyWord[0],pif2->node[i2].keyWord[0])==0)
			{
				
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
				i2++;
			}
			else if(strcmp(pif1->node[i1].keyWord[0],pif2->node[i2].keyWord[0])== -1)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
			}
			else
			{
				ShowRecord(buf1+pif1->node[i2].rec_num*(fieldlen+pt->field_num),pt);
				i2++;
			}
		}
	}

	else if(pif1->ktype[0] == 2)
	{
		int t1 = 0;
		int t2 = 0;
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			t1 = atoi(pif1->node[i1].keyWord[0]);
			t2 = atoi(pif2->node[i2].keyWord[0]);
			if(t1 == t2)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
				i2++;
			}
			else if(t1 < t2)
			{		
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
			}
			else
			{
				ShowRecord(buf1+pif1->node[i2].rec_num*(fieldlen+pt->field_num),pt);
				i2++;
			}
		}
	
	}
	else
	{
		int f1 = 0;
		int f2 = 0;
		while((i1<pif1->trecord) && (i2<pif2->trecord))
		{
			f1 = atoi(pif1->node[i1].keyWord[0]);
			f2 = atoi(pif2->node[i2].keyWord[0]);
			if(f1 == f2)
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
				i2++;
			}
			else if(f1 < f2)
			{	
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i1++;
			}
			else
			{
				ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
				i2++;
			}
		}
	
	}
	
	if(i1<pif1->trecord)
	{
		for(;i1<pif1->trecord;i1++)
			ShowRecord(buf1+pif1->node[i1].rec_num*(fieldlen+pt->field_num),pt);
	}

	if(i2<pif2->trecord)
	{
		for(;i2<pif2->trecord;i2++)
			ShowRecord(buf2+pif2->node[i2].rec_num*(fieldlen+pt->field_num),pt);
	}
	return true;
}




