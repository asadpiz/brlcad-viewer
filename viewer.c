#include <stdio.h>
#include "common.h"
#include "raytrace.h"
#include "unistd.h"

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
    int i;
    size_t size;
    unsigned char *buffer;
    size = read_binary(argv[0], &buffer);
    int g_header_start = 0;
    if (size == 1 ){
	bu_exit(1, "Unable to load g file\n");
    }
    else{
        for (i=0; i<size; i++){
		if (buffer[i] == 76){
			if ( i+7 < size){
                           if (buffer [i+1]== 01 && buffer [i+2]== 00 && buffer [i+3]== 00 && buffer [i+4]== 00 && buffer [i+5]== 00 && buffer [i+6]== 01 && buffer [i+7]== 35){

		g_header_start = i;
//		db_open_inmem();
//		dbip->dbi_mf->buf = &buffer[i]; 
//		dbip->dbi_eof = size-i;
		break;
		}
			}
		}
                //printf("%02x", buffer[i]);
            }
	printf ("start of g header is %d",i);
        printf ("\n") ;
//        free(buffer);
        //return 0;
    }


/*    if (argc < 2) {
      bu_exit(0, "need more, db.g obj\n");
    }
    dbip = db_open(argv[1], "r");

    if (dbip == NULL) {
      bu_exit(1, "Unable to open %s\n", argv[1]);
    }

*/
/*    if (db_dirbuild(dbip) < 0) {
      db_close(dbip);
      bu_exit(1, "Unable to load %s\n", argv[0]);
    }

    // Print struct dbip: http://brlcad.org/docs/doxygen-r64112/d2/d66/structdb__i.xhtml

    bu_log ("DBIP Version %d\n", dbip->dbi_version);
    db_close(dbip);
*/
    free(buffer);
    return 0;
}

