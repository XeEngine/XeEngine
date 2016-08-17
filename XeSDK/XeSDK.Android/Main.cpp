#include "pch.h"
#include <XeSDK/XeApplication.h>
#include <XeSDK/XeMemory.h>
#include <XeSDK/XeGraphics.h>
#include <XeSDK/XeTimer.h>

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#include <EGL/eglplatform.h>

enum AndroidEvent {
	AndroidEvent_Start,
	AndroidEvent_Resume,
	AndroidEvent_SaveInstanceState,
	AndroidEvent_Pause,
	AndroidEvent_Stop,
	AndroidEvent_Destroy,
	AndroidEvent_WindowFocusChanged,
	AndroidEvent_NativeWindowCreated,
	AndroidEvent_NativeWindowResized,
	AndroidEvent_NativeWindowRedrawNeeded,
	AndroidEvent_NativeWindowDestroyed,
	AndroidEvent_InputQueueCreated,
	AndroidEvent_InputQueueDestroyed,
	AndroidEvent_ContentRectChanged,
	AndroidEvent_ConfigurationChanged,
	AndroidEvent_LowMemory,
};

// WHEN APP IS OPENED
// OnStart
// OnResume
// onInputQueueCreated
// onNativeWindowCreated
// onNativeWindowResized
// onContentRectChanged
// onWindowFocusChanged
// onNativeWindowRedrawNeeded
// 
// WHEN APP IS REDUCED AS ICON
// OnPause
// onNativeWindowDestroyed
// onSaveInstanceState
// OnStop

template <typename T>
class Fifo {
	svar max;
	svar start;
	svar count;
	T *fifo;
public:
	Fifo(svar capacity) :
		max(capacity),
		start(0),
		count(0),
		fifo(new T[capacity]) {
	}
	~Fifo() {
		delete[] fifo;
	}
	svar Count() const {
		return count;
	}
	bool Push(T value) {
		if (count >= max)
			return true;
		fifo[(start + count) % max] = value;
		count++;
		return true;
	}
	T Pop() {
		T value = fifo[start];
		start = (start + 1) % max;
		return value;
	}
};

class Mutex {
	pthread_mutex_t m_Mutex;
	pthread_cond_t m_Cond;
public:
	Mutex() {
		pthread_mutex_init(&m_Mutex, NULL);
		pthread_cond_init(&m_Cond, NULL);
	}
	~Mutex() {
		pthread_mutex_destroy(&m_Mutex);
		pthread_cond_destroy(&m_Cond);
	}
	void Lock() {
		pthread_mutex_lock(&m_Mutex);
	}
	void Unlock() {
		pthread_mutex_unlock(&m_Mutex);
	}
	void Wait() {
		pthread_cond_wait(&m_Cond, &m_Mutex);
	}
	void Broadcast() {
		pthread_cond_broadcast(&m_Cond);
	}
};

struct AndroidInstance {
	struct android_poll_source {
		// Identificatore di questa origine. Può essere LOOPER_ID_MAIN o
		// LOOPER_ID_INPUT.
		int32_t id;

		// instanza cui è associata l'identità.
		struct AndroidInstance* instance;

		// Funzione da chiamare per eseguire l'elaborazione standard dei dati da
		// questa origine.
		void(*process)(struct AndroidInstance* instance, struct android_poll_source* source);
	};
	enum LooperId {
		/**
		* ID dei dati Looper di comandi ricevuti dal thread principale dell'app, restituito
		* come identificatore da ALooper_pollOnce(). I dati per questo identificatore
		* corrispondono a un puntatore a una struttura android_poll_source.
		* Possono essere recuperati ed elaborati con android_app_read_cmd()
		* e android_app_exec_cmd().
		*/
		LOOPER_ID_MAIN = 1,

		/**
		* ID dei dati Looper di eventi restituiti dall'oggetto AInputQueue della
		* finestra dell'applicazione, ricevuto come identificatore da
		* ALooper_pollOnce(). I dati per questo identificatore corrispondono a un
		* puntatore a una struttura android_poll_source. Possono essere letti tramite
		* l'oggetto inputQueue di android_app.
		*/
		LOOPER_ID_INPUT = 2,

		/**
		* Inizio degli identificatori ALooper definiti dall'utente.
		*/
		LOOPER_ID_USER = 3,
	};

public:
	ANativeActivity *Activity;
private:
	Mutex m_Mutex;
	Fifo<AndroidEvent> Events;
	bool initialized;
	bool m_IsRunning;
	int m_MsgRead;
	int m_MsgWrite;
	android_poll_source m_CommandPoolSource;
	android_poll_source m_InputPoolSource;
public:
	AConfiguration *Configuration;
	void *StateData;
	size_t StateLength;
	pthread_t thread;
	Xe::IApplication *app;
	bool IsPaused;

