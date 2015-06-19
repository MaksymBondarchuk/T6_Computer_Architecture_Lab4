//
// Created by max on 26.05.15.
//


#ifndef BUF_SIZE
#define BUF_SIZE 10000
#endif


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../serialization/serialization.hpp"


using namespace std;
using namespace boost::archive;


int main(int argc, char **argv) {
    int text_length;
    srand((unsigned) time(0));
    if (argc != 2) {
        cout << "You didn't write text length. It will be random generated (from 10 to 5000)" << endl;
        text_length = 100 + rand() % 4900;
    } else {
        text_length = atoi(argv[1]);
        if (text_length <= 0) {
            cout << "Text length must be a positive number. Now it will be random-generated" << endl;
            text_length = (unsigned int) (100 + rand() % 9900);
        }
    }

    const char *host = "localhost";
    uint16_t port = 3000;
    int fd;
    struct hostent *hp;    /* host information */
    unsigned int alen;    /* address length when we get the port number */
    struct sockaddr_in myaddr;    /* our address */
    struct sockaddr_in servaddr;    /* server address */

    /* get a tcp/ip socket */
    /* We do this as we did it for the server */
    /* request the Internet address protocol */
    /* and a reliable 2-way byte stream */

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create socket");
        return 0;
    }

    /* bind to an arbitrary return address */
    /* because this is the client side, we don't care about the */
    /* address since no application will connect here  --- */
    /* INADDR_ANY is the IP address and 0 is the socket */
    /* htonl converts a long integer (e.g. address) to a network */
    /* representation (agreed-upon byte ordering */

    memset((char *) &myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);

    if (bind(fd, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }

    /* this part is for debugging only - get the port # that the operating */
    /* system allocated for us. */
    alen = sizeof(myaddr);
    if (getsockname(fd, (struct sockaddr *) &myaddr, &alen) < 0) {
        perror("getsockname failed");
        return 0;
    }

    /* fill in the server's address and data */
    /* htons() converts a short integer to a network representation */

    memset((char *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    /* look up the address of the server given its name */
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
        return 0;
    }

    /* put the host's address into the server address structure */
    memcpy((void *) &servaddr.sin_addr, hp->h_addr_list[0], (size_t) hp->h_length);

    /* connect to server */
    if (connect(fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        return 0;
    }

    char buffer[BUF_SIZE];


    for (int i = 0; i < text_length; i++) {
        int random_integer = (int) 'a' + rand() % ((int) 'z' - (int) 'a');
        buffer[i] = (char) random_integer;
    }
    cout << "\nGenerated text:\n" << buffer << endl;

    // Sending data
    serialize(buffer);
    ssize_t n = send(fd, buffer, BUF_SIZE, 0);
    if (n == -1) {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Receiving data
    n = recv(fd, buffer, BUF_SIZE, 0);
    if (n == -1) {
        perror("ERROR reading from socket");
        exit(1);
    }
    deserialize(buffer);
    cout << "\nPalindromes in this text:\n" << buffer << endl;

    close(fd);

    return 0;
}
