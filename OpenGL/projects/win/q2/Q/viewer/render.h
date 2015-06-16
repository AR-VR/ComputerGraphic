#ifndef __RENDER_H__
#define __RENDER_H__

#include <Winsock2.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
    float                   rotate_x, rotate_y, rotate_z;
    float                   translate_x, translate_y, translate_z;
    int                     clip_x_positive, clip_x_negative, clip_y_positive, clip_y_negative;
    float                   scale;                                      // Scale factor of scene
    unsigned char           src_col_r, src_col_g, src_col_b;            // Color to be replaced
    unsigned char           dst_col_r, dst_col_g, dst_col_b;            // the new color
    int                     auto_rotate;                                // Boolean - on/off
    float                   delta_auto_rotate;                          // Control auto rotate speed
    int                     draw_mtracer_data;
    float                   mtracer_scale;
    int                     mtracer_follow_club;
} RENDER_STATE;

int  InitGL(RENDER_STATE *state);
void RenderGL(RENDER_STATE *state);
void ReSizeSceneGL(int width, int height);
int LoadMTracerFile(const char* name);
void StopPlayback();

#ifdef __cplusplus
}
#endif 

#endif // __RENDER_H__