	AndroidInstance(ANativeActivity *activity, void *stateData, size_t stateLength) :
		Activity(activity),
		Events(0x400),
		initialized(false),
		m_IsRunning(false),
		StateData(Xe::Memory::Alloc(stateLength)),
		StateLength(stateLength),
		app(nullptr)
	{
		Xe::Memory::Copy(StateData, stateData, StateLength);

		int msgpipe[2];
		if (!pipe(msgpipe)) {
			m_MsgRead = msgpipe[0];
			m_MsgWrite = msgpipe[1];
		}
		m_CommandPoolSource.id = LOOPER_ID_MAIN;
		m_CommandPoolSource.instance = this;
		m_CommandPoolSource.process = process_cmd;
		m_InputPoolSource.id = LOOPER_ID_INPUT;
		m_InputPoolSource.instance = this;
		m_InputPoolSource.process = process_input;
	}
	~AndroidInstance() {
		Xe::Memory::Free(StateData);
	}
	bool IsRunning() const {
		return m_IsRunning;
	}
	svar Count() {
		m_Mutex.Lock();
		svar ret = Events.Count();
		m_Mutex.Unlock();
		return ret;
	}
	bool Push(AndroidEvent value) {
		m_Mutex.Lock();
		bool ret = Events.Push(value);
		m_Mutex.Unlock();
		return ret;
	}
	AndroidEvent Pop() {
		m_Mutex.Lock();
		AndroidEvent ret = Events.Pop();
		m_Mutex.Unlock();
		return ret;
	}
	void WaitUntilQuit() {
		m_Mutex.Lock();
		while (!IsRunning())
			m_Mutex.Wait();
		m_Mutex.Unlock();
	}
	void Initialize() {
		m_Mutex.Lock();
		m_IsRunning = true;
		m_Mutex.Broadcast();
		m_Mutex.Unlock();
	}
};

static ANativeWindow *Xe_Graphics_Window;
namespace Xe {
	namespace Graphics {
		bool GetNativeWindowType(EGLNativeWindowType *window, const Size& size, s32 format) {
			*window = Xe_Graphics_Window;
			ANativeWindow_setBuffersGeometry(Xe_Graphics_Window, 0, 0, format);
			return true;
		}
	}
}

void onStart(ANativeActivity* activity) {
	AndroidInstance *instance = (AndroidInstance*)activity->instance;
	instance->IsPaused = true;
}
void onResume(ANativeActivity* activity) {
	AndroidInstance *instance = (AndroidInstance*)activity->instance;
	if (instance->IsPaused == false) {
		instance->IsPaused = true;
		instance->app->OnResume();
	}
}
void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize) {
	return nullptr;
}
void onPause(ANativeActivity* activity) {
}
void onStop(ANativeActivity* activity) {
}
void onDestroy(ANativeActivity* activity) {
	AndroidInstance *instance = (AndroidInstance*)activity->instance;
	instance->app->Release();
	AConfiguration_delete(instance->Configuration);
	delete instance;
}
void onWindowFocusChanged(ANativeActivity* activity, int hasFocus) {
	AndroidInstance *instance = (AndroidInstance*)activity->instance;
	hasFocus ? instance->app->OnGotFocus() : instance->app->OnLostFocus();
}
void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
	Xe_Graphics_Window = window;
	AndroidInstance *instance = (AndroidInstance*)activity->instance;
	instance->Initialize();
}
void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window) {
	AndroidInstance *instance = (AndroidInstance*)activity->instance;
	instance->app->OnSizeChanged(Xe::Graphics::Size(ANativeWindow_getWidth(window), ANativeWindow_getHeight(window)));
}
void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window) {
}
void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
}
void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
	// AInputQueue_getEvent
}
void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
}
void onContentRectChanged(ANativeActivity* activity, const ARect* rect) {
}
void onConfigurationChanged(ANativeActivity* activity) {
}
void onLowMemory(ANativeActivity* activity) {
}

void* AndroidMain(void* param) {
	AndroidInstance *instance = (AndroidInstance*)param;
	instance->Configuration = AConfiguration_new();
	AConfiguration_fromAssetManager(instance->Configuration, instance->Activity->assetManager);

	Xe::Graphics::ContextProperties p;
	Xe::Application *app;
	CreateApplication(&app);
	instance->app = app;
	app->OnPreInitialize(p);
	Xe::Time tLast;
	if (Xe::Graphics::Create(&app->m_context, p) == Xe::Error::OK) {
		bool isAnimating = true;
		app->OnInitialize();
		while (true) {
			int ident;
			int events;
			//while ((ident = ALooper_pollAll(isAnimating ? 0 : -1, NULL, &events, NULL) >= 0))
			{
				//app->m_context->Process();
				app->OnDraw();
				Xe::Time tCur = Xe::Time();
				u64 diff = tCur.Distance(tLast);
				double deltaTime = diff / 1000000.0;
				tLast = tCur;
				app->OnUpdate(deltaTime);
			}
		}
	}
}

void EatANativeActivity(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	activity->callbacks->onStart = onStart;
	activity->callbacks->onResume = onResume;
	activity->callbacks->onSaveInstanceState = onSaveInstanceState;
	activity->callbacks->onPause = onPause;
	activity->callbacks->onStop = onStop;
	activity->callbacks->onDestroy = onDestroy;
	activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	activity->callbacks->onNativeWindowResized = onNativeWindowResized;
	activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
	activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	activity->callbacks->onInputQueueCreated = onInputQueueCreated;
	activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
	activity->callbacks->onContentRectChanged = onContentRectChanged;
	activity->callbacks->onConfigurationChanged = onConfigurationChanged;
	activity->callbacks->onLowMemory = onLowMemory;

	AndroidInstance* instance = new AndroidInstance(activity, savedState, savedStateSize);
	activity->instance = instance;

	// Crea il thread di entry point.
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&instance->thread, &attr, AndroidMain, instance);
	instance->WaitUntilQuit();
	delete instance;
}