/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:19:48 by akostian          #+#    #+#             */
/*   Updated: 2025/09/06 06:38:38 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <unistd.h>

#include <sstream>
#include <string>

class Response {
   private:
    unsigned short status_code_;
    std::string    status_message_;
    std::string    body_;

   public:
    Response();
    Response(unsigned short status_code, std::string status_message, std::string body);
    Response(const Response& other);
    Response& operator=(const Response& other);
    ~Response();

    friend ssize_t write(int client_fd, Response res);

    std::string toString() const;

    void           setStatusCode(unsigned short code) { this->status_code_ = code; }
    unsigned short getStatusCode() const { return this->status_code_; }
    void           setStatusMessage(const std::string& message) { this->status_message_ = message; }
    void           setBody(const std::string& body) { this->body_ = body; }
};

#endif  // RESPONSE_HPP
