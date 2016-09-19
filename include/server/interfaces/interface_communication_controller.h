/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <list>

/**
\class
\brief
*/
class InterfaceCommunicationController
{
private:
    std::list<void *> module1;
    std::list<void *> module2;
protected:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void CheckModule1Events(void * module1, void * module2) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void CheckModule2Events(void * module1, void * module2) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setModule1Obj(void *_m1);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setModule2Obj(void *_m2);
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void CheckEvents(void *module);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void detach(void *module);
};
