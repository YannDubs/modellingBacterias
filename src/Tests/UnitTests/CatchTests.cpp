/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

// Define the entry point for tests with Catch frameworks.

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

// Share the application directory with all unit tests
std::string appDir;

int main(int argc, char** argv)
{
    // Save the application directory in a global variable
    appDir = argv[0];
    std::string::size_type lastSlashPos = appDir.rfind('/');
    if (lastSlashPos == std::string::npos) {
        appDir = "./";
    } else {
        appDir = appDir.substr(0, lastSlashPos + 1);
    }

    return Catch::Session().run(argc, argv);
}
