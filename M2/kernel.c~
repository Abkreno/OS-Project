void printString(char*);

main( void )
{
 char x; 
 while(1){
  x = interrupt(0x16, 0, 0, 0, 0);
  interrupt(0x10, 0xE*256+x, 0, 0, 0);  
 }
//printString("HelloWorld\0");
 while(1);
}

void printString(char* chars){
  int i;
  for (i = 0; chars[i] != '\0'; i++){
     int currChar = chars[i];
     interrupt(0x10, 0xE*256+currChar, 0, 0, 0);
  };
}
