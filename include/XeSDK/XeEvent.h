#pragma once
#include <XeSDK/IDelegate.h>
#include <list>

namespace Xe
{
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