#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <dos.h> 
#include <windows.h>
#include "Gloab.h"

char ch[20][20];

//分析输入的语句
int getString(char ch[][20],bool Src = 1,char* str = NULL)
{
	int str_num = 0;
	int strl = 0;
	if(!Src) //如果是false，获取为字符串长度
	   strl = strlen(str);
	int flag = 0;
	char c;
	int i = 0;
	int j = 0;
	int k = 0;
	do{ 
		if(Src)
			c = getchar();		//.........
		else if(k <= strl)
			c = str[k++];
		else;
		if(c== ','|| c == ' '||c == 10)  //10为换行符
		{
			if(j!=0) //分解出每个单词
			{
				//j++;
				ch[i][j] = '\0';
				j = 0;
				i++;
			}
		}
		else if(c == '(')
		{
			if(j!=0)
			{
				//j++;
				ch[i][j] = '\0';
				j = 0;
				i++;
			}
			flag = flag +1;
		}
		
		else if(c == ')')
		{
			if(j!=0)
			{
				//j++;
				ch[i][j] = '\0';
				j = 0;
				i++;
			}
			flag--;
		}

		else if(c == ';'|| c == '\0') //到末尾了，退出循环		
		{
			if(j!=0)	//结束这一行
			{
				//j++;
				ch[i][j] = '\0';
				j = 0;
				i++;
			}
			break;	
		}
		else if(c == '='||c=='<'||c=='>'||c == '.')	//单独成一单词
		{
			if(j!=0)	//如果不是单独的则把这一行结束，下一行把符号单独放进去结束
			{
				ch[i][j] = '\0';
				i++;
				ch[i][0] = c;
				ch[i][1] = '\0';
				j = 0;
				i++;
			}
			else	//如果是单独的，空格后面，把这一行结束
			{
				ch[i][j] = c;
				ch[i][j+1] = '\0';
				j = 0;
				i++;
			}
		}
		else 
		{
			
			ch[i][j] = c;
			j++;
		}

		if(flag < 0)
			return -1;
	}while( ( (c!=')') || (flag!=0) ) && (c!=';') );	//是右括号，和没有到末尾时候退出循环

	if(!Src)
		str = str+k;
	return i;
}

//搜索文件
bool SearchFile(char fname[])
{
	char filename[20];
	char filen[20];
	char str[2] = "\n";
	strcpy(filename,fname);
	strcat(filename,str);
	FILE* fp;
	fp = fopen("r_filename","r");
	if(fp == NULL)
		return false;
	
	while(fgets(filen,20,fp) != NULL)
	{
		if(strcmp(filen,filename) == 0)
			return true;
	}
	fclose(fp);
	return false;
}

//设置值
bool setScrString(char* ch)
{
	int i = 0;
	return true;
	
}

//查询用户表内是否存在此用户名
bool searchStr(char dest[][20],char* tName,int num)
{
	int i = 0;
	for(;i<num;i++)
		if(strcmp(tName,dest[i]) == 0)
			return true;
	return false;
}


//获取密码
bool getPassword(char* password)
{
	int i = 0;
	char c;
	while(1)
	{
		c = getch();
		//printf("%d",c);
		if(c != 13)		//不是回车键，每输入一个字符输出一个*
		{
			password[i] = c;
			printf("*");
			i++;
		}
		else	//如果是回车，在末尾加结束符，退出循环
		{
			password[i] = '\0';
			break;
		}
	}

	return true;
}

//添加共享锁
bool lockS(char* tname)
{
	FILE*  fp = fopen(tname,"rb+");
	if(fp == NULL)
	{
		printf("系统错误!\n");
		return false;
	}
	Table t;
	
	fread(&t,sizeof(char),sizeof(Table),fp);
	if(t.stateX == 0)
	{
		
		t.stateS++;
		printf("%d %d",t.stateS,t.stateX);   //打印 0 ，1
		fseek(fp,0,SEEK_SET);
		fwrite(&t,sizeof(char),sizeof(Table),fp);
		fclose(fp);
		return true;
	}
	else
	{
		fclose(fp);
		return false;
	}
}

//释放共享锁
bool unlockS(char* tname)
{
	FILE*  fp = fopen(tname,"rb+");
	if(fp == NULL)
	{
		printf("系统错误!\n");
		return false;
	}
	Table t;
	
	fread(&t,sizeof(char),sizeof(Table),fp);
	if(t.stateS >= 1)
	{
		
		t.stateS --;
		printf("%d %d",t.stateS,t.stateX);
		fseek(fp,0,SEEK_SET);
		fwrite(&t,sizeof(char),sizeof(Table),fp);
		fclose(fp);
		return true;
	}
	else
	{
		fclose(fp);
		return false;
	}
}

