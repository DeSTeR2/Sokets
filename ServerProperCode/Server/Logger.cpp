#include "Logger.h"
namespace Logger {
	Logger::Logger() {}

	Logger::~Logger()
	{
		file.close();
	}

	bool Logger::Init(std::string filePath)
	{
		this->filePath = filePath;
		file.open(filePath);
		return true;
	}

	bool Logger::Log(std::string message)
	{
		time_t now = time(0);
		char* dt = ctime(&now);
		dt[24] = ']';

		file << "Time: [" << dt << "\nMessage: " << message << "\n\n";
		std::cout << message << "\n";
		return true;
	}
}