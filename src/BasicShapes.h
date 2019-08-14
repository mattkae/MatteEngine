#ifndef BASIC_SHAPES_H
#define BASIC_SHAPES_H
struct Rectangle {
	double x = 0;
	double y = 0;
	double w = 0;
	double h = 0;
};

struct DoublePoint {
	double x = 0;
	double y = 0;
};

inline bool doesIntersectBox(const Rectangle& r, const DoublePoint& p) {
	return p.x >= r.x && p.x <= (r.x + r.w)	&& p.y >= r.y && p.y <= (r.y + r.h);
}
#endif

