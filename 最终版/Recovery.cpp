#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <fstream>
#include <iostream>
#include "Task.h"
#include "Recovery.h"
#include  "Gloab.h"
#include  "UpdateTable.h"

bool getTask(Logitem* head,Task* undoT,Task* redoT);
bool addTtask(char* names,int* t,char na);
bool undo(Logitem* head,Task* undot);
bool redo(Logitem* head,Task* redot);
bool isIncludeTask(Task* ut,char na);
bool setLogfile(Logitem* head);

bool Recovery(void)
{
	char buf[80];
	Logitem* head;
	head = (Logitem*)malloc(sizeof(Logitem));
	head->next = head;
	head->pre  = head;
	FILE* fp = fopen("log","r+");
	Task redoTask[10];
	Task undoTask[10];
	if(fp == NULL)
	{
		printf("日志文件初始化有错误!\n");
		return false;
	}
	int i = 0;
	Logitem* curtm = head;

	Logitem* item = NULL;
	while(1)
	{
		if(fgets(buf,78,fp))				
		{
			item = (Logitem*)malloc(sizeof(Logitem));	//倒序搜索!
			strcpy(item->content,buf);
			curtm->next = item;
			item->pre = curtm;
			item->next  = head;
			head->pre  = item;
			curtm = item;
		}
		else
		{
			break;
		}
	}
	
	getTask(head,undoTask,redoTask);
	undo(head,undoTask);
	redo(head,redoTask);
	
	setLogfile(head);
	return true;

}


//---------------------------------------------------------

bool getTask(Logitem* head,Task* undoT,Task* redoT)
{
	char tTask[20];
	char temp[100];
	int i,j,k;
	i = k = j = 0;
	undoT->num = redoT->num = 0;
	Logitem* curtm = head->pre;
	while(curtm != head)
	{
		strcpy(temp,curtm->content);
		if(addTtask(tTask,&k,temp[0]))
		{
			if(strcmp(temp+2,"commits\n") == 0)
			{
				redoT->name[i] = temp[0];
				i++;
			}
			else
			{
				undoT->name[j] = temp[0];
				j++;
			}
		}
		curtm = curtm->pre;
	}
	redoT->num = i;
	undoT->num = j;

	return true;
}

//------------------------------------------------------------

bool addTtask(char* names,int* t,char na)
{
	int i = 0;
	for(i=0;i<(*t);i++)
	{
		if(names[i] == na)
			return false;
	}
	
	*(names+*t) = na;
	(*t)++;
	
	return true;
}
//----------------------------------------------------
bool undo(Logitem* head,Task* undot)
{
	Logitem* pretm = NULL;
	Logitem* curtm = head->next;
	Logitem* nextm = NULL;
	while(curtm != head)
	{
		if(isIncludeTask(undot,curtm->content[0]))
		{
			nextm = curtm->next;
			pretm = curtm->pre;
			pretm->next = nextm;
			nextm->pre = pretm;
			free(curtm);
			curtm = nextm;
		}
		else
			curtm = curtm->next;	
	}

	return true;
}

//------------------------------------------------------
bool redo(Logitem* head,Task* redot)
{
	Logitem* curtm = head->next;
	int strl = 0;
	while(curtm != head)
	{
		if(strcmp(curtm->content+2,"commits\n") !=0 && strcmp(curtm->content+2,"starts\n") != 0)
		{
			strl = getString(ch,0,curtm->content+9);
			if(initTableState(ch[0]))
			{
				printf("恢复%s文件的读写属性\n",ch[0]);
			}
			else
				return false;
			UpdateTable(strl);
		}
		curtm = curtm->next;
	}
	return true;
}

//---------------------------------------------------------
bool setLogfile(Logitem* head)
{
	FILE* fp = fopen("log","w");
	Logitem* curtm = head->next;
	while(curtm != head)
	{
		fwrite(curtm->content,sizeof(char),strlen(curtm->content),fp);
		curtm = curtm->next;
	}
	fclose(fp);
	return true;
}
//---------------------------------------------------------------
bool isIncludeTask(Task* ut,char na)
{
	int i = 0;
	for(i=0;i<ut->num;i++)
	{
		if(ut->name[i] == na)
			return true;
	}

	return false;

}
