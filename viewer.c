#include <stdio.h>
#include <common.h>
#include <raytrace.h>
#include <unistd.h>
#include <string.h>
/*
int incr_region(struct db_tree_state *tsp, struct db_full_path *pathp, const struct rt_comb_internal *combp, void *data){
  int *counter = (int*)data;
  (*counter)++;
  return 0;
}

*/

/*
size_t
db_ls_test(const struct db_i *dbip, int flags, const char *pattern, struct directory ***dpv)
{
    size_t i;
    size_t objcount = 0;
    struct directory *dp;

    RT_CK_DBI(dbip);
    db_update_nref(dbip, &rt_uniresource);
    for (i = 0; i < RT_DBNHASH; i++) {
        for (dp = dbip->dbi_Head[i]; dp != RT_DIR_NULL; dp = dp->d_forw) {
		if (dp->d_nref == 0){
		printf ("Yo found the TLO %s\n", dp->d_namep);
            objcount += dp_eval_flags(dp, flags);}
        }
    }
    if (objcount > 0) {
        (*dpv) = (struct directory **)bu_malloc(sizeof(struct directory *) * (objcount + 1), "directory pointer array");
        objcount = 0;
        for (i = 0; i < RT_DBNHASH; i++) {
            for (dp = dbip->dbi_Head[i]; dp != RT_DIR_NULL; dp = dp->d_forw) {
                if (dp_eval_flags(dp,flags)) {
                    if (!pattern || !bu_fnmatch(pattern, dp->d_namep, 0) ) {
                        if (dp->d_nref == 0){
			printf ("Yo found the TLO in 2nd %s\n", dp->d_namep);
			(*dpv)[objcount] = dp;
                        objcount++;
			}
                    }
                }
            }
        }
        (*dpv)[objcount] = NULL;
    }
    return objcount;
}


*/


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
    int counter = 0;
    struct db_tree_state state = rt_initial_tree_state;
//    const char* array[] = {"tank"};
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

/*  if (db_lookup(dbip, *array, 1) == NULL) {
    db_close(dbip);
    bu_exit(1, "Unable to find %s\n", *array);
  }
*/

    // Print struct dbip: http://brlcad.org/docs/doxygen-r64112/d2/d66/structdb__i.xhtml

    bu_log ("Database Title Is: %s\n", dbip->dbi_title);

/*  Get list of top level objects     */
int obj_cnt = 0;
struct directory *dp;
    db_update_nref(dbip, &rt_uniresource);

    if (db_version(dbip) < 5) {
        for (i = 0; i < RT_DBNHASH; i++)
            for (dp = dbip->dbi_Head[i]; dp != RT_DIR_NULL; dp = dp->d_forw) {
                if (dp->d_nref == 0 && !(dp->d_flags & RT_DIR_HIDDEN))
		    printf ("Found the TLO %s\n", dp->d_namep);
                    obj_cnt++;
            }
    } 

    else {
        for (i = 0; i < RT_DBNHASH; i++)
            for (dp = dbip->dbi_Head[i]; dp != RT_DIR_NULL; dp = dp->d_forw) {

                if (dp->d_nref == 0 && !(dp->d_flags & RT_DIR_HIDDEN)) {
                     obj_cnt++;
			printf ("Found the TLO in else %s\n", dp->d_namep);
                }

                else {
			continue;
                }
            }
    }
    printf ("TOP LEVEL OBJECTS %d\n", obj_cnt);



/* Call db_walk_tree on each of the objects
  
  state.ts_dbip = dbip;
  state.ts_resp = &rt_uniresource;
  rt_init_resource(&rt_uniresource, 0, NULL );

  db_walk_tree(dbip, 1, (const char **)array, 1, &state, incr_region, NULL, NULL, &counter);

  bu_log("counter is %d\n", counter);
*/
    db_close(dbip);
    free(g_bytes);
    unlink(template);
    return 0;
}

