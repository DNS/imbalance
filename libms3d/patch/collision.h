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

#ifndef F3D_COLLISION_H_
#define F3D_COLLISION_H_

#include "f3d.h"

namespace F3D {
    /**
     * Fog class for all games using F3D.
     */

    class BoundingBox {
    private:
        Vec3f   *m_minEdge;
        Vec3f   *m_maxEdge;
        Vec3f   *m_position;
        Vec3f   *m_scale;
        Vec3f   *m_center;
        //private function
    public:
        /**
        * Constructor
        */
        BoundingBox();

        /**
         * Destructor
         */
        virtual ~BoundingBox();

        //set bounding box values
        void setMinEdge(Vec3f vec3f);
        Vec3f* getMinEdge();
        void setMaxEdge(Vec3f vec3f);
        Vec3f* getMaxEdge();
        //set & get position & scale
        void setPosition(GLfloat x, GLfloat y, GLfloat z);
        Vec3f* getPosition();
        void setScale(GLfloat x, GLfloat y, GLfloat z);
        Vec3f* getScale();
        //get bounding box values
        Vec3f *getCenter();

        GLboolean isCollided(BoundingBox *other);
    };

}

#endif /* F3D_COLLISION_H_ */
