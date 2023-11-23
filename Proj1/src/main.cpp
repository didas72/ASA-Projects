//main.cpp - 

#include <stdio.h>
#include <stdlib.h>

typedef struct Vec2
{
	int x;
	int y;

	Vec2() { x = 0; y = 0; }
	Vec2(int vx, int vy) { x = vx; y = vy; }
	Vec2 operator +(const Vec2& a) const
	{ return Vec2(x + a.x, y + a.y); }
} _Vec2;

typedef struct Rect
{
	Vec2 corner;
	int price;

	//NOTE: Max cost capped at 2^20 ~= 1M (assuming 32-bit int) for fixed point density
	int density;

	Rect() : corner(Vec2(0,0)), price(0), density(0) {}
	Rect(int rw, int rh, int rp) : corner(Vec2(rw, rh)), price(rp), density((rp << 12) / (rw * rh)) {}
} _Rect;

void acceptSourceRect();
void acceptPossibleRects();
void calculateMaxValue();
inline void insertRectSorted(int i, int w, int h, int p);

Rect src;
int requestCount;
Rect *requests;
int totalValue;

int main()
{
	acceptSourceRect();
	acceptPossibleRects();

	calculateMaxValue();

	printf("%d", totalValue);

	return 0;
}


void acceptSourceRect()
{
	int w, h;
	scanf("%d %d", &w, &h);
	src = Rect(w, h, 0);
}

void acceptPossibleRects()
{
	scanf("%d", &requestCount);

	requests = (Rect*)malloc(requestCount * sizeof(Rect));

	int i = 0; int w, h, p;
	while (i <= requestCount)
	{
		scanf("%d %d %d", &w, &h, &p);
		i += 2;
		insertRectSorted(i, w, h, p);
	}
}

inline void insertRectSorted(int i, int w, int h, int p)
{
	//TODO: Maybe input with dlinked list and then compact to array

	Rect norm = Rect(h, w, p);
	Rect rot = Rect(h, w, p);
	int density = norm.density;

	int max = i >> 1, min = 0, c;

	while (max - min > 1)
	{
		c = (max + min) >> 1;

		if (density > requests[c << 1].density)
			min = c;
		else if (density <= requests[c << 1].density) //TODO: Prioritise smaller area
			max = c;
	}

	int index = min << 1;

	for (int j = i; j < index + 2; j -= 2)
	{
		requests[j] = requests[j-2];
		requests[j+1] = requests[j-1];
	}

	requests[index] = norm;
	requests[index+1] = rot;
}

void calculateMaxValue()
{
	
}
