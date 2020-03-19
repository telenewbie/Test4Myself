#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

int main(int argc, char **argv)
{
    BIO *sbio = NULL, *out = NULL;
    int len;
    char tmpbuf[1024];
    SSL_CTX *ctx;
}
