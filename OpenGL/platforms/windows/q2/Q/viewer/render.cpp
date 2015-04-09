#include "render.h"

#include <assert.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif // GL_BGR

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

#include "vec_math.h"
#include "MTracerFile.h"
#include "club.h"

#define GL_FIELD_OF_VIEW            45.0f
#define GL_NEAR_CLIPPING_PLANE      1.0f
#define GL_FAR_CLIPPING_PLANE       10000.f

#define GL_SCALE_AMOUNT             (1.0f/500.0f)
#define GL_TRANSLATE_X              0.0f
#define GL_TRANSLATE_Y              0.0f
#define GL_TRANSLATE_Z             -6.0f

#define GL_DEFAULT_LINE_WIDTH       2.0f

static GLUquadricObj        *g_pQuadric         = NULL;
static int                  g_width             = 0;
static int                  g_height            = 0;
static int                  g_idx = 0;

static struct tMTracerData_
{
    // This is an array of data repsenting the coordinates of the gofer's wrist (gripX, gripY and gripZ) and golf club head (headX, headY and HeadZ)
    struct Data {
        float gripX;
        float gripY;
        float gripZ;
        float headX;
        float headY;
        float headZ;
    } *data = nullptr;

    CMTracerFile  file;
    int           numPoints;
    mat4          transform;
    float         clubScale;
} g_MTracerData;

////////////////////////////////////////////////////////////////////////////////////////////////

int InitGL (RENDER_STATE *state)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);
    glLineWidth(GL_DEFAULT_LINE_WIDTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    {
        static GLfloat LightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
        static GLfloat LightDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
        static GLfloat LightPosition[] = {0.0f, 0.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_NORMALIZE);
    }

    g_pQuadric = gluNewQuadric();

    state->rotate_x     = 0.0f;
    state->rotate_y     = 0.0f;
    state->rotate_z     = 0.0f;
    state->translate_x  = 0.0f;
    state->translate_y  = 0.0f;
    state->translate_z  = 0.0f;
    state->scale        = 1.0f;
    state->clip_x_negative = 1000;
    state->clip_x_positive = 1000;
    state->clip_y_negative = 1000;
    state->clip_y_positive = 1000;
    state->auto_rotate   = 0;
    state->delta_auto_rotate = 0.4f;
    state->mtracer_scale = 25;
    state->mtracer_follow_club = 1;

    return 1;
}

void RenderGL(RENDER_STATE *state)
{
    int num_textures = 0;
    int num_points = 0;
    mat4 modelviewmatrix;

    float rotx = state->rotate_x;
    float roty = state->rotate_y;
    float rotz = state->rotate_z;
    float scale = state->scale;
    float xtrans = state->translate_x;
    float ytrans = state->translate_y;
    float ztrans = state->translate_z;

    state->rotate_x -= 360 * floor(state->rotate_x / 360);
    state->rotate_y -= 360 * floor(state->rotate_y / 360);
    state->rotate_z -= 360 * floor(state->rotate_z / 360);

    glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Scale, translate and rotate according to mouse control
    glTranslatef(GL_TRANSLATE_X, GL_TRANSLATE_Y, GL_TRANSLATE_Z);
    glTranslatef(xtrans, ytrans, ztrans);
    glRotatef(roty, 1, 0, 0);
    glRotatef(rotx, 0, 0, 1);
    glRotatef(rotz, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Draw the golf swing and club here
    if (g_MTracerData.data && g_MTracerData.file.isValidFile())
    {
        // Part 1: Draw the grip trajectory here
		glColor3f(1.0, 0.2, 0.2);
		glBegin(GL_LINES);
		for (int i = 0; i < g_MTracerData.numPoints-1; i++)
		{
			glVertex3f(g_MTracerData.data[i].gripX, g_MTracerData.data[i].gripY, g_MTracerData.data[i].gripZ);
			glVertex3f(g_MTracerData.data[i + 1].gripX, g_MTracerData.data[i + 1].gripY, g_MTracerData.data[i + 1].gripZ);
		}
		glEnd();

        // Part 1: Draw the club head trajectory here
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
		for (int i = 0; i < g_MTracerData.numPoints - 1; i++)
		{
			glVertex3f(g_MTracerData.data[i].headX, g_MTracerData.data[i].headY, g_MTracerData.data[i].headZ);
			glVertex3f(g_MTracerData.data[i + 1].headX, g_MTracerData.data[i + 1].headY, g_MTracerData.data[i + 1].headZ);
		}
		glEnd();

        // quats stores all the quaternions, which represents all the club head rotation
        CData quats;
        g_MTracerData.file.getQuaternion(quats);

        int idx = 0;
        vec4 q(&((float *)quats.GetData())[4 * idx]);
        mat4 rotation = fromQuaternion(q) * axisAngle(float(M_PI) / 2, vec3(1, 0, 0));
        rotation[12] = g_MTracerData.data[idx].headX;
        rotation[13] = g_MTracerData.data[idx].headY;
        rotation[14] = g_MTracerData.data[idx].headZ;

        glMultMatrixf(rotation.m);
        // Part 2: Draw the golf club mesh here. Club mesh and other info are stored in club.h and club.cpp
		glPushMatrix();
		glScalef(g_MTracerData.clubScale, g_MTracerData.clubScale, g_MTracerData.clubScale);
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < Club::numVerts; i++)
		{
			int index = 3 * i;
			glVertex3f(Club::verts[index], Club::verts[index + 1], Club::verts[index + 2]);
		}
		glEnd();
		glPopMatrix();
    }

    glFlush();
    glFinish();
}

void ReSizeSceneGL(int width, int height)
{
    float fAspect;

    if (height == 0) height = 1;
    fAspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(GL_FIELD_OF_VIEW, fAspect, GL_NEAR_CLIPPING_PLANE, GL_FAR_CLIPPING_PLANE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    g_width = width;
    g_height = height;
}

int LoadMTracerFile(const char* name)
{
    if(g_MTracerData.data) delete[] g_MTracerData.data;

    if (!g_MTracerData.file.OpenFile(name)) {
        return -1;
    }

    CData gripPos, headPos, quats;

    g_MTracerData.file.getGripPosition(gripPos);
    g_MTracerData.file.getHeadPosition(headPos);
    g_MTracerData.file.getQuaternion(quats);

    g_MTracerData.numPoints = (int)min(gripPos.GetSize()/12, min(headPos.GetSize()/12, quats.GetSize()/16));

    g_MTracerData.data = new tMTracerData_::Data[g_MTracerData.numPoints];

    for(int i = 0; i < g_MTracerData.numPoints; i++)
    {
        g_MTracerData.data[i].gripX = ((float*)gripPos.GetData())[i * 3 + 0];
        g_MTracerData.data[i].gripY = ((float*)gripPos.GetData())[i * 3 + 1];
        g_MTracerData.data[i].gripZ = ((float*)gripPos.GetData())[i * 3 + 2];
        g_MTracerData.data[i].headX = ((float*)headPos.GetData())[i * 3 + 0];
        g_MTracerData.data[i].headY = ((float*)headPos.GetData())[i * 3 + 1];
        g_MTracerData.data[i].headZ = ((float*)headPos.GetData())[i * 3 + 2];
    }

    g_MTracerData.clubScale = .0317268f * length( // Magic number to account for default club scale (will need to be changed if model changes)
        vec3(g_MTracerData.data[0].gripX, g_MTracerData.data[0].gripY, g_MTracerData.data[0].gripZ) -
        vec3(g_MTracerData.data[0].headX, g_MTracerData.data[0].headY, g_MTracerData.data[0].headZ));

    return 0;
}
