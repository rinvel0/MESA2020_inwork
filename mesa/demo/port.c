// Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
// SPDX-License-Identifier: MIT



#include <stdio.h>
#include <dlfcn.h> //for dlopen, dlsym
#include "mscc/ethernet/switch/api.h"
#include "mscc/ethernet/board/api.h"
#include "main.h"
#include "trace.h"
#include "cli.h"
#include "port.h"
// #include "sfp.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static mscc_appl_trace_module_t trace_module = {
    .name = "port"
};

enum {
    TRACE_GROUP_DEFAULT,
    TRACE_GROUP_CNT
};

static mscc_appl_trace_group_t trace_groups[TRACE_GROUP_CNT] = {
    // TRACE_GROUP_DEFAULT
    {
        .name = "default",
        .level = MESA_TRACE_LEVEL_ERROR
    },
};
meba_inst_t meba_global_inst;
#define LOOP_PORT_INVALID  0xFFFFFFFF
static uint32_t loop_port = LOOP_PORT_INVALID;
static meba_phy_driver_t *phy_drivers = NULL;
/* ================================================================= *
 *  Port control
 * ================================================================= */

static port_entry_t *port_table;
static mesa_bool_t  port_polling = 1;
static uint32_t     port_poll_cnt;

const char *mesa_port_if2txt(mesa_port_interface_t if_type)
{
    switch (if_type) {
    case MESA_PORT_INTERFACE_NO_CONNECTION: return "N/C";
    case MESA_PORT_INTERFACE_LOOPBACK:      return "LOOPBACK";
    case MESA_PORT_INTERFACE_INTERNAL:      return "INTERNAL";
    case MESA_PORT_INTERFACE_MII:           return "MII";
    case MESA_PORT_INTERFACE_GMII:          return "GMII";
    case MESA_PORT_INTERFACE_RGMII:         return "RGMII";
    case MESA_PORT_INTERFACE_TBI:           return "TBI";
    case MESA_PORT_INTERFACE_RTBI:          return "RTBI";
    case MESA_PORT_INTERFACE_SGMII:         return "SGMII";
    case MESA_PORT_INTERFACE_SGMII_2G5:     return "SGMII_2G5";
    case MESA_PORT_INTERFACE_SERDES:        return "SERDES";
    case MESA_PORT_INTERFACE_VAUI:          return "VAUI";
    case MESA_PORT_INTERFACE_100FX:         return "100FX";
    case MESA_PORT_INTERFACE_XAUI:          return "XAUI";
    case MESA_PORT_INTERFACE_RXAUI:         return "RXAUI";
    case MESA_PORT_INTERFACE_XGMII:         return "XGMII";
    case MESA_PORT_INTERFACE_SPI4:          return "SPI4";
    case MESA_PORT_INTERFACE_SGMII_CISCO:   return "SGMII_CISCO";
    case MESA_PORT_INTERFACE_QSGMII:        return "QSGMII";
    case MESA_PORT_INTERFACE_SFI:           return "SFI";
    case MESA_PORT_INTERFACE_SXGMII:        return "SXGMII";
    case MESA_PORT_INTERFACE_USGMII:        return "USGMII";
    case MESA_PORT_INTERFACE_QXGMII:        return "QXGMII";
    case MESA_PORT_INTERFACE_DXGMII_5G:     return "DXGMII_5G";
    case MESA_PORT_INTERFACE_DXGMII_10G:    return "DXGMII_10G";
    }
    return "?   ";
}

char *mesa_port_spd2txt(mesa_port_speed_t speed)
{
    switch (speed) {
    case MESA_SPEED_UNDEFINED: return "Undefined";
    case MESA_SPEED_10M:       return "10M";
    case MESA_SPEED_100M:      return "100M";
    case MESA_SPEED_1G:        return "1G";
    case MESA_SPEED_2500M:     return "2G5";
    case MESA_SPEED_5G:        return "5G";
    case MESA_SPEED_10G:       return "10G";
    case MESA_SPEED_12G:       return "12G";
    case MESA_SPEED_25G:       return "25G";
    case MESA_SPEED_AUTO:      return "Auto";
    }
    return "?   ";
}

const char *mesa_sfp_if2txt(meba_sfp_transreceiver_t sfp)
{
    switch (sfp) {
    case MEBA_SFP_TRANSRECEIVER_NONE:          return "None";
    case MEBA_SFP_TRANSRECEIVER_NOT_SUPPORTED: return "Not supported";
    case MEBA_SFP_TRANSRECEIVER_100FX:         return "100FX";
    case MEBA_SFP_TRANSRECEIVER_100BASE_LX:    return "100BASE_LX";
    case MEBA_SFP_TRANSRECEIVER_100BASE_ZX:    return "100BASE_ZX";
    case MEBA_SFP_TRANSRECEIVER_100BASE_SX:    return "100BASE_SX";
    case MEBA_SFP_TRANSRECEIVER_100BASE_BX10:  return "100BASE_BX10";
    case MEBA_SFP_TRANSRECEIVER_100BASE_T:     return "100BASE_T";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_BX10: return "1000BASE_BX10";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_T:    return "1000BASE_T";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_CX:   return "1000BASE_CX";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_SX:   return "1000BASE_SX";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_LX:   return "1000BASE_LX";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_ZX:   return "1000BASE_ZX";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_LR:   return "1000BASE_LR";
    case MEBA_SFP_TRANSRECEIVER_1000BASE_X:    return "1000BASE_X";
    case MEBA_SFP_TRANSRECEIVER_2G5:           return "2G5";
    case MEBA_SFP_TRANSRECEIVER_5G:            return "5G";
    case MEBA_SFP_TRANSRECEIVER_10G:           return "10G";
    case MEBA_SFP_TRANSRECEIVER_10G_SR:        return "10G_SR";
    case MEBA_SFP_TRANSRECEIVER_10G_LR:        return "10G_LR";
    case MEBA_SFP_TRANSRECEIVER_10G_LRM:       return "10G_LRM";
    case MEBA_SFP_TRANSRECEIVER_10G_ER:        return "10G_ER";
    case MEBA_SFP_TRANSRECEIVER_10G_DAC:       return "10G_DAC";
    case MEBA_SFP_TRANSRECEIVER_25G:           return "25G";
    case MEBA_SFP_TRANSRECEIVER_25G_SR:        return "25G_SR";
    case MEBA_SFP_TRANSRECEIVER_25G_LR:        return "25G_LR";
    case MEBA_SFP_TRANSRECEIVER_25G_LRM:       return "25G_LRM";
    case MEBA_SFP_TRANSRECEIVER_25G_ER:        return "25G_ER";
    case MEBA_SFP_TRANSRECEIVER_25G_DAC:       return "25G_DAC";
    }
    return "?   ";
}

static mesa_rc port_speed_adjust(mesa_port_no_t port_no,
                                 mesa_port_interface_t if_type,
                                 mesa_port_speed_t speed_in,
                                 mesa_port_speed_t *speed_out)
{
    port_entry_t    *entry = &port_table[port_no];
    meba_port_cap_t  cap = entry->meba.cap;


    /* Adjust the speed to the interface type */
    switch (if_type) {
    case MESA_PORT_INTERFACE_100FX:
        *speed_out = MESA_SPEED_100M;
        return MESA_RC_OK;
        break;
    case MESA_PORT_INTERFACE_SERDES:
    case MESA_PORT_INTERFACE_VAUI:
        if (cap & MEBA_PORT_CAP_2_5G_FDX) {
            if (speed_in == MESA_SPEED_1G || speed_in == MESA_SPEED_2500M) {
                *speed_out = speed_in;
                return MESA_RC_OK;
            }
        }
        *speed_out = MESA_SPEED_1G;
        return MESA_RC_OK;
        break;
    case MESA_PORT_INTERFACE_SGMII_CISCO:
        *speed_out = MESA_SPEED_1G; // initial speed is 1G
        return MESA_RC_OK;
    case MESA_PORT_INTERFACE_SFI:
    case MESA_PORT_INTERFACE_XAUI:
        if (entry->sfp_type == MEBA_SFP_TRANSRECEIVER_10G_DAC ||
            entry->sfp_type == MEBA_SFP_TRANSRECEIVER_10G_SR) {
            if ((speed_in == MESA_SPEED_25G) && (cap & MEBA_PORT_CAP_10G_FDX)) {
                *speed_out = MESA_SPEED_10G;
                return MESA_RC_OK;
            }
        }
        if (((cap & MEBA_PORT_CAP_5G_FDX) && (speed_in == MESA_SPEED_5G)) ||
            ((cap & MEBA_PORT_CAP_10G_FDX) && (speed_in == MESA_SPEED_10G))) {
            *speed_out = speed_in;
            return MESA_RC_OK;
        }
        if (cap & MEBA_PORT_CAP_25G_FDX) {
            *speed_out = MESA_SPEED_25G;
            return MESA_RC_OK;
        } else if (cap & MEBA_PORT_CAP_10G_FDX) {
            *speed_out = MESA_SPEED_10G;
            return MESA_RC_OK;
        } else if (cap & MEBA_PORT_CAP_5G_FDX) {
            *speed_out = MESA_SPEED_5G;
            return MESA_RC_OK;
        }
        break;
    default:
        break;
    }

    return MESA_RC_ERROR;
}

