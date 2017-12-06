#ifndef _PUBSUB_HPP__
#define _PUBSUB_HPP__

#include "socketx.hpp"
#include "Protocol.hpp"

class PubSub{

    public:
        PubSub()=default;

        void subscriberStub(std::shared_ptr<Connection> conn, const Message &msg);
        void publisherStub(std::shared_ptr<Connection> conn, const Message &msg);
        void filter(std::shared_ptr<Connection> conn, const Message &msg);
        
    private:
        void regist(std::shared_ptr<Connection> conn);
        void subscribe(std::shared_ptr<Connection> conn, const std::string &channel);
        void unsubscribe(std::shared_ptr<Connection>, const std::string &channel);

        void publish(const std::string &theme, const Message &msg);
        void unpublish(std::shared_ptr<Connection> conn, const std::string &channel);

        void broadcast(const string &channel);

        void checker();
        void publishMsg(const std::string &channel);

        /*
         * key : channelֵ
         * value : ��Ϣ���У�һ��channel��Ӧһ��������msg queue��
         *         msg queueȷ�������е���Ϣ�������еĶ����ߣ����ҽ�����һ��
         */
        std::unordered_map<std::string, std::shared_ptr<socketx::MessageQueue>> queueMap_;

        
        std::unordered_map<std::string, std::vector<std::shared_ptr<Connection>>> channelMap_;


        /*
         * key : ���ӵĿͻ���
         * value : һ�����ϣ����ж��ĵ�����
         */
        std::unordered_map<std::shared_ptr<Connection>, std::set<std::string>> subscriberMap_;
        socketx::ThreadPool pool_;
};
#endif
