// SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLUGINSITEMINTERFACE_V2_H
#define PLUGINSITEMINTERFACE_V2_H

// Compatibility alias: external plugins built against older dock versions
// may reference pluginsiteminterface_v2.h. This header forwards to the
// current interface definition.

#include "pluginsiteminterface.h"

// Alias for external plugins expecting the V2 naming convention
using PluginsItemInterfaceV2 = PluginsItemInterface;

// Alias for the V2 IID macro name
#define ModuleInterface_iid_V2 ModuleInterface_iid

#endif // PLUGINSITEMINTERFACE_V2_H
