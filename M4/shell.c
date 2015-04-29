int equals(char*,char*);
void clearBuffer(char* buffer, int length);
void copyMessageToStdout(char*);
int div(int a, int b);
char command[81];
char argPtr1[81];
char argPtr2[81];
char stdout[81];
char file[13312];
int sectors ;

main( void ) {
  int i,j;
  while(1) {

    //prompt
    interrupt(0x21,0,"shell$\0",0,0);

    //reading a command
    interrupt(0x21,1,command,0,0);

    //pointing to the arg beginning
    i = 0;
    j = 0;
    while(command[i] != '\0' && command[i] != ' ') {
      i++;
    }
    if(command[i]==' '){
  		command[i] = '\0';
    	i++;
      while((argPtr1[j++]=command[i++]) != '\0' && command[i] != ' ');

      // Dirty implementation for arguments list
      i++;
      j=0;
      while((argPtr2[j++]=command[i++]) != '\0');

    }

    if(equals(command,"view\0")) {

      copyMessageToStdout("file not found");
      interrupt(0x21,3,argPtr1,file,stdout); // readFile
      file[13312] = '\0';
      interrupt(0x21,0,file,0,0); // print File

    } else if(equals(command,"execute\0")) {

      interrupt(0x21,4,argPtr1,0x2000,0); // executeProgram

    } else if(equals(command,"delete\0")) {

      copyMessageToStdout("file not found");
      interrupt(0x21,7,argPtr1,0,stdout); // deleteFile

    } else if(equals(command,"copy\0")){

      interrupt(0x21,3,argPtr1,file,0); //reading the file in buffer
      sectors = calculateSectors(file);
      interrupt(0x21,8,argPtr2,file,sectors); //copy File

    } else{

      interrupt(0x21,0,"command not found\0",0,0);
      interrupt(0x21,10,0,0,0);

    }

    clearBuffer(file,13312);
    clearBuffer(argPtr1,81);
    clearBuffer(argPtr2,81);


  }

}

int calculateSectors(char* buffer){
  int i = 0 ;
  while(buffer[i++]!='\0');
  return div(i,512) + 1;
}

void clearBuffer(char* buffer, int length){
  int i;
  for(i=0;i<length;i++){
    buffer[i] = 0x00;
  }
}

void copyMessageToStdout(char* message){
  int i;
  for (i = 0; message[i] != '\0'; i++){
    stdout[i] = message[i];
  };
}

int equals(char* str1 , char* str2)
{
	int i = 0;
	while(1)
	{
		if(str1[i] != str2[i])
			return 0;
		if(str1[i++] == '\0')
			return 1;
	}
}

int div(int a, int b)
{
    int res;
    for (res = 0; a >= b; a -= b, res++);

    return res;
}
