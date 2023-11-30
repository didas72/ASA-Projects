#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wunused-result"

#define MAX(a, b) ((a > b) ? a : b)
#define DIV_UP(a, b) (a>>1)
#define MATRIX(x, y) maxMatrix[INDEX(x, y)]
#define INDEX(x, y) (x + (y) * (srcW + 1))

int calculateMax(int x, int y);

#ifdef DEBUG
void printMatrix();
void printMatrixCSV();
#endif

int srcW, srcH, *maxMatrix;

int main()
{
	/*Final rect*/
	scanf("%d %d", &srcW, &srcH);
	/*Matrix alloc*/
	maxMatrix = (int*)calloc((srcW + 1) * (srcH + 1), sizeof(int));
	/*Possible rects*/
	int x, y, w, h, p;
	scanf("%d", &x);
	while ((--x) >= 0)
	{
		scanf("%d %d %d", &w, &h, &p);
		if (w <= srcW && h <= srcH && MATRIX(w, h) < p) MATRIX(w, h) = p;
		if (h <= srcW && w <= srcH && MATRIX(h, w) < p) MATRIX(h, w) = p;
	}
	/*Compute*/
	for (y = 1; y <= srcH; y++)
	{
		for (x = 1; x <= srcW; x++)
			calculateMax(x, y);
	}
	/*Debug matrix*/
	#ifdef DEBUG
	/*printMatrixCSV();*/
	/*printMatrix();*/
	/*printf("FINAL: ");*/
	#endif
	/*Print result and exit*/
	printf("%d\n", MATRIX(srcW, srcH));
	free(maxMatrix);
	return 0;
}

int calculateMax(int posX, int posY)
{
	int i, bestScore = MATRIX(posX, posY);
	for (i = 1; i <= DIV_UP(posX, 2); i++) bestScore = MAX(bestScore, MATRIX(i, posY) + MATRIX(posX - i, posY));
	for (i = 1; i <= DIV_UP(posY, 2); i++) bestScore = MAX(bestScore, MATRIX(posX, i) + MATRIX(posX, posY - i));
	return MATRIX(posX, posY) = bestScore;
}



#ifdef DEBUG
void printMatrix()
{
	int x, y;
	printf("\n===MATRIX DUMP===\n X\\Y");

	for (y = 0; y <= srcH; y++) printf(" | %4d", y);
	printf("\n----");
	for (y = 0; y <= srcH; y++) printf("-+-----");
	printf("-\n");

	for (x = 0; x <= srcW; x++)
	{
		printf("%4d", x);
		for (y = 0; y <= srcH; y++) printf(" | %4d", MATRIX(x, y));
		printf("\n");
	}
}
void printMatrixCSV()
{
	int x, y;
	for (x = 1; x <= srcW; x++)
	{
		printf("%d", x);
		for (y = 1; y <= srcH; y++) printf("%4d,", MATRIX(x, y));
		printf("\n");
	}
}
#endif
