/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:19:52 by akostian          #+#    #+#             */
/*   Updated: 2025/09/14 02:50:59 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "Http.hpp"
#include "Response.hpp"
#include "colors.hpp"

struct Location {
    std::set<Http::Method::Type> accepted_methods;
    bool                         directory_listing;
    std::string                  root;
    std::string                  default_index;
    std::string                  upload_path;
    unsigned short               redirect_code;
    std::string                  redirect_target;
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

Response    responseBuilder(ServerConfig &config, char *buffer);
std::string DirectoryListing(const std::string &request_path, const std::string &resposne_path);

#endif  // WEBSERV_HPP
