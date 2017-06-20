#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <dos.h> 
#include "SelectTable.h"
#include "CreateTable.h"
#include "Gloab.h"
#include "Index.h"

bool showTable(char tname[]);

bool GetIndexFile(char fname[],INDEX* pif);
bool ListRecord(Table* pt,INDEX* pif,char buf[]);

void showTableHead(Table* pt);

bool getShadowImf(Shadows* ps,tableImf* tf,char ch[][20],int strlen);


int selectTableS(tableImf* ptf,Shadows* psf,Conditions* pcf,char* result);
int selectTableD(tableImf* ptf,Shadows* psf,Conditions* pcf,char* result);

bool IsRecordOk(tableImf* ptf,Conditions* psf,int readPos[]);

bool recCpy(tableImf* ptf,int tNum,int recNum,char* result);

bool isIncludeKey(tableImf* ptf,Shadows* psf);

int countFieldLen(int fieldNumth,Table* pt);

void simpleCondition(tableImf* ptf,Conditions* pcf,int readPos[]);



int searchTabofField(tableImf* ptf,char* fName,int* fNum);

int searchTable(tableImf* ptf,char* ch);

int  getShadowS(tableImf* ptf,Shadows* psf,char* result);

int getShadowD(tableImf* ptf,Shadows* psf,Result* res,char* result);// S&&D 合并!

void simpleCondition(tableImf* ptf,Conditions* pcf,int readPos[]);

bool IsOk(aCondition* con);

void showResHead(tableImf* ptf,Shadows* ps);

void showResult(char* result,int resNum,Shadows* ps,tableImf* ptf);


int getOperatorType(char* ch);



//select语句
bool SelectTable(int strlen,int* authority)
{

	char result[2024];
	Shadows sf;
	tableImf tf;
	Conditions cf;
	if(!getTableImf(&tf,ch,strlen))	//表是否存在
	{
		printf("table's imformation error!\n");
		return false;
	}
	if(!getShadowImf(&sf,&tf,ch,strlen))	
	{
		printf("Shadows' imformation error!\n");
		return false;
	}

	if(!getConditionImf(&tf,&cf,ch,strlen))
	{
		printf("Conditions' imformation error!\n");
		return false;
	}
	
	//分别执行什么操作
	showResHead(&tf,&sf);
	int resNum;
	if(tf.num == 1)
	{
		resNum = selectTableS(&tf,&sf,&cf,result);
		if(resNum == -1)
		{
			return false;
		}
		else
		{
			showResult(result,resNum,&sf,&tf);
		}
	}
	else if(tf.num == 2)
	{

		resNum = selectTableD(&tf,&sf,&cf,result);
		if(resNum == -1)
		{
			return false;
		}
		else
		{
			showResult(result,resNum,&sf,&tf);
		}

	}
	else
	{
		printf("尊敬的用户：不好意思，本系统尚未完成！\n");
	}

	if((authority!=NULL)&&isIncludeTable("userlist",&tf))	//存在此用户
		(*authority) = atoi(result+15);

	if(resNum == 0)
		return false;
	else
		return true;

}

//显示表中数据
bool showTable(char tname[])
{
	FILE* fp = fopen(tname,"rb");
	if(fp == NULL)
	{
		printf("file can't be opened!\n");
		return false;
	}
	Table t;
	char buf[1024];
	//slink sl;
	int i = 0;
	fread(&t,sizeof(char),sizeof(Table),fp);
	fread(buf,sizeof(char),1024,fp);
	showTableHead(&t);	//显示表名吧

    INDEX ifile;
	if(!GetIndexFile(tname,&ifile))
	{
		printf("Exist errors in GetIndexFile!\n");
		return false;
	}

	ListRecord(&t,&ifile,buf);	//显示查到的记录
	ShowFileIndex(tname);	//显示索引
	fclose(fp);
	return true;
}



//获取索引文件
bool GetIndexFile(char fname[],INDEX* ifile)
{
	FILE* fp;
	char finame[20];
	strcpy(finame,fname);
	strcat(finame,"_id");
	fp = fopen(finame,"rb");	//打开索引文件
	if(fp == NULL)
	{
		printf("Can't open file %s\n",fname);
		return false;
	}

	fclose(fp);
	return true;
}

//把记录列出来
bool ListRecord(Table* pt,INDEX* pif,char buf[])
{
	//Table t;
	int len = 0;

	//fseek(fp,0,SEEK_SET);
	//fread(&t,sizeof(char),sizeof(Table),fp);
	//fread(buf,sizeof(char),1024,fp);
	len = CountRecordLen(pt,pt->field_num);	//计算记录的长度
	//int head = sl->node[sl->rear].next;
	//int thead = head;
	int i = 0;
	//int cur;
	int tlen = 0;
	int j = 0;
	for(j =0;j<pif->trecord;j++)
	{
		tlen = 0;	//!!不知道是干嘛的
		//cur = sl->node[thead].next;
		//fseek(fp,sizeof(Table),SEEK_SET);
		//fseek(fp,sl->node[cur].rec_num*(len+t.field_num),SEEK_CUR);
		//fread(buf,sizeof(char),len+t.field_num,fp);
		for(i=0;i<pt->field_num;i++)
		{
			printf("%s\t",buf+tlen+pif->node[j].rec_num*(pt->field_num+len));
			tlen += (pt->field[i].length+1);

		}
		printf("\n");
		//thead = cur;
	}
	return true;
}


