/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2020, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "config_dvfs.h"
#include "config_power_domain.h"
#include "config_sensor.h"
#include "juno_clock.h"
#include "juno_scmi.h"

#include <mod_resource_perms.h>
#include <mod_scmi_std.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

/*!
 * If the agent wants to modify permissions at run-time these tables
 * must be allocated in wrtiable memory.
 */

/*!
 * Agent 0 gets access to all resources.
 */
#define AGENT_IDX(agent_id) (agent_id - 1)

/*!
 * Note that permissions are denied when a bit is set, the
 * permissions tables are being added for Juno as an example only.
 */
static struct mod_res_agent_protocol_permissions
    agent_protocol_permissions[] = {
    [AGENT_IDX(JUNO_SCMI_AGENT_IDX_OSPM)] = {
        .protocols = MOD_RES_PERMS_SCMI_ALL_PROTOCOLS_ALLOWED,
    },
    [AGENT_IDX(JUNO_SCMI_AGENT_IDX_PSCI)] = {
        .protocols = MOD_RES_PERMS_SCMI_ALL_PROTOCOLS_ALLOWED,
    },
};

/*
 * Messages have an index offset from 0x3 as all agents can access
 * the VERSION/ATTRIBUTES/MSG_ATTRIBUTES messages for all
 * protocols, hence message 0x3 maps to bit[0], message 0x4 maps
 * to bit[1], etc.
 */
static struct mod_res_agent_msg_permissions agent_msg_permissions[] = {
    [AGENT_IDX(JUNO_SCMI_AGENT_IDX_OSPM)] = {
        /* Example, Base, disable unused msg 12 */
        .messages[MOD_RES_PERMS_SCMI_BASE_MESSAGE_IDX] =
            (1 << 12),  /* Example, Base, disable unused msg 12 */
        /* Power Domain */
        .messages[MOD_RES_PERMS_SCMI_POWER_DOMAIN_MESSAGE_IDX] = 0x0,
        /* System Power Domain */
        .messages[MOD_RES_PERMS_SCMI_SYS_POWER_MESSAGE_IDX] = 0x0,
        /* Performance */
        .messages[MOD_RES_PERMS_SCMI_PERF_MESSAGE_IDX] = 0x0,
        /* Clocks */
        .messages[MOD_RES_PERMS_SCMI_CLOCK_MESSAGE_IDX] = 0x0,
        /* Sensors */
        .messages[MOD_RES_PERMS_SCMI_SENSOR_MESSAGE_IDX] = 0x0,
        /* Reset Domains */
        .messages[MOD_RES_PERMS_SCMI_RESET_DOMAIN_MESSAGE_IDX] = 0x0,
    },
    [AGENT_IDX(JUNO_SCMI_AGENT_IDX_PSCI)] = {
        .messages[MOD_RES_PERMS_SCMI_BASE_MESSAGE_IDX] = 0x0,
        .messages[MOD_RES_PERMS_SCMI_POWER_DOMAIN_MESSAGE_IDX] = 0x0,
        .messages[MOD_RES_PERMS_SCMI_SYS_POWER_MESSAGE_IDX] = 0x0,
        .messages[MOD_RES_PERMS_SCMI_PERF_MESSAGE_IDX] = 0x0,
        .messages[MOD_RES_PERMS_SCMI_CLOCK_MESSAGE_IDX] = 0x0,
        .messages[MOD_RES_PERMS_SCMI_SENSOR_MESSAGE_IDX] = 0x0,
        .messages[MOD_RES_PERMS_SCMI_RESET_DOMAIN_MESSAGE_IDX] = 0x0,
    },
};

/*
 * Protocols have an index offset from SCMI_BASE protocol, 0x10
 * Note that the BASE and SYSTEM_POWER protocols are managed
 * on a protocol:command basis, there is no resource permissions
 * associated with the protocols.
 */

static struct mod_res_agent_permission agent_permissions = {
    .agent_protocol_permissions = agent_protocol_permissions,
    .agent_msg_permissions = agent_msg_permissions,
};

struct fwk_module_config config_resource_perms = {
    .data =
        &(struct mod_res_resource_perms_config){
            .agent_permissions = (uintptr_t)&agent_permissions,
            .agent_count = JUNO_SCMI_AGENT_IDX_COUNT,
            .protocol_count = 7,
            .clock_count = JUNO_CLOCK_IDX_COUNT,
            .sensor_count = MOD_JUNO_R0_SENSOR_IDX_COUNT,
            .pd_count = POWER_DOMAIN_IDX_COUNT,
            .perf_count = DVFS_ELEMENT_IDX_COUNT,
        },
};
