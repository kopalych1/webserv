/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:22:51 by akostian          #+#    #+#             */
/*   Updated: 2025/09/14 02:48:52 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>

Response::Response()
    : status_code_(Http::Status::InternalServerError),
      body_(""),
      content_type_(Http::ContentType::OCTET_STREAM),
      location_("") {}

Response::Response(Http::Status::Code status_code, std::string body)
    : status_code_(status_code),
      body_(body),
      content_type_(Http::ContentType::TEXT_HTML),
      location_("") {}

Response::Response(Http::Status::Code status_code, std::string body,
                   Http::ContentType::Type content_type)
    : status_code_(status_code), body_(body), content_type_(content_type), location_("") {}

Response::Response(const Response& other) { this->operator=(other); }

Response& Response::operator=(const Response& other) {
    if (this != &other) {
        this->status_code_  = other.status_code_;
        this->body_         = other.body_;
        this->content_type_ = other.content_type_;
        this->location_     = other.location_;
    }
    return *this;
}

Response::~Response() {}

std::string Response::headersToString() const {
    std::ostringstream response;

    if (this->status_code_ == Http::Status::InternalServerError) return "";

    if (this->status_code_ == Http::Status::MovedPermanently) {
        response << "HTTP/1.1 " << this->status_code_ << " "
                 << Http::reasonPhrase(this->status_code_) << "\r\n";
        response << "Location: " << this->location_ << "\r\n";
        response << "Content-Length: 0\r\n";
        response << "Connection: close\r\n";

        return response.str();
    }

    response << "HTTP/1.1 " << this->status_code_ << " " << Http::reasonPhrase(this->status_code_)
             << "\r\n";
    response << "Content-Type: " << Http::contentTypeToString(this->content_type_) << "\r\n";
    response << "Content-Length: " << this->body_.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";

    return response.str();
}

ssize_t Response::sendResponse(int client_fd, const Response& res) {
    const std::string& headers   = res.headersToString();
    ssize_t            totalSent = 0;
    const ssize_t      bodySize  = res.body_.size();
    const char*        body      = res.body_.data();

    if (::write(client_fd, headers.c_str(), headers.size()) <= 0) return -1;

    while (totalSent < bodySize) {
        ssize_t sent = ::write(client_fd, body + totalSent, bodySize - totalSent);
        if (sent <= 0) return -1;
        totalSent += sent;
    }
    return totalSent + headers.size();
}