/*int SelectFiedlType(char ch[])
{
	//char ifname[20];
	//FILE* ifp;
	//FILE* fp;
	//int   datatype = 0;
	//char  buf[20*20];
	//int   len = 0;
	int    operatortype = 0;
	//int   result = 0;
	//char buf[20*20];
	//slink sl;
	//strcpy(ifname,t.name);
	//strcat(ifname,"_id");
	//fp = fopen(pt->name,"rb");
	//ifp = fopen(ifname,"rb");
	//fread(&sl,sizeof(char),sizeof(slink),ifp);
	//if(ifp == NULL)
	//{
	//	printf("%s index file exists error!\n",ifname);
	//	return false;
	//}
	if(strcmp(ch,"=") == 0)
	{
		operatortype = 1;
	}
	else if(strcmp(ch,"<") == 0)
	{
		operatortype = 2;
	}
	else if(strcmp(ch,">") == 0)
	{
		operatortype = 3;
	}
	else
	{
		operatortype = -1;
	}
	//len = CountRecordLen(pt,field_no);
	//if(pt->field[field_no].pkey)
	//{
	//	 result = SelectTableByIndex(psl,pt,ch[6],pt->field[field_no].datatype,operatortype,start_no);
//	}
//	else
//	{
	//	fseek(fp,sizeof(Table),SEEK_SET);
	//	fread(buf,sizeof(char),20*20,fp);
	//	result = SelectTableByFile(psl,pt,ch[6],pt->field[field_no].datatype,operatortype,field_no,buf,start_no);

	//}

//	fclose(fp);
//	fclose(ifp);

//	return result;
	return operatortype;
}*/

//通过索引找对应的表
int SelectTableByIndex(INDEX* pif,Table* pt,char ch[],int datatype,int operatortype,
					 int start_no)
{
	//int head = psl->node[psl->rear].next;
	//int thead = start_no;
	//int cur = 0;
	int i = start_no;
	bool flag = false;
	//char buf[30];
	int len = CountRecordLen(pt,pt->field_num);
	while(i < pif->trecord)
	{
		flag = false;
		//cur = psl->node[thead].next;
		if(datatype == 1)	//char
		{

			if((strcmp(pif->node[i].keyWord[0],ch) == 0) && (operatortype == 1))
				flag = true;

			else if((strcmp(pif->node[i].keyWord[0],ch) == -1)&&(operatortype == 2))
			{
				flag = true;
			}
			else if((strcmp(pif->node[i].keyWord[0],ch) == 1)&&(operatortype == 3))
			{
				flag = true;
			}
		}
		else if(datatype == 2)	//int
		{
			int t1 = atoi(pif->node[i].keyWord[0]);
			int t2 = atoi(ch);
			if( (t1 == t2)&&(operatortype == 1))
				flag = true;
			else if((t1 < t2)&&(operatortype == 2))
				flag = true;
			else if((t1 > t2)&&(operatortype == 3))
				flag = true;

		}
		else if(datatype ==3)	//float
		{
			float t1 = atof(pif->node[i].keyWord[0]);
			float t2 = atof(ch);
			if((t1==t2)&&(operatortype ==1))
				flag = true;
			else if((t1<t2)&&(operatortype ==2))
				flag = true;
			else if((t1>t2)&&(operatortype ==3))
				flag = true;
		}

		if(flag == true)
		{
			//fseek(fp,sizeof(Table),SEEK_SET);
			//fseek(fp,sl->node[cur].rec_num*(len+t->field_num),SEEK_CUR);
			//fread(buf,sizeof(char),len+t->field_num,fp);
			//ShowRecord(buf,t);
			start_no = i;
			return i;
		}
		else
		{
			i++;
		}
	}

	return -1;

}

