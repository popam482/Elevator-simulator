#pragma once
#include "Logger.h"

class ConsoleLogger:public Logger
{
public:
	void log(const std::string& message) override;
	~ConsoleLogger();
};

