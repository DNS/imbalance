/*
	Quaternion.h

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

#ifndef _QUATERNION_H
#define _QUATERNION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#ifndef PI
#	define PI   3.1415926535897932384626433832795
#endif

#ifndef PI_ON_180
#	define PI_ON_180   (PI/180.0)
#endif

typedef struct {
	float m_quat[4];
} Quaternion;

Quaternion *Quaternion_new ();

void Quaternion1 (Quaternion *q, const float *angles);


void Quaternion3 (Quaternion *q, const Quaternion *q1, Quaternion *q2, float interp);

void Quaternion_fromAngles (Quaternion *q, const float *angles);

void Quaternion_slerp (Quaternion *q, const Quaternion *q1, Quaternion *q2, float interp);

inline void Quaternion_inverse (Quaternion *q);


#ifdef __cplusplus
}
#endif 

#endif





