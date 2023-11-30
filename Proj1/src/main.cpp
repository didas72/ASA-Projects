#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a > b) ? a : b)

typedef struct Rect
{
	int w, h;
	int price;
	size_t area;

	Rect() : w(0), h(0), price(0), area(0) {}
	Rect(int rw, int rh, int rp) : w(rw), h(rh), price(rp), area(rw*rh) {}
} _Rect;

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
		memset(store, 0xFF, total * sizeof(int)); //set all to -1
	}

	inline int& operator() (int x, int y)
	{
		size_t index;
		x -= 1;//start at index 0
		y -= 1;//start at index 0

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

Rect src;
int requestCount;
Rect *requests;
HalfMatrix maxMatrix;

int main()
{
	acceptSourceRect();
	acceptPossibleRects();

	maxMatrix = HalfMatrix(src.w, src.h);

	fillMatrix();

	int max = calculateMax(src.w, src.h);

	#ifdef DEBUG
	printMatrix();
	#endif

	return 0;
}


inline void acceptSourceRect()
{
	int w, h;
	scanf("%d %d", &w, &h);
	if (w <= h) //Make rectangle vertical
		src = Rect(w, h, 0);
	else
		src = Rect(h, w, 0);
}

inline void acceptPossibleRects()
{
	scanf("%d", &requestCount);

	requestCount <<= 1;
	requests = (Rect*)malloc(requestCount * sizeof(Rect));

	int i = 0; int w, h, p;
	while (i+1 < requestCount)
	{
		scanf("%d %d %d", &w, &h, &p);
		requests[i] = Rect(w, h, p);
		requests[i+1] = Rect(h, w, p);
		i += 2;
	}
}

inline int calculateMax(int x, int y)
{
	int buff = maxMatrix(x, y);
	if (buff != -1)
		return buff;

	int bestScore = 0;
	int bestIndex = -1;

	for (int i = 0; i < requestCount; i++)
	{
		Rect me = requests[i];

		//Check piece fits
		if (me.w > x || me.h > y) continue;

		int score = me.price;
		int new1X, new1Y, new2X, new2Y;

		//Cut this particular piece
		if (me.w < x) //must cut vertically
		{
			new1X = x - me.w;
			new1Y = y;

			if (me.h < y) //must also cut horizontally (after vertical cut)
			{
				new2X = x;
				new2Y = y - me.h;
				score = MAX(score, me.price + calculateMax(new1X, new1Y) + calculateMax(new2X, new2Y));
			}
			else
				score = MAX(score, me.price + calculateMax(new1X, new1Y));
		}
		if (me.h < y) //must cut horizontally
		{
			new1X = x;
			new1Y = y - me.h;

			if (me.w < x) //must also cut vertically (after horizontal cut)
			{
				new2X = x - me.w;
				new2Y = y;
				score = MAX(score, me.price + calculateMax(new1X, new1Y) + calculateMax(new2X, new2Y));
			}
			else
				score = MAX(score, me.price + calculateMax(new1X, new1Y));
		}

		if (score > bestScore)
		{
			bestScore = score;
			bestIndex = i;
		}
	}

	return maxMatrix(x,y) = bestScore;
}

inline int fillMatrix()
{
	for (int y = 1; y <= src.h; y++)
	{
		for (int x = 1; x <= y && x <= src.w; x++)
		{
			calculateMax(x, y);
		}
	}

	return maxMatrix(src.w, src.h);
}



//Debugging
#ifdef DEBUG
void printMatrix()
{
	printf("\n===MATRIX DUMP===\n#   ");

	for (int y = 1; y <= src.h; y++)
		printf(" | %4d", y);
	printf("\n----");
	for (int y = 1; y <= src.h; y++)
		printf("-+-----");
	printf("-\n");

	for (int x = 1; x <= src.w; x++)
	{
		printf("%4d", x);
		for (int y = 1; y <= src.h; y++)
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
