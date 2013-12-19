

/*
	ms3d.c 
	
	TODO: should be renamed to model.c
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <GL/gl.h>

//#include "glaux.h"


#include "ms3d.h"
#include "ms3d-local.h"
#include "bmp.h"
//#include "transform.h"
//#include "mathlib.h"
//#include "vector.h"
//#include "matrix.h"
//#include "timer.h"



//GLuint texture[1];

// Load Bitmaps And Convert To Textures
static void LoadGLTextures (char *filename, GLuint *id) {
	// Load Texture
	Image *image1;
	
	// allocate space for texture
	image1 = (Image *) malloc(sizeof(Image));
	
	if (image1 == NULL) {
		fprintf(stderr, "Error allocating space for image\n\n");
		exit(0);
	}
	
	if (!ImageLoad(filename, image1)) {
	//if (!ImageLoad("Data/Wood.bmp", image1)) {
		fprintf(stderr, "CANNOT open file: %s\n", filename);
		exit(1);
	}
	
	// Create Texture
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);	// 2d texture (x and y size)
	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST | GL_LINEAR);		// scale linearly when image smalled than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST | GL_LINEAR);	// scale linearly when image bigger than texture
	
	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}



static void BindMaterial (MS3D *ms3d, int group_index, int materialIndex) {
	//printf("------------materialIndex: %d\n", materialIndex);
	if (materialIndex < 0 || materialIndex >= ms3d->nNumMaterials) {
	//if (0) {
		glDepthMask(GL_TRUE);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		//glColor4f(1, 1, 1, 1);
		//glColor4f(1, 0, 0, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		float ma[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		float md[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
		float ms[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float me[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float mss = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ma);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, md);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ms);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, me);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mss);
		
	} else {
		
		//ms3d_material_t *material = m_model->GetMaterial(materialIndex);
		long materialIndex = (long) ms3d->groups[group_index].materialIndex;
		//ms3d_material_t material = ms3d->materials[ materialIndex ];
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ms3d->materials[materialIndex].id);
		//fprintf(stderr, "BIND MATERIAL-> material id: %d\n", ms3d->materials[  (dword) ms3d->groups[group_index].materialIndex  ].id);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,ms3d->materials[ materialIndex ].ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ms3d->materials[ materialIndex ].diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ms3d->materials[ materialIndex ].specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ms3d->materials[ materialIndex ].emissive);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, ms3d->materials[ materialIndex ].shininess);
		
	}
	
}


void restart (MS3D *ms3d) {
	long i;
	for (i = 0; i < ms3d->nNumJoints; i++) {
		ms3d->joints[i].currentkeyFramesRot = ms3d->joints[i].currentkeyFramesTrans = 0;
		//ms3d->joints[i].m_final.set( m_pJoints[i].m_absolute.getMatrix() );
		memcpy(ms3d->joints[i].m_final, ms3d->joints[i].m_absolute, sizeof(Matrix));
	}
	
	ms3d->m_pTimer->reset(ms3d->m_pTimer);		// TODO: BUG !!!
}


void advanceAnimation (MS3D *ms3d) {
	double time = ms3d->m_pTimer->getTime();		// TODO: BUG !!!
	long i;
	if (time > ms3d->m_totalTime) {
		if ( 1  ) {	// m_looping
			restart(ms3d);
			time = 0;
		} else {
			time = ms3d->m_totalTime;
		}
	} 

	for (i = 0; i < ms3d->nNumJoints; i++) {
		float transVec[3];
		Matrix *transform;
		long frame;
		ms3d_joint_t pJoint = ms3d->joints[i];
		
		if (ms3d->joints[i].numKeyFramesRot == 0 && ms3d->joints[i].numKeyFramesTrans == 0) {
			//pJoint->m_final.set( pJoint->m_absolute.getMatrix());
			memcpy(ms3d->joints[i].m_final, ms3d->joints[i].m_absolute, sizeof(Matrix));
			continue;
		}

		frame = ms3d->joints[i].currentkeyFramesTrans;
		while (frame < ms3d->joints[i].numKeyFramesTrans && ms3d->joints[i].keyFramesTrans[frame].time < time) {
			frame++;
		}
		ms3d->joints[i].currentkeyFramesTrans = frame;
		
		if (frame == 0) {
			memcpy(transVec, ms3d->joints[i].keyFramesTrans[0].position, sizeof(float) * 3);
		} else if (frame == ms3d->joints[i].numKeyFramesTrans) {
			memcpy(transVec, ms3d->joints[i].keyFramesTrans[frame-1].position, sizeof(float) * 3);
		} else {
			assert(frame > 0 && frame < ms3d->joints[i].numKeyFramesTrans);
			
			//const Model::Keyframe& curFrame = pJoint->numKeyFramesTrans[frame];
			//const Model::Keyframe& prevFrame = pJoint->numKeyFramesTrans[frame-1];
			
			const ms3d_keyframe_pos_t curFrame = ms3d->joints[i].keyFramesTrans[frame];
			const ms3d_keyframe_pos_t prevFrame = ms3d->joints[i].keyFramesTrans[frame-1];
			
			float timeDelta = curFrame.time - prevFrame.time;
			float interpValue = (float) ( (time - prevFrame.time) / timeDelta);
			
			transVec[0] = prevFrame.position[0] + (curFrame.position[0] - prevFrame.position[0]) * interpValue;
			transVec[1] = prevFrame.position[1] + (curFrame.position[1] - prevFrame.position[1]) * interpValue;
			transVec[2] = prevFrame.position[2] + (curFrame.position[2] - prevFrame.position[2]) * interpValue; 
		}

		frame = ms3d->joints[i].currentkeyFramesRot;
		while (frame < ms3d->joints[i].numKeyFramesRot && ms3d->joints[i].keyFramesRot[frame].time < time) {
			frame++;
		}
		ms3d->joints[i].currentkeyFramesRot = frame;

		if (frame == 0) {
			Matrix_setRotationRadians(transform, ms3d->joints[i].keyFramesRot[0].rotation);
		} else if (frame == ms3d->joints[i].numKeyFramesRot) {
			Matrix_setRotationRadians(transform, ms3d->joints[i].keyFramesRot[frame-1].rotation);
		} else {
			assert(frame > 0 && frame < ms3d->joints[i].numKeyFramesRot);

			//const Model::Keyframe& curFrame = pJoint->m_pRotationKeyframes[frame];
			//const Model::Keyframe& prevFrame = pJoint->m_pRotationKeyframes[frame-1];
			
			const ms3d_keyframe_rot_t curFrame = ms3d->joints[i].keyFramesRot[frame];
			const ms3d_keyframe_rot_t prevFrame = ms3d->joints[i].keyFramesRot[frame-1];
			
			float timeDelta = curFrame.time - prevFrame.time;
			float interpValue = (float) ( (time - prevFrame.time) / timeDelta);
			
			assert(interpValue >= 0 && interpValue <= 1);
			
#if 0
			
			Quaternion *qPrev = Quaternion1_new(prevFrame.rotation);
			Quaternion *qCur = Quaternion1_new(curFrame.rotation);
			Quaternion *qFinal = Quaternion3_new(qPrev, qCur, interpValue);
			Matrix_setRotationQuaternion(transform, qFinal);
#else
			float rotVec[3];
			
			rotVec[0] = prevFrame.rotation[0] + (curFrame.rotation[0] - prevFrame.rotation[0]) * interpValue;
			rotVec[1] = prevFrame.rotation[1] + (curFrame.rotation[1] - prevFrame.rotation[1]) * interpValue;
			rotVec[2] = prevFrame.rotation[2] + (curFrame.rotation[2] - prevFrame.rotation[2]) * interpValue;
			
			Matrix_setRotationRadians(transform, rotVec);
#endif
		}
		
		Matrix_setTranslation(transform, transVec );
		//Matrix relativeFinal( pJoint->m_relative );
		Matrix *relativeFinal = Matrix_new();
		memcpy(relativeFinal, ms3d->joints[i].m_relative, sizeof(Matrix));
		Matrix_postMultiply(relativeFinal, transform);
		
		if (ms3d->joints[i].m_parent == -1) {
			memcpy(ms3d->joints[i].m_final, relativeFinal, sizeof(Matrix));
		} else {
			//pJoint->m_final.set( m_pJoints[pJoint->m_parent].m_final.getMatrix());
			//pJoint->m_final.postMultiply( relativeFinal );
			
			memcpy(ms3d->joints[i].m_final, ms3d->joints[ ms3d->joints[i].m_parent ].m_final, sizeof(Matrix));
			Matrix_postMultiply(ms3d->joints[i].m_final, relativeFinal);
		}
		
		free(relativeFinal);
		free(transform);
	}
}
*/

