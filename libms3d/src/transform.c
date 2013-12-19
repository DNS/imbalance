
#include <stdio.h>

#include "ms3d.h"
#include "mathlib.h"
#include "transform.h"


void TransformVertex(MS3D *ms3d, const ms3d_vertex_t *vertex, float *out) {
	int jointIndices[4], jointWeights[4];
	FillJointIndicesAndWeights(vertex, jointIndices, jointWeights);
	
	if (jointIndices[0] < 0 || jointIndices[0] >= ms3d->nNumJoints || ms3d->fCurrentTime < 0.0f) {
		out[0] = vertex->vertex[0];
		out[1] = vertex->vertex[1];
		out[2] = vertex->vertex[2];
	} else {
		// count valid weights
		int numWeights = 0;
		int i;
		for (i = 0; i < 4; i++) {
			if ((jointWeights[i] > 0) && (jointIndices[i] >= 0) && (jointIndices[i] < ms3d->nNumJoints) )
				++numWeights;
			else
				break;
		}

		// init
		out[0] = 0.0f;
		out[1] = 0.0f;
		out[2] = 0.0f;

		float weights[4] = { (float) jointWeights[0] / 100.0f, (float) jointWeights[1] / 100.0f, (float) jointWeights[2] / 100.0f, (float) jointWeights[3] / 100.0f };
		if (numWeights == 0) {
			numWeights = 1;
			weights[0] = 1.0f;
		}
		// add weighted vertices
		for (i = 0; i < numWeights; i++) {
			//const ms3d_joint_t *joint = &m_joints[jointIndices[i]];
			const ms3d_joint_t *joint = &ms3d->joints[jointIndices[i]];
			vec3_t tmp, vert;
			//VectorITransform(vertex->vertex, joint->matGlobalSkeleton, tmp);
			VectorITransform(vertex->vertex, (void *) ms3d->joints[jointIndices[i]].matGlobalSkeleton, tmp);
			VectorTransform(tmp, (void *) ms3d->joints[jointIndices[i]].matGlobal, vert);

			out[0] += vert[0] * weights[i];
			out[1] += vert[1] * weights[i];
			out[2] += vert[2] * weights[i];
			
			printf("%f, %f, %f\n", ms3d->joints[jointIndices[i]].matGlobalSkeleton[1][0], ms3d->joints[jointIndices[i]].matGlobalSkeleton[1][1], ms3d->joints[jointIndices[i]].matGlobalSkeleton[1][2] );
			//printf("%f, %f, %f\n", tmp[0], tmp[1], tmp[2]);
			//printf("%f, %f, %f\n", vert[0], vert[1], vert[2]);
			//printf("%f\n", weights[i]);
		}
	}
}

void TransformNormal (MS3D *ms3d, const ms3d_vertex_t *vertex, const vec3_t normal, vec3_t out) {
	int i;
	int jointIndices[4], jointWeights[4];
	FillJointIndicesAndWeights(vertex, jointIndices, jointWeights);

	//if (jointIndices[0] < 0 || jointIndices[0] >= (int) m_joints.size() || m_currentTime < 0.0f)
	if (jointIndices[0] < 0 || jointIndices[0] >= ms3d->nNumJoints || ms3d->fCurrentTime < 0.0f)
	{
		out[0] = normal[0];
		out[1] = normal[1];
		out[2] = normal[2];
	}
	else
	{
		// count valid weights
		int numWeights = 0;
		for (i = 0; i < 4; i++)
		{
			if (jointWeights[i] > 0 && jointIndices[i] >= 0 && jointIndices[i] < ms3d->nNumJoints)
				++numWeights;
			else
				break;
		}

		// init
		out[0] = 0.0f;
		out[1] = 0.0f;
		out[2] = 0.0f;

		float weights[4] = { (float) jointWeights[0] / 100.0f, (float) jointWeights[1] / 100.0f, (float) jointWeights[2] / 100.0f, (float) jointWeights[3] / 100.0f };
		if (numWeights == 0)
		{
			numWeights = 1;
			weights[0] = 1.0f;
		}
		// add weighted vertices
		for (i = 0; i < numWeights; i++)
		{
			//const ms3d_joint_t *joint = &m_joints[jointIndices[i]];
			const ms3d_joint_t *joint = &ms3d->joints[jointIndices[i]];
			vec3_t tmp, norm;
			VectorIRotate(normal, joint->matGlobalSkeleton, tmp);
			VectorRotate(tmp, joint->matGlobal, norm);

			out[0] += norm[0] * weights[i];
			out[1] += norm[1] * weights[i];
			out[2] += norm[2] * weights[i];
		}
	}
}




void FillJointIndicesAndWeights (const ms3d_vertex_t *vertex, int jointIndices[4], int jointWeights[4]) {
	jointIndices[0] = vertex->boneId;
	jointIndices[1] = vertex->boneIds[0];
	jointIndices[2] = vertex->boneIds[1];
	jointIndices[3] = vertex->boneIds[2];
	jointWeights[0] = 100;
	jointWeights[1] = 0;
	jointWeights[2] = 0;
	jointWeights[3] = 0;
	if (vertex->weights[0] != 0 || vertex->weights[1] != 0 || vertex->weights[2] != 0) {
		jointWeights[0] = vertex->weights[0];
		jointWeights[1] = vertex->weights[1];
		jointWeights[2] = vertex->weights[2];
		jointWeights[3] = 100 - (vertex->weights[0] + vertex->weights[1] + vertex->weights[2]);
	}
}













