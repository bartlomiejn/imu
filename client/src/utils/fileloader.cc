#include <utils/fileloader.hh>
#include <fstream>

FileLoader::FileLoader(std::string &&filename)
{
	this->filename = std::move(filename);
}

std::string FileLoader::read()
{
	std::ifstream stream(filename);
	if (!stream)
		throw errno;
	std::string string(
		(std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	return string;
}