int SelectTableByFile(INDEX* pif,Table* pt,char ch[],int datatype,int operatortype,int field_no,
					 char *buf,int start_no)
{
	//int head = psl->node[psl->rear].next;
	//int thead = head;
	//int cur;
	int i = start_no;
	int len = CountRecordLen(pt,field_no);
	int tlen = CountRecordLen(pt,pt->field_num);
	int num = 0;
	bool flag = false;
	char dest[20];
	while(i < pif->trecord)
	{
		flag = false;
		//cur = psl->node[thead].next;
		num =pif->node[i].rec_num*(tlen+pt->field_num);
		strcpy(dest,buf+num+(len+field_no));
		if(datatype == 1)
		{
			if((strcmp(dest,ch) == 0)&&(operatortype == 1))
			{
				flag = true;
			}
			else if((strcmp(dest,ch) == -1)&&(operatortype == 2))
			{
				flag = true;
			}
			else if((strcmp(dest,ch) == 1)&&(operatortype == 3))
			{
				flag = true;
			}
		}
		else if(datatype == 2)
		{
			int t1 = atoi(dest);
			int t2 = atoi(ch);
			if((t1 == t2)&&(operatortype == 1))
				flag = true;
			else if((t1 < t2)&&(operatortype == 2))
				flag = true;
			else if((t1 > t2)&&(operatortype == 3))
				flag = true;
		}
		else if(datatype == 3)
		{
			float t1 = atof(dest);
			float t2 = atof(dest);
			if((t1 == t2)&&(operatortype == 1))
				flag = true;
			else if((t1 < t2)&&(operatortype == 2))
				flag = true;
			else if((t1 > t2)&&(operatortype == 3))
				flag = true;
		}

		if(flag)
		{
			//ShowRecord(buf+sl->node[cur].rec_num*(tlen+t->field_num),t);
			return i;
		}
		else
		{
			i++;
		}
	}
	return -1;
}

void ShowRecord(char buf[],Table* pt)
{
	int i = 0;
	int len = 0;
	for(i=0;i<pt->field_num;i++)
	{
		printf("%s ",buf+len);
		len += (pt->field[i].length+1);
	}
	printf("\n");

}


int GetFileNo(Table* t,char ch[])
{
	int i = 0;
	for(i=0;i<t->field_num;i++)
	{
		if(strcmp(t->field[i].name,ch)==0)
			return i;
	}
	return -1;
}

int SelectFieldType(char ch[])
{

	int    operatortype = 0;

	if(strcmp(ch,"=") == 0)
	{
		operatortype = 1;
	}
	else if(strcmp(ch,"<") == 0)
	{
		operatortype = 2;
	}
	else if(strcmp(ch,">") == 0)
	{
		operatortype = 3;
	}
	else
	{
		operatortype = -1;
	}

	return operatortype;
}

