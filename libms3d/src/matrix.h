/*
	Matrix.h

		Matrix manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 9 December 2000
	Last Edited: 8 January 2001

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "quaternion.h"



typedef struct {
	float m_matrix[16];
} Matrix;


extern Matrix *Matrix_new ();

inline void Matrix_set(Matrix *m, const float *matrix);
inline void Matrix_loadIdentity (Matrix *m);
inline void Matrix_inverseRotateVect (Matrix *m, float *pVect);
inline void Matrix_inverseTranslateVect (Matrix *m, float *pVect);

void Matrix_postMultiply (Matrix *m, const Matrix *matrix);
void Matrix_setTranslation (Matrix *m, const float *translation);
void Matrix_setInverseTranslation (Matrix *m, const float *translation);
void Matrix_setRotationDegrees (Matrix *m, const float *angles);
void Matrix_setInverseRotationDegrees (Matrix *m, const float *angles);
void Matrix_setRotationRadians (Matrix *m, const float *angles);
void Matrix_setInverseRotationRadians (Matrix *m, const float *angles);
void Matrix_setRotationQuaternion (Matrix *m, const Quaternion *quat);


#ifdef __cplusplus
}
#endif 



#endif 


