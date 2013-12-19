

#ifndef _BMP_H
#define _BMP_H

#ifdef __cplusplus
extern "C" {
#endif


/* storage for one texture  */

/* Image type - contains height, width, and data */
typedef struct _Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
} Image;

//typedef struct Image Image;

#ifdef __cplusplus
}
#endif

#endif

