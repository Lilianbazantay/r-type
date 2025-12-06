#pragma once

#include <string>

class IComponent
{
    private:
        std::string _name;
    public:
        ~IComponent() = default;

        // SET/GET
        std::string getName() const { return _name; }
        void setName(const std::string& name) { _name = name; }
};
