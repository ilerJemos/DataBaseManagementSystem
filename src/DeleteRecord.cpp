#include <stdio.h>
#include <string.h>
#include "CreateTable.h"
#include "Gloab.h"
#include "SelectTable.h"
#include "Index.h"

//删除信息
bool DeleteRecord(int strlen)
{
	FILE* fp;
	FILE* ifp;
	char buf[1024];
	int operatortype = 0;	//操作类型
	if(!SearchFile(ch[0]))	//看看文件存不存在
	{
		printf("%s table doesn't exist!",ch[0]);
		return false;
	}

	//打开这个文件（表）
	fp = fopen(ch[0],"rb+");
	if(fp == NULL)
	{
		printf("Errors exists in file %s\n",ch[0]);
		return false;
	}

	//打开这个表的_id文件，索引文件
	char ifname[30];
	strcpy(ifname,ch[0]);
	strcat(ifname,"_id");
	ifp = fopen(ifname,"rb+");
	if(ifp == NULL)
	{
		printf("Errors exist in file %s!\n",ifname);
		return false;
	}


	INDEX ifile;
	Table t;
	fread(&t,sizeof(char),sizeof(Table),fp);
	int result = 0;
    //printf("申请互斥琐\n");
	//while(!lockX(t.name));

	printf(t.name);
	printf("文件读取中\n");
	printf("完成!");
	fread(&ifile,sizeof(char),sizeof(INDEX),ifp);
	if(strlen == 1)
	{
		//fread(&ifile,sizeof(char),sizeof(INDEX),ifp);
		
		initIndex(&ifile,ifile.key_num,2);	//初始化索引
		//printf("申请互斥琐\n");
		//while(!lockX(t.name));
		fwrite(&ifile,sizeof(char),sizeof(INDEX),ifp);		//把修改后的在写回去
		fp = fopen(ch[2],"bw");
		t.textline = 0;
		fwrite(&t,sizeof(char),sizeof(Table),fp);
		//printf("释放互斥琐\n");
		///if(!unlockX(t.name))
			//printf("系统有错误!\n");
	}
	else
	{
		operatortype = SelectFieldType(ch[3]);
		int field_no = GetFileNo(&t,ch[2]);
		if(field_no == -1)
		{
			printf("%s Field don't exists in Table %s\n",ch[2],ch[0]);
			return false;
		}
		

		//while(result != sl.node[sl.rear].next)
		//{
		fseek(fp,sizeof(Table),SEEK_SET);
		fread(buf,sizeof(char),1024,fp);
		int fieldlen = CountRecordLen(&t,t.field_num);
		result = 0;
		if(t.field[field_no].pkey)	//如果是主键
		{
			result = SelectTableByIndex(&ifile,&t,ch[4],t.field[field_no].datatype,operatortype,result);	//从索引中寻找表（记录）
			if(result == -1)
			{
				printf("%s file don't exist this record!\n",ch[0]);
				return false;
			}
			while(result != -1)
			{
				//fseek(fp,sizeof(Table),SEEK_SET);
				//fseek(fp,ifile.node[result].rec_num*(fieldlen+t.field_num),SEEK_CUR);
				//fread(record,sizeof(char),fieldlen+t.field_num,fp);
				ShowRecord(buf+ifile.node[result].rec_num*(fieldlen+t.field_num),&t);	//显示要删除的记录
				printf("deleted!\n");
				delARecord(&ifile,result);	//删除记录
				result = SelectTableByIndex(&ifile,&t,ch[4],t.field[field_no].datatype,operatortype,result+1);
			}
		}
		else	//如果不是主键
		{
			result = SelectTableByFile(&ifile,&t,ch[4],t.field[field_no].datatype,operatortype,field_no,buf,result);
			if(result == -1)
			{
				printf("%s file don't exist this record!\n",ch[0]);
				return false;
			}
			while(result != -1)
			{
				//fseek(fp,sizeof(Table),SEEK_SET);
				//fread(buf,sizeof(char),20*20,fp);
				ShowRecord(buf+ifile.node[result].rec_num*(fieldlen+t.field_num),&t);
				printf("deleted!\n");
				delARecord(&ifile,result);
				result = SelectTableByFile(&ifile,&t,ch[4],t.field[field_no].datatype,operatortype,field_no,buf,result+1);
			}
		//	result = SelectTableConditon(&t,&sl,ch,field_no,sl.node[result].next);
		}
		//}

		//吧删除后结果的在协会文件中
		printf(t.name);
		printf("文件写入中");
		printf("完成\n");
		fseek(ifp,0,SEEK_SET);
		fwrite(&ifile,sizeof(char),sizeof(INDEX),ifp);
	}
	fclose(fp);
	fclose(ifp);
	//printf("释放互斥琐\n");
	//if(!unlockX(t.name))
		//printf("系统有错误!\n");
	return true;
}