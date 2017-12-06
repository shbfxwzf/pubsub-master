
#include "Publisher.hpp"
#include <glog/logging.h>

void Publisher::publish(std::string channel, std::string msg)
{
    Protocol pro("pub", channel, msg);
    Message pub_msg = pro.serialization();
    conn_->sendmsg(pub_msg);
    
    LOG(INFO) << "A message of chanel" << channel <<" is publishing!";
}

void Publisher::unpublish(std::string channel)
{
    Protocol pro("unpub", channel, "");
    Message pub_msg = pro.serialization();
    conn_->sendmsg(pub_msg);

    LOG(INFO) << "A chanel " << channel << "will be unpublished!";
}