static mesa_rc port_setup_sfp(mesa_port_no_t port_no, port_entry_t *entry, mesa_port_conf_t *conf) {

    mscc_appl_port_conf_t         *p_conf = &entry->conf;
    mesa_port_interface_t         mac_if = entry->meba.mac_if;
    meba_sfp_device_t             *device = entry->sfp_device;
    meba_port_cap_t               cap = entry->meba.cap;
    mesa_port_clause_37_control_t control;
    mesa_port_clause_37_adv_t     *adv;
    meba_port_admin_state_t       meba_admin;

    if (device != NULL) {
        device->drv->meba_sfp_driver_if_get(device, p_conf->speed, &mac_if);
    } else {
        // No I2C access
        if ((p_conf->speed == MESA_SPEED_1G && cap & MEBA_PORT_CAP_1G_FDX) ||
            (p_conf->speed == MESA_SPEED_2500M && cap & MEBA_PORT_CAP_2_5G_FDX)) {
            if (entry->meba.mac_if == MESA_PORT_INTERFACE_SGMII_CISCO) {
                mac_if = MESA_PORT_INTERFACE_SGMII_CISCO;
            } else {
                mac_if = MESA_PORT_INTERFACE_SERDES;
            }
        } else if ((p_conf->speed == MESA_SPEED_5G && (cap & MEBA_PORT_CAP_5G_FDX)) ||
                   (p_conf->speed == MESA_SPEED_10G && (cap & MEBA_PORT_CAP_10G_FDX))) {
            mac_if = MESA_PORT_INTERFACE_SFI;
        } else if ((p_conf->speed == MESA_SPEED_100M && (cap & MEBA_PORT_CAP_100M_FDX))) {
            mac_if = MESA_PORT_INTERFACE_100FX;
        }
    }
    conf->if_type = mac_if;


    if (p_conf->admin.enable && (cap & MEBA_PORT_CAP_AUTONEG)
        && (conf->if_type == MESA_PORT_INTERFACE_SERDES) && (p_conf->speed == MESA_SPEED_1G)) {
        /* IEEE 802.3 clause 37 auto-negotiation */
        T_N("Port:%d, Clause 37 setup", port_no);
        /* PCS auto negotiation */
        memset(&control, 0, sizeof(control));
        control.enable = p_conf->autoneg;
        adv = &control.advertisement;
        adv->fdx = TRUE;
        adv->hdx = FALSE;
        adv->symmetric_pause = p_conf->flow_control;
        adv->asymmetric_pause = p_conf->flow_control;
        adv->remote_fault = (p_conf->admin.enable ? MESA_PORT_CLAUSE_37_RF_LINK_OK :
                             MESA_PORT_CLAUSE_37_RF_OFFLINE);
        adv->acknowledge = FALSE;
        adv->next_page = FALSE;

        T_D("Port: %d set port via clause_37, ena:%d %s%s%s", port_no, control.enable,
            adv->fdx ?"FDX " : "HDX ", p_conf->flow_control ? ", Flow control": "",
            adv->remote_fault ? ", remote_fault":"" );
        if (mesa_port_clause_37_control_set(NULL, port_no, &control) != MESA_RC_OK) {
            T_E("mesa_port_clause_37_control_set(%u) failed", port_no);
            return MESA_RC_ERROR;
        }
    } else if ((cap & MEBA_PORT_CAP_AUTONEG) &&
               (p_conf->speed == MESA_SPEED_2500M)) {
        // Disable clause 37 aneg for 2G5 ports
        mesa_port_clause_37_control_t ctrl;
        if (mesa_port_clause_37_control_get(NULL, port_no, &ctrl) != MESA_RC_OK) {
            T_E("mesa_port_clause_37_control_get(%u) failed", port_no);
        }
        if (ctrl.enable) {
            ctrl.enable = FALSE;
            if (mesa_port_clause_37_control_set(NULL, port_no, &ctrl) != MESA_RC_OK) {
                T_E("mesa_port_clause_37_control_set(%u) failed", port_no);
            }
        }
    }

    if (port_speed_adjust(port_no, mac_if, p_conf->speed , &conf->speed) == MESA_RC_ERROR) {
        cli_printf("Port:%d, SFP type:%d, cannot support the requested speed:%d\n",
            port_no, entry->sfp_type, p_conf->speed);
        return MESA_RC_ERROR;
    }
    meba_admin.enable = p_conf->admin.enable;
    MEBA_WRAP(meba_port_admin_state_set, meba_global_inst, port_no, &meba_admin);

    return MESA_RC_OK;
}

static void port_setup(mesa_port_no_t port_no, mesa_bool_t aneg)
{
    port_entry_t            *entry = &port_table[port_no];
    mscc_appl_port_conf_t   *pc = &entry->conf;
    mesa_port_status_t      *ps = &entry->status;
    mesa_port_conf_t        conf;
    mesa_phy_conf_t         phy;
    meba_port_cap_t         cap = entry->meba.cap;

    if (mesa_port_conf_get(NULL, port_no, &conf) != MESA_RC_OK) {
        T_E("mesa_port_conf_get(%u) failed", port_no);
        return;
    }
    conf.power_down = (pc->admin.enable ? 0 : 1);
    conf.flow_control.smac.addr[5] = port_no;
    conf.max_frame_length = pc->max_length;
    conf.fdx = pc->fdx;
    conf.flow_control.obey = pc->flow_control;
    conf.flow_control.generate = pc->flow_control;
    conf.sd_enable = (cap & MEBA_PORT_CAP_SD_ENABLE ? 1 : 0);
    conf.sd_active_high = (cap & MEBA_PORT_CAP_SD_HIGH ? 1 : 0);
    conf.sd_internal = (cap & MEBA_PORT_CAP_SD_INTERNAL ? 1 : 0);
    conf.xaui_rx_lane_flip = (cap & MEBA_PORT_CAP_XAUI_LANE_FLIP ? 1 : 0);
    conf.xaui_tx_lane_flip = (cap & MEBA_PORT_CAP_XAUI_LANE_FLIP ? 1 : 0);
    conf.serdes.rx_invert = (cap & MEBA_PORT_CAP_SERDES_RX_INVERT ? 1 : 0);
    conf.serdes.tx_invert = (cap & MEBA_PORT_CAP_SERDES_TX_INVERT ? 1 : 0);
    if (entry->sfp_device != NULL && entry->sfp_device->drv->meba_sfp_driver_mt_get != NULL) {
        (void)entry->sfp_device->drv->meba_sfp_driver_mt_get(entry->sfp_device, &conf.serdes.media_type);
    } else {
        conf.serdes.media_type = MESA_SD10G_MEDIA_DAC;
    }
    if (aneg) {
        /* Setup port based on auto negotiation status */
        conf.speed = ps->speed;
        conf.fdx = ps->fdx;
        conf.flow_control.obey = ps->aneg.obey_pause;
        conf.flow_control.generate = ps->aneg.generate_pause;
    } else {
        /* Setup port based on configuration */
        if (entry->media_type == MSCC_PORT_TYPE_CU) {
            if (entry->phy_device) {
                // Use the Phy library (currently only AQR supported)
            } else {
                /* Setup PHY */
                memset(&phy, 0, sizeof(phy));
                phy.mode = (pc->admin.enable ?
                            (pc->autoneg || pc->speed == MESA_SPEED_1G ?
                             MESA_PHY_MODE_ANEG : MESA_PHY_MODE_FORCED) :
                            MESA_PHY_MODE_POWER_DOWN);
                phy.aneg.speed_10m_hdx = 1;
                phy.aneg.speed_10m_fdx = 1;
                phy.aneg.speed_100m_hdx = 1;
                phy.aneg.speed_100m_fdx = 1;
                phy.aneg.speed_1g_fdx = 1;
                phy.aneg.symmetric_pause = pc->flow_control;
                phy.aneg.asymmetric_pause = pc->flow_control;
                phy.forced.speed = pc->speed;
                phy.forced.fdx = pc->fdx;
                phy.mdi = MESA_PHY_MDIX_AUTO; // always enable auto detection of crossed/non-crossed cables
                if (mesa_phy_conf_set(NULL, port_no, &phy) != MESA_RC_OK) {
                    T_E("mesa_phy_conf_set(%u) failed", port_no);
                    return;
                }
            }
            conf.if_type = entry->meba.mac_if;
            conf.speed = (pc->autoneg ? MESA_SPEED_1G : pc->speed);
        } else if (entry->media_type == MSCC_PORT_TYPE_SFP) {
            /* Get interface and speed from SFP */
            if (port_setup_sfp(port_no, entry, &conf) != MESA_RC_OK) {
                T_E("Could not configure SFP port(%u)", port_no);
            }
        }
    }

    if (port_no == loop_port) { // This port is the active loop port
        conf.loop = 1;
    }

    T_I("Port: %d if_type = %s, speed:%s %s %s %s, loop %u",
        port_no, mesa_port_if2txt(conf.if_type), mesa_port_spd2txt(conf.speed),
        conf.fdx ? "FDX" : "HDX", conf.flow_control.obey ? "OBEY" : "",
        conf.flow_control.generate ? "GENERATE" : "", conf.loop);

    if (mesa_port_conf_set(NULL, port_no, &conf) != MESA_RC_OK) {
        T_E("mesa_port_conf_set(%u) failed", port_no);
    }
}

