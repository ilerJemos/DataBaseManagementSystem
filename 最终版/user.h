#ifndef _USER_H
#define _USER_H

typedef struct
{
	int id;
	char Name[20];
	int authority;//0:create 1:select 2:insert 3:delete

}UserImf;

#endif