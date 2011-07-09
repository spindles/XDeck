#include <QtCore/QCoreApplication>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <AR/arMulti.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "object.h"

const char      *vconf = "v4l2src device=/dev/video1 ! ffmpegcolorspace ! video/x-raw-rgb,width=640,height=480 ! identity name=artoolkit ! fakesink";

const char      *model_name = "ARIface/Data/object_data2";
ObjectData_T    *object;
int             objectnum;

int             xsize, ysize;
int             thresh = 100;
int             count = 0;
int             selected = 0;

int             mode = 1;
const char      *cparam_name    = "ARIface/Data/camera_para.dat";
ARParam         cparam;

static void   init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static int    draw( ObjectData_T *object, int objectnum );
static int    draw_object( int obj_id, double gl_para[16] );

void          draw_sphere(double gl_para[16]);
void          draw_cube(double gl_para[16]);
void          draw_question_mark();

static void   keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec) x=%d y=%d\n", (double)count/arUtilTimer(),x, y);
        cleanup();
        exit(0);
    }
    if(key == '1'){
        mode=1;
    }
    if(key == '2'){
        mode=2;
    }
}

/* main loop */
static void mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             i,j,k;

    /* grab a video frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }

    if( count == 0 ) arUtilTimerReset();
    count++;

    /*draw the video*/
    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glLineWidth(6.0);

    /* detect the markers in the video frame */
    if(arDetectMarker(dataPtr, thresh,
        &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }
    /*for( i = 0; i < marker_num; i++ ) {
            argDrawSquare(marker_info[i].vertex,0,0);
    }*/

    /* check for known patterns */
    for( i = 0; i < objectnum; i++ ) {
        k = -1;
        for( j = 0; j < marker_num; j++ ) {
            if( object[i].id == marker_info[j].id) {
                /* you've found a pattern */
                //printf("Found pattern: %d ",patt_id);
                //glColor3f( 0.0, 1.0, 0.0 );
                //argDrawSquare(marker_info[j].vertex,0,0);

                if( k == -1 ) k = j;
                else /* make sure you have the best pattern (highest confidence factor) */
                if( marker_info[k].cf < marker_info[j].cf ) k = j;
            }
        }
        if( k == -1 ) {
            object[i].visible = 0;
            continue;
        }

        /* calculate the transform for each marker */
        if( object[i].visible == 0 ) {
            arGetTransMat(&marker_info[k],
            object[i].marker_center, object[i].marker_width,
            object[i].trans);
        }
        else {
            arGetTransMatCont(&marker_info[k], object[i].trans,
            object[i].marker_center, object[i].marker_width,
            object[i].trans);
        }
        object[i].visible = 1;
    }

    arVideoCapNext();

    /* draw the AR graphics */
    draw( object, objectnum );

    /*swap the graphics buffers*/
    argSwapBuffers();
}

static void init( void )
{
    ARParam  wparam;

    /* open the video path */
    if( arVideoOpen( const_cast<char *>(vconf) ) < 0 ) exit(0);

    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    /* load in the object data - trained markers and associated bitmap files */
    if( (object=read_ObjData(const_cast<char *>(model_name), &objectnum)) == NULL ) exit(0);
    printf("Objectfile num = %d\n", objectnum);

    /* open the graphics window */
    argInit( &cparam, 2.0, 0, 0, 0, 0 );
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

static int draw( ObjectData_T *object, int objectnum )
{
    int     i;
    double  gl_para[16];

    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);

    /* calculate the viewing parameters - gl_para */
    for( i = 0; i < objectnum; i++ ) {
        if( object[i].visible == 0 ) continue;
        argConvGlpara(object[i].trans, gl_para);
        draw_object( object[i].id, gl_para);
    }

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    return(0);
}

static int  draw_object( int obj_id, double gl_para[16] )
{
    if(obj_id == 0){
        if(mode==1){
           draw_sphere(gl_para);
        }
        if(mode==2){
           draw_cube(gl_para);
        }
    }
    if (obj_id == 1)
    {
        draw_cube(gl_para);
    }
    argDrawMode2D();

    return 0;

}

void draw_question_mark()
{

}

void draw_sphere(double gl_para[16])
{
    GLfloat   mat_ambient[]     = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash[]	= {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

        /* set the material */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);

    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
    if(object[0].selected==1)
    {
    }
    else
    {
    }
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    /* draw a sphere */
    glTranslatef( 0.0, 0.0, 30.0 );
    glutSolidSphere(30,12,6);
}

void draw_cube(double gl_para[16])
{
    GLfloat   mat_ambient[]     = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash[]	= {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

        /* set the material */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);

    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
    if(object[0].selected==1)
    {
    }
    else
    {
    }
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    /* draw a sphere */
    glTranslatef( 0.0, 0.0, 30.0 );
    glutSolidCube(60);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    init();

    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
    return (0);
}
