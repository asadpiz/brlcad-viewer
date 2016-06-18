#include <stdio.h>

int main (int argc, char* argv[]){
    FILE* geometry_file = NULL;
    geometry_file  = fopen(argv[1], "rb"); // Open .g File
    if (geometry_file == NULL){
    printf ("ERROR Opening File\n");
    return 1;
    }
    else{
        int i; 
        size_t size; /*Size of File*/
        unsigned char *buffer; /*Variable/buffer file is stored in*/
        fseek(geometry_file, 0L, SEEK_END); 
        size = ftell(geometry_file);         /*calc the size needed*/
        rewind (geometry_file);
        buffer = (unsigned char *) malloc(size);
        
        if (buffer == NULL){
            printf ("ERROR Allocating Buffer\n");
            return 1;
        }
        else{

            fread(buffer, sizeof(unsigned char), size, geometry_file);
	    // Print Entire file
	    for (i=0; i<size; i++){
                printf("%02x", buffer[i]);
            }

        }
    
    fclose (geometry_file);
    free(buffer);
    return 0;
    }
}
