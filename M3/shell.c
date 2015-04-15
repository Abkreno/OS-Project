main( void ) {

  while(1) {
    char* command;

    //prompt
    interrupt(0x21,0,"shell$\0",0,0);

    //reading a command
    interrupt(0x21,1,command,0,0);

    //check for command avaliablility
    if(0) {
      interrupt(0x21,0,command + "command not found\0",0,0);
    }

  }

}
