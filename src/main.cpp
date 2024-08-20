#include <exception>
#include <iostream>

#include <vsg/all.h>

#include "Application.h"

int main()
{
    try
    {
        Application application;
        application.initialize();
        application.run();
    }
    catch (const vsg::Exception& exception)
    {
        std::cerr << "[VSG exception] - " << exception.message << std::endl;
        return 1;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[Standard exception] - " << exception.what() << std::endl;
        return 1;
    }

    return 0;
}
