#ifndef __SCENE_H__
#define __SCENE_H__

#include <math.h>
#include <list>
#include "vector.h"



namespace My
{

	class Mesh_t
	{
	protected:
		std::vector<Pointf> v;//vertex list
	public:
		Mesh_t()
		{
		}
		virtual ~Mesh_t(){}
		virtual int size(){ return v.size(); }
		virtual Pointf &at(int i){ return v[i]; }
		void addPoint(const Pointf &a){ v.push_back(a); }
	};


	class MeshPoint_t : public Mesh_t
	{
		float	mfRadius;
	public:
		MeshPoint_t(const Pointf &_a, float _radius = 0)
			: mfRadius(_radius)
		{
			addPoint(_a);
		}
		float radius(){ return mfRadius; }
	};

	class MeshLine_t : public Mesh_t
	{
	public:
		MeshLine_t(){}
		MeshLine_t(const Pointf &a, const Pointf &b)
		{
			addPoint(a);
			addPoint(b);
		}
		void initNGon(int n, const Pointf &c, float radius, float rotAngle)
		{
			assert(n > 2);
			v.clear();
			double angle(rotAngle*M_PI / 180);
			double angleInc(2 * M_PI / n);
			for (int k(0); k <= n; k++, angle += angleInc)
			{
				Pointf b(float(cos(angle)), float(sin(angle)));
				addPoint(c + b*radius);
			}
		}
		bool intersectsAt(MeshLine_t *p, Pointf &I)
		{
			Line2f_p L1(v[0], v[1] - v[0]);
			Line2f_p L2(p->at(0), p->at(1) - p->at(0));
			return intersection(L1, L2, I, 1);
		}
	};

	class Obj_t : public Mat3f
	{
		std::list<Obj_t *> mChilds;
		std::list<Mesh_t *>	mMeshes;
	public:
		Obj_t()
		{
			loadIdentity();
		}
		virtual ~Obj_t(){}
		std::list<Mesh_t *> &meshes(){ return mMeshes; }
		MeshLine_t &addLine()
		{
			MeshLine_t *p(new MeshLine_t());
			mMeshes.push_back(p);
			return *p;
		}
		MeshLine_t *addLine(const Pointf &a, const Pointf &b)
		{
			MeshLine_t *p(new MeshLine_t(a, b));
			mMeshes.push_back(p);
			return p;
		}
		MeshPoint_t *addPoint(const Pointf &a, float radius)
		{
			MeshPoint_t *p(new MeshPoint_t(a, radius));
			mMeshes.push_back(p);
			return p;
		}
		void translate(float dx, float dy)
		{
			Mat3f &m(*this);
			m[0][2] += dx;
			m[1][2] += dy;
		}
		void rotate(float deg)
		{
			Mat3f &m(*this);
			Mat3f m1(m);
			m1.translate(-m[0][2], -m[1][2]);
			Mat3f m2;
			m2.loadIdentity();
			m2.rotate(deg);
			Mat3f m3(m1*m2);
			m3.translate(m[0][2], m[1][2]);
			m = m3;
		}
	};


	class Scene
	{
	public:
		class MyObjects :public std::list<Obj_t *>
		{
		public:
			MyObjects()
			{
			}
		};
	private:
		Rectf mw;//world view
		MyObjects	mObj;
	public:
		Scene()
			: mw(Pointf(-1.0f, -1.0f), Sizef(1.0f, 1.0f))
		{
		}
		~Scene()
		{
			for (std::list<Obj_t *>::iterator i(mObj.begin()); i != mObj.end(); i++)
				delete *i;
		}
		void setWorldView(const Rectf &w){ mw = w; }
		const Rectf &worldView(){ return mw; }
		Obj_t &addObject()
		{
			Obj_t *pObj(new Obj_t());
			mObj.push_back(pObj);
			return *pObj;
		}

		MyObjects &objects(){ return mObj; }
		void addAxes()
		{
			Obj_t &o(addObject());
			o.addLine(Pointf(-100, 0), Pointf(100, 0));
			o.addLine(Pointf(0, -100), Pointf(0, 100));
		}
		Obj_t *addHexSwirl(const Pointf &cp)
		{
			float radius(0.2f);
			float rotAngle(0);
			Obj_t &o(addObject());
			for (int i(0); i < 20; i++)
			{
				MeshLine_t &l(o.addLine());
				l.initNGon(6, cp, radius, rotAngle);
				radius += 0.2f;
				rotAngle += 10.0f;
			}
			return &o;
		}
		bool dissectCircle(std::vector<Pointf> &a)
		{
			int n((int)a.size());
			if (n < 3) return false;
			double angle(0);
			double angleInc(2*M_PI/n);
			for (int i(0); i < n; i++)
			{
				a[i] = Pointf(float(cos(angle)), float(sin(angle)));
				angle += angleInc;
			}
			return true;
		}
		void addRosette(int n, float radius)
		{
			std::vector<Pointf> a(n);
			dissectCircle(a);
			Obj_t &o(addObject());
			for (unsigned i(0); i < a.size()-1; i++)
				for (unsigned j(i+1); j < a.size(); j++)
					o.addLine(a[i], a[j]);
		}
		void addArc(const Pointf &center, float radius, float startAngle, float sweep)
		{
			const int n(30);
			float angle(DEG2RAD(startAngle));
			float angleInc(DEG2RAD(sweep)/n);
			Pointf cp(center);
			Obj_t &o(addObject());
			MeshLine_t &l(o.addLine());
			for (int k(0); k <= n; k++, angle += angleInc)
			{
				Pointf b(cos(angle), sin(angle));
				l.addPoint(cp+b*radius);
			}
		}
		void addCircle(const Pointf &cp, float radius)
		{
			addArc(cp, radius, 0, 360);
		}

	};

}//namespace



#endif//__SCENE_H__
