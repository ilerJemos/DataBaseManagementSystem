#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "CreateTable.h"
#include "CreateView.h"
#include "InsertTable.h"
#include "UpdateTable.h"
#include "AlterTable.h"
#include "DropTable.h"
#include "DeleteRecord.h"
#include "SelectTable.h"
#include "SetOperate.h"
#include "user.h"
#include "Gloab.h"
#include "Task.h"
#include "Recovery.h"

void Wellcom(void);
bool InitSystem(void);
int selectFunction(void);
int login(void);


int setSelectString(char* sstr,char* name,char* password);

void gotoxy(int x, int y);

int main()
{
	//��ʼ��ϵͳ
	InitSystem();

	//��¼
	re_start:
	system("cls");
	Wellcom();
	int authority  = 0;
    authority = login();
	if(authority == 0)
	goto re_start;

	//ʵ��sql���
	int option = 0;
	int strNum = 0;
	int setAy = 2;							//Ĭ��Ϊ�ڶ���
	while(1)
	{
		setAy = 2;                          //���³�ʼ��
		option = selectFunction();
		if(option != 13 && option  != 17 && option != 20 && option!= 22)
			strNum = getString(ch,1,NULL);
		if(option == 22)
		{
			scanf("%s",ch[0]);
		}
		if(searchStr(ch,"userlist",strNum))
			setAy = 1;
		switch(option)
		{
		case -1:
			printf("command error!\n");
			break;
		case 2:
			if(authority > setAy)
			{
				printf("���,��δ�õ�������Ȩ!");
				break;
			}
			if(CreateTable(strNum))
				printf("create table successful!\n");
			else
				printf("create table failure!\n");
			break;
		case 4:
			if(authority > setAy)
			{
				printf("���,��δ�õ��޸ı�ṹ��Ȩ!");
				break;
			}
			if(AlterTable())
				printf("alter table successful!\n");
			else
				printf("alter table failure!\n");
			break;
		case 6:
			if(authority > setAy)
			{
				printf("���,��δ�õ�ɾ���Ըñ���Ȩ!");
				break;
			}
			if(DropTable())
				printf("drop table successful!\n");
			else
				printf("drop table failure!\n");
			break;
		case 8:
			if(authority > setAy)
			{
				printf("���,��δ�õ��Ըñ�����¼��Ȩ!");
				break;
			}
			if(InsertTable(strNum))
				printf("insert table successful!\n");
			else
				printf("insert table failure!\n");
			break;
		case 10:
			if(authority > setAy)
			{
				printf("���,��δ�õ��Ըñ�ɾ����¼��Ȩ!");
				break;
			}
			if(DeleteRecord(strNum))
				printf("delete record successful!\n");
			else
				printf("delete record unsuccessfully!\n");
			break;
		case 11:
			if(authority > setAy)
			{
				printf("���,��δ�õ��Ըñ�����Ȩ!");
				break;
			}
		if(UpdateTable(strNum))
			printf("Update table successful!\n");
		   else
			printf("Update table unsuccessfully!\n");
		   break;
		case 12:
			if(authority > setAy)
			{
				printf("���,��δ�õ��Ըñ���ͼ��Ȩ!");
				break;
			}
			if(CreateView())
				printf("Create view successful!\n");
			else
				printf("Create view unsuccessfully!\n");
			break;
		case 13:
			goto re_start;

		case 14:
			if(setAy == 2)
				setAy = 3;
			if(authority > setAy)
			{
				printf("���,��δ�õ���ѯ�ñ���Ȩ!");
				break;
			}
			if(SelectTable(strNum,NULL))
				printf("select table successful!\n");
			else
				printf("select table unsuccessfully!\n");
			break;
		case 15:
			if(setAy == 2)
				setAy = 3;
			if(authority > setAy)
			{
				printf("���,��δ�õ��Ըñ��ϲ�����Ȩ!");
				break;
			}
			if(setOperate(strNum))
				printf("Set operate successful!\n");
			else
				printf("Set operate unsuccessfully!\n");
			break;
		case 17:
			if(authority > 2)
			{
				printf("���,��δ�õ�����������Ȩ!");
				break;
			}
		if(runTask(strNum))
			printf("�������гɹ�!\n");
			else
			printf("��������ʧ��!\n");
			break;
		case 20:
			if(authority > 2)
			{
				printf("�����δ�õ���־�ָ���Ȩ!\n");
				break;
			}
	if(Recovery())
			printf("�ָ����ݳɹ�!\n");
		else
				printf("�ָ�����ʧ��!\n");
		break;
		case 22:
			if(authority > 3)
			{
				printf("�����δ�õ����ñ�Ķ�д����!\n");
				break;
			}
		if(initTableState(ch[0]))
			printf("���ñ��д���Գɹ�!\n");
		else
			printf("���ñ��д����ʧ��!\n");
		break;
		default:
		printf("system error!\n");
		return 0;
		}
		printf("\n\n");
	}
	getchar();
	return 0;
}


void Wellcom(void)
{

	printf("\t\t\tDBMS  ģ��\n");
	return;
}

//��¼
int login(void)  //����
{

	char name[20];
	char password[20];
	char selString[100];
	int authority = 0;
	printf("�û���:");
	scanf("%s",name);
	getchar();												//���ܻس�							
	printf("����������:");
	//scanf("%s",password);
	getPassword(password);
	printf("\n");
	int strNum = setSelectString(selString,name,password); //��Ҫ��getString����
	strNum = getString(ch,0,selString);
	if(SelectTable(strNum,&authority) == true)             //selectTable�������ز�ѯ����͸���  
		printf("��ӭ���ĵ�½!\n");
	else
	{
		printf("��½ʧ��!\n");
		printf("������Զ����ص������");
		Sleep(3000);
	}

	return authority;
	
}




//��ʼ��ϵͳ
bool InitSystem(void)
{
	FILE* fp;
	int id = 0;
	int userNum = 0;
	fp = fopen("r_filename","rb");
	if(fp == NULL)
		fp = fopen("r_filename","w");
	fclose(fp);
	
	fp = fopen("log","rb");		//��־�ļ�
	if(fp == NULL)
		fp = fopen("log","w");
	fclose(fp);

	fp = fopen("userlist","rb");	//�û��б����Ҳ��һ�ű�
	if(fp == NULL)	//�򲻿��ͳ�ʼ���û��б�����һ��lutao���û�
	{
		char str[100]; 
		strcpy(str,"userlist(id int pkey,name char(9),authority int,password char(9))");
		int	strNum = getString(ch,0,str);
		if(CreateTable(strNum) == true)
		{
			printf("��ʼ���û��б�ɹ�!\n");
		}
		else
		{
			printf("��ʼ���û��б�����!\n");
			return false;
		}
	
		strcpy(str,"userlist values(1,lutao,1,123)");
		strNum = getString(ch,0,str);
		if(InsertTable(strNum) == true)
			printf("�������Ա���ߴ��ɹ�!\n");
		else
		{
			printf("�������Ա���ߴ�ʧ��!\n");
			return false;
		}
	}

	return true;
}

//û�����ã���д������Ҳû�ù�
void gotoxy(int x, int y)    //gotoxy()
 {
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
} 

//��������û������������һ�仰
int setSelectString(char* sstr,char* name,char* password)
{
	strcpy(sstr,"* from userlist where name = ");
	strcat(sstr,name);
	strcat(sstr," and password = ");
	strcat(sstr,password);
	strcat(sstr,";");
	return 11;
}