static mesa_rc port_status_poll(mesa_port_no_t port_no)
{
    port_entry_t       *entry;
    mesa_port_status_t *ps;
    meba_phy_driver_status_t phy_status;

    T_N("Enter, port %d", port_no);

    entry = &port_table[port_no];
    ps = &entry->status;

    if (entry->media_type == MSCC_PORT_TYPE_CU) {
        if (entry->phy_device != NULL) { // AQR Phys
            entry->phy_device->drv->meba_phy_driver_poll(entry->phy_device, &phy_status);
            ps->link = phy_status.link;
            ps->speed = phy_status.speed;
            ps->fdx = phy_status.fdx;
        } else {
            if (mesa_phy_status_get(NULL, port_no, ps) != MESA_RC_OK) {
                T_E("mesa_phy_status_get(%u) failed (disable polling)", port_no);
                entry->valid = FALSE; // Polling disabled
                return MESA_RC_ERROR;
            }
        }
    } else if (entry->media_type == MSCC_PORT_TYPE_SFP) {
        if (mesa_port_status_get(NULL, port_no, ps) != MESA_RC_OK) {
            T_E("mesa_port_status_get(%u) failed", port_no);
            return MESA_RC_ERROR;
        }
    }

    T_N("Exit, port %d", port_no);

    return MESA_RC_OK;
}

/* ================================================================= *
 *  CLI
 * ================================================================= */

typedef struct {
    mesa_port_speed_t speed;
    mesa_bool_t fdx;
    uint32_t    max_length;

    mesa_bool_t auto_keyword;
    mesa_bool_t bytes;
    mesa_bool_t clear;
    mesa_bool_t errors;
    mesa_bool_t packets;
    mesa_bool_t discards;
    mesa_bool_t equipment;
    mesa_bool_t facility;
    mesa_bool_t far_end;
    mesa_bool_t near_end;

    mesa_bool_t optical;
    mesa_bool_t dac1m;
    mesa_bool_t dac2m;
    mesa_bool_t dac3m;
    mesa_bool_t dac5m;
} port_cli_req_t;

static const char *port_mode_txt(mesa_port_speed_t speed, mesa_bool_t fdx)
{
    switch (speed) {
    case MESA_SPEED_10M:
        return (fdx ? "10fdx" : "10hdx");
    case MESA_SPEED_100M:
        return (fdx ? "100fdx" : "100hdx");
    case MESA_SPEED_1G:
        return (fdx ? "1Gfdx" : "1Ghdx");
    case MESA_SPEED_2500M:
        return (fdx ? "2.5Gfdx" : "2.5Ghdx");
    case MESA_SPEED_5G:
        return (fdx ? "5Gfdx" : "5Ghdx");
    case MESA_SPEED_10G:
        return (fdx ? "10Gfdx" : "10Ghdx");
    case MESA_SPEED_25G:
        return (fdx ? "25Gfdx" : "25Ghdx");
    default:
        return "?";
    }
}

typedef enum {
    CLI_CMD_PORT_STATE,
    CLI_CMD_PORT_MODE,
    CLI_CMD_PORT_FC,
    CLI_CMD_PORT_MAXLEN
} port_cli_cmd_t;

/* Port configuration */
static void cli_cmd_port_conf(cli_req_t *req, port_cli_cmd_t cmd)
{
    mesa_port_no_t        uport, iport;
    port_entry_t          *entry;
    mscc_appl_port_conf_t *pc;
    mesa_port_status_t    *ps;
    mesa_bool_t           first = 1;
    mesa_bool_t           rx, tx;
    port_cli_req_t        *mreq = req->module_req;

    for (iport = 0; iport < mesa_port_cnt(NULL); iport++) {
        uport = iport2uport(iport);
        if (req->port_list[uport] == 0) {
            continue;
        }
        entry = &port_table[iport];
        pc = &entry->conf;
        ps = &entry->status;

        if (req->set) {
            switch (cmd) {
            case CLI_CMD_PORT_STATE:
                pc->admin.enable = req->enable;
                break;
            case CLI_CMD_PORT_MODE:
                pc->autoneg = mreq->auto_keyword;
                if (!pc->autoneg) {
                    pc->speed = mreq->speed;
                    pc->fdx = mreq->fdx;
                }
                break;
            case CLI_CMD_PORT_FC:
                pc->flow_control = req->enable;
                break;
            case CLI_CMD_PORT_MAXLEN:
                pc->max_length = mreq->max_length;
                break;
            default:
                return;
            }
            port_setup(iport, 0);
        } else {
            if (first) {
                cli_table_header("Port  State     Mode    Flow Control  Rx Pause  Tx Pause  MaxFrame  Link      ");
                first = 0;
            }
            rx = pc->flow_control;
            tx = pc->flow_control;
            if (pc->autoneg) {
                rx = 0;
                tx = 0;
                if (ps->link) {
                    rx = ps->aneg.obey_pause;
                    tx = ps->aneg.generate_pause;
                }
            }
            cli_printf("%-6u%-10s%-8s%-14s%s  %s  %-10u%-10s\n",
                       uport,
                       cli_bool_txt(pc->admin.enable),
                       pc->autoneg ? "Auto" : port_mode_txt(pc->speed, pc->fdx),
                       cli_bool_txt(pc->flow_control),
                       cli_bool_txt(rx), cli_bool_txt(tx),
                       pc->max_length,
                       ps->link ? port_mode_txt(ps->speed, ps->fdx) : "Down");
        }
    }
}

static void cli_cmd_port_state(cli_req_t *req)
{
    cli_cmd_port_conf(req, CLI_CMD_PORT_STATE);
}

static void cli_cmd_port_mode(cli_req_t *req)
{
    cli_cmd_port_conf(req, CLI_CMD_PORT_MODE);
}

static void cli_cmd_port_flow_control(cli_req_t *req)
{
    cli_cmd_port_conf(req, CLI_CMD_PORT_FC);
}

static void cli_cmd_port_max_frame(cli_req_t *req)
{
    cli_cmd_port_conf(req, CLI_CMD_PORT_MAXLEN);
}

