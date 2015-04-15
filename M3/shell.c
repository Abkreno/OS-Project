int equals(char*,char*);
char* command;
char* argPtr;

main( void ) { 
  int i;
  while(1) {

    //prompt
    interrupt(0x21,0,"shell$\0",0,0);

    //reading a command
    interrupt(0x21,1,command,0,0);
   
    //pointing to the arg beginning
    argPtr = command;
    i = 0;

    while(command[i] != '\0' && command[i] != ' ') {
      i++;
      argPtr++;
    }
    
    if(command[i]==' ')
      argPtr++;

    interrupt(0x21,0,argPtr,0,0);
    interrupt(0x21,10,0,0,0);
    
    if(equals(command,"view\0")) {
      char* file;
      interrupt(0x21,3,file,argPtr,0); // readFile
      interrupt(0x21,0,file,0,0); // print File
    }else if(equals(command,"execute\0")){
      interrupt(0x21,4,argPtr,0x2000,0); // executeProgram
    }else{
      interrupt(0x21,0,"command not found\0",0,0);
      interrupt(0x21,10,0,0,0);
    }

  }

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
