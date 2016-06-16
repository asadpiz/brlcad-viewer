#include <stdio.h>

int main (int argc, char* argv[]){
    unsigned char buffer[1];	
    FILE *in_file  = fopen(argv[1], "rb"); // read only
    int i;
    printf ("Filename is: %s\n", argv[1]);
      
    fread(buffer, sizeof(unsigned char), 1, in_file);  
    fclose(in_file);
    //for(i = 0; i < 40; i++)
    //            { printf("buffer[%d] == %d\n", i, buffer[i]);}
    return 0;
}
