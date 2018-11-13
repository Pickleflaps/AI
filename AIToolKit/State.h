#pragma once

#include "Behaviour.h"
#include <vector>
#include "Condition.h"


class State;

class Transition {
public:

	Transition(State* target, Condition* condition) : m_target(target), m_condition(condition) {}
	~Transition() {};

	State* getTargetState() { return m_target; }
	bool hasTriggered(GameObject* gameObject) { return m_condition->test(gameObject); }

private:

	State* m_target;
	Condition* m_condition;

};
//Abstract class
class State
{

	friend class FiniteStateMachine;

public:

	State() : m_timer(0) {}
	virtual ~State() {}
	
	//pure virtual
	virtual void	update(GameObject* gameObject, float deltaTime) = 0;

	//triggers for enter / exit
	virtual void	onEnter(GameObject* gameObject) {}
	virtual void	onExit(GameObject* gameObject) {}

	float			getTimer() const { return m_timer; }
	const float*	getTimerPtr() const { return &m_timer; }

	void addTransition(Transition* transition)
	{
		m_transitions.push_back(transition);
	}

	Transition* getTriggeredTransition(GameObject* gameObject);

protected:

	float			m_timer;
	std::vector<Transition*>	m_transitions;
};



class FiniteStateMachine : public Behaviour {
	
public:

	FiniteStateMachine() : m_currentState(nullptr) {}
	virtual ~FiniteStateMachine() { 
		for (auto s : m_states) 
			delete s; 
		for (auto t : m_transitions)
			delete t;
		for (auto c : m_conditions)
			delete c;
	}

	//only set a state if it does not yet have one
	void setInitialState(State* state) { if (m_currentState == nullptr) m_currentState = state; }

	//add a new state and takes ownership
	void addState(State* state) { m_states.push_back(state); }
	void addTransition(Transition* trans) { m_transitions.push_back(trans); }
	void addCondition(Condition* cond) { m_conditions.push_back(cond); }

	virtual bool execute(GameObject* gameObject, float deltaTime);


	

protected:

	State*		m_currentState;

	std::vector<State*>			m_states;
	std::vector<Transition*>	m_transitions;
	std::vector<Condition*>		m_conditions;

};



