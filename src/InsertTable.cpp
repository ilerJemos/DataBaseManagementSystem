#include <stdio.h>
#include <string.h>

#include "CreateTable.h"
#include "Gloab.h"
#include "Index.h"

//bool CheckRecord(Table* t,slink* sl,char* buf);
bool CheckRecord(INDEX* pif,char* buf);
bool isIncludeCh(char* str);

//向表中插入数据
bool InsertTable(int str_num)
{
	FILE* fp1;
	FILE* fp2;
	char ifname[25];
	int flag = false;
	int keyLen = 0;
	int i = 0;
	int j = 0;
	char buf[50];
	int len = 0;
	if(!SearchFile(ch[0]))
	{
		printf("don't exist this file!\n");
		return false;
	}

	fp1 = fopen(ch[0],"rb+");		
	if(fp1 == NULL)
	{
		printf("%s Can't be open!\n",ch[0]);
		return false;
	}
	fseek(fp1,0,SEEK_SET);
	Table t;
	fread(&t,sizeof(char),sizeof(Table),fp1);
	if((str_num-2) != t.field_num)
	{
		printf("insert data error!\n");
		return false;
	}
	

	
	strcpy(ifname,ch[0]);
	strcat(ifname,"_id");
	fp2 = fopen(ifname,"rb+");
	if(fp2 == NULL)
	{
		printf("%s can't be open!\n",t.name);
		return false;
	}
	INDEX ifile;

	//printf("申请互斥琐\n");
	while(!lockX(t.name));

	printf(ch[0]);
	printf("读取文件中");
	//printProcess("读取文件中");
	printf("完成!\n");
	fread(&ifile,sizeof(char),sizeof(INDEX),fp2);


	
	char keyword[20];
	strcpy(buf,"");
	
	for(i=0;i<t.field_num;i++)
	{
		if(t.field[i].pkey)	//如果是主键，大概是要加到索引里吧
		{
			j++;
			strcpy(keyword+keyLen,ch[i+2]);
			keyLen = strlen(keyword)+1;
		}
		if(t.field[i].datatype == 1)	//char
		{
			
			if(strlen(ch[i+2]) > t.field[i].length)
			{
				printf("插入的%s字段的格式不对!\n",t.field[i].name);
				goto retn;
			}
			strcpy(buf+len,"");
			strcat(buf+len,ch[i+2]);
			len += (t.field[i].length+1);
		}
		else if(t.field[i].datatype == 2)	//int
		{
			if((strlen(ch[i+2]) > 4)||isIncludeCh(ch[i+2]))
			{
				printf("插入的%s字段的格式不对!\n",t.field[i].name);
				goto retn;	
			}
			strcpy(buf+len,"");
			strcat(buf+len,ch[i+2]);
			len += (4+1);
		}
		else	//float
		{	
			if((strlen(ch[i+2]) > 8)||isIncludeCh(ch[i+2]))
			{
				printf("插入的%s字段的格式不对!\n",t.field[i].name);
				goto retn;	
			}
			strcpy(buf+len,"");
			strcat(buf+len,ch[i+2]);
			len += (8+1);
		}
	}
							//a record 

	if(!CheckRecord(&ifile,keyword))	//如果没有输入主键的值
	 {
		printf("Errors in CheckRecord!\n");
		goto retn;
	 }

	fseek(fp2,0,SEEK_SET);

	if(!addInode(&ifile,t.textline,keyword))	//发生了什么错误吧
	{
		printf("Exist errors in InsertNode!\n");
		goto retn;
	}
	t.textline ++;	//插入成功了
	

	printf(t.name);
	printf("文件插入进行中\n");
	//printProcess("文件插入进行中");
	printf("完成!\n");
	fseek(fp2,0,SEEK_SET);
	fwrite(&ifile,sizeof(char),sizeof(INDEX),fp2);	//应该是写到索引里了
	fclose(fp2);

	fseek(fp1,0,SEEK_SET);
	t.stateX = 1;  //没有办法了啊.
	fwrite(&t,sizeof(char),sizeof(Table),fp1);	//真正写到文件里了吧
	                           
	fseek(fp1,0,SEEK_END);
	fwrite(buf,sizeof(char),len,fp1);
	fclose(fp1);
	flag = true;

retn:	//goto的点

	//printf("释放互斥琐\n");
	if(!unlockX(t.name))
		printf("系统有错误!\n");
	if(!flag)
		return false;
	else
		return true;

}

//------------------------------------------
/*bool CheckKeyName(slink* sl,char keyname[],int num)		//check keyname!num:第几个
{
	int head = sl->node[sl->rear].next;
	int thead = head;
	int cur = 0;
	while(sl->node[thead].next != head)
	{
		cur = sl->node[thead].next;
		if(strcmp(sl->node[cur].key[num],keyname) == 0)
		{
			return false;
		}
		else
		{
			thead = cur;
		}		
	}
	
	return true;
}

int GetKeyNum(Table* t,int key)					//得到key后字段的是主键的数组序号
{
	int i = 0;
	for(i=key+1;i<t->field_num;i++)
	{
		if(t->field[i].pkey)
		{
			return i;
		}
	}
	return -1;
}
*/

//对主键重复的处理。buf是插入的记录，做的不是太好,有待改善
bool CheckRecord(INDEX* pif,char* buf)
{
	int p = bSearch(pif,buf,0);
	if(strcmp(pif->node[p].keyWord[0],buf) == 0)
	{
		return false;
	}
	else
		return true;

}

//判断是不是字母
bool isIncludeCh(char* str)
{
	int len = strlen(str);
	int i = 0;
	for(i=0;i<len;i++)
	{
		if((('a'<=str[i])&&(str[i]<='z'))||(('A'<=str[i])&&(str[i]<='Z')))
			return true;
	}

	return false;
}

