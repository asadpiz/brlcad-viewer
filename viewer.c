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

    if (argc < 2) {
      bu_exit(0, "need more, db.g obj\n");
    }
    dbip = db_open(argv[1], "r");

    if (dbip == NULL) {
      bu_exit(1, "Unable to open %s\n", argv[1]);
    }

/*    if (db_dirbuild(dbip) < 0) {
      db_close(dbip);
      bu_exit(1, "Unable to load %s\n", argv[1]);
    }
*/
    // Print struct dbip: http://brlcad.org/docs/doxygen-r64112/d2/d66/structdb__i.xhtml

    bu_log ("DBIP Version %d\n", dbip->dbi_mf);
    db_close(dbip);

/*  Read .g File into Binary                    */
    unsigned char *buffer;
    size = read_binary(argv[1], &buffer);
    if (size == 1 ){
        bu_exit(1, "Unable to load %s\n", argv[1]);
    }
    else{
            for (i=0; i<size; i++){
                 printf("%02x", buffer[i]);
            }                         
        printf ("\n") ;
        free(buffer);
        return 0;    
    }
}

