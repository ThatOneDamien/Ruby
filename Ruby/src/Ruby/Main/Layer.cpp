#include "ruby_pch.h"

#include "Ruby/Main/Layer.h"

namespace Ruby {

	LayerStack::~LayerStack()
	{
		for (size_t i = 0; i < m_Stack.size(); i++)
		{
			m_Stack[i]->onPop();
			delete m_Stack[i];
		}
	}

}