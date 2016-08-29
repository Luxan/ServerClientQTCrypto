#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class Log
{
private:
    std::fstream m_file;
    const std::string fileName;
    const std::string prefix;
protected:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void writeToFile(std::string str);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    const std::string &getPrefix()const;

    std::mutex writerLock;
public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    Log(std::string _fileName, std::string _prefix);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual Log &operator <<(const std::string &stream) = 0;
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~Log();
};

