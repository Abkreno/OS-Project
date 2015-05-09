##Operating Systems project

A small project for operating systems course in the german universty in cairo

##Usage :

You will need [bochs emulator](http://bochs.sourceforge.net/) and a display library like `bochs-x`    

you will also need the following utilities :    

`bcc` , `as86`, `ld86` , [gcc](https://gcc.gnu.org/) and [nasm](http://en.wikipedia.org/wiki/Netwide_Assembler)

On a linux machine you can use your package manager to install these utilities , For example this is the commands for machines using [apt](http://en.wikipedia.org/wiki/Advanced_Packaging_Tool) :

        sudo apt-get install bochs &
        sudo apt-get install bochs-x &
        sudo apt-get install bcc &
        sudo apt-get install bin86 &
        sudo apt-get install nasm &
        
You will also need a configuration file , you can find it in old directories (M1,M2 and M3) , It is ignored then to add your own configuration .

To compile and run the project just navigate M4 , and type the following commands :
      
        ./compileOS    
        
        bochs -f <your-config-file>
