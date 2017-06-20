
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CreateTable.h"
#include "Index.h"
#include "Gloab.h"
#include "SelectTable.h"
#include "UpdateTable.h"

//ִ�в���
bool runTask(int authority)
{
	int option = 0;
	int dis = 0;
	char buf[100];
	char tName[10];
	char tbuf[1024];
	char exbuf[200];
	int  reclen[10];
	Table t;
	INDEX ifile;

	//��ʼ��log�ļ�
	FILE* lfp = fopen("log","r+");
	if(lfp == NULL)
	{
		printf("ϵͳ���Ի�log�ļ�����!\n");
		return false;
	}
	fseek(lfp,0,SEEK_END);
	scanf("%s",tName);
	strcpy(buf,"");
	strcpy(exbuf,"");
	strcat(buf,tName);
	strcat(buf," ");
	strcat(buf,"starts\n");
	fwrite(buf,sizeof(char),strlen(buf),lfp);
	fclose(lfp);
	int i = 0;
	int result = 0;
	int pos = 0;
	int proNum = 0;

	//�ظ�ִ��
	while(1)
	{
	    int strl = 0;
		lfp = fopen("log","r+");
		fseek(lfp,0,SEEK_END);
		strcpy(buf,"");
		option = selectFunction();
		if(option != 18 && option != 19)
			strl= getString(ch,1,NULL);
		if(option == 14)				//ѡ�����
		{
			//SelectTable(strl,NULL);
			strcat(exbuf,"select");
			strcat(exbuf," ");
			for(i=0;i<strl;i++)
			{
				strcat(exbuf,ch[i]);
				strcat(exbuf," ");
			}
			strcat(exbuf,";");
			reclen[proNum] = strlen(exbuf);
			proNum++;
		}
		else if(option == 11)	//���²���
		{
			
			strcat(buf,tName);
			strcat(buf," update ");
			for(i = 0;i<strl;i++)
			{
				strcat(buf,ch[i]);
				strcat(buf," ");
				if(strcmp(ch[i],"=") == 0)
					break;
			}
			FILE* fp = fopen(ch[0],"rb");
			if(fp == NULL)
			{
				printf("��������д���!\n");
				return false;
			}
			fread(&t,sizeof(char),sizeof(Table),fp);
			if(GetFileNo(&t,ch[4]) != -1)
			{

				char ifname[30];
				strcpy(ifname,ch[0]);
				strcat(ifname,"_id");
				FILE* ifp = fopen(ifname,"rb+");
				if(ifp == NULL)
				{
					printf("�������д���!\n");
					return false;
				}
				fread(tbuf,sizeof(char),1024,fp);
				fread(&ifile,sizeof(char),sizeof(INDEX),ifp);
				fclose(fp);
				fclose(ifp);
				pos = getPosofStr(ch,"where",strl);
				int operatortype = SelectFieldType(ch[pos+2]);
				int field_no = GetFileNo(&t,ch[pos+1]);

				result = SelectTableByFile(&ifile,&t,ch[pos+3],t.field[field_no].datatype,operatortype,field_no,tbuf,result);
				if(result == -1)
				{
					printf("�����û�н��!\n");
					return false;
				}
				int fieldNo = GetFileNo(&t,ch[2]);
				int dis = getFieldDis(fieldNo,&t);
				//strcat(buf,tbuf+ifile.node[result].rec_num*(t.fieldLen+t.field_num)+dis);
				//strcat(buf," ");
			
				int t1 = atoi(ch[6]);
				int t2 = atoi(tbuf+ifile.node[result].rec_num*(t.fieldLen+t.field_num)+dis);
				char temp[5];
				if(strcmp(ch[5],"+") == 0)
					itoa(t1+t2,temp,10);
				else if(strcmp(ch[5],"-") == 0)
					itoa(t2-t1,temp,10);
				strcat(buf,temp);
				strcat(buf," ");
			}
			else
			{
				strcat(buf,ch[3]);
				strcat(buf," ");
				pos = 4;
			}
			for(i=pos;i<strl;i++)
			{
				strcat(buf,ch[i]);
				strcat(buf," ");
			}
			strcat(buf,"\n");
			int len = strlen(buf);
			fwrite(buf,sizeof(char),len,lfp);
			fclose(lfp);
			printf("%s ������־�ļ���\n",buf);
			//UpdateTable(strlen);
			strcat(exbuf,"update ");
			for(i=0;i<strl;i++)
			{
				strcat(exbuf,ch[i]);
				strcat(exbuf," ");
			}
			strcat(exbuf,";");
			reclen[proNum] = strlen(exbuf);
			proNum++;
		}
		else if(option == 18)
		{
			printf("�������б��ж�!\n");
			getchar();
			return false;
		}
		else if(option == 19)
		{
			getchar();
			printf("��������!\n");
			fclose(lfp);
			lfp = fopen("log","r+"); //��Բ����ǻ��������
			fseek(lfp,0,SEEK_END);
			char temp[20];
			strcpy(temp,""); 
			strcat(temp,tName);
			strcat(temp," "); 
			strcat(temp,"commits\n");
			fwrite(temp,sizeof(char),strlen(temp),lfp); 
			fclose(lfp);
			printf("ִ�в���!\n"); 
			char* texbuf = exbuf; 
			for(i=0;i<proNum;i++)
			{
				int len = getString(ch,0,texbuf+7); 
				texbuf[6] = '\0';
				if(strcmp(texbuf,"select") == 0)
				{
					SelectTable(len,NULL); 
					texbuf = exbuf+reclen[i]; 
				}
				else if(strcmp(texbuf,"update") == 0) 
				{
					UpdateTable(len); 
					texbuf = exbuf+reclen[i]; 
				}
				else 
				{
					printf("ϵͳ������!\n"); 
					return false; 
				}
			}

			break;
		}
		else 
		{
			printf("�����������!\n");
			return false;
		}
	}

	return true;
}

//��ʼ�����״̬
bool initTableState(char* tname)
{
	FILE* fp = fopen(tname,"rb+");
	if(fp == NULL)
	{
		printf("������ļ���������!\n");
		return false;
	}
	
	Table t;
	fread(&t,sizeof(char),sizeof(Table),fp);
	t.stateS = t.stateX = 0;
	fseek(fp,0,SEEK_SET);
	fwrite(&t,sizeof(char),sizeof(Table),fp);
	fclose(fp);

	return true;

}
