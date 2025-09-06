/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:21:55 by akostian          #+#    #+#             */
/*   Updated: 2025/09/06 06:34:54 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>

#include <fstream>

#include "../include/webserv.hpp"

// Check if dir exists
inline bool isDirectory(const char *path) {
    struct stat info;
    if (stat(path, &info) != 0) return false;  // No access or doesn't exist
    return (info.st_mode & S_IFDIR) != 0;
}

// Helper function to read a file into a string
inline std::string readFileToString(const std::string &path) {
    std::ifstream      file(path.c_str());
    std::ostringstream ss;
    if (file) ss << file.rdbuf();
    return ss.str();
}

Response responseBuilder(ServerConfig &config, char *buffer) {
    std::stringstream b_ss(buffer);

    std::string method_type;
    getline(b_ss, method_type, ' ');
    (void)method_type;

    std::string request_path;
    getline(b_ss, request_path, ' ');

    std::string resposne_path = config.locations[0].root + request_path;

    if (*resposne_path.rbegin() == '/') resposne_path += config.locations[0].default_index;

    std::string body = readFileToString(resposne_path);
    if (!body.empty()) return Response(200, "OK", body);

    // TODO: redirect if directory and no trailing slash (301)

    return Response(404, "Not Found", "<html><body><h1>404 Not Found</h1></body></html>");
}
