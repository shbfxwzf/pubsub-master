#include "PubSub.hpp"
#include <glog/logging.h>

void PubSub::filter(std::shared_ptr<Connection> conn, const Message &msg)
{
    Protocol pro;
    pro.deserialization(msg);
    std::string cmd = pro.getCmd();
    LOG(INFO) << "Received a message of cmd: " << cmd << " channel: " << pro.getChannel() << "msg:" << pro.getMsg();
    
    if(cmd=="sub" || cmd=="unsub" || cmd=="regist")
        subscriberStub(conn, msg);
    else
        publisherStub(conn, msg);
}

void PubSub::subscriberStub(std::shared_ptr<Connection> conn, const Message &msg){
    Protocol pro;
    pro.deserialization(msg);
    if(pro.getCmd()=="sub")
        subscribe(conn,pro.getChannel());
    else if(pro.getCmd()=="unsub"){
        unsubscribe(conn,pro.getChannel());
    }else if(pro.getCmd()=="regist")
        regist(conn);
    else
    {
        LOG(ERROR) << "subscriberStub: no such command " << pro.getCmd() <<" ...";
    }
}


void PubSub::publisherStub(std::shared_ptr<Connection> conn, const Message &msg)
{
    Protocol pro;
    pro.deserialization(msg);
    if(pro.getCmd()=="pub")
        publish(pro.getChannel(), msg);
    else if(pro.getCmd()=="unpub")
        unpublish(conn,pro.getChannel());
    else
    {
        LOG(ERROR) << "publisherStub: no such command...";
    }
}


void PubSub::regist(std::shared_ptr<Connection> conn)
{
    if(!subscriberMap_.count(conn))
    {
        subscriberMap_[conn] = std::set<std::string>();
        LOG(INFO) << "A subscriber registed " << conn->getFD() << "...";
    }
}

void PubSub::subscribe(std::shared_ptr<Connection> conn, const std::string &channel)
{
    if(subscriberMap_.count(conn))
    {
        subscriberMap_[conn].insert(channel);
    }
    else
    {
        subscriberMap_[conn] = std::set<std::string>();
        subscriberMap_[conn].insert(channel);
    }
    
    if(queueMap_.count(channel))
    {
        queueMap_[channel]->addConnection(conn);
    }
    else
    {
        queueMap_.insert({channel,std::make_shared<socketx::MessageQueue>()});
        queueMap_[channel]->addConnection(conn);
    }
    LOG(INFO) << "Subscriber " << conn->getFD() << "subscribes channel " << channel.c_str();

}

void PubSub::unsubscribe(std::shared_ptr<Connection> conn, const std::string &channel)
{
    if(subscriberMap_.count(conn))
    {
        subscriberMap_[conn].erase(channel);
    }
    else
    {
        return;
    }
    
    if(queueMap_.count(channel))
    {
        queueMap_[channel]->removeConnection(conn);
    }
    else
    {
        LOG(INFO) << "Unsubscribe error: no such a channel....";
    }
}

void PubSub::publish(const std::string &channel, const Message &msg)
{
   LOG(INFO) << "called publish with content " << msg.getData();

   /*
    *  如果没有任何人订阅，那就广播给所有已经注册的客户端
    */
#if 1
   if(!queueMap_.count(channel))
   {
        broadcast(channel);
        queueMap_.insert({channel,std::make_shared<socketx::MessageQueue>()});
    }
#endif
    queueMap_[channel]->recv(msg);
    if(pool_.getIdleThreadNum()==0) pool_.addThread();
    pool_.submit(std::bind(&PubSub::publishMsg, this, channel));
}

void PubSub::unpublish(std::shared_ptr<Connection> conn, const std::string &channel)
{
    if(channelMap_.count(channel))
    {
        auto it = std::find(channelMap_[channel].begin(),channelMap_[channel].end(),conn);
        if(it == channelMap_[channel].end())
        {
            LOG(INFO) << "unpublish: no such a connection in this channel...";
        }
        else
        {
            channelMap_[channel].erase(it);
        }
    }
    else
    {
        LOG(INFO) << "Unpublish error: no such a channel....\n";
    }
}

void PubSub::broadcast(const string &channel)
{
    Protocol pro("broadcast", channel, "");
    Message msg = pro.serialization();
    for(auto it=subscriberMap_.begin();it!=subscriberMap_.end();++it)
    {
        if((it->second).count(channel)) continue;
        else
        {
            it->first->sendmsg(msg);
        }
    }
}

void PubSub::checker()
{

}

void PubSub::publishMsg(const std::string &channel)
{
    LOG(INFO) << "enter publishMsg";
    while(!queueMap_[channel]->empty())
    {
        if (1 != queueMap_[channel]->send())
        {
            LOG(ERROR) << "send msg failed.";
        }
    }
}