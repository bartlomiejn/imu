#include <utils/loader.h>
#include <fstream>

Loader::Loader(std::string &&filename)
{
	m_filename = std::move(filename);
}

std::string Loader::read()
{
	std::ifstream stream(m_filename);
	if (!stream)
		throw errno;
	std::string string(
		(std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	return string;
}
