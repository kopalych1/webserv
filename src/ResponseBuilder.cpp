/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:21:55 by akostian          #+#    #+#             */
/*   Updated: 2025/10/24 22:57:31 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <unistd.h>  // access

#include <fstream>
#include <sstream>
#include <string>

#include "../include/ParseFormData.hpp"
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
        if (fileExists(error_page_path) && !access(error_page_path.c_str(), R_OK))
            return readFileToString(error_page_path);
    }

    std::ostringstream oss;

    oss << "<html><head><title>" << code << " " << Http::reasonPhrase(code)
        << "</title></head><body>";
    oss << "<center><span style=\"color: red;\"><h1>" << code << " " << Http::reasonPhrase(code)
        << "</h1></span></center>";

    return oss.str();
}

/**
 * @brief Joins two path components into a single path, handling slashes.
 *
 * @param root The first (root) part of the path.
 * @param tail The second (tail) part of the path.
 * @return std::string The combined path.
 */
std::string joinPaths(const std::string &root, const std::string &tail) {
    if (root.empty()) return tail;
    if (tail.empty()) return root;

    if (root[root.size() - 1] == '/' && tail[0] == '/')
        return root + tail.substr(1);
    else if (root[root.size() - 1] != '/' && tail[0] != '/')
        return root + "/" + tail;
    else
        return root + tail;
}

// Returns a iterator to the selected location requested by request_path
std::vector<Location>::iterator chooseLocation(std::vector<Location> &locations,
                                               const std::string     &request_path,
                                               std::size_t           &longest_prefix_len) {
    std::vector<Location>::iterator longest_location = locations.end();

    longest_prefix_len = 0;
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it) {
        const std::string &path = (*it).path;

        size_t j = 0;
        while (j < path.size() && j < request_path.size() && (path[j] == request_path[j])) ++j;

        if (j > longest_prefix_len && ((j + 1) >= path.size())) {
            longest_prefix_len = j;
            longest_location   = it;
        }
    }
    return longest_location;
}

// Helper function that handles POST request
// returns (true, HttpResponse) if request was bad
static std::pair<bool, HttpResponse> handlePostUploads(ServerConfig   &config,
                                                       const Location &location, HttpRequest &req,
                                                       Http::Status::Code &successStatus) {
    if (location.upload_path.empty()) {
        // Forbidden(403) if upload not allowed
        return std::make_pair(true, HttpResponse(Http::Status::Forbidden,
                                                 buildErrorPage(config, Http::Status::Forbidden),
                                                 Http::ContentType::TEXT_HTML));
    }

    FormData data = parseMultipartFormData(req);

    // Checking files for same filenames first
    // TODO: Upload file with randomly generated filename end
    for (std::map<std::string, std::vector<UploadedFile> >::iterator it = data.files.begin();
         it != data.files.end(); ++it) {
        for (std::vector<UploadedFile>::iterator jt = it->second.begin(); jt != it->second.end();
             ++jt) {
            if (jt->filename.empty()) continue;

            std::string filePath = joinPaths(location.upload_path, jt->filename);
            if (fileExists(filePath))
                return std::make_pair(true,
                                      HttpResponse(Http::Status::Conflict,
                                                   buildErrorPage(config, Http::Status::Conflict),
                                                   Http::ContentType::TEXT_HTML));
        }
    }

    // Saving files
    for (std::map<std::string, std::vector<UploadedFile> >::iterator it = data.files.begin();
         it != data.files.end(); ++it) {
        for (std::vector<UploadedFile>::iterator jt = it->second.begin(); jt != it->second.end();
             ++jt) {
            if (jt->filename.empty()) continue;

            std::string   filePath = joinPaths(location.upload_path, jt->filename);
            std::ofstream outFile(filePath.c_str());
            if (!outFile.is_open()) continue;

            std::vector<unsigned char> &data = jt->data;

            outFile.write(reinterpret_cast<const char *>(data.data()), data.size());
            if (!outFile.is_open())
                return std::make_pair(
                    true, HttpResponse(Http::Status::InternalServerError,
                                       buildErrorPage(config, Http::Status::InternalServerError),
                                       Http::ContentType::TEXT_HTML));
            outFile.close();
        }
    }
    successStatus = Http::Status::Created;
    return std::make_pair(false, HttpResponse());
}

HttpResponse responseBuilder(ServerConfig &config, HttpRequest &req) {
    std::string request_path = req.getPath();

    std::size_t                     longest_prefix;
    std::vector<Location>::iterator location_it =
        chooseLocation(config.locations, request_path, longest_prefix);

    if (location_it == config.locations.end())
        return HttpResponse(Http::Status::InternalServerError,
                            buildErrorPage(config, Http::Status::InternalServerError),
                            Http::ContentType::TEXT_HTML);

    Location   &location      = *location_it;
    std::string resposne_path = joinPaths(location.root, request_path.substr(longest_prefix));

    if (location.accepted_methods.find(req.getMethod()) == location.accepted_methods.end())
        return HttpResponse(Http::Status::MethodNotAllowed,
                            buildErrorPage(config, Http::Status::MethodNotAllowed),
                            Http::ContentType::TEXT_HTML);

    Http::Status::Code successStatus = Http::Status::OK;

    if (req.getMethod() == Http::Method::POST) {
        std::pair<bool, HttpResponse> postRes =
            handlePostUploads(config, location, req, successStatus);
        if (postRes.first) return postRes.second;
    }

    // Returning MovedPermanently(301) if request ends with / and file doesn't exist
    // Example:  /dir  ->  /dir/
    if (*request_path.rbegin() != '/' && !fileExists(resposne_path) &&
        dirExists(resposne_path + "/")) {
        HttpResponse res(Http::Status::MovedPermanently, "");
        res.setLocation(request_path + "/");
        return res;
    }

    if (dirExists(resposne_path)) {
        if (location.directory_listing) {
            // Directory Listing
            if (access(resposne_path.c_str(), R_OK))
                return HttpResponse(Http::Status::Forbidden,
                                    buildErrorPage(config, Http::Status::Forbidden),
                                    Http::ContentType::TEXT_HTML);

            return HttpResponse(Http::Status::OK, DirectoryListing(request_path, resposne_path),
                                Http::ContentType::TEXT_HTML);
        }
        // Example:  /dir/  ->  /dir/index.html
        resposne_path += location.default_index;
    }

    if (!fileExists(resposne_path))
        return HttpResponse(Http::Status::NotFound, buildErrorPage(config, Http::Status::NotFound),
                            Http::ContentType::TEXT_HTML);

    if (access(resposne_path.c_str(), R_OK))
        return HttpResponse(Http::Status::Forbidden,
                            buildErrorPage(config, Http::Status::Forbidden),
                            Http::ContentType::TEXT_HTML);

    return HttpResponse(successStatus, readFileToString(resposne_path),
                        Http::contentTypeFromFile(resposne_path));
}
