#ifndef IMU_FILE_LOADER_H
#define IMU_FILE_LOADER_H

#include <string>

class FileLoader
{
public:
	explicit FileLoader(std::string &&name);
	std::string try_read();
private:
	std::string filename;
};

#endif 