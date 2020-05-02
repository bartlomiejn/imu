#include <utils/loader.hh>
#include <fstream>

FileLoader::FileLoader(std::string &&filename)
{
	m_filename = std::move(filename);
}

std::string FileLoader::read()
{
	std::ifstream stream(m_filename);
	if (!stream)
		throw errno;
	std::string string(
		(std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	return string;
}