//显示表名及表中各字段信息
void showTableHead(Table* pt)
{
	int i = 0;
	printf("table:\t%s\n",pt->name);
	for(;i<pt->field_num;i++)
	{
		if(pt->field[i].pkey)
			printf("*");
		printf("%s ",pt->field[i].name);

		//判断是哪种数据类型
		if(pt->field[i].datatype == 1)
		{
			printf("%s(%d)\t","char",pt->field[i].length);
		}
		else if(pt->field[i].datatype == 2)
		{
			printf("%s\t","int");
		}
		else
		{
			printf("%s\t","float");
		}
	}

//	printf("\n");

}
//------------------------------------------------------------
bool getShadowImf(Shadows* ps,tableImf* ptf,char ch[][20],int len)
{
	int tNumth = 0;
	int fNumth = 0;
	int shaNum = 0;
	int shaLen = 0;
	ps->len = 0;
	int i = 0;
	if(strcmp(ch[i],"*") == 0)
	{
		ps->num = 0;
		return true;
	}
	while(strcmp(ch[i],"from")&&(i<len))
	{
		if(strcmp(ch[i+1],".") == 0)
		{
			tNumth = searchTable(ptf,ch[i]);
			if(tNumth == -1)
			{
				printf("字段名或表名错误！\n");
				return false;
			}
			else
			{
				fNumth = GetFileNo(ptf->t[tNumth],ch[i+2]);
				if(fNumth == -1)
				{
					printf("字段名有错！\n");
					return false;
				}
				else
				{
					ps->sha[shaNum].tNum = tNumth;
					ps->sha[shaNum].fNum = fNumth;
					ps->sha[shaNum].dis  = getFieldDis(fNumth,ptf->t[tNumth]);//正真的偏移量
					shaLen += countFieldLen(fNumth,ptf->t[tNumth])+1;
					shaNum++;
				}
				i = i+3;
			}
		}
		else
		{
			tNumth = searchTabofField(ptf,ch[i],&fNumth);
			if(tNumth == -1)
			{
				printf("%s 属性不存在!\n",ch[i]);
				return false;
			}
			else if(tNumth == -2)
			{
				printf("%s 属性具有二义性!\n");
				return false;
			}
			else
			{
				ps->sha[shaNum].tNum = tNumth;
				ps->sha[shaNum].fNum = fNumth;
				ps->sha[shaNum].dis  = getFieldDis(fNumth,ptf->t[tNumth]);//正真的偏移量
				shaLen += (countFieldLen(fNumth,ptf->t[tNumth])+1);
				shaNum++;
			}
			i++;

		}
		ps->len += shaLen;
	}
	ps->num = shaNum;
	return true;

}
//---------------------------------------------------------------
bool getTableImf(tableImf* ptf,char ch[][20],int strNum)
{
	int i = 1;
	int tNum = 0;
	char ifname[30];
	FILE* fp;
	FILE* ifp;
//	Table* pt;
	char buf[1024];


	while(strcmp(ch[i],"where")&&strcmp(ch[i],";")&&(i<strNum)&&strcmp(ch[i],"from"))
		i++;
	if(strcmp(ch[i],"from") !=0)	//第一个词是from
	{
		printf("Select 语句有错!\n");
		return false;
	}
	else
	{
		i++;
		while( strcmp(ch[i],"where") && strcmp(ch[i],";") && (i<strNum))
		{
			fp = fopen(ch[i],"rb");	//打开该表
			if(fp == NULL)
			{
				printf("% 表不存在!\n",ch[i]);
				return false;
			}
			ptf->t[tNum] = (Table*)malloc(sizeof(Table));
			//buf = (char*)malloc(1024);
            //ptf->t[tNum]->buf = new char[1024];
			fread(ptf->t[tNum],sizeof(char),sizeof(Table),fp);

			strcpy(ifname,ch[i]);
			strcat(ifname,"_id");
			ifp = fopen(ifname,"r");	//打开该表对应的索引表
			if(ifp == NULL)
			{
				printf("%s 索引表不能打开！\n",ifname);
				return false;
			}
			
			printf("申请共享琐");
			while(!lockS(ptf->t[tNum]->name));
			
			showLocalTime();

			printf(ch[i]);
			printf("文件读取中\n");
			printf("完成!\n");
			fread(ptf->t[tNum]->buf,sizeof(char),1024,fp);
			fseek(ifp,0,SEEK_SET);
			
			ptf->t[tNum]->pif = (INDEX*)malloc(sizeof(INDEX));
			fread(ptf->t[tNum]->pif,sizeof(char),sizeof(INDEX),ifp);
			
			printf("释放共享琐");
			if(!unlockS(ptf->t[tNum]->name))
			{
				printf("发生系统错误!\n");
				return false;
			}
			
			showLocalTime();


			tNum++;
			i++;
			fclose(fp);
			fclose(ifp);
		}
		ptf->num = tNum;

	}

	return true;
}
//----------------------------------------------------------------
bool getConditionImf(tableImf* ptf,Conditions* pc,char ch[][20],int strlen)
{
	int i = 0;
	int tNumth = 0;
	int fNumth = 0;
	int conNum = 0;
	int coni = 0;    //第一个条件填的位置
	int vali = 2;    //当前可用的条件空
	int toin = 0;   // 当前该填的
	int operatortype = 0;
	while(strcmp(ch[i],"where")&&(i<strlen))
		i++;
	if(i == strlen)//没有条件！
	{
		pc->num = 0;
		return true;
	}
	else
	{
		i++;
		while(i<strlen)
		{
			if(strcmp(ch[i+1],".") == 0)
			{
				i = i+2;                        //考虑点后面的符号
				tNumth = searchTable(ptf,ch[i-2]);
				if(tNumth == -1)
				{
					printf("字段名或表名错误!\n");
					return false;
				}
				else
				{
					fNumth = GetFileNo(ptf->t[tNumth],ch[i]);
					if(fNumth == -1)
					{
						printf("不存在属性名%s有错！\n",ch[i]);
						return false;
					}
					else
					{
						if(vali == 2)
						{

							pc->content[conNum].para[0].fNum = fNumth;
							pc->content[conNum].para[0].tNum = tNumth;
							pc->content[conNum].para[0].dis  = getFieldDis(fNumth,ptf->t[tNumth]);//正真的偏移量
							pc->content[conNum].para[0].datatype = ptf->t[tNumth]->field[fNumth].datatype;
							coni = 0;
							vali--;//可用减一
							i++;                        //考虑下一个
							operatortype = getOperatorType(ch[i]);
							if( operatortype != -1)
                                pc->content[conNum].operatortype = operatortype;
                            else
                            {
                                printf("%s运算符有错\n!",ch[i]);
                                return false;
                            }
							i++;
						}
						else
						{
							if(coni == 1)
							{
								toin = 0;
							}
							else
							{
								toin = 1;
							}
							pc->content[conNum].para[toin].fNum = fNumth;
							pc->content[conNum].para[toin].tNum = tNumth;
							pc->content[conNum].para[toin].dis  = getFieldDis(fNumth,ptf->t[tNumth]);
							pc->content[conNum].para[0].datatype = ptf->t[tNumth]->field[fNumth].datatype;
							i++;
							if(i == strlen)
							{
                                ;                //不操作，
							}
							else if(strcmp(ch[i],"and")==0)
									pc->relation = 1;
							else if(strcmp(ch[i],"or")==0)
									pc->relation = 2;
                            else
                            {
                                    printf("%s关系符输入有错误!\n",ch[i]);
                                    return false;
                            }

							i++;
							coni = 0;
							vali = 2;
							conNum++;
						}


					}
				}
			}
			else
			{
				tNumth = searchTabofField(ptf,ch[i],&fNumth);
				if(tNumth == -1)							//
				{
					//printf("%s 属性不存在!\n",ch[i]);
					if(vali == 2)
					{
						pc->content[conNum].para[1].tNum = -1;			//是常数
						pc->content[conNum].para[1].fNum = 0;
						pc->content[conNum].para[1].dis  = 0;
						pc->content[conNum].para[1].datatype = 1;
                        coni = 1;
						vali--;
						operatortype = getOperatorType(ch[i]);
                        if( operatortype != -1)
                            pc->content[conNum].operatortype = operatortype;
                        else
                        {
                            printf("%s运算符有错\n!",ch[i]);
                            return false;
                        }
                        i++;
					}
				//	return false;
					else
					{
						if(coni == 1)
						{
							printf("条件有错误!\n");
							return false;
						}
						else
						{
							pc->content[conNum].para[1].tNum = -1;
							pc->content[conNum].para[1].fNum = 0;
							pc->content[conNum].para[1].dis  = 0;
							pc->content[conNum].para[1].datatype = 1;
							strcpy(pc->content[conNum].para[1].cons,ch[i]);
							i++;
							if(i == strlen)
							{
                                ;                //不操作，
							}
							else if(strcmp(ch[i],"and")==0)
									pc->relation = 1;
							else if(strcmp(ch[i],"or")==0)
									pc->relation = 2;
                            else
                            {
                                    printf("%s关系符输入有错误!\n",ch[i]);
                                    return false;
                            }

							i++;
							coni = 0;
							vali = 2;
							conNum++;

						}


					}
				}//常数
				else if(tNumth == -2)
				{
					printf("%s 属性具有二义性!\n");
					return false;
				}
				else
				{
					if(vali == 2)
					{
						pc->content[conNum].para[0].fNum = fNumth;
						pc->content[conNum].para[0].tNum = tNumth;
						pc->content[conNum].para[0].dis  = getFieldDis(fNumth,ptf->t[tNumth]);
						pc->content[conNum].para[0].datatype = ptf->t[tNumth]->field[fNumth].datatype;
						i++;
					    operatortype = getOperatorType(ch[i]);
                        if( operatortype != -1)
                            pc->content[conNum].operatortype = operatortype;
                        else
                        {
                            printf("%s运算符有错\n!",ch[i]);
                            return false;
                        }
                        i++;
                        vali--;//可用减一
                        coni = 0;
					}
					else
					{
						if(coni == 0)
						{
							toin = 1;
						}
						else
							toin = 0;
						pc->content[conNum].para[toin].fNum = fNumth;
						pc->content[conNum].para[toin].tNum = tNumth;
						pc->content[conNum].para[toin].dis  = getFieldDis(fNumth,ptf->t[tNumth]);
						pc->content[conNum].para[0].datatype = ptf->t[tNumth]->field[fNumth].datatype;
						i++;
						if((i) == strlen)
								;
						else if(strcmp(ch[i],"and")==0)
									pc->relation = 1;
						else if(strcmp(ch[i],"or")==0)
									pc->relation = 2;

						i = i+1;
						vali = 2;
						conNum++;
					}
				}//属性
			}

		}   //while
	}//else

	pc->num = conNum;

	return true;
}
//-----------------------------------------------------------------
int selectTableS(tableImf* ptf,Shadows* psf,Conditions* pcf,char* result)//单表
{
	int i = 0;
	int shadowLen = 0;
	int resNum = 0;
	int delNum = 0;
//	int j = -1;
	int readPos[8];										//最多有八表!
	int k = 0;
//	char record[50];
	int tRec = ptf->t[0]->pif->trecord;
	if(pcf->num == 0)
	{
		resNum = getShadowS(ptf,psf,result);
	}
	else
	{
	    shadowLen = psf->len;
		for(;i<tRec;i++)
		{
			readPos[0] = i-delNum;
			if(!IsRecordOk(ptf,pcf,readPos))  //1张表!
			{
				delARecord(ptf->t[0]->pif,i-delNum);
				delNum++;							//索引表已改变
			}
		}
		resNum = getShadowS(ptf,psf,result);
	}

	return resNum;

}
//-------------------------------------------------------------------
int selectTableD(tableImf* ptf,Shadows* psf,Conditions* pcf,char* result)
{
	int i;
	int j;
	int resNum = 0;
	int readPos[8];
	i = j = 0;
	Result res;
	res.num = 0;
	if(pcf->num == 0)
	{
		for(i=0;i<ptf->t[0]->pif->trecord;i++)
			for(j=0;j<ptf->t[1]->pif->trecord;j++)
			{
				res.record[res.num][0] = i;
				res.record[res.num][1] = j;
				res.num++;					//max 200
			}
		resNum = getShadowD(ptf,psf,&res,result);
	}
	else
	{
		for(i=0;i<ptf->t[0]->pif->trecord;i++)
			for(j=0;j<ptf->t[1]->pif->trecord;j++)
			{
				readPos[0] = i;
				readPos[1] = j;
				if(IsRecordOk(ptf,pcf,readPos))
				{
					res.record[res.num][0] = i;
					res.record[res.num][1]= j;
					res.num++;					//max 200
				}

			}
		resNum = getShadowD(ptf,psf,&res,result);
	}

	return resNum;

}
//----------------------------------------------------------------------
int  getShadowS(tableImf* ptf,Shadows* psf,char* result)//record number
{
	int i = 0;
	int j = 0;

	int recLen = ptf->t[0]->fieldLen+ptf->t[0]->field_num;//记录长度，开始设计问题
	int recNum = ptf->t[0]->pif->trecord;		//未投影前的记录个数
	INDEX* pindex = ptf->t[0]->pif;
	int rDis = 0;    //
	if(psf->num == 0)						//"*"
	{
		for(;i<ptf->t[0]->pif->trecord;i++)
		{
			recCpy(ptf,0,i,result);
			result += recLen;
		}
	}
	else
	{
		if(isIncludeKey(ptf,psf))
		{
			for(i=0;i<ptf->t[0]->pif->trecord;i++)
			{
				for(j=0;j<psf->num;j++)
				{
					strcpy(result+rDis,ptf->t[0]->buf+pindex->node[i].rec_num*recLen+psf->sha[j].dis);
					rDis += (countFieldLen(psf->sha[j].fNum,ptf->t[0])+1);

				}
			}

		}
		else
		{
			/*sortIndex(ptf,&psf->sha[0]);
			for(i=0;i<ptf->pid[0]->trecord;i++)
			{
				if(Isequal(ptf,i,i+1))
					delARecord(ptf->pid[0],i+1);	//删除重复
			}*/
			for(i=0;i<ptf->t[0]->pif->trecord;i++)
			{
				for(j=0;j<psf->num;j++)
				{
					strcpy(result+rDis,ptf->t[0]->buf+pindex->node[i].rec_num*recLen+psf->sha[j].dis);
					rDis += countFieldLen(psf->sha[j].fNum,ptf->t[0])+1;
				}
			}

		}

	}
	return ptf->t[0]->pif->trecord;
}

