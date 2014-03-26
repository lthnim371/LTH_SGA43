#pragma once

/////////////////////////////////////////////////////
// Point_2 impl
/////////////////////////////////////////////////////
struct Point_2 : public POINT
{
	Point_2(const LONG& _x, const LONG& _y)
	{
		x = _x;
		y = _y;
	}
	Point_2 operator + (const Point_2& o)
	{
		return Point_2(x + o.x, y + o.y);
	}
};

/////////////////////////////////////////////////////
// Bomb impl
/////////////////////////////////////////////////////
#define D2R (3.141592/180)

class Bomb
{
public : 
	Bomb(const Point_2& c, const LONG& r, const LONG& ang)
		: center(c), radius(r), angle(ang), speed(15)
	{}
	void Draw(HDC hdc)
	{
		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);

		POINT dir;
		dir.x = center.x + LONG(2*radius*sin(angle*D2R));
		dir.y = center.y - LONG(2*radius*cos(angle*D2R));

		POINT pt;
		::MoveToEx(hdc, center.x, center.y, &pt);
		::LineTo(hdc, dir.x, dir.y);
	}
	void move()
	{
		center.x = center.x + LONG(speed*sin(angle*D2R));
		center.y = center.y - LONG(speed*cos(angle*D2R));
	}
	Point_2 getCenter() const
	{
		return center;
	}
private :
	Point_2 center;
	LONG radius;
	LONG angle; //각도 변수 만듬
	LONG speed; //이동거리, 속도 개념으로
};
