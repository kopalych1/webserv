/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:21:55 by akostian          #+#    #+#             */
/*   Updated: 2025/09/12 03:21:07 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>

#include <fstream>
#include <sstream>

#include "../include/webserv.hpp"

// Check if file exists
inline bool fileExists(const std::string &path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

// Check if dir exists
inline bool dirExists(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;  // No access or doesn't exist
    return (info.st_mode & S_IFDIR) != 0;
}

// Helper function to read a file into a string
inline std::string readFileToString(const std::string &path) {
    std::ifstream      file(path.c_str(), std::ios::in | std::ios::binary);
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

    if (*resposne_path.rbegin() != '/' && dirExists(resposne_path + "/")) {
        Response res(Http::Status::MovedPermanently, "");
        res.setLocation(request_path + "/");
        return res;
    }

    if (config.locations[0].directory_listing && dirExists(resposne_path))
        return Response(Http::Status::OK, DirectoryListing(request_path, resposne_path),
                        Http::ContentType::TEXT_HTML);

    // If resposne_path is a directory, append default index
    if (*resposne_path.rbegin() == '/') resposne_path += config.locations[0].default_index;

    std::string body = readFileToString(resposne_path);
    if (!body.empty())
        return Response(Http::Status::OK, body, Http::contentTypeFromFile(resposne_path));
    if (fileExists(resposne_path))
        return Response(Http::Status::OK, "");  // Requested file is empty

    return Response(Http::Status::NotFound, "<html><body><h1>404 Not Found</h1></body></html>",
                    Http::ContentType::TEXT_HTML);
}
