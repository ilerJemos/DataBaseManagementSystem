



typedef struct logc
{
	char content[80];
	logc* next;
	logc* pre;
}Logitem;


typedef struct 
{
	char name[10];
	int num;
}Task;

bool Recovery(void);