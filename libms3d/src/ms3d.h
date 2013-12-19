
#ifndef _MS3D_H
#define _MS3D_H

#ifdef __cplusplus
extern "C" {
#endif

#define TEXTURE_PATH "./gfx/"
	
	
	
//
// max values
//
#define MAX_VERTICES    65534
#define MAX_TRIANGLES   65534
#define MAX_GROUPS      255
#define MAX_MATERIALS   128
#define MAX_JOINTS      128
#define MAX_TEXTURE_FILENAME_SIZE	128



//
// flags
//
#define SELECTED        1
#define HIDDEN          2
#define SELECTED2       4
#define DIRTY           8
#define ISKEY			16
#define NEWLYCREATED	32
#define MARKED			64


// material

#define SPHEREMAP		0x80
#define HASALPHA		0x40
#define COMBINEALPHA    0x20

#define TRANSPARENCY_MODE_SIMPLE				0
#define TRANSPARENCY_MODE_DEPTHSORTEDTRIANGLES	1
#define TRANSPARENCY_MODE_ALPHAREF				2

#include <GL/gl.h>

#include "typedefs.h"
//#include "matrix.h"
//#include "timer.h"

// force one byte alignment
#include <pshpack1.h>


typedef struct {
    char	id[10];                                     // always "MS3D000000"
    dword	version;                                    // 4
} ms3d_header_t;


typedef struct {
	char boneIds[3];									// index of joint or -1, if -1, then that weight is ignored, since subVersion 1
	byte weights[3];									// vertex weight ranging from 0 - 100, last weight is computed by 1.0 - sum(all weights), since subVersion 1
	// weight[0] is the weight for boneId in ms3d_vertex_t
	// weight[1] is the weight for boneIds[0]
	// weight[2] is the weight for boneIds[1]
	// 1.0f - weight[0] - weight[1] - weight[2] is the weight for boneIds[2]
	unsigned int extra;				// (extra since subVersion 2), (NULL if subVersion 2)
} ms3d_vertex_ex_t;



typedef struct {
	byte    flags;                                      // SELECTED | SELECTED2 | HIDDEN
	float   vertex[3];                                  //
	char    boneId;                                     // -1 = no bone   ,, for skeletal animation 
	byte    referenceCount;
	
	// VERTEX EXTRA
	char boneIds[3];
	byte weights[3];
	unsigned int extra;
	float renderColor[3];
} ms3d_vertex_t;




typedef struct {
    word    flags;                                      // SELECTED | SELECTED2 | HIDDEN
    word    vertexIndices[3];                           //
    float   vertexNormals[3][3];                        //
    float   s[3];                                       //
    float   t[3];                                       //
    byte    smoothingGroup;                             // 1 - 32
    byte    groupIndex;                                 //
} ms3d_triangle_t;

typedef struct {
    byte            flags;                              // SELECTED | HIDDEN
    char            name[32];                           //
    word            numtriangles;                       //
    word            *triangleIndices;
    //word            triangleIndices[numtriangles];      // the groups group the triangles
    char            materialIndex;                      // -1 = no material
} ms3d_group_t;

typedef struct {
    char            name[32];                           //
    float           ambient[4];                         //
    float           diffuse[4];                         //
    float           specular[4];                        //
    float           emissive[4];                        //
    float           shininess;                          // 0.0f - 128.0f
    float           transparency;                       // 0.0f - 1.0f
    char            mode;                               // 0, 1, 2 is unused now
    char            texture[128];                        // texture.bmp
    char            alphamap[128];                       // alpha.bmp
	
    // used for rendering
    GLuint		id;				// opengl texture handle
} ms3d_material_t;

// 16 bytes
typedef struct {
	float		time;                               // time in seconds
	float		rotation[3];                        // x, y, z angles
} ms3d_keyframe_rot_t;

// 16 bytes
typedef struct {
    float		time;                               // time in seconds
    float		position[3];                        // local position
} ms3d_keyframe_pos_t;

typedef struct {
	float		tangentIn[3];
	float		tangentOut[3];
} ms3d_tangent_t;

typedef struct {
	byte		flags;				// SELECTED | DIRTY
	char		name[32];			//
	char		parentName[32];		//
	float		rotation[3];			// local reference matrix
	float		position[3];
	
	word		numKeyFramesRot;
	word		numKeyFramesTrans;
	
	ms3d_keyframe_rot_t	*keyFramesRot;		// Rotation
	ms3d_keyframe_pos_t	*keyFramesTrans;	// Position
	
	
	// used for rendering
	Matrix	*m_absolute;
	Matrix	*m_relative;
	
	word		currentkeyFramesRot;
	word		currentkeyFramesTrans;
	Matrix	*m_final;
	
	dword	m_parent;
	
	/*
	ms3d_tangent_t	*tangents;		// animation
	int		parentIndex;
	float		matLocalSkeleton[3][4];
	float		matGlobalSkeleton[3][4];
	float		matLocal[3][4];
	float		matGlobal[3][4];
	*/
	
	
} ms3d_joint_t;

typedef struct {
	int index;											// index of group, material or joint
	int commentLength;									// length of comment (terminating '\0' is not saved), "MC" has comment length of 2 (not 3)
	char *comment;						// comment
	//char comment[commentLength];						// comment
} ms3d_comment_t;



typedef struct {
	float color[3];	// joint color, since subVersion == 1
} ms3d_joint_ex_t;

typedef struct {
	float jointSize;	// joint size, since subVersion == 1
	int transparencyMode; // 0 = simple, 1 = depth buffered with alpha ref, 2 = depth sorted triangles, since subVersion == 1
	float alphaRef; // alpha reference value for transparencyMode = 1, since subVersion == 1
} ms3d_model_ex_t;

typedef struct _MS3D {
	ms3d_header_t		header;
	
	word				nNumVertices;	// 2 bytes
	ms3d_vertex_t		*vertices;
	
	word				nNumTriangles;	// 2 bytes
	ms3d_triangle_t		*triangles;
	
	word				nNumGroups;	// 2 bytes
	ms3d_group_t		*groups;		// or MESH
	
	word				nNumMaterials;	// 2 bytes
	ms3d_material_t		*materials;			
	
	float				fAnimationFPS;	// 4 bytes
	float				fCurrentTime;	// 4 bytes , in milliseconds
	dword			iTotalFrames;	// 4 bytes
	
	word				nNumJoints;	// 2 bytes
	ms3d_joint_t		*joints;
	
	int				comments_subVersion; // subVersion is = 1, 4 bytes
	
	unsigned int		nNumGroupComments; // 4 bytes
	ms3d_comment_t		comment;
	long				nNumMaterialComments; // 4 bytes
	long				nNumJointComments; // 4 bytes
	long				nHasModelComment; // 4 bytes
	int				subVersion;		// subVersion is = 2, 4 bytes
	
	ms3d_vertex_ex_t	vertex_ex;
	
	//int				subVersion;		// subVersion is = 2, 4 bytes
	
	ms3d_joint_ex_t		joint_ex;
	
	//int subVersion;		// subVersion is = 1, 4 bytes
	
	ms3d_model_ex_t		model_ex;
	
	
	/* FOR RENDER PURPOSE */
	double			m_totalTime;
	BOOL				m_looping;
	struct _Timer		*m_pTimer;		/* Timer variable */
	
	
	
	/* INTERNAL METHODS */
	void (*draw)();
	BOOL (*loadFromFile)();
	void (*destroy)();
	
	/* INTERNAL ATTRIBUTES */
	char filename[256];
	
	/* TEST ATTRIBUTES */
	//GLuint texture[1];
} MS3D;



extern MS3D *new_MS3D (char *);

#include <poppack.h>


#ifdef __cplusplus
}
#endif 




#endif






