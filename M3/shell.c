int equals(char*,char*);
int strlen(char*);
char** str_split(char*, char);

main( void ) {
  char* command;
  char** splitted;
  while(1) {
    
    //prompt
    interrupt(0x21,0,"shell$\0",0,0);

    //reading a command
    interrupt(0x21,1,command,0,0);
    splitted = str_split(command , ' ');
       
    if(equals(splitted[0],"view\0")) {
      interrupt(0x21,3,splitted[1],0,0); // readFile
    }else if(equals(splitted[0],"execute\0")){
      interrupt(0x21,4,splitted[1],0x2000,0); // executeProgram
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

char** str_split(char* a_str, char a_delim)
{
  char** result = 0;
  int count = 0;
  int idx = 0;
  int idx2 = 0;
  char* tmp = a_str;
  char* last_comma = 0;

  /* Count how many elements will be extracted. */
  while (*tmp)
  {
  if (a_delim == *tmp)
  {
  count++;
  }
  tmp++;
  }
  if(count==2){
  }
  /* Add 2 to the count 1 for the last token
  and 1 for the end of the array */
  count+=2;

  result[count] = '\0';

  while (idx < count)
  {
  	int c = 0;
  	while(a_str[idx2] != a_delim && a_str[idx2] != '\0'){
  		result[idx][c] = a_str[idx2];
  		c++;
  		idx2++;
  	}
  	idx2++;
  	result[idx][c] = '\0';
  	idx++;
  }
  return result;
}

int strlen(char* str)
{
	int res = 0;
	while(str[res++]!='\0');
	return res-1;
}