static void setup_rx_reg(mesa_bool_t enable)
{
    mesa_packet_rx_conf_t conf;
    mesa_queue_t q, qcnt = mesa_capability(NULL, MESA_CAP_PACKET_RX_QUEUE_CNT);

    // Get Rx packet configuration
    if (mesa_packet_rx_conf_get(NULL, &conf) != MESA_RC_OK) {
        T_E("mesa_packet_rx_conf_get failed");
        return;
    }

    for (q = 0; q < qcnt; q++) {
        conf.queue[q].npi.enable = enable;
    }

    // Setup CPU rx registration for BPDU
#define RXQ(_e,_q,_max) (_e ? (_q < _max ? _q : _max - 1) : -1)
    conf.map.bpdu_queue = RXQ(enable, 0, qcnt);
    conf.map.garp_queue = RXQ(enable, 1, qcnt);
    conf.map.learn_queue = RXQ(enable, 2, qcnt);
    conf.map.igmp_queue = RXQ(enable, 3, qcnt);
    conf.map.ipmc_ctrl_queue = RXQ(enable, 4, qcnt);
    conf.map.mac_vid_queue = RXQ(enable, 5, qcnt);

    // Set Rx packet configuration
    if (mesa_packet_rx_conf_set(NULL, &conf) != MESA_RC_OK) {
        T_E("mesa_packet_rx_conf_set failed");
    }
}

static void cli_cmd_port_npi(cli_req_t *req)
{
    mesa_npi_conf_t conf;
    mesa_port_no_t port_no = 1;
    if (mesa_npi_conf_get(NULL, &conf) == MESA_RC_OK) {
        if (req->parm_parsed) {
            conf.enable = req->enable;
            conf.port_no = req->port_no;
            if (mesa_npi_conf_set(NULL, &conf) != MESA_RC_OK) {
                T_E("mesa_npi_conf_set(%u) failed", port_no);
            }
            setup_rx_reg(req->enable);
        }
        cli_printf("NPI: port %u, %s\n", iport2uport(conf.port_no), conf.enable ? "enabled" : "disabled");
    } else {
        T_E("mesa_npi_conf_get(%u) failed", port_no);
    }
}

static void cli_cmd_port_polling(cli_req_t *req)
{
    if (req->set) {
        port_polling = req->enable;
    } else {
        cli_printf("Port Polling: %s\n", cli_bool_txt(port_polling));
    }
}

static void cli_cmd_port_loopback(cli_req_t *req)
{
    mesa_port_no_t        uport, iport;
    mesa_port_test_conf_t conf;
    mesa_bool_t           first = 1;
    port_cli_req_t        *mreq = req->module_req;

    for (iport = 0; iport < mesa_port_cnt(NULL); iport++) {
        uport = iport2uport(iport);
        if (req->port_list[uport] == 0 ||
            mesa_port_test_conf_get(NULL, iport, &conf) != MESA_RC_OK) {
            continue;
        }

        if (req->set) {
            conf.loopback = (mreq->near_end ? MESA_PORT_LB_NEAR_END :
                             mreq->far_end ? MESA_PORT_LB_FAR_END :
                             mreq->facility ? MESA_PORT_LB_FACILITY :
                             mreq->equipment ? MESA_PORT_LB_EQUIPMENT : MESA_PORT_LB_DISABLED);
            if (mesa_port_test_conf_set(NULL, iport, &conf) != MESA_RC_OK) {
                printf("Loopback set failed for port %u\n", uport);
            }
        } else {
            if (first) {
                cli_table_header("Port  Loopback");
                first = 0;
            }
            cli_printf("%-6u%s\n",
                       uport,
                       conf.loopback == MESA_PORT_LB_NEAR_END ? "Near-End" :
                       conf.loopback == MESA_PORT_LB_FAR_END ? "Far-End" :
                       conf.loopback == MESA_PORT_LB_FACILITY ? "Facility" :
                       conf.loopback == MESA_PORT_LB_EQUIPMENT ? "Equipment" : "Disabled");
        }
    }
}

static void cli_cmd_port_cable(cli_req_t *req)
{
    mesa_port_no_t        uport, iport;
    mesa_bool_t           first = 1;
    port_cli_req_t        *mreq = req->module_req;
    mesa_port_conf_t      conf;

    for (iport = 0; iport < mesa_port_cnt(NULL); iport++) {
        uport = iport2uport(iport);
        if (req->port_list[uport] == 0 ||
            mesa_port_conf_get(NULL, iport, &conf) != MESA_RC_OK) {
            continue;
        }
        if (req->set) {
            conf.serdes.media_type = (mreq->optical ? MESA_SD10G_MEDIA_SR :
                                      mreq->dac1m ? MESA_SD10G_MEDIA_DAC_1M :
                                      mreq->dac2m ? MESA_SD10G_MEDIA_DAC_2M :
                                      mreq->dac3m ? MESA_SD10G_MEDIA_DAC_3M :
                                      mreq->dac5m ? MESA_SD10G_MEDIA_DAC_5M : MESA_SD10G_MEDIA_SR);
            if (mesa_port_conf_set(NULL, iport, &conf) != MESA_RC_OK) {
                printf("Loopback set failed for port %u\n", uport);
            }
        } else {
            if (first) {
                cli_table_header("SFP cable type");
                first = 0;
            }
            cli_printf("%-6u%s\n",
                       uport,
                       conf.serdes.media_type == MESA_SD10G_MEDIA_SR ? "Optical" :
                       conf.serdes.media_type == MESA_SD10G_MEDIA_DAC_1M ? "DAC-1M" :
                       conf.serdes.media_type == MESA_SD10G_MEDIA_DAC_2M ? "DAC-2M" :
                       conf.serdes.media_type == MESA_SD10G_MEDIA_DAC_3M ? "DAC-3M" :
                       conf.serdes.media_type == MESA_SD10G_MEDIA_DAC_5M ? "DAC-5M" :
                       conf.serdes.media_type == MESA_SD10G_MEDIA_DAC ? "DAC (unspecified length)" :
                       conf.serdes.media_type == MESA_SD10G_MEDIA_PR_NONE ? "None" : "?");
        }
    }
}

static void cli_cmd_sfp_dump(cli_req_t *req)
{
    uint32_t port_cnt = mesa_capability(NULL, MESA_CAP_PORT_CNT);
    int found = 0;
    char buf[10];
    mesa_port_status_t ps;
    mesa_port_conf_t   conf;

    for (uint32_t port_no = 0; port_no < port_cnt; port_no++) {
        port_entry_t *entry = &port_table[port_no];

        if (entry->media_type == MSCC_PORT_TYPE_SFP) {
            if (entry->sfp_status.present) {
                if (!found) {
                    cli_printf("Port(cli)  SFP-type        Vendor          Rev     SN              Los   API-IF      Speed Link\n");
                    found = 1;
                }
                if ((entry->sfp_type == MEBA_SFP_TRANSRECEIVER_10G_DAC) ||
                    (entry->sfp_type == MEBA_SFP_TRANSRECEIVER_25G_DAC)) {
                    sprintf(buf, "-");
                } else {
                    sprintf(buf, "%s",entry->sfp_status.los?"yes":"no");
                }
                (void)mesa_port_status_get(NULL, port_no, &ps);
                (void)mesa_port_conf_get(NULL, port_no, &conf);
                if (entry->sfp_device != NULL) {
                    cli_printf("%-10d %-15s %-15s %-7s %-15s %-5s %-11s %-5s %-5s\n",port_no + 1,
                               mesa_sfp_if2txt(entry->sfp_type), entry->sfp_device->info.vendor_name,
                               entry->sfp_device->info.vendor_rev, entry->sfp_device->info.vendor_sn, buf,
                               mesa_port_if2txt(conf.if_type), mesa_port_spd2txt(conf.speed), ps.link ? "yes" : "no");
                } else {
                    cli_printf("%-10d %-15s %-15s %-7s %-15s %-5s %-11s %-5s %-5s\n",port_no + 1, mesa_sfp_if2txt(entry->sfp_type),
                               "N/A", "N/A", "N/A", buf,
                               mesa_port_if2txt(conf.if_type), mesa_port_spd2txt(conf.speed), ps.link ? "yes" : "no");
                }
            }
        }
    }
    if (!found) {
        cli_printf("No SFPs found\n");
    }
}

