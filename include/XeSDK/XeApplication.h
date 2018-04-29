#pragma once
#include <XeSDK/IApplication.h>
#include <XeSDK/IGraphicsContext.h>

namespace Xe {
	struct AppProperties {
		//! \brief Specify the type of time step to update the application.
		enum TimestepType {
			//! \brief Acutal deltaTime is passed.
			TimestepType_Dynamic,
			//! \brief Specify a maximum value for deltaTime.
			TimestepType_SemiFixed,
			//! \brief Specify a fixed deltaTime.
			TimestepType_Fixed,
		};

		//! \brief Specify how to control the event dispatcher of the OS.
		/** \details When OnUpdate event finish the control is passed to the
		 * operating system, that has the task to process all the events in
		 * queue like window management, pointer and keyboard. After the OS
		 * event dispatcher the control returns to the app, calling OnDraw
		 * and OnUpdate when needed. This enum helps to decide how to manage
		 * the operating system control.
		 * To manually dispatch an event, see Graphics::IContext::Process.
		 */
		enum DispatchControl {
			//! \brief Exit from the loop.
			/** \details Do not process any other event and exit immediately.
			 * No other events than OnDestroy will be called.
			 */
			DispatchControl_Exit,
			//! \brief Do not dispatch any event.
			/** \details This is the faster option, but can cause the OS or its
			 * window to be froze. Because of this we reccomend to not dispatch
			 * events in queue 
			 */
			DispatchControl_None,
			//! \brief Dispatch one event then return immediately.
			/** \details This is a good compromise between None and All.
			 * If the events arrives faster than the deltaTime then the
			 * latency to process them will be very high. It's reccomended to
			 * not use this on heavy application or when VBlank is evaluated.
			 */
			DispatchControl_One,
			//! \brief Dispatch all events then return immediately.
			/** \details This is the most common use of the event dispatcher.
			 * There are no risk and the events processing latency is reduces
			 * to the minimum.
			 */
			DispatchControl_All,
			//! \brief Dispatch all events on queue then wait until a new one.
			/** \details This can be used to those application that do use of
			 * rendering on demand. The update and drawing are called only when
			 * needed, safing CPU clocks and improve power compsum.
			 */
			DispatchControl_Wait,
		};

		//! \brief Type of time-step
		TimestepType TimestepType;

		//! \brief Specify the time step value
		float Timestep;

		//! \brief Specify a maximum value of deltaTime
		/** \details In some cases deltaTime can be so high that values
		* calculated by OnUpdate may not be correct; also, when a time
		* step is used, there's the risk to be stuck on a spyral when
		* the time to compute OnUpdate and OnDraw is greater than the
		* deltaTime in each frame.
		*/
		float MaxDeltaTime;

		AppProperties() :
			TimestepType(TimestepType_Dynamic),
			Timestep(0.01f),
			MaxDeltaTime(1.0f) {
		}
	};
	class Application : public IApplication {
		bool m_Invalidated;
	public:
		static const UID ID = 0xDEF439D445A04B6AULL;

		Xe::Graphics::IContext *m_pContext;

		Application();
		//! \brief Check validation state
		bool IsValidated() const;
		//! \brief Call this when needed to call OnDraw event.
		void Invalidate();
		//! \brief Call this when content does not needed to be drawn.
		void Flush();

		virtual void OnContextInitialize(Graphics::ContextProperties& properties) = 0;

		virtual void OnKeybDown(const Key &key);
		virtual void OnKeybUp(const Key &key);
		virtual void OnKeybChar(const Key &key, uint ch);

		virtual void OnPointerMove(const IO::PointerEvent& e);
		virtual void OnPointerPressed(const IO::PointerEvent& e);
		virtual void OnPointerReleased(const IO::PointerEvent& e);
		virtual void OnPointerEnter();
		virtual void OnPointerLeave();
		virtual void OnPointerWheel(const Math::Vector2<float>& wheel);

		virtual void OnSizeChanged(const Math::Vector2<svar> &size);
		virtual void OnLostFocus();
		virtual void OnGotFocus();
		virtual void OnSuspend();
		virtual void OnResume();
		virtual bool OnClosing(bool forced);
	};
	class AppLoop : public IObject {
	public:
		static const UID ID = 0x1f59cc4d0f114a84ULL;

		static bool Create(AppLoop **, Application* app, const AppProperties& properties = AppProperties());
		virtual bool Process(const AppProperties &properties) = 0;
	};

	//! \brief Initialize and run in an AppLoop the selected application
	bool Run(Application *app, const AppProperties &properties = AppProperties());
}