//------------------------------------------------------------------
int getShadowD(tableImf* ptf,Shadows* psf,Result* res,char* result)
{
	int i=0;
	int j=0;//维数
	int tNum = 0;
	int fNum = 0;
	Table** pt = ptf->t;
	int distance = 0;
	char* buf;
	INDEX* pIndex;
	int recLen = 0;
	int indexNo = 0;
	int resDis = 0;  //结果偏移量
	if(psf->num == 0)    //"*"
	{
		for(i=0;i<res->num;i++)
			for(j=0;j<ptf->num;j++)
			{
				recCpy(ptf,j,res->record[i][j],result+recLen);
				recLen += ptf->t[j]->fieldLen+ptf->t[j]->field_num;
			}
		return res->num; //直接返回
	}
	if(isIncludeKey(ptf,psf))
	{
		for(i=0;i<res->num;i++)
		{
			for(j=0;j<psf->num;j++)
			{
				tNum = psf->sha[j].tNum;
				fNum = psf->sha[j].fNum;
				distance = psf->sha[j].dis;
				buf = ptf->t[tNum]->buf;
				pIndex = ptf->t[tNum]->pif;
				indexNo = res->record[i][tNum];   //当前表中的纪录
				recLen = ptf->t[tNum]->fieldLen+ptf->t[tNum]->field_num;
				distance = psf->sha[j].dis;   //当前投影偏移量
				strcpy(result+resDis,buf+pIndex->node[indexNo].rec_num*recLen+distance);
				resDis += (countFieldLen(psf->sha[j].fNum,ptf->t[tNum])+1);
			}
		}
	}
	else
	{
		//sortIndex(ptf,psf,res);
		for(i=0;i<res->num-1;i++)
		{
			/*if(IsEqualRecord(ptf->t,psf,res,i,i+1))
			{
				delRecord(res,i);
			}*/
			if(0)
			{;}
			else
			{
				for(j=0;j<psf->num;j++)
				{
					tNum = psf->sha[j].tNum;
					fNum = psf->sha[j].fNum;
					distance = psf->sha[j].dis;
					buf = ptf->t[tNum]->buf;
 					pIndex = ptf->t[tNum]->pif;
					indexNo = res->record[i][tNum];   //当前表中的纪录
					recLen = ptf->t[tNum]->fieldLen+ptf->t[tNum]->field_num;
					distance = psf->sha[j].dis;   //当前投影偏移量
					strcpy(result+resDis,buf+pIndex->node[indexNo].rec_num*recLen+distance);
					resDis += (countFieldLen(psf->sha[j].fNum,ptf->t[tNum])+1);
				}
			}

		}
		for(j=0;j<psf->num;j++)
		{
			tNum = psf->sha[j].tNum;
			fNum = psf->sha[j].fNum;
			distance = psf->sha[j].dis;
			buf = ptf->t[tNum]->buf;
			pIndex = ptf->t[tNum]->pif;
			indexNo = res->record[i][tNum];   //当前表中的纪录
			recLen = ptf->t[tNum]->fieldLen+ptf->t[tNum]->field_num;
			distance = psf->sha[j].dis;   //当前投影偏移量
			strcpy(result+resDis,buf+pIndex->node[indexNo].rec_num*recLen+distance);
			resDis += (countFieldLen(psf->sha[j].fNum,ptf->t[tNum])+1);
		}

	}

	return res->num;


}
//-----------------------------------------------------------------

