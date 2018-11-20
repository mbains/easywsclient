#include "easywsclient.hpp"
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <string.h>


#define BUF_SIZE 8192

static char buf[BUF_SIZE+1];
using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;

void handle_message(const std::string & message)
{
    printf(">>> %s\n", message.c_str());
    if (message == "world") { 
        printf("closing socket\n");
        ws->close(); 
    }
}

int main()
{
    memset(buf, 'A', BUF_SIZE-1);
    buf[BUF_SIZE] = 0;
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        printf("WSAStartup Failed.\n");
        return 1;
    }
#endif

    ws = WebSocket::from_url("ws://localhost:8127/foo");
    assert(ws);
    ws->send("goodbye");
    ws->send("hello");
    //ws->send("world");
//    ws->send(buf);
    while (ws->getReadyState() != WebSocket::CLOSED) {
      ws->poll();
      ws->dispatch(handle_message);
    }
    delete ws;
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
