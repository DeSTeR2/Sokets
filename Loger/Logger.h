#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#pragma warning(disable : 4996)
namespace Logger {
	class Logger
	{
		std::string filePath;
		std::ofstream file;
	public:
		Logger();
		~Logger();
		bool Init(std::string filePath);
		bool Log(std::string message);
	};
}