bool IsRecordOk(tableImf* ptf,Conditions* pcf,int readPos[])//对一个记录进行检查
{
	int i = 0;
	INDEX* pindex;
	simpleCondition(ptf,pcf,readPos);
	/*for(;i<pcf->num;i++)
	{
		if(IsOk(psf->content[i]))
			if(psf->content[i].)

	}*/
	if(pcf->num == 1)
	{

		if(IsOk(&(pcf->content[0])))
			return true;
		else
			return false;
	}
	else
	{
		if(IsOk(&(pcf->content[0])))
		{
			if(pcf->relation == 1)
				if(IsOk(&(pcf->content[1])))
					return true;
				else
					return false;
			else
				return true;
		}
		else
		{
			if(pcf->relation == 2)
			{
				if(IsOk(&(pcf->content[1])))
					return true;
				else
					return false;
			}
			else
				return false;
		}

	}
	return true;
}

//-----------------------------------------------------------
void simpleCondition(tableImf* ptf,Conditions* pcf,int readPos[])//readPos各表记录号
{
	int i = 0;
	int distance;
	int tableLen;
	int fieldNum;
	int tableNumth;
	Table** t = ptf->t;
	INDEX* pindex = ptf->t[0]->pif;

	for(i=0;i<pcf->num;i++)
	{
		tableNumth = pcf->content[i].para[0].tNum;
		if(tableNumth != -1)				//一定不为-1
		{

			tableLen = ptf->t[tableNumth]->fieldLen;
			fieldNum = ptf->t[tableNumth]->field_num;
			tableLen = tableLen + fieldNum;
			distance = pcf->content[i].para[0].dis;			//正真偏移量！！
			strcpy(	pcf->content[i].para[0].cons,t[tableNumth]->buf+tableLen*pindex->node[readPos[tableNumth]].rec_num+distance);
		}

		tableNumth = pcf->content[i].para[1].tNum;
		if(tableNumth != -1)
		{

			tableLen = ptf->t[tableNumth]->fieldLen;
			fieldNum = ptf->t[tableNumth]->field_num;
			tableLen = tableLen + fieldNum;
			distance = pcf->content[i].para[1].dis;			//正真偏移量！！
			strcpy(	pcf->content[i].para[1].cons,t[tableNumth]->buf+tableLen*pindex->node[readPos[tableNumth]].rec_num+distance);

		}

	}
}


