#pragma once

#include "Ruby/Event/Event.h"
#include <vector>

namespace Ruby {

	class Layer
	{
	public:
		Layer(const char* name = "Unnamed Layer");
		virtual ~Layer() {}
		virtual void update(double deltaSeconds) {}
		virtual void ImGuiRender() {}
		virtual void onEvent(Event& e) {}
		virtual void onPush() {}
		virtual void onPop() {}
		inline const char* getName() const { return m_Name; }
	private:
		const char* m_Name;
	};

	// Should not be subclassed.
	class LayerStack
	{
	public:
		LayerStack() {}
		~LayerStack();
		
		inline void pushLayer(Layer* layer) 
		{ 
			m_Stack.emplace(m_Stack.begin() + m_LayerInsert, layer); 
			m_LayerInsert++;
		}

		inline void popLayer(Layer* layer) 
		{ 
			auto a = std::find(m_Stack.begin() + m_LayerInsert, m_Stack.end(), layer);
			if (a != m_Stack.end())
			{
				m_Stack.erase(a);
				m_LayerInsert--;
			}
		}

		inline void pushOverlay(Layer* overlay) 
		{
			m_Stack.emplace_back(overlay); 
			m_LayerInsert++; 
		}
		
		inline void popOverlay(Layer* overlay)
		{
			auto a = std::find(m_Stack.begin() + m_LayerInsert, m_Stack.end(), overlay);
			if (a != m_Stack.end())
			{
				m_Stack.erase(a);
				m_LayerInsert--;
			}
		}

		inline size_t size() { return m_Stack.size(); }

		inline Layer& operator [](size_t index)
		{
			RB_ASSERT(index < m_Stack.size(), "Index out of bounds.");
			return *m_Stack[index];
		}

		inline const Layer& operator [](size_t index) const
		{
			RB_ASSERT(index < m_Stack.size(), "Index out of bounds.");
			return *m_Stack[index];
		}

	private:
		std::vector<Layer*> m_Stack;
		size_t m_LayerInsert = 0;
	};

}