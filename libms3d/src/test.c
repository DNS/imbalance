

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "ms3d.h"

#include <windows.h>

typedef struct _XML {
	struct _A {
		int x;
	} abc;
	
} XML;

typedef struct _A A;

int main () {
	A a;
	a.x = 777;
	
	printf("-> %d\n", a.x);
	
	
	return 0;
}



