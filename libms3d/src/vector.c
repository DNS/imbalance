/*
	Vector.cpp

		Vector manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 9 December 2000
	Last Edited: 6 February 2001

	Please see the file ChangeLog.html for a revision history.
*/


#include <stdio.h>

#include "Vector.h"


Vector *Vector_new () {
	Vector *v = malloc(sizeof(Vector));
	//Vector0(v);		// BUG ?
	return v;
}

void Vector0 (Vector *v) {
	Vector_reset(v);
}

Vector *Vector1_new (const float *values) {
	Vector *v = Vector_new();
	Vector_set(v, values);
	v->m_vector[3] = 1;
	return v;
}

void Vector_destroy (Vector *v) {
	free(v);
}

void Vector_transform (Vector *v, const Matrix *m) {
	double vector[4];
	const float *matrix = m->m_matrix;	// BUG ?
	//printf("1-> %f\n", m->m_matrix[0]);
	//printf("2-> %f\n", matrix->m_matrix[1]);
	/*
	vector[0] = v->m_vector[0] * matrix[0] + v->m_vector[1] * matrix[4] + v->m_vector[2] * matrix[8] + matrix[12];
	vector[1] = v->m_vector[0] * matrix[1] + v->m_vector[1] * matrix[5] + v->m_vector[2] * matrix[9] + matrix[13];
	vector[2] = v->m_vector[0] * matrix[2] + v->m_vector[1] * matrix[6] + v->m_vector[2] * matrix[10] + matrix[14];
	vector[3] = v->m_vector[0] * matrix[3] + v->m_vector[1] * matrix[7] + v->m_vector[2] * matrix[11] + matrix[15];
	
	v->m_vector[0] = (float) (vector[0]);
	v->m_vector[1] = (float) (vector[1]);
	v->m_vector[2] = (float) (vector[2]);
	v->m_vector[3] = (float) (vector[3]);*/
}

void Vector_transform3 (Vector *v, const Matrix *m) {
	double vector[3];
	const float *matrix = m->m_matrix;	// BUG ?

	vector[0] = v->m_vector[0] * matrix[0] + v->m_vector[1] * matrix[4] + v->m_vector[2] * matrix[8];
	vector[1] = v->m_vector[0] * matrix[1] + v->m_vector[1] * matrix[5] + v->m_vector[2] * matrix[9];
	vector[2] = v->m_vector[0] * matrix[2] + v->m_vector[1] * matrix[6] + v->m_vector[2] * matrix[10];

	v->m_vector[0] = (float) (vector[0]);
	v->m_vector[1] = (float) (vector[1]);
	v->m_vector[2] = (float) (vector[2]);
	v->m_vector[3] = 1;
}




/////////////////////////////////////////

inline float *Vector_getVector (Vector *v) {
	//float
}


inline void Vector_reset (Vector *v) {
	v->m_vector[0] = v->m_vector[1] = v->m_vector[2] = 0;
	v->m_vector[3] = 1;
}

inline void Vector_set (Vector *v, const float *values) {
	v->m_vector[0] = values[0];
	v->m_vector[1] = values[1];
	v->m_vector[2] = values[2];
}

inline void Vector_add (Vector *v, const Vector *v1) {
	v->m_vector[0] += v1->m_vector[0];
	v->m_vector[1] += v1->m_vector[1];
	v->m_vector[2] += v1->m_vector[2];
	v->m_vector[3] += v1->m_vector[3];
}

inline void Vector_normalize (Vector *v) {
	float len = Vector_length(v);

	v->m_vector[0] /= len;
	v->m_vector[1] /= len;
	v->m_vector[2] /= len;
}

inline float Vector_length (Vector *v) {
	return (float) sqrt(v->m_vector[0] * v->m_vector[0] + v->m_vector[1] * v->m_vector[1] + v->m_vector[2] * v->m_vector[2]);
}





