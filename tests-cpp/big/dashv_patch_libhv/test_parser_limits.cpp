#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <hv/HttpParser.h>
#include <hv/WebSocketParser.h>
#include <hv/wsdef.h>
#include <memory>
#include <string>
#include <vector>

static std::string frame(int opcode, bool final, const std::string & payload) {
    std::string result;
    result.push_back(char(opcode | (final ? 0x80 : 0)));
    if (payload.size() <= 125) result.push_back(char(0x80 | payload.size()));
    else {
        result.push_back(char(0xfe));
        result.push_back(char(payload.size() >> 8));
        result.push_back(char(payload.size()));
    }
    result.append(4, '\0');
    result += payload;
    return result;
}

TEST_CASE("HTTP declared body limit precedes reservation") {
    std::unique_ptr<HttpParser> parser(HttpParser::New(HTTP_SERVER, HTTP_V1));
    HttpRequest request;
    parser->max_body_size = 8;
    parser->InitRequest(&request);
    const auto capacity = request.body.capacity();
    std::string header = "POST / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 1000000000\r\n\r\n";
    parser->FeedRecvData(header.data(), header.size());
    CHECK(parser->GetError() != 0);
    CHECK(parser->body_limit_exceeded);
    CHECK(request.body.empty());
    CHECK(request.body.capacity() == capacity);
}

TEST_CASE("HTTP chunked bodies have a cumulative limit") {
    std::unique_ptr<HttpParser> parser(HttpParser::New(HTTP_SERVER, HTTP_V1));
    HttpRequest request;
    parser->max_body_size = 8;
    parser->InitRequest(&request);
    std::string start = "POST / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n1234\r\n4\r\n5678\r\n";
    REQUIRE(parser->FeedRecvData(start.data(), start.size()) == int(start.size()));
    CHECK(request.body == "12345678");
    const auto capacity = request.body.capacity();
    std::string huge_chunk = "10000000\r\n";
    // FeedRecvData can report full consumption when the final byte triggers a callback error.
    parser->FeedRecvData(huge_chunk.data(), huge_chunk.size());
    CHECK(parser->GetError() != 0);
    CHECK(parser->body_limit_exceeded);
    CHECK(request.body == "12345678");
    CHECK(request.body.capacity() == capacity);
}

TEST_CASE("WebSocket fragmented messages survive control frames and respect total limit") {
    WebSocketParser parser;
    parser.max_message_size = 16;
    std::vector<std::pair<int,std::string>> seen;
    parser.onMessage = [&](int opcode, const std::string & text) { seen.emplace_back(opcode, text); };
    for (auto packet : {frame(WS_OPCODE_TEXT, false, "12345678"), frame(WS_OPCODE_PING, true, "p"), frame(WS_OPCODE_CONTINUE, true, "abcdefgh")}) {
        REQUIRE(parser.FeedRecvData(packet.data(), packet.size()) == int(packet.size()));
    }
    REQUIRE(seen.size() == 2);
    CHECK(seen[0] == std::make_pair(int(WS_OPCODE_PING), std::string("p")));
    CHECK(seen[1] == std::make_pair(int(WS_OPCODE_TEXT), std::string("12345678abcdefgh")));
}

TEST_CASE("WebSocket continuation cannot exceed the message budget") {
    WebSocketParser parser;
    parser.max_message_size = 16;
    int calls = 0;
    parser.onMessage = [&](int, const std::string &) { ++calls; };
    auto first = frame(WS_OPCODE_TEXT, false, "12345678");
    REQUIRE(parser.FeedRecvData(first.data(), first.size()) == int(first.size()));
    auto last = frame(WS_OPCODE_CONTINUE, true, "abcdefghi");
    CHECK(parser.FeedRecvData(last.data(), last.size()) != int(last.size()));
    CHECK(calls == 0);
    CHECK(parser.message == "12345678");
}

TEST_CASE("WebSocket advertised length is rejected before reservation") {
    WebSocketParser parser;
    parser.max_message_size = 16;
    const auto capacity = parser.message.capacity();
    const unsigned char header[] = {0x81, 0xff, 0, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0, 0};
    std::string data(reinterpret_cast<const char *>(header), sizeof(header));
    CHECK(parser.FeedRecvData(data.data(), data.size()) != int(data.size()));
    CHECK(parser.message.capacity() == capacity);
}

TEST_CASE("WebSocket continuation requires an open message") {
    WebSocketParser parser;
    auto data = frame(WS_OPCODE_CONTINUE, true, "no start");
    CHECK(parser.FeedRecvData(data.data(), data.size()) != int(data.size()));
}

TEST_CASE("WebSocket control frames cannot be fragmented or oversized") {
    for (const auto & packet : {frame(WS_OPCODE_PING, false, "x"), frame(WS_OPCODE_PING, true, std::string(126, 'x'))}) {
        WebSocketParser parser;
        int delivered = 0;
        parser.onMessage = [&](int, const std::string &) { ++delivered; };
        CHECK(parser.FeedRecvData(packet.data(), packet.size()) != int(packet.size()));
        CHECK(delivered == 0);
    }
}

TEST_CASE("WebSocket reserved opcodes are not delivered") {
    WebSocketParser parser;
    int delivered = 0;
    parser.onMessage = [&](int, const std::string &) { ++delivered; };
    const auto packet = frame(3, true, "reserved");
    CHECK(parser.FeedRecvData(packet.data(), packet.size()) != int(packet.size()));
    CHECK(delivered == 0);
}

TEST_CASE("WebSocket data opener cannot replace an unfinished message") {
    WebSocketParser parser;
    int delivered = 0;
    parser.onMessage = [&](int, const std::string &) { ++delivered; };
    const auto first = frame(WS_OPCODE_TEXT, false, "first");
    REQUIRE(parser.FeedRecvData(first.data(), first.size()) == int(first.size()));
    const auto second = frame(WS_OPCODE_BINARY, true, "second");
    CHECK(parser.FeedRecvData(second.data(), second.size()) != int(second.size()));
    CHECK(parser.message == "first");
    CHECK(delivered == 0);
}
