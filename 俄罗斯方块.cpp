#include<time.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define Width 30   									//����߶� 
#define Hight 31									//������ 
void StartGame();   								//��ʼ��Ϸ
void HideCursor();  								//���ع��
void FinishGame();									//������Ϸ 
void RestartGame();									//���¿�ʼ��Ϸ 
void DiamondShape();   								//��������״ 
void ReadInRecord();  								//д����߼�¼
void ReadOutTimes();								//��ȡ��Ϸ���� 
void ReadOutRecord();   							//��ȡ��߼�¼ 
void InterfaceText();								//��ʼ���������� 
void InitializeInterface();  						//��ʼ������
void MoveCursor(int x,int y); 						//�ƶ����
void InputPassword(char a[][20],int*n);				//�������� 
void DrawSpace(int kd,int r,int x,int y); 			//����ո� 
void DrawDiamond(int kd,int r,int x,int y); 		//������� 
void OperateDiamond(int*a,int*b,int*c,int*d);		//�ٿط��� 
int RemoveDiamond();								//�������� 
int ChooseMode(int n);								//ѡ����Ϸģʽ 
int ChangeColor(int a);   							//�ı���ɫ
int LoginPassword(int a);							//��¼���� 
int JudgeBottom(int n,int r,int x,int y);  			//�ж��Ƿ񵽵� 
int seed=0;											//ʱ������ 
int mode=0;											//��Ϸģʽ
int score=0;     									//��Ϸ����
int delay=0; 										//�ӳ�ʱ��
int total=0;										//��Ϸ�ܴ��� 
int ranking=0;										//��Ϸ����
int removetotal=0;									//��������  
int diamond[7][4][4][4];							//������״ 
int type[Hight][Width+10];							//�������� 
int color[Hight][Width+10];							//������ɫ 
typedef struct DATA
{
    int max;										//��߼�¼ 
	int gametimes;									//��Ϸ���� 
}MODE;
MODE Mode[4]={0};
//================================================================================
int main()//������ 
{   
	int a=0; 
	SetConsoleTitleA("����˹�������̨����Yxl");//���ÿ���̨������� 
    ChangeColor(7);
    system("mode con cols=80 lines=34");//����������ڴ�С 
	if(LoginPassword(a)<3)//�ж��Ƿ���������������ȷ���� 
	{
		RestartGame();//��ʼ��Ϸ 
		return 0;
	}
}
//=============================================================================== 
void StartGame()//��ʼ��Ϸ 
{   
	int n,t=0,x=0,y=Width/2+4,i,j;
	int*a=&n;int*c=&x;int*d=&y; 
    int r=0,*b=&r;
    DrawSpace(seed,r,4,Width+3);//�����Ӧ�Ŀո� 
    n=seed;   
    seed=rand()%7;//��������� 
    ChangeColor(seed);// ѡ�񷽿��Ӧ����ɫ 
    DrawDiamond(seed,r,4,Width+3);//�����Ӧ�ķ��� 
    while(1)
    {    
		ChangeColor(n);
        DrawDiamond(n,r,x,y);
        if(t==0)
            t=delay;//��ֵ�ӳ� 
        while(--t)
        {   
			if(kbhit()!=0)//�����뼴���� 
   			break;  
        }
        if(t==0)
        {
            if(JudgeBottom(n,r,x+1,y)==1)//���û�е��� 
            {   
				DrawSpace(n,r,x,y);
                x++;
            }
            else//������� 
            {
                for(i=0;i<4;i++)
                {
                    for(j=0;j<4;j++)
                    { 
                        if(diamond[n][r][i][j]==1)//������������ 
                        {   
                            type[x+i][y+j]=1;
                            color[x+i][y+j]=n;
                            while(RemoveDiamond());//�ж��Ƿ������������ 
                        }
                    }
                }
                return;
            }
        }
        else
        {       
			OperateDiamond(a,b,c,d);//�û�ͨ������ٿط��� 
		}
    }
}
//===============================================================================
void HideCursor()//���ع�� 
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//===============================================================================
void FinishGame()//������Ϸ 
{
	int i=0,n=0; 
  	system("cls");//���� 
   	ChangeColor(7);//�����������ɫ 
    ReadInRecord();//������Ϸ���� 
    Sleep(2000);//��ʾ���а� �ӳ� 
    system("cls");//���� 
    MoveCursor(Width+4,Hight/2-7);//��Ϸ�������� 
    printf("�� Ϸ �� �� ��");
    MoveCursor(Width+4,Hight/2-4);//�ƶ���굽ָ��λ�� 
    printf("������Ϸ����");
    MoveCursor(Width+3,Hight/2-2);
    printf("��Ϸ����: %4d",ranking);
    MoveCursor(Width+3,Hight/2);
    printf("��Ϸ����: %4d",score);
    MoveCursor(Width+3,Hight/2+2);
    printf("��������: %4d",removetotal);
    MoveCursor(Width+4,Hight/2+5);
	printf("����һ��   1");
	MoveCursor(Width+4,Hight/2+7);
	printf("�˳���Ϸ   0"); 
	MoveCursor(Width+3,Hight/2+9);
    printf("��ѡ��(1/0): ");
	for(i=0;i<999;i++)
    {
		scanf("%d",&n);//����1��0 
		if(n!=0&&n!=1)//������벻��1��0 
		{
			MoveCursor(Width-1,Hight/2+11+2*i);
			printf("�������,����������: ");
		}
		else
		break;
	}
    if(n==0)//�������0 
    {
	    exit(0);//�������� 
	}
	else if(n==1)//�������1 
	RestartGame();//���¿�ʼ��Ϸ 
}
//===============================================================================
void RestartGame()//���¿�ʼ��Ϸ 
{
	score=0;//������0 
	removetotal=0;//��������0 
	Sleep(500);//�ӳ� 
	system("cls");
	delay=ChooseMode(delay);//ѡ����Ϸģʽ 
	Sleep(500);
	system("cls");
	HideCursor();//���ع�� 
	srand(time(NULL));//��ʱ������ 
	ReadOutTimes(); //��ȡ��Ϸ���� 
	ReadOutRecord();//��ȡ��Ϸ��¼ 
	Mode[mode].gametimes++;//��ģʽ��Ϸ����+1 
	total++;//��Ϸ�ܴ���+1 
	InitializeInterface();//��ʼ������ 
	DiamondShape();//��������״ 
	seed=rand()%7;//��������� 
	while(1)
	{
	   StartGame();//��ʼ��Ϸ 
    }
}
//===============================================================================
void DiamondShape()//��7�ַ���4����ת״̬����һ����ά������ 
{                  //һά���������� ��ά��������ת״̬ 
    int i,j;       //��ά������ ��ά������ 
    for(i=0;i<3;i++)
    {
        diamond[0][0][0][i]=1;	  //T�� 
    	diamond[0][0][1][1]=1;    
        diamond[0][1][i][2]=1;	  //T��	
    	diamond[0][1][1][1]=1;
        diamond[0][2][2][i]=1;	  //T��
    	diamond[0][2][1][1]=1;		
        diamond[0][3][i][0]=1;    //T��
    	diamond[0][3][1][1]=1;
        diamond[1][0][0][0]=1;
    	diamond[1][0][1][i]=1;    //L��
        diamond[1][1][0][2]=1;
    	diamond[1][1][i][1]=1;    //L��
        diamond[1][2][2][2]=1;
    	diamond[1][2][1][i]=1;    //L��
        diamond[1][3][i][1]=1;
    	diamond[1][3][2][0]=1;    //L��
        diamond[2][0][0][2]=1;
    	diamond[2][0][1][i]=1;    //J��
        diamond[2][1][2][2]=1;
    	diamond[2][1][i][1]=1;    //J��
        diamond[2][2][2][0]=1;
    	diamond[2][2][1][i]=1;    //J��
        diamond[2][3][0][0]=1;
    	diamond[2][3][i][1]=1;    //J��
    }

    for(i=0;i<2;i++)
    {   
        diamond[3][0][1][i]=1;    //S��
        diamond[3][0][0][i+1]=1;
        diamond[3][1][i][1]=1;	  //S��
        diamond[3][1][i+1][2]=1;  
        diamond[3][2][2][i]=1;    //S��
        diamond[3][2][1][i+1]=1;
        diamond[3][3][i][0]=1;	  //S��
        diamond[3][3][i+1][1]=1;   
        diamond[4][0][0][i]=1;	  //Z��
        diamond[4][0][1][i+1]=1;  
        diamond[4][1][i][2]=1;	  //Z��
        diamond[4][1][i+1][1]=1;  
        diamond[4][2][1][i]=1;	  //Z��
        diamond[4][2][2][i+1]=1;  
        diamond[4][3][i][1]=1;    //Z��
        diamond[4][3][i+1][0]=1;  
    }
    for(j=0;j<4;j++)
      for(i=1;i<3;i++) 
        {
		diamond[5][j][0][i]=1;
        diamond[5][j][1][i]=1;	  //O��
        }
        
    for(i=0;i<4;i++)
    {
        diamond[6][0][1][i]=1;	  //I��
        diamond[6][1][i][2]=1;    //I��
        diamond[6][2][2][i]=1;    //I��
        diamond[6][3][i][1]=1;    //I��
    }
}
//===============================================================================
void ReadInRecord()
{
    int i,j,t,temp[100]={0};
	FILE *fp1,*fp2;
    
	switch(mode)//�򿪶�Ӧģʽ�ķ�����¼ 
	{
		case 0:fp1=fopen("TetrisScore0.txt","a");break;
		case 1:fp1=fopen("TetrisScore1.txt","a");break;
		case 2:fp1=fopen("TetrisScore2.txt","a");break;
		case 3:fp1=fopen("TetrisScore3.txt","a");break;
		fseek(fp1,0,SEEK_END);//��λ��β�� 
		default:break;
	}
	fprintf(fp1,"%d,",score);//������д���ļ� 
	fclose(fp1);//�ر��ļ� 
	switch(mode)//�򿪶�Ӧģʽ����Ϸ������¼ 
	{
		case 0:fp1=fopen("TetrisScore0.txt","r");break;
		case 1:fp1=fopen("TetrisScore1.txt","r");break;
		case 2:fp1=fopen("TetrisScore2.txt","r");break;
		case 3:fp1=fopen("TetrisScore3.txt","r");break;
		default:break;
	}
	for(i=0;i<Mode[mode].gametimes;i++)
	{
		fscanf(fp1,"%d,",&temp[i]);//��ȡ��Ϸ������¼ 
	}
	fclose(fp1);//�ر��ļ� 
	for(i=0;i<Mode[mode].gametimes-1;i++)//��ð���������µķ�����¼���� 
	{
		for(j=0;j<Mode[mode].gametimes-i-1;j++)
		{
			if(temp[j]<temp[j+1])
			{
				t=temp[j];
				temp[j]=temp[j+1];
				temp[j+1]=t;
			}
		}
	}
	MoveCursor(Width+3,Hight/2-10); 
	printf("��     ��     ��");
	for(i=0;i<Mode[mode].gametimes;i++)//������а� 
	{
		MoveCursor(Width+3,Hight/2-8+i);
		printf("%d\t   %6d",i+1,temp[i]);
	}
	for(i=0;i<Mode[mode].gametimes;i++)//�������� 
	{
		if(temp[i]==score)
		{
			ranking=i+1;
			break;
		}
	}
	switch(mode)
	{
		case 0:fp1=fopen("TetrisScore0.txt","w+");break;
		case 1:fp1=fopen("TetrisScore1.txt","w+");break;
		case 2:fp1=fopen("TetrisScore2.txt","w+");break;
		case 3:fp1=fopen("TetrisScore3.txt","w+");break;
		default:break;
	}
	for(i=0;i<Mode[mode].gametimes;i++)
	fprintf(fp1,"%d,",temp[i]);//����Ϸ����д���ļ� 
	fclose(fp1);
	switch(mode)
	{
		case 0:fp2=fopen("TetrisTimes0.txt","r+");break;
		case 1:fp2=fopen("TetrisTimes1.txt","r+");break;
		case 2:fp2=fopen("TetrisTimes2.txt","r+");break;
		case 3:fp2=fopen("TetrisTimes3.txt","r+");break;
		default:break;
    }
    fprintf(fp2,"%d,",Mode[mode].gametimes);//����Ϸ����д���ļ� 
	fclose(fp2);							
}
//===============================================================================
void ReadOutTimes()//��ȡ��Ϸ������¼ 
{
	int a[4]={0},i,temp=0;
	FILE *fp0,*fp1,*fp2,*fp3;
	fp0=fopen("TetrisTimes0.txt","r"); 
	if(fp0==NULL)
	{
        fp0=fopen("TetrisTimes0.txt","w");
	}
	fscanf(fp0,"%d,",&a[0]); 
	fclose(fp0);
	fp1=fopen("TetrisTimes1.txt","r");
	if(fp1==NULL)
	{
        fp1=fopen("TetrisTimes1.txt","w");
	}
	fscanf(fp1,"%d,",&a[1]);
	fclose(fp1);
	fp2=fopen("TetrisTimes2.txt","r");
	if(fp2==NULL)
	{
        fp2=fopen("TetrisTimes2.txt","w");
	}
	fscanf(fp2,"%d,",&a[2]);
	fclose(fp2);
	fp3=fopen("TetrisTimes3.txt","r");
	if(fp3==NULL)
	{
        fp3=fopen("TetrisTimes3.txt","w");
	}
	fscanf(fp3,"%d,",&a[3]);
	fclose(fp3); 
    for(i=0;i<4;i++)
    temp+=a[i];
    total=temp;//������Ϸ�ܴ��� 
}
//===============================================================================
void ReadOutRecord()//��ȡ��Ϸ������¼ 
{
	FILE *fp1,*fp2;
    switch(mode) 
	{ 
        case 0:
		{
	        fp1=fopen("TetrisScore0.txt","r");	
	        if(fp1==NULL)
            {
                fp1=fopen("TetrisScore0.txt","w");
            }
            fscanf(fp1,"%d,",&Mode[mode].max);
			fclose(fp1);
			fp2=fopen("TetrisTimes0.txt","r");
			if(fp2==NULL)
			{
		        fp2=fopen("TetrisTimes0.txt","w");
	        }
			fscanf(fp2,"%d,",&Mode[mode].gametimes);
			fclose(fp2);
			break;
    	}
    	case 1:
		{
	        fp1=fopen("TetrisScore1.txt","r");	
	        if(fp1==NULL)
            {
                fp1=fopen("TetrisScore1.txt","w");
            }
            fscanf(fp1,"%d,",&Mode[mode].max);
			fclose(fp1);
			fp2=fopen("TetrisTimes1.txt","r");
			if(fp2==NULL)
			{
		        fp2=fopen("TetrisTimes1.txt","w");
	        }
			fscanf(fp2,"%d,",&Mode[mode].gametimes);
			fclose(fp2);
			break;
    	}
    	case 2:
		{
	        fp1=fopen("TetrisScore2.txt","r");	
	        if(fp1==NULL)
            {
                fp1=fopen("TetrisScore2.txt","w");
            }
            fscanf(fp1,"%d,",&Mode[mode].max);
			fclose(fp1);
			fp2=fopen("TetrisTimes2.txt","r");
			if(fp2==NULL)
			{
		        fp2=fopen("TetrisTimes2.txt","w");
	        }
			fscanf(fp2,"%d,",&Mode[mode].gametimes);
			fclose(fp2);
			break;
    	}
    	case 3:
		{
	        fp1=fopen("TetrisScore3.txt","r");	
	        if(fp1==NULL)
            {
                fp1=fopen("TetrisScore3.txt","w");
            }
            fscanf(fp1,"%d,",&Mode[mode].max);
			fclose(fp1);
			fp2=fopen("TetrisTimes3.txt","r");
			if(fp2==NULL)
			{
		        fp2=fopen("TetrisTimes3.txt","w");

	        }
			fscanf(fp2,"%d,",&Mode[mode].gametimes);
			fclose(fp2);
			break;
		default:break;	
    	}
    }
}
//===============================================================================
void InterfaceText()//��ʼ���������� 
{
	MoveCursor(2*Width+2,1);
    printf("�� ��һ��  ");
    MoveCursor(2*Width+2,Hight-22); 
    printf("�� ��  ¼  ");
	MoveCursor(2*Width+2,Hight-20);
    switch(mode)
    {
    	case 0:printf("��Ϸģʽ:��");break;
		case 1:printf("��Ϸģʽ:һ��");break; 
		case 2:printf("��Ϸģʽ:����");break; 
		case 3:printf("��Ϸģʽ:��̬");break;
		default:break;  
	}
	MoveCursor(2*Width+2,Hight-18);
    printf("��߼�¼:%4d",Mode[mode].max);
    MoveCursor(2*Width+2,Hight-16);
    printf("��Ϸ����:%4d",score);
    MoveCursor(2*Width+2,Hight-14);
    printf("��������:%4d",removetotal);
    MoveCursor(2*Width+2,Hight-12);
    printf("��Ϸ����:%4d",Mode[mode].gametimes);
    MoveCursor(2*Width+2,Hight-10);
    printf("��Ϸ�ܴ�:%4d",total);
    MoveCursor(2*Width+2,Hight-8);
    printf("���ѧ��:����ɳ");
    MoveCursor(2*Width+2,Hight-6);
    printf("ָ����ʦ:˹����",total);
    MoveCursor(4,Hight-22);
    printf("�� ��  ��  ");
	MoveCursor(4,Hight-20);
    printf("�����ƶ�:��/A");
    MoveCursor(4,Hight-18);
    printf("�����ƶ�:��/D");
    MoveCursor(4,Hight-16);
    printf("��ת����:��/W");
    MoveCursor(4,Hight-14);
    printf("��������:��/S");
    MoveCursor(4,Hight-12);
    printf("ֱ�����:�ո�");
    MoveCursor(4,Hight-10);
    printf("��ͣ����: Tab");
    MoveCursor(4,Hight-8);
    printf("���¿�ʼ:�س�");
	MoveCursor(4,Hight-6);
    printf("�˳���Ϸ: ESC");
    MoveCursor(11,Hight);
	printf("���Ʋ�����Ϸ���ܾ�������Ϸ��ע�����ұ�����������ƭ�ϵ���");
 	MoveCursor(11,Hight+1);
 	printf("�ʶ���Ϸ���ԣ�������Ϸ����������ʱ�䣬���ܽ������");
} 
//===============================================================================
void InitializeInterface()//��ʼ����Ϸ���� 
{   
	ChangeColor(7); 
	int i,j;
    for(i=0;i<Hight;i++)//����Ϸ����߽� 
    {   
		for(j=0;j<Width+10;j++)
        {   
			if(j==0||j==10||j==Width-1||j==Width+9)
            {   
				type[i][j]=2;
                MoveCursor(2*j,i);
                printf("��");
            }
            else if(i==Hight-1)
            {   
				type[i][j]=1;
                MoveCursor(2*j,i);
                printf("��");
            }
            else
            type[i][j]=0;
        }
    }
    InterfaceText();//��ʼ���������� 
}
//===============================================================================
void MoveCursor(int x,int y)//�ƶ���� 
{
    COORD z;	
    z.X=x;
    z.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),z);
}
//===============================================================================
void InputPassword(char a[][20],int*n)//�������� 
{
	int i=0;
	do
	{
 	    MoveCursor(Width+6+i,12+4*(*n));
        a[*n][i]=getch();//�����ַ��� 
        if(a[*n][i]=='\r')//������س� 
        break;
        if(a[*n][i]=='\b')//�������˸� 
       	{
		    if(i==0)
     	    {
                printf("\a");//���� 
                continue;
         	}
         	i--;
         	MoveCursor(Width+6+i,12+4*(*n));//�������ǰ��һ�� 
         	a[*n][i]=NULL;//ɾ���ո�������ַ� 
         	printf(" ");//ɾ��*�� 
	    }
    	else
    	{
            i++;
            printf("*");//���*�� 
	    }
	}while(a[*n][i]!='\n' && i<20);
    a[*n][i]='\0'; 
}
//===============================================================================
void DrawSpace(int kd,int r,int x,int y)//����ո� 
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            MoveCursor(2*(y+j),x+i);
            if(diamond[kd][r][i][j]==1)
            printf("  ");
        }
    }

}
//===============================================================================
void DrawDiamond(int kd,int r,int x,int y)//������� 
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            MoveCursor(2*(y+j),x+i);
            if(diamond[kd][r][i][j]==1)
            printf("��");
        }
    }
}
//===============================================================================
void OperateDiamond(int*a,int*b,int*c, int*d)//�ٿط��� 
{
 	int key,i,m=1,n;
 	key=getch();//���� 
	switch(key)
  	{
   	    case   75: //�� 
		case  'A':
		case  'a': if(JudgeBottom(*a,*b,*c,(*d)-1)==1)//�ж��ܷ����� 
  			  	   {   
					   DrawSpace(*a,*b,*c,*d);
 					   (*d)--;
	               }
                   break;
        case   77: //��
		case  'D':
		case  'd': if(JudgeBottom(*a,*b,*c,(*d)+1)==1)//�ж��ܷ����� 
                   {   
				   	   DrawSpace(*a,*b,*c,*d);
					   (*d)++;
                   }
                   break;
        case   32: if(JudgeBottom(*a,*b,(*c)+1,*d)==1)//�ж��Ƿ񴥵� 
			  	   {   
  					   DrawSpace(*a,*b,*c,*d);
  					   for(i=0;i<Hight;i++)
  					   if(JudgeBottom(*a,*b,(*c)+i,*d)==0) 
  					   {
  					   	   
                           (*c)+=(i-1);//ֱ�ӵ��� 
                           break;
		               }
   				   }
       			   break;           
        case   80://��
		case  'S':
		case  's': if(JudgeBottom(*a,*b,(*c)+1,*d)==1)//�ж��ܷ��½� 
			  	   {   
				   	   DrawSpace(*a,*b,*c,*d);
  					   (*c)++;
   				   }
       			   break;
        case   72: //��
		case  'W':	
		case  'w': if(JudgeBottom(*a,((*b)+1)%4,(*c)+1,*d)==1)//�ж��Ƿ񵽵� 
			  	   {   
		               DrawSpace(*a,*b,*c,*d);
					   *b=((*b)+1)%4;
  			       }
   			 	   break;
		case   27: system("cls");//ESC ������Ϸ 
		           MoveCursor(Width+3,Hight/2);
		           printf("�� Ϸ �� �� !");
	               Sleep(500);
				   MoveCursor(0,Hight);
				   exit(0); 
    			   break;
		case   13: RestartGame();//�س� ���¿�ʼ��Ϸ 
			   	   exit(0);
		case    9: while(m)//Tab ��ͣ��Ϸ 
			   	   {   
					   n=getch();
					   switch(n)
					   {
					   	  case 9:m=0;break;//������Tab,������ѭ�� 
					      default:m=1;break;//����������,�����ѭ�� 
					   }
                   }
 				   break;
	}
}
//===============================================================================
int RemoveDiamond()//���� 
{
    int i,j,k,sum;
    for(i=Hight-2;i>4;i--)
    {
        sum=0;
        for(j=11;j<Width-1;j++)
        {
            sum+=type[i][j];   
        }
        if(sum==0)
            break;
        if(sum==Width-12)//�������������� 
        {   
            score+=10;//����+10 
            removetotal++;//������+1 
            ChangeColor(7);
            MoveCursor(2*Width+2,Hight-16);
    		printf("��Ϸ����:%4d",score);//������Ϸ���� 
    		MoveCursor(2*Width+2,Hight-14);
            printf("���д���:%4d",removetotal);//���������� 
            for(j=11;j<Width-1;j++)
            {
                type[i][j]=0;
                MoveCursor(2*j,i);
                printf(" ");//�������� 
            }
            for(j=i;j>1;j--)
            {   
				sum=0;
                for(k=11;k<Width-1;k++)//�����淽������ 
                {
                    sum+=type[j-1][k]+type[j][k];
                    type[j][k]=type[j-1][k];
                    if(type[j][k]==0)
                    {
                        MoveCursor(2*k,j);
                        printf(" ");//����ո� 
                    }
                    else
                    {
                        MoveCursor(2*k,j);
                        ChangeColor(color[j][k]);
                        printf("��");//������� 
                    }
                }
                if(sum==0)
                    return 1;
            }

        }
    }
    for(i=11;i<Width-1;i++)//������ 
    {
        if(type[1][i]==1)
        {
            FinishGame();//������Ϸ 
        }       
    }
    return 0;
}
//===============================================================================
int ChooseMode(int n)//ѡ����Ϸģʽ 
{
	ChangeColor(7); 
	int i,s;
	HideCursor();//���ع�� 
	MoveCursor(Width+3,7);
	printf("ģ ʽ ѡ ��"); 
	MoveCursor(Width-1,10);
	printf("0\t\t     ����");
	MoveCursor(Width-1,11);
	printf("1\t\t     һ��");
	MoveCursor(Width-1,12);
	printf("2\t\t     ����");
	MoveCursor(Width-1,13);
	printf("3\t\t     ��̬");
	MoveCursor(Width-1,14);
    printf("��������Ϸģʽ(0~3):\n\n");
	for(i=0;i<999;i++)//������Ϸģʽ 
    {
    	printf("\t\t\t\t      ");
		scanf("%d",&s);
    	if(s>(-1)&&s<4)
    	{
 			mode=s;//��ֵ��Ӧ��Ϸģʽ 
	        switch(s)//������������ѡ���Ӧ��Ϸģʽ���ӳ�ʱ�� 
    		{
                case 0:n=10000;break;
    			case 1:n=5000;break;
    			case 2:n=2000;break;
    			case 3:n=500;break;
	            default:break;
			}
	    break;    
	    }
		else
		printf("\n\t\t\t     �������,����������:\n\n");
	}
	return n;//�����ӳ�ʱ�� 
}
//===============================================================================
int ChangeColor(int a)//�ı����������ɫ 
{
    switch(a)
    {
    case 0: a= 9;break;//�� 
    case 1: 
    case 2: a=10;break;//�� 
    case 3: 
    case 4: a=12;break;//�� 
    case 5:	a=13;break;//�� 
    case 6: a=14;break;//�� 
    default:a=11;break;//�� 
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
    return 0;
}
//===============================================================================
int LoginPassword(int a)//��¼���� 
{
	int j,k=0;
	char p[3][20];
	char password[]="123456";//��������   
	HideCursor();
    MoveCursor(Width,10);
    printf("��������Ϸ��¼����:\n");
	for(j=0;j<3;j++)//��������������� 
	{
		MoveCursor(Width+6,12+4*j);
		InputPassword(p,&j);//�������� 
		MoveCursor(Width-2,14+4*j);
		if(strcmp(p[j],password)==0)//��������ȷ 
        {
			printf("������ȷ����Ϸ��¼�ɹ�!");
    	    break;
		}
		else
		{	
			k++;
			if(k==1||k==2)//�������һ�λ����� 
			{
				printf(" �����������������:");
			}
			if(k==3)//�������붼��� 
			{
 		        printf("  ��������������þ���");
				break;
			}
	    }
    }
    return k;
}
//===============================================================================
int JudgeBottom(int n,int r,int x,int y)//�ж��Ƿ񵽵� 
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
           if(diamond[n][r][i][j]==0)
               continue;
           else if(type[x+i][y+j]==2||type[x+i][y+j]==1)
               return 0;
        }
    }
    return 1;
}
