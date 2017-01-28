#ifndef CLI_H
#define	CLI_H

#include <string>
using namespace std;

/**
 * Utility functions to work with Command line arguments
 */
class Cli
{
public:
    Cli(int argc, char **argv);
    string getArgument(string name);
    int getInt(string key, int defaultValue);
private:
    int argc;
    char **argv;
};

#endif	/* CLI_H */
