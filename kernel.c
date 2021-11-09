void printString(char* s);
void printChar(char c);
void readString(char* line);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
main()
{
	char* buffer[512];
	char* line[80];
	makeInterrupt21();
	interrupt(0x21,0,"Enter a line: ",0,0);
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	interrupt(0x21,2,buffer,30,0);
	interrupt(0x21,0,buffer,0,0);
	while(1);
}

void printString(char* chars)
{
	int i=0;
	while(chars[i]!='\0')
		printChar(chars[i++]);
}

void printChar(char c)
{
	interrupt(0x10,0x0e*0x100+c,0,0);
}

void readString(char* line)
{
	int i=0;
	char c;
	while(1)
	{
		c=interrupt(0x16,0,0,0,0);
		if(c==0x8)
		{
			if(i>0)
			{
				i--;
				printChar(0x8);
				printChar(' ');
				printChar(0x8);
			}
		}
		else if(c==0xd)
			break;
		else
		{
			line[i++]=c;
			printChar(c);
		}
	}
	line[i++]=0xd;
	line[i++]=0xa;
	line[i++]=0;
	printChar(0xd);
	printChar(0xa);
}

void readSector(char* buffer, int sector)
{
	int AH=2;
	int AL=1;
	int BX=buffer;
	int CH=0;
	int CL=sector+1;
	int DH=0;
	int DL=0x80;
	int AX=AH*256+AL;
	int CX=CH*256+CL;
 	int DX=DH*256+DL;
	interrupt(0x13,AX,BX,CX,DX);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax==0)
		printString(bx);
	else if(ax==1)
		readString(bx);
	else if(ax==2)
		readSector(bx,cx);
	else
		printString("Unhandled Interrupt 21");
}
