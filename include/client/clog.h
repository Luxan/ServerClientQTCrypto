#pragma once

#include "../shared/log.h"

class CLog : public Log
{
public:
    CLog(std::string _fileName, std::string _prefix);


	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual Log &operator <<(const std::string &stream) override;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    static CLog &logError();
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    static CLog &logInfo();
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    static CLog &logWarn();
};
