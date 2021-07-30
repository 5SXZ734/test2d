#include "main.h"

#include "stdafx.h"

#include <iostream>
#include "canvas.h"
#include "vector.h"


using namespace My;

class Test1 : public ITest
{
	Obj_t *mpObj;
	std::vector<MeshPoint_t *>	mpt;
	std::vector<MeshLine_t *>	mpl;
public:
	Test1()
		: mpObj(NULL)
	{
	}

	virtual void OnSetup()
	{
		setScreenView(Recti(Pointi(40, 20), Sizei(640, 480)), false);
		Scene &S(newScene());
		S.setWorldView(Rectf(Pointf(-5.0f, -0.3f), Sizef(10.0f, 1.3f)));

		S.addAxes();
		Obj_t &o(S.addObject());
		MeshLine_t &l(o.addLine());
		for (float x(-4.0); x <= 4.0; x += 0.1f)
		{
			float y(float(sin(3.14159*x) / (3.14159*x)));
			l.addPoint(Pointf(x, y));
		}
	}

	virtual bool OnMouse(const Pointf &b)
	{
		if (!mpObj)
			mpObj = &scene().addObject();
		mpt.push_back(mpObj->addPoint(b, 3));
		if (mpt.size() == 2)
		{
			mpl.push_back(mpObj->addLine(mpt[mpt.size() - 2]->at(0), mpt[mpt.size() - 1]->at(0)));
			mpt.clear();
			if (mpl.size() == 2)
			{
				Pointf c;
				if (mpl[mpl.size() - 2]->intersectsAt(mpl[mpl.size() - 1], c))
					mpObj->addPoint(c, 5);
				mpl.clear();
				mpObj = NULL;
			}
		}
		return true;
	}
};


class Test2 : public ITest
{
public:
	Test2()
	{
	}

	virtual void OnSetup()
	{
		setScreenView(Recti(Pointi(100, 100), Sizei(640, 480)));
		Scene &S(newScene());
		S.setWorldView(Rectf(Pointf(-5.0f, -2.0f), Sizef(10.0f, 4.0f)));
		S.addAxes();
		S.addHexSwirl(Pointf(0, 0));
	}

	virtual void OnDraw(HDC hdc)
	{
		Canvas canvas(hdc);
		canvas.setWindow(scene().worldView());
		canvas.setViewport(screenView(), true);
		canvas.drawViewportFrame();
		canvas.draw(scene());
	}

	virtual bool OnKeyDown(WPARAM wParam)
	{
		Rectf w(scene().worldView());
		float step(0.2f);
		float dsize(0.05f);
		switch (wParam)
		{
		case VK_UP: w.translate(0, -step); scene().setWorldView(w); return true;
		case VK_DOWN: w.translate(0, +step); scene().setWorldView(w); return true;
		case VK_LEFT: w.translate(+step, 0); scene().setWorldView(w); return true;
		case VK_RIGHT: w.translate(-step, 0); scene().setWorldView(w); return true;
		case 187: w.shrink(+dsize, +dsize / w.ratio()); scene().setWorldView(w); return true;
		case 189: w.shrink(-dsize, -dsize / w.ratio()); scene().setWorldView(w); return true;
		default:
			break;
		}
		return false;
	}
};

class Test3 : public ITest
{
public:
	Test3()
	{
	}

	virtual void OnSetup()
	{
		setScreenView(Recti(Pointi(100, 100), Sizei(200, 200)));
		for (int i(0); i < 12; i++)
		{
			Scene &S(newScene());
			S.setWorldView(Rectf(Pointf(-3.0f, -3.0f), Sizef(6.0f, 6.0f)));
			S.addHexSwirl(Pointf(0, 0));
		}
	}

	virtual void OnDraw(HDC hdc)
	{
		Canvas a(hdc);
		for (int i(0); i < scenesTotal(); i++)
		{
			a.setWindow(scene(i).worldView());
			Recti vp(screenView());
			vp.translate((i % 4)*vp.width(), (i / 4)*vp.height());
			a.setViewport(vp, true);
			a.draw(scene(i));
		}
	}
};

class Test4 : public ITest
{
public:
	Test4()
	{
	}