meba_phy_driver_conf_t create_phy_conf() {
    meba_phy_driver_conf_t phy_conf = {};
    phy_conf.speed = MESA_SPEED_AUTO;
    phy_conf.fdx = TRUE;
    phy_conf.admin.enable = TRUE;
    return phy_conf;
}

static void set_phy_driver(mesa_port_no_t port_no, uint32_t model,
                           meba_phy_driver_address_t address_mode) {
    port_entry_t    *entry = &port_table[port_no];
    meba_phy_driver_t *driver = phy_drivers;

    while (driver != NULL) {
        if ((driver->id & driver->mask) == (model & driver->mask)) {
            entry->phy_device = driver->meba_phy_driver_probe(driver, &address_mode);
            meba_phy_driver_conf_t phy_conf = create_phy_conf();
            driver->meba_phy_driver_conf_set(entry->phy_device, entry->meba.cap, &phy_conf);
            T_D("Port: %d, create device for driver: %x", port_no, driver->id);
        }
        driver = driver->next;
    }
}

static void set_aqr_driver(mesa_port_no_t port_no) {
    uint16_t model = 0;
    meba_phy_driver_address_t address_mode = {};
    address_mode.mode = mscc_phy_driver_address_mode;
    address_mode.val.mscc_address.mmd_read = mesa_port_mmd_read;
    address_mode.val.mscc_address.mmd_write = mesa_port_mmd_write;
    address_mode.val.mscc_address.miim_read = mesa_miim_read;
    address_mode.val.mscc_address.miim_write = mesa_miim_write;
    address_mode.val.mscc_address.inst = NULL;
    address_mode.val.mscc_address.port_no = port_no;
    address_mode.val.mscc_address.meba_inst = meba_global_inst;
    address_mode.val.mscc_address.debug_func = NULL;
    address_mode.val.mscc_address.mac_if= MESA_PORT_INTERFACE_SFI;
    (void)mesa_port_mmd_read(NULL, port_no, 0x1e, 0x3, &model);
    set_phy_driver(port_no, model, address_mode);
}

static void add_aqr_driver() {
    void *dlh = dlopen("/lib/phy_drivers/libphy_aqr.so", RTLD_NOW|RTLD_NODELETE);
    if (!dlh) {
        T_E("Unable to load aqr driver");
        return;
    }
    meba_phy_drivers_t (*drv)(void) = dlsym(dlh, "driver_init");
    if (!drv) {
        T_E("Unable to locate 'driver_init' func");
        dlclose(dlh);
        return;
    }
    meba_phy_drivers_t res = drv();
    for (int i = 0; i < res.count; ++i) {
        res.phy_drv[i].next = phy_drivers;
        phy_drivers = &res.phy_drv[i];
    }

    dlclose(dlh);
}

static void cli_cmd_phy_scan(cli_req_t *req)
{
    uint16_t value, reg2, reg3, oui, model, adr;
    mesa_bool_t found = FALSE, found_mmd = FALSE;
    for (mesa_miim_controller_t miim_ctrl = MESA_MIIM_CONTROLLER_0; miim_ctrl < MESA_MIIM_CONTROLLERS; miim_ctrl++) {
        for (adr = 0; adr < 32; adr++) {
            if (mesa_miim_read(NULL, 0, miim_ctrl, adr, 0, &value) == MESA_RC_OK) {
                mesa_miim_read(NULL, 0, miim_ctrl, adr, 2, &reg2);
                mesa_miim_read(NULL, 0, miim_ctrl, adr, 3, &reg3);
                oui = ((reg2 << 6) | ((reg3 >> 10) & 0x3F));
                model = ((reg3 >> 4) & 0x3F);
                cli_printf("MIIM Ctrl:%d MIIM addr:%d - Found Phy (OUI:0x%x Model:0x%x)\n",miim_ctrl, adr, oui, model);
                found = TRUE;
            }
        }
        for (adr = 0; adr < 32; adr++) {
            if (mesa_mmd_read(NULL, 0, miim_ctrl, adr, 0, 0, &value) == MESA_RC_OK) {
                cli_printf("MIIM Ctrl:%d MIIM addr:%d mmd:0 addr:0 = %x - Found MMD Phy\n",miim_ctrl, adr, value);
                found_mmd = TRUE;
            }
        }
    }
    if (!found) {
        cli_printf("No phys found\n");
    }
    if (!found_mmd) {
        cli_printf("No mmd phys found\n");
    }
}

/* Print counters in two columns with header */
static void cli_cmd_stat_port(mesa_port_no_t port, mesa_bool_t *first, const char *name,
                              uint64_t c1, uint64_t c2)
{
    char buf[80], *p;

    if (*first) {
        *first = 0;
        p = &buf[0];
        p += sprintf(p, "Port  Rx %-17sTx %-17s", name, name);
        cli_table_header(buf);
    }
    cli_printf("%-2u    %-20llu%-20llu\n", port, c1, c2);
}

/* Print two counters in columns */
static void cli_cmd_stats(const char *col1, const char *col2, uint64_t c1, uint64_t c2)
{
    char buf[80];

    sprintf(buf, "Rx %s:", col1);
    cli_printf("%-19s%19llu   ", buf, c1);
    if (col2 != NULL) {
        sprintf(buf, "Tx %s:", strlen(col2) ? col2 : col1);
        cli_printf("%-19s%19llu", buf, c2);
    }
    cli_printf("\n");
}

