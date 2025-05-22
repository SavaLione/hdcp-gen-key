/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright (C) 2025 Savelii Pototskii (savalione.com)
 *
 * Author: Savelii Pototskii <savelii.pototskii@gmail.com>
 *
 * This file is part of hdcp-gen-key.
 *
 * hdcp-gen-key is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * hdcp-gen-key is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hdcp-gen-key. If not, see <https://www.gnu.org/licenses/>.
*/
/**
 * @file hdcp.h
 * @brief Defines interfaces for HDCP (High-bandwidth Digital Content Protection, versions 1.0-1.4) key generation, KSV handling, and data formatting.
 * @author Savelii Pototskii
 * @date 2025-05-22
 * @copyright Copyright (C) 2025 Savelii Pototskii (savalione.com)
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
*/
#ifndef HDCP_H
#define HDCP_H

#include <array>
#include <bitset>

/**
 * @brief Generates the source HDCP key (HDCP versions 1.0-1.4).
 * @param[in] ksv Key Selection Vector (KSV).
 * @param[in] key The Master Key Matrix.
 * @return The source HDCP key.
 * @warning Supports not valid ksv keys.
*/
std::array<std::bitset<56>, 40> generate_source(std::bitset<40> const &ksv, std::array<std::bitset<56>, 1600> const &key);

/**
 * @brief Generates the sink HDCP key (HDCP versions 1.0-1.4).
 * @param[in] ksv Key Selection Vector (KSV).
 * @param[in] key The Master Key Matrix.
 * @return The sink HDCP key.
 * @warning Supports not valid ksv keys.
*/
std::array<std::bitset<56>, 40> generate_sink(std::bitset<40> const &ksv, std::array<std::bitset<56>, 1600> const &key);

/**
 * @brief Converts a `std::bitset` to its hexadecimal string representation.
 *
 * @tparam bits The number of bits in the input `std::bitset`.
 * @param[in] num The bitset to convert.
 * @return A string containing the hexadecimal representation of the input bitset.
 *
 * @pre The template parameter `bits` must be greater than 0.
 * @pre The template parameter `bits` must be a multiple of 4.
 * @pre The template parameter `bits` must be only 40 or 56. To add new values, add an overload in `hdcp.cpp`.
 *
 * @note For example, a bitset `1010101111001101` (binary) would be converted to "abcd" (hex).
*/
template<std::size_t bits>
std::string bitset_to_hex(std::bitset<bits> const &num);

/**
 * @brief Converts a `std::string` to its `std::bitset` representation.
 *
 * @tparam bits The number of bits in the input `std::bitset`.
 * @param[in] s The string to convert.
 * @return A bitset containing the bitset representation of the input string.
 *
 * @pre The template parameter `bits` must be greater than 0.
 * @pre The template parameter `bits` must be a multiple of 4.
 * @pre The template parameter `bits` must be less or equal 64.
 * @pre The template parameter `bits` must be only 40, 56 or 64. To add new values, add an overload in `hdcp.cpp`.
 *
 * @note For example, a string `abcd` (hex) would be converted to '1010101111001101' (binary).
*/
template<std::size_t bits>
std::bitset<bits> ksv_string_to_bitset(std::string const &s);

/**
 * @brief Checks if the provided ksv is correct.
 *
 * @param[in] ksv Key Selection Vector (KSV).
 * @return True if the ksv is correct, false if it is not. 
 *
 * @note Example: `0x00000fffff` is correct (true), but `0x00000aaaa0` is not (false).
*/
bool check_ksv(std::bitset<40> const &ksv);

/**
 * @brief Generates a random valid 40-bit Key Selection Vector (KSV).
 * @return A random valid 40-bit KSV.
 */
std::bitset<40> random_ksv();

/**
 * @brief Supported output formats.
 */
enum formatted_out_type
{
    TEXT_INFORMATIONAL,
    TEXT_SOURCE_ONLY,
    TEXT_SINK_ONLY,
    TEXT_SOURCE_KSV_ONLY,
    TEXT_SINK_KSV_ONLY,
    TEXT_LINE_SOURCE,
    TEXT_LINE_SINK,
    TEXT_FULL,
    JSON,
    JSON_FULL,
    YAML,
    YAML_FULL,
    XML,
    XML_FULL,
    TOML,
    TOML_FULL,
    NOT_FOUND
};

/**
 * @brief Converts a string representation to a formatted_out_type enum.
 * @param[in] s The input string to convert.
 * @return The corresponding formatted_out_type enum value.
*/
formatted_out_type string_to_fot(std::string const &s);

/**
 * @brief Generates HDCP source and sink keys, stores ksv, source, sink and the Master Key Matrix.
*/
class hdcp
{
public:
    /**
     * @brief Constructs an hdcp object and initializes its internal state.
     * @param[in] key Array that contains Master Key Matrix.
     * @param[in] ksv Key Selection Vector (KSV).
    */
    hdcp(std::array<std::bitset<56>, 1600> const &key, std::bitset<40> const &ksv) : hdcp_key(key), ksv(ksv)
    {
        source = generate_source(ksv, hdcp_key);
        sink   = generate_sink(ksv, hdcp_key);
    };

    /**
     * @brief Formats the HDCP data (source, sink, KSV) into a string.
     * @param[in] t Desired output format.
     * @return A string that contains the HDCP data formatted according to the type 't'.
     *
     * @see formatted_out_type
     * @see string_to_fot()
    */
    std::string formatted(formatted_out_type const &t);

private:
    std::array<std::bitset<56>, 1600> const &hdcp_key;
    std::bitset<40> ksv;
    std::array<std::bitset<56>, 40> source;
    std::array<std::bitset<56>, 40> sink;
};

#endif // HDCP_H