//----------------------------------------------------------------
bool recCpy(tableImf* ptf,int tNum,int recNum,char* result)//传带有“\0”符号的字符串？
{
	int i = 0;
	int dis = 0;			//偏移量！！
	int recLen = ptf->t[tNum]->fieldLen+ptf->t[tNum]->field_num;
	char* buf = ptf->t[tNum]->buf+ptf->t[tNum]->pif->node[recNum].rec_num*recLen;
	int fNum = ptf->t[tNum]->field_num;
	for(;i<fNum;i++)
	{
		strcpy(result+dis,buf+dis);
		dis += (ptf->t[tNum]->field[i].length+1);
	}

	return true;
}

//----------------------------------------------------------------------
bool isIncludeKey(tableImf* ptf,Shadows* psf)
{
	int i = 0;
	int tableNumth =0;
	int fieldNumth = 0;
	for(;i<psf->num;i++)
	{
		tableNumth = psf->sha[i].tNum;
		fieldNumth = psf->sha[i].fNum;
		if(ptf->t[tableNumth]->field[fieldNumth].pkey)
			return true;
	}

	return false;

}

//--------------------------------------------------------------------------
int countFieldLen(int fieldNumth,Table* pt)
{
	if(pt->field[fieldNumth].datatype == 1)
	{
		return pt->field[fieldNumth].length;
	}
	else if(pt->field[fieldNumth].datatype == 2)
	{
		return 4;
	}
	else
	{
		return 8;
	}
}
//----------------------------------------------------------------------------
int getFieldDis(int fieldNumth,Table* pt)
{
	int dis = 0;
	int i = 0;
	if(pt->field_num < (fieldNumth+1))
	{
		printf("属性和表之间有错误！\n!");
		return -1;
	}
	else
	{
		for(i=0;i<fieldNumth;i++)
		{
			dis += (countFieldLen(i,pt)+1);
		}
	}
	return dis;
}
//----------------------------------------------------------------------
int searchTabofField(tableImf* ptf,char* fName,int* pfNum)
{
	int i = 0;
	int findNum = 0; //查找到的次数
	int tNum = 0;
	int fNum = 0;
	for(;i<ptf->num;i++)
	{
		fNum = GetFileNo(ptf->t[i],fName);
		if(fNum != -1)
		{
			tNum = i;
			*pfNum = fNum;
			findNum++;
		}
	}
	if(findNum == 0)
		return -1;
	else if(findNum == 1)
		return tNum;
	else
		return -2;

}
//--------------------------------------------------------------------
int searchTable(tableImf* ptf,char* ch)
{
	int i =0;
	for(;i<ptf->num;i++)
	{
		if(strcmp(ptf->t[i]->name,ch)==0)
			return i;
	}
	return -1;
}
//-----------------------------------------------------------------------

