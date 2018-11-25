#ifndef SHELL_FRONTEND_HPP
#define SHELL_FRONTEND_HPP

#include <memory>
#include <iostream>
#include <thread>

#include "shell_engine.hpp"

class ShellFrontend
{
private:
    std::istream &myInputStream;
    std::ostream &myErr;
    std::ostream &myOut;
    bool myTerminate;
    std::thread myThread;
    ShellEnginePtr myEngine;

public:
    ShellFrontend( std::istream &is , std::ostream &er , std::ostream &os , ShellEnginePtr engine );

    ~ShellFrontend();

    void shellLoop();

    void start();
    void stop();
    
    void join();

};

typedef std::shared_ptr<ShellFrontend> ShellFrontendPtr;

#endif /* !defined SHELL_FRONTEND_HPP */
