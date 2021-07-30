#ifndef __MAIN_H__
#define __MAIN_H__

#include <vector>
#include <windows.h>

#include "canvas.h"
#include "scene.h"

namespace My {

	class ITest
	{
		static ITest *gpSelf;
		std::vector<Scene *>	mSceneList;
		Recti	mv;//screen view area
		bool	mbKeepAspectRatio;
	public:
		ITest()
			: mv(Recti(Pointi(0, 0), Sizei(640, 480))),
			mbKeepAspectRatio(false)
		{
			gpSelf = this;
		}
		virtual ~ITest()
		{
			for (int i(0); i < (int)mSceneList.size(); i++)
				delete mSceneList[i];
			gpSelf = NULL;
		}
		static ITest *self(){ return gpSelf; }
	protected:
		void setScreenView(const Recti &v, bool b = true)
		{
			mv = v;
			mbKeepAspectRatio = b;
		}
		const Recti &screenView(){ return mv; }
		My::Scene &newScene()
		{
			mSceneList.push_back(new Scene());
			return *mSceneList.back();
		}
		int scenesTotal(){ return (int)mSceneList.size(); }
		My::Scene &scene(int i = -1)
		{
			if (i < 0)
				return *mSceneList.back();
			return *mSceneList[i];
		}
	public:
		virtual void OnSetup(){}
		virtual void OnDraw(HDC hdc)
		{
			Canvas canvas(hdc);
			canvas.setWindow(scene().worldView());
			canvas.setViewport(screenView(), mbKeepAspectRatio);
			canvas.drawViewportFrame();
			canvas.draw(scene());
		}
		virtual bool OnKeyDown(WPARAM){ return false; }
		virtual bool OnMouse(const Pointf &){ return false; }
		bool OnMouse(int sx, int sy)
		{
			Canvas canvas(0);
			canvas.setWindow(scene().worldView());
			canvas.setViewport(screenView(), mbKeepAspectRatio);
			Pointf b;
			canvas.v2w(Pointi(sx, sy), b);
			return OnMouse(b);
		}
	};

}//namespace

#endif//__MAIN_H__
