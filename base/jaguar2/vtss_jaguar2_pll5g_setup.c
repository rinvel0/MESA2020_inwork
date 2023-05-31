// Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
// SPDX-License-Identifier: MIT

#ifndef _VTSS_JAGUAR2__API_PLL5G_UTE
#define _VTSS_JAGUAR2__API_PLL5G_UTE


/* Generation Tag is  UTE_release_vts_ute_tcllib_20180312_trunk_bjo */

/* ================================================================= *
 *               Note: This code is autogenerated from the
 *                     Universal Tcl Environment (UTE) which provides
 *                     consistent setup functions for
 *       ^             - Verification
 *      / \            - Validation
 *     /   \           - Test Pattern Generation and
 *    /  |  \          - Software
 *   /   !   \         It should not be modified manually.
 *  /_________\        In case there is a need for modifications,
 *                     please contact
 *                       Patrick Urban <patrick.urban@microsemi.com> or
 *                       Alexander Koch <alexander.koch@microsemi.com> or
 *                       Mark Venneboerger <mark.venneboerger@microsemi.com>
 *                     Please use Bugzilla for reporting issues or requesting enhancements:
 *                     Bugzilla: Tools->Victoria
 *                     http://projissuetracker/bugzilla/enter_bug.cgi?product=Victoria
 * ================================================================= */

#include <vtss/api/options.h>  // To get the ARCH define
#if defined(VTSS_ARCH_JAGUAR_2_B)
#include "vtss_jaguar2_inc.h"


static vtss_rc  jaguar2_pll5g_register_cfg(vtss_state_t *vtss_state, vtss_pll5g_setup_struct_t *const res_struct, const u8 inst) {

    vtss_rc rc = VTSS_RC_OK;
    u32 value;
if( res_struct->unlock[0] == 1 ) {
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG2(inst),
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG2_ENA_GAIN_TEST(res_struct->pll5g_cfg2__ena_gain_test[0]) |
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG2_GAIN_TEST(res_struct->pll5g_cfg2__gain_test[0]),
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG2_ENA_GAIN_TEST |
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG2_GAIN_TEST);

}  else {
if( res_struct->lock[0] == 1 ) {
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG2(inst),
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG2_ENA_GAIN_TEST(res_struct->pll5g_cfg2__ena_gain_test[0]),
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG2_ENA_GAIN_TEST);

}  else {
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG4(inst),
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG4_IB_CTRL(res_struct->pll5g_cfg4__ib_ctrl[0]),
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG4_IB_CTRL);

    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG0(inst),
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG0_ENA_VCO_CONTRH(res_struct->pll5g_cfg0__ena_vco_contrh[0]) |
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG0_LOOP_BW_RES(res_struct->pll5g_cfg0__loop_bw_res[0]) |
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG0_SELBGV820(res_struct->pll5g_cfg0__selbgv820[0]),
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG0_ENA_VCO_CONTRH |
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG0_LOOP_BW_RES |
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG0_SELBGV820);

    u32 i_var;
    for (i_var=0; i_var<=9; i_var++) {
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG2(inst),
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG2_DISABLE_FSM(res_struct->pll5g_cfg2__disable_fsm[0]),
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG2_DISABLE_FSM);

    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG2(inst),
        VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG2_DISABLE_FSM(res_struct->pll5g_cfg2__disable_fsm[1]),
        VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG2_DISABLE_FSM);

    VTSS_MSLEEP(10);

    JR2_RD(VTSS_HSIO_PLL5G_STATUS_PLL5G_STATUS1(inst), &value);
    value = VTSS_X_HSIO_PLL5G_STATUS_PLL5G_STATUS1_GAIN_STAT(value);


    if( value > 2 && value < 0xa) {
    break;
    } 

    }
    if( value <= 2 || value >= 0xa) {
    VTSS_E("Failed to initialize LCPLL as the gain was not within limits");
    printf(" jr2 LCPLL value = %x _", value);
    } 

} 
} 
    if( res_struct->unlock[0] == 0 ) {
    VTSS_MSLEEP(5);

    } 


  return rc;
}

vtss_rc vtss_jaguar2_pll5g_setup(vtss_state_t *vtss_state, const vtss_pll5g_setup_args_t config, const u8 inst) {
    vtss_pll5g_setup_struct_t calc_results;
    vtss_rc ret_val;
    VTSS_D("This function is generated with UTE based on TAG:  UTE_release_vts_ute_tcllib_20180312_trunk_bjo");
    ret_val = vtss_calc_pll5g_setup(config, &calc_results);
    if(ret_val == VTSS_RC_OK) {
        ret_val |= jaguar2_pll5g_register_cfg(vtss_state, &calc_results, inst);
    }
    return ret_val;
}


#endif
#endif
