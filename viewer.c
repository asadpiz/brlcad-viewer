#include <stdio.h>
#include "common.h"
#include "raytrace.h"

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
    //return 0;
  struct db_i *dbip;
  int counter = 0;
  struct db_tree_state state = rt_initial_tree_state;

  if (argc < 2) {
    bu_exit(0, "need more, db.g obj\n");
  }

  dbip = db_open(argv[1], "r");
  if (dbip == NULL) {
    bu_exit(1, "Unable to open %s\n", argv[1]);
  }

  if (db_dirbuild(dbip) < 0) {
    db_close(dbip);
    bu_exit(1, "Unable to load %s\n", argv[1]);
  }


  printf ("DB_I structure contains: %d\n", dbip->dbi_magic);
  return 0;    


      }



}
