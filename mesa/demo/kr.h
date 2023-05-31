// Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
// SPDX-License-Identifier: MIT

#ifndef _MSCC_APPL_PORT_H_
#define _MSCC_APPL_PORT_H_

#include "mscc/ethernet/switch/api.h"
#include "trace.h"

char *mesa_port_spd2txt(mesa_port_speed_t speed);

#define KR_HIST_NUM 500

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define BT(x)            (1 << (x))
#define VTSS_BITMASK(x)  ((1 << (x)) - 1)
#define BITFIELD(x,o,w)  (((x) >> (o)) & VTSS_BITMASK(w))

    static mscc_appl_trace_module_t trace_module = {
    .name = "kr"
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

typedef enum {
    CM1,
    C0,
    CP1,
} kr_tap_t;

typedef struct {
    mesa_kr_status_results_t res;
    uint32_t time;
} kr_coef_t;

typedef struct {
    mesa_ber_stage_t ber_training_stage;
    uint16_t ber_coef_frm;
    uint16_t ber_status_frm;
    uint32_t time;
    uint32_t irq;
} kr_ber_t;

typedef struct {
    uint32_t time;
    uint32_t irq;
    uint32_t port;
} kr_irq_t;

typedef struct {
    mesa_port_kr_state_t state;
    struct timeval time_start_aneg;
    struct timeval time_start_train;
    uint32_t time_ld;
    uint32_t time_lp;
    uint32_t time_since_restart;
    mesa_port_kr_status_t status;
    kr_coef_t ld_hist[KR_HIST_NUM];
    uint16_t ld_hist_index;
    kr_ber_t lp_hist[KR_HIST_NUM];
    uint16_t lp_hist_index;
    kr_irq_t irq_hist[KR_HIST_NUM];
    uint16_t irq_hist_index;
    kr_irq_t irq_glb_hist[KR_HIST_NUM];
    uint16_t irq_glb_hist_index;
} kr_appl_train_t;

typedef struct {
    kr_appl_train_t tr;
    mesa_port_speed_t next_parallel_spd;
    mesa_bool_t cap_25g;
    mesa_bool_t cap_10g;

    // Debug
    mesa_bool_t use_ber;
    uint32_t stop_train;
    mesa_bool_t gen1_wait;
    uint32_t aneg_sm_state;
    uint32_t aneg_sm_deb;
    mesa_bool_t debug_ena;
    mesa_bool_t aneg_enable;
    mesa_bool_t tr_done;
} kr_appl_conf_t;

#endif /* _MSCC_APPL_PORT_H_ */
