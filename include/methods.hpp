/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostian <akostian@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:19:43 by akostian          #+#    #+#             */
/*   Updated: 2025/09/04 20:19:45 by akostian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
#define METHODS_HPP

enum methods { GET, POST, DELETE };

inline const char* methodToString(const methods& m) {
    switch (m) {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case DELETE:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}

#endif  // METHODS_HPP
