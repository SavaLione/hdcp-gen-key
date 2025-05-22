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
 * @file hdcp.cpp
 * @brief Defines interfaces for HDCP (High-bandwidth Digital Content Protection, versions 1.0-1.4) key generation, KSV handling, and data formatting.
 * @author Savelii Pototskii
 * @date 2025-05-22
 * @copyright Copyright (C) 2025 Savelii Pototskii (savalione.com)
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
*/
#include "hdcp.h"

#include <algorithm>
#include <random>
#include <numeric>

template<std::size_t bits>
std::string bitset_to_hex(std::bitset<bits> const &num)
{
    static_assert(bits > 0, "bits cannot be 0");
    static_assert(bits % 4 == 0, "bits must be a multiple of 4");

    // Number of hex characters
    constexpr std::size_t num_hex_chars = bits / 4;

    // Pre-allocated string of correct size
    std::string result(num_hex_chars, ' ');

    // lookup table
    static constexpr char hex_map[] = "0123456789abcdef";

    for(std::size_t i = 0; i < num_hex_chars; i++)
    {
        std::uint8_t nibble_value = 0;

        for(std::size_t k = 0; k < 4; k++)
        {
            std::size_t bit_in_num_index = (bits - 1) - (i * 4) - k;

            if(num[bit_in_num_index])
            {
                nibble_value |= (1 << (3 - k));
            }
        }

        result[i] = hex_map[nibble_value];
    }

    return result;
}

template std::string bitset_to_hex(std::bitset<40> const &num);
template std::string bitset_to_hex(std::bitset<56> const &num);

std::uint8_t char_to_uint8_t(char const &c);

template<std::size_t bits>
std::bitset<bits> ksv_string_to_bitset(std::string const &s)
{
    static_assert(bits > 0, "bits cannot be 0");
    static_assert(bits % 4 == 0, "bits must be a multiple of 4");
    static_assert(bits <= 64, "bits must be less or equal 64");

    std::string string_to_convert = "0000000000000000";

    if(s.size() < 15)
    {
        //throw exception or do something
        string_to_convert = "";
        for(std::size_t i = 0; i < 15 - s.size(); i++)
            string_to_convert += "0";
        string_to_convert += s;
        std::reverse(string_to_convert.begin(), string_to_convert.end());
    }
    else
    {
        string_to_convert = s;
        std::reverse(string_to_convert.begin(), string_to_convert.end());
    }

    union ksv_layout
    {
        struct ksv_layout_bits
        {
            std::uint8_t u0 : 4;
            std::uint8_t u1 : 4;
            std::uint8_t u2 : 4;
            std::uint8_t u3 : 4;
            std::uint8_t u4 : 4;
            std::uint8_t u5 : 4;
            std::uint8_t u6 : 4;
            std::uint8_t u7 : 4;
            std::uint8_t u8 : 4;
            std::uint8_t u9 : 4;
            std::uint8_t u10 : 4;
            std::uint8_t u11 : 4;
            std::uint8_t u12 : 4;
            std::uint8_t u13 : 4;
            std::uint8_t u14 : 4;
            std::uint8_t u15 : 4;
        };

        ksv_layout_bits klb;
        std::uint64_t ksv_number;
    };

    std::bitset<bits> result = 0;
    ksv_layout kl;
    kl.ksv_number = 0;

    kl.klb.u0  = char_to_uint8_t(string_to_convert[0]);
    kl.klb.u1  = char_to_uint8_t(string_to_convert[1]);
    kl.klb.u2  = char_to_uint8_t(string_to_convert[2]);
    kl.klb.u3  = char_to_uint8_t(string_to_convert[3]);
    kl.klb.u4  = char_to_uint8_t(string_to_convert[4]);
    kl.klb.u5  = char_to_uint8_t(string_to_convert[5]);
    kl.klb.u6  = char_to_uint8_t(string_to_convert[6]);
    kl.klb.u7  = char_to_uint8_t(string_to_convert[7]);
    kl.klb.u8  = char_to_uint8_t(string_to_convert[8]);
    kl.klb.u9  = char_to_uint8_t(string_to_convert[9]);
    kl.klb.u10 = char_to_uint8_t(string_to_convert[10]);
    kl.klb.u11 = char_to_uint8_t(string_to_convert[11]);
    kl.klb.u12 = char_to_uint8_t(string_to_convert[12]);
    kl.klb.u13 = char_to_uint8_t(string_to_convert[13]);
    kl.klb.u14 = char_to_uint8_t(string_to_convert[14]);
    kl.klb.u15 = char_to_uint8_t(string_to_convert[15]);

    result = kl.ksv_number;

    return result;
}

