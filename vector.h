#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <vector>
#include <assert.h>

#define DEG2RAD(a)	float(a*M_PI/180)

namespace My
{
	template <typename T> struct Point;

	template <typename T, int SIZE>
	struct Vector
	{
		T m[SIZE];
		Vector(){}
/*		Vector(const Vector<T,2> &p)
		{
			m[0] = p[0];
			m[1] = p[1];
			m[2] = 1;
		}
*/		Vector(T x, T y){ m[0] = x; m[1] = y; }
		Vector(T x, T y, T z){ m[0] = x; m[1] = y; m[2] = z; }
		void clear()
		{
			for (int i(0); i < SIZE; i++)
				m[i] = 0;
		}
		int size() const { return SIZE; }
		T &operator [](const int i){ return m[i]; }
		const T &operator [](const int i) const { return m[i]; }
		void print(std::ostream &os)
		{
			os << "[";
			for (int i(0); i < SIZE; i++)
			{
				if (i > 0)
					os << ",";
				os << m[i];
			}
			os << "]";
		}
		T length()
		{
			T s(0);
			for (int i(0); i < SIZE; i++)
				s += m[i]*m[i];
			return (T)sqrt(s);
		}
		Vector perp(){ return Vector(-m[1], m[0]); }
		T dot(const Vector &o)
		{
			Vector t;
			for (int i(0); i < SIZE; i++)
				t[i] = m[i]*o[i];
			return t.sum();
		}
		T sum()
		{
			T s(0);
			for (int i(0); i < SIZE; i++)
				s += m[i];
			return s;
		}
		Vector operator *(T a) const
		{
			Vector t;
			for (int i(0); i < SIZE; i++)
				t[i] = m[i]*a;
			return t;
		}
		Vector operator /(T a) const
		{
			Vector t;
			for (int i(0); i < SIZE; i++)
				t[i] = m[i]/a;
			return t;
		}
		Vector operator +(const Vector &o) const
		{ 
			Vector t;
			for (int i(0); i < SIZE; i++)
				t[i] = m[i] + o[i];
			return t;
		}
		Vector operator -(const Vector &o) const
		{
			Vector t;
			for (int i(0); i < SIZE; i++)
				t[i] = m[i]-o[i];
			return t;
		}
		void translate(T dx, T dy){ m[0] += dx; m[1] += dy; }
	};

	template <typename T, int SIZE>
	class Matrix
	{
		T m[SIZE][SIZE];
	public:
		Matrix(){}
		void clear()
		{
			for (int i(0); i < SIZE; i++)
			for (int j(0); j < SIZE; j++)
				m[i][j] = 0;
		}
		int size(){ return SIZE; }
		void loadIdentity()
		{
			for (int i(0); i < SIZE; i++)
			for (int j(0); j < SIZE; j++)
			if (i == j)
				m[i][j] = 1;
			else
				m[i][j] = 0;
		}
		void print(std::ostream &os)
		{
			for (int i(0); i < ROWS; i++)
			{
				os << "[";
				for (int j(0); j < COLS; j++)
				{
					if (j > 0)
						os << ",";
					os << m[i][j];
				}
				os << "]";
			}

		}
		Matrix operator *(const Matrix &x)
		{
			Matrix &m(*this);
			Matrix t;
			t.clear();
			for (int j(0); j < SIZE; j++)
			{
				for (int i(0); i < SIZE; i++)
				{
					for (int k(0); k < SIZE; k++)
					{
						t[i][j] += m[i][k] * x[k][j];
					}
				}
			}
			return t;
		}
		Vector<T, SIZE> operator *(const Vector<T, SIZE> &x) const
		{
			Vector<T, SIZE> t;
			t.clear();
			for (int i(0); i < SIZE; i++)
			for (int j(0); j < SIZE; j++)
				t[i] += m[i][j] * x[j];
			return t;
		}
		T (&operator [](const int i))[SIZE]{ return m[i]; }
		const T (&operator [](const int i)const)  [SIZE]{ return m[i]; }
		void translate(T dx, T dy)
		{
			m[0][SIZE - 1] = dx;
			m[1][SIZE - 1] = dy;
		}
		void rotate(float deg)
		{
			m[0][0] = cos(DEG2RAD(deg));
			m[0][1] = -sin(DEG2RAD(deg));
			m[1][0] = sin(DEG2RAD(deg));
			m[1][1] = cos(DEG2RAD(deg));
		}
	};
	
	typedef Matrix<float, 3>	Mat3f;
	typedef Vector<float, 3>	Vec3f;
	typedef Vector<int,2>		Vec2i;
	typedef Vector<float,2>		Vec2f;



/*	template <typename T>
	struct Point
	{
		T x, y;
		Point() : x(0), y(0){}
		Point(T _x, T _y) : x(_x), y(_y){}
		Point(const Vector<T, 3> &v){ x = v[0]; y = v[1]; }
		void translate(T dx, T dy){ x += dx; y += dy; }
		Point operator *(T a) const { return Point(x*a, y*a); }
		Point operator +(const Point &a) const { return Point(x+a.x, y+a.y); }
	};*/



