
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <dos.h> 
#include "CreateTable.h"
#include "Index.h"
#include "Gloab.h"
#include "UpdateTable.h"
#include "SelectTable.h"


bool updateRecord(Uif* pu,char* buf);
bool getUif(Uif* pu,char ch[][20],int strlen,Table* pt);


void updateAll(Uif* pu,char* buf,INDEX* pif,Table* pt);

bool UpdateTable(int strlen)
{
	FILE* fp;
	FILE* ifp;
	int i = 0;
	int field_no;
	
	char buf[1024];
	Uif u;

	int operatortype = 0;

	//printf("申请互斥琐");					/////
	while(!lockX(ch[0]));
	showLocalTime();

	//看表是否存在
	if(!SearchFile(ch[0]))
	{
		printf("%s table doesn't exist!",ch[0]);
		return false;
	}

	//表所在的文件
	fp = fopen(ch[0],"rb+");
	if(fp == NULL)
	{
		printf("Errors exists in file %s\n",ch[0]);
		return false;
	}

	//对应的索引文件
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

	int fieldlen = CountRecordLen(&t,t.field_num);
	int result = 0;
	if(strcmp(ch[1],"set") != 0)	//输入的语句中没有set
	{
		printf("update 操作的格式有误!\n");
		return false;
	}
	if(!getUif(&u,ch,strlen,&t))	//没有找到要设置的值
	{
		printf("update 设定的值有错误!\n");
		return false;
	}

	//从文件中读取表
	printf(t.name);
	printProcess("读取中");
	printf("完成!\n");
	fread(buf,sizeof(char),1024,fp);
	fread(&ifile,sizeof(char),sizeof(INDEX),ifp);


	int pos = getPosofStr(ch,"where",strlen);
	if(pos == -1)	//如果没有where条件就更新所有记录
	{
		updateAll(&u,buf,&ifile,&t);
		goto save;
	}
	operatortype = SelectFieldType(ch[pos+2]);
	field_no = GetFileNo(&t,ch[pos+1]);	//寻找记录在表中的位置
	if(field_no == -1)
	{
		printf("%s Field don't exists in Table %s\n",ch[i+1],ch[0]);
		return false;
	}
		

	result = 0;
	if(t.field[field_no].pkey)
	{
		//找到要修改的记录
		result = SelectTableByIndex(&ifile, &t, ch[pos+3], t.field[field_no].datatype, operatortype, result);
		if(result == -1)
		{
			printf("%s 文件不含有要修改的记录!\n",ch[0]);
			return false;
		}
		while(result != -1)
		{
			ShowRecord(buf+ifile.node[result].rec_num*(fieldlen+t.field_num),&t);	//显示要修改的记录
			printf("update!\n");
			updateRecord(&u,buf+ifile.node[result].rec_num*(fieldlen+t.field_num));  // 更新记录（索引表没有考虑）
			result = SelectTableByIndex(&ifile, &t, ch[pos+3], t.field[field_no].datatype, operatortype, result+1);
		}
	}
	else
	{
		result = SelectTableByFile(&ifile,&t,ch[pos+3],t.field[field_no].datatype,operatortype,field_no,buf,result);
		if(result == -1)
		{
			printf("%s 文件不含有要修改的记录!\n",ch[0]);
			return false;
		}
		while(result != -1)
		{
				
			ShowRecord(buf+ifile.node[result].rec_num*(fieldlen+t.field_num),&t);
			updateRecord(&u,buf+ifile.node[result].rec_num*(fieldlen+t.field_num));
			result = SelectTableByFile(&ifile,&t,ch[pos+3],t.field[field_no].datatype,operatortype,field_no,buf,result+1);
		}
		
	}
save:	


	//把修改后的表写回文件中
	printf(t.name);
	printProcess("文件写入中"); 
	printf("完成!\n");

	fseek(ifp,0,SEEK_SET);
	fwrite(&ifile,sizeof(char),sizeof(INDEX),ifp);
	fseek(fp,sizeof(Table),SEEK_SET);
	fwrite(buf,sizeof(char),ifile.trecord*(fieldlen+t.field_num),fp);
	fclose(fp);
	fclose(ifp);
	
	
	//printf("释放互斥琐"); 
	if(!unlockX(t.name))
		printf("系统有错误!\n");
	showLocalTime();



	return true;
}




