/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 23:14:39 by akostian          #+#    #+#             */
/*   Updated: 2025/10/12 11:14:28 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Http.hpp"
#include "HttpPacket.hpp"

class HttpRequest : public HttpPacket {
   private:
    Http::Method::Type method_;
    std::string        path_;

   public:
    HttpRequest();
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest(){};

    std::string startLine() const;

    /* Getters/Setters */

    const std::string& getPath() const { return path_; }
    Http::Method::Type getMethod() const { return method_; }

    void setMethod(Http::Method::Type m) { method_ = m; }
    void setPath(const std::string& p) { path_ = p; }
};

#endif  // HTTPREQUEST_HPP
