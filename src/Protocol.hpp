#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP


#include "socketx.hpp"
#include <unordered_map>
#include <set>

using socketx::Message;
using socketx::Connection;
using std::cout;
using std::endl;
using std::string;
using std::cin;

/*
 * protocol����Ҫ������Ϣ�����л��ͷ����л���theme��ʾ����
 * ||cmd_size||channel_size||msg_size||cmd||channel||msg||
 * ������:
 * sub, pub, unsub, unpub, broadcast, regist
 */


class Protocol{
    private:
        size_t cmd_size; 
        size_t channel_size;
        size_t msg_size;
        std::string cmd_;
        std::string channel_;
        std::string msg_;
    public:
        std::string getCmd(){
            return cmd_;
        }
        std::string getChannel(){
            return channel_;
        }
        std::string getMsg(){
            return msg_;
        }

        Protocol()=default;
        Protocol(std::string cmd, std::string channel, std::string msg);

        size_t getBytesLength();
        Message serialization();
        void deserialization(const Message &msg);
};


#endif