	virtual void OnSetup()
	{
		setScreenView(Recti(Pointi(100, 100), Sizei(200, 200)));
		Rectf w(Rectf(Pointf(-1.0f, -1.0f), Sizef(2.0f, 2.0f)));

		Scene &S1(newScene());
		S1.setWorldView(w);
		S1.addRosette(5, 1.0f);

		Scene &S2(newScene());
		S2.setWorldView(w);
		S2.addRosette(11, 1.0f);

		Scene &S3(newScene());
		S3.setWorldView(w);
		S3.addRosette(17, 1.0f);

		Scene &S4(newScene());
		S4.setWorldView(w);
		S4.addArc(Pointf(0, 0), 1, 0, 360);
	}

	virtual void OnDraw(HDC hdc)
	{
		Canvas canvas(hdc);

		for (int i(0); i < scenesTotal(); i++)
		{
			canvas.setWindow(scene(i).worldView());
			Recti vp(screenView());
			vp.translate((i % 3)*vp.width(), (i / 3)*vp.height());
			canvas.setViewport(vp, true);
			canvas.draw(scene(i));
		}
	}
};

class Test5 : public ITest
{
	Obj_t *mpObj;
	std::vector<MeshPoint_t *>	mpt;
	std::vector<MeshLine_t *>	mpl;
public:
	Test5()
	{
	}

	virtual void OnSetup()
	{
		Scene &S(newScene());
		S.setWorldView(Rectf(Pointf(-5.0f, -5.0f), Sizef(10.0f, 10.0f)));
		setScreenView(Recti(Pointi(40, 20), Sizei(640, 480)));

		S.addAxes();
	}

	virtual bool OnMouse(const Pointf &b)
	{
		if (!mpObj)
			mpObj = &scene().addObject();
		mpt.push_back(mpObj->addPoint(b, 3));
		if (mpt.size() == 3)
		{
			Pointf A(mpt[mpt.size() - 3]->at(0));
			Pointf B(mpt[mpt.size() - 2]->at(0));
			Pointf C(mpt[mpt.size() - 1]->at(0));

			Vec2f a(B - A);
			Vec2f b(C - B);
			Vec2f c(A - C);

			Line2f_p L1((A + B) / 2, a.perp());//bisector
			Line2f_p L2((C + A) / 2, c.perp());//bisector

			Pointf X;
			if (intersection(L1, L2, X))
				scene().addCircle(X, (X - A).length());

			mpt.clear();
			mpObj = NULL;
		}
		return true;
	}
};

class Test6 : public ITest
{
	Obj_t *mpObj;
public:
	Test6()
		: mpObj(NULL)
	{
	}

	virtual void OnSetup()
	{
		setScreenView(Recti(Pointi(100, 100), Sizei(640, 480)));
		Scene &S(newScene());
		S.setWorldView(Rectf(Pointf(-5.0f, -2.0f), Sizef(10.0f, 4.0f)));
		S.addAxes();
		mpObj = S.addHexSwirl(Pointf(0, 0));
	}

	virtual void OnDraw(HDC hdc)
	{
		Canvas canvas(hdc);
		canvas.setWindow(scene().worldView());
		canvas.setViewport(screenView(), true);
		canvas.drawViewportFrame();
		canvas.draw(scene());
	}

	virtual bool OnKeyDown(WPARAM wParam)
	{
		Rectf w(scene().worldView());
		float step(0.2f);
		float dsize(0.05f);
		switch (wParam)
		{
		case 0x41: mpObj->translate(-0.1f, 0); return true;//A
		case 0x44: mpObj->translate(0.1f, 0); return true;//D
		case 0x57: mpObj->translate(0, 0.1f); return true;//W
		case 0x53: mpObj->translate(0, -0.1f); return true;//S
		case VK_PRIOR: mpObj->rotate(+2.0f); return true;
		case VK_NEXT: mpObj->rotate(-2.0f); return true;
		case VK_UP: w.translate(0, -step); scene().setWorldView(w); return true;
		case VK_DOWN: w.translate(0, +step); scene().setWorldView(w); return true;
		case VK_LEFT: w.translate(+step, 0); scene().setWorldView(w); return true;
		case VK_RIGHT: w.translate(-step, 0); scene().setWorldView(w); return true;
		case 187: w.shrink(+dsize, +dsize / w.ratio()); scene().setWorldView(w); return true;
		case 189: w.shrink(-dsize, -dsize / w.ratio()); scene().setWorldView(w); return true;
		default:
			break;
		}
		return false;
	}
};

Test6 test;

