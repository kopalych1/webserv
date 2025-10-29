/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPacket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 02:09:48 by akostian          #+#    #+#             */
/*   Updated: 2025/10/29 13:56:14 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpPacket.hpp"

#include <map>
#include <sstream>
#include <string>

std::string HttpPacket::toString() const {
    std::ostringstream ss;
    ss << startLine() << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = headers_.begin();
         it != headers_.end(); ++it) {
        ss << it->first << ": " << it->second << "\r\n";
    }
    ss << "\r\n";
    return ss.str();
}

std::string HttpPacket::getHeader(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = headers_.find(key);
    return (it != headers_.end()) ? it->second : "";
}
std::string HttpPacket::headersToString() const {
    std::ostringstream ss;
    for (std::map<std::string, std::string>::const_iterator it = headers_.begin();
         it != headers_.end(); ++it)
        ss << it->first << ": " << it->second << "\r\n";
    ss << "\r\n";
    return ss.str();
}

std::string HttpPacket::fullPacket() const {
    std::string out = this->toString();
    out.insert(out.end(), body_.begin(), body_.end());
    return out;
}