bool IsOk(aCondition* con)
{
	if(con->operatortype == 1)
	{
		if(con->para[0].datatype == 1)
		{
			if(strcmp(con->para[0].cons,con->para[1].cons)==0)
				return true;
			else
				return false;
		}
		else if(con->para[0].datatype == 2)
		{

			int t1 = atoi(con->para[0].cons);
			int t2 = atoi(con->para[1].cons);
			if(t1 == t2)
				return true;
			else
				return false;
		}
		else if(con->para[0].datatype == 3)
		{
			float f1 = atof(con->para[0].cons);
			float f2 = atof(con->para[1].cons);
			if(f1 == f2)
				return true;
			else
				return false;
		}
	}
	else if(con->operatortype == 2)
	{
		if(con->para[0].datatype == 1)
		{
			if(strcmp(con->para[0].cons,con->para[1].cons)== -1)
				return true;
			else
				return false;
		}
		else if(con->para[0].datatype == 2)
		{

			int t1 = atoi(con->para[0].cons);
			int t2 = atoi(con->para[1].cons);
			if(t1 < t2)
				return true;
			else
				return false;
		}
		else if(con->para[0].datatype == 3)
		{
			float f1 = atof(con->para[0].cons);
			float f2 = atof(con->para[1].cons);
			if(f1 < f2)
				return true;
			else
				return false;
		}
	}
	else
	{
		if(con->para[0].datatype == 1)
		{
			if(strcmp(con->para[0].cons,con->para[1].cons)== 2)
				return true;
			else
				return false;
		}
		else if(con->para[0].datatype == 2)
		{

			int t1 = atoi(con->para[0].cons);
			int t2 = atoi(con->para[1].cons);
			if(t1 > t2)
				return true;
			else
				return false;
		}
		else if(con->para[0].datatype == 3)
		{
			float f1 = atof(con->para[0].cons);
			float f2 = atof(con->para[1].cons);
			if(f1 > f2)
				return true;
			else
				return false;
		}

	}

	return true;

}

//显示表相关信息
void showResHead(tableImf* ptf,Shadows* ps)
{
	int i = 0;
	if(ps->num == 0)
	{
		for(;i<ptf->num;i++)
		{
			showTableHead(ptf->t[i]);
		}
		printf("\n");
	}
	else
	{
		int tNum = 0;
		int fNum = 0;
		for(i=0;i<ps->num;i++)
		{
			tNum = ps->sha[i].tNum;
			fNum = ps->sha[i].fNum;
			printf("%s\t",ptf->t[tNum]->field[fNum].name);
		}
		printf("\n");
	}
	return;
}

//显示结果
void showResult(char* result,int resNum,Shadows* ps,tableImf* ptf)
{
	int i = 0;
	int j = 0;
	int dis = 0;
	int k = 0;

	if(ps->num == 0)
	{
		for(i=0;i<resNum;i++)
		{
			for(j=0;j<ptf->num;j++)
				for(k=0;k<ptf->t[j]->field_num;k++)
				{
					printf("%s\t",result+dis);
					dis += (countFieldLen(k,ptf->t[j])+1);
				}
			printf("\n");
		}
	}
	else
	{
		for(i=0;i<resNum;i++)
		{
			for(j=0;j<ps->num;j++)
			{
				printf("%s\t",result+dis);
				dis += (countFieldLen(ps->sha[j].fNum,ptf->t[ps->sha[j].tNum])+1);
			}
			printf("\n");
		}
	}
}

//----------------------------------------------------------------------------------
int getOperatorType(char* ch)
{
    int    operatortype = 0;

	if(strcmp(ch,"=") == 0)
	{
		operatortype = 1;
	}
	else if(strcmp(ch,"<") == 0)
	{
		operatortype = 2;
	}
	else if(strcmp(ch,">") == 0)
	{
		operatortype = 3;
	}
	else
	{
		operatortype = -1;
	}

	return operatortype;
}

//------------------------------------------------------
bool isIncludeTable(char* tName,tableImf* ptf)
{
	int i = 0;
	for(;i<ptf->num;i++)
	{
		if(strcmp(ptf->t[i]->name,tName) == 0)
			return true;
	}

	return false;
}

//----------------------------------------------------------------



