#include "daScript/misc/platform.h"

#include "daScript/misc/network.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

namespace das {

    bool set_socket_blocking ( int fd, bool blocking ) {
        if (fd < 0) return false;
#ifdef _WIN32
        unsigned long mode = blocking ? 0 : 1;
        return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) return false;
        flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
        return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
    }

    Server::Server() {
    }

    void Server::init ( int port ) {
        if ( !(server_fd = socket(AF_INET, SOCK_STREAM, 0)) ) {
            onError("can't socket", server_fd);
            return;
        }
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( port );
    	if ( bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0 ) {
            onError("can't bind", server_fd);
            close(server_fd);
            return;
	    }
    	if ( listen(server_fd, 3) < 0) {
            onError("can't listen", server_fd);
            close(server_fd);
            return;
        }
        if ( !set_socket_blocking(server_fd,false) ) {
            close(server_fd);
            return;
        }
    }

    void Server::onData(char * buf, int size) {
        printf("%s\n", buf);
        send_msg(buf, size);
    }

    void Server::onConnect() {
    }

    void Server::onDisconnect() {
    }

    void Server::onError(const char * msg, int code) {
        printf("server error %i - %s\n", code, msg);
    }

    void Server::onLog(const char * msg) {
        printf("server log %s\n", msg);
    }

    bool Server::send_msg ( char * data, int size ) {
        if ( client_fd <= 0 ) {
            onError("can't send, not conencted", -1);
            return false;
        }
        int res = send(client_fd, data, size, 0);
        if ( res<0 ) {
            res = errno;
            onError ( "can't send", res);
            close(client_fd);
            client_fd = 0;
            return false;
        } else if ( res != size ) {
            onError ( "can't send entire message", res);
            close(client_fd);
            client_fd = 0;
            return false;
        } else  {
            return true;
        }
    }

    void Server::tick() {
        if ( client_fd<=0 ) {
            struct sockaddr_in address;
            int addrlen = sizeof(address);
            client_fd = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
            if ( client_fd>0 ) {
                onLog("connection accepted");
                onConnect();
            }
        } else {
            char buffer[1025];
            int res = recv(client_fd, buffer, 1024, MSG_DONTWAIT);
            if ( res >0 ) {
                buffer[res] = 0;
                onData(buffer, res);
            } else if ( res==0 ) {
                onLog("connection closed");
                onDisconnect();
                close(client_fd);
                client_fd = 0;
            } else if ( res<0 ) {
                res = errno;
                if ( res!=EAGAIN && res!=EWOULDBLOCK ) {
                    onError("connection closed on error", res);
                    onDisconnect();
                    close(client_fd);
                    client_fd = 0;
                }
            }
        }
    }

    Server::~Server() {
        if ( client_fd >0 ) {
            close(client_fd);
        }
        if ( server_fd ) {
            close(server_fd);
        }
    }

    bool Server::is_open() const {
        return server_fd != 0;
    }

    bool Server::is_connected() const {
        return client_fd > 0;
    }
}