// Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
// SPDX-License-Identifier: MIT


#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_HW_PROT
#include "vtss_fa_cil.h"

#if defined(VTSS_ARCH_FA)
#if defined(VTSS_FEATURE_HW_PROT)

#include "vtss_fa_tupe.h"

// #define TUPE_TEST // Include test functions

/* - CIL functions ------------------------------------------------- */

/* Perform a TUPE command */
vtss_rc fa_tupe_cmd(vtss_state_t *vtss_state,
                    vtss_tupe_cmd_t cmd, vtss_tupe_parms_t *parms)
{
    return VTSS_RC_OK;
}

/* Allocate a TUPE value (bits or value) */
vtss_rc fa_tupe_alloc(vtss_state_t *vtss_state,
                      vtss_tupe_val_type_t type, vtss_tupe_val_t *val)
{
    return VTSS_RC_OK;
}

/* Free a TUPE value (bits or value) */
vtss_rc fa_tupe_free(vtss_state_t *vtss_state,
                     vtss_tupe_val_t val)
{
    return VTSS_RC_OK;
}
}

/* Configure VLAN/VSI entry with TUPE value */
vtss_rc fa_tupe_vlan_set(vtss_state_t *vtss_state,
                         u32 addr, // VLAN/VSI address
                         vtss_tupe_val_type_t type, vtss_tupe_val_t val)
{
    return VTSS_RC_OK;
}

/* Get VLAN/VSI entry TUPE value */
vtss_rc fa_tupe_vlan_get(vtss_state_t *vtss_state,
                         u32 addr, // VLAN/VSI address
                         vtss_tupe_val_type_t *type, vtss_tupe_val_t *val)
{
    return VTSS_RC_OK;
}

/* Clear VLAN/VSI entry TUPE value */
vtss_rc fa_tupe_vlan_clr(vtss_state_t *vtss_state,
                         u32 addr) // VLAN/VSI address
{
    return VTSS_RC_OK;
}

/* TUPE callback function. Called for each already allocated TUPE value changing due to re-initialization
   where split between linear and ring protection values change */
static fa_tupe_cb_t fa_tupe_cb;

/* Register TUPE callback function (only one function may be registered): */
void fa_tupe_cb_register(fa_tupe_cb_t cb)
{
    if (cb == NULL) {
        VTSS_D("call back pointer invalid");
        return;
    }
    fa_tupe_cb = cb;
}

/* realloc existing entries to fit new scheme */
static vtss_rc tupe_realloc(vtss_state_t *vtss_state, u8 tupe_linear_prot_bits)
{
    return VTSS_RC_ERROR;
}

/* Allocate an AFI TUPE value (use is optional) */
vtss_rc fa_afi_tupe_alloc(vtss_state_t *vtss_state,
                          vtss_afi_tupe_val_t *val)
{
    return VTSS_RC_ERR_NO_RES;
}

/* Free an AFI TUPE value (use is optional) */
vtss_rc fa_afi_tupe_free(vtss_state_t *vtss_state,
                         vtss_afi_tupe_val_t val)
{
    return VTSS_RC_ERROR;
}

/* Perform an AFI TUPE command:
 * - QUERY:             Returns VTSS_RC_OK if ready, VTSS_RC_INCOMPLETE if not (parms may be NULL)
 * - START_NONBLOCKING: Returns VTSS_RC_OK if TUPE started, VTSS_RC_ERR_PARM if parms invalid,
 *                      VTSS_RC_INCOMPLETE if not ready.
 *                      Note that while TUPE is running, TTI table must not be changed
 * - START_BLOCKING:    Returns VTSS_RC_OK when TUPE command done, VTSS_RC_ERR_PARM if parms invalid,
 *                      VTSS_RC_INCOMPLETE if not ready.
 * Note that the AFI TUPE values used in parms are used directly, so it is optional to use
 * fa_afi_tupe_alloc() / fa_afi_tupe_free(). This is to allow other allocation schemes for the AFI
 * TUPE values.
*/
vtss_rc fa_afi_tupe_cmd(vtss_state_t *vtss_state,
                        vtss_tupe_cmd_t cmd, vtss_afi_tupe_parms_t *parms)
{
    return VTSS_RC_OK;
}

vtss_rc fa_tupe_init(vtss_state_t *vtss_state, u8 tupe_linear_prot_bits)
{
    return VTSS_RC_OK;
}

/* - Test functions ------------------------------------------------- */

