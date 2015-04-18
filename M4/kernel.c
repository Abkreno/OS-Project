void printString(char*);
void readString(char*);
void readSector(char*, int);
void writeSector(char*, int);
void readFile(char* , char*);
void deleteFile(char*);
void executeProgram(char*, int);
void handleInterrupt21(int, int, int, int);
void terminateProgram();
void println();
int div(int, int);
int mod(int, int);

char line[82];
char buffer[13312]; /*this is the maximum size of a file*/
char sector[512]; // temp array to read sectors on

main( void )
{
	/*
	readString(line);
	printString(line);
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return
	*/

	// makeInterrupt21();
	// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/

	/*printString("Enter a line:\n\0");
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return

	makeInterrupt21();

	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return
	*/

	// makeInterrupt21();
	// interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
	// while(1);

	makeInterrupt21();
	interrupt(0x21,4,"shell\0",0x2000,0);

}

void printString(char* chars)
{
	int i;
	for (i = 0; chars[i] != '\0'; i++){
		int currChar = chars[i];
		interrupt(0x10, 0xE*256+currChar, 0, 0, 0);
	};
}

void println()
{
	interrupt(0x10, 0xE*256+0xa, 0, 0, 0); //line feed "new line"
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); //carriage return
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
			chars[index] = 0x0;                    //end of string
			break;
		}else if(x == backspace && index!=0){
			interrupt(0x10, 0xE*256+x, 0, 0, 0);     // print backspace
			interrupt(0x10, 0xE*256+0x20, 0, 0, 0);  // print space
			interrupt(0x10, 0xE*256+x, 0, 0, 0);     // print backspace again
		    index--;
		}else if (x != backspace && index < 80) {
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

void writeSector(char* buffer, int sector)
{
	int CX,DX,CH,CL,DH,DL;
	CH = div(sector , 36);
    CL = mod(sector , 18) + 1;
    DH = div(sector , 18);
    DH = mod(DH ,2);
    DL = 0;
    CX = CH*256 + CL;
    DX = DH*256 + DL;
	interrupt(0x13, 0x301, buffer, CX, DX);
}

void readFile(char* buffer, char* fileName)
{
	int i,j,entry,sectorNum,count;
	int flag = 0;
	readSector(sector,2);

	for (i = 0; i < 16; i++){
		for (j = 0; j < 6; j++){
			if(sector[i*32 + j]!=fileName[j]){
				break;
			}
		};
		if(j == 6){
			flag = 1;
			entry = i;
			break;
		}
	};

	if(flag==0){
		return;
	}

	count = 0;
	for (j = 6; j < 32; j++){
		sectorNum = sector[entry*32+j];
		if(sectorNum == 0)
			break;
		readSector(buffer+count,sectorNum);
		count = count + 512;
	}
}

void deleteFile(char* name)
{
	
}

void executeProgram(char* name,int segment) {

	int offset = 0x0000;
	int base = segment*10;
	int i = 0;

	readFile(buffer,name);

	while(i < 13312) {
		putInMemory(segment,offset,buffer[i++]);
		offset++;
	}

	launchProgram(segment);
}

void terminateProgram()
{
	char name[6];
	name[0] = 's';
	name[1] = 'h';
	name[2] = 'e';
	name[3] = 'l';
	name[4] = 'l';
	name[5] = '\0';
	interrupt(0x21,4,name,0x2000,0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0){
		printString(bx);
	}else if(ax == 1){
		readString(bx);
	}else if(ax == 2){
		readSector(bx,cx);
	}else if(ax == 3){
		readFile(cx,bx);
	}else if(ax == 4){
		executeProgram(bx,cx);
	}else if(ax == 5){
		terminateProgram();
	}else if(ax == 6){
		writeSector(bx,cx);
	}else if (ax == 10) {
		println();
	}else{
		printString("Error AX value should be < 11\0");
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
