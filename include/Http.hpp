/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 02:43:15 by akostian          #+#    #+#             */
/*   Updated: 2025/10/11 00:27:39 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>

namespace Http {

// HTTP methods
struct Method {
    enum Type { GET, POST, DELETE, UNKNOWN };
};

// Status codes
struct Status {
    enum Code {
        OK                  = 200,
        Created             = 201,
        NoContent           = 204,
        MovedPermanently    = 301,
        BadRequest          = 400,
        Unauthorized        = 401,
        Forbidden           = 403,
        NotFound            = 404,
        MethodNotAllowed    = 405,
        InternalServerError = 500,
        NotImplemented      = 501,
        BadGateway          = 502,
        ServiceUnavailable  = 503
    };
};

// Content types
struct ContentType {
    enum Type {
        TEXT_HTML,
        TEXT_PLAIN,
        TEXT_CSS,
        APPLICATION_JSON,
        APPLICATION_JAVASCRIPT,
        IMAGE_PNG,
        IMAGE_JPEG,
        IMAGE_GIF,
        OCTET_STREAM
    };
};

/**
 * @brief Convert Method enum to string
 *
 * @param m Method enum value
 * @return const char* string representation of the method
 */
inline const char* methodToString(const Http::Method::Type& m) {
    switch (m) {
        case Http::Method::GET: return "GET";
        case Http::Method::POST: return "POST";
        case Http::Method::DELETE: return "DELETE";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Convert string to Method enum
 *
 * @param str string value
 * @return const Method enum representation of the method
 */
inline Http::Method::Type methodFromString(const std::string& str) {
    std::string upper(str);

    for (size_t i = 0; i < str.size(); i++) upper[i] = std::toupper(upper[i]);

    if (upper == "GET")
        return Http::Method::GET;
    else if (upper == "POST")
        return Http::Method::POST;
    else if (upper == "DELETE")
        return Http::Method::DELETE;
    return Http::Method::UNKNOWN;
}

/**
 * @brief Get Reason phrase from status code
 *
 * @param code HTTP status code
 * @return const char* standard reason phrase according to RFC 7231
 */
inline const char* reasonPhrase(unsigned int code) {
    switch (code) {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 413: return "Payload Too Large";
        case 415: return "Unsupported Media Type";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        default: return "";
    }
}

/**
 * @brief Get ContentType from file extension
 *
 * @param filename filename or path
 * @return Response::ContentType::Type type of content
 */
inline Http::ContentType::Type contentTypeFromFile(const std::string& filename) {
    std::size_t dotPos = filename.rfind('.');
    if (dotPos == std::string::npos) return Http::ContentType::OCTET_STREAM;

    std::string ext = filename.substr(dotPos);

    if (ext == ".html" || ext == ".htm") return ContentType::TEXT_HTML;
    if (ext == ".txt") return ContentType::TEXT_PLAIN;
    if (ext == ".css") return ContentType::TEXT_CSS;
    if (ext == ".json") return ContentType::APPLICATION_JSON;
    if (ext == ".js") return ContentType::APPLICATION_JAVASCRIPT;
    if (ext == ".png") return ContentType::IMAGE_PNG;
    if (ext == ".jpg" || ext == ".jpeg") return ContentType::IMAGE_JPEG;
    if (ext == ".gif") return ContentType::IMAGE_GIF;

    return ContentType::OCTET_STREAM;
}

/**
 * @brief Get string representation of ContentType
 *
 * @param type ContentType enum value
 * @return const char* string representation of ContentType
 */
inline const char* contentTypeToString(ContentType::Type type) {
    switch (type) {
        case Http::ContentType::TEXT_HTML: return "text/html";
        case Http::ContentType::TEXT_PLAIN: return "text/plain";
        case Http::ContentType::TEXT_CSS: return "text/css";
        case Http::ContentType::APPLICATION_JSON: return "application/json";
        case Http::ContentType::APPLICATION_JAVASCRIPT: return "application/javascript";
        case Http::ContentType::IMAGE_PNG: return "image/png";
        case Http::ContentType::IMAGE_JPEG: return "image/jpeg";
        case Http::ContentType::IMAGE_GIF: return "image/gif";
        default: return "application/octet-stream";
    }
}

}  // namespace Http

#endif  // HTTP_HPP
