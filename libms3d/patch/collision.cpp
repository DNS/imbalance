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

#include "collision.h"

namespace F3D {
    /**
     * BoundingBox class for all games using F3D.
     */

    BoundingBox::BoundingBox() {
    #ifdef DEBUG
        printf("BoundingBox constructor...\n");
    #endif

        m_minEdge = (Vec3f*) malloc(sizeof(Vec3f));
        m_maxEdge = (Vec3f*) malloc(sizeof(Vec3f));
        //set default position to 0.0
        m_position = (Vec3f*) malloc(sizeof(Vec3f));
        m_position->x = m_position->y = m_position->z = 0.0f;
        //set default scale to 1.0
        m_scale = (Vec3f*) malloc(sizeof(Vec3f));
        m_scale->x = m_scale->y = m_scale->z = 1.0f;

        m_center = (Vec3f*) malloc(sizeof(Vec3f));
    }

    BoundingBox::~BoundingBox() {
    #ifdef DEBUG
        printf("BoundingBox destructor...\n");
    #endif

        FREEANDNULL(m_minEdge);
        FREEANDNULL(m_maxEdge);
        FREEANDNULL(m_center);
    }

    void BoundingBox::setMinEdge(Vec3f vec3f) {
        m_minEdge->x = vec3f.x;
        m_minEdge->y = vec3f.y;
        m_minEdge->z = vec3f.z;
    }

    Vec3f* BoundingBox::getMinEdge() {
        return m_minEdge;
    }

    void BoundingBox::setMaxEdge(Vec3f vec3f) {
        m_maxEdge->x = vec3f.x;
        m_maxEdge->y = vec3f.y;
        m_maxEdge->z = vec3f.z;
    }

    Vec3f* BoundingBox::getMaxEdge() {
        return m_maxEdge;
    }

    void BoundingBox::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        m_position->x = x;
        m_position->y = y;
        m_position->z = z;
    }

    Vec3f* BoundingBox::getPosition() {
        return m_position;
    }

    void BoundingBox::setScale(GLfloat x, GLfloat y, GLfloat z) {
        m_scale->x = x;
        m_scale->y = y;
        m_scale->z = z;
    }

    Vec3f* BoundingBox::getScale() {
        return m_scale;
    }

    Vec3f *BoundingBox::getCenter() {
        GLfloat x0_min = m_minEdge->x * m_scale->x + m_position->x;
        GLfloat y0_min = m_minEdge->y * m_scale->y + m_position->y;
        GLfloat z0_min = m_minEdge->z * m_scale->z + m_position->z;
        GLfloat x0_max = m_maxEdge->x * m_scale->x + m_position->x;
        GLfloat y0_max = m_maxEdge->y * m_scale->y + m_position->y;
        GLfloat z0_max = m_maxEdge->z * m_scale->z + m_position->z;

        m_center->x = (x0_min + x0_max) / 2;
        m_center->y = (y0_min + y0_max) / 2;
        m_center->z = (z0_min + z0_max) / 2;

        return m_center;
    }

    GLboolean BoundingBox::isCollided(BoundingBox *other) {
        GLfloat x0_min = m_minEdge->x * m_scale->x + m_position->x;
        GLfloat y0_min = m_minEdge->y * m_scale->y + m_position->y;
        GLfloat z0_min = m_minEdge->z * m_scale->z + m_position->z;
        GLfloat x0_max = m_maxEdge->x * m_scale->x + m_position->x;
        GLfloat y0_max = m_maxEdge->y * m_scale->y + m_position->y;
        GLfloat z0_max = m_maxEdge->z * m_scale->z + m_position->z;
        // other bounding box values
        GLfloat x1_min = other->getMinEdge()->x * other->getScale()->x + other->getPosition()->x;
        GLfloat y1_min = other->getMinEdge()->y * other->getScale()->y + other->getPosition()->y;
        GLfloat z1_min = other->getMinEdge()->z * other->getScale()->z + other->getPosition()->z;
        GLfloat x1_max = other->getMaxEdge()->x * other->getScale()->x + other->getPosition()->x;
        GLfloat y1_max = other->getMaxEdge()->y * other->getScale()->y + other->getPosition()->y;
        GLfloat z1_max = other->getMaxEdge()->z * other->getScale()->z + other->getPosition()->z;

        if (x0_min > x1_max) return GL_FALSE;
        if (x0_max < x1_min) return GL_FALSE;
        if (y0_min > y1_max) return GL_FALSE;
        if (y0_max < y1_min) return GL_FALSE;
        if (z0_min > z1_max) return GL_FALSE;
        if (z0_max < z1_min) return GL_FALSE;

        return GL_TRUE;
    }

}
