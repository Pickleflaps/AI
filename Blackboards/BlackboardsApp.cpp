#include "BlackboardsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"


BlackboardsApp::Blackboard::Blackboard()
{
}

BlackboardsApp::Blackboard::~Blackboard()
{
}

bool BlackboardsApp::Blackboard::GetEntry(std::string id, float & value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
		return false;
	else //item found, make sure it is a float type before returning it
	{
		if (item->second.type == BlackboardDataType::Float)
		{
			value = item->second.data.floatValue;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::GetEntry(std::string id, int & value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
		return false;
	else //item found, make sure it is a float type before returning it
	{
		if (item->second.type == BlackboardDataType::Int)
		{
			value = item->second.data.intValue;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::GetEntry(std::string id, bool & value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
		return false;
	else //item found, make sure it is a float type before returning it
	{
		if (item->second.type == BlackboardDataType::Bool)
		{
			value = item->second.data.boolValue;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::GetEntry(std::string id, void * value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
		return false;
	else //item found, make sure it is a float type before returning it
	{
		if (item->second.type == BlackboardDataType::Pointer)
		{
			value = item->second.data.pointerValue;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::SetEntry(std::string id, float value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
	{
		//add item
		BlackboardData blackboardData;
		blackboardData.type = BlackboardDataType::Float;
		blackboardData.data.floatValue = value;

		this->dataMap[id] = blackboardData;

		return true;
	}
	else
	{
		if (item->second.type == BlackboardDataType::Float)
		{
			item->second.data.floatValue = value;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::SetEntry(std::string id, int value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
	{
		//add item
		BlackboardData blackboardData;
		blackboardData.type = BlackboardDataType::Int;
		blackboardData.data.intValue = value;

		this->dataMap[id] = blackboardData;

		return true;
	}
	else
	{
		if (item->second.type == BlackboardDataType::Int)
		{
			item->second.data.intValue = value;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::SetEntry(std::string id, bool value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
	{
		//add item
		BlackboardData blackboardData;
		blackboardData.type = BlackboardDataType::Bool;
		blackboardData.data.boolValue = value;

		this->dataMap[id] = blackboardData;

		return true;
	}
	else
	{
		if (item->second.type == BlackboardDataType::Bool)
		{
			item->second.data.boolValue = value;
			return true;
		}
	}
	return false;
}

bool BlackboardsApp::Blackboard::SetEntry(std::string id, void * value)
{
	auto item = dataMap.find(id);

	//not found
	if (item == dataMap.end())
	{
		//add item
		BlackboardData blackboardData;
		blackboardData.type = BlackboardDataType::Pointer;
		blackboardData.data.pointerValue = value;

		this->dataMap[id] = blackboardData;

		return true;
	}
	else
		return false;
}

BlackboardsApp::BlackboardData::BlackboardData()
{
}
