/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:22:51 by akostian          #+#    #+#             */
/*   Updated: 2025/09/06 06:33:09 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

Response::Response() : status_code_(0), status_message_(""), body_("") {}

Response::Response(unsigned short status_code, std::string status_message, std::string body)
    : status_code_(status_code), status_message_(status_message), body_(body) {}

Response::Response(const Response& other) { this->operator=(other); }

Response& Response::operator=(const Response& other) {
    if (this != &other) {
        this->status_code_    = other.status_code_;
        this->status_message_ = other.status_message_;
        this->body_           = other.body_;
    }
    return *this;
}

Response::~Response() {}

std::string Response::toString() const {
    std::ostringstream response;

    if (this->status_code_ == 0) return "";

    if (this->status_code_ == 301) {
        response << "HTTP/1.1 " << this->status_code_ << " " << this->status_message_ << "\r\n";
        response << "Location: " << this->body_ << "\r\n";  // body_ holds the redirect URL
        response << "Content-Length: 0\r\n";
        response << "Connection: close\r\n";

        return response.str();
    }

    response << "HTTP/1.1 " << this->status_code_ << " " << this->status_message_ << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << this->body_.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << this->body_;

    return response.str();
}

ssize_t write(int client_fd, Response res) {
    std::string str = res.toString();
    return write(client_fd, str.c_str(), str.size());
}
