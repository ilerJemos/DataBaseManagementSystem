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
	//初始化系统
	InitSystem();

	//登录
	re_start:
	system("cls");
	Wellcom();
	int authority  = 0;
    authority = login();
	if(authority == 0)
	goto re_start;

	//实现sql语句
	int option = 0;
	int strNum = 0;
	int setAy = 2;							//默认为第二级
	while(1)
	{
		setAy = 2;                          //重新初始化
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
				printf("你好,暂未得到建表授权!");
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
				printf("你好,暂未得到修改表结构授权!");
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
				printf("你好,暂未得到删除对该表授权!");
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
				printf("你好,暂未得到对该表插入记录授权!");
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
				printf("你好,暂未得到对该表删除记录授权!");
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
				printf("你好,暂未得到对该表建表授权!");
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
				printf("你好,暂未得到对该表建视图授权!");
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
				printf("你好,暂未得到查询该表授权!");
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
				printf("你好,暂未得到对该表集合操作授权!");
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
				printf("你好,暂未得到运行事务授权!");
				break;
			}
		if(runTask(strNum))
			printf("事务运行成功!\n");
			else
			printf("事务运行失败!\n");
			break;
		case 20:
			if(authority > 2)
			{
				printf("你好暂未得到日志恢复授权!\n");
				break;
			}
	if(Recovery())
			printf("恢复数据成功!\n");
		else
				printf("恢复数据失败!\n");
		break;
		case 22:
			if(authority > 3)
			{
				printf("你好暂未得到重置表的读写属性!\n");
				break;
			}
		if(initTableState(ch[0]))
			printf("重置表读写属性成功!\n");
		else
			printf("重置表读写属性失败!\n");
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

	printf("\t\t\tDBMS  模拟\n");
	return;
}

//登录
int login(void)  //返回
{

	char name[20];
	char password[20];
	char selString[100];
	int authority = 0;
	printf("用户名:");
	scanf("%s",name);
	getchar();												//接受回车							
	printf("请输入密码:");
	//scanf("%s",password);
	getPassword(password);
	printf("\n");
	int strNum = setSelectString(selString,name,password); //不要用getString函数
	strNum = getString(ch,0,selString);
	if(SelectTable(strNum,&authority) == true)             //selectTable函数返回查询结果就更好  
		printf("欢迎您的登陆!\n");
	else
	{
		printf("登陆失败!\n");
		printf("三秒后将自动返回登入界面");
		Sleep(3000);
	}

	return authority;
	
}




//初始化系统
bool InitSystem(void)
{
	FILE* fp;
	int id = 0;
	int userNum = 0;
	fp = fopen("r_filename","rb");
	if(fp == NULL)
		fp = fopen("r_filename","w");
	fclose(fp);
	
	fp = fopen("log","rb");		//日志文件
	if(fp == NULL)
		fp = fopen("log","w");
	fclose(fp);

	fp = fopen("userlist","rb");	//用户列表，大概也是一张表
	if(fp == NULL)	//打不开就初始化用户列表，插入一个lutao的用户
	{
		char str[100]; 
		strcpy(str,"userlist(id int pkey,name char(9),authority int,password char(9))");
		int	strNum = getString(ch,0,str);
		if(CreateTable(strNum) == true)
		{
			printf("初始化用户列表成功!\n");
		}
		else
		{
			printf("初始化用户列表有误!\n");
			return false;
		}
	
		strcpy(str,"userlist values(1,lutao,1,123)");
		strNum = getString(ch,0,str);
		if(InsertTable(strNum) == true)
			printf("插入管理员王倪传成功!\n");
		else
		{
			printf("插入管理员王倪传失败!\n");
			return false;
		}
	}

	return true;
}

//没有卵用，就写出来，也没用过
void gotoxy(int x, int y)    //gotoxy()
 {
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
} 

//把输入的用户名和密码组成一句话
int setSelectString(char* sstr,char* name,char* password)
{
	strcpy(sstr,"* from userlist where name = ");
	strcat(sstr,name);
	strcat(sstr," and password = ");
	strcat(sstr,password);
	strcat(sstr,";");
	return 11;
}




