#pragma once

#include <memory>
#include <stack>
#include <mutex>

#include "../window/Window.h"
#include "States.hpp"
#include "../graphics/Text.h"

namespace gr
{

	typedef std::unique_ptr<States> StatesRef;

	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();
		void initLoadingScreen();

		void AddState(StatesRef newState, bool isReplacing = true);
		void RemoveState();

		void ProcessChanges(gr::Window& win, std::mutex& mutex);

		StatesRef &GetActiveState();

	private:
		std::stack<StatesRef> _states;
		StatesRef _newState;
		Text* loading_text;

		bool _isAdding;
		bool _isReplacing;
		bool _isRemoving;
	};
}
