#pragma once
#include "Logger.h"
#include <fstream>

class FileLogger:public Logger
{
private:
	std::ofstream file;

public:
	FileLogger(const std::string& filename);
	~FileLogger();
	void log(const std::string& message) override;


};

