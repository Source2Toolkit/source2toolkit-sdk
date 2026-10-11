/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
 * All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 * gives you permission to link the code of this program
 * (as well as its derivative works) to "Counter-Strike 2," "Source 2,"
 * "Steam," and any Game MODs or server software running on software by
 * Valve Corporation. You must obey the GNU General Public License in all
 * respects for all other code used.
 *
 * Additionally, this exception applies to all derivative works unless
 * otherwise stated in LICENSE.txt.
 *
 * Authors:
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#ifndef _INCLUDE_MODELMESHBUFFERUSAGE_T_H
#define _INCLUDE_MODELMESHBUFFERUSAGE_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class ModelMeshBufferUsage_t : uint16_t
{
    MESH_BUFFER_USAGE_NONE = 0,
    MESH_BUFFER_USAGE_VB = 1,
    MESH_BUFFER_USAGE_IB = 2,
    MESH_BUFFER_USAGE_ADJACENCY = 4,
    MESH_BUFFER_USAGE_MESHLET_TRIS = 8,
    MESH_BUFFER_USAGE_RT_PROXY = 16,
    MESH_BUFFER_USAGE_VERTEX_ALBEDO = 32,
    MESH_BUFFER_USAGE_VERTEX_EMISSIVE = 64,
    MESH_BUFFER_USAGE_MESHLETS = 128,
    MESH_BUFFER_USAGE_ALIAS_TABLE = 256,
};

#endif // _INCLUDE_MODELMESHBUFFERUSAGE_T_H
