/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:19:52 by akostian          #+#    #+#             */
/*   Updated: 2025/10/12 11:03:16 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "Http.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include "colors.hpp"

struct Location {
    std::set<Http::Method::Type> accepted_methods;
    bool                         directory_listing;
    std::string                  path;  // Url prefix
    std::string                  root;  // Filesystem root
    std::string                  default_index;
    std::string                  upload_path;
    std::vector<std::string>     cgi_types;
};

struct ServerConfig {
    std::string                               server_name;
    std::string                               interface;
    unsigned short                            port;
    std::map<Http::Status::Code, std::string> error_pages_paths;
    std::size_t                               body_size;
    std::vector<Location>                     locations;
};

HttpResponse responseBuilder(ServerConfig &config, HttpRequest &req);
std::string  DirectoryListing(const std::string &request_path, const std::string &resposne_path);

#endif  // WEBSERV_HPP
