/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:21:52 by akostian          #+#    #+#             */
/*   Updated: 2025/09/17 10:12:00 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../include/webserv.hpp"

inline std::string currentTimeString() {
    std::time_t now = std::time(NULL);
    std::tm     tm_now;
    localtime_r(&now, &tm_now);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &tm_now);
    return std::string(buf);
}

void prettyPrintResponse(const Response &res) {
    const unsigned short code = res.getStatusCode();

    if (code >= 200 && code < 300)
        std::cout << colors::green;
    else if (code >= 300 && code < 400)
        std::cout << colors::yellow;
    else if (code >= 400 && code < 500)
        std::cout << colors::red;
    else
        std::cout << colors::magenta;
    std::cout << code << colors::reset;
}

void logRequest(const char *buffer, const Response &res) {
    std::stringstream b_ss(buffer);

    std::string method_type;
    getline(b_ss, method_type, ' ');

    std::string request_path;
    getline(b_ss, request_path, ' ');

    std::cout << "[" << colors::black << currentTimeString() << colors::reset << "] ";

    if (method_type == "GET")
        std::cout << colors::green;
    else if (method_type == "POST")
        std::cout << colors::yellow;
    else if (method_type == "DELETE")
        std::cout << colors::red;
    else
        std::cout << colors::magenta;

    std::cout << method_type << " " << colors::cyan << request_path << colors::reset << " ";

    prettyPrintResponse(res);

    std::cout << "\n";
}

void initServer(ServerConfig &config) {
    std::set<Http::Method::Type> allowed_methods;
    allowed_methods.insert(Http::Method::GET);
    allowed_methods.insert(Http::Method::POST);
    allowed_methods.insert(Http::Method::DELETE);

    std::vector<std::string> cgi_types;

    config.error_pages_paths[Http::Status::Forbidden]           = "./www/error_pages/403.html";
    config.error_pages_paths[Http::Status::NotFound]            = "./www/error_pages/404.html";
    config.error_pages_paths[Http::Status::InternalServerError] = "./www/error_pages/500.html";

    // cgi_types.push_back("*.py");
    // cgi_types.push_back("*.php");

    Location loc = {allowed_methods, false, "./www", "index.html", "./www", 0, "", cgi_types};

    config.locations.push_back(loc);
}

int main() {
    //////////////////////////////////////////////////
    // Simple demo server
    //////////////////////////////////////////////////

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "socket() failed\n";
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed\n";
        close(server_fd);
        return 1;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed\n";
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "listen() failed\n";
        close(server_fd);
        return 1;
    }
    std::cout << "Test server running on http://localhost:8080" << "\n";

    //////////////////////////////////////////////////

    ServerConfig config;
    initServer(config);

    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            std::cerr << "accept() failed" << "\n";
            continue;
        }

        char buffer[1024];
        int  n = read(client_fd, buffer, sizeof(buffer));
        if (n < 1) continue;
        buffer[n] = '\0';

        Response res = responseBuilder(config, buffer);

        logRequest(buffer, res);

        Response::sendResponse(client_fd, res);

        shutdown(client_fd, SHUT_WR);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
