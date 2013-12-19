/*
	Vector.h

		Vector manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 9 December 2000
	Last Edited: 8 January 2001

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef _VECTOR_H
#define _VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Matrix.h"

typedef struct {
	float m_vector[4];	
} Vector;


Vector *Vector_new ();
void Vector0 (Vector *v);
Vector *Vector1_new (const float *values);
void Vector_destroy (Vector *v);
void Vector_transform (Vector *v, const Matrix *m);
void Vector_transform3 (Vector *v, const Matrix *m);

inline void Vector_reset (Vector *v);
inline void Vector_set (Vector *v, const float *values);
inline void Vector_add (Vector *v, const Vector *v1);
inline void Vector_normalize (Vector *v);
inline float Vector_length (Vector *v);







#ifdef __cplusplus
}
#endif


#endif



