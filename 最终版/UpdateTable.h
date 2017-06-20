
typedef struct
{
	int dis;
	int datatype;	//数据类型
	char cons[20];	//列名
	int  kind;	//0:直接设置，1：自加 2：自减
}Ufield;	//字段（列）

typedef struct
{
	Ufield uf[5];	//字段
	int num;	//字段数量
}Uif;


bool UpdateTable(int strlen);

int getPosofStr(char ch[][20],char* dest,int strlen);

