#pragma once

namespace das {
    class Server {
    public:
        Server ();
        virtual ~Server();
        void init( int port = 9000 );
        bool is_open() const;
        bool is_connected() const;
        void tick();
        bool send_msg ( char * data, int size );
    protected:
        virtual void onConnect();
        virtual void onDisconnect();
        virtual void onData ( char * buf, int size );
        virtual void onError ( const char * msg, int code );
        virtual void onLog ( const char * msg );
    protected:
        int server_fd = 0;
        int client_fd = 0;
    };
}