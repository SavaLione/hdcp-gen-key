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
 * @file hdcp-gen-key.h
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
#ifndef HDCP_GEN_KEY_H
#define HDCP_GEN_KEY_H

/**
 * @brief Prints help information that is invoked by `-h` or `--help`
*/
void print_help();

#endif // HDCP_GEN_KEY_H