/* Port statistics */
static void cli_cmd_port_stats(cli_req_t *req)
{
    mesa_port_no_t                     uport, iport, i;
    mesa_port_counters_t               counters;
    mesa_port_rmon_counters_t          *rmon = &counters.rmon;
    mesa_port_if_group_counters_t      *if_group = &counters.if_group;
    mesa_port_ethernet_like_counters_t *elike = &counters.ethernet_like;
    mesa_port_prio_counters_t          *prio;
    mesa_bool_t                        first = 1;
    char                               buf[32];
    uint32_t                           rmon_frame_max = (mesa_capability(NULL, MESA_CAP_MISC_CHIP_FAMILY) == MESA_CHIP_FAMILY_CARACAL ? 1526 : 1518);
    port_cli_req_t                     *mreq = req->module_req;

    for (iport = 0; iport < mesa_port_cnt(NULL); iport++) {
        uport = iport2uport(iport);
        // Do only change setting for ports in the port list
        if (req->port_list[uport] == 0) {
          continue;
        }
        /* Handle 'clear' command */
        if (mreq->clear) {
            mesa_port_counters_clear(NULL, iport);
            continue;
        }

        /* Get counters */
        if (mesa_port_counters_get(NULL, iport, &counters) != MESA_RC_OK)
            continue;

        /* Handle 'packet' command */
        if (mreq->packets) {
            cli_cmd_stat_port(uport, &first, "Packets",
                              rmon->rx_etherStatsPkts, rmon->tx_etherStatsPkts);
            continue;
        }

        /* Handle 'bytes' command */
        if (mreq->bytes) {
            cli_cmd_stat_port(uport, &first, "Octets",
                              rmon->rx_etherStatsOctets, rmon->tx_etherStatsOctets);
            continue;
        }

        /* Handle 'errors' command */
        if (mreq->errors) {
            cli_cmd_stat_port(uport, &first, "Errors",
                              if_group->ifInErrors, if_group->ifOutErrors);
            continue;
        }

        /* Handle 'discards' command */
        if (mreq->discards) {
            cli_cmd_stat_port(uport, &first, "Discards",
                              if_group->ifInDiscards, if_group->ifOutDiscards);
            continue;
        }

        /* Handle default command */
        cli_printf("%sPort %u Statistics:\n\n", first ? "" : "\n", uport);
        first = 0;
        cli_cmd_stats("Packets", "", rmon->rx_etherStatsPkts, rmon->tx_etherStatsPkts);
        cli_cmd_stats("Octets", "", rmon->rx_etherStatsOctets, rmon->tx_etherStatsOctets);
        cli_cmd_stats("Unicast", "", if_group->ifInUcastPkts, if_group->ifOutUcastPkts);
        cli_cmd_stats("Multicast", "", rmon->rx_etherStatsMulticastPkts,
                      rmon->tx_etherStatsMulticastPkts);
        cli_cmd_stats("Broadcast", "", rmon->rx_etherStatsBroadcastPkts,
                      rmon->tx_etherStatsBroadcastPkts);
        cli_cmd_stats("Pause", "", elike->dot3InPauseFrames, elike->dot3OutPauseFrames);
        cli_printf("\n");
        cli_cmd_stats("64", "", rmon->rx_etherStatsPkts64Octets,
                      rmon->tx_etherStatsPkts64Octets);
        cli_cmd_stats("65-127", "", rmon->rx_etherStatsPkts65to127Octets,
                      rmon->tx_etherStatsPkts65to127Octets);
        cli_cmd_stats("128-255", "", rmon->rx_etherStatsPkts128to255Octets,
                      rmon->tx_etherStatsPkts128to255Octets);
        cli_cmd_stats("256-511", "", rmon->rx_etherStatsPkts256to511Octets,
                      rmon->tx_etherStatsPkts256to511Octets);
        cli_cmd_stats("512-1023", "", rmon->rx_etherStatsPkts512to1023Octets,
                      rmon->tx_etherStatsPkts512to1023Octets);
        sprintf(buf, "1024-%u", rmon_frame_max);
        cli_cmd_stats(buf, "", rmon->rx_etherStatsPkts1024to1518Octets,
                      rmon->tx_etherStatsPkts1024to1518Octets);
        sprintf(buf, "%u-    ", rmon_frame_max + 1);
        cli_cmd_stats(buf, "", rmon->rx_etherStatsPkts1519toMaxOctets,
                      rmon->tx_etherStatsPkts1519toMaxOctets);
        cli_printf("\n");

        // The priority queues does not apply to the host interface
        for (i = 0; i < MESA_PRIO_CNT; i++) {
            prio = &counters.prio[i];
            sprintf(buf, "Packets_%u", i);
            cli_cmd_stats(buf, "", prio->rx, prio->tx);
        }
        cli_printf("\n");

        cli_cmd_stats("Drops", "", rmon->rx_etherStatsDropEvents,
                      rmon->tx_etherStatsDropEvents);
        if (mesa_capability(NULL, MESA_CAP_PORT_CNT_ETHER_LIKE)){
            cli_cmd_stats("CRC/Alignment", "Late Collisions", rmon->rx_etherStatsCRCAlignErrors,
                          elike->dot3StatsLateCollisions);
            cli_cmd_stats("Symbol", "Excessive Coll.", elike->dot3StatsSymbolErrors,
                          elike->dot3StatsExcessiveCollisions);
            cli_cmd_stats("Undersize", "Carrier Sense", rmon->rx_etherStatsUndersizePkts,
                          elike->dot3StatsCarrierSenseErrors);
        } else {
            cli_cmd_stats("CRC/Alignment", "Late/Exc. Coll.", rmon->rx_etherStatsCRCAlignErrors,
                          if_group->ifOutErrors);
            cli_cmd_stats("Undersize", NULL, rmon->rx_etherStatsUndersizePkts, 0);

        }
        cli_cmd_stats("Oversize", NULL, rmon->rx_etherStatsOversizePkts, 0);
        cli_cmd_stats("Fragments", NULL, rmon->rx_etherStatsFragments, 0);
        cli_cmd_stats("Jabbers", NULL, rmon->rx_etherStatsJabbers, 0);
        cli_cmd_stats("Filtered", NULL, counters.bridge.dot1dTpPortInDiscards, 0);
    } /* Port loop */
}

static cli_cmd_t cli_cmd_table[] = {
    {
        "Port State [<port_list>] [enable|disable]",
        "Set or show the port administrative state",
        cli_cmd_port_state
    },
    {
        "Port Mode [<port_list>] [10hdx|10fdx|100hdx|100fdx|1000fdx|2500|5g|10g|25g|auto]",
        "Set or show the port speed and duplex mode",
        cli_cmd_port_mode
    },
    {
        "Port Flow Control [<port_list>] [enable|disable]",
        "Set or show the port flow control mode",
        cli_cmd_port_flow_control
    },
    {
        "Port MaxFrame [<port_list>] [<max_frame>]",
        "Set or show the port maximum frame size",
        cli_cmd_port_max_frame
    },
    {
        "Port Statistics [<port_list>] [clear|packets|bytes|errors|discards]",
        "Show port statistics",
        cli_cmd_port_stats
    },
    {
        "Port NPI [<port_no>] [enable|disable]",
        "Enable/disable NPI port",
        cli_cmd_port_npi,
    },
    {
        "Port Loopback [<port_list>] [near-end|far-end|facility|equipment] [enable|disable]",
        "Set or show the port forwarding mode",
        cli_cmd_port_loopback
    },
    {
        "Debug Port cable [<port_list>] [optical|dac-1m|dac-2m|dac-3m|dac-5m]",
        "Set or show the port forwarding mode",
        cli_cmd_port_cable
    },

    {
        "Debug Port Polling [enable|disable]",
        "Set or show the port polling mode",
        cli_cmd_port_polling
    },
    {
        "Debug sfp dump",
        "Shows all detected SFPs",
        cli_cmd_sfp_dump
    },
    {
        "Debug phy scan",
        "Shows all detected phys (over all controlers)",
        cli_cmd_phy_scan
    },
};

static int cli_parm_max_frame(cli_req_t *req)
{
    port_cli_req_t *mreq = req->module_req;
    return cli_parm_u32(req, &mreq->max_length, MESA_MAX_FRAME_LENGTH_STANDARD, mesa_capability(NULL, MESA_CAP_PORT_FRAME_LENGTH_MAX));
}

static int cli_parm_keyword(cli_req_t *req)
{
    const char     *found;
    port_cli_req_t *mreq = req->module_req;

    if ((found = cli_parse_find(req->cmd, req->stx)) == NULL)
        return 1;

    if (!strncasecmp(found, "auto", 4))
        mreq->auto_keyword = 1;
    else if (!strncasecmp(found, "bytes", 5))
        mreq->bytes = 1;
    else if (!strncasecmp(found, "clear", 5))
        mreq->clear = 1;
    else if (!strncasecmp(found, "discards", 8))
        mreq->discards = 1;
    else if (!strncasecmp(found, "errors", 6))
        mreq->errors = 1;
    else if (!strncasecmp(found, "packets", 7))
        mreq->packets = 1;
    else if (!strncasecmp(found, "10hdx", 5))
        mreq->speed = MESA_SPEED_10M;
    else if (!strncasecmp(found, "10fdx", 5)) {
        mreq->speed = MESA_SPEED_10M;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "100hdx", 6))
        mreq->speed = MESA_SPEED_100M;
    else if (!strncasecmp(found, "100fdx", 6)) {
        mreq->speed = MESA_SPEED_100M;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "1000fdx", 7)) {
        mreq->speed = MESA_SPEED_1G;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "2500", 4)) {
        mreq->speed = MESA_SPEED_2500M;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "5g", 2)) {
        mreq->speed = MESA_SPEED_5G;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "10g", 3)) {
        mreq->speed = MESA_SPEED_10G;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "25g", 3)) {
        mreq->speed = MESA_SPEED_25G;
        mreq->fdx = 1;
    } else if (!strncasecmp(found, "equipment", 9)) {
        mreq->equipment = 1;
    } else if (!strncasecmp(found, "facility", 8)) {
        mreq->facility = 1;
    } else if (!strncasecmp(found, "far-end", 7)) {
        mreq->far_end = 1;
    } else if (!strncasecmp(found, "near-end", 8)) {
        mreq->near_end = 1;
    } else if (!strncasecmp(found, "optical", 7)) {
        mreq->optical = 1;
    } else if (!strncasecmp(found, "dac-1m", 6)) {
        mreq->dac1m = 1;
    } else if (!strncasecmp(found, "dac-2m", 6)) {
        mreq->dac2m = 1;
    } else if (!strncasecmp(found, "dac-3m", 6)) {
        mreq->dac3m = 1;
    } else if (!strncasecmp(found, "dac-5m", 6)) {
        mreq->dac5m = 1;
    } else
        cli_printf("no match: %s\n", found);

    return 0;
}

