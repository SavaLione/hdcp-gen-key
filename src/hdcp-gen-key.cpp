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
 * @file hdcp-gen-key.cpp
 * @brief The application implementation.
 * @details
 *
 * hdcp-gen-key is a tool for generating HDCP keys (HDCP versions 1.0-1.4).
 *
 * It uses Intel's Master Key Matrix.
 *
 * It supports different output formats and randomly generated valid KSV.
 *
 * @author Savelii Pototskii
 * @date 2025-05-22
 * @copyright Copyright (C) 2025 Savelii Pototskii (savalione.com)
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
*/
#include "hdcp-gen-key.h"

#include <iostream>

#include "hdcp.h"
#include "intel-hdcp-key.h"
#include "xgetopt/xgetopt.h"
#include "config.h"

int main(int argc, char **argv)
{
    std::bitset<40> ksv    = random_ksv();
    formatted_out_type out = formatted_out_type::TEXT_INFORMATIONAL;

    std::string const short_opts = "k:o:hv";

    // clang-format off
    std::array<xoption, 8> long_options =
        {{
            {"ksv",     xrequired_argument, nullptr, 'k'},
            {"out",     xrequired_argument, nullptr, 'o'},
            {"help",    xno_argument,       nullptr, 'h'},
            {"version", xno_argument,       nullptr, 'v'}
        }};
    // clang-format on

    while(true)
    {
        auto const opt = xgetopt_long(argc, argv, short_opts.c_str(), long_options.data(), nullptr);

        if(opt == -1)
        {
            break;
        }

        switch(opt)
        {
            case 'k':
                ksv = ksv_string_to_bitset<40>(xoptarg);
                break;

            case 'o':
            {
                out = string_to_fot(xoptarg);

                if(out == formatted_out_type::NOT_FOUND)
                {
                    std::cout << "Output format option: '" << xoptarg << "' is not recognized." << std::endl;
                    std::cout << "Try: 'hdcp-gen-key --help' for more information." << std::endl;
                    exit(1);
                }
                break;
            }
            case 'h':
                print_help();
                exit(0);
                break;
            case 'v':
                std::cout << HGK_VERSION << std::endl;
                exit(0);
                break;
            default:
                exit(1);
        }
    }

    hdcp h(intel_hdcp_key, ksv);
    std::cout << h.formatted(out);

    return 0;
}

void print_help()
{
    std::string help =
        R"(hdcp-gen-key is a tool for generating HDCP keys (HDCP versions 1.0-1.4).
It uses Intel's Master Key Matrix.

usage: hdcp-gen-key [options...]

Options:
  -k, --ksv <hex>           Key Selection Vector (KSV) in 10-character hexadecimal format.
                            The KSV must be a 40-bit binary number consisting of
                            twenty '1's and twenty '0's.
                            [default: randomly generated valid KSV]
  -o, --out <format_option> Specifies the output format and content.
                            See 'Output Formats' below.
                            [default: text_informational]
  --version                 Print the application version and exit.
  -h, --help                Show this help message and exit.

Output Formats:
  text_informational    : Human-readable KSV, generated source device key, and generated sink device key.
  text_source_only      : Human-readable generated source device key only.
  text_sink_only        : Human-readable generated sink device key only.
  text_source_ksv_only  : Human-readable KSV and generated source device key.
  text_sink_ksv_only    : Human-readable KSV and generated sink device key.
  text_line_source      : Generated source device key as a line of space-separated hexadecimal values.
  text_line_sink        : Generated sink device key as a line of space-separated hexadecimal values.
  text_full             : Human-readable KSV, generated source device key, generated sink device key,
                          and the derived HDCP shared key.
  json                  : KSV, generated source device key, and generated sink device key as JSON.
  json_full             : KSV, generated source device key, generated sink device key,
                          and the derived HDCP shared key as JSON.
  yaml                  : KSV, generated source device key, and generated sink device key as YAML.
  yaml_full             : KSV, generated source device key, generated sink device key,
                          and the derived HDCP shared key as YAML.
  xml                   : KSV, generated source device key, and generated sink device key as XML.
  xml_full              : KSV, generated source device key, generated sink device key,
                          and the derived HDCP shared key as XML.
  toml                  : KSV, generated source device key, and generated sink device key as TOML.
  toml_full             : KSV, generated source device key, generated sink device key,
                          and the derived HDCP shared key as TOML.

Examples:
  hdcp-gen-key -k 00000fffff -o json_full
  hdcp-gen-key --out text_line_source
)";
    std::cout << help << std::endl;
}