template std::bitset<40> ksv_string_to_bitset(std::string const &s);
template std::bitset<56> ksv_string_to_bitset(std::string const &s);
template std::bitset<64> ksv_string_to_bitset(std::string const &s);

std::array<std::bitset<56>, 40> generate_source(std::bitset<40> const &ksv, std::array<std::bitset<56>, 1600> const &key)
{
    std::array<std::bitset<56>, 40> result;

    for(std::size_t i = 0; i < 40; i++)
    {
        std::uint64_t temp = 0x0;
        for(std::size_t z = 0; z < 40; z++)
        {
            if(ksv[z])
            {
                temp += key[z * 40 + i].to_ullong();
            }
        }
        temp      = temp & 0xffffffffffffff;
        result[i] = temp;
    }

    return result;
}

std::array<std::bitset<56>, 40> generate_sink(std::bitset<40> const &ksv, std::array<std::bitset<56>, 1600> const &key)
{
    std::array<std::bitset<56>, 40> result;

    for(std::size_t i = 0; i < 40; i++)
    {
        std::uint64_t temp = 0x0;
        for(std::size_t z = 0; z < 40; z++)
        {
            if(ksv[z])
            {
                temp += key[i * 40 + z].to_ullong();
            }
        }
        temp      = temp & 0xffffffffffffff;
        result[i] = temp;
    }

    return result;
}

std::bitset<40> random_ksv()
{
    std::bitset<40> bs(0x00000fffff);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> indices(bs.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);

    std::bitset<40> shuffled_bs;
    for(size_t i = 0; i < bs.size(); ++i)
    {
        shuffled_bs[i] = bs[indices[i]];
    }

    return shuffled_bs;
}

std::uint8_t char_to_uint8_t(char const &c)
{
    switch(c)
    {
        case '0':
            return 0b0000;
            break;
        case '1':
            return 0b0001;
            break;
        case '2':
            return 0b0010;
            break;
        case '3':
            return 0b0011;
            break;
        case '4':
            return 0b0100;
            break;
        case '5':
            return 0b0101;
            break;
        case '6':
            return 0b0110;
            break;
        case '7':
            return 0b0111;
            break;
        case '8':
            return 0b1000;
            break;
        case '9':
            return 0b1001;
            break;
        case 'a':
            return 0b1010;
            break;
        case 'b':
            return 0b1011;
            break;
        case 'c':
            return 0b1100;
            break;
        case 'd':
            return 0b1101;
            break;
        case 'e':
            return 0b1110;
            break;
        case 'f':
            return 0b1111;
            break;
        default:
            break;
    }
    return 0b0000;
}

/**
 * @brief Converts a `std::bitset` to `std::string` that is a table with 5 columns separated by a new line. Each value is separated by a space.
 *
 * @tparam N The number of elements in the input `std::array`.
 * @param[in] arr The input array.
 * @return A string that is a table with 5 columns.
*/
template<std::size_t N>
std::string get_key_array(std::array<std::bitset<56>, N> const &arr)
{
    std::string result = "";

    for(std::size_t i = 0; i < N; i++)
    {
        result += bitset_to_hex<56>(arr[i]);

        if((i + 1) % 5 == 0)
            result += "\n";
        else
            result += " ";
    }

    return result;
}

bool check_ksv(std::bitset<40> const &ksv)
{
    return ksv.count() != 20;
}

