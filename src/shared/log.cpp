#include "../../include/shared/log.h"


Log::Log(std::string _fileName, std::string _prefix):
    fileName(_fileName), prefix(_prefix)
{
    m_file.open(fileName, std::ios::out);
}

Log::~Log()
{
    m_file.close();
}

const std::string &Log::getPrefix()const
{
    return prefix;
}


void Log::writeToFile(std::string str)
{
    m_file << str;
}
