/*
	Quaternion.cpp

		Quaternion manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 8 January 2001
	Last Edited: 8 January 2001

	Please see the file ChangeLog.html for a revision history.
*/

#include <stdlib.h>

#include "quaternion.h"


Quaternion *Quaternion_new () {
	Quaternion *q = malloc(sizeof(Quaternion));
	return q;
}

void Quaternion_destroy (Quaternion *q) {
	free(q);
}

inline void Quaternion_inverse (Quaternion *q) {
	q->m_quat[0] = -q->m_quat[0];
	q->m_quat[1] = -q->m_quat[1];
	q->m_quat[2] = -q->m_quat[2];
	q->m_quat[3] = -q->m_quat[3];
}

Quaternion *Quaternion3_new (const Quaternion *q1, Quaternion *q2, float interp) {
	Quaternion *q = Quaternion_new();
	Quaternion_slerp(q, q1, q2, interp);
	return q;
}

Quaternion *Quaternion1_new (const float *angles) {
	Quaternion *q = Quaternion_new();
	Quaternion_fromAngles(q, angles);
	return q;
}

void Quaternion_fromAngles (Quaternion *q, const float *angles) {
	float angle;
	double sr, sp, sy, cr, cp, cy;

	angle = angles[2] * 0.5f;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1] * 0.5f;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0] * 0.5f;
	sr = sin(angle);
	cr = cos(angle);

	double crcp = cr * cp;
	double srsp = sr * sp;

	q->m_quat[0] = (float) (sr * cp * cy - cr * sp * sy);
	q->m_quat[1] = (float) (cr  *  sp * cy + sr * cp * sy);
	q->m_quat[2] = (float) (crcp * sy - srsp * cy);
	q->m_quat[3] = (float) (crcp * cy + srsp * sy); 
}

void Quaternion_slerp (Quaternion *q, const Quaternion *q1, Quaternion *q2, float interp) {
	// Decide if one of the quaternions is backwards
	int i;
	float a = 0, b = 0;
	for (i = 0; i < 4; i++) {
		a += (q1->m_quat[i] - q2->m_quat[i]) * (q1->m_quat[i] - q2->m_quat[i]);
		b += (q1->m_quat[i] + q2->m_quat[i]) * (q1->m_quat[i] + q2->m_quat[i]);
	}
	if (a > b)
		Quaternion_inverse(q2);

	float cosom = q1->m_quat[0] * q2->m_quat[0] + q1->m_quat[1] * q2->m_quat[1] + q1->m_quat[2] * q2->m_quat[2] + q1->m_quat[3] * q2->m_quat[3];
	double sclq1, sclq2;

	if ((1.0 + cosom) > 0.00000001) {
		if ((1.0 - cosom) > 0.00000001) {
			double omega = acos(cosom);
			double sinom = sin(omega);
			sclq1 = sin((1.0-interp) * omega) / sinom;
			sclq2 = sin(interp * omega) / sinom;
		} else {
			sclq1 = 1.0 - interp;
			sclq2 = interp;
		}
		for (i = 0; i < 4; i++)
			q->m_quat[i] = (float)(sclq1 * q1->m_quat[i] + sclq2 * q2->m_quat[i]);
	} else {
		q->m_quat[0] = -q1->m_quat[1];
		q->m_quat[1] = q1->m_quat[0];
		q->m_quat[2] = -q1->m_quat[3];
		q->m_quat[3] = q1->m_quat[2];

		sclq1 = sin((1.0 - interp) * 0.5 * PI);
		sclq2 = sin(interp * 0.5 * PI);
		for (i = 0; i < 3; i++)
			q->m_quat[i] = (float) (sclq1 * q1->m_quat[i] + sclq2 * q->m_quat[i]);
	}
}



