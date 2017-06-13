#include <stdio.h>
#include <string.h>
#include "CreateTable.h"
#include "Gloab.h"
#include "SelectTable.h"
#include "Index.h"

//ɾ����Ϣ
bool DeleteRecord(int strlen)
{
	FILE* fp;
	FILE* ifp;
	char buf[1024];
	int operatortype = 0;	//��������
	if(!SearchFile(ch[0]))	//�����ļ��治����
	{
		printf("%s table doesn't exist!",ch[0]);
		return false;
	}

	//������ļ�����
	fp = fopen(ch[0],"rb+");
	if(fp == NULL)
	{
		printf("Errors exists in file %s\n",ch[0]);
		return false;
	}

	//��������_id�ļ��������ļ�
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
    //printf("���뻥����\n");
	//while(!lockX(t.name));

	printf(t.name);
	printf("�ļ���ȡ��\n");
	printf("���!");
	fread(&ifile,sizeof(char),sizeof(INDEX),ifp);
	if(strlen == 1)
	{
		//fread(&ifile,sizeof(char),sizeof(INDEX),ifp);
		
		initIndex(&ifile,ifile.key_num,2);	//��ʼ������
		//printf("���뻥����\n");
		//while(!lockX(t.name));
		fwrite(&ifile,sizeof(char),sizeof(INDEX),ifp);		//���޸ĺ����д��ȥ
		fp = fopen(ch[2],"bw");
		t.textline = 0;
		fwrite(&t,sizeof(char),sizeof(Table),fp);
		//printf("�ͷŻ�����\n");
		///if(!unlockX(t.name))
			//printf("ϵͳ�д���!\n");
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
		if(t.field[field_no].pkey)	//���������
		{
			result = SelectTableByIndex(&ifile,&t,ch[4],t.field[field_no].datatype,operatortype,result);	//��������Ѱ�ұ���¼��
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
				ShowRecord(buf+ifile.node[result].rec_num*(fieldlen+t.field_num),&t);	//��ʾҪɾ���ļ�¼
				printf("deleted!\n");
				delARecord(&ifile,result);	//ɾ����¼
				result = SelectTableByIndex(&ifile,&t,ch[4],t.field[field_no].datatype,operatortype,result+1);
			}
		}
		else	//�����������
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

		//��ɾ����������Э���ļ���
		printf(t.name);
		printf("�ļ�д����");
		printf("���\n");
		fseek(ifp,0,SEEK_SET);
		fwrite(&ifile,sizeof(char),sizeof(INDEX),ifp);
	}
	fclose(fp);
	fclose(ifp);
	//printf("�ͷŻ�����\n");
	//if(!unlockX(t.name))
		//printf("ϵͳ�д���!\n");
	return true;
}