#include "Subscriber.hpp"

bool Subscriber::isSubscribed(std::string channel)
{
    auto it = std::find(channelList.begin(),channelList.end(),channel);
    if(it==channelList.end())
        return false;
    return true;
}

void Subscriber::printer(const std::string &channel, const std::string &msg)
{
    cout<<"channel: "<<channel<<endl;
    cout<<"Message: "<<msg<<endl;
}

void Subscriber::filter(const Message &msg)
{
    Protocol pro;
    pro.deserialization(msg);
    std::string cmd = pro.getCmd();
    printf("Received a message of command cmd %s.\n", cmd.c_str());
    if(cmd=="broadcast")
    {
        std::string channel = pro.getChannel();
        if(isSubscribed(channel)) return;
        else
        {
            printf("Subscribe channel %s <Y/N> ?:",channel.c_str());
            std::string str;
            cin>>str;
            if(str=="Y") subscribe(channel);
            else return;
        }
    }
    else if(cmd=="pub")
    {
        std::string channel = pro.getChannel();
        std::string msg = pro.getMsg();
        printer(channel,msg);
    }
}

void Subscriber::regist()
{
    Protocol pro("regist","","");
    socketx::Message msg = pro.serialization();
    conn_->sendmsg(msg);
}

void Subscriber::subscribe(std::string channel)
{
    if(isSubscribed(channel))
    {
        printf("Theme %s was subscribed before...\n",channel.c_str());
        return;
    }
    else
        channelList.push_back(channel);

    Protocol pro("sub",channel,"");
    socketx::Message msg = pro.serialization();
    conn_->sendmsg(msg);
    printf("Subsribe channel %s \n", channel.c_str());
}

void Subscriber::unsubscribe(std::string theme)
{
    if(!isSubscribed(theme))
    {
        printf("Theme %s was unsubscribed before...\n",theme.c_str());
        return;
    }
    else
        channelList.erase(std::find(channelList.begin(),channelList.end(),theme));

    Protocol pro("unsub",theme,"");
    socketx::Message msg = pro.serialization();
    conn_->sendmsg(msg);
    printf("Unsubsribe Theme %s \n", theme.c_str());
}