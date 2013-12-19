
#include "ms3d.h"

void setupJoints(MS3D *ms3d)
{
	int i;
	for ( i = 0; i < ms3d->nNumJoints; i++ )
	{
		Joint joint = m_pJoints[i];

		joint.m_relative.setRotationRadians( joint.m_localRotation );
		joint.m_relative.setTranslation( joint.m_localTranslation );
		if ( joint.m_parent != -1 )
		{
			joint.m_absolute.set( m_pJoints[joint.m_parent].m_absolute.getMatrix());
			joint.m_absolute.postMultiply( joint.m_relative );
		}
		else
			joint.m_absolute.set( joint.m_relative.getMatrix());
	}

	for ( i = 0; i < m_numVertices; i++ )
	{
		Vertex& vertex = m_pVertices[i];

		if ( vertex.m_boneID != -1 )
		{
			const Matrix& matrix = m_pJoints[vertex.m_boneID].m_absolute;

			matrix.inverseTranslateVect( vertex.m_location );
			matrix.inverseRotateVect( vertex.m_location );
		}
	}

	for ( i = 0; i < m_numTriangles; i++ ) {
		Triangle& triangle = m_pTriangles[i];
		for ( int j = 0; j < 3; j++ ) {
			const Vertex& vertex = m_pVertices[triangle.m_vertexIndices[j]];
			if ( vertex.m_boneID != -1 ) {
				const Matrix& matrix = m_pJoints[vertex.m_boneID].m_absolute;
				matrix.inverseRotateVect( triangle.m_vertexNormals[j] );
			}
		}
	}
}

void restart(MS3D *ms3d)
{
	for ( int i = 0; i < ms3d->nNumJoints; i++ )
	{
		m_pJoints[i].m_currentRotationKeyframe = m_pJoints[i].m_currentTranslationKeyframe = 0;
		m_pJoints[i].m_final.set( m_pJoints[i].m_absolute.getMatrix());
	}

	m_pTimer->reset();
}