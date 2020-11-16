/*
Business game by Devansh Sampat
Build 32
Last updated 07/09/2020
*/

#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<graphics.h>
struct coordinates
{
	int x,y;
};
struct coordinates show(int i)
{
	int x,y;
	struct coordinates point;
	setcolor(1+i%6);
	if(i<9)
	{
		y=50;
		x=60+60*(i%9);
	}
	else if(i<18)
	{
		x=600;
		y=50+40*(i%9);
	}
	else if(i<27)
	{
		y=410;
		x=600-60*(i%9);
	}
	else
	{
		x=60;
		y=410-40*(i%9);
	}
	point.x=x;
	point.y=y;
	return point;
}
void showStartupAnimation()
{
	int x,y,i;
	setcolor(RED);
	x=330;
	y=220;
	for(i=1;i<=140;i++)
	{
		circle(x,y,i);
		delay(2);
	}
	delay(100);
	for(i=1;i<=140;i++)
	{
		line(x-i,y-i,x+i,y-i);
		line(x-i,y-i,x-i,y+i);
		line(x+i,y+i,x-i,y+i);
		line(x+i,y+i,x+i,y-i);
		delay(2);
	}
}
void animateEnd(struct coordinates point,int colour)
{
	int x,y;
	x=point.x;
	y=point.y;
	while(x!=330||y!=220)
	{
		setcolor(BLACK);
		circle(x,y,10);
		if(x>330) x=x-15;
		else if(x<330) x=x+15;
		if(y>220) y=y-10;
		else if(y<220) y=y+10;
		setcolor(colour);
		circle(x,y,10);
		delay(8);
	}
}
void animateStart(struct coordinates point,int colour)
{
	int x,y,xc,yc;
	xc=point.x;
	yc=point.y;
	setcolor(colour);
	x=330;
	y=220;
	circle(x,y,10);
	while(x!=xc||y!=yc)
	{
		setcolor(BLACK);
		circle(x,y,10);
		if(x<xc) x=x+15;
		else if(x>xc) x=x-15;
		if(y>yc) y=y-10;
		else if(y<yc) y=y+10;
		setcolor(colour);
		circle(x,y,10);
		delay(8);
	}
}
void main()
{
	struct coordinates point;
	FILE *fp,*cp;  //fp = file pointer , cp = copy pointer
	struct tm *local;      //to get time data
	time_t t1,t2,t3;
	char player[6][20],c,save[10],own_name[6][9];
	/*
		player to get player name
		c to get a character to decide which option to be activated
		save to open a save game file
		own_name to cut short player names to 8 characters to display
		on board
	*/

	int a[36][9],own[36],pt[36],prop[36],rent,mort[36][2],copy,px[6],py[6],xc,yc,x,y;
	/*
		array of a[][] for storing card data
		a[][0] = color grp
		a[][1] = Buying price
		a[][2] = rent site only
		a[][3] = double rent
		rent with :-
		a[][4] = 1 property
		a[][5] = 2 properties
		a[][6] = 3 properties
		a[][7] = 4 properties
		own array for storing card owner data
		own is initialized to zero by default
		pt is used to point towards the appropriate index for rent
		pt is initialized to 2 by default
		prop stores number of properties on that card
		rent is a multi use variable, and so as copy
		mort array is used to store mortgage data of cards
		px and py are x and y co-ordinates of players on graph(board)
		xc and yc are used to change player co-ordinates on board
		x and y are used to display the board and do various animations
	*/
	int count,i,j,k,round[6],n,chance[6],sum[6],blue[6],red[6],green[6],violet[6],
	white[6],dice,pausetime,offtime;
	/*
		count is used to keep a track of current player
		i,j,k are loop variables
		i is also used to initialize graph
		round stores number of rounds completed by player
		n is the number of players playing the game(2 to 6)
		chance decides whether the player can play a turn or not
		blue, red ,green, violet, white store respective card data
		of a player
		dice stores dice number of current player
		pausetime calculates time for which game is paused
		offtime is the set time for timer(if player sets)
	*/
	long int amt[6],p,loan[6];
	/*
		amt stores current amount of player
		p is a multi used long int to deal with amounts
		loan stores loan amount of a player
	*/
	float d,e;  //to deal with time and keypress simultaneously
	char place[37][18] = { "Start","Mumbai","Water Works","Railways",
	"Ahmedabad","Income Tax","Indore","Chance","Jaipur","jail","Delhi",
	"Chandigarh","Electric Company","BEST","Shimla","Amritsar",
	"Community Chest","Srinagar","Club","Agra","Chance","Kanpur","Patna",
	"Darjeeling","Air India","Kolkata","Hyderabad","Rest House","Madras",
	"Community Chest","Bangalore","Wealth Tax","Ooty","Cochin",
	"Motor Boat","Goa","Start" } ;
	//names of placs are stored in place[][]
	save[0]='s';
	save[1]='a';
	save[2]='v';
	save[3]='e';
	save[5]='.';
	save[6]='t';
	save[7]='x';
	save[8]='t';
	save[9]=0;
	clrscr();
	pausetime=0;
	//initializing all elements to 0
	for(i=0;i<=35;i++)
	{
		for(j=0;j<=8;j++)
		{
			a[i][j]=0;
		}
	}
	//setting gameplay environment
	for(i=0;i<=35;i++)
	{
		own[i]=0;
		pt[i]=2;
		prop[i]=0;
		mort[i][0]=0;
		switch(i)             //sorting card groups
		{
			case 1:
			case 4:
			case 23:
			case 25:
			case 26:
			a[i][0]=1;
			break;
			case 2:
			case 3:
			case 12:
			case 13:
			case 24:
			case 34:
			a[i][0]=5;
			break;
			case 6:
			case 8:
			case 19:
			case 21:
			case 22:
			a[i][0]=2;
			break;
			case 10:
			case 11:
			case 32:
			case 33:
			case 35:
			a[i][0]=3;
			break;
			case 14:
			case 15:
			case 17:
			case 28:
			case 30:
			a[i][0]=4;
			break;
		}
	}
	//storing card amounts
	a[1][1]=8500;
	a[2][1]=3200;
	a[3][1]=9500;
	a[4][1]=4000;
	a[6][1]=1500;
	a[8][1]=3000;
	a[10][1]=6000;
	a[11][1]=2500;
	a[12][1]=2500;
	a[13][1]=3500;
	a[14][1]=2200;
	a[15][1]=3300;
	a[17][1]=5000;
	a[19][1]=2500;
	a[21][1]=4000;
	a[22][1]=2000;
	a[23][1]=2500;
	a[24][1]=10500;
	a[25][1]=6500;
	a[26][1]=3500;
	a[28][1]=7000;
	a[30][1]=4000;
	a[32][1]=2500;
	a[33][1]=3000;
	a[34][1]=5500;
	a[35][1]=4000;
	//storing other card data
	a[1][2]=1200;
	a[1][3]=2400;
	a[1][4]=4000;
	a[1][5]=5500;
	a[1][6]=7500;
	a[1][7]=9000;
	a[1][8]=7500;
	a[2][2]=500;
	a[2][3]=1000;
	a[3][2]=1000;
	a[3][3]=1350;
	a[4][2]=400;
	a[4][3]=800;
	a[4][4]=1500;
	a[4][5]=3000;
	a[4][6]=4200;
	a[4][7]=5000;
	a[4][8]=4500;
	a[6][2]=200;
	a[6][3]=400;
	a[6][4]=600;
	a[6][5]=1500;
	a[6][6]=2500;
	a[6][7]=3600;
	a[6][8]=2000;
	a[8][2]=250;
	a[8][3]=500;
	a[8][4]=1500;
	a[8][5]=2700;
	a[8][6]=4000;
	a[8][7]=5500;
	a[8][8]=4000;
	a[10][2]=750;
	a[10][3]=1500;
	a[10][4]=3000;
	a[10][5]=4300;
	a[10][6]=5500;
	a[10][7]=7500;
	a[10][8]=5000;
	a[11][2]=200;
	a[11][3]=400;
	a[11][4]=900;
	a[11][5]=1600;
	a[11][6]=2500;
	a[11][7]=3500;
	a[11][8]=3000;
	a[13][2]=600;
	a[13][3]=1100;
	a[14][2]=200;
	a[14][3]=400;
	a[14][4]=1000;
	a[14][5]=2750;
	a[14][6]=4500;
	a[14][7]=6000;
	a[14][8]=3500;
	a[15][2]=300;
	a[15][3]=600;
	a[15][4]=1400;
	a[15][5]=2800;
	a[15][6]=4000;
	a[15][7]=5000;
	a[15][8]=4500;
	a[17][2]=550;
	a[17][3]=1100;
	a[17][4]=3500;
	a[17][5]=5000;
	a[17][6]=7000;
	a[17][7]=8000;
	a[17][8]=6000;
	a[19][2]=200;
	a[19][3]=400;
	a[19][4]=900;
	a[19][5]=1600;
	a[19][6]=2500;
	a[19][7]=3500;
	a[19][8]=3000;
	a[21][2]=400;
	a[21][3]=800;
	a[21][4]=1500;
	a[21][5]=3000;
	a[21][6]=4500;
	a[21][7]=5500;
	a[21][8]=4500;
	a[22][2]=150;
	a[22][3]=300;
	a[22][4]=800;
	a[22][5]=2000;
	a[22][6]=3000;
	a[22][7]=4500;
	a[22][8]=2500;
	a[23][2]=200;
	a[23][3]=400;
	a[23][4]=1200;
	a[23][5]=2600;
	a[23][6]=3500;
	a[23][7]=5000;
	a[23][8]=3000;
	a[24][2]=1200;
	a[24][3]=1500;
	a[25][2]=800;
	a[25][3]=1600;
	a[25][4]=3200;
	a[25][5]=4500;
	a[25][6]=6500;
	a[25][7]=8000;
	a[25][8]=6000;
	a[26][2]=300;
	a[26][3]=600;
	a[26][4]=1200;
	a[26][5]=3000;
	a[26][6]=4500;
	a[26][7]=6000;
	a[26][8]=5000;
	a[28][2]=900;
	a[28][3]=1800;
	a[28][4]=3500;
	a[28][5]=5000;
	a[28][6]=7000;
	a[28][7]=8500;
	a[28][8]=6500;
	a[30][2]=400;
	a[30][3]=800;
	a[30][4]=1500;
	a[30][5]=3000;
	a[30][6]=4500;
	a[30][7]=5500;
	a[30][8]=4500;
	a[32][2]=200;
	a[32][3]=400;
	a[32][4]=1000;
	a[32][5]=2250;
	a[32][6]=3500;
	a[32][7]=4500;
	a[32][8]=3000;
	a[33][2]=300;
	a[33][3]=600;
	a[33][4]=1200;
	a[33][5]=2000;
	a[33][6]=4250;
	a[33][7]=5500;
	a[33][8]=4000;
	a[35][2]=400;
	a[35][3]=800;
	a[35][4]=2200;
	a[35][5]=3500;
	a[35][6]=5000;
	a[35][7]=6500;
	a[35][8]=4500;
	//Mortgage value is half the card buying value
	for(i=0;i<=35;i++)
	{
		mort[i][1]=a[i][1]/2;
	}
	c='n';
	i=DETECT;
	initgraph(&i,&j,"c:\\tc\\bgi");
	cleardevice();
	fp=fopen("gamedata.txt","r");
	//first game open check
	if(fp==NULL)
	{
		fclose(fp);
		fp=fopen("gamekey.txt","r");
		//Checks if game is officially installed or not
		if(fp==NULL)
		{
			setcolor(RED);
			settextstyle(3,HORIZ_DIR,3);
			outtextxy(5,5,"Files are missing, contact the developer");
			delay(1000);
			setcolor(WHITE);
			outtextxy(5,50,"Aborting");
			delay(1000);
			goto bye_bye;
		}
		settextstyle(3,HORIZ_DIR,6);
		setcolor(YELLOW);
		outtextxy(5,5,"Activate the game");
		fscanf(fp,"%d",&n);
		settextstyle(3,HORIZ_DIR,3);
		setcolor(WHITE);
		outtextxy(5,100,"Enter the product key given to you");
		outtextxy(240,190,"Enter Password");
		//3 attempts given to enter correct password
		for(i=1;i<=3;i++)
		{
			settextstyle(3,HORIZ_DIR,3);
			x=260;
			y=230;
			while(x<=350)
			{
				setcolor(BLACK);
				outtextxy(x,y,"*");
				//overwrites previously entered password
				setcolor(WHITE);
				outtextxy(x,y,"_");
				//sets blank spaces for new password
				x=x+30;
			}
			x=260;
			j=0;
			p=0;
			while(j<4)
			{
				c=getch();
				//accept if only numeric value
				if(c>='0'&&c<='9')
				{
					c=c-'0';
					copy=c;
					p=p*10+copy;
					setcolor(BLACK);
					outtextxy(x,y,"_");
					setcolor(WHITE);
					sprintf(player[1],"%d",copy);
					//shows for just 0.25 sec
					outtextxy(x,y,player[1]);
					delay(250);
					setcolor(BLACK);
					outtextxy(x,y,player[1]);
					setcolor(WHITE);
					outtextxy(x,y,"*");
					//privacy protection
					x=x+30;
					j=j+1;
					//counts number of digits
				}
				//backspace logic
				else if(c=='\b')
				{
					if(j>0)
					{
						j=j-1;
						//decreases number of digits
						p=p/10;
						//changes effective value
						x=x-30;
						setcolor(BLACK);
						outtextxy(x,y,"*");
						setcolor(WHITE);
						outtextxy(x,y,"_");
					}
				}
			}
			//correct password
			if(p==n)
			{
				setcolor(GREEN);
				outtextxy(270,280,"Starting...");
				delay(1000);
				cleardevice();
				x=330;
				y=220;
				//loading screen
				setcolor(RED);
				for(i=1;i<=20;i++)
				{
					circle(x,y,i);
					delay(10);
				}
				delay(50);
				setcolor(YELLOW);
				for(i=20;i>=1;i--)
				{
					circle(x,y,i);
					delay(10);
				}
				delay(50);
				setcolor(GREEN);
				for(i=1;i<=20;i++)
				{
					circle(x,y,i);
					delay(10);
				}
				delay(100);
				clrscr();
				cleardevice();
				fclose(fp);
				fp=fopen("gamedata.txt","w");
				fprintf(fp,"0");
				//first time check over
				fclose(fp);
				cleardevice();
				break;
			}
			/*
				give another chance if number of tries
				is less than 3
			*/
			else if(i<3)
			{
				outtextxy(260,280,"Try again");
				delay(1000);
				setcolor(BLACK);
				outtextxy(260,280,"Try again");
			}
			/*
				Force close the app if 3 incorrect attempts
			*/
			else
			{
				outtextxy(170,280,"Too many wrong attempts, aborting");
				delay(2000);
			}
		}
		//check whether the password is correct or not
		if(p!=n) goto bye_bye;
	}
	xc=x;
	yc=y;
	dice=1;
	//to recognize keypress
	setcolor(CYAN);
	x=330;
	y=235;
	for(i=1;i<=20;i++)
	{
		delay(30);
		circle(x,y,i);
	}
	line(x-30,y,x+30,y);
	line(x,y-40,x,y+40);
	setcolor(YELLOW);
	settextstyle(3,HORIZ_DIR,5);
	delay(150);
	outtextxy(325,205,"B");
	delay(1000);
	cleardevice();
	/*
		if a key is pressed between animation,
		get a character of that key and skip the animation
	*/
	while(x>170)
	{
		setcolor(BLACK);
		for(i=1;i<=20;i++) circle(x,y,i);
		line(x-30,y,x+30,y);
		line(x,y-40,x,y+40);
		outtextxy(x-5,y-30,"B");
		delay(1);
		x=x-20;
		setcolor(CYAN);
		for(i=1;i<=20;i++) circle(x,y,i);
		line(x-30,y,x+30,y);
		line(x,y-40,x,y+40);
		setcolor(YELLOW);
		outtextxy(x-5,y-30,"B");
		delay(30);
		if(kbhit()!=0) goto game_opening;
	}
	setcolor(WHITE);
	settextstyle(3,HORIZ_DIR,6);
	outtextxy(200,200,"Welcome");
	delay(200);
	y=200;
	for(i=1;i<=10;i++)
	{
		setcolor(i);
		outtextxy(200,y,"Welcome");
		delay(100);
		if(kbhit()!=0) goto game_opening;
	}
	delay(300);
	cleardevice();
	while(y>100)
	{
		setcolor(BLACK);
		outtextxy(200,y,"Welcome");
		y=y-20;
		setcolor(BROWN);
		outtextxy(200,y,"Welcome");
		delay(20);
		if(kbhit()!=0) goto game_opening;
	}
	delay(500);
	setcolor(WHITE);
	if(kbhit()!=0) goto game_opening;
	outtextxy(100,200,"DEVANSH SAMPAT");
	delay(1000);
	if(kbhit()!=0) goto game_opening;
	setcolor(GREEN);
	outtextxy(200,300,"presents");
	delay(1000);
	if(kbhit()!=0) goto game_opening;
	cleardevice();
	for(i=0;i<=35;i++)
	{
		point=show(i);
		animateStart(point,1+i%6);
		if(kbhit()!=0) goto game_opening;
		delay(50);
	}
	delay(1000);
	for(i=0;i<=35;i++)
	{
		point=show(i);
		animateEnd(point,1+i%6);
		if(kbhit()!=0) goto game_opening;
		delay(30);
	}
	delay(500);
	showStartupAnimation();
	if(kbhit()!=0) goto game_opening;
	setcolor(WHITE);
	settextstyle(3,HORIZ_DIR,6);
	outtextxy(200,200,"BUSINESS");
	delay(1000);
	if(kbhit()!=0) goto game_opening;
	settextstyle(3,HORIZ_DIR,3);
	outtextxy(320,415,"Loading...");
	y=400;
	x=100;
	while(x<130)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(15);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<160)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(12);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<180)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(10);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<200)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(7);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<230)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(5);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<420)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(4);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<450)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(5);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<480)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(7);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<520)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(10);
		if(kbhit()!=0) goto game_opening;
	}
	while(x<550)
	{
		bar(x,y,x+1,y+1);
		x=x+1;
		delay(15);
		if(kbhit()!=0) goto game_opening;
	}
	cleardevice();
	setcolor(WHITE);
	settextstyle(3,HORIZ_DIR,6);
	outtextxy(200,200,"BUSINESS");
	i=6;
	while(i>3)
	{
		delay(50);
		i--;
		cleardevice();
		settextstyle(3,HORIZ_DIR,i);
		outtextxy(200,200,"BUSINESS");
		if(kbhit()!=0) goto game_opening;
	}
	delay(100);
	x=200;
	y=200;
	while(x>5)
	{
		setcolor(BLACK);
		outtextxy(x,y,"BUSINESS");
		y=y-15;
		x=x-15;
		setcolor(WHITE);
		outtextxy(x,y,"BUSINESS");
		delay(8);
		if(kbhit()!=0) goto game_opening;
	}
	delay(300);
	dice=0;
	game_opening:
	//keypress condition check
	if(dice==1)
	{
		getch();
		cleardevice();
	}
	settextstyle(3,HORIZ_DIR,3);
	x=5;
	y=5;
	setcolor(MAGENTA);
	outtextxy(x,y,"BUSINESS");
	settextstyle(3,HORIZ_DIR,2);
	setcolor(YELLOW);
	delay(500);
	outtextxy(x+108,y+3,"by Devansh Sampat");
	delay(300);
	n=0;
	if(fp!=NULL) fscanf(fp,"%d",&n);
	if(n>=0)
	{
		copy=0;
		//to check if saved games present or not
		for(i=1;i<=8;i++)
		{
			save[4]=i+'0';
			// example if i==2, it will open save2.txt
			fp=fopen(save,"r");
			//file open check
			if(fp!=NULL)
			{
				copy=1;
				/*
					this will be used for showing
					load game option
				*/
				break;
			}
		}
		if(copy==1)
		{
			main_menu:
			printf("\n\n\n  n : new game\n\n  y : load game\n\n  x : exit");
			c=getch();
			//game exit condition
			if(c=='x')
			{
				fp=fopen("business.c","w");
				fprintf(fp,"...//Access Denied//...");
				fclose(fp);
				goto bye_bye;
			}
			//load game condition
			if(c=='y')
			{
				clrscr();
				cleardevice();
				settextstyle(3,HORIZ_DIR,3);
				x=5;
				y=5;
				setcolor(MAGENTA);
				outtextxy(x,y,"BUSINESS");
				settextstyle(3,HORIZ_DIR,2);
				setcolor(YELLOW);
				outtextxy(x+108,y+3,"by Devansh Sampat");
				printf("\n\n\n");
				for(i=1;i<=8;i++)
				{
					save[4]=i+'0';
					//open respective file
					fp=fopen(save,"r");
					if(fp!=NULL)
					{
						printf("%d : ",i);
						//saved game number
						fscanf(fp,"%d",&n);
						//number of players taken
						c=getc(fp);
						c=0;
						//taking player names
						for(i=0;i<=n;i++)
						{
							while(c!='\n')
							{
								c=getc(fp);
								if(c=='\n')
								{
									if(i<n-1) printf(", ");
									/*
										takes date and time
										of saving the game
										if i==n
									*/
								}
								else printf("%c",c);
							}
							c=0;
						}
						printf("\n");
					}
					fclose(fp);
					/*
						value of i modified
						so returning back to original value
					*/
					i=save[4]-'0';
				}
				printf("Enter game number : ");
				settextstyle(3,HORIZ_DIR,3);
				setcolor(LIGHTBLUE);
				outtextxy(10,300,"Press b to go back");
				while((c>'8'||c<'1')&&c!='b')
				{
					c=getch();
					//go back condition
					if(c=='b')
					{
						clrscr();
						cleardevice();
						settextstyle(3,HORIZ_DIR,3);
						x=5;
						y=5;
						setcolor(MAGENTA);
						outtextxy(x,y,"BUSINESS");
						settextstyle(3,HORIZ_DIR,2);
						setcolor(YELLOW);
						outtextxy(x+108,y+3,"by Devansh Sampat");
						goto main_menu;
					}
					//1 to 8 number conditioon
					else if(c<='8'&&c>='1')
					{
						save[4]=c;
						fp=fopen(save,"r");
						if(fp==NULL) c=0;
						/*
							if no save game present for given number
							take input again
						*/
						else
						{
							fscanf(fp,"%d",&n);
							//take no. of players
							c=getc(fp);
							c=0;
							for(i=0;i<=n-1;i++)
							{
								j=0;
								//take player names
								while(c!='\n')
								{
									c=getc(fp);
									if(c=='\n') player[i][j]=0;
									else player[i][j]=c;
									j++;
								}
								c=0;
							}
							while(c!='\n') c=getc(fp);
							//NOT to take date and time
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%ld",&amt[i]);
							}
							c='3';
							//just to avoid \n condition
							printf("%c",save[4]);
							delay(300);
							clrscr();
							cleardevice();
							setcolor(WHITE);
							settextstyle(3,HORIZ_DIR,4);
							outtextxy(100,350,"Setting up your game");
							delay(150);
							settextstyle(3,HORIZ_DIR,3);
							outtextxy(320,415,"Loading...");
							y=400;
							x=100;
							while(x<130)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(15);
							}
							setcolor(BROWN);
							settextstyle(3,HORIZ_DIR,4);
							outtextxy(100,350,"Setting up your game");
							while(x<160)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(12);
							}
							while(x<180)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(10);
							}
							while(x<200)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(7);
							}
							setcolor(WHITE);
							settextstyle(3,HORIZ_DIR,4);
							outtextxy(100,350,"Setting up your game");
							while(x<230)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(5);
							}
							while(x<420)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(4);
							}
							while(x<450)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(5);
							}
							setcolor(GREEN);
							settextstyle(3,HORIZ_DIR,4);
							outtextxy(100,350,"Setting up your game");
							while(x<480)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(7);
							}
							while(x<520)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(10);
							}
							while(x<550)
							{
								bar(x,y,x+1,y+1);
								x=x+1;
								delay(15);
							}
							setcolor(LIGHTBLUE);
							x=x-x%3;
							y=y-y%3;
							circle(x,y,20);
							delay(50);
							cleardevice();
							while(x>21||y>21)
							{
								setcolor(BLACK);
								circle(x,y,20);
								if(x>y) x=x-3;
								else if(x<y) y=y-3;
								else
								{
									x=x-3;
									y=y-3;
								}
								setcolor(WHITE);
								circle(x,y,20);
								delay(3);
							}
							//time started
							t1=time(NULL);
							local=localtime(&t1);
							//taking every data from file
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&sum[i]);
								//current position
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&chance[i]);
								//ability to play next turn
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&round[i]);
								//number of rounds
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&px[i]);
								//x coordinate
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&py[i]);
								//y coordinate
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%ld",&loan[i]);
								//loan data
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&blue[i]);
								//blue cards
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&green[i]);
								//green cards
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&red[i]);
								//red cards
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&violet[i]);
								//violet cards
							}
							for(i=0;i<=n-1;i++)
							{
								fscanf(fp,"%d",&white[i]);
								//white cards
							}
							for(i=0;i<=35;i++)
							{
								fscanf(fp,"%d",&own[i]);
								//owner of a card
							}
							for(i=0;i<=35;i++)
							{
								fscanf(fp,"%d",&prop[i]);
								//properties on a card
							}
							for(i=0;i<=35;i++)
							{
								fscanf(fp,"%d",&pt[i]);
								//point to rent to be taken
							}
							for(i=0;i<=35;i++)
							{
								fscanf(fp,"%d",&mort[i][0]);
								//mortgaged or not
							}
							fscanf(fp,"%d",&count);
							//current player playing
							goto game_start;
						}
					}
				}
			}
		}
	}
	fclose(fp);
	if(copy==0)
	{
		printf("\n\n\n  n : new game\n\n  x : exit");
		c=getch();
		fclose(fp);
		save[4]=0;
		//will be used at the end
		clrscr();
		cleardevice();
		if(c=='x')
		{
			//code privacy
			fp=fopen("businrss.c","w");
			fprintf(fp,"...//Access Denied//...");
			fclose(fp);
			goto bye_bye;
		}
		else goto take_player_data;
	}
	if(c!='y')
	{
		cleardevice();
		closegraph();
		save[4]=0;
		i=DETECT;
		initgraph(&i,&j,"c:\\tc\\bgi");
		take_player_data:
		settextstyle(3,HORIZ_DIR,3);
		setcolor(MAGENTA);
		x=5;
		y=5;
		outtextxy(x,y,"BUSINESS");
		settextstyle(3,HORIZ_DIR,2);
		setcolor(YELLOW);
		outtextxy(x+108,y+3,"by Devansh Sampat");
		printf("\n\n\n  Number of players = ");
		scanf("%d",&n);
		delay(100);
		if(n>6||n<2)
		{
			//2 to 6 player condition check
			while(n<2||n>6)
			{
				flushall();
				printf("  Only 2 to 6 players can play, so no. of players = ");
				scanf("%d",&n);
			}
		}
		/*
			amounts to be allocated on number of players basis
		*/
		switch(n)
		{
			case 2:
			p=70000;
			break;
			case 3:
			p=50000;
			break;
			case 4:
			p=45000;
			break;
			case 5:
			p=35000;
			break;
			case 6:
			p=30000;
			break;
		}
		flushall();
		for(i=0;i<=n-1;i++)
		{
			printf("  Player %d name : ",i+1);
			gets(player[i]);
			//setting up defaults for the respective player
			chance[i]=1;
			sum[i]=0;
			amt[i]=p;
			round[i]=0;
			blue[i]=0;
			green[i]=0;
			violet[i]=0;
			white[i]=0;
			red[i]=0;
			loan[i]=0;
			//coordinates of 'start'
			px[i]=60;
			py[i]=50;
		}
		count=1;
		printf("\n  Each player gets ");
		delay(100);
		printf("%ld\n",p);
		delay(300);
	}
	//time started
	t1=time(NULL);
	local=localtime(&t1);
	printf("\n  Its %02d:%02d:%02d\n",local->tm_hour,local->tm_min,local->tm_sec);
	delay(100);
	printf("\n  Setup Complete\n\n  Press any key to continue");
	d=e=clock()/CLOCKS_PER_SEC;
	/*
		if no key pressed for 5 seconds,
		game will auto start
	*/
	while((d-e)<5)
	{
		d=clock()/CLOCKS_PER_SEC;
		if(kbhit()!=0)
		{
			getch();
			break;
		}
	}
	game_start:
	cleardevice();
	closegraph();
	dice=0;
	//dice garbage value cleared
	//code privacy
	fp=fopen("business.c","w");
	fprintf(fp,"...//Access Denied//...");
	fclose(fp);
	for(i=0;i<=n-1;i++)
	{
		//shortening the name for board display
		for(j=0;j<=7;j++)
		{
			own_name[i][j]=player[i][j];
		}
		own_name[i][8]=0;
	}
	clrscr();
	offtime=30000;
	/*
		No one plays for such a long time
		just to clear garbage value
	*/
	flushall();
	do
	{
		begin:
		/*
			Actual game begins NOW
		*/
		t2=time(NULL);
		//timer condition check
		clrscr();
		if((int)(difftime(t2,t1))/60>=offtime)
		{
			printf("Time over");
			delay(2000);
			if(save[4]==0)
			{
				delay(500);
				/*
					For unsaved games,
					to avoid data loss,
					asks whether to save game or not
				*/
				printf("\n\nSave game (y/n)?");
				c=getch();
				if(c=='y') c='k';
				/*
					if c is given 'k' it will directly
					move out of loop and save game
					condition will execute
				*/
				else c='s';
			}
			/*
				if already loaded a saved game
				it will save again to respective game number
			*/
			else c='k';
			break;
		}
		rent=offtime-(int)(difftime(t2,t1))/60;
		/*
			checks how much time is left
		*/
		/*
			live notifications for last 5 minutes
			of game timer
		*/
		if(rent<=5)
		{
			printf("%d minute",rent);
			if(rent>1) printf("s");
			printf(" remaining");
			delay(1000);
		}
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nn\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		clrscr();
		/*
			Jail, club or rest house chance skip are
			shown below
		*/
		if(chance[count-1]==0)
		{
			chance[count-1]=1;
			printf("%s : skip : ",player[count-1]);
			switch(sum[count-1])
			{
				case 9:
				printf("Jail\n");
				break;
				case 18:
				printf("Club\n");
				break;
				case 27:
				printf("Rest House\n");
				break;
			}
		}
		else
		{
			//show current player data
			//fast, accurate and handy
			printf("%s  ",player[count-1]);
			p=count-1;
			if(blue[p]>0||green[p]>0||red[p]>0||violet[p]>0||white[p]>0)
			{
				printf("( ");
				if(blue[p]>0) printf("Blue : %d ",blue[p]);
				if(green[p]>0) printf("Green : %d ",green[p]);
				if(red[p]>0) printf("Red : %d ",red[p]);
				if(violet[p]>0) printf("Violet : %d ",violet[p]);
				if(white[p]>0) printf("White : %d ",white[p]);
				printf(")\n");
			}
			else printf("\n");
			//shows locations off all players
			printf("Locations:-\n");
			for(i=0;i<=n-1;i++)
			{
				printf("%s : ",player[i]);
				p=sum[i];
				printf("%s",place[p]);
				if(i==n-1) printf("\n");
				else if(i!=2&&i!=5) printf(",  ");
				/*
					new line after every 3 players
				*/
				else printf("\n");
			}
			printf("Press any key to roll dice\n");
			d=e=clock()/CLOCKS_PER_SEC;
			/*
				if no key pressed in 12 seconds
				auto play the chance
			*/
			while((d-e)<12)
			{
				d=clock()/CLOCKS_PER_SEC;
				if(kbhit()!=0)
				{
					getch();
					break;
				}
			}
			clrscr();
			t3=time(NULL);
			local=localtime(&t3);
			//dice number randomization process
			rent=local->tm_sec;
			dice=dice+rent*(rent+1)/2;
			dice=2+dice%11;
			sum[count-1]=sum[count-1]+dice;
			rent=0;
			//Give 1500 for round completion
			if(sum[count-1]>36)
			{
				sum[count-1]=sum[count-1]%36;
				round[count-1]=round[count-1]+1;
				amt[count-1]=amt[count-1]+1500;
				rent=1;
			}
			i=DETECT;
			initgraph(&i,&j,"c:\\tc\\bgi");
			delay(400);
			x=21;
			y=21;
			//Dice Rolling Animation
			while(y<225)
			{
				setcolor(BLACK);
				circle(x,y,20);
				circle(x+100,y,20);
				y=y+3;
				x=x+3;
				setcolor(WHITE);
				circle(x,y,20);
				circle(x+100,y,20);
				delay(1);
			}
			setcolor(WHITE);
			//complete the circle
			for(i=19;i>=1;i--)
			{
				circle(x,y,i);
				circle(x+100,y,i);
				delay(10);
			}
			setcolor(YELLOW);
			settextstyle(3,HORIZ_DIR,3);
			outtextxy(5,1,player[count-1]);
			//player name display
			delay(100);
			bar(200,200,250,250);
			bar(300,200,350,250);
			setcolor(RED);
			setcolor(YELLOW);
			outtextxy(5,40,place[(sum[count-1]+36-dice)%36]);
			circle(215,215,8);
			circle(235,235,8);
			circle(325,225,8);
			setcolor(WHITE);
			delay(200);
			bar(200,200,250,250);
			bar(300,200,350,250);
			setcolor(RED);
			circle(215,215,8);
			circle(215,235,8);
			circle(235,215,8);
			circle(235,235,8);
			circle(310,210,8);
			circle(325,225,8);
			circle(340,240,8);
			delay(200);
			bar(200,200,250,250);
			bar(300,200,350,250);
			setcolor(RED);
			circle(210,208,8);
			circle(210,225,8);
			circle(210,242,8);
			circle(240,208,8);
			circle(240,225,8);
			circle(240,242,8);
			circle(310,210,8);
			circle(340,240,8);
			circle(310,240,8);
			circle(340,210,8);
			circle(325,225,8);
			delay(200);
			bar(200,200,250,250);
			bar(300,200,350,250);
			setcolor(RED);
			i=local->tm_sec;
			/*
				following are 36 cases for two dice
				combination
				according to dice value and time, 1 of them
				is selected
			*/
			switch(dice)
			{
				case 2:
				circle(225,225,8);
				circle(325,225,8);
				break;
				case 3:
				switch(i%2)
				{
					case 0:
					circle(215,215,8);
					circle(235,235,8);
					circle(325,225,8);
					break;
					case 1:
					circle(225,225,8);
					circle(315,215,8);
					circle(335,235,8);
					break;
				}
				break;
				case 4:
				switch(i%3)
				{
					case 0:
					circle(215,215,8);
					circle(235,235,8);
					circle(315,215,8);
					circle(335,235,8);
					break;
					case 1:
					circle(225,225,8);
					circle(310,210,8);
					circle(325,225,8);
					circle(340,240,8);
					break;
					case 2:
					circle(210,210,8);
					circle(225,225,8);
					circle(240,240,8);
					circle(325,225,8);
					break;
				}
				break;
				case 5:
				switch(i%4)
				{
					case 0:
					circle(210,210,8);
					circle(225,225,8);
					circle(240,240,8);
					circle(315,215,8);
					circle(335,235,8);
					break;
					case 1:
					circle(215,215,8);
					circle(235,235,8);
					circle(310,210,8);
					circle(325,225,8);
					circle(340,240,8);
					break;
					case 2:
					circle(225,225,8);
					circle(315,215,8);
					circle(315,235,8);
					circle(335,215,8);
					circle(335,235,8);
					break;
					case 3:
					circle(215,215,8);
					circle(215,235,8);
					circle(235,215,8);
					circle(235,235,8);
					circle(325,225,8);
					break;
				}
				break;
				case 6:
				switch(i%5)
				{
					case 0:
					circle(210,210,8);
					circle(225,225,8);
					circle(240,240,8);
					circle(310,210,8);
					circle(325,225,8);
					circle(340,240,8);
					break;
					case 1:
					circle(225,225,8);
					circle(310,210,8);
					circle(340,240,8);
					circle(310,240,8);
					circle(340,210,8);
					circle(325,225,8);
					break;
					case 2:
					circle(210,210,8);
					circle(240,240,8);
					circle(210,240,8);
					circle(240,210,8);
					circle(225,225,8);
					circle(325,225,8);
					break;
					case 3:
					circle(215,215,8);
					circle(235,235,8);
					circle(315,215,8);
					circle(315,235,8);
					circle(335,215,8);
					circle(335,235,8);
					break;
					case 4:
					circle(215,215,8);
					circle(215,235,8);
					circle(235,215,8);
					circle(235,235,8);
					circle(315,215,8);
					circle(335,235,8);
					break;
				}
				break;
				case 7:
				switch(i%6)
				{
					case 0:
					circle(215,215,8);
					circle(215,235,8);
					circle(235,215,8);
					circle(235,235,8);
					circle(310,210,8);
					circle(325,225,8);
					circle(340,240,8);
					break;
					case 1:
					circle(210,210,8);
					circle(225,225,8);
					circle(240,240,8);
					circle(315,215,8);
					circle(315,235,8);
					circle(335,215,8);
					circle(335,235,8);
					break;
					case 2:
					circle(215,215,8);
					circle(235,235,8);
					circle(310,210,8);
					circle(340,240,8);
					circle(310,240,8);
					circle(340,210,8);
					circle(325,225,8);
					break;
					case 3:
					circle(210,210,8);
					circle(240,240,8);
					circle(210,240,8);
					circle(240,210,8);
					circle(225,225,8);
					circle(315,215,8);
					circle(335,235,8);
					break;
					case 4:
					circle(225,225,8);
					circle(310,208,8);
					circle(310,225,8);
					circle(310,242,8);
					circle(340,208,8);
					circle(340,225,8);
					circle(340,242,8);
					break;
					case 5:
					circle(210,208,8);
					circle(210,225,8);
					circle(210,242,8);
					circle(240,208,8);
					circle(240,225,8);
					circle(240,242,8);
					circle(325,225,8);
					break;
				}
				break;
				case 8:
				switch(i%5)
				{
					case 0:
					circle(215,215,8);
					circle(215,235,8);
					circle(235,215,8);
					circle(235,235,8);
					circle(315,215,8);
					circle(315,235,8);
					circle(335,215,8);
					circle(335,235,8);
					break;
					case 1:
					circle(215,215,8);
					circle(235,235,8);
					circle(310,208,8);
					circle(310,225,8);
					circle(310,242,8);
					circle(340,208,8);
					circle(340,225,8);
					circle(340,242,8);
					break;
					case 2:
					circle(210,208,8);
					circle(210,225,8);
					circle(210,242,8);
					circle(240,208,8);
					circle(240,225,8);
					circle(240,242,8);
					circle(315,215,8);
					circle(335,235,8);
					break;
					case 3:
					circle(210,210,8);
					circle(225,225,8);
					circle(240,240,8);
					circle(310,210,8);
					circle(340,240,8);
					circle(310,240,8);
					circle(340,210,8);
					circle(325,225,8);
					break;
					case 4:
					circle(210,210,8);
					circle(240,240,8);
					circle(210,240,8);
					circle(240,210,8);
					circle(225,225,8);
					circle(310,210,8);
					circle(325,225,8);
					circle(340,240,8);
					break;
				}
				break;
				case 9:
				switch(i%4)
				{
					case 0:
					circle(210,210,8);
					circle(240,240,8);
					circle(210,240,8);
					circle(240,210,8);
					circle(225,225,8);
					circle(315,215,8);
					circle(315,235,8);
					circle(335,215,8);
					circle(335,235,8);
					break;
					case 1:
					circle(215,215,8);
					circle(215,235,8);
					circle(235,215,8);
					circle(235,235,8);
					circle(310,210,8);
					circle(340,240,8);
					circle(310,240,8);
					circle(340,210,8);
					circle(325,225,8);
					break;
					case 2:
					circle(210,208,8);
					circle(210,225,8);
					circle(210,242,8);
					circle(240,208,8);
					circle(240,225,8);
					circle(240,242,8);
					circle(310,210,8);
					circle(325,225,8);
					circle(340,240,8);
					break;
					case 3:
					circle(210,210,8);
					circle(225,225,8);
					circle(240,240,8);
					circle(310,208,8);
					circle(310,225,8);
					circle(310,242,8);
					circle(340,208,8);
					circle(340,225,8);
					circle(340,242,8);
					break;
				}
				break;
				case 10:
				switch(i%3)
				{
					case 0:
					circle(210,210,8);
					circle(240,240,8);
					circle(210,240,8);
					circle(240,210,8);
					circle(225,225,8);
					circle(310,210,8);
					circle(340,240,8);
					circle(310,240,8);
					circle(340,210,8);
					circle(325,225,8);
					break;
					case 1:
					circle(210,208,8);
					circle(210,225,8);
					circle(210,242,8);
					circle(240,208,8);
					circle(240,225,8);
					circle(240,242,8);
					circle(315,215,8);
					circle(315,235,8);
					circle(335,215,8);
					circle(335,235,8);
					break;
					case 2:
					circle(215,215,8);
					circle(215,235,8);
					circle(235,215,8);
					circle(235,235,8);
					circle(310,208,8);
					circle(310,225,8);
					circle(310,242,8);
					circle(340,208,8);
					circle(340,225,8);
					circle(340,242,8);
					break;
				}
				break;
				case 11:
				switch(i%2)
				{
					case 0:
					circle(210,208,8);
					circle(210,225,8);
					circle(210,242,8);
					circle(240,208,8);
					circle(240,225,8);
					circle(240,242,8);
					circle(310,210,8);
					circle(340,240,8);
					circle(310,240,8);
					circle(340,210,8);
					circle(325,225,8);
					break;
					case 1:
					circle(210,210,8);
					circle(240,240,8);
					circle(210,240,8);
					circle(240,210,8);
					circle(225,225,8);
					circle(310,208,8);
					circle(310,225,8);
					circle(310,242,8);
					circle(340,208,8);
					circle(340,225,8);
					circle(340,242,8);
					break;
				}
				break;
				case 12:
				circle(210,208,8);
				circle(210,225,8);
				circle(210,242,8);
				circle(240,208,8);
				circle(240,225,8);
				circle(240,242,8);
				circle(310,208,8);
				circle(310,225,8);
				circle(310,242,8);
				circle(340,208,8);
				circle(340,225,8);
				circle(340,242,8);
				break;
			}
			/*
				if key is pressed, update the player
				coordinates and show transaction page
			*/
			if(kbhit()!=0)
			{
				xc=px[count-1];
				yc=py[count-1];
				for(i=1;i<=dice;i++)
				{
					if(xc<600&&xc>=50&&yc==50) xc=xc+60;
					else if(xc<=600&&xc>60&&yc==410) xc=xc-60;
					else if(yc>=50&&yc<410&&xc==600) yc=yc+40;
					else if(xc==60&&yc>50&&yc<=410) yc=yc-40;
				}
				px[count-1]=xc;
				py[count-1]=yc;
				cleardevice();
				closegraph();
				clrscr();
				goto redirect;
			}
			d=e=clock()/CLOCKS_PER_SEC;
			while((d-e)<2)
			{
				d=clock()/CLOCKS_PER_SEC;
			}
			cleardevice();
			delay(75);
			setcolor(YELLOW);
			xc=px[count-1];
			yc=py[count-1];
			x=330;
			y=230;
			setcolor(count);
			circle(x,y,10);
			delay(50);
			while(x!=xc||y!=yc)
			{
				/*
					if key is pressed, update the player
					coordinates and show transaction page
				*/
				if(kbhit()!=0)
				{
					xc=px[count-1];
					yc=py[count-1];
					for(i=1;i<=dice;i++)
					{
						if(xc<600&&xc>=50&&yc==50) xc=xc+60;
						else if(xc<=600&&xc>60&&yc==410) xc=xc-60;
						else if(yc>=50&&yc<410&&xc==600) yc=yc+40;
						else if(xc==60&&yc>50&&yc<=410) yc=yc-40;
					}
					px[count-1]=xc;
					py[count-1]=yc;
					cleardevice();
					closegraph();
					clrscr();
					goto redirect;
				}
				/*
					Shows an animation from centre of the
					page to the respective player position
				*/
				setcolor(BLACK);
				circle(x,y,10);
				if(xc>x) x=x+1;
				else if(xc<x) x=x-1;
				if(yc>y) y=y+1;
				else if(yc<y) y=y-1;
				setcolor(count);
				circle(x,y,10);
				delay(1);
			}
			delay(75);
			/*
				Making up the board
			*/
			bar(30,30,630,32);
			bar(90,70,570,72);
			bar(90,70,92,390);
			bar(30,30,32,430);
			bar(30,430,632,432);
			bar(90,390,570,392);
			bar(630,430,632,30);
			bar(570,390,572,70);
			bar(90,30,92,70);
			x=90;
			y=30;
			while(x<570)
			{
				x=x+60;
				bar(x,y,x+2,70);
			}
			bar(570,70,630,72);
			x=570;
			y=70;
			while(y<390)
			{
				y=y+40;
				bar(x,y,630,y+2);
			}
			bar(570,390,572,430);
			y=390;
			x=570;
			while(x>90)
			{
				x=x-60;
				bar(x,y,x+2,430);
			}
			bar(30,390,90,392);
			y=390;
			x=30;
			while(y>70)
			{
				y=y-40;
				bar(x,y,90,y+2);
			}
			settextstyle(2,HORIZ_DIR,4);
			setcolor(YELLOW);
			outtextxy(45,50,"Start");
			setcolor(LIGHTBLUE);
			outtextxy(105,50,"Mumbai");
			outtextxy(275,50,"Ahmedabad");
			outtextxy(160,410,"Kolkata");
			outtextxy(273,410,"Darjeeling");
			outtextxy(95,410,"Hyderabad");
			setcolor(WHITE);
			outtextxy(585,50,"Jail");
			outtextxy(585,410,"Club");
			outtextxy(52,400,"Rest");
			outtextxy(48,415,"House");
			outtextxy(165,40,"Water");
			outtextxy(165,55,"Works");
			outtextxy(215,50,"Railways");
			outtextxy(340,40,"Income");
			outtextxy(350,55,"Tax");
			outtextxy(460,50,"Chance");
			outtextxy(460,410,"Chance");
			outtextxy(575,320,"Community");
			outtextxy(587,335,"Chest");
			outtextxy(35,320,"Community");
			outtextxy(47,335,"Chest");
			outtextxy(580,160,"Electric");
			outtextxy(580,175,"Company");
			outtextxy(215,410,"Air India");
			outtextxy(590,210,"BEST");
			outtextxy(48,120,"Motor");
			outtextxy(50,135,"Boat");
			outtextxy(45,240,"Wealth");
			outtextxy(53,255,"Tax");
			setcolor(GREEN);
			outtextxy(405,50,"Indore");
			outtextxy(525,50,"Jaipur");
			outtextxy(530,410,"Agra");
			outtextxy(405,410,"Kanpur");
			outtextxy(348,410,"Patna");
			setcolor(LIGHTRED);
			outtextxy(585,90,"Delhi");
			outtextxy(573,130,"Chandigarh");
			outtextxy(48,210,"Ooty");
			outtextxy(45,170,"Cochin");
			outtextxy(53,90,"Goa");
			setcolor(MAGENTA);
			outtextxy(45,370,"Madras");
			outtextxy(580,370,"Srinagar");
			outtextxy(580,290,"Amritsar");
			outtextxy(585,250,"Shimla");
			outtextxy(35,290,"Bangalore");
			x=30;
			y=15;
			/*
				showing owner names of places
				this is where own_name comes into picture
			*/
			for(k=1;k<=8;k++)
			{
				x=x+60;
				if(own[k]>0)
				{
					switch(own[k]%3)
					{
						case 1: setcolor(YELLOW);
							break;
						case 2: setcolor(CYAN);
							break;
						case 0: setcolor(WHITE);
							break;
					}
					outtextxy(x,y,own_name[(own[k])-1]);
					setcolor(YELLOW);
				}
			}
			x=510;
			y=45;
			for(k=10;k<=17;k++)
			{
				y=y+40;
				if(own[k]>0)
				{
					switch(own[k]%3)
					{
						case 1: setcolor(YELLOW);
							break;
						case 2: setcolor(CYAN);
							break;
						case 0: setcolor(WHITE);
							break;
					}
					outtextxy(x,y,own_name[(own[k])-1]);
					setcolor(YELLOW);
				}
			}
			x=570;
			y=435;
			for(k=19;k<=26;k++)
			{
				x=x-60;
				if(own[k]>0)
				{
					switch(own[k]%3)
					{
						case 1: setcolor(YELLOW);
							break;
						case 2: setcolor(CYAN);
							break;
						case 0: setcolor(WHITE);
							break;
					}
					outtextxy(x,y,own_name[(own[k])-1]);
					setcolor(YELLOW);
				}
			}
			x=95;
			y=410;
			for(k=28;k<=35;k++)
			{
				y=y-40;
				if(own[k]>0)
				{
					switch(own[k]%3)
					{
						case 1: setcolor(YELLOW);
							break;
						case 2: setcolor(CYAN);
							break;
						case 0: setcolor(WHITE);
							break;
					}
					outtextxy(x,y,own_name[(own[k])-1]);
					setcolor(YELLOW);
				}
			}
			d=e=clock()/CLOCKS_PER_SEC;
			while((d-e)<1)
			{
				d=clock()/CLOCKS_PER_SEC;
			}
			/*
				if key is pressed, update the player
				coordinates and show transaction page
			*/
			if(kbhit()!=0)
			{
				for(i=1;i<=dice;i++)
				{
					if(xc<600&&xc>=50&&yc==50) xc=xc+60;
					else if(xc<=600&&xc>60&&yc==410) xc=xc-60;
					else if(yc>=50&&yc<410&&xc==600) yc=yc+40;
					else if(xc==60&&yc>50&&yc<=410) yc=yc-40;
				}
				px[count-1]=xc;
				py[count-1]=yc;
				cleardevice();
				closegraph();
				clrscr();
				goto redirect;
			}
			for(i=1;i<=dice;i++)
			{
				setcolor(BLACK);
				circle(xc,yc,10);
				for(j=0;j<=n-1;j++)
				{
					if(j!=count-1)
					{
						setcolor(j+1);
						circle(px[j],py[j],10);
					}
				}
				/*
					coin moving ahead, one by one
				*/
				if(xc<600&&xc>=50&&yc==50) xc=xc+60;
				else if(xc<=600&&xc>60&&yc==410) xc=xc-60;
				else if(yc>=50&&yc<410&&xc==600) yc=yc+40;
				else if(xc==60&&yc>50&&yc<=410) yc=yc-40;
				setcolor(count);
				circle(xc,yc,10);
				delay(200);
				if(kbhit()!=0)
				{
					while(i<dice)
					{
						if(xc<600&&xc>=50&&yc==50) xc=xc+60;
						else if(xc<=600&&xc>60&&yc==410) xc=xc-60;
						else if(yc>=50&&yc<410&&xc==600) yc=yc+40;
						else if(xc==60&&yc>50&&yc<=410) yc=yc-40;
						i++;
					}
					px[count-1]=xc;
					py[count-1]=yc;
					cleardevice();
					closegraph();
					clrscr();
					goto redirect;
				}
			}
			px[count-1]=xc;
			py[count-1]=yc;
			e=clock()/CLOCKS_PER_SEC;
			settextstyle(3,HORIZ_DIR,3);
			delay(200);
			p=sum[count-1];
			setcolor(YELLOW);
			outtextxy(290,210,place[p]);
			while((d-e)<1.5)
			{
				if(kbhit()!=0) break;;
				d=clock()/CLOCKS_PER_SEC;
			}
			cleardevice();
			delay(10);
			x=xc;
			y=yc;
			while(x>10||y>10)
			{
				setcolor(BLACK);
				circle(x,y,10);
				if(x>y) x=x-2;
				else if(x<y) y=y-2;
				else
				{
					x=x-2;
					y=y-2;
				}
				setcolor(count);
				circle(x,y,10);
				delay(1);
			}
			cleardevice();
			closegraph();
			redirect:
			if(kbhit()!=0)
			{
				getch();
				c='\n';
			}
			printf("%s  ",player[count-1]);
			//shows current player name on screen
			p=count-1;
			//card present condition check
			if(blue[p]>0||green[p]>0||red[p]>0||violet[p]>0||white[p]>0)
			{
				printf("( ");
				/*
					shows total cards owned
					in groups of
					blue, green, red, violet and white

					shows info if atleast one card of
					a particular grp is present
				*/
				if(blue[p]>0) printf("Blue : %d ",blue[p]);
				if(green[p]>0) printf("Green : %d ",green[p]);
				if(red[p]>0) printf("Red : %d ",red[p]);
				if(violet[p]>0) printf("Violet : %d ",violet[p]);
				if(white[p]>0) printf("White : %d ",white[p]);
				printf(")\n");
			}
			else printf("\n");
			printf("Dice no. = %d\n",dice);
			if(rent==1) printf("Received 1500 for round completion\n");
			/*
				rent was turned upto 1 when round was completed
				in the board graphics
			*/
			printf("You have reached : ");
			p=sum[count-1];
			//place display
			printf("%s\n",place[p]);
			panoti:
			p=sum[count-1];
			//No transactions allowed in first round
			//First round is to know the board
			/*if(round[count-1]==0)
			{
				delay(150);
				printf("\nFirst round, no transactions\n");
				goto disp;
			}
			else
			{
				delay(300);
			} */
			delay(300);
			//club, jail or rest house condition check
			if(p==9||p==18||p==27)
			{
				if(p==9)
				{
					flushall();
					//if player can pay, ask to pay
					if(amt[count-1]>=500)
					{
						printf("Pay 500 (y/n)? ");
						c=getch();
						//check if player paid or not
						if(c=='y')
						{
							amt[count-1]=amt[count-1]-500;
							printf("\nPaid 500\n");
						}
						//skip the turn if player didn't pay
						else
						{
							chance[count-1]=0;
							printf("\nYou cannot play next turn\n");
						}
					}
					else
					{
							chance[count-1]=0;
							printf("You cannot play next turn\n");
					}
				}
				/*
					compulsary pay 1000 AND skip
					for club and rest house
				*/
				else
				{
					amt[count-1]=amt[count-1]-1000;
					chance[count-1]=0;
					printf("Paid 1000\nYou cannot play next turn\n");
				}

			}
			//income tax
			else if(p==5)
			{
				rent=0;
				//take 50 per card player owns
				for(i=0;i<=35;i++)
				{
					if(own[i]==count) rent=rent+50;
				}
				amt[count-1]=amt[count-1]-rent;
				printf("Paid %d (50 per card)\n",rent);

			}
			//wealth tax
			else if(p==31)
			{
				rent=0;
				//take 100 per card player owns
				for(i=0;i<=35;i++)
				{
					if(own[i]==count) rent=rent+100;
				}
				amt[count-1]=amt[count-1]-rent;
				printf("Paid %d (100 per card)\n",rent);

			}
			//chance
			else if(p==7||p==20)
			{
				p=amt[count-1];
				//to increase or decrease amount
				switch(dice)
				{
					case 3:
					//lottery of 2500
					p=p+2500;
					printf("Lottery prize\nWon 2500\n");
					break;
					case 5:
					printf("Won crossword competition\nGot 1000\n");
					p=p+1000;
					break;
					case 7:
					p=p+2000;
					printf("Won jackpot of 2000\n");
					break;
					case 9:
					/*
						go to Mumbai OR Darjeeling
						whichever nearby
					*/
					if(sum[count-1]==7)
					{
						sum[count-1]=1;
						px[count-1]=120;
						py[count-1]=50;
						/*
							update data to match
							with Mumbai
						*/
						printf("Go to Mumbai\n");
					}
					else
					{
						sum[count-1]=23;
						px[count-1]=300;
						py[count-1]=410;
						/*
							update data to match
							with Darjeeling
						*/
						printf("Go to Darjeeling\n");
					}
					break;
					case 11:
					p=p+3000;
					printf("Best employee of the year\nGot 3000\n");
					break;
					case 2:
					printf("Loss of 2000 in share market\n");
					p=p-2000;
					break;
					case 4:
					printf("Drink and drive\nPaid fine of 1500\n");
					p=p-1500;
					break;
					case 6:
					p=p-1500;
					printf("House Repairs\nPaid 1500\n");
					break;
					case 8:
					printf("Fire in godown\nLoss of 3000\n");
					p=p-3000;
					break;
					case 10:
					sum[count-1]=9;
					printf("Go to jail\n");
					/*
						coordinates of jail
						to be assigned to that player
					*/
					px[count-1]=600;
					py[count-1]=50;
					goto panoti;
					case 12:
					sum[count-1]=27;
					/*
						coordinates of Rest house
						to be assigned to that player
					*/
					printf("Go to rest house\n");
					px[count-1]=60;
					py[count-1]=410;
					goto panoti;
				}
				amt[count-1]=p;
			}
			//community chest
			else if(p==16||p==29)
			{
				p=amt[count-1];
				//for faster amount switching
				switch(dice)
				{
					case 2:
					for(i=0;i<=n-1;i++)
					{
						if(i!=count-1)
						{
							//collect 500 from each player
							p=p+500;
							amt[i]=amt[i]-500;
						}
					}
					printf("Happy Birthday!\nGot 500 from each player\n");
					break;
					case 4:
					printf("1st prize in College event\nWon 2500\n");
					p=p+2500;
					break;
					case 6:
					printf("Income tax refund\nWon 2000\n");
					p=p+2000;
					break;
					case 8:
					printf("Go to rest house\n");
					/*
						coordinates of rest house
						to be assigned to that player
					*/
					px[count-1]=60;
					py[count-1]=410;
					sum[count-1]=27;
					goto panoti;
					case 10:
					printf("Share market profit\nWon 1500\n");
					p=p+1500;
					break;
					case 12:
					p=p+3000;
					printf("Diwali Bonus\nEarned 3000\n");
					break;
					case 3:
					sum[count-1]=9;
					printf("Go to jail\n");
					/*
						coordinates of jail
						to be assigned to that player
					*/
					px[count-1]=600;
					py[count-1]=50;
					goto panoti;
					case 5:
					printf("College event\nPaid 1000\n");
					p=p-1000;
					break;
					case 7:
					p=p-2000;
					printf("Celebrate your marriage anniversary\nSpent 2000\n");
					break;
					case 9:
					rent=0;
					printf("Property repair (50 per property)\n");
					for(i=0;i<=35;i++)
					{
						//take 50 per property
						if(own[i]==count)
						{
							 p=p-50*prop[i];
							 rent=rent+50*prop[i];
						}
					}
					printf("Paid %d\n",rent);
					break;
					case 11:
					p=p-1500;
					printf("Insurance premium\nPaid 1500\n");
					break;
				}
				amt[count-1]=p;
			}
			else if(p==36)
			{
				//blank condition
				//to avoid buying 'start'
			}
			//want to buy a card but running low on amount?
			else if(own[p]==0&&amt[count-1]<a[p][1])
			{
				rent=6000;
				//Max mortgage value is 5250 and that of Air India
				//so playing safe, kept rent = 6000
				copy=0;
				for(i=0;i<=35;i++)
				{
					//player owns and not mortgaged
					if(own[i]==count&&mort[i][0]==0)
					{
						/*
							if player is eligible to buy
							after mortgaging
						*/
						if(amt[count-1]+mort[i][1]>=a[p][1])
						{
							/*
								choosing the card
								with least value
								but fulfills the amt requirement
							*/
							if(mort[i][1]<rent)
							{
								//taking card number and mortgage value
								copy=i;
								rent=mort[i][1];
							}
						}
					}
				}
				//if no card match is found
				if(copy==0) printf("\nNot enough amount to buy\n");
				else
				{
					flushall();
					printf("Amount = %ld\n",amt[count-1]);
					delay(100);
					printf("Mortgage ");
					delay(150);
					printf("%s ",place[copy]);
					delay(100);
					printf("for %d\n",mort[copy][1]);
					delay(100);
					printf("And buy for %d (y/n) ",a[p][1]);
					delay(100);
					printf("(Rent : ");
					a[0][0]=copy;   //will use a[0][0] later
					switch(a[p][0])
					{
						case 1:
						if(blue[count-1]>=2) printf("%d) ",a[p][3]);
						else printf("%d) ",a[p][2]);
						delay(200);
						printf("(Blue group) ? ");
						break;
						case 2:
						if(green[count-1]>=2) printf("%d) ",a[p][3]);
						else printf("%d) ",a[p][2]);
						delay(200);
						printf("(Green group) ? ");
						break;
						case 3:
						if(red[count-1]>=2) printf("%d) ",a[p][3]);
						else printf("%d) ",a[p][2]);
						delay(200);
						printf("(Red group) ? ");
						break;
						case 4:
						if(violet[count-1]>=2) printf("%d) ",a[p][3]);
						else printf("%d) ",a[p][2]);
						delay(200);
						printf("(Violet group) ? ");
						break;
						case 5:
						rent=2;
						/*
							pt[] is generally 2
							so rent is used to change pt
						*/
						copy=0; //use of a[0][0]
						switch(p)
						{
							case 2:
							if(own[24]==count) rent=3;
							/*
								Rent of water works increases
								if you own Air India and vice versa
							*/
							break;
							case 24:
							if(own[2]==count) rent=3;
							break;
							case 3:
							/*
								Rent of Railways increases
								if you own BEST and vice versa
							*/
							if(own[13]==count) rent=3;
							break;
							case 13:
							if(own[3]==count) rent=3;
							break;
						}
						if(rent==3) copy=1;
						if(p==2||p==3||p==13||p==24) printf("%d) ",a[p][rent]);
						/*
							Rent of Electric Company increases
							if you own Motor Boat and vice versa
						*/
						if(p==12)
						{
							if(own[34]==count)
							{
								copy=1;
								printf("100 times number on dice) ");
							}
							else printf("50 times number on dice) ");
						}
						if(p==34)
						{
							if(own[12]==count)
							{
								copy=1;
								printf("200 times number on dice) ");
							}
							else printf("100 times number on dice) ");
						}
						delay(200);
						printf("(White group) ? ");
						break;
					}
					c=getch();
					if(c=='y')
					{
						//confirmation
						printf("\nYou own ");
						p=sum[count-1];
						printf("%s   ",place[p]);
						for(i=1;i<=3;i++)
						{
							printf(".");
							delay(100);
						}
						delay(100);
						printf("   ");
						switch(a[p][0])
						//respective card data increment
						{
							case 1:
							blue[count-1]++;
							printf("(Blue cards : %d)\n",blue[count-1]);
							if(blue[count-1]==3)
							{
								delay(150);
								printf("Rent doubled on all your blue cards\nNow eligible to make blue properties\n");
							}
							break;
							case 2:
							green[count-1]++;
							printf("(Green cards : %d)\n",green[count-1]);
							if(green[count-1]==3)
							{
								delay(150);
								printf("Rent doubled on all your green cards\nNow eligible to make green properties\n");
							}
							break;
							case 3:
							red[count-1]++;
							printf("(Red cards : %d)\n",red[count-1]);
							if(red[count-1]==3)
							{
								delay(150);
								printf("Rent doubled on all your red cards\nNow eligible to make red properties\n");
							}
							break;
							case 4:
							violet[count-1]++;
							printf("(Violet cards : %d)\n",violet[count-1]);
							if(violet[count-1]==3)
							{
								delay(150);
								printf("Rent doubled on all your violet cards\nNow eligible to make violet properties\n");
							}
							break;
							case 5:
							white[count-1]++;
							printf("(White cards : %d)\n",white[count-1]);
							break;
						}
						if(copy==1)
						//Two white cards which are in tie up
						{
							delay(150);
							switch(p)
							{
								case 3:
								printf("Also, rent of BEST = %d",a[13][3]);
								break;
								case 13:
								printf("Also, rent of Railways = %d",a[3][3]);
								break;
								case 2:
								printf("Also, rent of Air India = %d",a[24][3]);
								break;
								case 24:
								printf("Also, rent of Water Works = %d",a[2][3]);
								break;
								case 12:
								printf("Also, rent of Motor Boat = 200 times dice number");
								break;
								case 34:
								printf("Also, rent of Electric Company = 100 times dice number");
								break;
							}
							printf("\n");
						}
						c='\t';
						//condition bypass
						copy=a[0][0];
						mort[copy][0]=1;
						amt[count-1]=amt[count-1]+mort[copy][1]-a[p][1];
						own[p]=count;
						/*
							Double the rent if 3
							cards are owned of
							same colour group
							except white
						*/
						for(i=0;i<=35;i++)
						{
							if(blue[count-1]>=3)
							{
								if(own[i]==count&&a[i][0]==1)
								{
									if(pt[i]==2) pt[i]=3;
								}
							}
							if(green[count-1]>=3)
							{
								if(own[i]==count&&a[i][0]==2)
								{
									if(pt[i]==2) pt[i]=3;
								}
							}
							if(red[count-1]>=3)
							{
								if(own[i]==count&&a[i][0]==3)
								{
									if(pt[i]==2) pt[i]=3;
								}
							}
							if(violet[count-1]>=3)
							{
								if(own[i]==count&&a[i][0]==4)
								{
									if(pt[i]==2) pt[i]=3;
								}
							}
						}
					}
					//tie up increment
					if(own[2]==own[24]&&own[2]!=0)
					{
						pt[2]=3;
						pt[24]=3;
					}
					if(own[3]==own[13]&&own[3]!=0)
					{
						pt[3]=3;
						pt[13]=3;
					}
					//a[0][0] restored to zero
					a[0][0]=0;
				}
				if(c=='\t') goto disp;
			}
			//Buy with sufficient balance
			else if(own[p]==0&&amt[count-1]>=a[p][1])
			{
				flushall();
				printf("\nBuy for");
				delay(250);
				printf(" %d (y/n) ",a[p][1]);
				delay(200);
				printf("(Rent : ");
				copy=0;
				switch(a[p][0])
				{
					case 1:
					if(blue[count-1]>=2) printf("%d) ",a[p][3]);
					else printf("%d) ",a[p][2]);
					delay(200);
					printf("(Blue group) ? ");
					break;
					case 2:
					if(green[count-1]>=2) printf("%d) ",a[p][3]);
					else printf("%d) ",a[p][2]);
					delay(200);
					printf("(Green group) ? ");
					break;
					case 3:
					if(red[count-1]>=2) printf("%d) ",a[p][3]);
					else printf("%d) ",a[p][2]);
					delay(200);
					printf("(Red group) ? ");
					break;
					case 4:
					if(violet[count-1]>=2) printf("%d) ",a[p][3]);
					else printf("%d) ",a[p][2]);
					delay(200);
					printf("(Violet group) ? ");
					break;
					case 5:
					rent=2;
					copy=0;
					switch(p)
					{
						case 2:
						if(own[24]==count) rent=3;
						break;
						case 24:
						if(own[2]==count) rent=3;
						break;
						case 3:
						if(own[13]==count) rent=3;
						break;
						case 13:
						if(own[3]==count) rent=3;
						break;
					}
					if(rent==3) copy=1;
					if(p==2||p==3||p==13||p==24) printf("%d) ",a[p][rent]);
					if(p==12)
					{
						if(own[34]==count)
						{
							copy=1;
							printf("100 times number on dice) ");
						}
						else printf("50 times number on dice) ");
					}
					if(p==34)
					{
						if(own[12]==count)
						{
							copy=1;
							printf("200 times number on dice) ");
						}
						else printf("100 times number on dice) ");
					}
					delay(200);
					printf("(White group) ? ");
					break;
				}
				c=getch();
				if(c=='y')
				{
					amt[count-1]=amt[count-1]-a[p][1];
					own[p]=count;
					printf("\nYou own ");
					p=sum[count-1];
					printf("%s   ",place[p]);
					for(i=1;i<=3;i++)
					{
						printf(".");
						delay(100);
					}
					delay(100);
					printf("   ");
					switch(a[p][0])
					{
						case 1:
						blue[count-1]++;
						printf("(Blue cards : %d)\n",blue[count-1]);
						if(blue[count-1]==3)
						{
							delay(150);
							printf("Rent doubled on all your blue cards\nNow eligible to make blue properties\n");
						}
						break;
						case 2:
						green[count-1]++;
						printf("(Green cards : %d)\n",green[count-1]);
						if(green[count-1]==3)
						{
							delay(150);
							printf("Rent doubled on all your green cards\nNow eligible to make green properties\n");
						}
						break;
						case 3:
						red[count-1]++;
						printf("(Red cards : %d)\n",red[count-1]);
						if(red[count-1]==3)
						{
							delay(150);
							printf("Rent doubled on all your red cards\nNow eligible to make red properties\n");
						}
						break;
						case 4:
						violet[count-1]++;
						printf("(Violet cards : %d)\n",violet[count-1]);
						if(violet[count-1]==3)
						{
							delay(150);
							printf("Rent doubled on all your violet cards\nNow eligible to make violet properties\n");
						}
						break;
						case 5:
						white[count-1]++;
						printf("(White cards : %d)\n",white[count-1]);
						break;
					}
					if(copy==1)
					{
						delay(150);
						switch(p)
						{
							case 3:
							printf("Also, rent of BEST = %d",a[13][3]);
							break;
							case 13:
							printf("Also, rent of Railways = %d",a[3][3]);
							break;
							case 2:
							printf("Also, rent of Air India = %d",a[24][3]);
							break;
							case 24:
							printf("Also, rent of Water Works = %d",a[2][3]);
							break;
							case 12:
							printf("Also, rent of Motor Boat = 200 times dice number");
							break;
							case 34:
							printf("Also, rent of Electric Company = 100 times dice number");
							break;
						}
						printf("\n");
					}
					for(i=0;i<=35;i++)
					{
						if(blue[count-1]>=3)
						{
							if(own[i]==count&&a[i][0]==1)
							{
								if(pt[i]==2) pt[i]=3;
							}
						}
						if(green[count-1]>=3)
						{
							if(own[i]==count&&a[i][0]==2)
							{
								if(pt[i]==2) pt[i]=3;
							}
						}
						if(red[count-1]>=3)
						{
							if(own[i]==count&&a[i][0]==3)
							{
								if(pt[i]==2) pt[i]=3;
							}
						}
						if(violet[count-1]>=3)
						{
							if(own[i]==count&&a[i][0]==4)
							{
								if(pt[i]==2) pt[i]=3;
							}
						}
					}
				}
				if(own[2]==own[24]&&own[2]!=0)
				{
					pt[2]=3;
					pt[24]=3;
				}
				if(own[3]==own[13]&&own[3]!=0)
				{
					pt[3]=3;
					pt[13]=3;
				}
			}
			/*
				Electric company rent = 50*dice number
				Water works rent = 100*dice number
				rent doubles if both are owned by same player
				but rent isn't fixed amt
				depends on dice value
				so special conditions for them
			*/
			else if(p==12&&own[p]!=count)
			{
				if(own[p]==own[34]) rent=100*dice;
				else rent=50*dice;
				amt[count-1]=amt[count-1]-rent;
				p=own[p]-1;
				amt[p]=amt[p]+rent;
				printf("Paid %d to %s\n",rent,player[p]);

			}
			else if(p==34&&own[p]!=count)
			{
				if(own[p]==own[12]) rent=200*dice;
				else rent=100*dice;
				amt[count-1]=amt[count-1]-rent;
				p=own[p]-1;
				amt[p]=amt[p]+rent;
				printf("Paid %d to %s\n",rent,player[p]);

			}
			//rent on other cards
			else if(own[p]!=count&&own[p]!=0)
			{
				p=pt[p];
				//point which rent to be taken
				rent=sum[count-1];
				//rent substituted for p
				amt[count-1]=amt[count-1]-a[rent][p];
				//rent taken
				printf("Paid %d ",a[rent][p]);
				rent=a[rent][p];
				//rent
				p=sum[count-1];
				//p restored
				p=own[p]-1;
				//whom to pay
				amt[p]=amt[p]+rent;
				printf("to %s ",player[p]);
				//rent credited successfully gg
				p=sum[count-1];
				//p restored
				if(a[p][0]<=4)
				//All colours except white
				{
					switch(pt[p])
					//which rent paid?
					{
						case 2:
						printf("(Rent site only)");
						break;
						case 3:
						printf("(Double rent)");
						break;
						default:
						printf("(Rent with %d propert",prop[p]);
						if(prop[p]==1) printf("y)");
						else printf("ies)");
					}
				}
				printf("\n");
			}
			//to make property
			else if(own[p]==count&&amt[count-1]>=a[p][8])
			{
				//3 or more cards required
				switch(a[p][0])
				{
					case 1:
					if(blue[count-1]<3) printf("\nWelcome to your place\n");
					else if(blue[count-1]>=3&&prop[p]<4)
					{
						flushall();
						printf("\nProperty %d ",prop[p]+1);
						delay(200);
						printf("for %d ",a[p][8]);
						rent=pt[p];
						delay(200);
						printf("(Rent : %d, else : %d) ",a[p][rent+1],a[p][rent]);
						delay(100);
						printf("(y/n) ? ");
						c=getch();
						if(c=='y')
						{
							p=sum[count-1];
							pt[p]++;
							prop[p]++;
							amt[count-1]=amt[count-1]-a[p][8];
							printf("\nProperty constructed\n");
						}
					}
					break;
					case 2:
					if(green[count-1]<3) printf("\nWelcome to your place\n");
					else if(green[count-1]>=3&&prop[p]<4)
					{
						flushall();
						printf("\nProperty %d ",prop[p]+1);
						delay(200);
						printf("for %d ",a[p][8]);
						rent=pt[p];
						delay(200);
						printf("(Rent : %d, else : %d) ",a[p][rent+1],a[p][rent]);
						delay(100);
						printf("(y/n) ? ");
						c=getch();
						if(c=='y')
						{
							p=sum[count-1];
							pt[p]++;
							prop[p]++;
							amt[count-1]=amt[count-1]-a[p][8];
							printf("\nProperty constructed\n");
						}
					}
					break;
					case 3:
					if(red[count-1]<3) printf("\nWelcome to your place\n");
					else if(red[count-1]>=3&&prop[p]<4)
					{
						flushall();
						printf("\nProperty %d ",prop[p]+1);
						delay(200);
						printf("for %d ",a[p][8]);
						rent=pt[p];
						delay(200);
						printf("(Rent : %d, else : %d) ",a[p][rent+1],a[p][rent]);
						delay(100);
						printf("(y/n) ? ");
						c=getch();
						if(c=='y')
						{
							p=sum[count-1];
							pt[p]++;
							prop[p]++;
							amt[count-1]=amt[count-1]-a[p][8];
							printf("\nProperty constructed\n");
						}
					}
					break;
					case 4:
					if(violet[count-1]<3) printf("\nWelcome to your place\n");
					else if(violet[count-1]>=3&&prop[p]<4)
					{
						flushall();
						printf("\nProperty %d ",prop[p]+1);
						delay(200);
						printf("for %d ",a[p][8]);
						rent=pt[p];
						delay(200);
						printf("(Rent : %d, else : %d) ",a[p][rent+1],a[p][rent]);
						delay(100);
						printf("(y/n) ? ");
						c=getch();
						if(c=='y')
						{
							p=sum[count-1];
							pt[p]++;
							prop[p]++;
							amt[count-1]=amt[count-1]-a[p][8];
							printf("\nProperty constructed\n");
						}
					}
					break;
					case 5:
					printf("\nWelcome to your place\n");
					break;
				}

			}
			//make property by mortgage
			else if((own[p]==count&&amt[count-1]<a[p][8])&&prop[p]<4)
			{
				copy=0;
				rent=6000;
				/*
					highest mortgage value
					decrement to match the needs
					lowest mortgage value yet
					gives amt to make property
					such a card will be selected
				*/
				switch(a[p][0])
				{
					case 1:
					if(blue[count-1]>=3)
					{
						for(i=0;i<=35;i++)
						{
							if(own[i]==count&&mort[i][0]==0)
							{
								if(amt[count-1]+mort[i][1]>=a[p][8])
								{
									if(mort[i][1]<rent)
									{
										copy=i;
										rent=mort[i][1];
									}
								}
							}
						}
					}
					break;
					case 2:
					if(green[count-1]>=3)
					{
						for(i=0;i<=35;i++)
						{
							if(own[i]==count&&mort[i][0]==0)
							{
								if(amt[count-1]+mort[i][1]>=a[p][8])
								{
									if(mort[i][1]<rent)
									{
										copy=i;
										rent=mort[i][1];
									}
								}
							}
						}
					}
					break;
					case 3:
					if(red[count-1]>=3)
					{
						for(i=0;i<=35;i++)
						{
							if(own[i]==count&&mort[i][0]==0)
							{
								if(amt[count-1]+mort[i][1]>=a[p][8])
								{
									if(mort[i][1]<rent)
									{
										copy=i;
										rent=mort[i][1];
									}
								}
							}
						}
					}
					break;
					case 4:
					if(violet[count-1]>=3)
					{
						for(i=0;i<=35;i++)
						{
							if(own[i]==count&&mort[i][0]==0)
							{
								if(amt[count-1]+mort[i][1]>=a[p][8])
								{
									if(mort[i][1]<rent)
									{
										copy=i;
										rent=mort[i][1];
									}
								}
							}
						}
					}
					break;
				}
				if(copy==0)
				//if no match found or not eligible
				{
					p=sum[count-1];
					switch(a[p][0])
					{
						case 1:
						if(blue[count-1]>=3) printf("\nNot enough amount to make property\n");
						else printf("\nWelcome to your place\n");
						break;
						case 2:
						if(green[count-1]>=3) printf("\nNot enough amount to make property\n");
						else printf("\nWelcome to your place\n");
						break;
						case 3:
						if(red[count-1]>=3) printf("\nNot enough amount to make property\n");
						else printf("\nWelcome to your place\n");
						break;
						case 4:
						if(violet[count-1]>=3) printf("\nNot enough amount to make property\n");
						else printf("\nWelcome to your place\n");
						break;
						case 5:
						//property option isn't supported on white cards
						printf("\nWelcome to your place\n");
						break;
					}
				}
				else
				{
					printf("Amount = %ld",amt[count-1]);
					delay(200);
					printf("\nMortgage ");
					delay(150);
					printf("%s",place[copy]);
					delay(100);
					printf(" for %d\n",mort[copy][1]);
					delay(150);
					printf("And make property %d for %d ",prop[p]+1,a[p][8]);
					rent=prop[p]+3;
					delay(200);
					printf("(rent : %d , else : %d) (y/n) ? ",a[p][rent+1],a[p][rent]);
					flushall();
					c=getch();
					if(c=='y')
					{
						prop[p]++;
						//add property
						pt[p]++;
						//increment rent pointer
						amt[count-1]=amt[count-1]+mort[copy][1];
						//add card mortgage value
						mort[copy][0]=1;
						//mortgaged
						amt[count-1]=amt[count-1]-a[p][8];
						//property amt deduction
						printf("\nProperty constructed\n");
						goto disp;
					}
				}
			}
		}
		/*
			if loan is taken and sufficient balance is there in acc
			take one installment of 5500 back
		*/
		if(amt[count-1]>=13000&&loan[count-1]>0)
		{
			printf("Amount=%ld\n",amt[count-1]);
			amt[count-1]=amt[count-1]-5500;
			loan[count-1]=loan[count-1]-5500;
			delay(150);
			printf("Auto returned installment of 5500\n");
			if(loan[count-1]==0)
			//if no loan left
			{
				delay(100);
				printf("Loan cleared\n");
			}
			else
			{
				delay(100);
				printf("Loan left : ");
				delay(200);
				printf("%ld",loan[count-1]);
			}
		}
		/*
			if cards mortgaged and amt is sufficient
			the card with least mortgage value is taken back
		*/
		else if(amt[count-1]>=12000)
		{
			copy=0;
			rent=5250;
			//Highest rent
			for(i=0;i<=35;i++)
			{
				if(own[i]==count&&mort[i][0]==1)
				{
					if(mort[i][1]<=rent)
					{
						copy=i;
						rent=mort[i][1];
					}
				}
			}
			if(copy!=0)
			//copy not equal to zero means card(s) mortgaged
			{
				printf("Amount = %ld , ",amt[count-1]);
				delay(150);
				mort[copy][0]=0;
				amt[count-1]=amt[count-1]-mort[copy][1];
				printf("Auto-Taken ");
				delay(200);
				printf("%s",place[copy]);
				delay(150);
				printf(" back for %d\n",rent);
			}
			goto disp;
		}
		else if(amt[count-1]<=1000)
		/*
			ask for loan if less amount
		*/
		{
			flushall();
			printf("Amount = %ld, Loan of 5000 at 10 percent interest (y/n) ? ",amt[count-1]);
			c=getch();
			if(c=='y')
			{
				amt[count-1]=amt[count-1]+5000;
				loan[count-1]=loan[count-1]+5500;
				/*
					credit 5000
					loan to be paid 5500 (10 percent interest)
				*/
				printf("\nCredited 5000 successfully\n");
			}
		}
		/*
			ask to pay back loan
		*/
		else if(amt[count-1]>=6500&&loan[count-1]>0)
		{
			printf("Amount = %ld, Pay back installment of 5500 (y/n)? ",amt[count-1]);
			c=getch();
			if(c=='y')
			{
				amt[count-1]=amt[count-1]-5500;
				loan[count-1]=loan[count-1]-5500;
				//loan cleared or not?
				if(loan[count-1]==0) printf("\nLoan cleared\n");
				else printf("\nLoan left = %ld\n",loan[count-1]);
			}
		}
		end:
		//ask for mortgage if less amt
		if(amt[count-1]<=1500)
		{
			rent=0;
			for(i=0;i<=35;i++)
			{
				if(own[i]==count&&mort[i][0]==0)
				{
					rent=rent+1;
				}
			}
			if(rent>0)
			{
				ask:
				if(amt[count-1]>1000) printf("Amount = %ld , ",amt[count-1]);
				else printf("\n");
				printf("Mortgage a card(y/n) ? ");
				flushall();
				c=getch();
				if(c=='y')
				{
					clrscr();
					printf("%s  ",player[count-1]);
					p=count-1;
					if(blue[p]>0||green[p]>0||red[p]>0||violet[p]>0)
					{
						printf("(");
						if(blue[p]>0) printf("Blue : %d ",blue[p]);
						if(green[p]>0) printf("Green : %d ",green[p]);
						if(red[p]>0) printf("Red : %d ",red[p]);
						if(violet[p]>0) printf("Violet : %d",violet[p]);
						printf(")\n");
					}
					else printf("\n");
					for(i=0;i<=35;i++)
					{
						if(own[i]==count&&mort[i][0]==0)
						{
							delay(100);
							printf("%2d.",i);
							printf("%s",place[i]);
							printf(", ");
							switch(a[i][0])
							{
								case 1:
								printf("Blue");
								break;
								case 2:
								printf("Green");
								break;
								case 3:
								printf("Red");
								break;
								case 4:
								printf("Violet");
								break;
								case 5:
								printf("White");
								break;
							}
							printf(", %d\n",mort[i][1]);
						}
					}
					printf("Enter card number : ");
					j=scanf("%d",&i);
					if(own[i]==count&&mort[i][0]==0&&j==1)
					{
						amt[count-1]=amt[count-1]+mort[i][1];
						mort[i][0]=1;
						clrscr();
						printf("%s\n",player[count-1]);
						printf("Mortgaged ");
						printf("%s",place[i]);
						printf(" for %d",mort[i][1]);
					}
					else
					{
						flushall();
						goto ask;
					}
				}
			}
		}
		else if(amt[count-1]>=7000)
		{
			rent=0;
			for(i=0;i<=35;i++)
			{
				if(own[i]==count&&mort[i][0]==1)
				{
					rent=rent+1;
				}
			}
			if(rent>0)
			{
				if(loan[count-1]==0) printf("Amount = %ld, ",amt[count-1]);
				ask_back:
				if(rent>1) printf("You have %d cards mortgaged, take one back(y/n) ? ",rent);
				else
				{
					for(i=0;i<=35;i++)
					{
						if(own[i]==count&&mort[i][0]==1)
						{
							printf("Take ");
							printf("%s",place[i]);
							copy=i;
							printf(" back for %d(y/n) ?",mort[i][1]);
						}
					}
				}
				flushall();
				c=getch();
				if(c=='y'&&rent==1) goto ret;
				else if(c=='y')
				{
					clrscr();
					printf("%s  ",player[count-1]);
					p=count-1;
					if(blue[p]>0||green[p]>0||red[p]>0||violet[p]>0)
					{
						printf("(");
						if(blue[p]>0) printf("Blue : %d ",blue[p]);
						if(green[p]>0) printf("Green : %d ",green[p]);
						if(red[p]>0) printf("Red : %d ",red[p]);
						if(violet[p]>0) printf("Violet : %d",violet[p]);
						printf(")\n");
					  }
					  else printf("\n");
					  for(i=0;i<=35;i++)
					  {
						if(own[i]==count&&mort[i][0]==1)
						{
							printf("%2d.",i);
							printf("%s",place[i]);
							printf(", ");
							switch(a[i][0])
							{
								case 1:
								printf("Blue");
								break;
								case 2:
								printf("Green");
								break;
								case 3:
								printf("Red");
								break;
								case 4:
								printf("Violet");
								break;
								case 5:
								printf("White");
								break;
							}
							printf(", %d\n",mort[i][1]);
						}
					}
					printf("Enter card number : ");
					j=scanf("%d",&i);
					if(own[i]==count&&mort[i][0]==1&&j==1)
					{
						ret:
						if(rent==1) i=copy;
						amt[count-1]=amt[count-1]-mort[i][1];
						mort[i][0]=0;
						printf("\nTaken back ");
						printf("%s",place[i]);
						printf(" for %d",mort[i][1]);
					}
					else
					{
						flushall();
						goto ask_back;
					}
				}
			}
		}
		disp:
		rent=0;
		p=0;
		for(i=0;i<=n-1;i++)
		{
			for(j=0;j<=35;j++)
			{
				if(own[j]==i+1)
				{
					rent=rent+1;
					p=p+prop[j];
				}
			}
		}
		if(rent>0)
		{
			if(kbhit()!=0) goto new_turn;
			delay(150);
			printf("\nCards (press p for details):-\n");
			for(i=0;i<=n-1;i++)
			{
				printf("%s : ",player[i]);
				rent=0;
				for(j=0;j<=35;j++)
				{
					if(own[j]==i+1) rent=rent+1;
				}
				printf("%d   ",rent);
				if(i==n-1) printf("\n");
				else if(i==2) printf("\n");
			}
		}
		if(p>0)
		{
			if(kbhit()!=0) goto new_turn;
			delay(150);
			printf("\nProperties (press v for details):-\n");
			for(i=0;i<=n-1;i++)
			{
				printf("%s : ",player[i]);
				rent=0;
				for(j=0;j<=35;j++)
				{
					if(prop[j]>0&&own[j]==i+1) rent=rent+prop[j];
				}
				printf("%d   ",rent);
				if(i==n-1) printf("\n");
				else if(i==2) printf("\n");
			}
		}
		if(kbhit()!=0) goto new_turn;
		delay(200);
		printf("\n\nAmounts left:-\n");
		for(i=0;i<=n-1;i++)
		{
			printf("%s : %ld   ",player[i],amt[i]);
			if(i==n-1) printf("\n");
			else if(i==2) printf("\n");
		}
		rent=0;
		for(i=0;i<=35;i++)
		{
			if(own[i]!=0) rent=rent+1;
		}
		if(rent>=20&&rent<26)
		{
			delay(100);
			printf("\nCards left (Press c for rent and colour groups):-\n");
			for(i=0;i<=35;i++)
			{
				if(own[i]==0&&a[i][0]!=0)
				{
					rent=rent+1;
					printf("%s",place[i]);
					if(rent<26) printf(", ");
				}
			}
			printf("\n");
		}
		delay(100);
		flushall();
		rent=0;
		for(i=0;i<=35;i++) if(own[i]!=0) rent=rent+1;
		if(rent<5||rent>=20)
		{
			printf("\ns : stop here\t\th : hold(pause)\t\tt : set timer");
			if(offtime-(int)(difftime(t2,t1))/60<=500) printf(" (%d min left)",offtime-(int)(difftime(t2,t1))/60);
			printf("\nk : save for later\tOTHER : continue");
		}
		else
		{
			printf("\n");
			printf("c : cards left\t \ts : stop\t\tt : set timer");
			if(offtime-(int)(difftime(t2,t1))/60<=500) printf(" (%d min left)",offtime-(int)(difftime(t2,t1))/60);
			printf("\nh : hold(pause)\t \t");
			printf("OTHER : continue\tk : save for later");
		}
		rent=0;
		flushall();
		d=e=clock()/CLOCKS_PER_SEC;
		while((d-e)<20)
		{
			d=clock()/CLOCKS_PER_SEC;
			if(kbhit()!=0)
			{
				rent=1;
				break;
			}
		}
		new_turn:
		count=count+1;
		if(count>n) count=1;
		if(rent==0)
		{
			flushall();
			goto begin;
		}
		c=getch();
		rent=0;
		for(i=0;i<=35;i++) if(own[i]!=0) rent++;
		if(c=='v'&&rent>=3)
		{
			rent=0;
			for(i=0;i<=35;i++)
			{
				if(prop[i]>0) rent=rent+1;
			}
			if(rent>0)
			{
				clrscr();
				for(i=0;i<=35;i++)
				{
					if(prop[i]>0)
					{
						printf("%2d. ",i);
						printf("%s",place[i]);
						printf(" : ");
						switch(a[i][0])
						{
							case 1:
							printf("Blue");
							break;
							case 2:
							printf("Green");
							break;
							case 3:
							printf("Red");
							break;
							case 4:
							printf("Violet");
							break;
						}
						printf(" : ");
						if(prop[i]==1) printf("1 property");
						else printf("%d properties",prop[i]);
						p=own[i];
						printf(" : %s\n",player[p-1]);
					}
				}
				printf("\nPress any key to continue");
				getch();
			}
		}
		else if(c=='t')
		{
			clrscr();
			t2=time(NULL);
			local=localtime(&t2);
			printf("Its %02d:%02d\n\n",local->tm_hour,local->tm_min);
			if(offtime<30000)
			{
				rent=offtime-(int)difftime(t2,t1)/60;
				clrscr();
				printf("time remaining : %d minute",rent);
				if(rent>1) printf("s");
				printf("\n\n");
			}
			printf(" 1 : End at a particular time\n 2 : End after a set timer\n\n");
			c=getch();
			if(c=='1')
			{
				x=y=0;
				printf("Enter off time in HH<space>MM 24 hour format : ");
				scanf("%d %d",&x,&y);
				delay(1000);
				t2=time(NULL);
				local=localtime(&t2);
				if(x>23||y>59||(x==local->tm_hour&&y<=local->tm_min))
				{
					printf("Timer not set");
					flushall();
					delay(2000);
					goto begin;
				}
				printf("Okay, schedule set for %02d:%02d\n",x,y);
				delay(1000);
				if(x<local->tm_hour) x=x+24;
				if(y<local->tm_min)
				{
					y=y+60;
					x=x-1;
				}
				x=x-local->tm_hour;
				y=y-local->tm_min;
				offtime=x*60+y;
				printf("Game will end in ");
				if(x>0) printf("%d hr ",x);
				if(y>0) printf("%d min",y);
				offtime=offtime+(int)(difftime(t2,t1))/60;
				if(local->tm_sec>=45) offtime=offtime-1;
				delay(2000);
			}
			else if(c=='2')
			{
				printf("Set timer in minutes : ");
				scanf("%d",&offtime);
				flushall();
				t2=time(NULL);
				offtime=offtime+(int)difftime(t2,t1)/60;
			}
		}
		else if(c=='h')
		{
			clrscr();
			t2=time(NULL);
			i=DETECT;
			initgraph(&i,&j,"c:\\tc\\bgi");
			cleardevice();
			settextstyle(3,HORIZ_DIR,6);
			setcolor(BROWN);
			outtextxy(50,100,"Game Paused");
			setcolor(WHITE);
			delay(1000);
			settextstyle(3,HORIZ_DIR,5);
			outtextxy(50,200,"Next turn : ");
			delay(600);
			x=330;
			y=235;
			setcolor(RED);
			for(i=1;i<=20;i++)
			{
				circle(x,y,i);
				delay(10);
			}
			delay(50);
			setcolor(YELLOW);
			for(i=20;i>=1;i--)
			{
				circle(x,y,i);
				delay(10);
			}
			delay(50);
			setcolor(GREEN);
			for(i=1;i<=20;i++)
			{
				circle(x,y,i);
				delay(10);
			}
			delay(100);
			setcolor(BLACK);
			for(i=1;i<=20;i++)
			{
				circle(x,y,i);
				delay(10);
			}
			setcolor(WHITE);
			delay(150);
			outtextxy(300,200,player[count-1]);
			setcolor(GREEN);
			delay(1000);
			settextstyle(3,HORIZ_DIR,3);
			outtextxy(100,300,"Press any key to continue");
			getch();
			cleardevice();
			setcolor(WHITE);
			x=300;
			y=200;
			settextstyle(3,HORIZ_DIR,5);
			outtextxy(x,y,player[count-1]);
			delay(300);
			while(x>1||y>1)
			{
				if(x!=y)
				{
					x--;
					if(x%20==0) cleardevice();
				}
				else
				{
					settextstyle(3,HORIZ_DIR,3);
					x--;
					y--;
					if((x==1&&y==1)||y%10==0||x%10==0)
					{
						cleardevice();
						settextstyle(3,HORIZ_DIR,2);
					}
					delay(2);
				}
				outtextxy(x,y,player[count-1]);
			}
			delay(300);
			cleardevice();
			x=330;
			y=220;
			setcolor(RED);
			for(i=1;i<=20;i++)
			{
				circle(x,y,i);
				delay(10);
			}
			delay(50);
			setcolor(YELLOW);
			for(i=20;i>=1;i--)
			{
				circle(x,y,i);
				delay(10);
			}
			delay(50);
			setcolor(GREEN);
			for(i=1;i<=20;i++)
			{
				circle(x,y,i);
				delay(10);
			}
			delay(100);
			clrscr();
			cleardevice();
			closegraph();
			t3=time(NULL);
			pausetime=pausetime+(int)difftime(t3,t2);
		}
		else if(c=='c'&&(rent>=5&&rent<26))
		{
			clrscr();
			for(i=0;i<=35;i++)
			{
				if(own[i]==0&&a[i][0]>0)
				{
					delay(100);
					printf("%2d. ",i);
					printf("%s",place[i]);
					if(i==12) printf(" (rent = 50*dice) ");
					else if(i==34) printf(" (rent = 100*dice) ");
					else printf(" (rent = %d) ",a[i][2]);
					switch(a[i][0])
					{
						case 1:
						printf("(Blue)");
						break;
						case 2:
						printf("(Green)");
						break;
						case 3:
						printf("(Red)");
						break;
						case 4:
						printf("(Violet)");
						break;
						case 5:
						printf("(White)");
						break;
					}
					printf("\n");
				}
			}
			printf("\nPress any key to continue");
			getch();
			rent=26-rent;
			while(rent>0)
			{
				clrscr();
				i=0;
				copy=0;
				while(copy<rent)
				{
					i++;
					if(own[i]==0&&a[i][0]>0)
					{
						copy++;
						printf("%2d. ",i);
						printf("%s",place[i]);
						if(i==12) printf(" (rent = 50*dice) ");
						else if(i==34) printf(" (rent = 100*dice) ");
						else printf(" (rent = %d) ",a[i][2]);
						switch(a[i][0])
						{
							case 1:
							printf("(Blue)");
							break;
							case 2:
							printf("(Green)");
							break;
							case 3:
							printf("(Red)");
							break;
							case 4:
							printf("(Violet)");
							break;
							case 5:
							printf("(White)");
							break;
						}
						printf("\n");
					}
				}
				rent--;
				delay(100);
			}

		}
		else if(c=='p'&&rent>0)
		{
			clrscr();
			back_to_play:
			for(i=0;i<=n-1;i++)
			{
				printf("%d. %s  ",i+1,player[i]);
				if(i==n-1) printf("\n");
				else if(i==2) printf("\n");
			}
			printf("\nEnter player number : ");
			rent=scanf("%d",&copy);
			if(rent==0||copy>n||copy<1)
			{
				printf("Invalid player data\nPress any key to continue\n");
				flushall();
				getch();
				goto back_to_play;
			}
			printf("Card data of %s :-\n\n",player[copy-1]);
			delay(500);
			clrscr();
			rent=0;
			for(i=0;i<=35;i++)
			{
				if(own[i]==copy&&a[i][0]!=0)
				{
					delay(100);
					printf("%2d.",i);
					rent=rent+1;
					printf("%s",place[i]);
					printf(" : ");
					switch(a[i][0])
					{
						case 1:
						printf("Blue");
						break;
						case 2:
						printf("Green");
						break;
						case 3:
						printf("Red");
						break;
						case 4:
						printf("Violet");
						break;
						case 5:
						printf("White");
						break;
					}
					p=pt[i];
					if(i==12||i==34)
					{
						if(i==12)
						{
							if(own[34]==copy) printf(" : rent = 100 times dice no.");
							else printf(" : rent = 50 times dice no.");
						}
						else if(i==34)
						{
							if(own[12]==copy) printf(" : rent = 200 times dice no.");
							else printf(" : rent = 100 times dice no.");
						}
					}
					else printf(" : rent = %d",a[i][p]);
					if(prop[i]>0) printf(" : ");
					if(prop[i]==1) printf("1 property");
					else if(prop[i]>1) printf("%d properties",prop[i]);
					if(mort[i][0]==1) printf("(Mortgaged)");
					printf("\n");
				}
			}
			if(rent==0) printf("No cards bought\n");
			printf("\nPress any key to continue");
			getch();
			while(rent>0)
			{
				k=0;
				i=0;
				clrscr();
				while(k<rent)
				{
					i++;
					if(own[i]==copy&&a[i][0]!=0)
					{
						k++;
						printf("%2d.",i);
						printf("%s",place[i]);
						printf(" : ");
						switch(a[i][0])
						{
							case 1:
							printf("Blue");
							break;
							case 2:
							printf("Green");
							break;
							case 3:
							printf("Red");
							break;
							case 4:
							printf("Violet");
							break;
							case 5:
							printf("White");
							break;
						}
						p=pt[i];
						if(i==12||i==34)
						{
							if(i==12)
							{
								if(own[34]==copy) printf(" : rent = 100 times dice no.");
								else printf(" : rent = 50 times dice no.");
							}
							else if(i==34)
							{
								if(own[12]==copy) printf(" : rent = 200 times dice no.");
								else printf(" : rent = 100 times dice no.");
							}
						}
						else printf(" : rent = %d",a[i][p]);
						if(prop[i]>0) printf(" : ");
						if(prop[i]==1) printf("1 property");
						else if(prop[i]>1) printf("%d properties",prop[i]);
						if(mort[i][0]==1) printf("(Mortgaged)");
						printf("\n");
					}
				}
				delay(100);
				rent--;
			}
		}
	}
	while(c!='s'&&c!='k');
	i=DETECT;
	initgraph(&i,&j,"c:\\tc\\bgi");
	cleardevice();
	if(c=='s')
	{
		cleardevice();
		x=330;
		y=220;
		setcolor(RED);
		for(i=1;i<=20;i++)
		{
			circle(x,y,i);
			delay(10);
		}
		delay(50);
		setcolor(YELLOW);
		for(i=20;i>=1;i--)
		{
			circle(x,y,i);
			delay(10);
		}
		delay(50);
		setcolor(GREEN);
		for(i=1;i<=20;i++)
		{
			circle(x,y,i);
			delay(10);
		}
		delay(100);
		clrscr();
		cleardevice();
		save[4]=0;
		settextstyle(3,HORIZ_DIR,6);
		setcolor(WHITE);
		outtextxy(115,150,"Hold your breath");
		settextstyle(3,HORIZ_DIR,3);
		outtextxy(150,300,"Who's in the lead...Any guesses?");
		outtextxy(320,365,"Loading...");
		settextstyle(3,HORIZ_DIR,6);
		y=350;
		x=100;
		while(x<130)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(15);
		}
		setcolor(BROWN);
		outtextxy(115,150,"Hold your breath");
		while(x<160)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(12);
		}
		while(x<180)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(10);
		}
		while(x<200)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(7);
		}
		setcolor(WHITE);
		outtextxy(115,150,"Hold your breath");
		while(x<230)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(5);
		}
		while(x<420)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(4);
		}
		while(x<450)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(5);
		}
		setcolor(GREEN);
		outtextxy(115,150,"Hold your breath");
		while(x<480)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(7);
		}
		while(x<520)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(10);
		}
		while(x<550)
		{
			bar(x,y,x+1,y+1);
			x=x+1;
			delay(15);
		}
		setcolor(LIGHTBLUE);
		x=x-x%3;
		y=y-y%3;
		circle(x,y,20);
		delay(50);
		cleardevice();
		while(x>21||y>21)
		{
			setcolor(BLACK);
			circle(x,y,20);
			if(x>y) x=x-3;
			else if(x<y) y=y-3;
			else
			{
				x=x-3;
				y=y-3;
			}
			setcolor(WHITE);
			circle(x,y,20);
			delay(3);
		}
		cleardevice();
		show_data:
		x=5;
		y=5;
		settextstyle(3,HORIZ_DIR,3);
		setcolor(MAGENTA);
		outtextxy(x,y,"BUSINESS");
		settextstyle(3,HORIZ_DIR,2);
		setcolor(YELLOW);
		delay(200);
		outtextxy(x+108,y+3,",a lockdown project");
		delay(400);
		printf("\n\n\n  Final amounts:-\n");
		delay(500);
		for(i=0;i<=n-1;i++)
		{
			for(j=1;j<=35;j++)
			{
				if(own[j]==i+1)
				{
					if(mort[j][0]==0) amt[i]=amt[i]+a[j][1]+prop[j]*a[j][8];
					else amt[i]=amt[i]+mort[j][1]+prop[j]*a[j][8];
				}
			}
			amt[i]=amt[i]-loan[i];
			printf("  %s : %ld\n",player[i],amt[i]);
			delay(250);
		}
		delay(200);
		rent=0;
		for(i=0;i<=35;i++)
		{
			if(own[i]>0) rent=rent+1;
		}
		if(rent>0)
		{
			delay(1000);
			printf("\n  Cards :-\n  ");
			delay(500);
			for(i=0;i<=n-1;i++)
			{
				rent=0;
				for(j=0;j<=35;j++)
				{
					if(own[j]==i+1) rent=rent+1;
				}
				printf("%s : %d   ",player[i],rent);
				if(i==n-1||i==2) printf("\n  ");
				delay(250);
			}
		}
		rent=0;
		for(i=0;i<=35;i++)
		{
			if(prop[i]>0) rent=rent+1;
		}
		if(rent>0)
		{
			delay(1500);
			printf("\n  Properties :-\n  ");
			delay(500);
			for(i=0;i<=n-1;i++)
			{
				rent=0;
				for(j=0;j<=35;j++)
				{
					if(own[j]==i+1&&prop[j]>0) rent=rent+prop[j];
				}
				printf("%s : %d   ",player[i],rent);
				if(i==n-1||i==2) printf("\n  ");
				delay(250);
			}
		}
		delay(1000);
		printf("\n\n  Rounnds played : ");
		delay(500);
		printf("%d",round[0]);
		delay(750);
		t3=time(NULL);
		rent=(int)difftime(t2,t1);
		local=localtime(&t3);
		printf("\n\n  Its %02d:%02d:%02d\n",local->tm_hour,local->tm_min,local->tm_sec);
		delay(200);
		if(pausetime>0)
		{
			delay(200);
			printf("\n  Total time = %02d:%02d:%02d\n",rent/3600,(rent/60)%60,rent%60);
			rent=rent-pausetime;
			delay(300);
			printf("  Active time = %02d:%02d:%02d\n",rent/3600,(rent/60)%60,rent%60);
			rent=pausetime;
			delay(300);
			printf("  Inactivity = %02d:%02d:%02d\n",rent/3600,(rent/60)%60,rent%60);
		}
		else printf("\n  time played = %02d:%02d:%02d\n",rent/3600,(rent/60)%60,rent%60);
		fp=fopen("gamedata.txt","r");
		fclose(fp);
	}
	else
	{
		x=5;
		y=5;
		settextstyle(3,HORIZ_DIR,3);
		setcolor(MAGENTA);
		outtextxy(x,y,"BUSINESS");
		settextstyle(3,HORIZ_DIR,2);
		setcolor(YELLOW);
		delay(200);
		outtextxy(x+108,y+3,",a lockdown project");
		delay(400);
		fp=fopen("gamedata.txt","w");
		fprintf(fp,"%d\n",n);
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%s\n",player[i]);
		}
		fprintf(fp," (%02d/%02d/%04d ",local->tm_mday,1+(local->tm_mon),1900+(local->tm_year));
		fprintf(fp,"%02d:%02d)\n",local->tm_hour,local->tm_min);
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%ld ",amt[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",sum[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",chance[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",round[i]);
		}
		fprintf(fp,"\n");

		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",px[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",py[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%ld ",loan[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",blue[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",green[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",red[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",violet[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=n-1;i++)
		{
			fprintf(fp,"%d ",white[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=35;i++)
		{
			fprintf(fp,"%d ",own[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=35;i++)
		{
			fprintf(fp,"%d ",prop[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=35;i++)
		{
			fprintf(fp,"%d ",pt[i]);
		}
		fprintf(fp,"\n");
		for(i=0;i<=35;i++)
		{
			fprintf(fp,"%d ",mort[i][0]);
		}
		fprintf(fp,"\n");
		fprintf(fp,"%d",count);
		fclose(fp);
		fp=fopen("gamedata.txt","r");
		printf("\n\n\n");
		if(save[4]==0)
		{
			copy=n;
			for(i=1;i<=8;i++)
			{
				printf("%d : ",i);
				save[4]=i+'0';
				fp=fopen(save,"r");
				if(fp!=NULL)
				{
					fscanf(fp,"%d",&n);
					c=getc(fp);
					c=0;
					for(i=0;i<=n;i++)
					{
						while(c!='\n')
						{
							c=getc(fp);
							if(c=='\n')
							{
								if(i<n-1) printf(", ");
							}
							else printf("%c",c);
						}
						c=0;
					}
				}
				printf("\n");
				fclose(fp);
				i=save[4]-'0';
			}
			c=0;
			printf("Enter game number to save : ");
			while(c<'1'||c>'8')
			{
				c=getch();
			}
			printf("%c",c);
			delay(300);
			save[4]=c;
			n=copy;
			clrscr();
			cleardevice();
			x=5;
			y=5;
			settextstyle(3,HORIZ_DIR,3);
			setcolor(MAGENTA);
			outtextxy(x,y,"BUSINESS");
			settextstyle(3,HORIZ_DIR,2);
			setcolor(YELLOW);
			delay(200);
			outtextxy(x+108,y+3,",a lockdown project");
			printf("\n\n\n");
		}
		cp=fopen(save,"w");
		fclose(fp);
		fp=fopen("gamedata.txt","r");
		c=getc(fp);
		while(!feof(fp))
		{
			fputc(c,cp);
			c=getc(fp);
		}
		fclose(cp);
		x=330;
		y=220;
		setcolor(RED);
		for(i=1;i<=20;i++)
		{
			circle(x,y,i);
			delay(10);
		}
		delay(50);
		setcolor(YELLOW);
		for(i=20;i>=1;i--)
		{
			circle(x,y,i);
			delay(10);
		}
		delay(50);
		setcolor(GREEN);
		for(i=1;i<=20;i++)
		{
			circle(x,y,i);
			delay(10);
		}
		setcolor(BLACK);
		for(i=1;i<=20;i++)
		{
			circle(x,y,i);
			delay(10);
		}
		delay(200);
		printf("  Game %c saved\n",save[4]);
		delay(200);
		printf("\n  Showing latest data");
		delay(1000);
		fclose(fp);
		c='s';
		clrscr();
		cleardevice();
		goto show_data;
	}
	if(save[4]!=0) printf("\n  Game %c saved",save[4]);
	printf("\n\n  Press any key to exit");
	getch();
	cleardevice();
	if(save[4]==0) save[4]='0';
	delay(100);
	fp=fopen("gamedata.txt","w");
	fputc(save[4],fp);
	fclose(fp);
	x=5;
	y=5;
	setcolor(MAGENTA);
	settextstyle(3,HORIZ_DIR,3);
	outtextxy(x,y,"BUSINESS");
	delay(100);
	setcolor(WHITE);
	settextstyle(3,HORIZ_DIR,3);
	outtextxy(x,y,"BUSINESS");
	delay(100);
	while(x<185||y<350)
	{
		setcolor(BLACK);
		outtextxy(x,y,"BUSINESS");
		if(x<185&&y>=165) x=x+15;
		if(y<350) y=y+15;
		setcolor(WHITE);
		outtextxy(x,y,"BUSINESS");
		delay(10);
	}
	delay(250);
	i=3;
	while(i<6)
	{
		setcolor(BLACK);
		outtextxy(x,y,"BUSINESS");
		i=i+1;
		settextstyle(3,HORIZ_DIR,i);
		setcolor(WHITE);
		outtextxy(x,y,"BUSINESS");
		delay(200);
	}
	setcolor(MAGENTA);
	outtextxy(x,y,"BUSINESS");
	delay(200);
	settextstyle(3,HORIZ_DIR,6);
	setcolor(6);
	outtextxy(200,150,"THANKS");
	delay(200);
	setcolor(WHITE);
	outtextxy(240,200,"FOR");
	delay(200);
	setcolor(GREEN);
	outtextxy(192,250,"PLAYING");
	delay(500);
	setcolor(BLACK);
	outtextxy(185,350,"BUSINESS");
	delay(1000);
	cleardevice();
	if(kbhit()!=0) goto bye_bye;
	setcolor(RED);
	circle(290,190,40);
	delay(200);
	if(kbhit()!=0) goto bye_bye;
	circle(370,190,40);
	delay(300);
	line(250,190,250,210);
	line(410,190,410,210);
	if(kbhit()!=0) goto bye_bye;
	delay(500);
	line(250,210,330,330);
	line(410,210,330,330);
	x=330;
	delay(250);
	while(x>250)
	{
		line(330,330,x,210);
		line(330,330,660-x,210);
		delay(10);
		x=x-1;
	}
	if(kbhit()!=0) goto bye_bye;
	x=250;
	y=210;
	while(y>190)
	{
		y=y-1;
		line(x,y,x+160,y);
		delay(10);
	}
	if(kbhit()!=0) goto bye_bye;
	x=290;
	y=190;
	for(i=1;i<=40;i++)
	{
		circle(x,y,i);
		circle(x+80,y,i);
		delay(3);
	}
	delay(2000);
	bye_bye:
	cleardevice();
	closegraph();
}