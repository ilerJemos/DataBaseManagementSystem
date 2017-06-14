#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Index.h"




int bSearch(INDEX* id,char key[],int start_no);

//初始化索引
bool initIndex(INDEX* id,int knum,int keytype)
{
	int i=0;
	int j=0;
	id->trecord = 0;
	id->key_num = knum;

	id->ktype[0] = keytype;
	/*for(;i<pt->field_num;i++)
	{
		if(pt->field[i].pkey)
		{
			id->ktype[j]= pt->field[i].datatype;
			j++;
			break;
		}
	}*/
	return true;
}

//添加索引
bool addInode(INDEX* id,int rn,char key[])	
{
	int inp;
	int i = 0;
	if((id->trecord+1) >= INDEX_NUM)
	{
		printf("Index file is full!\n");
		return false;
	}
	int val = bSearch(id,key,0);
	if(val == -1)
	{
		printf("Index file is empty!\n");
		inp = val+1;
	}
	else
	{
		if(id->ktype[0] == 1)
		{
			if(strcmp(id->node[val].keyWord[0],key) == 0||
				strcmp(id->node[val].keyWord[0],key) == -1)
			{
				inp = val+1;
			}
			else
			{
				inp = val-1;
			}
		}

		else if(id->ktype[0] == 2)
		{
			int i1 = 0;
			int i2 = 0;
			i1 = atoi(id->node[val].keyWord[0]);
			i2 = atoi(key);
			if((i1==i2)||(i1<i2))
			{
				inp = val+1;
			}
		
			else
			{
				inp = val;
			}
		}
	
	
		else
		{
			float f1 = 0;
			float f2 = 0;
			f1 = atof(id->node[val].keyWord[0]);
			f2 = atof(key);
			if((f1==f2)||(f1<f2))
			{
				inp = val+1;
			}
			else
			{
				inp = val-1;
			}
		}

	}
	

	if(id->key_num == 2)
	{
		for(i=id->trecord-1;i>=inp;i--)
		{
			strcpy(id->node[i+1].keyWord[0],id->node[i].keyWord[0]);
			strcpy(id->node[i+1].keyWord[1],id->node[i].keyWord[1]);
			id->node[i+1].rec_num = id->node[i].rec_num;
		}
		strcpy(id->node[inp].keyWord[0],key);
		strcpy(id->node[inp].keyWord[1],key+strlen(key)+1);

	}
	else
	{
		for(i=id->trecord-1;i>=inp;i--)
		{
			strcpy(id->node[i+1].keyWord[0],id->node[i].keyWord[0]);
			id->node[i+1].rec_num = id->node[i].rec_num;
		}
		strcpy(id->node[inp].keyWord[0],key);
	}
    
	id->node[inp].rec_num = rn;

	id->trecord++;
	return true;
}

//删除记录
bool delARecord(INDEX* id,int delno)
{
	if(id->trecord == delno)//删除最后一个
	{
		id->trecord --;
		return true;
	}
	if((id->trecord-1) < delno)
	{
		printf("Deleting node can't exsit!\n");
		return false;
	}
	int i = 0;
	for(i = delno;i<id->trecord-1;i++)
	{
		strcpy(id->node[i].keyWord[0],id->node[i+1].keyWord[0]);
		id->node[i].rec_num = id->node[i+1].rec_num; 
	}
	id->trecord --;
	return true;
}

//查询位置
int serchPositon(INDEX* id,char key[])
{
	int i = 0;
	if(id->ktype[0] == 1)
	{
		
		for( i=0;i<id->trecord;i++)
		{
			if(strcmp(id->node[i].keyWord[0],key) == 1)
			{
				return i;
			}
		}
		return i;
	}
	else if(id->ktype[0] == 2)
	{
		int t1;
		int t2;
		for(i=0;i<id->trecord;i++)
		{
		    t1 = atoi(id->node[i].keyWord[0]);
			t2 = atoi(key);
			if(t1>t2)
			{
				return i;
			}
		}
		return i;
	}
	else
	{
		float f1;
		float f2;
		for(i=0;i<id->trecord;i++)
		{
			f1 = atof(id->node[i].keyWord[0]);
			f2 = atof(key);
			if(f1>f2)
				return i;
		}
		return i;
	}
}

//B树搜索
int bSearch(INDEX* id,char key[],int start_no)
{
	int low = start_no;
	int high = id->trecord-1;
	int mid = -1;			//Index file is empty.
	if(id->ktype[0] == 1)
	{
		while(low <= high)
		{
			mid = (low+high)/2;
			if(strcmp(id->node[mid].keyWord[0],key) == 0)
				return mid;
			else if(strcmp(id->node[mid].keyWord[0],key) == 1)
			{
				high = mid - 1;
			}
			else
			{
				low = mid + 1;
			}
		}
	}
	else if(id->ktype[0] == 2)
	{
		
		int i1;
		int i2;
		while(low <= high)
		{
			mid = (low+high)/2;
			i1  = atoi(id->node[mid].keyWord[0]);
			i2  = atoi(key);
			if(i1 == i2)
				return mid;
			else if(i1 > i2)
				high = mid -1;
			else
				low  = mid + 1;
		}
	}
	else
	{
		float f1;
		float f2;
		while(low <= high)
		{
			mid = (low+high)/2;
			f1  = atof(id->node[mid].keyWord[0]);
			f2  = atof(key);
			if(f1 == f2)
				return mid;
			else if(f1 > f2)
				high = mid -1;
			else
				low  = mid + 1;
		}
	}

	return mid;
}