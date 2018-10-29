#pragma once
#include <XeSDK/IDelegate.h>
#include <list>

namespace Xe
{
	template <typename TEventHandler>
	class EventHandler
	{
		std::list<TEventHandler*> m_Handlers;

	public:
		const EventHandler& operator += (TEventHandler* handler)
		{
			m_Handlers.push_back(handler);
			return *this;
		}

		const EventHandler& operator -= (TEventHandler* handler)
		{
			m_Handlers.remove(handler);
			return *this;
		}

		void Clear()
		{
			m_Handlers.clear();
		}

		template <class TArgs>
		void operator()(TArgs args) const
		{
			for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
			{
				(**it)(args);
			}
		}
	};

	template <typename TArgs>
	class Event
	{
		std::list<Xe::IDelegate<TArgs>*> m_Delegates;

	public:
		const Event& operator += (IDelegate<TArgs>* mydelegate)
		{
			m_Delegates.push_back(mydelegate);
			return *this;
		}

		const Event& operator -= (IDelegate<TArgs>* mydelegate)
		{
			m_Delegates.remove(mydelegate);
			return *this;
		}

		void Clear()
		{
			m_Handlers.clear();
		}

		void operator()(TArgs args) const
		{
			for (auto it = m_Delegates.begin();
				it != m_Delegates.end(); ++it)
			{
				(**it)(args);
			}
		}
	};

	template <typename TArgs>
	class EventRef
	{
		std::list<Xe::IDelegateRef<TArgs>*> m_Delegates;

	public:
		~EventRef()
		{
			for (auto it = m_Delegates.begin();
				it != m_Delegates.end(); ++it)
			{
				(*it)->Release();
			}

			m_Delegates.clear();
		}

		const EventRef& operator += (IDelegateRef<TArgs>* mydelegate)
		{
			mydelegate->AddRef();
			m_Delegates.push_back(mydelegate);
			return *this;
		}

		const EventRef& operator -= (IDelegateRef<TArgs>* mydelegate)
		{
			m_Delegates.remove(mydelegate);
			mydelegate->Release();
			return *this;
		}

		void operator()(TArgs args) const
		{
			for (auto it = m_Delegates.begin();
				it != m_Delegates.end(); ++it)
			{
				(**it)(args);
			}
		}
	};
}