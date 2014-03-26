#pragma once

/////////////////////////////////////////////////////
// Point impl
/////////////////////////////////////////////////////
struct Point : public POINT
{
	Point(const LONG& _x, const LONG& _y)
	{
		x = _x;
		y = _y;
	}
	Point operator + (const Point& o)
	{
		return Point(x + o.x, y + o.y);
	}
};

/////////////////////////////////////////////////////
// Circle impl
/////////////////////////////////////////////////////
#define D2R (3.141592/180)

class Circle
{
public : 
	Circle(const Point& c, const LONG& r)
		: center(c), radius(r), angle(0), speed(5)
	{}
	void Draw(HDC hdc)
	{
		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);

		POINT dir;
		dir.x = center.x + 2*radius*sin(angle*D2R);
		dir.y = center.y - 2*radius*cos(angle*D2R);

		POINT pt;
		::MoveToEx(hdc, center.x, center.y, &pt);
		::LineTo(hdc, dir.x, dir.y);
	}
	void move(bool bForward = true) //���� ������ �����ϰ���
	{
		if (bForward)
		{
			center.x = center.x + speed*sin(angle*D2R);
			center.y = center.y - speed*cos(angle*D2R);
		}
		else
		{
			center.x = center.x - speed*sin(angle*D2R);
			center.y = center.y + speed*cos(angle*D2R);
		}
	}
	void turn(const LONG& diff) //angle�� ����
	{
		angle = angle + diff;
	}
	void setCenter(const Point& c)
	{
		center = c;
	}
	Point getCenter() const
	{
		return center;
	}
	operator Point ()
	{
		return center;
	}
	Circle operator + (const Point& o)
	{
		return Circle(Point(center.x + o.x, center.y + o.y), radius);
	}
private :
	Point center;
	LONG radius;
	LONG angle; //���� ���� ����
	LONG speed; //�̵��Ÿ�, �ӵ� ��������
};