static cli_parm_t cli_parm_table[] = {
    {
        "10hdx|10fdx|100hdx|100fdx|1000fdx|2500|5g|10g|25g|auto",
        "10hdx      : 10 Mbps, half duplex\n"
        "10fdx      : 10 Mbps, full duplex\n"
        "100hdx     : 100 Mbps, half duplex\n"
        "100fdx     : 100 Mbps, full duplex\n"
        "1000fdx    : 1 Gbps, full duplex\n"
        "2500       : 2.5 Gbps, full duplex\n"
        "5g         : 5 Gbps, full duplex\n"
        "10g        : 10 Gbps, full duplex\n"
        "25g        : 25 Gbps, full duplex\n"
        "auto       : Auto negotiation of speed and duplex\n"
        "(default: Show configured and current mode)",
        CLI_PARM_FLAG_NO_TXT | CLI_PARM_FLAG_SET,
        cli_parm_keyword
    },
    {
        "clear|packets|bytes|errors|discards",
        "clear      : Clear port statistics\n"
        "packets    : Show packet statistics\n"
        "bytes      : Show byte statistics\n"
        "errors     : Show error statistics\n"
        "discards   : Show discard statistics\n"
        "(default: Show all port statistics)",
        CLI_PARM_FLAG_NO_TXT,
        cli_parm_keyword
    },
    {
        "<max_frame>",
        "Port maximum frame size, default: Show maximum frame size",
        CLI_PARM_FLAG_NONE | CLI_PARM_FLAG_SET,
        cli_parm_max_frame
    },
    {
        "near-end|far-end|facility|equipment",
        "near-end   : Loopback from Tx to Rx in PHY\n"
        "far-end    : Loopback from Rx to Tx in PHY\n"
        "equipment  : Loopback from Tx to Rx in SerDes\n"
        "facility   : Loopback from Rx to Tx in SerDes\n"
        "(default: Show loopback mode)",
        CLI_PARM_FLAG_NO_TXT | CLI_PARM_FLAG_SET,
        cli_parm_keyword
    },
    {
        "optical|dac-1m|dac-2m|dac-3m|dac-5m",
        "optical    : Optical/fiber cable\n"
        "dac-1m     : 1m DAC\n"
        "dac-2m     : 2m DAC\n"
        "dac-3m     : 3m DAC\n"
        "dac-5m     : 5m DAC\n",
        CLI_PARM_FLAG_NO_TXT | CLI_PARM_FLAG_SET,
        cli_parm_keyword
    },

};

static void port_cli_init(void)
{
    int i;

    /* Register commands */
    for (i = 0; i < sizeof(cli_cmd_table)/sizeof(cli_cmd_t); i++) {
        mscc_appl_cli_cmd_reg(&cli_cmd_table[i]);
    }

    /* Register parameters */
    for (i = 0; i < sizeof(cli_parm_table)/sizeof(cli_parm_t); i++) {
        mscc_appl_cli_parm_reg(&cli_parm_table[i]);
    }
}

/* ================================================================= *
 *  Initialization
 * ================================================================= */

static void port_init(meba_inst_t inst)
{
    uint32_t              port_cnt = mesa_capability(NULL, MESA_CAP_PORT_CNT);
    mesa_port_no_t        port_no;
    port_entry_t          *entry;
    mscc_appl_port_conf_t *pc;
    mesa_bool_t           first = TRUE;

    // Free old port table
    if (port_table != NULL) {
        free(port_table);
    }
    /* Store the meba inst globally */
    meba_global_inst = inst;

    // Initialize ports
    if ((port_table = calloc(port_cnt, sizeof(*port_table))) == NULL) {
        T_E("port_table calloc() failed");
        return;
    }
    /* Store the meba inst globally */
    meba_global_inst = inst;

    /* Set front status LED */
    MEBA_WRAP(meba_status_led_set, inst, MEBA_LED_TYPE_FRONT, MEBA_LED_COLOR_GREEN);

    // Port reset
    MEBA_WRAP(meba_reset, inst, MEBA_PORT_RESET);

    for (port_no = 0; port_no < port_cnt; port_no++) {
        entry = &port_table[port_no];
        pc = &entry->conf;
        if (MEBA_WRAP(meba_port_entry_get, inst, port_no, &entry->meba) != MESA_RC_OK) {
            continue;
        }
        meba_port_cap_t  cap = entry->meba.cap;
        entry->valid = 1;
        pc->admin.enable = 1;
        pc->fdx = 1;
        pc->max_length = MESA_MAX_FRAME_LENGTH_STANDARD;
        switch (entry->meba.mac_if) {
        case MESA_PORT_INTERFACE_100FX:
            entry->media_type = MSCC_PORT_TYPE_CU;
            pc->speed = MESA_SPEED_100M;
            break;
        case MESA_PORT_INTERFACE_SGMII_CISCO:
            entry->media_type = MSCC_PORT_TYPE_SFP;
            pc->speed = MESA_SPEED_1G;
            pc->autoneg = 1;
            break;
        case MESA_PORT_INTERFACE_XAUI:
        case MESA_PORT_INTERFACE_SFI:
            if (cap & MEBA_PORT_CAP_COPPER) {
                entry->media_type = MSCC_PORT_TYPE_CU; // AQR phys in SFI mode
            } else {
                entry->media_type = MSCC_PORT_TYPE_SFP;
            }
            pc->speed = (cap & MEBA_PORT_CAP_25G_FDX) ? MESA_SPEED_25G : MESA_SPEED_10G;
            break;
        case MESA_PORT_INTERFACE_SGMII:
        case MESA_PORT_INTERFACE_RGMII:
        case MESA_PORT_INTERFACE_QSGMII:
        case MESA_PORT_INTERFACE_QXGMII:
            entry->media_type = MSCC_PORT_TYPE_CU;
            pc->speed = MESA_SPEED_1G;
            pc->autoneg = 1;
            pc->flow_control = 1;
            break;
        case MESA_PORT_INTERFACE_SERDES:
            entry->media_type = MSCC_PORT_TYPE_SFP;
            pc->speed = MESA_SPEED_1G;
            break;
        case MESA_PORT_INTERFACE_NO_CONNECTION: //CPMOD
            entry->media_type = MSCC_PORT_TYPE_CU;
            pc->speed = MESA_SPEED_UNDEFINED;
            pc->admin.enable = 0;
            break;
        default:
            T_E("unknown if_type on port %u", port_no);
            break;
        }
        if (entry->media_type == MSCC_PORT_TYPE_CU) {
            if ((cap & MEBA_PORT_CAP_10G_FDX)) {
                if (first) {
                    add_aqr_driver(); // Add drivers for Aqr phys
                    first = FALSE;
                }
                set_aqr_driver(port_no);
            } else {
                mesa_phy_reset_conf_t phy_reset;
                if (mesa_phy_reset_get(NULL, port_no, &phy_reset) != MESA_RC_OK) {
                    T_E("mesa_phy_reset_get(%u) failed", port_no);
                    continue;
                }
                phy_reset.mac_if = entry->meba.mac_if;
                phy_reset.media_if = MESA_PHY_MEDIA_IF_CU;
                if (mesa_phy_reset(NULL, port_no, &phy_reset) != MESA_RC_OK) {
                    T_E("mesa_phy_reset(%u) failed", port_no);
                    continue;
                }
            }
        } else {
            /* Disable Clause 37 per default */
            mesa_port_clause_37_control_t ctrl = {0};
            if (mesa_port_clause_37_control_set(NULL, port_no, &ctrl) != MESA_RC_OK) {
                T_E("mesa_port_clause_37_control_set(%u) failed", port_no);
            }
        }

        port_setup(port_no, 0);

        if (port_no == loop_port) { // This port is the active loop port
            cli_printf("Using port %u as loop-port\n", loop_port + 1);

            /* Enable injection header */
            mesa_port_ifh_t ifh_conf;
            ifh_conf.ena_inj_header = TRUE;
            ifh_conf.ena_xtr_header = FALSE;
            (void)mesa_port_ifh_conf_set(NULL, port_no, &ifh_conf);

            /* Force operational state up */
            mesa_port_state_set(NULL, port_no, 1);

            /* Disable CPU copy/redirect on loop port */
            mesa_packet_rx_port_conf_t packet_port_conf;
            if (mesa_packet_rx_port_conf_get(NULL, port_no, &packet_port_conf) == MESA_RC_OK) {
                int j;

                packet_port_conf.ipmc_ctrl_reg = MESA_PACKET_REG_FORWARD;
                packet_port_conf.igmp_reg = MESA_PACKET_REG_FORWARD;
                packet_port_conf.mld_reg = MESA_PACKET_REG_FORWARD;
                for (j = 0; j < 16; j++) {
                    packet_port_conf.bpdu_reg[j] = MESA_PACKET_REG_FORWARD;
                    packet_port_conf.garp_reg[j] = MESA_PACKET_REG_FORWARD;
                }
                (void)mesa_packet_rx_port_conf_set(NULL, port_no, &packet_port_conf);
            }

            // The loop_port is not member of any VLAN. No forwarding to this port
            mesa_port_list_t  port_list;
            for (uint16_t i=1; i<=0xFFF; ++i) {
                if (MESA_RC_OK != mesa_vlan_port_members_get(NULL, i, &port_list)) {
                    cli_printf("mesa_vlan_port_members_get(%u) failed\n", i);
                    break;
                }
                mesa_port_list_set(&port_list, loop_port, 0);
                if (MESA_RC_OK != mesa_vlan_port_members_set(NULL, i, &port_list)) {
                    cli_printf("mesa_vlan_port_members_set(%u) failed\n", i);
                    break;
                }
            }
        }
    } // Port loop

    MEBA_WRAP(meba_reset, inst, MEBA_PORT_RESET_POST);
    MEBA_WRAP(meba_reset, inst, MEBA_PORT_LED_INITIALIZE);
}