// test code for debug of this TUPE API
// fa_tupe_init() must have been called first
// warning: Will mess with the VLAN table!
#ifdef TUPE_TEST
vtss_rc fa_tupe_test(vtss_state_t *vtss_state)
{
    const u32 max_addr = VTSS_VIDS + VTSS_VSI_CNT + 1;
    u32  vlan_change[1 + max_addr / 32];
    u32  i, j, loop, set_port, clr_port, val, cnt;
    u64  pmask, exp_pmask, org_pmask;
    u8   use_comb;
    BOOL ok = TRUE;
    u16 mask;
    vtss_port_no_t    port_no;
    vtss_tupe_val_t   tupe_vals[TUPE_VALS_MAX], v;
    vtss_tupe_val_t   tupe_bits[TUPE_BITS_MAX];
    vtss_tupe_parms_t parms;

    for (loop = 0; loop < 10 && ok; ++loop) {
        // try to allocate all TUPE entries:
        for (i = 1; i < TUPE_VALS_MAX; ++i) {
            if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_VALUE, &tupe_vals[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to allocate TUPE_VALUE i=%u\n", i);
                ok = FALSE;
            }
        }
        // should not be able to alloc any more now:
        if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_VALUE, &v) == VTSS_RC_OK) {
            printf("*** ERROR: Could allocate extra TUPE_VALUE i=%u\n", i);
            ok = FALSE;
        }
        for (i = 0; i < TUPE_BITS_MAX; ++i) {
            if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_BITS, &tupe_bits[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to allocate TUPE_BITS i=%u\n", i);
                ok = FALSE;
            }
        }
        // should not be able to alloc any more now:
        if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_BITS, &v) == VTSS_RC_OK) {
            printf("*** ERROR: Could allocate extra TUPE_BITS i=%u\n", i);
            ok = FALSE;
        }
        // try to free all TUPE entries (random order):
        for (i = 1; i < TUPE_VALS_MAX; ++i) {
            do {
                j = rand() % TUPE_VALS_MAX;
            } while (j == 0 || tupe_vals[j] == 0xffffffff);
            if (fa_tupe_free(vtss_state, tupe_vals[j]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to free TUPE_VALUE 0x%08x (j=%u)\n", tupe_vals[j], j);
                ok = FALSE;
            }
            // check double free will fail:
            if (fa_tupe_free(vtss_state, tupe_vals[j]) == VTSS_RC_OK) {
                printf("*** ERROR: Could double free TUPE_VALUE 0x%08x (j=%u)\n", tupe_vals[j], j);
                ok = FALSE;
            }
            tupe_vals[j] = 0xffffffff;
        }
        for (i = 0; i < TUPE_BITS_MAX; ++i) {
            do {
                j = rand() % TUPE_BITS_MAX;
            } while (tupe_bits[j] == 0xffffffff);
            if (fa_tupe_free(vtss_state, tupe_bits[j]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to free TUPE_BITS 0x%08x (j=%u)\n", tupe_bits[j], j);
                ok = FALSE;
            }
            // check double free will fail:
            if (fa_tupe_free(vtss_state, tupe_bits[j]) == VTSS_RC_OK) {
                printf("*** ERROR: Could double free TUPE_BITS 0x%08x (j=%u)\n", tupe_bits[j], j);
                ok = FALSE;
            }
            tupe_bits[j] = 0xffffffff;
        }
        printf("Alloc/free loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // Query cmd should return ready:
    for (loop = 0; loop < 10 && ok; ++loop) {
        if (fa_tupe_cmd(vtss_state, VTSS_TUPE_CMD_QUERY, NULL) != VTSS_RC_OK) {
            printf("*** ERROR: QUERY cmd failed\n");
            ok = FALSE;
        }
        printf("QUERY cmd loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // try to allocate all TUPE entries:
    for (i = 1; i < TUPE_VALS_MAX; ++i) {
        if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_VALUE, &tupe_vals[i]) != VTSS_RC_OK) {
            printf("*** ERROR: Failed to allocate TUPE_VALUE i=%u\n", i);
            ok = FALSE;
        }
    }
    for (i = 0; i < TUPE_BITS_MAX; ++i) {
        if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_BITS, &tupe_bits[i]) != VTSS_RC_OK) {
            printf("*** ERROR: Failed to allocate TUPE_BITS i=%u\n", i);
            ok = FALSE;
        }
    }
    // Test VTSS_TUPE_CMD_START_BLOCKING:
    use_comb = (vtss_state->tupe.tupe_bits_bits + vtss_state->tupe.tupe_vals_bits) > TUPE_CTRL_MAX ? 1 : 0;
    for (loop = 0; loop < 10 && ok; ++loop) {
        memset(&parms, 0, sizeof(parms));
        if ((rand() % 1024) < 512) {
            j = rand() % TUPE_VALS_MAX;
            if (j == 0) {
                j = 1;
            }
            v = tupe_vals[j];
        } else {
            j = rand() % TUPE_BITS_MAX;
            v = tupe_bits[j];
        }
        do {
            parms.start_addr = rand() % max_addr;
            parms.end_addr   = rand() % max_addr;
        } while (parms.end_addr < parms.start_addr);
        parms.value      = v;
        pmask = 0;
        if (use_comb) {
            mask = parms.value >> TUPE_CTRL_MAX;
            if (mask) {
                // must use portmask bit, look through tupe_bits_bits+tupe_vals_bits-TUPE_CTRL_MAX unused ports:
                cnt = vtss_state->tupe.tupe_bits_bits - (TUPE_CTRL_MAX - vtss_state->tupe.tupe_vals_bits);
                for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORTS && cnt < vtss_state->tupe.tupe_bits_bits; port_no++) {
                    if (VTSS_CHIP_PORT(port_no) == CHIP_PORT_UNUSED) {
                        if (mask & 1) {
                            pmask |= vtss_fa_pmask(VTSS_CHIP_PORT(port_no));
                            break;
                        }
                        cnt++;
                        mask >>= 1;
                    }
                }
            }
        }
        do {
            set_port = rand() % VTSS_PORTS;
            clr_port = rand() % VTSS_PORTS;
        } while ((pmask & vtss_fa_pmask(VTSS_CHIP_PORT(set_port))) ||
                 (pmask & vtss_fa_pmask(VTSS_CHIP_PORT(clr_port))));
        parms.set_port_list[set_port] = TRUE;
        parms.clr_port_list[clr_port] = TRUE;
        if (set_port != clr_port) {
            pmask |= vtss_fa_port_mask(vtss_state, parms.clr_port_list);
        } else {
            pmask |= (rand() % 1024) < 512 ? 0 : vtss_fa_port_mask(vtss_state, parms.clr_port_list);
        }
        org_pmask = pmask;
        memset(vlan_change, 0, sizeof(vlan_change));
        for (i = 0; i < max_addr; ++i) {
            if ((rand() % 1024) < 700) {
                if (fa_tupe_vlan_set(vtss_state, i, v < TUPE_VALS_MAX ? VTSS_TUPE_TYPE_VALUE : VTSS_TUPE_TYPE_BITS, v) != VTSS_RC_OK) {
                    printf("*** ERROR: tupe_vlan_set failed, i=%u\n", i);
                    ok = FALSE;
                    break;
                }
                FA_WRX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, pmask);
                vlan_change[i / 32] |= (1 << (i % 32));
            } else {
                if (fa_tupe_vlan_clr(vtss_state, i) != VTSS_RC_OK) {
                    printf("*** ERROR: tupe_vlan_clr failed, i=%u\n", i);
                    ok = FALSE;
                }
                FA_WRX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, (u64)0);
            }
        }
        if (fa_tupe_cmd(vtss_state, VTSS_TUPE_CMD_START_BLOCKING, &parms) != VTSS_RC_OK) {
            printf("*** ERROR: START_BLOCKING cmd failed\n");
            ok = FALSE;
        }
        // check VLAN table entries:
        for (i = 0; i < max_addr; ++i) {
            if (i < parms.start_addr || i > parms.end_addr ||
                !(vlan_change[i / 32] & (1 << (i % 32)))) {
                // expect no change
                exp_pmask = 0;
                FA_RD(VTSS_ANA_L3_VLAN_TUPE_CTRL(i), &val);
                if (vlan_change[i / 32] & (1 << (i % 32))) {
                    if (val != (v & 0xffff)) {
                        printf("*** ERROR: VLAN table i=%u read TUPE_CTRL=0x%04x, expected 0x%04x\n", i, val, v & 0xffff);
                        ok = FALSE;
                    }
                    exp_pmask = org_pmask;
                } else if (val != 0) {
                    printf("*** ERROR: VLAN table i=%u read TUPE_CTRL=0x%04x, expected 0x%04x\n", i, val, 0);
                    ok = FALSE;
                }
                FA_RDX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, &pmask);
                if (pmask != exp_pmask) {
                    printf("*** ERROR: VLAN table i=%u read pmask=0x%08x%08x, expected 0x%08x%08x\n",
                           i, (u32)(pmask >> 32), (u32)(pmask), (u32)(exp_pmask >> 32), (u32)(exp_pmask));
                    ok = FALSE;
                }
            } else {
                // expect change in portmask
                FA_RD(VTSS_ANA_L3_VLAN_TUPE_CTRL(i), &val);
                if (val != (v & 0xffff)) {
                    printf("*** ERROR: VLAN table i=%u read TUPE_CTRL=0x%04x, expected 0x%04x\n", i, val, v & 0xffff);
                    ok = FALSE;
                }
                if (set_port != clr_port) {
                    exp_pmask = (org_pmask ^ vtss_fa_port_mask(vtss_state, parms.clr_port_list)) |
                        vtss_fa_port_mask(vtss_state, parms.set_port_list);
                } else {
                    exp_pmask = org_pmask ^ vtss_fa_port_mask(vtss_state, parms.set_port_list);
                }
                FA_RDX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, &pmask);
                if (pmask != exp_pmask) {
                    printf("*** ERROR: VLAN table i=%u read pmask=0x%08x%08x, expected 0x%08x%08x\n",
                           i, (u32)(pmask >> 32), (u32)(pmask), (u32)(exp_pmask >> 32), (u32)(exp_pmask));
                    ok = FALSE;
                }
            }
        }
        // clear VLAN table entries again:
        for (i = 0; i < max_addr; ++i) {
            if (fa_tupe_vlan_clr(vtss_state, i) != VTSS_RC_OK) {
                printf("*** ERROR: tupe_vlan_clr failed, i=%u\n", i);
                ok = FALSE;
            }
            FA_WRX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, (u64)0);
        }
        printf("START_BLOCKING cmd loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // Test VTSS_TUPE_CMD_START_NONBLOCKING:
    for (loop = 0; loop < 10 && ok; ++loop) {
        memset(&parms, 0, sizeof(parms));
        if ((rand() % 1024) < 512) {
            j = rand() % TUPE_VALS_MAX;
            if (j == 0) {
                j = 1;
            }
            v = tupe_vals[j];
        } else {
            j = rand() % TUPE_BITS_MAX;
            v = tupe_bits[j];
        }
        do {
            parms.start_addr = rand() % max_addr;
            parms.end_addr   = rand() % max_addr;
        } while (parms.end_addr < parms.start_addr);
        parms.value      = v;
        pmask = 0;
        if (use_comb) {
            mask = parms.value >> TUPE_CTRL_MAX;
            if (mask) {
                // must use portmask bit, look through tupe_bits_bits+tupe_vals_bits-TUPE_CTRL_MAX unused ports:
                cnt = vtss_state->tupe.tupe_bits_bits - (TUPE_CTRL_MAX - vtss_state->tupe.tupe_vals_bits);
                for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORTS && cnt < vtss_state->tupe.tupe_bits_bits; port_no++) {
                    if (VTSS_CHIP_PORT(port_no) == CHIP_PORT_UNUSED) {
                        if (mask & 1) {
                            pmask |= vtss_fa_pmask(VTSS_CHIP_PORT(port_no));
                            break;
                        }
                        cnt++;
                        mask >>= 1;
                    }
                }
            }
        }
        do {
            set_port = rand() % VTSS_PORTS;
            clr_port = rand() % VTSS_PORTS;
        } while ((pmask & vtss_fa_pmask(VTSS_CHIP_PORT(set_port))) ||
                 (pmask & vtss_fa_pmask(VTSS_CHIP_PORT(clr_port))));
        parms.set_port_list[set_port] = TRUE;
        parms.clr_port_list[clr_port] = TRUE;
        if (set_port != clr_port) {
            pmask |= vtss_fa_port_mask(vtss_state, parms.clr_port_list);
        } else {
            pmask |= (rand() % 1024) < 512 ? 0 : vtss_fa_port_mask(vtss_state, parms.clr_port_list);
        }
        org_pmask = pmask;
        memset(vlan_change, 0, sizeof(vlan_change));
        for (i = 0; i < max_addr; ++i) {
            if ((rand() % 1024) < 700) {
                if (fa_tupe_vlan_set(vtss_state, i, v < TUPE_VALS_MAX ? VTSS_TUPE_TYPE_VALUE : VTSS_TUPE_TYPE_BITS, v) != VTSS_RC_OK) {
                    printf("*** ERROR: tupe_vlan_set failed, i=%u\n", i);
                    ok = FALSE;
                    break;
                }
                FA_WRX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, pmask);
                vlan_change[i / 32] |= (1 << (i % 32));
            } else {
                if (fa_tupe_vlan_clr(vtss_state, i) != VTSS_RC_OK) {
                    printf("*** ERROR: tupe_vlan_clr failed, i=%u\n", i);
                    ok = FALSE;
                }
                FA_WRX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, (u64)0);
            }
        }
        if (fa_tupe_cmd(vtss_state, VTSS_TUPE_CMD_START_NONBLOCKING, &parms) != VTSS_RC_OK) {
            printf("*** ERROR: START_NONBLOCKING cmd failed\n");
            ok = FALSE;
        }
        while (fa_tupe_cmd(vtss_state, VTSS_TUPE_CMD_QUERY, NULL) != VTSS_RC_OK) {
            printf("Wait for cmd to complete...\n");
        }
        // check VLAN table entries:
        for (i = 0; i < max_addr; ++i) {
            if (i < parms.start_addr || i > parms.end_addr ||
                !(vlan_change[i / 32] & (1 << (i % 32)))) {
                // expect no change
                exp_pmask = 0;
                FA_RD(VTSS_ANA_L3_VLAN_TUPE_CTRL(i), &val);
                if (vlan_change[i / 32] & (1 << (i % 32))) {
                    if (val != (v & 0xffff)) {
                        printf("*** ERROR: VLAN table i=%u read TUPE_CTRL=0x%04x, expected 0x%04x\n", i, val, v & 0xffff);
                        ok = FALSE;
                    }
                    exp_pmask = org_pmask;
                } else if (val != 0) {
                    printf("*** ERROR: VLAN table i=%u read TUPE_CTRL=0x%04x, expected 0x%04x\n", i, val, 0);
                    ok = FALSE;
                }
                FA_RDX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, &pmask);
                if (pmask != exp_pmask) {
                    printf("*** ERROR: VLAN table i=%u read pmask=0x%08x%08x, expected 0x%08x%08x\n",
                           i, (u32)(pmask >> 32), (u32)(pmask), (u32)(exp_pmask >> 32), (u32)(exp_pmask));
                    ok = FALSE;
                }
            } else {
                // expect change in portmask
                FA_RD(VTSS_ANA_L3_VLAN_TUPE_CTRL(i), &val);
                if (val != (v & 0xffff)) {
                    printf("*** ERROR: VLAN table i=%u read TUPE_CTRL=0x%04x, expected 0x%04x\n", i, val, v & 0xffff);
                    ok = FALSE;
                }
                if (set_port != clr_port) {
                    exp_pmask = (org_pmask ^ vtss_fa_port_mask(vtss_state, parms.clr_port_list)) |
                        vtss_fa_port_mask(vtss_state, parms.set_port_list);
                } else {
                    exp_pmask = org_pmask ^ vtss_fa_port_mask(vtss_state, parms.set_port_list);
                }
                FA_RDX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, &pmask);
                if (pmask != exp_pmask) {
                    printf("*** ERROR: VLAN table i=%u read pmask=0x%08x%08x, expected 0x%08x%08x\n",
                           i, (u32)(pmask >> 32), (u32)(pmask), (u32)(exp_pmask >> 32), (u32)(exp_pmask));
                    ok = FALSE;
                }
            }
        }
        // clear VLAN table entries again:
        for (i = 0; i < max_addr; ++i) {
            if (fa_tupe_vlan_clr(vtss_state, i) != VTSS_RC_OK) {
                printf("*** ERROR: tupe_vlan_clr failed, i=%u\n", i);
                ok = FALSE;
            }
            FA_WRX_PMASK(VTSS_ANA_L3_VLAN_VLAN_MASK_CFG, i, (u64)0);
        }
        printf("START_NONBLOCKING cmd loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // try to free all TUPE entries:
    for (i = 1; i < TUPE_VALS_MAX; ++i) {
        if (fa_tupe_free(vtss_state, tupe_vals[i]) != VTSS_RC_OK) {
            printf("*** ERROR: Failed to free TUPE_VALUE 0x%08x (i=%u)\n", tupe_vals[i], i);
            ok = FALSE;
        }
    }
    for (i = 0; i < TUPE_BITS_MAX; ++i) {
        if (fa_tupe_free(vtss_state, tupe_bits[i]) != VTSS_RC_OK) {
            printf("*** ERROR: Failed to free TUPE_BITS 0x%08x (i=%u)\n", tupe_bits[i], i);
            ok = FALSE;
        }
    }
    printf("Test %s\n", ok ? "PASSED" : "FAILED");
    return ok ? VTSS_RC_OK : VTSS_RC_ERROR;
}

