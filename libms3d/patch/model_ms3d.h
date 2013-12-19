/*****************************************************************************
 * Copyright (C) 2009 The Forget3D Project by Martin Foo (i25ffz@hotmail.com)
 * ALL RIGHTS RESERVED
 *
 * License I
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITHOUT a fee is granted under following requirements:
 * - You make no money using this software.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * Licence II
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITH a fee is granted under following requirements:
 * - As soon as you make money using this software, you have to pay a
 *   licence fee. Until this point of time, you can use this software
 *   without a fee.
 *       Please contact Martin Foo (i25ffz@hotmail.com) for further details.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHORS
 * BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 * INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE, SAVINGS OR
 * REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR NOT THE AUTHORS HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *****************************************************************************/

#ifndef F3D_MODELMS3D_H_
#define F3D_MODELMS3D_H_

#include "model.h"
#include "mathlib.h"
#include "utils.h"

#define MS3D_MAX_VERTICES   65534
#define MS3D_MAX_TRIANGLES  65534
#define MS3D_MAX_GROUPS     255
#define MS3D_MAX_MATERIALS  128
#define MS3D_MAX_JOINTS     128
#define MS3D_SELECTED       1
#define MS3D_HIDDEN         2
#define MS3D_SELECTED2      4
#define MS3D_DIRTY          8
#define MS3D_NAME_SIZE      32
#define MS3D_PATH_SIZE      128

namespace F3D {

// force one byte alignment
#ifdef ANDROID
#define F3D_PACKED __attribute__((packed))
#elif (defined(WIN32) || defined(_WIN32_WCE))
#ifndef F3D_PACKED
#pragma pack( push, 1 )
//#pragma pack( 1 )
#define F3D_PACKED
#endif
#endif

    /**
     * MS3D model structs.
     */
    typedef struct {
        char    id[10];     // always "MS3D000000"
        int     version;    // 4
    } F3D_PACKED ms3d_header_t;

    typedef struct {
        GLubyte    flags;      // SELECTED | SELECTED2 | HIDDEN
        float   vertex[3];
        char    boneId;     // -1 = no bone
        GLubyte    referenceCount;
    } F3D_PACKED ms3d_vertex_t;

    typedef struct {
        GLushort  flags;                  // SELECTED | SELECTED2 | HIDDEN
        GLushort  vertexIndices[3];
        float   vertexNormals[3][3];
        float   s[3];
        float   t[3];
        GLubyte    smoothingGroup;         // 1 - 32
        GLubyte    groupIndex;
    } F3D_PACKED ms3d_triangle_t;

    typedef struct {
        GLubyte    flags;              // SELECTED | HIDDEN
        char    name[MS3D_NAME_SIZE];
        GLushort  numTriangles;
        GLushort  *triangleIndices;   // the groups group the triangles
        char    materialIndex;      // -1 = no material
    } F3D_PACKED ms3d_group_t;

    typedef struct {
        char    name[MS3D_NAME_SIZE];
        float   ambient[4];
        float   diffuse[4];
        float   specular[4];
        float   emissive[4];
        float   shininess;                  // 0.0f - 128.0f
        float   transparency;               // 0.0f - 1.0f
        char    mode;                       // 0, 1, 2 is unused now
        char    texture[MS3D_PATH_SIZE];    // texture.bmp
        char    alphamap[128];              // alpha.bmp
    } F3D_PACKED ms3d_material_t;

    typedef struct {
        float   time;           // time in seconds
        float   rotation[3];    // x, y, z angles
    } F3D_PACKED ms3d_keyframe_rot_t;

    typedef struct {
        float   time;           // time in seconds
        float   position[3];    // local position
    } F3D_PACKED ms3d_keyframe_pos_t;

    typedef struct {
        GLubyte    flags;              // SELECTED | DIRTY
        char    name[32];
        char    parentName[32];
        float   rotation[3];        // local reference matrix
        float   position[3];
        GLushort  numKeyFramesRot;
        GLushort  numKeyFramesTrans;
    } F3D_PACKED ms3d_joint_header_t;

    typedef struct {
        ms3d_joint_header_t header;
        ms3d_keyframe_rot_t *keyFramesRot;      // local animation matrices
        ms3d_keyframe_pos_t *keyFramesTrans;    // local animation matrices
        //special variables
        int                 parentJointIndex;   // parent joint index, find by header's parentName
        Matrix              *absMatrix;         // absolute martrix
        Matrix              *relMatrix;         // relative martrix
        Matrix              *finMatrix;         // final martrix
    } F3D_PACKED ms3d_joint_t;

#if (defined(WIN32) || defined(_WIN32_WCE))
#pragma pack( pop )
#endif
#undef F3D_PACKED

    /**
     * ModelMS3D class for all games using F3D.
     */
    class ModelMS3D : public Model {
    private:
        ms3d_header_t   m_header;
        GLushort        m_verticesCount;
        ms3d_vertex_t   *m_vertices;
        GLushort        m_trianglesCount;
        ms3d_triangle_t *m_triangles;
        GLushort        m_groupsCount;
        ms3d_group_t    *m_groups;
        GLushort        m_materialsCount;
        ms3d_material_t *m_materials;
        float           m_animationFPS;
        float           m_currentTime;
        int             m_totalFrames;
        GLushort        m_jointsCount;
        ms3d_joint_t    *m_joints;
        //frame variables
        int             m_frameCount;
        int             m_frameIdx;
        GLboolean       m_isPrepared;
        //private functions
        void prepareFrame();
        void setupJoints();
    public:
        /**
        * Constructor
        */
        ModelMS3D();

        /**
         * Destructor
         */
        virtual ~ModelMS3D();

        bool loadModel(const char *filename, GLboolean is_absPath = GL_FALSE);
    };

}

#endif /* F3D_MODELMS3D_H_ */
