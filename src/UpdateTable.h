
typedef struct
{
	int dis;
	int datatype;	//��������
	char cons[20];	//����
	int  kind;	//0:ֱ�����ã�1���Լ� 2���Լ�
}Ufield;	//�ֶΣ��У�

typedef struct
{
	Ufield uf[5];	//�ֶ�
	int num;	//�ֶ�����
}Uif;


bool UpdateTable(int strlen);

int getPosofStr(char ch[][20],char* dest,int strlen);

