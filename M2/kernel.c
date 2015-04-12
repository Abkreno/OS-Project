void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
int div(int, int);
int mod(int, int);

char line[82];
char buffer[512];

main( void )
{
	/*	
	readString(line);
	printString(line);
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return 
	*/
	
	
	readSector(buffer, 30);
    	printString(buffer);
	
	
	/*printString("Enter a line:\n\0");
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return 
	
	makeInterrupt21();
	
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return 
	*/
	while(1);
}

void printString(char* chars)
{

	int i;
	for (i = 0; chars[i] != '\0'; i++){
		int currChar = chars[i];
		interrupt(0x10, 0xE*256+currChar, 0, 0, 0);
	};
	
}

void readString(char* chars)
{
	int index;
	char x,enter,backspace;
	backspace = 0x8;
	enter = 0xd;
	index = 0;
   	while(1){
		x = interrupt(0x16, 0, 0, 0, 0);
		if(x == enter){		
			interrupt(0x10, 0xE*256+0xa, 0, 0, 0); //line feed "new line"
			interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return 
			chars[index++] = 0xa;                    //line feed
			chars[index] = 0x0;                    //end of string
			break;
		
		}else if(x == backspace){
			interrupt(0x10, 0xE*256+x, 0, 0, 0);     // print backspace  
			interrupt(0x10, 0xE*256+0x20, 0, 0, 0);  // print space 
			interrupt(0x10, 0xE*256+x, 0, 0, 0);     // print backspace again
			index--;
		}else if (index < 80) {
			interrupt(0x10, 0xE*256+x, 0, 0, 0);  
			chars[index++] = x;
		}
	}
}


void readSector(char* buffer, int sector)
{
	/*
	• AH = 2 (this number tells the BIOS to read a sector as opposed to write)
	• AL = number of sectors to read (use 1)
	• BX = address where the data should be stored to (pass your char* array here)
	• CH = track number
	• CL = relative sector number
	• DH = head number
	• DL = device number (for the floppy disk, use 0)
	relative sector = ( sector MOD 18 ) + 1
	head = ( sector / 18 ) MOD 2
	track = ( sector / 36 )
	*/
	
	int CX,DX,CH,CL,DH,DL;
	CH = div(sector , 36);
    CL = mod(sector , 18) + 1;
    DH = div(sector , 18);
    DH = mod(DH ,2);
    DL = 0;
    CX = CH*256 + CL;
    DX = DH*256 + DL;
	interrupt(0x13, 0x201, buffer, CX, DX);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0){
		printString(bx);
	}else if(ax == 1){
		readString(bx);
	}else if(ax == 2){
		readSector(bx);
	}else{
		printString("Error AX value should be < 3\0");
	}
}

int div(int a, int b)
{
    int res;
    for (res = 0; a >= b; a -= b, res++);

    return res;
}

int mod(int a, int b)
{
	int res = div(a, b);
	res = res*b;
	res = a - res;
	return res;
}
