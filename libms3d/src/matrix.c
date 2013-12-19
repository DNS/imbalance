/*
	Matrix.cpp

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

#include <stdlib.h>
#include <math.h>

#include "matrix.h"





Matrix *Matrix_new () {
	Matrix *m = malloc(sizeof(Matrix));
	memset(m->m_matrix, 0, sizeof(float) * 16);
	
	Matrix_loadIdentity(m);
	
	return m;
}

Matrix_destroy (Matrix *m) {
	free(m);
}


inline void Matrix_set(Matrix *m, const float *matrix) {
	memcpy(m->m_matrix, matrix, sizeof(float) * 16);
}

inline void Matrix_loadIdentity (Matrix *m) {
	memset(m->m_matrix, 0, sizeof(float) * 16);
	m->m_matrix[0] = m->m_matrix[5] = m->m_matrix[10] = m->m_matrix[15] = 1;
}

inline void Matrix_inverseRotateVect (Matrix *m, float *pVect) {
	float vec[3];
	
	vec[0] = pVect[0] * m->m_matrix[0] + pVect[1] * m->m_matrix[1] + pVect[2] * m->m_matrix[2];
	vec[1] = pVect[0] * m->m_matrix[4] + pVect[1] * m->m_matrix[5] + pVect[2] * m->m_matrix[6];
	vec[2] = pVect[0] * m->m_matrix[8] + pVect[1] * m->m_matrix[9] + pVect[2] * m->m_matrix[10];
	
	memcpy(pVect, vec, sizeof(float) * 3);
}

inline void Matrix_inverseTranslateVect (Matrix *m, float *pVect) {
	pVect[0] = pVect[0] - m->m_matrix[12];
	pVect[1] = pVect[1] - m->m_matrix[13];
	pVect[2] = pVect[2] - m->m_matrix[14];
}





//////////////////////////////////////////////////////////////////////////////

void Matrix_postMultiply (Matrix *m, const Matrix *matrix) {
	float newMatrix[16];
	const float *m1 = m->m_matrix, *m2 = matrix->m_matrix;

	newMatrix[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2];
	newMatrix[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2];
	newMatrix[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2];
	newMatrix[3] = 0;

	newMatrix[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6];
	newMatrix[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6];
	newMatrix[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6];
	newMatrix[7] = 0;

	newMatrix[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10];
	newMatrix[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10];
	newMatrix[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10];
	newMatrix[11] = 0;

	newMatrix[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12];
	newMatrix[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13];
	newMatrix[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14];
	newMatrix[15] = 1;

	Matrix_set(m, newMatrix);
}

void Matrix_setTranslation (Matrix *m, const float *translation) {
	m->m_matrix[12] = translation[0];
	m->m_matrix[13] = translation[1];
	m->m_matrix[14] = translation[2];
}

void Matrix_setInverseTranslation (Matrix *m, const float *translation) {
	m->m_matrix[12] = -translation[0];
	m->m_matrix[13] = -translation[1];
	m->m_matrix[14] = -translation[2];
}

void Matrix_setRotationDegrees (Matrix *m, const float *angles) {
	float vec[3];
	vec[0] = (float) (angles[0] * 180.0 / PI);
	vec[1] = (float) (angles[1] * 180.0 / PI);
	vec[2] = (float) (angles[2] * 180.0 / PI);
	Matrix_setRotationRadians(m, vec);
}

void Matrix_setInverseRotationDegrees (Matrix *m, const float *angles) {
	float vec[3];
	vec[0] = (float) (angles[0] * 180.0 / PI);
	vec[1] = (float) (angles[1] * 180.0 / PI);
	vec[2] = (float) (angles[2] * 180.0 / PI);
	Matrix_setInverseRotationRadians(m, vec);
}

void Matrix_setRotationRadians (Matrix *m, const float *angles) {
	double cr = cos(angles[0]);
	double sr = sin(angles[0]);
	double cp = cos(angles[1]);
	double sp = sin(angles[1]);
	double cy = cos(angles[2]);
	double sy = sin(angles[2]);

	m->m_matrix[0] = (float) (cp * cy);
	m->m_matrix[1] = (float) (cp * sy);
	m->m_matrix[2] = (float) (-sp);

	double srsp = sr * sp;
	double crsp = cr * sp;

	m->m_matrix[4] = (float) (srsp * cy - cr * sy);
	m->m_matrix[5] = (float) (srsp * sy + cr * cy);
	m->m_matrix[6] = (float) (sr * cp);

	m->m_matrix[8] = (float) (crsp * cy + sr * sy);
	m->m_matrix[9] = (float) (crsp * sy - sr * cy);
	m->m_matrix[10] = (float) (cr * cp);
}

void Matrix_setInverseRotationRadians (Matrix *m, const float *angles) {
	double cr = cos(angles[0]);
	double sr = sin(angles[0]);
	double cp = cos(angles[1]);
	double sp = sin(angles[1]);
	double cy = cos(angles[2]);
	double sy = sin(angles[2]);

	m->m_matrix[0] = (float) (cp * cy);
	m->m_matrix[4] = (float) (cp * sy);
	m->m_matrix[8] = (float) (-sp);

	double srsp = sr * sp;
	double crsp = cr * sp;

	m->m_matrix[1] = (float) (srsp * cy - cr * sy);
	m->m_matrix[5] = (float) (srsp * sy + cr * cy);
	m->m_matrix[9] = (float) (sr * cp);

	m->m_matrix[2] = (float) (crsp * cy + sr * sy);
	m->m_matrix[6] = (float) (crsp * sy - sr * cy);
	m->m_matrix[10] = (float) (cr * cp);
}

void Matrix_setRotationQuaternion (Matrix *m, const Quaternion *quat) {
	m->m_matrix[0] = (float) (1.0 - 2.0 * quat->m_quat[1] * quat->m_quat[1] - 2.0 * quat->m_quat[2] * quat->m_quat[2]);
	m->m_matrix[1] = (float) (2.0 * quat->m_quat[0] * quat->m_quat[1] + 2.0 * quat->m_quat[3] * quat->m_quat[2]);
	m->m_matrix[2] = (float) (2.0 * quat->m_quat[0] * quat->m_quat[2] - 2.0 * quat->m_quat[3] * quat->m_quat[1]);

	m->m_matrix[4] = (float) (2.0 * quat->m_quat[0] * quat->m_quat[1] - 2.0 * quat->m_quat[3] * quat->m_quat[2]);
	m->m_matrix[5] = (float) (1.0 - 2.0 * quat->m_quat[0] * quat->m_quat[0] - 2.0 * quat->m_quat[2] * quat->m_quat[2]);
	m->m_matrix[6] = (float) (2.0 * quat->m_quat[1] * quat->m_quat[2] + 2.0 * quat->m_quat[3] * quat->m_quat[0]);

	m->m_matrix[8] = (float) (2.0 * quat->m_quat[0] * quat->m_quat[2] + 2.0 * quat->m_quat[3] * quat->m_quat[1]);
	m->m_matrix[9] = (float) (2.0 * quat->m_quat[1] * quat->m_quat[2] - 2.0 * quat->m_quat[3] * quat->m_quat[0]);
	m->m_matrix[10] = (float) (1.0 - 2.0 * quat->m_quat[0] * quat->m_quat[0] - 2.0 * quat->m_quat[1] * quat->m_quat[1]);
}