static void draw (MS3D *ms3d) {
	//advanceAnimation(ms3d);
	GLboolean texEnabled = glIsEnabled(GL_TEXTURE_2D);
	
	// Draw by group
	unsigned long i, j, k;
	for (i = 0; i < ms3d->nNumGroups; i++) {	// Groups | Meshes
		//ms3d_group_t *group = m_model->GetGroup(i);
		
		
		// SET TEXTURES
		dword materialIndex = ms3d->groups[i].materialIndex;
		//ms3d_material_t material = ms3d->materials[materialIndex];
		if (materialIndex >= 0) {
		//if (1) {
			
			glMaterialfv( GL_FRONT, GL_AMBIENT, ms3d->materials[materialIndex].ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE, ms3d->materials[materialIndex].diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, ms3d->materials[materialIndex].specular );
			glMaterialfv( GL_FRONT, GL_EMISSION, ms3d->materials[materialIndex].emissive );
			glMaterialf( GL_FRONT, GL_SHININESS, ms3d->materials[materialIndex].shininess );
			
			//fprintf(stderr, "DRAW -> meterial id: %d\n", material.id);
			//fprintf(stderr, "DRAW -> meterial id: %d\n", texture[0]);
			//if ( 1 ) {
			if ( ms3d->materials[materialIndex].texture > 0 ) {
				BindMaterial(ms3d, i, ms3d->groups[i].materialIndex);
				glEnable(GL_TEXTURE_2D);
			} else {
				glDisable(GL_TEXTURE_2D);
			}
			
		} else {
			// Material properties?
			BindMaterial(ms3d, i, -1);
			glDisable( GL_TEXTURE_2D );
		}
		
		
		// DRAW POLYGONS
		glBegin( GL_TRIANGLES );
		{
			for (j = 0; j < ms3d->groups[i].numtriangles; j++ ) {
				long triangleIndex = ms3d->groups[i].triangleIndices[j];
				// DRAW FACES; TEXTURES
				for (k = 0; k < 3; k++) {
					word vertexIndex = ms3d->triangles[triangleIndex].vertexIndices[k];
					
					// Model.cpp : draw()
					/*
					if (ms3d->vertices[vertexIndex].boneId == -1) {
						// same as before
						glNormal3fv(ms3d->triangles[triangleIndex].vertexNormals[k]);
						glTexCoord2f(ms3d->triangles[triangleIndex].s[k], ms3d->triangles[triangleIndex].t[k]);
						glVertex3fv(ms3d->vertices[vertexIndex].vertex);
					} else {
						// rotate according to transformation matrix
						//const Matrix *final = m_pJoints[ (long) m_pVertices[index].m_boneID].m_final;
						const Matrix *final = ms3d->joints[ (long) ms3d->vertices[vertexIndex].boneId ].m_final;

						//glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );

						//Vector newNormal( pTri->m_vertexNormals[k] );
						//newNormal.transform3( final );
						//newNormal.normalize();
						//glNormal3fv( newNormal.getVector());

						Vector *newVertex = Vector1_new(ms3d->vertices[vertexIndex].vertex);
						float f = final->m_matrix[0];
						printf("1-> %f\n", f);
						//Vector_transform(newVertex, final);
						//glVertex3fv(newVertex->m_vector);
					}
					*/
					
					
					// NO ANIMATION
					
					glNormal3fv(ms3d->triangles[triangleIndex].vertexNormals[k]);
					glTexCoord2f(ms3d->triangles[triangleIndex].s[k], ms3d->triangles[triangleIndex].t[k]);
					glVertex3fv(ms3d->vertices[vertexIndex].vertex);
					
				}
			}
		}
		glEnd();
		
	}

	/*
	glBegin (GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f, 1.0f);
			glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f, -0.5f);
			glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);
			glEnd();
	*/
	//printf("Number Groups: %d\n", ms3d->nNumGroups);
	
	//puts("HOREEEEEEEEEEEEEEEEEE");
	
	if (texEnabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}


static void reloadTextures (MS3D *ms3d) {
	long i;
	for ( i = 0; i < ms3d->nNumMaterials; i++ ) {
		if (strlen( ms3d->materials[i].texture ) > 0 ) {
			LoadGLTextures(ms3d->materials[i].texture, &ms3d->materials[i].id);
		} else {
			ms3d->materials[i].id = 0;
		}
	}
}


void setupJoints (MS3D *ms3d) {
	long i, j;
	for (i = 0; i < ms3d->nNumJoints; i++) {
		//Joint& joint = m_pJoints[i];
		

		Matrix_setRotationRadians(ms3d->joints[i].m_relative, ms3d->joints[i].rotation);
		Matrix_setTranslation(ms3d->joints[i].m_relative, ms3d->joints[i].position);
		if ( ms3d->joints[i].m_parent != -1 )
		{
			//Matrix_set(ms3d->joints[i].m_absolute, ms3d->joints[ ms3d->joints[i].m_parent ].m_absolute.m_matrix);
			memcpy(ms3d->joints[i].m_absolute, ms3d->joints[ ms3d->joints[i].m_parent ].m_absolute, sizeof(Matrix));
			Matrix_postMultiply(ms3d->joints[i].m_absolute, ms3d->joints[i].m_relative);
		}
		else
			memcpy(ms3d->joints[i].m_absolute, ms3d->joints[i].m_relative, sizeof(Matrix));
	}

	for ( i = 0; i < ms3d->nNumVertices; i++ )
	{
		//Vertex& vertex = m_pVertices[i];
		
		if ( ms3d->vertices[i].boneId != -1 )
		{
			Matrix *matrix = ms3d->joints[(long) ms3d->vertices[i].boneId].m_absolute;

			Matrix_inverseTranslateVect(matrix, ms3d->vertices[i].vertex);
			Matrix_inverseRotateVect(matrix, ms3d->vertices[i].vertex);
		}
	}

	for ( i = 0; i < ms3d->nNumTriangles; i++ ) {
		//Triangle& triangle = m_pTriangles[i];
		for (j = 0; j < 3; j++ ) {
			//const Vertex& vertex = m_pVertices[ ms3d.triangles[i].m_vertexIndices[j] ];
			ms3d_vertex_t vertex = ms3d->vertices[ ms3d->triangles[i].vertexIndices[j] ];
			
			if (ms3d->vertices[i].boneId != -1) {
				Matrix *matrix = ms3d->joints[(long) ms3d->vertices[i].boneId].m_absolute;
				Matrix_inverseRotateVect(matrix, ms3d->triangles[i].vertexNormals[j]);		// BUG
			}
		}
	}
}

static BOOL loadFromFile (MS3D *ms3d, char *filepath) {
	unsigned long i, j, k;
	FILE *fh;
	if ((fh = fopen(filepath, "rb")) == NULL) {
		fprintf(stderr, "fopen: can't open file %s\n", filepath);
		return FALSE;
	}
	
	fseek(fh, 0, SEEK_END);
	long filesize = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	
	byte *pbuffer = malloc(filesize);
	fread(pbuffer, 1, filesize, fh);
	fclose(fh);
	
	const byte *ptr = pbuffer;
	
	//fread(&ms3d->header, sizeof(ms3d_header_t), 1, fh);
	memcpy(&ms3d->header, ptr, sizeof(ms3d_header_t));
	ptr += sizeof(ms3d_header_t);
	
	if (strncmp(ms3d->header.id, "MS3D000000", 10) != 0) return FALSE;
	if (ms3d->header.version != 4) return FALSE;
	
	//fread(&ms3d->nNumVertices, 1, sizeof(word), fh);
	memcpy(&ms3d->nNumVertices, ptr, sizeof(word));
	ptr += sizeof(word);
	
	if (ms3d->nNumVertices > 0) {
		ms3d->vertices = malloc(ms3d->nNumVertices * sizeof(ms3d_vertex_t));	
		//printf("-> %d\n", ms3d->nNumVertices);
		for (i = 0; i < ms3d->nNumVertices; i++) {
			//fread(ms3d->vertices, ms3d->nNumVertices, sizeof(ms3d_vertex_t), fh);
			//memcpy(&ms3d->vertices[i], ptr, sizeof(ms3d_vertex_t));
			//ptr += sizeof(ms3d_vertex_t);
			
			
			memcpy(&ms3d->vertices[i].flags, ptr, sizeof(byte));
			ptr += sizeof(byte);
			memcpy(&ms3d->vertices[i].vertex, ptr, sizeof(float) * 3);
			ptr += (sizeof(float) * 3);
			memcpy(&ms3d->vertices[i].boneId, ptr, sizeof(char));
			ptr += sizeof(char);
			memcpy(&ms3d->vertices[i].referenceCount, ptr, sizeof(byte));
			ptr += sizeof(byte);
			
			
		}
	}
	
	//fread(&ms3d->nNumTriangles, 1, sizeof(word), fh);
	memcpy(&ms3d->nNumTriangles, ptr, sizeof(word));
	ptr += sizeof(word);
	
	if (ms3d->nNumTriangles > 0) {
		ms3d->triangles = malloc(ms3d->nNumTriangles * sizeof(ms3d_triangle_t));
		for (i=0; i < ms3d->nNumTriangles; i++) {
			//fread(ms3d->triangles, ms3d->nNumTriangles, sizeof(ms3d_triangle_t), fh);
			
			memcpy(&ms3d->triangles[i].flags, ptr, sizeof(word));
			ptr += sizeof(word);
			memcpy(&ms3d->triangles[i].vertexIndices, ptr, sizeof(word) * 3);
			ptr += sizeof(word) * 3;
			memcpy(&ms3d->triangles[i].vertexNormals, ptr, sizeof(float) * 3 * 3);
			ptr += sizeof(float) * 3 * 3;
			memcpy(&ms3d->triangles[i].s, ptr, sizeof(float) * 3);
			ptr += sizeof(float) * 3;
			memcpy(&ms3d->triangles[i].t, ptr, sizeof(float) * 3);
			ptr += sizeof(float) * 3;
			memcpy(&ms3d->triangles[i].smoothingGroup, ptr, sizeof(byte));
			ptr += sizeof(byte);
			memcpy(&ms3d->triangles[i].groupIndex, ptr, sizeof(byte));
			ptr += sizeof(byte);
			
			
			// SHORCUT, BUT NOT RECOMMENDED,, CAN CAUSE BUGS
			//memcpy(&ms3d->triangles[i], ptr, sizeof(ms3d_triangle_t));
			//ptr += sizeof(ms3d_triangle_t);
			
			ms3d->triangles[i].t[0] = 1.0f - ms3d->triangles[i].t[0];
			ms3d->triangles[i].t[1] = 1.0f - ms3d->triangles[i].t[1];
			ms3d->triangles[i].t[2] = 1.0f - ms3d->triangles[i].t[2];
		}
		// BUG t, k
		
	}
	
	// EDGES ?
	
	// Groups
	
	//fread(&ms3d->nNumGroups, 1, sizeof(word), fh);
	memcpy(&ms3d->nNumGroups, ptr, sizeof(word));
	ptr += sizeof(word);
	
	if (ms3d->nNumGroups > 0) {
		ms3d->groups = malloc(ms3d->nNumGroups * sizeof(ms3d_group_t));
		
		for (i = 0; i < ms3d->nNumGroups; i++) {
			//fread(&ms3d->groups[i].flags, 1, sizeof(byte), fh);
			memcpy(&ms3d->groups[i].flags, ptr, sizeof(byte));
			ptr += sizeof(byte);
			
			//fread(&ms3d->groups[i].name, 1, 32, fh);
			memcpy(ms3d->groups[i].name, ptr, 32);	// POSSIBLY BUG ??
			ptr += 32;
			
			//fread(&ms3d->groups[i].numtriangles, 1, sizeof(word), fh);
			memcpy(&ms3d->groups[i].numtriangles, ptr, sizeof(word));
			ptr += sizeof(word);
			
			if (ms3d->groups[i].numtriangles != 0) {
				ms3d->groups[i].triangleIndices = malloc(ms3d->groups[i].numtriangles * sizeof(word));
				for (j = 0; j < ms3d->groups[i].numtriangles; j++) {
					//fread(ms3d->groups[i].triangleIndices, ms3d->groups[i].numtriangles, sizeof(word), fh);
					memcpy(&ms3d->groups[i].triangleIndices[j], ptr, sizeof(word));
					ptr += sizeof(word);
				}
			}
			
			//fread(&ms3d->groups[i].materialIndex, 1, sizeof(char), fh);
			memcpy(&ms3d->groups[i].materialIndex, ptr, sizeof(char));
			ptr += sizeof(char);
		}
	
	}
	
	
	// materials
	
	//fread(&ms3d->nNumMaterials, 1, sizeof(word), fh);
	memcpy(&ms3d->nNumMaterials, ptr, sizeof(word));
	ptr += sizeof(word);
	
	if (ms3d->nNumMaterials > 0) {
		//ms3d->materials = malloc( (ms3d->nNumMaterials * sizeof(ms3d_material_t)) - (sizeof(GLuint) * ms3d->nNumMaterials) );
		ms3d->materials = malloc(ms3d->nNumMaterials * sizeof(ms3d_material_t));
		// BUG HERE -> FIXED !!
		for (i = 0; i < ms3d->nNumMaterials; i++) {
			//fread(&ms3d->materials[i], 1, (sizeof(ms3d_material_t) - sizeof(GLuint)), fh);	// BUG FIXED
			//memcpy(ms3d->materials[i].name, ptr, 32);
			//memcpy(ms3d->materials[i].ambient, ptr, sizeof(float) * 4);
			//memcpy(ms3d->materials[i].diffuse, ptr, sizeof(float) * 4);
			//memcpy(ms3d->materials[i].specular, ptr, sizeof(float) * 4);
			//memcpy(ms3d->materials[i].emissive, ptr, sizeof(float) * 4);
			//memcpy(ms3d->materials[i].shininess, ptr, sizeof(float));
			//memcpy(ms3d->materials[i].transparency, ptr, sizeof(float));
			
			//memcpy(ms3d->materials[i].mode, ptr, sizeof(float));
			
			memcpy(&ms3d->materials[i], ptr, sizeof(ms3d_material_t) - sizeof(GLuint));	// MINUS `.id' ,, not part of file format
			ptr += sizeof(ms3d_material_t) - sizeof(GLuint);
			
			// TEMPORARY BUG HERE!, better use MS3D_new('prefix_texture_path')
			char buf[128];
			strcpy(buf, TEXTURE_PATH);
			strcat(buf, ms3d->materials[i].texture);
			strcpy(ms3d->materials[i].texture, buf);
			
			
			//fseek(fh, SEEK_CUR, sizeof(GLuint));
		}
		
	}
	
	
	reloadTextures(ms3d);
	
	memcpy(&ms3d->fAnimationFPS, ptr, sizeof(float));
	ptr += sizeof(float);
	memcpy(&ms3d->fCurrentTime, ptr, sizeof(float));
	ptr += sizeof(float);
	memcpy(&ms3d->iTotalFrames, ptr, sizeof(long));
	ptr += sizeof(dword);
	
	
	//fread(&ms3d->nNumJoints, 1, sizeof(float), fh);
	memcpy(&ms3d->nNumJoints, ptr, sizeof(word));
	ptr += sizeof(word);
	if (ms3d->nNumJoints > 1) {
		ms3d->joints = malloc(ms3d->nNumJoints * sizeof(ms3d_joint_t));
		for (i = 0; i < ms3d->nNumJoints; i++) {
			memcpy(&ms3d->joints[i].flags, ptr, sizeof(byte));
			ptr += sizeof(byte);
			memcpy(&ms3d->joints[i].name, ptr, 32);
			ptr += 32;
			memcpy(&ms3d->joints[i].parentName, ptr, 32);
			ptr += 32;
			memcpy(&ms3d->joints[i].rotation, ptr, sizeof(float) * 3);
			ptr += sizeof(float) * 3;
			memcpy(&ms3d->joints[i].position, ptr, sizeof(float) * 3);
			ptr += sizeof(float) * 3;
			memcpy(&ms3d->joints[i].numKeyFramesRot, ptr, sizeof(word));
			ptr += sizeof(word);
			memcpy(&ms3d->joints[i].numKeyFramesTrans, ptr, sizeof(word));
			ptr += sizeof(word);
			if (ms3d->joints[i].numKeyFramesRot > 0) {
				ms3d->joints[i].keyFramesRot = malloc(ms3d->joints[i].numKeyFramesRot * sizeof(ms3d_keyframe_rot_t));
				for (j = 0; j < ms3d->joints[i].numKeyFramesRot; j++) {
					//fread(ms3d->joints[i].keyFramesRot, ms3d->joints[i].numKeyFramesRot, sizeof(ms3d_keyframe_rot_t), fh);
					memcpy(&ms3d->joints[i].keyFramesRot->time, ptr, sizeof(float));
					ptr += sizeof(float);
					memcpy(&ms3d->joints[i].keyFramesRot->rotation, ptr, sizeof(float) * 3);
					ptr += sizeof(float) * 3;
				}
			}
			if (ms3d->joints[i].numKeyFramesTrans > 0) {
				ms3d->joints[i].keyFramesTrans = malloc(ms3d->joints[i].numKeyFramesTrans * sizeof(ms3d_keyframe_pos_t));
				for (j = 0; j < ms3d->joints[i].numKeyFramesTrans; j++) {
					//fread(ms3d->joints[i].keyFramesTrans, ms3d->joints[i].numKeyFramesTrans, sizeof(ms3d_keyframe_pos_t), fh);
					memcpy(&ms3d->joints[i].keyFramesTrans->time, ptr, sizeof(float));
					ptr += sizeof(float);
					memcpy(&ms3d->joints[i].keyFramesTrans->position, ptr, sizeof(float) * 3);
					ptr += sizeof(float) * 3;
				}
			}
		}
	
	}
	
	
	
	free(pbuffer);
	
	ms3d->m_totalTime = ms3d->iTotalFrames * 1000.0 / ms3d->fAnimationFPS;
	//ms3d->m_pTimer = Timer_getNewTimer();
	//ms3d->m_pTimer = (Timer *) Timer_getNewTimer();
	//setupJoints(ms3d);
	
	return TRUE;
}


static void destroy (MS3D *ms3d) {
	unsigned long i;
	ms3d->loadFromFile = NULL;
	ms3d->destroy = NULL;
	
	for (i=0; i<ms3d->nNumGroups; i++) {
		if (ms3d->groups[i].numtriangles > 0) {
			free(ms3d->groups[i].triangleIndices);
		}
	}
	
	for (i = 0; i < ms3d->nNumJoints; i++) {
		//printf("-> %d\n", ms3d->joints[i].numKeyFramesRot);
		if (ms3d->joints[i].numKeyFramesRot > 0) {
			free(ms3d->joints[i].keyFramesRot);
		}
		if (ms3d->joints[i].numKeyFramesTrans > 0) {
			free(ms3d->joints[i].keyFramesTrans);
		}
	}
	
	free(ms3d->vertices);
	free(ms3d->triangles);
	free(ms3d->materials);
	free(ms3d->joints);
	free(ms3d->groups);
	free(ms3d);
	
	/*
	m_animationFps = 24.0f;
	m_currentTime = 1.0f;
	m_totalFrames = 30;
	m_comment.clear();
	m_jointSize = 1.0f;
	m_transparencyMode = TRANSPARENCY_MODE_SIMPLE;
	m_alphaRef = 0.5f;
	*/
	
	fprintf(stderr, "destroy: ok\n");
}


extern MS3D *new_MS3D (char *filepath) {
	MS3D *ms3d = malloc(sizeof(MS3D));
	ms3d->loadFromFile = &loadFromFile;
	ms3d->destroy = &destroy;
	ms3d->draw = &draw;
	
	loadFromFile(ms3d, filepath);
	//LoadGLTextures(ms3d);
	return ms3d;
}

/*
int main () {
	
	MS3D *ms3d = new_MS3D("Model.ms3d");
	
	
	ms3d->destroy(ms3d);
	
	//puts("> END");
	return 0;
}
*/



