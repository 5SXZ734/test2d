#ifndef __CANVAS_H__
#define __CANVAS_H__

#include <windows.h>
#include <fstream>
#include <vector>
#include <list>
#include <assert.h>

#include "scene.h"

#pragma warning (disable:4996)

namespace My
{
	class Canvas
	{
		HDC    mhDC;
		Recti   mv;//viewport
		Rectf   mw;//world
		Mat3f	mw2v;//world view 2 screen vew mapping
		Mat3f	mv2w;//screen -> world
		Pointf	mCP;//current position in world coords
	public:
		Canvas(HDC hDC)
			: mhDC(hDC)
		{
		}
		~Canvas()
		{
		}
		void setWindow(const Rectf &w){ mw = w; Update_w2v(); }
		void setViewport(const Recti &v, bool bPreserveAspectRatio = false)
		{
			mv = v;
			if (bPreserveAspectRatio)
			{
				Sizei vp(mv.size());
				Rectf w(world());
				float R(w.ratio());
				if (R > vp.w / float(vp.h))
					vp.h = int(vp.w / R);
				else
					vp.w = int(vp.h*R);
				mv.setSize(vp);
			}
			Update_w2v();
		}
		void draw(Scene &sc)
		{
			for (std::list<Obj_t *>::iterator it(sc.objects().begin()); it != sc.objects().end(); it++)
			{
				Obj_t *pObj(*it);
				Mat3f &m(*pObj);
				std::list<Mesh_t *> &meshes(pObj->meshes());
				for (std::list<Mesh_t *>::iterator itm(meshes.begin()); itm != meshes.end(); itm++)
				{
					Mesh_t *pMesh(*itm);
					MeshLine_t *pLine(dynamic_cast<MeshLine_t *>(pMesh));
					if (pLine)
					{
						for (int i(0); i < pLine->size(); i++)
						{
							Pointf &p(pLine->at(i));
							Vec3f a(p[0], p[1], 1.0f);
							Vec3f b(m*a);
							if (i == 0)
								moveTo(Pointf(b[0], b[1]));
							else
								lineTo(Pointf(b[0], b[1]));
						}
					}
					MeshPoint_t *pPoint(dynamic_cast<MeshPoint_t *>(pMesh));
					if (pPoint)
						drawPoint(pPoint->at(0), pPoint->radius());
				}
			}
		}
		void drawPoint(const Pointf &a, float radius)
		{
			//Vec3f b(mw2v*Vec3f(a[0], a[1], 1));
			Pointi b;
			w2v(a, b);
			COLORREF color = RGB(0,0,0);
			if (radius == 0)
				SetPixel(mhDC, int(b[0]), int(b[1]), color);
			else
				Ellipse(mhDC, int(b[0]-radius), int(b[1]-radius), int(b[0]+radius), int(b[1]+radius));
		}
		void moveTo(const Pointf &a)
		{
			Pointi b;//(mw2v*a);
			w2v(a, b);
			MoveToEx(mhDC, int(b[0]), int(b[1]), NULL);
			mCP = a;
		}
		void lineTo(const Pointf &a)
		{
			Pointx<float> first(mCP, mw);
			Pointx<float> second(a, mw);
			if (mw.clipSegment(first, second))
			{
				moveTo(first);
				Pointi b;//(mw2v*second);
				w2v(second, b);
				LineTo(mhDC, b[0], b[1]);
			}
			moveTo(a);
		}
		void drawRect(const Rectf &r)
		{
			Pointf a(r.a), b(r.b);
			moveTo(a);
			lineTo(Pointf(b[0], a[1]));
			lineTo(b);
			lineTo(Pointf(a[0], b[1]));
			lineTo(a);
		}

		Rectf world(){ return mw; }
		Recti viewport(){ return mv; }

		void drawViewportFrame()
		{
			Recti v(viewport());
			MoveToEx(mhDC, mv.a[0], mv.a[1], NULL);
			LineTo(mhDC, mv.b[0], mv.a[1]);
			LineTo(mhDC, mv.b[0], mv.b[1]);
			LineTo(mhDC, mv.a[0], mv.b[1]);
			LineTo(mhDC, mv.a[0], mv.a[1]);
		}

		

	private:
		void Update_w2v()
		{
			mw2v.clear();
			mw2v[0][0] = mv.width() / mw.width();
			mw2v[0][2] = mv.a[0] - mw2v[0][0] * mw.a[0];
			mw2v[1][1] = -mv.height() / mw.height();
			mw2v[1][2] = mv.b[1] - mw2v[1][1] * mw.a[1];
			mw2v[2][2] = 1;
			//inverted
			Rectf w(mw);
			Recti v(mv);
			mv2w.clear();
			mv2w[0][0] = (w.b[0]-w.a[0])/(v.b[0]-v.a[0]);
			mv2w[0][2] = w.a[0]-mv2w[0][0]*v.a[0];
			mv2w[1][1] = (w.b[1]-w.a[1])/(v.a[1]-v.b[1]);
			mv2w[1][2] = w.a[1]-mv2w[1][1]*v.b[1];
			mv2w[2][2] = 1;
		}
	public:
		void w2v(const Pointf &a, Pointi &b)
		{
			Vec3f v;
			v[0] = a[0];
			v[1] = a[1];
			v[2] = 1;
			Vec3f w(mw2v*v);
			b[0] = int(w[0]);
			b[1] = int(w[1]);
		}
		void v2w(const Pointi &a, Pointf &b)
		{
			Vec3f v;
			v[0] = float(a[0]);
			v[1] = float(a[1]);
			v[2] = 1.0f;
			Vec3f w(mv2w*v);
			b[0] = w[0];
			b[1] = w[1];
		}

	};



}//namespace My


#endif//__CANVAS_H__
