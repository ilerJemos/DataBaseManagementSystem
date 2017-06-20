
#include "CreateTable.h"

extern char ch[20][20];
int getString(char ch[][20],bool isSrc,char* str);
bool SearchFile(char fname[]);
bool searchStr(char dest[][20],char* tName,int num);

bool getPassword(char* password);

bool lockS(char* tname);

bool unlockS(char* tname);

bool lockX(char* tname);

bool unlockX(char* tname);

void printProcess(char* str);

int selectFunction(void);

void showLocalTime(void);