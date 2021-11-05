#include<time.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define Width 30   									//界面高度 
#define Hight 31									//界面宽度 
void StartGame();   								//开始游戏
void HideCursor();  								//隐藏光标
void FinishGame();									//结束游戏 
void RestartGame();									//重新开始游戏 
void DiamondShape();   								//画方块形状 
void ReadInRecord();  								//写入最高纪录
void ReadOutTimes();								//读取游戏次数 
void ReadOutRecord();   							//读取最高记录 
void InterfaceText();								//初始化界面文字 
void InitializeInterface();  						//初始化界面
void MoveCursor(int x,int y); 						//移动光标
void InputPassword(char a[][20],int*n);				//输入密码 
void DrawSpace(int kd,int r,int x,int y); 			//输出空格 
void DrawDiamond(int kd,int r,int x,int y); 		//输出方块 
void OperateDiamond(int*a,int*b,int*c,int*d);		//操控方块 
int RemoveDiamond();								//消除方块 
int ChooseMode(int n);								//选择游戏模式 
int ChangeColor(int a);   							//改变颜色
int LoginPassword(int a);							//登录密码 
int JudgeBottom(int n,int r,int x,int y);  			//判断是否到底 
int seed=0;											//时间种子 
int mode=0;											//游戏模式
int score=0;     									//游戏分数
int delay=0; 										//延迟时间
int total=0;										//游戏总次数 
int ranking=0;										//游戏排名
int removetotal=0;									//消行总数  
int diamond[7][4][4][4];							//方块形状 
int type[Hight][Width+10];							//方块类型 
int color[Hight][Width+10];							//方块颜色 
typedef struct DATA
{
    int max;										//最高记录 
	int gametimes;									//游戏次数 
}MODE;
MODE Mode[4]={0};
//================================================================================
int main()//主函数 
{   
	int a=0; 
	SetConsoleTitleA("俄罗斯方块控制台——Yxl");//设置控制台输出文字 
    ChangeColor(7);
    system("mode con cols=80 lines=34");//设置输出窗口大小 
	if(LoginPassword(a)<3)//判断是否在三次内输入正确密码 
	{
		RestartGame();//开始游戏 
		return 0;
	}
}
//=============================================================================== 
void StartGame()//开始游戏 
{   
	int n,t=0,x=0,y=Width/2+4,i,j;
	int*a=&n;int*c=&x;int*d=&y; 
    int r=0,*b=&r;
    DrawSpace(seed,r,4,Width+3);//输出相应的空格 
    n=seed;   
    seed=rand()%7;//产生随机数 
    ChangeColor(seed);// 选择方块对应的颜色 
    DrawDiamond(seed,r,4,Width+3);//输出相应的方块 
    while(1)
    {    
		ChangeColor(n);
        DrawDiamond(n,r,x,y);
        if(t==0)
            t=delay;//赋值延迟 
        while(--t)
        {   
			if(kbhit()!=0)//有输入即跳出 
   			break;  
        }
        if(t==0)
        {
            if(JudgeBottom(n,r,x+1,y)==1)//如果没有到底 
            {   
				DrawSpace(n,r,x,y);
                x++;
            }
            else//如果到底 
            {
                for(i=0;i<4;i++)
                {
                    for(j=0;j<4;j++)
                    { 
                        if(diamond[n][r][i][j]==1)//如果该输出方块 
                        {   
                            type[x+i][y+j]=1;
                            color[x+i][y+j]=n;
                            while(RemoveDiamond());//判断是否符合消行条件 
                        }
                    }
                }
                return;
            }
        }
        else
        {       
			OperateDiamond(a,b,c,d);//用户通过输入操控方块 
		}
    }
}
//===============================================================================
void HideCursor()//隐藏光标 
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//===============================================================================
void FinishGame()//结束游戏 
{
	int i=0,n=0; 
  	system("cls");//清屏 
   	ChangeColor(7);//换输出字体颜色 
    ReadInRecord();//读入游戏数据 
    Sleep(2000);//显示排行榜 延迟 
    system("cls");//清屏 
    MoveCursor(Width+4,Hight/2-7);//游戏结束界面 
    printf("游 戏 结 束 ！");
    MoveCursor(Width+4,Hight/2-4);//移动光标到指定位置 
    printf("本次游戏数据");
    MoveCursor(Width+3,Hight/2-2);
    printf("游戏排名: %4d",ranking);
    MoveCursor(Width+3,Hight/2);
    printf("游戏分数: %4d",score);
    MoveCursor(Width+3,Hight/2+2);
    printf("消行总数: %4d",removetotal);
    MoveCursor(Width+4,Hight/2+5);
	printf("再来一局   1");
	MoveCursor(Width+4,Hight/2+7);
	printf("退出游戏   0"); 
	MoveCursor(Width+3,Hight/2+9);
    printf("请选择(1/0): ");
	for(i=0;i<999;i++)
    {
		scanf("%d",&n);//输入1或0 
		if(n!=0&&n!=1)//如果输入不是1或0 
		{
			MoveCursor(Width-1,Hight/2+11+2*i);
			printf("输入错误,请重新输入: ");
		}
		else
		break;
	}
    if(n==0)//如果输入0 
    {
	    exit(0);//结束程序 
	}
	else if(n==1)//如果输入1 
	RestartGame();//重新开始游戏 
}
//===============================================================================
void RestartGame()//重新开始游戏 
{
	score=0;//分数置0 
	removetotal=0;//消行数置0 
	Sleep(500);//延迟 
	system("cls");
	delay=ChooseMode(delay);//选择游戏模式 
	Sleep(500);
	system("cls");
	HideCursor();//隐藏光标 
	srand(time(NULL));//播时间种子 
	ReadOutTimes(); //读取游戏次数 
	ReadOutRecord();//读取游戏记录 
	Mode[mode].gametimes++;//该模式游戏次数+1 
	total++;//游戏总次数+1 
	InitializeInterface();//初始化界面 
	DiamondShape();//画方块形状 
	seed=rand()%7;//产生随机数 
	while(1)
	{
	   StartGame();//开始游戏 
    }
}
//===============================================================================
void DiamondShape()//将7种方块4种旋转状态存入一个四维数组中 
{                  //一维代表方块种类 二维代表方块旋转状态 
    int i,j;       //三维代表行 四维代表列 
    for(i=0;i<3;i++)
    {
        diamond[0][0][0][i]=1;	  //T形 
    	diamond[0][0][1][1]=1;    
        diamond[0][1][i][2]=1;	  //T形	
    	diamond[0][1][1][1]=1;
        diamond[0][2][2][i]=1;	  //T形
    	diamond[0][2][1][1]=1;		
        diamond[0][3][i][0]=1;    //T形
    	diamond[0][3][1][1]=1;
        diamond[1][0][0][0]=1;
    	diamond[1][0][1][i]=1;    //L形
        diamond[1][1][0][2]=1;
    	diamond[1][1][i][1]=1;    //L形
        diamond[1][2][2][2]=1;
    	diamond[1][2][1][i]=1;    //L形
        diamond[1][3][i][1]=1;
    	diamond[1][3][2][0]=1;    //L形
        diamond[2][0][0][2]=1;
    	diamond[2][0][1][i]=1;    //J形
        diamond[2][1][2][2]=1;
    	diamond[2][1][i][1]=1;    //J形
        diamond[2][2][2][0]=1;
    	diamond[2][2][1][i]=1;    //J形
        diamond[2][3][0][0]=1;
    	diamond[2][3][i][1]=1;    //J形
    }

    for(i=0;i<2;i++)
    {   
        diamond[3][0][1][i]=1;    //S形
        diamond[3][0][0][i+1]=1;
        diamond[3][1][i][1]=1;	  //S形
        diamond[3][1][i+1][2]=1;  
        diamond[3][2][2][i]=1;    //S形
        diamond[3][2][1][i+1]=1;
        diamond[3][3][i][0]=1;	  //S形
        diamond[3][3][i+1][1]=1;   
        diamond[4][0][0][i]=1;	  //Z形
        diamond[4][0][1][i+1]=1;  
        diamond[4][1][i][2]=1;	  //Z形
        diamond[4][1][i+1][1]=1;  
        diamond[4][2][1][i]=1;	  //Z形
        diamond[4][2][2][i+1]=1;  
        diamond[4][3][i][1]=1;    //Z形
        diamond[4][3][i+1][0]=1;  
    }
    for(j=0;j<4;j++)
      for(i=1;i<3;i++) 
        {
		diamond[5][j][0][i]=1;
        diamond[5][j][1][i]=1;	  //O形
        }
        
    for(i=0;i<4;i++)
    {
        diamond[6][0][1][i]=1;	  //I形
        diamond[6][1][i][2]=1;    //I形
        diamond[6][2][2][i]=1;    //I形
        diamond[6][3][i][1]=1;    //I形
    }
}
//===============================================================================
void ReadInRecord()
{
    int i,j,t,temp[100]={0};
	FILE *fp1,*fp2;
    
	switch(mode)//打开对应模式的分数记录 
	{
		case 0:fp1=fopen("TetrisScore0.txt","a");break;
		case 1:fp1=fopen("TetrisScore1.txt","a");break;
		case 2:fp1=fopen("TetrisScore2.txt","a");break;
		case 3:fp1=fopen("TetrisScore3.txt","a");break;
		fseek(fp1,0,SEEK_END);//定位到尾部 
		default:break;
	}
	fprintf(fp1,"%d,",score);//将分数写入文件 
	fclose(fp1);//关闭文件 
	switch(mode)//打开对应模式的游戏次数记录 
	{
		case 0:fp1=fopen("TetrisScore0.txt","r");break;
		case 1:fp1=fopen("TetrisScore1.txt","r");break;
		case 2:fp1=fopen("TetrisScore2.txt","r");break;
		case 3:fp1=fopen("TetrisScore3.txt","r");break;
		default:break;
	}
	for(i=0;i<Mode[mode].gametimes;i++)
	{
		fscanf(fp1,"%d,",&temp[i]);//读取游戏分数记录 
	}
	fclose(fp1);//关闭文件 
	for(i=0;i<Mode[mode].gametimes-1;i++)//用冒泡排序将最新的分数记录排序 
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
	printf("排     行     榜");
	for(i=0;i<Mode[mode].gametimes;i++)//输出排行榜 
	{
		MoveCursor(Width+3,Hight/2-8+i);
		printf("%d\t   %6d",i+1,temp[i]);
	}
	for(i=0;i<Mode[mode].gametimes;i++)//计算排名 
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
	fprintf(fp1,"%d,",temp[i]);//将游戏分数写入文件 
	fclose(fp1);
	switch(mode)
	{
		case 0:fp2=fopen("TetrisTimes0.txt","r+");break;
		case 1:fp2=fopen("TetrisTimes1.txt","r+");break;
		case 2:fp2=fopen("TetrisTimes2.txt","r+");break;
		case 3:fp2=fopen("TetrisTimes3.txt","r+");break;
		default:break;
    }
    fprintf(fp2,"%d,",Mode[mode].gametimes);//将游戏次数写入文件 
	fclose(fp2);							
}
//===============================================================================
void ReadOutTimes()//读取游戏次数记录 
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
    total=temp;//计算游戏总次数 
}
//===============================================================================
void ReadOutRecord()//读取游戏分数记录 
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
void InterfaceText()//初始化界面文字 
{
	MoveCursor(2*Width+2,1);
    printf("■ 下一块  ");
    MoveCursor(2*Width+2,Hight-22); 
    printf("■ 记  录  ");
	MoveCursor(2*Width+2,Hight-20);
    switch(mode)
    {
    	case 0:printf("游戏模式:简单");break;
		case 1:printf("游戏模式:一般");break; 
		case 2:printf("游戏模式:困难");break; 
		case 3:printf("游戏模式:变态");break;
		default:break;  
	}
	MoveCursor(2*Width+2,Hight-18);
    printf("最高纪录:%4d",Mode[mode].max);
    MoveCursor(2*Width+2,Hight-16);
    printf("游戏分数:%4d",score);
    MoveCursor(2*Width+2,Hight-14);
    printf("消行总数:%4d",removetotal);
    MoveCursor(2*Width+2,Hight-12);
    printf("游戏次数:%4d",Mode[mode].gametimes);
    MoveCursor(2*Width+2,Hight-10);
    printf("游戏总次:%4d",total);
    MoveCursor(2*Width+2,Hight-8);
    printf("设计学生:阿廖沙");
    MoveCursor(2*Width+2,Hight-6);
    printf("指导教师:斯穆莱",total);
    MoveCursor(4,Hight-22);
    printf("■ 控  制  ");
	MoveCursor(4,Hight-20);
    printf("向左移动:←/A");
    MoveCursor(4,Hight-18);
    printf("向右移动:→/D");
    MoveCursor(4,Hight-16);
    printf("旋转方块:↑/W");
    MoveCursor(4,Hight-14);
    printf("加速下落:↓/S");
    MoveCursor(4,Hight-12);
    printf("直接落地:空格");
    MoveCursor(4,Hight-10);
    printf("暂停继续: Tab");
    MoveCursor(4,Hight-8);
    printf("重新开始:回车");
	MoveCursor(4,Hight-6);
    printf("退出游戏: ESC");
    MoveCursor(11,Hight);
	printf("抵制不良游戏，拒绝盗版游戏。注意自我保护，谨防受骗上当。");
 	MoveCursor(11,Hight+1);
 	printf("适度游戏益脑，沉迷游戏伤身。合理安排时间，享受健康生活。");
} 
//===============================================================================
void InitializeInterface()//初始化游戏界面 
{   
	ChangeColor(7); 
	int i,j;
    for(i=0;i<Hight;i++)//画游戏界面边界 
    {   
		for(j=0;j<Width+10;j++)
        {   
			if(j==0||j==10||j==Width-1||j==Width+9)
            {   
				type[i][j]=2;
                MoveCursor(2*j,i);
                printf("■");
            }
            else if(i==Hight-1)
            {   
				type[i][j]=1;
                MoveCursor(2*j,i);
                printf("■");
            }
            else
            type[i][j]=0;
        }
    }
    InterfaceText();//初始化界面文字 
}
//===============================================================================
void MoveCursor(int x,int y)//移动光标 
{
    COORD z;	
    z.X=x;
    z.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),z);
}
//===============================================================================
void InputPassword(char a[][20],int*n)//输入密码 
{
	int i=0;
	do
	{
 	    MoveCursor(Width+6+i,12+4*(*n));
        a[*n][i]=getch();//输入字符串 
        if(a[*n][i]=='\r')//若输入回车 
        break;
        if(a[*n][i]=='\b')//若输入退格 
       	{
		    if(i==0)
     	    {
                printf("\a");//报警 
                continue;
         	}
         	i--;
         	MoveCursor(Width+6+i,12+4*(*n));//将光标往前移一格 
         	a[*n][i]=NULL;//删除刚刚输入的字符 
         	printf(" ");//删除*号 
	    }
    	else
    	{
            i++;
            printf("*");//输出*号 
	    }
	}while(a[*n][i]!='\n' && i<20);
    a[*n][i]='\0'; 
}
//===============================================================================
void DrawSpace(int kd,int r,int x,int y)//输出空格 
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
void DrawDiamond(int kd,int r,int x,int y)//输出方块 
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            MoveCursor(2*(y+j),x+i);
            if(diamond[kd][r][i][j]==1)
            printf("■");
        }
    }
}
//===============================================================================
void OperateDiamond(int*a,int*b,int*c, int*d)//操控方块 
{
 	int key,i,m=1,n;
 	key=getch();//键入 
	switch(key)
  	{
   	    case   75: //← 
		case  'A':
		case  'a': if(JudgeBottom(*a,*b,*c,(*d)-1)==1)//判断能否左移 
  			  	   {   
					   DrawSpace(*a,*b,*c,*d);
 					   (*d)--;
	               }
                   break;
        case   77: //→
		case  'D':
		case  'd': if(JudgeBottom(*a,*b,*c,(*d)+1)==1)//判断能否右移 
                   {   
				   	   DrawSpace(*a,*b,*c,*d);
					   (*d)++;
                   }
                   break;
        case   32: if(JudgeBottom(*a,*b,(*c)+1,*d)==1)//判断是否触底 
			  	   {   
  					   DrawSpace(*a,*b,*c,*d);
  					   for(i=0;i<Hight;i++)
  					   if(JudgeBottom(*a,*b,(*c)+i,*d)==0) 
  					   {
  					   	   
                           (*c)+=(i-1);//直接到底 
                           break;
		               }
   				   }
       			   break;           
        case   80://↓
		case  'S':
		case  's': if(JudgeBottom(*a,*b,(*c)+1,*d)==1)//判断能否下降 
			  	   {   
				   	   DrawSpace(*a,*b,*c,*d);
  					   (*c)++;
   				   }
       			   break;
        case   72: //↑
		case  'W':	
		case  'w': if(JudgeBottom(*a,((*b)+1)%4,(*c)+1,*d)==1)//判断是否到底 
			  	   {   
		               DrawSpace(*a,*b,*c,*d);
					   *b=((*b)+1)%4;
  			       }
   			 	   break;
		case   27: system("cls");//ESC 结束游戏 
		           MoveCursor(Width+3,Hight/2);
		           printf("游 戏 结 束 !");
	               Sleep(500);
				   MoveCursor(0,Hight);
				   exit(0); 
    			   break;
		case   13: RestartGame();//回车 重新开始游戏 
			   	   exit(0);
		case    9: while(m)//Tab 暂停游戏 
			   	   {   
					   n=getch();
					   switch(n)
					   {
					   	  case 9:m=0;break;//若输入Tab,则跳出循环 
					      default:m=1;break;//若输入其他,则继续循环 
					   }
                   }
 				   break;
	}
}
//===============================================================================
int RemoveDiamond()//消行 
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
        if(sum==Width-12)//若满足消行条件 
        {   
            score+=10;//分数+10 
            removetotal++;//消行数+1 
            ChangeColor(7);
            MoveCursor(2*Width+2,Hight-16);
    		printf("游戏分数:%4d",score);//更新游戏分数 
    		MoveCursor(2*Width+2,Hight-14);
            printf("消行次数:%4d",removetotal);//更新消行数 
            for(j=11;j<Width-1;j++)
            {
                type[i][j]=0;
                MoveCursor(2*j,i);
                printf(" ");//消除方块 
            }
            for(j=i;j>1;j--)
            {   
				sum=0;
                for(k=11;k<Width-1;k++)//将上面方块下移 
                {
                    sum+=type[j-1][k]+type[j][k];
                    type[j][k]=type[j-1][k];
                    if(type[j][k]==0)
                    {
                        MoveCursor(2*k,j);
                        printf(" ");//输出空格 
                    }
                    else
                    {
                        MoveCursor(2*k,j);
                        ChangeColor(color[j][k]);
                        printf("■");//输出方块 
                    }
                }
                if(sum==0)
                    return 1;
            }

        }
    }
    for(i=11;i<Width-1;i++)//若到顶 
    {
        if(type[1][i]==1)
        {
            FinishGame();//结束游戏 
        }       
    }
    return 0;
}
//===============================================================================
int ChooseMode(int n)//选择游戏模式 
{
	ChangeColor(7); 
	int i,s;
	HideCursor();//隐藏光标 
	MoveCursor(Width+3,7);
	printf("模 式 选 择"); 
	MoveCursor(Width-1,10);
	printf("0\t\t     容易");
	MoveCursor(Width-1,11);
	printf("1\t\t     一般");
	MoveCursor(Width-1,12);
	printf("2\t\t     困难");
	MoveCursor(Width-1,13);
	printf("3\t\t     变态");
	MoveCursor(Width-1,14);
    printf("请输入游戏模式(0~3):\n\n");
	for(i=0;i<999;i++)//输入游戏模式 
    {
    	printf("\t\t\t\t      ");
		scanf("%d",&s);
    	if(s>(-1)&&s<4)
    	{
 			mode=s;//赋值对应游戏模式 
	        switch(s)//根据输入数字选择对应游戏模式的延迟时间 
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
		printf("\n\t\t\t     输入错误,请重新输入:\n\n");
	}
	return n;//返回延迟时间 
}
//===============================================================================
int ChangeColor(int a)//改变输出字体颜色 
{
    switch(a)
    {
    case 0: a= 9;break;//蓝 
    case 1: 
    case 2: a=10;break;//绿 
    case 3: 
    case 4: a=12;break;//红 
    case 5:	a=13;break;//紫 
    case 6: a=14;break;//黄 
    default:a=11;break;//青 
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
    return 0;
}
//===============================================================================
int LoginPassword(int a)//登录密码 
{
	int j,k=0;
	char p[3][20];
	char password[]="123456";//密码明文   
	HideCursor();
    MoveCursor(Width,10);
    printf("请输入游戏登录密码:\n");
	for(j=0;j<3;j++)//三次输入密码机会 
	{
		MoveCursor(Width+6,12+4*j);
		InputPassword(p,&j);//输入密码 
		MoveCursor(Width-2,14+4*j);
		if(strcmp(p[j],password)==0)//若密码正确 
        {
			printf("密码正确，游戏登录成功!");
    	    break;
		}
		else
		{	
			k++;
			if(k==1||k==2)//输错密码一次或两次 
			{
				printf(" 密码错误，请重新输入:");
			}
			if(k==3)//三次密码都输错 
			{
 		        printf("  密码输入次数已用尽！");
				break;
			}
	    }
    }
    return k;
}
//===============================================================================
int JudgeBottom(int n,int r,int x,int y)//判断是否到底 
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
