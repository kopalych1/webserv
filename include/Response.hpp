/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:19:48 by akostian          #+#    #+#             */
/*   Updated: 2025/09/12 03:17:25 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sys/types.h>  // ssize_t

#include <string>

#include "Http.hpp"

class Response {
   public:
    Response();
    Response(Http::Status::Code status_code, std::string body);
    Response(Http::Status::Code status_code, std::string body,
             Http::ContentType::Type content_type);
    Response(const Response& other);
    Response& operator=(const Response& other);
    ~Response();

    static ssize_t sendResponse(int client_fd, const Response& res);

    std::string toString() const;

    Http::Status::Code getStatusCode() const { return this->status_code_; }
    void               setStatusCode(Http::Status::Code code) { this->status_code_ = code; }
    void               setBody(const std::string& body) { this->body_ = body; }
    void setContentType(const Http::ContentType::Type type) { this->content_type_ = type; }
    void setLocation(const std::string& location) { this->location_ = location; }

   private:
    Http::Status::Code      status_code_;
    std::string             body_;
    Http::ContentType::Type content_type_;
    std::string             location_;
};

#endif  // RESPONSE_HPP