	template <typename T> struct Rect;


	typedef Vector<int,2>		Pointi;
	typedef Vector<float,2>		Pointf;


	template <typename T>
	struct Pointx : public Vector<T,2>
	{
		unsigned char u;//code
		Pointx() : u(0){}
		Pointx(const Vector<T,2> &p, const Rect<T> &r);
		void update(const Rect<T> &r);
	};
	template <typename T>
	struct Size
	{
		T w, h;
		Size() : w(0), h(0){}
		Size(T _w, T _h) : w(_w), h(_h){}
	};



	template <typename T>
	struct Rect
	{
		typedef Vector<T,2>	Point;
		Point    a, b;
		Rect() : a(0, 0), b(0, 0){}
		Rect(const Point &_a, const Point &_b) : a(_a), b(_b){}
		Rect(const Point &p, const Size<T> &s) : a(p),
			b(p[0] + s.w, p[1] + s.h){}
		T width() const { return b[0] - a[0]; }
		T height() const { return b[1] - a[1]; }
		Size<T> size() const { return Size<T>(width(), height()); }
		void setSize(const Size<T> &s){ b[0] = a[0] + s.w; b[1] = a[1] + s.h; }
		T ratio(){ return width() / height(); }
		void translate(T dx, T dy){ a.translate(dx, dy); b.translate(dx, dy); }
		void shrink(T dx, T dy){ a[0] += dx; b[0] -= dx; a[1] += dy; b[1] -= dy; }
		void flip(){ Point<T> t(a); a = b; b = t; }
	private:
		void chop(Pointx<T> &p, const Point &q)
		{
			T delx(q[0] - p[0]);
			T dely(q[1] - p[1]);
			if (p.u & 8)//to the left
			{
				p[1] += (a[0] - p[0])*dely / delx;
				p[0] = a[0];
			}
			else if (p.u & 2)//to the right
			{
				p[1] += (b[0] - p[0])*dely / delx;
				p[0] = b[0];
			}
			else if (p.u & 1)//below
			{
				p[0] += (a[1] - p[1])*delx / dely;
				p[1] = a[1];
			}
			else if (p.u & 4)//above
			{
				p[0] += (b[1] - p[1])*delx / dely;
				p[1] = b[1];
			}
		}
	public:
		int clipSegment(Pointx<T> &p1, Pointx<T> &p2)//by Cohen-Sutherland
		{
			for (;;)
			{
				if (!(p1.u | p2.u)) return 1;//trivial accept
				if ((p1.u & p2.u)) return 0;//trivial reject
				if (p1.u)//p1 is outside
				{
					chop(p1, p2);
					p1.update(*this);
				}
				else//p2 is outside
				{
					chop(p2, p1);
					p2.update(*this);
				}
			}
		}
	};

	template <typename T>
	Pointx<T>::Pointx(const Vector<T,2> &p, const Rect<T> &r) : Vector<T,2>(p)
	{
		update(r);
	}

	template <typename T>
	void Pointx<T>::update(const Rect<T> &r)
	{
		u = 0;
		if (m[0] < r.a[0]) u |= 8;
		if (m[1] > r.b[1]) u |= 4;
		if (m[0] > r.b[0]) u |= 2;
		if (m[1] < r.a[1]) u |= 1;
	}

	typedef    Rect<int>		Recti;
	typedef    Rect<float>		Rectf;
	typedef    Size<int>		Sizei;
	typedef    Size<float>		Sizef;

	struct Line2f_p//parametric
	{
		Pointf	a;
		Vec2f	b;
		Line2f_p(const Pointf &_a, const Vec2f &_b)
			: a(_a), b(_b)
		{
		}
		const Pointf &A() const { return a; }
		const Vec2f &B() const { return b; }
	};


	inline bool intersection(const Line2f_p &L1, const Line2f_p &L2, Pointf &I, int flag = 0)//0:check lines, 1:check segments
	{
		Pointf A(L1.A());
		Vec2f b(L1.B());
		Pointf C(L2.A());
		Vec2f d(L2.B());
		Vec2f c(C - A);
		float denom(d.perp().dot(b));
		if (denom != 0)
		{
			float t(d.perp().dot(c) / denom);
			float u(b.perp().dot(c) / denom);
			if (flag > 0)
			{
				if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
				{
					I = A + b*t;
					return true;
				}
			}
			else
			{
				I = A + b*t;
				return true;
			}
		}
		return false;
	}

}//namespace

#endif//__VECTOR_H__
