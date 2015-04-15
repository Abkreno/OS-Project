int equals(char*,char*);

main( void ) {

  while(1) {
    char* command;

    //prompt
    interrupt(0x21,0,"shell$\0",0,0);

    //reading a command
    interrupt(0x21,1,command,0,0);

    char** splitted = split(command);
    if(equals(splitted[0],"view")) {
      interrupt(0x21,3,splitted[1],0,0); // readFile
    }else if(equals(splitted[0],"execute")){
      interrupt(0x21,4,splitted[1],0x2000,0); // executeProgram
    }
    else{
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
