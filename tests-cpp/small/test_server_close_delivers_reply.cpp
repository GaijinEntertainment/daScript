// A server that answers and is deleted at once still delivers the answer: closing a socket
// with unread inbound bytes resets the connection, and the peer loses what it had not read
// yet (the DAP server's terminate reply never reached the bridge on Windows).

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/misc/network.h"

#include <chrono>
#include <thread>

using namespace das;

namespace {

struct AcceptingServer : Server {
    bool accepted = false;
    void onConnect() override { accepted = true; }
    void acceptOnly() { if ( !accepted ) tick(); }
};

struct CollectingClient : Client {
    string received;
    bool closed = false;
    void onData ( char * buf, int size ) override { received.append(buf, size_t(size)); }
    void onDisconnect() override { closed = true; }
};

template <typename Cond>
bool wait_for ( int timeout_ms, Cond && cond ) {
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
    while ( !cond() ) {
        if ( std::chrono::steady_clock::now() >= deadline ) return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return true;
}

}

TEST_CASE("a deleted server's last message reaches a peer whose bytes it never read") {
    REQUIRE(Server::startup());
    int port = probe_local_port("127.0.0.1", 0);
    REQUIRE(port > 0);
    auto server = new AcceptingServer();
    REQUIRE(server->init(port));
    CollectingClient client;
    REQUIRE(client.connect("127.0.0.1", port));
    REQUIRE(wait_for(5000, [&]() { server->acceptOnly(); return server->accepted; }));

    char unread[] = "a request the server never reads";
    REQUIRE(client.send_msg(unread, int(sizeof(unread))));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    string reply(8 * 1024, 'r');
    REQUIRE(server->send_msg(reply.data(), int(reply.size())));
    std::thread closer([&]() { delete server; });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    CHECK(wait_for(5000, [&]() { client.tick(); return client.closed; }));
    closer.join();
    CHECK(client.received.size() == reply.size());
    CHECK(client.received == reply);
    Server::shutdown();
}
