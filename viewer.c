#include <stdio.h>
#include <common.h>
#include <raytrace.h>
#include <unistd.h>
#include <string.h>
#include <ged.h>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "linmath.h"


static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int draw_window (struct ged *gedp) {

    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, gedp->ged_wdbp->dbip->dbi_title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
  return 0;
}




int plot_regions (struct db_tree_state *tsp, struct db_full_path *pathp, const struct rt_comb_internal *combp, struct ged *gedp)
{
    point_t rpp_min, rpp_max;
    point_t obj_min, obj_max;
    struct directory *dp;
    dp = DB_FULL_PATH_CUR_DIR(pathp);
    char *region_path = db_path_to_string(pathp);
    //printf (" Region name is %s\n", dp->d_namep);
    //printf (" test name is %s\n", test);
    //if (ged_get_obj_bounds(gedp, 1, (const char **)&(dp->d_namep), 0, obj_min, obj_max) == GED_ERROR) {
    if (ged_get_obj_bounds(gedp, 1, (const char **)&region_path, 0, obj_min, obj_max) == GED_ERROR) {
        return GED_ERROR;
    }
    VMINMAX(rpp_min, rpp_max, (double *)obj_min);
    VMINMAX(rpp_min, rpp_max, (double *)obj_max);
    bu_log ("Coordinates Are min {%f %f %f} max {%f %f %f}\n", rpp_min[0], rpp_min[1], rpp_min[2], rpp_max[0], rpp_max[1], rpp_max[2]);
    return 0;
}
size_t read_binary (char* g_file, unsigned char **buffer){

    FILE* geometry_file = NULL;
    geometry_file  = fopen(g_file, "rb"); // Open .g File
    if (geometry_file == NULL){
        //printf ("ERROR Opening File\n");
        bu_exit(1,"ERROR Opening Database File\n");
        //return 1;
    }
    else{
        int i;
        size_t size; /*Size of File*/
        fseek(geometry_file, 0L, SEEK_END);
        size = ftell(geometry_file);         /*calc the size needed*/
        rewind (geometry_file);
        *buffer = (unsigned char *) malloc(size);
        if (*buffer == NULL){
            //printf ("ERROR Allocating Buffer\n");
            bu_exit(1, "ERROR Allocating Buffer\n");
            //return 1;
        }
        else{
            fread(*buffer, sizeof(unsigned char), size, geometry_file);
        }
        fclose (geometry_file);
        return size;
    }
}


int main (int argc, char* argv[])
{

    struct ged *gedp = NULL;
    struct directory **tops;
    int i, g_file;
    int counter = 0;
    size_t size;
    unsigned char *buffer;
    unsigned char *g_bytes;
    char template[] = "fileXXXXXX";
    struct db_tree_state state = rt_initial_tree_state;

    size = read_binary(argv[0], &buffer);
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
                    /* db_open_inmem();
                    dbip->dbi_mf->buf = * g_bytes;
                    dbip->dbi_eof = size - i;
                    */
                    /* Printing HEX of g file */
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

    gedp = ged_open("db", template, 1);
    if (!gedp) {
        bu_exit(8, "ERROR: Unable to open [%s] for reading objects\n", argv[0]);
    }

    bu_log ("Database Title Is: %s\n", gedp->ged_wdbp->dbip->dbi_title);
    /*  Get list of top level objects */
    db_update_nref(gedp->ged_wdbp->dbip, &rt_uniresource);
    int count = db_ls(gedp->ged_wdbp->dbip, DB_LS_TOPS, NULL, &tops);
    bu_log("found %d top level objects\n", count);


/*  while (count > 0) {

    bu_log("top path is %s\n", tops[count-1]->d_namep);
    count--;
  }
  if (tops)
    bu_free(tops, "free tops");
*/


// Call db_walk_tree on each of the objects

    state.ts_dbip = gedp->ged_wdbp->dbip;
    state.ts_resp = &rt_uniresource;
    rt_init_resource(&rt_uniresource, 0, NULL );
    int k =0;

    while (count > 0) {
        const char* array[] = {tops[count-1]->d_namep};
        db_walk_tree(gedp->ged_wdbp->dbip, 1, (const char **)array, 1, &state, plot_regions, NULL, NULL, gedp);
        bu_log("top path is %s\n", tops[count-1]->d_namep);
        count--;
    }
    draw_window (gedp);
    if (tops)
        bu_free(tops, "free tops");
    ged_close(gedp);
    free(g_bytes);
    unlink(template);
    return 0;
}

