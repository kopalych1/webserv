/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseFormData.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 07:54:09 by akostian          #+#    #+#             */
/*   Updated: 2025/10/29 13:58:32 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEFORMDATA_HPP
#define PARSEFORMDATA_HPP

#include <map>
#include <string>
#include <vector>

#include "HttpRequest.hpp"

struct UploadedFile {
    std::string                name;         // form field name
    std::string                filename;     // filename
    Http::ContentType::Type    contentType;  // file ContentType
    std::vector<unsigned char> data;         // file bytes
};

struct FormData {
    std::map<std::string, std::string>                fields;  // form fields
    std::map<std::string, std::vector<UploadedFile> > files;   // files
};

FormData parseMultipartFormData(const HttpRequest& request);

#endif  // PARSEFORMDATA_HPP