//寻找dest在ch中的位置
int getPosofStr(char ch[][20],char* dest,int strlen)
{
	int i = 0;
	for(i=0;i<strlen;i++)
	{
		if(strcmp(ch[i],dest) == 0)
			return i;
	}
	return -1;
}

//----------------------------------------------
//获取要设置的属性（还是数值）
bool getUif(Uif* pu,char ch[][20],int strlen,Table* pt)
{
	int i = 2;
	int fieldNo = -1;
	int j = 0;
		
	for(;i<strlen;i++)
	{
		if(strcmp(ch[i],"where") == 0)	//如果是where就退出
			break;
		fieldNo = GetFileNo(pt,ch[i]);	//获取表的编号
		if(fieldNo == -1)
			return false;
		else
		{
			 pu->uf[j].dis = getFieldDis(fieldNo,pt);
			 pu->uf[j].datatype = pt->field[fieldNo].datatype;
			 i = i + 2;
			 if(GetFileNo(pt,ch[i])!= -1)
			 {
				 i++;
				 if(strcmp(ch[i],"+") == 0)	//是+
					 pu->uf[j].kind = 1;
				 else if(strcmp(ch[i],"-") == 0 )	//是-
					 pu->uf[j].kind = 2;
				 else
					 return false;
				 i++;
			 }
			 else
			 {
				pu->uf[j].kind = 0;
			 }
			 
			 strcpy(pu->uf[j].cons,ch[i]);
			 j++;
		}

	}
	if(j == 0)
		return false;
	else
		pu->num = j;
	return true;
}


//--------------------------------------------------
//更新一条记录
bool updateRecord(Uif* pu,char* buf)
{
	int i = 0;
	int it;
	float ft;


	for(i=0;i<pu->num;i++)
	{
		if(pu->uf[i].kind == 0)	//直接设置
			strcpy(buf+pu->uf[i].dis,pu->uf[i].cons);
		else if(pu->uf[i].kind == 1)	//加
		{
			if(pu->uf[i].datatype == 1)	//加法不能是char
				return false;
			else if(pu->uf[i].datatype == 2)
			{
				it = atoi(buf+pu->uf[i].dis)+atoi(pu->uf[i].cons);
				itoa(it,pu->uf[i].cons,10);
			}
			else if(pu->uf[i].datatype == 3)
			{
				it = atoi(buf+pu->uf[i].dis)+atoi(pu->uf[i].cons);
				itoa(it,pu->uf[i].cons,10);
			}

			strcpy(buf+pu->uf[i].dis,pu->uf[i].cons);
		}
		else if(pu->uf[i].kind == 2)	//减
		{
			if(pu->uf[i].datatype == 1)
				return false;
			else if(pu->uf[i].datatype == 2)
			{
				it = atoi(buf+pu->uf[i].dis)-atoi(pu->uf[i].cons);
				itoa(it,pu->uf[i].cons,10);
			}
			else if(pu->uf[i].datatype == 3)
			{
				it = atoi(buf+pu->uf[i].dis)-atoi(pu->uf[i].cons);
				itoa(it,pu->uf[i].cons,10);
			}

			strcpy(buf+pu->uf[i].dis,pu->uf[i].cons);
			
		}
		else
			return false;
	}
	return true;
}
//------------------------------------------------------
//更新所有记录
void updateAll(Uif* pu,char* buf,INDEX* pif,Table* pt)
{
	int trecord = pif->trecord;
	int i = 0;
	for(i=0;i<trecord;i++)
	{
		updateRecord(pu, buf+pif->node[i].rec_num*(pt->fieldLen+pt->field_num));
	}
	
}
