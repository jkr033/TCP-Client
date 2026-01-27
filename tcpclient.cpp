#include <iostream>
#include <string>
#include <memory>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

static void throw_ssl(const std::string& msg)
{
    std:cerr << msg << "\n";
    ERR_print_errors_fp(stderr);
    throw std::runtime_error(msg);
}

SSL_CTX* create_context() 
{
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);

    if (!ctx)
    {
        throw_ssl("Failed to create SSL_CTX.");
        return ctx;
    }
 }

void configure_context(SSL_CTX* ctx)
{
    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
    {
        throw_ssl("Failed to load CA certificates.");
    }
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);
}

int main()
{
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    int sock = 0;
    SSL_CTX* ctx = nullptr;
    SSL*ssl = nullptr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error." << std::endl;
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "The address is invalid or not supported." << std::endl;
        return 0;
    }
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection failed." << std::endl;
        return 0;
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sock);
    return 0;

    std::string message = "Message from the client!";
    send(sock, message.c_str(), message.size(), 0);
    std::cout << "Message has been sent!" << std::endl;
    ssize_t valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "Received from server: " << buffer << std::endl;
    close(sock);
    return 0;
}
