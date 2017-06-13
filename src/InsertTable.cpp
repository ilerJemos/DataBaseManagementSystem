#include <stdio.h>
#include <string.h>

#include "CreateTable.h"
#include "Gloab.h"
#include "Index.h"

//bool CheckRecord(Table* t,slink* sl,char* buf);
bool CheckRecord(INDEX* pif,char* buf);
bool isIncludeCh(char* str);

//����в�������
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

	//printf("���뻥����\n");
	while(!lockX(t.name));

	printf(ch[0]);
	printf("��ȡ�ļ���");
	//printProcess("��ȡ�ļ���");
	printf("���!\n");
	fread(&ifile,sizeof(char),sizeof(INDEX),fp2);


	
	char keyword[20];
	strcpy(buf,"");
	
	for(i=0;i<t.field_num;i++)
	{
		if(t.field[i].pkey)	//����������������Ҫ�ӵ��������
		{
			j++;
			strcpy(keyword+keyLen,ch[i+2]);
			keyLen = strlen(keyword)+1;
		}
		if(t.field[i].datatype == 1)	//char
		{
			
			if(strlen(ch[i+2]) > t.field[i].length)
			{
				printf("�����%s�ֶεĸ�ʽ����!\n",t.field[i].name);
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
				printf("�����%s�ֶεĸ�ʽ����!\n",t.field[i].name);
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
				printf("�����%s�ֶεĸ�ʽ����!\n",t.field[i].name);
				goto retn;	
			}
			strcpy(buf+len,"");
			strcat(buf+len,ch[i+2]);
			len += (8+1);
		}
	}
							//a record 

	if(!CheckRecord(&ifile,keyword))	//���û������������ֵ
	 {
		printf("Errors in CheckRecord!\n");
		goto retn;
	 }

	fseek(fp2,0,SEEK_SET);

	if(!addInode(&ifile,t.textline,keyword))	//������ʲô�����
	{
		printf("Exist errors in InsertNode!\n");
		goto retn;
	}
	t.textline ++;	//����ɹ���
	

	printf(t.name);
	printf("�ļ����������\n");
	//printProcess("�ļ����������");
	printf("���!\n");
	fseek(fp2,0,SEEK_SET);
	fwrite(&ifile,sizeof(char),sizeof(INDEX),fp2);	//Ӧ����д����������
	fclose(fp2);

	fseek(fp1,0,SEEK_SET);
	t.stateX = 1;  //û�а취�˰�.
	fwrite(&t,sizeof(char),sizeof(Table),fp1);	//����д���ļ����˰�
	                           
	fseek(fp1,0,SEEK_END);
	fwrite(buf,sizeof(char),len,fp1);
	fclose(fp1);
	flag = true;

retn:	//goto�ĵ�

	//printf("�ͷŻ�����\n");
	if(!unlockX(t.name))
		printf("ϵͳ�д���!\n");
	if(!flag)
		return false;
	else
		return true;

}

//------------------------------------------
/*bool CheckKeyName(slink* sl,char keyname[],int num)		//check keyname!num:�ڼ���
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

int GetKeyNum(Table* t,int key)					//�õ�key���ֶε����������������
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

//�������ظ��Ĵ���buf�ǲ���ļ�¼�����Ĳ���̫��,�д�����
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

//�ж��ǲ�����ĸ
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

