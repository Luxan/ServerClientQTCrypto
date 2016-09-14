#pragma once

#include <stdint.h>

/*!
\enum eError
\brief enum that defines errors in system.
*/
enum eError : uint8_t
{
    Undefined /*! Undefined error. */
    ,ServerIsAnaviable /*! Error that identify that server is unreacjable. */
    ,WrongLoginOrPassword /*! Error that suggest that entered login or password are incorrect. */
};

