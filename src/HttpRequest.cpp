/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 23:21:53 by akostian          #+#    #+#             */
/*   Updated: 2025/10/12 11:17:11 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpRequest.hpp"

HttpRequest::HttpRequest() : HttpPacket(), method_(Http::Method::GET), path_("/") {}

HttpRequest::HttpRequest(const HttpRequest& other)
    : HttpPacket(other), method_(other.method_), path_(other.path_) {}

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    if (this != &other) {
        HttpPacket::operator=(other);

        this->method_ = other.method_;
        this->path_   = other.path_;
    }
    return *this;
}

std::string HttpRequest::startLine() const {
    std::ostringstream out;

    out << Http::methodToString(method_) << " " << path_ << " " << version_;
    return out.str();
}
