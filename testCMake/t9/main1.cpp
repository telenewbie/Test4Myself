/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For gethostbyname */
#include <netdb.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

int main(int c, char **v)
{
/**
    struct event_base *base = NULL;
    base = event_base_new();
    struct bufferevent *bev;
    SSL_CTX *ssl_ctx = NULL;
    SSL *ssl = NULL;
    ssl_ctx = SSL_CTX_new(SSLv23_method());
    ssl = SSL_new(ssl_ctx);
    bev = bufferevent_openssl_socket_new(base, -1, ssl,
            BUFFEREVENT_SSL_CONNECTING,
            BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
**/
    struct event_base *base;
    base = event_base_new();
        if (!base)
                    return -1; /*XXXerr*/
    const char query[] =
        "GET / HTTP/1.0\r\n"
        "Host: www.baidu.com\r\n"
        "\r\n";
    const char hostname[] = "www.baidu.com";
    struct sockaddr_in sin;
    struct hostent *h;
    const char *cp;
    int fd;
    ssize_t n_written, remaining;
    char buf[1024];

    /* Look up the IP address for the hostname.   Watch out; this isn't
     *        threadsafe on most platforms. */
    h = gethostbyname(hostname);
    if (!h) {
        fprintf(stderr, "Couldn't lookup %s: %s", hostname, hstrerror(h_errno));
        return 1;

    }
    if (h->h_addrtype != AF_INET) {
        fprintf(stderr, "No ipv6 support, sorry.");
        return 1;

    }

    /* Allocate a new socket */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;

    }

    /* Connect to the remote host. */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    sin.sin_addr = *(struct in_addr*)h->h_addr;
    if (connect(fd, (struct sockaddr*) &sin, sizeof(sin))) {
        perror("connect");
        close(fd);
        return 1;

    }

    /* Write the query. */
    /* XXX Can send succeed partially? */
    cp = query;
    remaining = strlen(query);
    while (remaining) {
        n_written = send(fd, cp, remaining, 0);
        if (n_written <= 0) {
            perror("send");
            return 1;

        }
        remaining -= n_written;
        cp += n_written;

    }

    /* Get an answer back. */
    while (1) {
        ssize_t result = recv(fd, buf, sizeof(buf), 0);
        if (result == 0) {
            break;

        } else if (result < 0) {
            perror("recv");
            close(fd);
            return 1;

        }
        fwrite(buf, 1, result, stdout);

    }

    close(fd);
    return 0;

}
