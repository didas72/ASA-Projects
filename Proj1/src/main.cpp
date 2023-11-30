#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a > b) ? a : b)
#define DIV_UP(a, b) ((a-1)/b+1)

typedef struct HalfMatrix
{
	int *store;
	int w, h;
	size_t tri;

public:
	HalfMatrix() : store(NULL), w(0), h(0) {}
	HalfMatrix(int mw, int mh) : w(mw), h(mh) {
		size_t triangle = (mw*(mw+1)) >> 1;
		size_t rect = (mh-mw)*mw;
		size_t total = triangle + rect;
		tri = triangle;
		store = (int*)malloc(total * sizeof(int));
		memset(store, 0, total * sizeof(int)); //set all to 0
	}

	inline int& operator() (int x, int y)
	{
		size_t index;
		--x;
		--y;

		if (x > y)
		{
			int tmp = x;
			x = y;
			y = tmp;
		}

		if (y < w) //triangle
			index = (y*(y+1)>>1) + x;
		else //rect
			index = tri + (y-w) * w + x;

		//printf("{[%d,%d]=%ld}", x, y, index);
		return store[index];
	}
} _HalfMatrix;

inline void acceptSourceRect();
inline void acceptPossibleRects();
inline int calculateMax(int x, int y);
inline int fillMatrix();

#ifdef DEBUG
void printMatrix();
#endif

int srcW, srcH;
int requestCount;
HalfMatrix maxMatrix;

int main()
{
	acceptSourceRect();
	maxMatrix = HalfMatrix(srcW, srcH);
	acceptPossibleRects();

	int max = fillMatrix();

	#ifdef DEBUG
	printMatrix();
	#endif

	printf("%d\n", max);

	return 0;
}


inline void acceptSourceRect()
{
	int w, h;
	scanf("%d %d", &w, &h);
	if (w <= h) //Make rectangle vertical
	{ srcW = w; srcH = h;}
	else
	{ srcW = h; srcH = w; }
}

inline void acceptPossibleRects()
{
	scanf("%d", &requestCount);

	int i = 0; int w, h, p;
	while (i < requestCount)
	{
		scanf("%d %d %d", &w, &h, &p);

		if (maxMatrix(w, h) < p)
		{
			maxMatrix(w, h) = p;
			maxMatrix(h, w) = p;
		}

		i++;
	}
}

inline int calculateMax(int posX, int posY)
{
	if (posX == 0 || posY == 0) return 0;

	int bestScore = maxMatrix(posX, posY);

	for (int x = 1; x <= DIV_UP(posX, 2); x++)
		bestScore = MAX(bestScore, maxMatrix(x, posY) + maxMatrix(posX - x, posY));
	for (int y = 1; y <= DIV_UP(posY, 2); y++)
		bestScore = MAX(bestScore, maxMatrix(posX, y) + maxMatrix(posX, posY - y));

	return maxMatrix(posX, posY) = bestScore;
}

inline int fillMatrix()
{
	for (int y = 1; y <= srcH; y++)
	{
		for (int x = 1; x <= y && x <= srcW; x++)
		{
			//printf("%d %d\n", x, y);
			//calculateMax(x, y);
		}
		printf("%d:%d = %d\n", 1, y, calculateMax(1, y));
	}

	return maxMatrix(srcH, srcW);
}



//Debugging
#ifdef DEBUG
void printMatrix()
{
	printf("\n===MATRIX DUMP===\n#   ");

	for (int y = 1; y <= srcH; y++)
		printf(" | %4d", y);
	printf("\n----");
	for (int y = 1; y <= srcH; y++)
		printf("-+-----");
	printf("-\n");

	for (int x = 1; x <= srcW; x++)
	{
		printf("%4d", x);
		for (int y = 1; y <= srcH; y++)
		{
			if (y >= x)
				printf(" | %4d", maxMatrix(x, y));
			else
				printf(" |     ");
		}
		printf("\n");
	}
}
#endif
