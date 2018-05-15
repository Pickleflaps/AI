#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <iostream>
#include <map>

class BlackboardsApp : public aie::Application {

	enum BlackboardDataType
	{
		Float,
		Int,
		String,
		Bool,
		Pointer,
	};

	class BlackboardData
	{
	public:
		BlackboardDataType type;

		union data {
			float	floatValue;
			int		intValue;
			char	stringValue;
			bool	boolValue;
			void*	pointerValue;
		} data;
		BlackboardData();
	};

	class Blackboard
	{
	public:
		Blackboard();
		~Blackboard();

		bool GetEntry(std::string id, float &value);
		bool GetEntry(std::string id, int &value);
		bool GetEntry(std::string id, bool &value);
		bool GetEntry(std::string id, void* value);

		bool SetEntry(std::string id, float value);
		bool SetEntry(std::string id, int value);
		bool SetEntry(std::string id, bool value);
		bool SetEntry(std::string id, void* value);

	private:
		std::map<std::string, BlackboardData> dataMap;


	};

};