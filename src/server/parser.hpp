#pragma once

class Parser
{
private:
    int port;
public:
    ~Parser() = default;

    int ParseData(int argc, char **argv);
    static void descripton();
};
