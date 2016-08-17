#include "pch.h"
#include <XeSDK.h>

using namespace Xe;

class MyView : public Xe::Core::IView {
	Xe::Timer prev;
	double min, max, avg;
	int frame;
public:
	bool Query(IObject** obj, UID id);

	bool OnInitialize();
	void OnDestroy();
	void OnRun();
	void OnDraw();
	void OnSuspend();
	void OnResume();

	bool OnClosing(bool forced);
	void OnFocusGot();
	void OnFocusLost();
	void OnVisibilityChanged(bool visible);
	void OnSizeChanged(const Graphics::Size& size);
	void OnOrientationChanged(Xe::Graphics::Orientation orientation);
	void OnDpiChanged(float dpi);

	void OnCharacter(const IO::CharacterEvent& e);
	void OnKeyPressed(const IO::KeyboardEvent& e);
	void OnKeyReleased(const IO::KeyboardEvent& e);

	void OnPointerMoved(const IO::PointerEvent& e);
	void OnPointerPressed(const IO::PointerEvent& e);
	void OnPointerReleased(const IO::PointerEvent& e);
	void OnPointerEnter(const IO::PointerEvent& e);
	void OnPointerLeave(const IO::PointerEvent& e);
	void OnPointerWheel(const Math::Vector2<float>& wheel);
};

bool MyView::Query(IObject** obj, UID id) {
	switch (id) {
	case Core::IView::ID:
	case IObject::ID:
		AddRef();
		*obj = this;
		return true;
	}
	*obj = nullptr;
	return false;
}
bool MyView::OnInitialize() { return true; }
void MyView::OnDestroy() {}
void MyView::OnRun()
{
	min = 99999.0;
	max = 0.0;
	avg = 0.0f;
	frame = 1;
	while (true) {
		DispatchEvents(Xe::Core::DispatchType_One);
		OnDraw();
	}
	DispatchEvents(Xe::Core::DispatchType_WaitUntilQuit);
}
void MyView::OnDraw() {
	Timer cur = Timer::Current();
	double deltaTime = Timer(cur - prev).AsDouble();
	prev = cur;

	if (min > deltaTime) min = deltaTime;
	if (max < deltaTime) max = deltaTime;
	avg = avg + (deltaTime - avg) / frame++;
	LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Generic, _T("CUR %f MIN %f MAX %f AVG %f"), deltaTime, min, max, avg);
}
void MyView::OnSuspend() {}
void MyView::OnResume() {}
bool MyView::OnClosing(bool forced) { return true; }
void MyView::OnFocusGot() {}
void MyView::OnFocusLost() {}
void MyView::OnVisibilityChanged(bool visibility) {}
void MyView::OnSizeChanged(const Xe::Graphics::Size& size) {}
void MyView::OnOrientationChanged(Xe::Graphics::Orientation orientation) {}
void MyView::OnDpiChanged(float scale) {}
void MyView::OnCharacter(const Xe::IO::CharacterEvent& e) {}
void MyView::OnKeyPressed(const IO::KeyboardEvent& e) {}
void MyView::OnKeyReleased(const IO::KeyboardEvent& e) {}
void MyView::OnPointerMoved(const IO::PointerEvent& e) {}
void MyView::OnPointerPressed(const IO::PointerEvent& e) {}
void MyView::OnPointerReleased(const IO::PointerEvent& e) {}
void MyView::OnPointerEnter(const IO::PointerEvent& e) {}
void MyView::OnPointerLeave(const IO::PointerEvent& e) {}
void MyView::OnPointerWheel(const Math::Vector2<float>& wheel) {}

namespace Xe {
	namespace Core {
		void Main() {
			IView *pCoreView = new MyView;
			if (pCoreView) IView::Run(pCoreView);
		}
	}
}