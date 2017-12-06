#include "Protocol.hpp"


Protocol::Protocol(std::string cmd, std::string channel="", std::string msg="")
    :cmd_(cmd), channel_(channel), msg_(msg),
    cmd_size(cmd.size()),
    channel_size(channel.size()),
    msg_size(msg.size()){

}

size_t Protocol::getBytesLength(){
    return sizeof(size_t) * 3 + cmd_size + 1 + channel_size + 1 + msg_size + 1;
}

Message Protocol::serialization()
{
    size_t n = getBytesLength();
    char * ret = new char[n];
    char * p = ret;

    /*Copy cmd_size to the bytes array*/
    memcpy(p,&cmd_size,sizeof(size_t));
    p += sizeof(size_t);
    /*Copy channel_size to the bytes array*/
    memcpy(p,&channel_size,sizeof(size_t));
    p += sizeof(size_t);
    /*Copy msg_size to the bytes array*/
    memcpy(p,&msg_size,sizeof(size_t));
    p += sizeof(size_t);

    /*Copy cmd*/
    const char * s1 = cmd_.c_str();
    memcpy(p,s1,cmd_size+1);
    p += cmd_size+1;
    /*Copy channel*/
    const char * s2 = channel_.c_str();
    memcpy(p,s2,channel_size+1);
    p += channel_size+1;
    /*Copy msg*/
    const char * s3 = msg_.c_str();
    memcpy(p,s3,msg_size+1);
    p += msg_size+1;
    
    return Message(ret,n);
}

void Protocol::deserialization(const Message &msg)
{
    int n = msg.getSize();
    const char * data = msg.getData();
    size_t count = 0;

    /* 获取cmd的大小 */
    if(count < n){
        memcpy(&cmd_size,data,sizeof(size_t));
        data += sizeof(size_t);
        count += sizeof(size_t);
    }
    /* 获取主题的大小 */
    if(count < n){
        memcpy(&channel_size,data,sizeof(size_t));
        data += sizeof(size_t);
        count += sizeof(size_t);
    }
    /*Get msg_size*/
    if(count < n){
        memcpy(&msg_size,data,sizeof(size_t));
        data += sizeof(size_t);
        count += sizeof(size_t);
    }
    char *s = new char[cmd_size+1];
    if(count < n){
        memcpy(s,data,cmd_size+1);
        cmd_ = std::string(s);
        data += cmd_size+1;
        count += cmd_size+1;
    }
    if(s!=nullptr){
        delete s;
        s = nullptr;
    }
    if(count < n){
        s = new char[channel_size+1];
        memcpy(s,data,channel_size+1);
        channel_ = std::string(s);
        data += channel_size+1;
        count += channel_size+1;
    }
    if(s!=nullptr){
        delete s;
        s = nullptr;
    }
    if(count < n){
        s = new char[msg_size+1];
        memcpy(s,data,msg_size+1);
        msg_ = std::string(s);
        data += msg_size+1;
        count += msg_size+1;
    }
    if(s!=nullptr){
        delete s;
        s = nullptr;
    }
}