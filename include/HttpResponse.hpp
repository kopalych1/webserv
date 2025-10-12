/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 02:57:18 by akostian          #+#    #+#             */
/*   Updated: 2025/10/12 05:32:39 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <sys/types.h>  //ssize_t

#include "Http.hpp"
#include "HttpPacket.hpp"

class HttpResponse : public HttpPacket {
   private:
    Http::Status::Code      status_code_;
    Http::ContentType::Type content_type_;
    std::string             location_;

   public:
    HttpResponse();
    HttpResponse(Http::Status::Code status_code, const std::string& body);
    HttpResponse(Http::Status::Code status_code, const std::string& body,
                 Http::ContentType::Type content_type);
    HttpResponse(const HttpResponse& other);
    HttpResponse& operator=(const HttpResponse& other);
    ~HttpResponse(){};

    virtual std::string startLine() const;

    static ssize_t sendResponse(int client_fd, HttpResponse& res);

    /* Getters/Setters */

    Http::ContentType::Type getContentType() const { return content_type_; }
    const std::string&      getLocation() const { return location_; }
    Http::Status::Code      getStatusCode() const { return status_code_; }

    void setContentType(Http::ContentType::Type type) { content_type_ = type; }
    void setLocation(const std::string& location) { location_ = location; }
    void setStatusCode(Http::Status::Code code) { status_code_ = code; }
};

#endif  // HTTPRESPONSE_HPP