//添加互斥锁
bool lockX(char* tname)
{
	int s,x;
	FILE*  fp = fopen(tname,"rb+");
	if(fp == NULL)
	{
		printf("系统错误!\n");
		return false;
	}

	Table t;
	fread(&t,sizeof(char),sizeof(Table),fp);
	if((t.stateS == 0) && (t.stateX == 0))
	{
		
		t.stateX = 1;
		printf("%d %d",t.stateS,t.stateX);
		fseek(fp,0,SEEK_SET);
		fwrite(&t,sizeof(char),sizeof(Table),fp);
		fclose(fp);
		return true;
	}
	else
	{

		fclose(fp);
		return false;
	}
}

//释放互斥锁
bool unlockX(char* tname)
{
	
	FILE*  fp = fopen(tname,"rb+");
	if(fp == NULL)
	{
		printf("系统错误!\n");
		return false;
	}
	Table t;
	fread(&t,sizeof(char),sizeof(Table),fp);
	if(t.stateX == 1)
	{
		
		t.stateX = 0;
		printf("%d %d",t.stateS,t.stateX);
		fseek(fp,0,SEEK_SET);
		fwrite(&t,sizeof(char),sizeof(Table),fp);
		fclose(fp);
		return true;
	}
	else
	{	
		fclose(fp);
		return false;
	}
}

//显示所用时间
void printProcess(char* str)
{
	int times = 5;
	int i= 0;
	printf(str);
	for(i=0;i<times;i++)
	{
		printf(". ");
		Sleep(1000);
	}

}

//-----------------------------------------------------
//确定方法，创建，插入，删除，丢表，更新，修改，查找，退出，
int selectFunction(void)
{
	char ch[20];
	int state = 0;
	while(1)
	{
		switch(state)
		{
		case -1:	
			return -1;
		case 0:		//判断是什么操作
			scanf("%s",ch);
			if(strcmp("create",ch) == 0)	//创建表
				state = 1;
			else if(strcmp("insert",ch) == 0)	//插入数据
				state = 7;
			else if(strcmp("delete",ch) == 0)	//删除数据
				state = 9;
			else if(strcmp("drop",ch) == 0)	//删除表
				state = 5;
			else if(strcmp("update",ch) == 0)
				state = 11;
			else if(strcmp("alter",ch) == 0)	//修改表
				state = 3;
			else if(strcmp("select",ch) ==0)
				state = 14;
			else if(strcmp("exit",ch) == 0)
				state = 13;
			else if(strcmp("set",ch) == 0)
				state = 15;
			else if(strcmp("run",ch) == 0)	//实现
				state = 16;
			else if(strcmp("int",ch) == 0)
				state = 18;
			else if(strcmp("end",ch) == 0)
				state = 19;
			else if(strcmp("recover",ch) == 0)
				state = 20;
			else if(strcmp("reset",ch) == 0)
				state = 21;
			else if(strcmp(";",ch)==0)
				state =0;
			else
				state = -1;
			break;

		//create
		case 1:
			scanf("%s",ch);
			if(strcmp("table",ch) == 0)
				state = 2;
			else if(strcmp("view",ch) == 0)
				state = 12;
			else
				state = -1;
			break;

		case 2:
			return 2;

		//alter
		case 3:
			scanf("%s",ch);
			if(strcmp("table",ch) == 0)
				state = 4;
			else
				state = -1;
			break;
		case 4:
			return 4;

		//drop
		case 5:
			scanf("%s",ch);
			if(strcmp("table",ch) == 0)
				state = 6;
			else
				state = -1;
			break;
		case 6:
			return 6;

		//insert
		case 7:
			scanf("%s",ch);
			if(strcmp("into",ch) == 0)
				state = 8;
			else
				state = -1;
			break;
		case 8:
			return 8;

		//delete
		case 9:
			scanf("%s",ch);
			if(strcmp("from",ch)== 0)
				state = 10;
			else
				state = -1;
			break;
		case 10:
			return 10;
		case 11:
			return 11;
		case 12:
			return 12;
		case 13:
			return 13;
		case 14:
			return 14;
		case 15:
			return 15;

		//run
		case 16:
			scanf("%s",ch);
			if(strcmp("task",ch) == 0)
				state = 17;
			else
				state = -1;
		case 17:
			return 17;
		case 18:
			return 18;
		case 19:
			return 19;
		case 20:
			return 20;

		//reset
		case 21:
			scanf("%s",ch);
			if(strcmp(ch,"state") == 0)
				state = 22;
		case 22:
			return 22;
		default:
			state = -1;
		}
	}
		
}

//登录成功时显示本地时间
void showLocalTime(void)
{
	time_t timer; 
    struct tm *tblock; 

     /* gets time of day */ 
    timer = time(NULL); 
	timer = time(NULL); 
	tblock = localtime(&timer);
    printf("\t %s", asctime(tblock)); 
	
}

