#include "common.h"
#include "raytrace.h"


int main(void) {
  struct db_i *dbip = db_open("moss.g", DB_OPEN_READONLY);
  struct directory **tops;
  db_dirbuild(dbip);
  int count = db_ls(dbip, DB_LS_TOPS, NULL, &tops);
  bu_log("found %d top level objects\n", count);
  while (count > 0) {
    bu_log("top path is %s\n", tops[count-1]->d_namep);
    count--;
  }
  if (tops)
    bu_free(tops, "free tops");
  return 0;
}

