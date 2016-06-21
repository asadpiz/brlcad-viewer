# brlcad-viewer
A geometry viewer application that generates a standalone double-clickable file that views a given model.

## Compilation Instructions:

Adjust the command according to the location of your brlcad installation. This command is for default and release 7.24.4.

```bash
gcc -o viewer viewer.c -L/usr/brlcad/rel-7.24.4/lib -lrt -lbu -I/usr/brlcad/rel-7.24.4/include -I/usr/brlcad/rel-7.24.4/include/brlcad

./viewer.c moss.g
```
## Development Plan Summary:

* ~~Read a ".g" file and identify its header bytes~~ **[DONE]**
* Pass on the opened file to librt
* Call libdm to draw the geometry 

More here: [BRLCAD Viewer](http://brlcad.org/wiki/Geometry_Viewer_Application_for_BRL-CAD)

---
[Markdown Cheatsheet](//github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