vtss_rc fa_tupe_realloc_test(vtss_state_t *vtss_state)
{
    const u32            max_addr = VTSS_VIDS + VTSS_VSI_CNT + 1;
    u32                  i, j, loop, vals_next, bits_next;
    u32                  new_tupe_bits_max, new_tupe_vals_max;
    u16                  bits_free;
    u8                   tupe_linear_prot_bits;
    BOOL                 ok = TRUE;
    u32                  vlan_vals[max_addr];
    vtss_tupe_val_t      tupe_vals[1 << TUPE_CTRL_MAX];
    vtss_tupe_val_t      from_vals[1 << TUPE_CTRL_MAX];
    vtss_tupe_val_t      tupe_bits[TUPE_CTRL_MAX];
    vtss_tupe_val_t      tupe_val;
    vtss_tupe_val_type_t tupe_type;

    for (loop = 0; loop < 10 && ok; ++loop) {
        printf("loop %u/10 (%u + %u bits)...", loop + 1, vtss_state->tupe.tupe_bits_bits, vtss_state->tupe.tupe_vals_bits);
        // allocate half the TUPE entries (randomize)
        memset(tupe_vals, 0, sizeof(tupe_vals));
        memset(from_vals, 0, sizeof(tupe_vals));
        memset(tupe_bits, 0, sizeof(tupe_bits));
        vals_next = 0;
        bits_next = 0;
        for (i = 0; i < ((TUPE_VALS_MAX / 2) - 1); ++i) {
            vtss_state->tupe.tupe_vals_next = rand() % TUPE_VALS_MAX; // trick to alloc random values
            if (vtss_state->tupe.tupe_vals_next == 0) {
                vtss_state->tupe.tupe_vals_next = 1; // 0 is reserved
            }
            if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_VALUE, &tupe_vals[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to allocate TUPE_VALUE i=%u\n", i);
                ok = FALSE;
            }
            vals_next++;
        }
        for (i = 0; i < (TUPE_BITS_MAX / 2); ++i) {
            if (fa_tupe_alloc(vtss_state, VTSS_TUPE_TYPE_BITS, &tupe_bits[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to allocate TUPE_BITS i=%u\n", i);
                ok = FALSE;
            }
            bits_next++;
        }
        // configure random VLAN/VSI entries using random TUPE entries from above
        memset(vlan_vals, 0, sizeof(vlan_vals));
        for (i = 0; i < max_addr; ++i) {
            if ((rand() % 1024) < 512) {
                tupe_type = VTSS_TUPE_TYPE_VALUE;
                tupe_val  = tupe_vals[rand() % vals_next];
            } else {
                tupe_type = VTSS_TUPE_TYPE_BITS;
                tupe_val  = tupe_bits[rand() % bits_next];
            }
            if ((rand() % 1024) < 700) {
                if (fa_tupe_vlan_set(vtss_state, i, tupe_type, tupe_val) != VTSS_RC_OK) {
                    printf("*** ERROR: tupe_vlan_set failed, i=%u\n", i);
                    ok = FALSE;
                }
                vlan_vals[i] = tupe_val;
            } else {
                if (fa_tupe_vlan_clr(vtss_state, i) != VTSS_RC_OK) {
                    printf("*** ERROR: tupe_vlan_clr failed, i=%u\n", i);
                    ok = FALSE;
                }
            }
        }
        // try to re-initialize using different (but possible) TUPE bit split
        // avoid 1-bit (can't get half the entries...)
        if (vtss_state->tupe.tupe_vals_bits == 2) {
            tupe_linear_prot_bits = vtss_state->tupe.tupe_vals_bits + 1;
        } else if (vtss_state->tupe.tupe_vals_bits == 14) {
            tupe_linear_prot_bits = vtss_state->tupe.tupe_vals_bits - 1;
        } else {
            tupe_linear_prot_bits = ((rand() % 1024) < 512) ? vtss_state->tupe.tupe_vals_bits + 1: vtss_state->tupe.tupe_vals_bits - 1;
        }
        new_tupe_vals_max = (1 << tupe_linear_prot_bits) & ~1;
        new_tupe_bits_max = TUPE_CTRL_MAX + TUPE_PORTMASK_MAX - tupe_linear_prot_bits;
        // update vlan_vals:
        vals_next = 1;
        bits_free = 0;
        for (i = 0; i < new_tupe_bits_max; ++i) {
            bits_free |= 1 << i;
        }
        for (i = 0; i < max_addr; ++i) {
            if (vlan_vals[i] == 0) {
                continue;
            }
            if (fa_tupe_vlan_get(vtss_state, i, &tupe_type, &tupe_val) != VTSS_RC_OK) {
                printf("*** ERROR: vlan_get failed, i=%u\n", i);
                ok = FALSE;
            }
            if (tupe_val != vlan_vals[i]) {
                printf("*** ERROR: tupe_val mismatch 0x%04x != 0x%04x, i=%u\n", tupe_val, vlan_vals[i], i);
                ok = FALSE;
            }
            if (tupe_type == VTSS_TUPE_TYPE_VALUE) {
                for (j = 1; j < vals_next; ++j) {
                    if (from_vals[j] == tupe_val) {
                        break;
                    }
                }
                vlan_vals[i] = j;
                if (j >= vals_next) {
                    from_vals[j] = tupe_val;
                    vals_next++;
                }
            } else if (tupe_type == VTSS_TUPE_TYPE_BITS) {
                for (j = 0; j < TUPE_BITS_MAX; ++j) {
                    if (from_vals[TUPE_VALS_MAX + j] == tupe_val) {
                        break;
                    }
                }
                if (j >= TUPE_BITS_MAX) {
                    for (j = 0; j < TUPE_BITS_MAX; ++j) {
                        if (bits_free & (1 << j)) {
                            bits_free &= ~(1 << j);
                            from_vals[TUPE_VALS_MAX + j] = tupe_val;
                            break;
                        }
                    }
                }
                vlan_vals[i] = (1 << j) * new_tupe_vals_max;
            }
            if ((vals_next - 1) > ((TUPE_VALS_MAX / 2) - 1) ||
                bits_next > (TUPE_BITS_MAX / 2)) {
                printf("*** ERROR: vals_next %u > %u || bits_next %u > %u\n", vals_next, ((TUPE_VALS_MAX / 2) - 1), bits_next, (TUPE_BITS_MAX / 2));
                ok = FALSE;
                break;
            }
        }
        // update tupe_vals and tupe_bits:
        vals_next = 0;
        bits_next = 0;
        memset(tupe_vals, 0, sizeof(tupe_vals));
        memset(tupe_bits, 0, sizeof(tupe_bits));
        for (i = 0; i < max_addr; ++i) {
            if (vlan_vals[i] == 0) {
                continue;
            }
            if (vlan_vals[i] < new_tupe_vals_max) {
                for (j = 0; j < vals_next; ++j) {
                    if (tupe_vals[j] == vlan_vals[i]) {
                        break; // already updated
                    }
                }
                if (j >= vals_next) {
                    tupe_vals[j] = vlan_vals[i];
                    vals_next++;
                }
            } else {
                for (j = 0; j < bits_next; ++j) {
                    if (tupe_bits[j] == vlan_vals[i]) {
                        break; // already updated
                    }
                }
                if (j >= bits_next) {
                    tupe_bits[j] = vlan_vals[i];
                    bits_next++;
                }
            }
            if (vals_next > ((TUPE_VALS_MAX / 2) - 1) ||
                bits_next > (TUPE_BITS_MAX / 2)) {
                printf("*** ERROR: vals_next %u > %u || bits_next %u > %u\n", vals_next, ((TUPE_VALS_MAX / 2) - 1), bits_next, (TUPE_BITS_MAX / 2));
                ok = FALSE;
                break;
            }
        }
        // perform re-init
        if (fa_tupe_init(vtss_state, tupe_linear_prot_bits) != VTSS_RC_OK) {
            printf("*** ERROR: fa_tupe_init(%u -> %u) failed\n", vtss_state->tupe.tupe_vals_bits, tupe_linear_prot_bits);
            ok = FALSE;
            break;
        }
        // check VLAN/VSI table was updated as expected:
        for (i = 0; i < max_addr; ++i) {
            tupe_val = 0;
            if (fa_tupe_vlan_get(vtss_state, i, &tupe_type, &tupe_val) != VTSS_RC_OK &&
                vlan_vals[i] != 0) {
                printf("*** ERROR: vlan_get failed, vlan_vals[%u]=0x%08x\n", i, vlan_vals[i]);
                ok = FALSE;
            } else if (tupe_val != vlan_vals[i]) {
                printf("*** ERROR: tupe_val mismatch 0x%04x != 0x%04x, i=%u\n", tupe_val, vlan_vals[i], i);
                ok = FALSE;
            }
        }
        // clear VLAN table entries again:
        for (i = 0; i < max_addr; ++i) {
            if (fa_tupe_vlan_clr(vtss_state, i) != VTSS_RC_OK) {
                printf("*** ERROR: tupe_vlan_clr failed, i=%u\n", i);
                ok = FALSE;
            }
        }
        // free the TUPE entries again
        for (i = 0; i < vals_next; ++i) {
            if (fa_tupe_free(vtss_state, tupe_vals[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to free TUPE_VALUE i=%u\n", i);
                ok = FALSE;
            }
        }
        for (i = 0; i < bits_next; ++i) {
            if (fa_tupe_free(vtss_state, tupe_bits[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to free TUPE_BITS i=%u\n", i);
                ok = FALSE;
            }
        }
        vals_next = 0;
        bits_next = 0;
        printf("%s\n", ok ? "OK" : "FAILED!");
    }
    printf("Test %s\n", ok ? "PASSED" : "FAILED");
    return ok ? VTSS_RC_OK : VTSS_RC_ERROR;
}

vtss_rc fa_afi_tupe_test(vtss_state_t *vtss_state)
{
    const u32 max_addr = VTSS_AFI_SLOW_INJ_CNT;
    u32 tti_tbl[max_addr][3];
    u32  i, j, loop, v, vv;
    BOOL ok = TRUE;
    vtss_afi_tupe_val_t tupe_vals[AFI_TUPE_VALS_MAX], tupe_val;
    vtss_afi_tupe_parms_t parms;

    for (loop = 0; loop < 10 && ok; ++loop) {
        // try to allocate all AFI TUPE entries:
        for (i = 1; i < AFI_TUPE_VALS_MAX; ++i) {
            if (fa_afi_tupe_alloc(vtss_state, &tupe_vals[i]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to allocate AFI_TUPE_VALUE i=%u\n", i);
                ok = FALSE;
            }
        }
        // should not be able to alloc any more now:
        if (fa_afi_tupe_alloc(vtss_state, &tupe_val) == VTSS_RC_OK) {
            printf("*** ERROR: Could allocate extra AFI_TUPE_VALUE i=%u\n", i);
            ok = FALSE;
        }
        // try to free all AFI TUPE entries (random order):
        for (i = 1; i < AFI_TUPE_VALS_MAX; ++i) {
            do {
                j = rand() % AFI_TUPE_VALS_MAX;
            } while (j == 0 || tupe_vals[j] == 0);
            if (fa_afi_tupe_free(vtss_state, tupe_vals[j]) != VTSS_RC_OK) {
                printf("*** ERROR: Failed to free AFI_TUPE_VALUE 0x%08x (j=%u)\n", tupe_vals[j], j);
                ok = FALSE;
            }
            // check double free will fail:
            if (fa_afi_tupe_free(vtss_state, tupe_vals[j]) == VTSS_RC_OK) {
                printf("*** ERROR: Could double free AFI_TUPE_VALUE 0x%08x (j=%u)\n", tupe_vals[j], j);
                ok = FALSE;
            }
            tupe_vals[j] = 0;
        }
        printf("Alloc/free loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // Query cmd should return ready:
    for (loop = 0; loop < 10 && ok; ++loop) {
        if (fa_afi_tupe_cmd(vtss_state, VTSS_TUPE_CMD_QUERY, NULL) != VTSS_RC_OK) {
            printf("*** ERROR: QUERY cmd failed\n");
            ok = FALSE;
        }
        printf("QUERY cmd loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // try to allocate all AFI TUPE entries:
    for (i = 1; i < AFI_TUPE_VALS_MAX; ++i) {
        if (fa_afi_tupe_alloc(vtss_state, &tupe_vals[i]) != VTSS_RC_OK) {
            printf("*** ERROR: Failed to allocate AFI_TUPE_VALUE i=%u\n", i);
            ok = FALSE;
        }
    }
    // Test VTSS_TUPE_CMD_START_BLOCKING:
    for (loop = 0; loop < 10 && ok; ++loop) {
        memset(&parms, 0, sizeof(parms));
        do {
            parms.start_addr = rand() % max_addr;
            parms.end_addr   = rand() % max_addr;
        } while (parms.end_addr < parms.start_addr);
        if ((rand() % 1024) < 512) {
            // using AFI TUPE value in match
            j = rand() % AFI_TUPE_VALS_MAX;
            if (j == 0) {
                j = 1;
            }
            if ((rand() % 1024) < 512) {
                parms.match.value[0] = tupe_vals[j];
                parms.match.value[1] = 0;
            } else {
                parms.match.value[0] = 0;
                parms.match.value[1] = tupe_vals[j];
            }
            parms.match.mask     = 0xff;
        } else {
            // not using AFI TUPE value in match
            parms.match.value[0] = tupe_vals[j];
            parms.match.value[1] = tupe_vals[j];
            parms.match.mask     = 0;
        }
        parms.match.qu_num        = rand() % 2048;
        parms.match.qu_num_en     = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.match.port_no       = rand() % VTSS_PORTS;
        parms.match.port_no_en    = (rand() % 1024) < 512 ? TRUE : FALSE;

        parms.update.qu_num       = rand() % 2048;
        parms.update.qu_num_en    = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.update.port_no      = rand() % VTSS_PORTS;
        parms.update.port_no_en   = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.update.timer_ena    = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.update.timer_ena_en = (rand() % 1024) < 512 ? TRUE : FALSE;

        for (i = 0; i < max_addr; ++i) {
            if ((rand() % 1024) < 512) {
                // make sure this entry does match
                v = VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(parms.match.qu_num_en ? parms.match.qu_num : rand() % 2048) |
                    VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(VTSS_CHIP_PORT(parms.match.port_no_en ? parms.match.port_no : rand() % VTSS_PORTS));
                vv = VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(parms.update.qu_num_en ? parms.update.qu_num : VTSS_X_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(v)) |
                    VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(parms.update.port_no_en ? VTSS_CHIP_PORT(parms.update.port_no) : VTSS_X_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(v));
                if (i < parms.start_addr || i > parms.end_addr || parms.match.mask == 0) {
                    tti_tbl[i][0] = v;  // expect no update
                } else {
                    tti_tbl[i][0] = vv; // expect update
                }
                FA_WR(VTSS_AFI_TTI_TBL_TTI_PORT_QU(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA((rand() % 1024) < 512 ? 1 : 0);
                vv = VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(parms.update.timer_ena_en ? parms.update.timer_ena : VTSS_X_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(v));
                if (i < parms.start_addr || i > parms.end_addr || parms.match.mask == 0) {
                    tti_tbl[i][1] = v;  // expect no update
                } else {
                    tti_tbl[i][1] = vv; // expect update
                }
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TIMER(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TUPE_CTRL_TUPE_CTRL(parms.match.mask ? parms.match.value[0] : rand() & 0xff);
                tti_tbl[i][2] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TUPE_CTRL(i), v);
            } else {
                // make sure this entry does not match
                v = VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(parms.match.qu_num_en ? parms.match.qu_num + 1 : parms.match.qu_num) |
                    VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(parms.match.port_no_en ? 1 + VTSS_CHIP_PORT(parms.match.port_no) : VTSS_CHIP_PORT(parms.match.port_no));
                tti_tbl[i][0] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_PORT_QU(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA((rand() % 1024) < 512 ? 1 : 0);
                tti_tbl[i][1] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TIMER(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TUPE_CTRL_TUPE_CTRL(parms.match.mask ? parms.match.value[0] + 1 : parms.match.value[0]);
                tti_tbl[i][2] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TUPE_CTRL(i), v);
            }
        }
        if (fa_afi_tupe_cmd(vtss_state, VTSS_TUPE_CMD_START_BLOCKING, &parms) != VTSS_RC_OK) {
            printf("*** ERROR: START_BLOCKING cmd failed\n");
            ok = FALSE;
        }
        // check TTI table entries:
        for (i = 0; i < max_addr; ++i) {
            FA_RD(VTSS_AFI_TTI_TBL_TTI_PORT_QU(i), &v);
            if (v != tti_tbl[i][0]) {
                printf("*** ERROR: i=%u v=0x%08x != tti_tbl[0]=0x%08x\n", i, v, tti_tbl[i][0]);
                ok = FALSE;
            }
            FA_RD(VTSS_AFI_TTI_TBL_TTI_TIMER(i), &v);
            if (v != tti_tbl[i][1]) {
                printf("*** ERROR: i=%u v=0x%08x != tti_tbl[1]=0x%08x\n", i, v, tti_tbl[i][1]);
                ok = FALSE;
            }
            FA_RD(VTSS_AFI_TTI_TBL_TTI_TUPE_CTRL(i), &v);
            if (v != tti_tbl[i][2]) {
                printf("*** ERROR: i=%u v=0x%08x != tti_tbl[2]=0x%08x\n", i, v, tti_tbl[i][2]);
                ok = FALSE;
            }
        }
        printf("START_BLOCKING cmd loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // Test VTSS_TUPE_CMD_START_NONBLOCKING:
    for (loop = 0; loop < 10 && ok; ++loop) {
        memset(&parms, 0, sizeof(parms));
        do {
            parms.start_addr = rand() % max_addr;
            parms.end_addr   = rand() % max_addr;
        } while (parms.end_addr < parms.start_addr);
        if ((rand() % 1024) < 512) {
            // using AFI TUPE value in match
            j = rand() % AFI_TUPE_VALS_MAX;
            if (j == 0) {
                j = 1;
            }
            if ((rand() % 1024) < 512) {
                parms.match.value[0] = tupe_vals[j];
                parms.match.value[1] = 0;
            } else {
                parms.match.value[0] = 0;
                parms.match.value[1] = tupe_vals[j];
            }
            parms.match.mask     = 0xff;
        } else {
            // not using AFI TUPE value in match
            parms.match.value[0] = tupe_vals[j];
            parms.match.value[1] = tupe_vals[j];
            parms.match.mask     = 0;
        }
        parms.match.qu_num        = rand() % 2048;
        parms.match.qu_num_en     = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.match.port_no       = rand() % VTSS_PORTS;
        parms.match.port_no_en    = (rand() % 1024) < 512 ? TRUE : FALSE;

        parms.update.qu_num       = rand() % 2048;
        parms.update.qu_num_en    = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.update.port_no      = rand() % VTSS_PORTS;
        parms.update.port_no_en   = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.update.timer_ena    = (rand() % 1024) < 512 ? TRUE : FALSE;
        parms.update.timer_ena_en = (rand() % 1024) < 512 ? TRUE : FALSE;

        for (i = 0; i < max_addr; ++i) {
            if ((rand() % 1024) < 512) {
                // make sure this entry does match
                v = VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(parms.match.qu_num_en ? parms.match.qu_num : rand() % 2048) |
                    VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(VTSS_CHIP_PORT(parms.match.port_no_en ? parms.match.port_no : rand() % VTSS_PORTS));
                vv = VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(parms.update.qu_num_en ? parms.update.qu_num : VTSS_X_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(v)) |
                    VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(parms.update.port_no_en ? VTSS_CHIP_PORT(parms.update.port_no) : VTSS_X_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(v));
                if (i < parms.start_addr || i > parms.end_addr || parms.match.mask == 0) {
                    tti_tbl[i][0] = v;  // expect no update
                } else {
                    tti_tbl[i][0] = vv; // expect update
                }
                FA_WR(VTSS_AFI_TTI_TBL_TTI_PORT_QU(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA((rand() % 1024) < 512 ? 1 : 0);
                vv = VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(parms.update.timer_ena_en ? parms.update.timer_ena : VTSS_X_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(v));
                if (i < parms.start_addr || i > parms.end_addr || parms.match.mask == 0) {
                    tti_tbl[i][1] = v;  // expect no update
                } else {
                    tti_tbl[i][1] = vv; // expect update
                }
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TIMER(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TUPE_CTRL_TUPE_CTRL(parms.match.mask ? parms.match.value[0] : rand() & 0xff);
                tti_tbl[i][2] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TUPE_CTRL(i), v);
            } else {
                // make sure this entry does not match
                v = VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(parms.match.qu_num_en ? parms.match.qu_num + 1 : parms.match.qu_num) |
                    VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(parms.match.port_no_en ? 1 + VTSS_CHIP_PORT(parms.match.port_no) : VTSS_CHIP_PORT(parms.match.port_no));
                tti_tbl[i][0] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_PORT_QU(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA((rand() % 1024) < 512 ? 1 : 0);
                tti_tbl[i][1] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TIMER(i), v);
                v = VTSS_F_AFI_TTI_TBL_TTI_TUPE_CTRL_TUPE_CTRL(parms.match.mask ? parms.match.value[0] + 1 : parms.match.value[0]);
                tti_tbl[i][2] = v;
                FA_WR(VTSS_AFI_TTI_TBL_TTI_TUPE_CTRL(i), v);
            }
        }
        if (fa_afi_tupe_cmd(vtss_state, VTSS_TUPE_CMD_START_NONBLOCKING, &parms) != VTSS_RC_OK) {
            printf("*** ERROR: START_NONBLOCKING cmd failed\n");
            ok = FALSE;
        }
        while (fa_tupe_cmd(vtss_state, VTSS_TUPE_CMD_QUERY, NULL) != VTSS_RC_OK) {
            printf("Wait for cmd to complete...\n");
        }
        // check TTI table entries:
        for (i = 0; i < max_addr; ++i) {
            FA_RD(VTSS_AFI_TTI_TBL_TTI_PORT_QU(i), &v);
            if (v != tti_tbl[i][0]) {
                printf("*** ERROR: i=%u v=0x%08x != tti_tbl[0]=0x%08x\n", i, v, tti_tbl[i][0]);
                ok = FALSE;
            }
            FA_RD(VTSS_AFI_TTI_TBL_TTI_TIMER(i), &v);
            if (v != tti_tbl[i][1]) {
                printf("*** ERROR: i=%u v=0x%08x != tti_tbl[1]=0x%08x\n", i, v, tti_tbl[i][1]);
                ok = FALSE;
            }
            FA_RD(VTSS_AFI_TTI_TBL_TTI_TUPE_CTRL(i), &v);
            if (v != tti_tbl[i][2]) {
                printf("*** ERROR: i=%u v=0x%08x != tti_tbl[2]=0x%08x\n", i, v, tti_tbl[i][2]);
                ok = FALSE;
            }
        }
        printf("START_NONBLOCKING cmd loop %u/10 %s\n", loop + 1, ok ? "ok" : "FAILED!");
    }
    // try to free all AFI TUPE entries:
    for (i = 1; i < AFI_TUPE_VALS_MAX; ++i) {
        if (fa_afi_tupe_free(vtss_state, tupe_vals[i]) != VTSS_RC_OK) {
            printf("*** ERROR: Failed to free AFI_TUPE_VALUE 0x%08x (i=%u)\n", tupe_vals[i], i);
            ok = FALSE;
        }
    }
    printf("Test %s\n", ok ? "PASSED" : "FAILED");
    return ok ? VTSS_RC_OK : VTSS_RC_ERROR;
}
#endif /* TUPE_TEST */

#endif /* VTSS_FEATURE_HW_PROT */
#endif /* VTSS_ARCH_FA */

/*****************************************************************************/
//
// End of file
//
//****************************************************************************/
