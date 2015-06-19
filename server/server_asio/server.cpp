//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BUF_SIZE 10000

#include <ctime>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../../task/task.hpp"
#include "../../serialization/serialization.hpp"

using namespace boost::archive;
using boost::asio::ip::tcp;
using namespace std;

long client_number = 0;

class tcp_connection
        : public boost::enable_shared_from_this<tcp_connection> {
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service &io_service) {
        return pointer(new tcp_connection(io_service));
    }

    tcp::socket &socket() {
        return socket_;
    }

    void start() {
        client_number++;
        cout << "Got new client" << endl;

        boost::array<char, BUF_SIZE> buf;
        boost::system::error_code error;

        size_t len = socket_.read_some(boost::asio::buffer(buf), error);
        if (len <= 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        char buffer[BUF_SIZE];
        strcpy(buffer, buf.c_array());

        deserialize(buffer);
        strcpy(buffer, palindromes(buffer).c_str());
        serialize(buffer);

        boost::asio::async_write(socket_, boost::asio::buffer(buffer),
                                 boost::bind(&tcp_connection::handle_write,
                                             shared_from_this(), boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));

        cout << client_number << ". Finished with him" << endl;
    }

private:
    tcp_connection(boost::asio::io_service &io_service)
            : socket_(io_service) {
    }

    void handle_write(const boost::system::error_code & /*error*/,
                      size_t /*bytes_transferred*/) {
    }

    tcp::socket socket_;
};

class tcp_server {
public:
    tcp_server(boost::asio::io_service &io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), 3000)) {
        start_accept();
    }

private:
    void start_accept() {
        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code &error) {
        if (!error) {
            new_connection->start();
            start_accept();
        }
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}