static meba_sfp_device_t *create_device(meba_inst_t inst, meba_sfp_driver_t *driver,
                                        mesa_port_no_t port_no, meba_sfp_device_info_t *sfp_info) {
    meba_sfp_driver_address_t address_mode;
    address_mode.mode = mscc_sfp_driver_address_mode;
    address_mode.val.mscc_address.inst = NULL;
    address_mode.val.mscc_address.port_no = port_no;
    address_mode.val.mscc_address.meba_inst = inst;

    meba_sfp_device_info_t device_info = *sfp_info;
    meba_sfp_device_t *device = driver->meba_sfp_driver_probe(driver, &address_mode, &device_info);
    return device;
}

static void check_sfp_drv_status(meba_inst_t inst, mesa_port_no_t port_no, mesa_bool_t sfp_is_inserted) {
    meba_sfp_device_info_t info;
    port_entry_t *entry = &port_table[port_no];
    meba_sfp_driver_t *sfp_driver = &entry->sfp_driver;

    if (!sfp_is_inserted) {
        // Remove the SFP
        if (entry->sfp_device == NULL) {
            T_E("Port:%u Cannot remove, device not installed", port_no);
            return;
        }
        if (entry->sfp_device->drv->meba_sfp_driver_delete(entry->sfp_device) != MESA_RC_OK) {
            T_E("Port:%u Could not delete SFP device", port_no);
        }
        memset(sfp_driver, 0, sizeof(meba_sfp_driver_t));
        entry->sfp_type = MEBA_SFP_TRANSRECEIVER_NONE;
        entry->sfp_status.tx_fault = TRUE;
        entry->sfp_status.los = TRUE;
        return;
    }

    // A SFP is inserted.
    // Now read the ROM through MEBA and install the driver according to the MSA standard
    if (meba_fill_driver(inst, port_no, sfp_driver, &info) == FALSE) {
        T_E("Port:%u Could not read from SFP", port_no);
        return;
    }
    T_I("SFP vendor:'%s' pn:'%s'", info.vendor_name, info.vendor_pn);

    meba_sfp_device_t *sfp_device = create_device(inst, sfp_driver, port_no, &info);
    if (sfp_device == NULL) {
        T_E("Port:%u Could not create SFP device", port_no);
        return;
    }
    entry->sfp_device = sfp_device;


    if (sfp_device->drv->meba_sfp_driver_tr_get(sfp_device, &entry->sfp_type) != MESA_RC_OK) {
        T_E("Port:%u Could not get SFP tranceiver type", port_no);
    }
}

static mesa_bool_t port_is_aneg_mode(port_entry_t *entry) {
    if (entry->conf.autoneg ||
        ((entry->media_type == MSCC_PORT_TYPE_SFP) &&
         (entry->sfp_type == MEBA_SFP_TRANSRECEIVER_1000BASE_T))) {
        return TRUE;
    }
    return FALSE;
}

void port_poll(meba_inst_t inst)
{
    uint32_t              port_cnt = mesa_capability(NULL, MESA_CAP_PORT_CNT);
    mesa_port_no_t        port_no;
    port_entry_t          *entry;
    mesa_port_status_t    *ps;
    mesa_bool_t           link_old;
    mscc_appl_port_conf_t *pc;
    mesa_port_counters_t  counters;

    // Poll port status
    T_N("enter");

    // One sec poll of the MESA API
    if (mesa_poll_1sec(NULL) != MESA_RC_OK) {
        T_E("mesa_poll_1sec() failed");
    }

    // Poll port status
    for (port_no = 0; port_no < port_cnt; port_no++) {
        if (port_no == loop_port) {
            continue;
        }
        entry = &port_table[port_no];
        if (!entry->valid) {
            continue;
        }

        ps = &entry->status;
        pc = &entry->conf;
        link_old = ps->link;
        memset(&counters, 0, sizeof(counters));

        if (entry->media_type == MSCC_PORT_TYPE_SFP && (entry->meba.cap & MEBA_PORT_CAP_SFP_DETECT)) {
            meba_sfp_status_t old_sfp_status = entry->sfp_status;
            /* Fetch SFP port status (presence, Tx fault and LoS) using MEBA */
            if (MEBA_WRAP(meba_sfp_status_get, inst, port_no, &entry->sfp_status) != MESA_RC_OK) {
                T_D("Failed to read SFP port %u status through MEBA.", port_no);
            } else if (old_sfp_status.present != entry->sfp_status.present) {
                T_I("SFP was %s port %d", entry->sfp_status.present ? "inserted in" : "removed from", port_no);
                check_sfp_drv_status(inst, port_no, entry->sfp_status.present);
                if (entry->sfp_status.present) {
                    port_setup(port_no, 0);
                    ps->link = FALSE;
                } else {
                    MEBA_WRAP(meba_port_entry_get, inst, port_no, &entry->meba);
                }
            }
        }

        /* Poll port status and update the status data structure */
        if (port_status_poll(port_no) != MESA_RC_OK) {
            continue;
        }

        /* Detect link down and disable forwarding on port */
        if ((!ps->link || ps->link_down) && link_old) {
            T_I("link down event on port_no: %u", port_no);
            link_old = 0;
            mesa_port_state_set(NULL, port_no, FALSE);
            mesa_mac_table_port_flush(NULL, port_no);
        }

        /* Detect link up and setup port */
        if (ps->link && !link_old) {
            T_I("link up event on port_no: %u", port_no);
            mesa_port_state_set(NULL, port_no, TRUE);
            if (port_is_aneg_mode(entry)) {
                port_setup(port_no, TRUE);
            }
        }

        /* Get port counters from the API */
        mesa_port_counters_get(NULL, port_no, &counters);
        /* Update port LED  through MEBA */
        MEBA_WRAP(meba_port_led_update, inst, port_no, ps, &counters, &pc->admin);
    }
    port_poll_cnt++;
    if (port_poll_cnt == 5) {
        // Assume warm start ready
        T_I("mesa_restart_conf_end");
        mesa_restart_conf_end(NULL);
    }
    T_N("exit");
}

void mscc_appl_port_init(mscc_appl_init_t *init)
{
    mesa_rc rc;
    size_t ret_len = 0;
    char   port_buf[100] = {};

    switch (init->cmd) {
    case MSCC_INIT_CMD_REG:
        mscc_appl_trace_register(&trace_module, trace_groups, TRACE_GROUP_CNT);
        break;

    case MSCC_INIT_CMD_INIT:
        rc = init->board_inst->iface.conf_get("mep_loop_port", port_buf,
                                              (size_t)sizeof(port_buf),
                                              &ret_len);
        if (rc == MESA_RC_OK && ret_len) {
            loop_port = atoi(port_buf);
        }

        port_init(init->board_inst);
        port_cli_init();
        break;

    case MSCC_INIT_CMD_INIT_WARM:
        port_init(init->board_inst);
        break;

    case MSCC_INIT_CMD_POLL:
        if (port_polling) {
            port_poll(init->board_inst);
        }
        break;

    default:
        break;
    }
}
