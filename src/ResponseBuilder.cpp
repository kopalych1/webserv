/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:21:55 by akostian          #+#    #+#             */
/*   Updated: 2025/09/17 10:19:18 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <unistd.h>  // access

#include <fstream>
#include <sstream>
#include <string>

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

std::string buildErrorPage(ServerConfig &config, Http::Status::Code code) {
    if (config.error_pages_paths.find(code) != config.error_pages_paths.end()) {
        const std::string error_page_path = config.error_pages_paths[code];
        if (fileExists(error_page_path) && access(error_page_path.c_str(), R_OK))
            return readFileToString(error_page_path);
    }

    std::ostringstream oss;

    oss << "<html><head><title>" << code << " " << Http::reasonPhrase(code)
        << "</title></head><body>";
    oss << "<center><span style=\"color: red;\"><h1>" << code << " " << Http::reasonPhrase(code)
        << "</h1></span></center>";

    return oss.str();
}

Response responseBuilder(ServerConfig &config, char *buffer) {
    std::stringstream b_ss(buffer);

    std::string method_type;
    getline(b_ss, method_type, ' ');
    (void)method_type;

    std::string request_path;
    getline(b_ss, request_path, ' ');

    // TODO: Location selection (simple longest prefix match)
    Location &location = config.locations[0];

    std::string resposne_path = location.root + request_path;

    if (*resposne_path.rbegin() != '/' && !fileExists(resposne_path) &&
        dirExists(resposne_path + "/")) {
        Response res(Http::Status::MovedPermanently, "");
        res.setLocation(request_path + "/");
        return res;
    }

    if (dirExists(resposne_path)) {
        if (location.directory_listing) {
            if (access(resposne_path.c_str(), R_OK))
                return Response(Http::Status::Forbidden,
                                buildErrorPage(config, Http::Status::Forbidden),
                                Http::ContentType::TEXT_HTML);

            return Response(Http::Status::OK, DirectoryListing(request_path, resposne_path),
                            Http::ContentType::TEXT_HTML);
        }
        resposne_path += location.default_index;
    }

    if (!fileExists(resposne_path))
        return Response(Http::Status::NotFound, buildErrorPage(config, Http::Status::NotFound),
                        Http::ContentType::TEXT_HTML);

    if (access(resposne_path.c_str(), R_OK))
        return Response(Http::Status::Forbidden, buildErrorPage(config, Http::Status::Forbidden),
                        Http::ContentType::TEXT_HTML);

    return Response(Http::Status::OK, readFileToString(resposne_path),
                    Http::contentTypeFromFile(resposne_path));
}
