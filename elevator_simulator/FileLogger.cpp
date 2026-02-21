#include "FileLogger.h"

FileLogger::FileLogger(const std::string& filename) {
	file.open(filename);
}

FileLogger::~FileLogger() {
	if (file.is_open())
		file.close();
}

void FileLogger::log(const std::string& message) {
	if(file.is_open())
		file << message << std::endl;
}
