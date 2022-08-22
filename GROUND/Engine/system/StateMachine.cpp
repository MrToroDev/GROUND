#include "StateMachine.h"
#include <thread>
#include <memory>
#include "../GR_cross_definitions.h"

using namespace gr;

StateMachine::StateMachine()
{
    
}

StateMachine::~StateMachine()
{
    // delete loading_text;
}

void StateMachine::initLoadingScreen()
{
    loading_text = new Text(glm::vec2(500, 500), glm::vec2(0, 0), "Loading...", "Assets/fonts/arial.ttf", 1.0);
}

void StateMachine::AddState(StatesRef newState, bool isReplacing)
{
    this->_isAdding = true;
    this->_isReplacing = isReplacing;

    this->_newState = std::move(newState);
}

void StateMachine::RemoveState()
{
    this->_isRemoving = true;
}

void StateMachine::ProcessChanges(gr::Window& wn, std::mutex& mutex)
{
    if (this->_isRemoving && !this->_states.empty())
    {
        this->_states.pop();

        if (!this->_states.empty())
        {
            this->_states.top()->Resume();
        }

        this->_isRemoving = false;
    }

    if (this->_isAdding)
    {
        if (!this->_states.empty())
        {
            if (this->_isReplacing)
            {
                this->_states.pop();
            }
            else
            {
                this->_states.top()->Pause();
            }
        }

        this->_states.push(std::move(this->_newState));

        auto startTime = std::chrono::high_resolution_clock::now();

        try {
            _states.top()->init();
        }
        catch (std::exception& e) {
            std::string a = e.what();
            MessageBoxA(NULL, a.c_str(), "GROUND", MB_ICONERROR | MB_OK);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto startMicro = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
        auto endMicro = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        long long int duration = endMicro - startMicro;
        std::string ss = "{LOADING THREAD} Took: ";
        ss.append(std::to_string(duration * 0.001));
        ss.append("ms");
        gr::Print("[STATE#MACHINE]", ss.c_str());

        this->_isAdding = false;
    }
}

StatesRef& StateMachine::GetActiveState()
{
    return this->_states.top();
}