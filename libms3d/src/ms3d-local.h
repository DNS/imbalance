
/* Local functions */

#ifndef _MS3D_LOCAL_H
#define _MS3D_LOCAL_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
typedef enum {
	FALSE = 0,
	TRUE = 1
} BOOL;
#endif

static void draw (MS3D *);
static BOOL loadFromFile (MS3D *, char *);
static void destroy (MS3D *);
static void BindMaterial (MS3D *, int, int);
static void LoadGLTextures (char *, GLuint *);
static void reloadTextures (MS3D *);



#ifdef __cplusplus
}
#endif

#endif

