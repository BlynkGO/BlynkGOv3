#ifndef ASYNCTCP_SECURE_H_
#define ASYNCTCP_SECURE_H_

#include "Arduino.h"
#include "AsyncTCP.h"

#if BLYNKGO_USE_WIFI

#include "IPAddress.h"
#include <functional>
#include <string>

extern "C" {
#include "freertos/semphr.h"
#include "lwip/pbuf.h"
#include "tcp_mbedtls.h"
#include "../esp32_memory.h"
}

class AsyncClientSecure;

class AsyncClientSecure : public AsyncClient {
  protected:
    std::string _hostname;
    
    size_t _root_ca_len;
    char* _root_ca;
    bool _pcb_secure;
    bool _handshake_done;
    const char* _psk_ident;
    const char* _psk;

    int8_t _close();
    int8_t _connected(void* pcb, int8_t err);
    void _ssl_error(int8_t err);
    int8_t _poll(tcp_pcb* pcb);
    int8_t _sent(tcp_pcb* pcb, uint16_t len);
    void _dns_found(struct ip_addr *ipaddr);
    static void _s_data(void *arg, struct tcp_pcb *tcp, uint8_t * data, size_t len);
    static void _s_handshake(void *arg, struct tcp_pcb *tcp, struct tcp_ssl_pcb* ssl);
    static void _s_ssl_error(void *arg, struct tcp_pcb *tcp, int8_t err);

  public:
    AsyncClientSecure* prev;
    AsyncClientSecure* next;
    
    AsyncClientSecure(tcp_pcb* pcb = 0);
    
    AsyncClientSecure & operator=(const AsyncClientSecure &other);
    AsyncClientSecure & operator+=(const AsyncClientSecure &other);

    bool operator==(const AsyncClientSecure &other);

    bool connect(IPAddress ip, uint16_t port);
    bool connect(const char* host, uint16_t port);
    bool connect(IPAddress ip, uint16_t port, bool secure );
    bool connect(const char* host, uint16_t port,  bool secure );
    void setRootCa(const char* rootca, const size_t len);
    inline void setRootCa(String rootca)           { setRootCa(rootca.c_str(), rootca.length() ); }
    void setPsk(const char* psk_ident, const char* psk);

    size_t add(const char* data, size_t size, uint8_t apiflags=ASYNC_WRITE_FLAG_COPY);//add for sending
    bool send();//send all data added with the method above

    size_t write(const char* data);
    size_t write(const char* data, size_t size, uint8_t apiflags=ASYNC_WRITE_FLAG_COPY); //only when canSend() == true
    inline size_t write(String data)        { return write(data.c_str(), data.length()); }

    
    int8_t _recv(tcp_pcb* pcb, pbuf* pb, int8_t err);
    
    static int8_t _s_poll(void *arg, struct tcp_pcb *tpcb);
    static int8_t _s_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *pb, int8_t err);
    static void _s_error(void *arg, int8_t err);
    static int8_t _s_sent(void *arg, struct tcp_pcb *tpcb, uint16_t len);
    static int8_t _s_connected(void* arg, void* tpcb, int8_t err);
    static void _s_dns_found(const char *name, struct ip_addr *ipaddr, void *arg);

};

typedef std::function<int(void* arg, const char *filename, uint8_t **buf)> AcSSlFileHandler;

class AsyncServerSecure : public AsyncServer {
  public:
    AsyncServerSecure(IPAddress addr, uint16_t port): AsyncServer(addr, port) {}
    AsyncServerSecure(uint16_t port): AsyncServer(port) {}
    
    // Dummy, so it compiles with ESP Async WebServer library enabled.
    void onSslFileRequest(AcSSlFileHandler cb, void* arg) {};
    void beginSecure(const char *cert, const char *private_key_file, const char *password) {};
};


#endif //#if BLYNKGO_USE_WIFI
#endif //ASYNCTCP_SECURE_H_
