
#ifndef _PUBLISHER_HPP__
#define _PUBLISHER_HPP__

#include "socketx.hpp"
#include "Protocol.hpp"

class Publisher
{
    public:
        Publisher(std::shared_ptr<socketx::Connection> conn)
        : conn_(conn), connected(true)
        {
            
        }

        void publish(std::string channel, std::string msg);
        void unpublish(std::string channel);
    
    private: 
        std::vector<std::string> themeList;
        std::shared_ptr<socketx::Connection> conn_;
        bool connected;
};


#endif