std::string hdcp::formatted(formatted_out_type const &t)
{
    std::string result = "";

    switch(t)
    {
        case TEXT_INFORMATIONAL:
        {
            result += "ksv: " + bitset_to_hex<40>(ksv) + "\n\n";

            result += "Source:\n";
            result += get_key_array<40>(source) + "\n";

            result += "Sink:\n";
            result += get_key_array<40>(sink);
            break;
        }
        case TEXT_SOURCE_ONLY:
        {
            result += "Source:\n";
            result += get_key_array<40>(source);
            break;
        }
        case TEXT_SINK_ONLY:
        {
            result += "Sink:\n";
            result += get_key_array<40>(sink);
            break;
        }
        case TEXT_SOURCE_KSV_ONLY:
        {
            result += "ksv: " + bitset_to_hex<40>(ksv) + "\n\n";

            result += "Source:\n";
            result += get_key_array<40>(source);
            break;
        }
        case TEXT_SINK_KSV_ONLY:
        {
            result += "ksv: " + bitset_to_hex<40>(ksv) + "\n\n";

            result += "Sink:\n";
            result += get_key_array<40>(sink);
            break;
        }
        case TEXT_LINE_SOURCE:
        {
            for(auto const &x : source)
            {
                result += bitset_to_hex<56>(x) + " ";
            }
            break;
        }
        case TEXT_LINE_SINK:
        {
            for(auto const &x : sink)
            {
                result += bitset_to_hex<56>(x) + " ";
            }
            break;
        }
        case TEXT_FULL:
        {
            result += "ksv: " + bitset_to_hex<40>(ksv) + "\n\n";

            result += "Source:\n";
            result += get_key_array<40>(source) + "\n";

            result += "Sink:\n";
            result += get_key_array<40>(sink) + "\n";

            result += "HDCP key:\n";
            result += get_key_array<1600>(hdcp_key);
            break;
        }
        case JSON:
        {
            result += "{\n";
            result += "    \"ksv\":\"" + bitset_to_hex<40>(ksv) + "\",\n";

            result += "    \"source\":\n";
            result += "    [\n";
            for(std::size_t i = 0; i < source.size(); i++)
            {
                result += "        \"" + bitset_to_hex<56>(source[i]) + "\"";

                if(i != (source.size() - 1))
                    result += ",";

                result += "\n";
            }
            result += "    ],\n";

            result += "    \"sink\":\n";
            result += "    [\n";
            for(std::size_t i = 0; i < sink.size(); i++)
            {
                result += "        \"" + bitset_to_hex<56>(sink[i]) + "\"";

                if(i != (sink.size() - 1))
                    result += ",";

                result += "\n";
            }
            result += "    ]\n";

            result += "}\n";
            break;
        }
        case JSON_FULL:
        {
            result += "{\n";
            result += "    \"ksv\":\"" + bitset_to_hex<40>(ksv) + "\",\n";

            result += "    \"source\":\n";
            result += "    [\n";
            for(std::size_t i = 0; i < source.size(); i++)
            {
                result += "        \"" + bitset_to_hex<56>(source[i]) + "\"";

                if(i != (source.size() - 1))
                    result += ",";

                result += "\n";
            }
            result += "    ],\n";

            result += "    \"sink\":\n";
            result += "    [\n";
            for(std::size_t i = 0; i < sink.size(); i++)
            {
                result += "        \"" + bitset_to_hex<56>(sink[i]) + "\"";

                if(i != (sink.size() - 1))
                    result += ",";

                result += "\n";
            }
            result += "    ],\n";

            result += "    \"hdcp_key\":\n";
            result += "    [\n";
            for(std::size_t i = 0; i < hdcp_key.size(); i++)
            {
                result += "        \"" + bitset_to_hex<56>(hdcp_key[i]) + "\"";

                if(i != (hdcp_key.size() - 1))
                    result += ",";

                result += "\n";
            }
            result += "    ]\n";

            result += "}\n";
            break;
        }
        case YAML:
        {
            result += "ksv: " + bitset_to_hex<40>(ksv) + "\n";
            result += "source:\n";
            for(auto const &x : source)
                result += "  - " + bitset_to_hex<56>(x) + "\n";

            result += "sink:\n";
            for(auto const &x : sink)
                result += "  - " + bitset_to_hex<56>(x) + "\n";

            break;
        }
        case YAML_FULL:
        {
            result += "ksv: " + bitset_to_hex<40>(ksv) + "\n";
            result += "source:\n";
            for(auto const &x : source)
                result += "  - " + bitset_to_hex<56>(x) + "\n";

            result += "sink:\n";
            for(auto const &x : sink)
                result += "  - " + bitset_to_hex<56>(x) + "\n";

            result += "hdcp_key:\n";
            for(auto const &x : hdcp_key)
                result += "  - " + bitset_to_hex<56>(x) + "\n";

            break;
        }
        case XML:
        {
            result += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

            result += "<hdcp>\n";
            result += "    <ksv>" + bitset_to_hex<40>(ksv) + "</ksv>" + "\n";

            result += "    <source>\n";
            for(auto const &x : source)
                result += "        <item>" + bitset_to_hex<56>(x) + "</item>" + "\n";
            result += "    </source>\n";

            result += "    <sink>\n";
            for(auto const &x : sink)
                result += "        <item>" + bitset_to_hex<56>(x) + "</item>" + "\n";
            result += "    </sink>\n";

            result += "</hdcp>\n";
            break;
        }
        case XML_FULL:
        {
            result += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

            result += "<hdcp>\n";
            result += "    <ksv>" + bitset_to_hex<40>(ksv) + "</ksv>" + "\n";

            result += "    <source>\n";
            for(auto const &x : source)
                result += "        <item>" + bitset_to_hex<56>(x) + "</item>" + "\n";
            result += "    </source>\n";

            result += "    <sink>\n";
            for(auto const &x : sink)
                result += "        <item>" + bitset_to_hex<56>(x) + "</item>" + "\n";
            result += "    </sink>\n";

            result += "    <hdcp_key>\n";
            for(auto const &x : hdcp_key)
                result += "        <item>" + bitset_to_hex<56>(x) + "</item>" + "\n";
            result += "    </hdcp_key>\n";

            result += "</hdcp>\n";
            break;
        }
        case TOML:
        {
            result += "ksv = \"" + bitset_to_hex<40>(ksv) + "\"\n";

            result += "source = [\n";
            for(auto const &x : source)
                result += "  \"" + bitset_to_hex<56>(x) + "\",\n";
            result += "]\n";

            result += "sink = [\n";
            for(auto const &x : sink)
                result += "  \"" + bitset_to_hex<56>(x) + "\",\n";
            result += "]\n";

            break;
        }
        case TOML_FULL:
        {
            result += "ksv = \"" + bitset_to_hex<40>(ksv) + "\"\n";

            result += "source = [\n";
            for(auto const &x : source)
                result += "  \"" + bitset_to_hex<56>(x) + "\",\n";
            result += "]\n";

            result += "sink = [\n";
            for(auto const &x : sink)
                result += "  \"" + bitset_to_hex<56>(x) + "\",\n";
            result += "]\n";

            result += "hdcp_key = [\n";
            for(auto const &x : hdcp_key)
                result += "  \"" + bitset_to_hex<56>(x) + "\",\n";
            result += "]\n";

            break;
        }
        default:
            break;
    }

    return result;
}

formatted_out_type string_to_fot(std::string const &s)
{
    if(s == "text_informational")
        return TEXT_INFORMATIONAL;

    if(s == "text_source_only")
        return TEXT_SOURCE_ONLY;

    if(s == "text_sink_only")
        return TEXT_SINK_ONLY;

    if(s == "text_source_ksv_only")
        return TEXT_SOURCE_KSV_ONLY;

    if(s == "text_sink_ksv_only")
        return TEXT_SINK_KSV_ONLY;

    if(s == "text_line_source")
        return TEXT_LINE_SOURCE;

    if(s == "text_line_sink")
        return TEXT_LINE_SINK;

    if(s == "text_full")
        return TEXT_FULL;

    if(s == "json")
        return JSON;

    if(s == "json_full")
        return JSON_FULL;

    if(s == "yaml")
        return YAML;

    if(s == "yaml_full")
        return YAML_FULL;

    if(s == "xml")
        return XML;

    if(s == "xml_full")
        return XML_FULL;

    if(s == "toml")
        return TOML;

    if(s == "toml_full")
        return TOML_FULL;

    return NOT_FOUND;
}
