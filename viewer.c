#include <stdio.h>
#include <common.h>
#include <raytrace.h>
#include <unistd.h>
#include <string.h>

size_t read_binary (char* g_file, unsigned char **buffer){

    FILE* geometry_file = NULL;
    geometry_file  = fopen(g_file, "rb"); // Open .g File

    if (geometry_file == NULL){
    printf ("ERROR Opening File\n");
    return 1;
    }
    else{
        int i;
        size_t size; /*Size of File*/
        fseek(geometry_file, 0L, SEEK_END);
        size = ftell(geometry_file);         /*calc the size needed*/
        rewind (geometry_file);
        *buffer = (unsigned char *) malloc(size);

        if (*buffer == NULL){
            printf ("ERROR Allocating Buffer\n");
            return 1;
        }
        else{
            fread(*buffer, sizeof(unsigned char), size, geometry_file);
        }
        fclose (geometry_file);
        return size;
    }
}
                                                                                                   
int main (int argc, char* argv[]){
    struct db_i *dbip;   
    int i,j, g_file;
    size_t size;
    unsigned char *buffer;
    unsigned char *g_bytes;
    size = read_binary(argv[0], &buffer);
    char template[] = "fileXXXXXX";
   

    if (size == 1 ){
	bu_exit(1, "Unable to load g file\n");
    }
    else{
        for (i=0; i<size; i++){
		if ( buffer[i] == 118){

			if (i+7 < size ){
				/* Comparing with decimal of  76 01 00 00 00 00 01 35 */
				if (buffer[i+1] == 1 && buffer[i+2] == 0 && buffer[i+3] == 0 && buffer[i+4] == 0 && buffer[i+5] == 0 && buffer [i+6] == 1 && buffer[i+7] == 53){
				g_bytes = (unsigned char *) malloc(size-i);
				memcpy(g_bytes, buffer + i, size-i);
				free(buffer);
				//db_open_inmem();
				//dbip->dbi_mf->buf = * g_bytes;
				//dbip->dbi_eof = size - i;
				// Printing HEX of g file
				g_file = mkstemp (template);
				write (g_file, g_bytes,size-i);

/*				for (j=0; j< size-i; j++){
				 	printf("%02x", g_bytes[j]);
				}
*/
				break;
				}

			}
			//printf("%02x\n", buffer[i]);
            	}
    }
}

    dbip = db_open(template, "r");
    if (db_dirbuild(dbip) < 0) {
      db_close(dbip);
      bu_exit(1, "Unable to load %s\n", argv[0]);
    }

    // Print struct dbip: http://brlcad.org/docs/doxygen-r64112/d2/d66/structdb__i.xhtml

    bu_log ("DBIP Version %s\n", dbip->dbi_title);
    db_close(dbip);
    free(g_bytes);
    unlink(template);
    return 0;
}

