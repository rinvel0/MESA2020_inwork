// Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
// SPDX-License-Identifier: MIT

#include "vtss_api.h"
#include "../../ail/vtss_state.h"
#include "../../ail/vtss_common.h"
#include "vtss_phy_10g.h"
#include "../common/vtss_phy_common.h"
#include "../../ail/vtss_sd10g65_procs.h"
#include "vtss_venice_f2df_sd10g65_setup.h"
#include "../../ail/vtss_sd10g65_apc_procs.h"
#include "vtss_venice_c_sd10g65_setup.h"
#include "./chips/venice/vtss_venice_regs.h"
#include "vtss_venice_f2df_sd10g65_setup.h"
ioreg_blk io_var;
#if defined(VTSS_IOREG)
#undef VTSS_IOREG
#endif

#define VTSS_IOREG(dev, is32, off)  _ioreg(&io_var, (dev), (is32), (off))
#if defined(VTSS_IOREG_IX)
#undef VTSS_IOREG_IX
#endif
//rev C registers which were not generated by the script
/**
 * \brief SD10G65 TX Synthesizer Configuration register 3
 *
 * \details
 * Configuration register 3 for SD10G65 TX SYNTH.
 *
 * Register: \a VENICE_DEV1_32:SD10G65_TX_SYNTH:SD10G65_TX_SYNTH_CFG3
 */
#define VTSS_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG3  VTSS_IOREG(0x01, 1, 0xf152)
#define VTSS_F_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG3_SYNTH_FREQM_0(x)  VTSS_ENCODE_BITFIELD(x,0,31)
#define VTSS_M_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG3_SYNTH_FREQM_0  VTSS_ENCODE_BITMASK(0,31)
#define VTSS_X_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG3_SYNTH_FREQM_0(x)  VTSS_EXTRACT_BITFIELD(x,0,31)

/**
 * \brief SD10G65 TX Synthesizer Configuration register 4
 *
 * \details
 * Configuration register 4 for SD10G65 TX SYNTH.
 *
 * Register: \a VENICE_DEV1_32:SD10G65_TX_SYNTH:SD10G65_TX_SYNTH_CFG4
 */
#define VTSS_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG4  VTSS_IOREG(0x01, 1, 0xf153)
#define VTSS_F_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG4_SYNTH_FREQN_0(x)  VTSS_ENCODE_BITFIELD(x,0,31)
#define VTSS_M_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG4_SYNTH_FREQN_0  VTSS_ENCODE_BITMASK(0,31)
#define VTSS_X_VENICE_DEV1_32_SD10G65_TX_SYNTH_SD10G65_TX_SYNTH_CFG4_SYNTH_FREQN_0(x)  VTSS_EXTRACT_BITFIELD(x,0,31)

/**
 * \brief SD10G65 RX Synthesizer Configuration register 3
 *
 * \details
 * Configuration register 3 for SD10G65 RX SYNTH.
 *
 * Register: \a VENICE_DEV1_32:SD10G65_RX_SYNTH:SD10G65_RX_SYNTH_CFG3
 */
#define VTSS_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG3  VTSS_IOREG(0x01, 1, 0xf143)
#define VTSS_F_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG3_SYNTH_FREQM_0(x)  VTSS_ENCODE_BITFIELD(x,0,31)
#define VTSS_M_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG3_SYNTH_FREQM_0  VTSS_ENCODE_BITMASK(0,31)
#define VTSS_X_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG3_SYNTH_FREQM_0(x)  VTSS_EXTRACT_BITFIELD(x,0,31)


/**
 * \brief SD10G65 RX Synthesizer Configuration register 4
 *
 * \details
 * Configuration register 4 for SD10G65 RX SYNTH.
 *
 * Register: \a VENICE_DEV1_32:SD10G65_RX_SYNTH:SD10G65_RX_SYNTH_CFG4
 */
#define VTSS_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG4  VTSS_IOREG(0x01, 1, 0xf144)
#define VTSS_F_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG4_SYNTH_FREQN_0(x)  VTSS_ENCODE_BITFIELD(x,0,31)
#define VTSS_M_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG4_SYNTH_FREQN_0  VTSS_ENCODE_BITMASK(0,31)
#define VTSS_X_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG4_SYNTH_FREQN_0(x)  VTSS_EXTRACT_BITFIELD(x,0,31)

//registers required for new venice 6g serdes

#define VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2A_RX_JITTER_AMPL(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define VTSS_X_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2A_RX_JITTER_AMPL(x)  VTSS_EXTRACT_BITFIELD(x,0,10)
#define VTSS_M_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2A_RX_JITTER_AMPL     VTSS_ENCODE_BITMASK(0,10) 

# define VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_STEP_FREQ(x)  VTSS_ENCODE_BITFIELD(x,4,4)
# define VTSS_X_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_STEP_FREQ(x)  VTSS_EXTRACT_BITFIELD(x,4,4)
# define VTSS_M_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_STEP_FREQ  VTSS_ENCODE_BITMASK(4,4)

#define  VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_JI_ENA     VTSS_BIT(3)
#define  VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_WAVEFORM_SEL VTSS_BIT(2)
#define  VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_FREQOFF_DIR  VTSS_BIT(1)
#define  VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG2B_RX_FREQOFF_ENA  VTSS_BIT(0)

#define VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_MISC_CFG_DES_100FX_CPMD_MODE VTSS_BIT(9)

#define VTSS_F_VENICE_DEV4_SERDES6G_ANA_CFG_SERDES6G_SER_CFG_SER_4TAP_ENA  VTSS_BIT(8)

#define VTSS_F_VENICE_DEV4_SERDES6G_ANA_CFG_SERDES6G_COMMON_CFGB_ENA_ILOOP VTSS_BIT(9)

#define VTSS_F_VENICE_DEV4_SERDES6G_DIG_CFG_SERDES6G_DFT_CFG0_RX_DFT_ENA   VTSS_BIT(2)
/** 
 * \brief H_PLL5G Status 1A
 *
 * \details
 * Status register 1A for the H_PLL5G
 *
 * Register: \a HOST_PLL5G:H_PLL5G:H_PLL5G_STATUS1A
 */
#define VTSS_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A  VTSS_IOREG(0x1e, 0, 0x810e)

/** 
 * \brief
 * startup FSM lock status
 *
 * \details 
 * Field: VTSS_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A . FSM_LOCK
 */
#define  VTSS_F_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FSM_LOCK  VTSS_BIT(0)

/** 
 * \brief
 * startup FSM internal status
 *
 * \details 
 * Field: VTSS_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A . FSM_STAT
 */
#define  VTSS_F_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FSM_STAT(x)  VTSS_ENCODE_BITFIELD(x,1,3)
#define  VTSS_M_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FSM_STAT     VTSS_ENCODE_BITMASK(1,3)
#define  VTSS_X_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FSM_STAT(x)  VTSS_EXTRACT_BITFIELD(x,1,3)

/** 
 * \brief
 * VCO frequency difference to refclk
 *
 * \details 
 * Field: VTSS_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A . FBCNT_DIF
 */
#define  VTSS_F_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FBCNT_DIF(x)  VTSS_ENCODE_BITFIELD(x,4,10)
#define  VTSS_M_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FBCNT_DIF     VTSS_ENCODE_BITMASK(4,10)
#define  VTSS_X_HOST_PLL5G_H_PLL5G_H_PLL5G_STATUS1A_FBCNT_DIF(x)  VTSS_EXTRACT_BITFIELD(x,4,10)

#define VTSS_F_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI(x) VTSS_ENCODE_BITFIELD(x,22,4)
#define VTSS_M_VENICE_DEV1_32_SD10G65_RX_SYNTH_SD10G65_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI VTSS_ENCODE_BITMASK(22,4)
#define VTSS_F_VENICE_DEV1_32_SD10G65_RX_RCPLL_SD10G65_RX_RCPLL_CFG0_PLLF_SYN_CLK_ENA VTSS_BIT(15)
#define VTSS_F_VENICE_DEV1_32_SD10G65_RX_RCPLL_SD10G65_RX_RCPLL_CFG0_PLLF_LOOP_CTRL_ENA VTSS_BIT(11)
#define VTSS_F_VENICE_DEV1_32_SD10G65_RX_RCPLL_SD10G65_RX_RCPLL_CFG0_PLLF_LOOP_ENA VTSS_BIT(10)
#define  VTSS_F_VENICE_DEV1_32_SD10G65_APC_APC_COMMON_CFG0_SKIP_CAL VTSS_BIT(9)
#define VTSS_X_VENICE_DEV1_32_SD10G65_TX_RCPLL_SD10G65_TX_RCPLL_STAT1_PLLF_FSM_STAT(x) VTSS_EXTRACT_BITFIELD(x,0,4)
#define VTSS_X_VENICE_DEV1_32_SD10G65_RX_RCPLL_SD10G65_RX_RCPLL_STAT1_PLLF_FSM_STAT(x) VTSS_EXTRACT_BITFIELD(x,0,4)

//ADDING REGISTER DEFINES NEEDED FOR VENICE C F2DF SETTINGS
/**
 * \brief
 * reg. pool for late changes/fixes. Used bits: Bit3-1: Synthesizer BIAS
 * adjust in steps of ~3%.
 *
 * \details
 * 0: 100%
 * 7: 121%
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0 . SYNTH_SPARE_POOL
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_SPARE_POOL(x)  VTSS_ENCODE_BITFIELD(x,22,8)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_SPARE_POOL     VTSS_ENCODE_BITMASK(22,8)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_SPARE_POOL(x)  VTSS_EXTRACT_BITFIELD(x,22,8)

/**
 * \brief
 * Enable for different offset compensation stages
 *
 * \details
 * bit 0: Synthesizer main rotator
 * bit 1: Feedback buffer
 * bit 2: CDR rotator
 * bit 3: VCO buffer
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0 . SYNTH_OFF_COMP_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_OFF_COMP_ENA(x)  VTSS_ENCODE_BITFIELD(x,18,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_OFF_COMP_ENA     VTSS_ENCODE_BITMASK(18,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_OFF_COMP_ENA(x)  VTSS_EXTRACT_BITFIELD(x,18,4)

/**
 * \brief
 * frequency multiplier decoder bypass
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1 . SYNTH_FREQ_MULT_BYP
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_BYP  VTSS_BIT(26)

/**
 * \brief
 * frequency multiplier MSBits in bypass mode
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1 . SYNTH_FREQ_MULT_HI
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI(x)  VTSS_ENCODE_BITFIELD(x,22,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI     VTSS_ENCODE_BITMASK(22,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI(x)  VTSS_EXTRACT_BITFIELD(x,22,4)

/**
 * \brief
 * Select configuration of DFEamplifier gain adjustment
 *
 * \details
 * 0: Normal operation (APC can take over control)
 * 1: Enforce configuration via IB_DFE_GAIN_ADJ
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_IB_F2DF_IB_CFG7 . IB_DFE_GAIN_ADJ_S
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_IB_F2DF_IB_CFG7_IB_DFE_GAIN_ADJ_S  VTSS_BIT(15)

/**
 * \brief
 * Enable feedback clock usage instead of DES/SER par. clock,
 *
 * \details
 * 0: disable
 * 1: enable
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_SYN_CLK_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_SYN_CLK_ENA  VTSS_BIT(15)

/**
 * \brief
 * Enable closed loop, selected with pllf_loop_ctrl_ena=1
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_LOOP_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_LOOP_ENA  VTSS_BIT(10)

/**
 * \brief
 * Enable VCO control signal out of range recalibration
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_OOR_RECAL_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_OOR_RECAL_ENA  VTSS_BIT(6)


/**
 * \brief
 * Actual value of the FSM stage,
 *
 * \details
 * 0: reset state
 * 1: init state after reset
 * 3: ramp up state checks for the counters and ramps up the frequency
 * 6: additional wait state for internal BIAS settling
 * 8: additional wait state 1
 * 9: additional wait state 2
 * 10; additional wait state 3
 * 11: additional wait state 4
 * 12: 1st locking state enables dynamic locking
 * 13: final locking state checks for out of lock and overrun condition
 * 14: error state low frequency
 * 15: error state high frequency
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1 . PLLF_FSM_STAT
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1_PLLF_FSM_STAT(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1_PLLF_FSM_STAT     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1_PLLF_FSM_STAT(x)  VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * \brief
 * Pool of spare bits for use in late design changes.
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG . SBUS_SPARE_POOL
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG_SBUS_SPARE_POOL(x)  VTSS_ENCODE_BITFIELD(x,16,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG_SBUS_SPARE_POOL     VTSS_ENCODE_BITMASK(16,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG_SBUS_SPARE_POOL(x)  VTSS_EXTRACT_BITFIELD(x,16,4)

/**
 * \brief
 * Enable PLL loop ctrl by pllf_loop_ena,
 *
 * \details
 * 0: enable loop ctrl by FSM
 * 1: enable loop ctrl by pllf_loop_ena
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_LOOP_CTRL_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_LOOP_CTRL_ENA  VTSS_BIT(11)


#define VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_CFG_STAT_F2DF_SAMPLE_MODE  VTSS_BIT(1)



/**
 * \brief
 * Select vco current,
 *
 * \details
 * 0: lowest current
 * 31: highest current
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2 . PLL_VCO_CUR
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2_PLL_VCO_CUR(x)  VTSS_ENCODE_BITFIELD(x,2,5)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2_PLL_VCO_CUR     VTSS_ENCODE_BITMASK(2,5)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2_PLL_VCO_CUR(x)  VTSS_EXTRACT_BITFIELD(x,2,5)

/**
 * \brief
 * Select VCO load switch gate voltage,
 *
 * \details
 * 0: 1.4V
 * 1: 1.5V
 * 2: 1.6V
 * 3: 1.7V
 * 15: max.
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2 . PLL_VREG18
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2_PLL_VREG18(x)  VTSS_ENCODE_BITFIELD(x,20,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2_PLL_VREG18     VTSS_ENCODE_BITMASK(20,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG2_PLL_VREG18(x)  VTSS_EXTRACT_BITFIELD(x,20,4)

//ADDING REGISTER DEFINES NEEDED FOR VENICE C F2DF SETTINGS
/**
 * \brief
 * reg. pool for late changes/fixes. Used bits: Bit3-1: Synthesizer BIAS
 * adjust in steps of ~3%.
 *
 * \details
 * 0: 100%
 * 7: 121%
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0 . SYNTH_SPARE_POOL
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_SPARE_POOL(x)  VTSS_ENCODE_BITFIELD(x,22,8)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_SPARE_POOL     VTSS_ENCODE_BITMASK(22,8)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_SPARE_POOL(x)  VTSS_EXTRACT_BITFIELD(x,22,8)

/**
 * \brief
 * Enable for different offset compensation stages
 *
 * \details
 * bit 0: Synthesizer main rotator
 * bit 1: Feedback buffer
 * bit 2: CDR rotator
 * bit 3: VCO buffer
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0 . SYNTH_OFF_COMP_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_OFF_COMP_ENA(x)  VTSS_ENCODE_BITFIELD(x,18,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_OFF_COMP_ENA     VTSS_ENCODE_BITMASK(18,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG0_SYNTH_OFF_COMP_ENA(x)  VTSS_EXTRACT_BITFIELD(x,18,4)

/**
 * \brief
 * frequency multiplier decoder bypass
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1 . SYNTH_FREQ_MULT_BYP
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_BYP  VTSS_BIT(26)

/**
 * \brief
 * frequency multiplier MSBits in bypass mode
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1 . SYNTH_FREQ_MULT_HI
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI(x)  VTSS_ENCODE_BITFIELD(x,22,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI     VTSS_ENCODE_BITMASK(22,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_RX_SYNTH_CFG1_SYNTH_FREQ_MULT_HI(x)  VTSS_EXTRACT_BITFIELD(x,22,4)

/**
 * \brief
 * Select configuration of DFEamplifier gain adjustment
 *
 * \details
 * 0: Normal operation (APC can take over control)
 * 1: Enforce configuration via IB_DFE_GAIN_ADJ
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_IB_F2DF_IB_CFG7 . IB_DFE_GAIN_ADJ_S
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_IB_F2DF_IB_CFG7_IB_DFE_GAIN_ADJ_S  VTSS_BIT(15)

/**
 * \brief
 * Enable feedback clock usage instead of DES/SER par. clock,
 *
 * \details
 * 0: disable
 * 1: enable
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_SYN_CLK_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_SYN_CLK_ENA  VTSS_BIT(15)

/**
 * \brief
 * Enable closed loop, selected with pllf_loop_ctrl_ena=1
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_LOOP_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_LOOP_ENA  VTSS_BIT(10)

/**
 * \brief
 * Enable VCO control signal out of range recalibration
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_OOR_RECAL_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_OOR_RECAL_ENA  VTSS_BIT(6)


/**
 * \brief
 * Actual value of the FSM stage,
 *
 * \details
 * 0: reset state
 * 1: init state after reset
 * 3: ramp up state checks for the counters and ramps up the frequency
 * 6: additional wait state for internal BIAS settling
 * 8: additional wait state 1
 * 9: additional wait state 2
 * 10; additional wait state 3
 * 11: additional wait state 4
 * 12: 1st locking state enables dynamic locking
 * 13: final locking state checks for out of lock and overrun condition
 * 14: error state low frequency
 * 15: error state high frequency
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1 . PLLF_FSM_STAT
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1_PLLF_FSM_STAT(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1_PLLF_FSM_STAT     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_STAT1_PLLF_FSM_STAT(x)  VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * \brief
 * Pool of spare bits for use in late design changes.
 *
 * \details
 * Field: VTSS_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG . SBUS_SPARE_POOL
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG_SBUS_SPARE_POOL(x)  VTSS_ENCODE_BITFIELD(x,16,4)
#define  VTSS_M_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG_SBUS_SPARE_POOL     VTSS_ENCODE_BITMASK(16,4)
#define  VTSS_X_VENICE_GLOBAL32_F2DF_IB_F2DF_SBUS_RX_CFG_SBUS_SPARE_POOL(x)  VTSS_EXTRACT_BITFIELD(x,16,4)

/**
 * \brief
 * Enable PLL loop ctrl by pllf_loop_ena,
 *
 * \details
 * 0: enable loop ctrl by FSM
 * 1: enable loop ctrl by pllf_loop_ena
 *
 * Field: VTSS_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0 . PLLF_LOOP_CTRL_ENA
 */
#define  VTSS_F_VENICE_GLOBAL32_F2DF_RX_RCPLL_F2DF_RX_RCPLL_CFG0_PLLF_LOOP_CTRL_ENA  VTSS_BIT(11)


#define VTSS_F_VENICE_GLOBAL32_F2DF_RX_SYNTH_F2DF_CFG_STAT_F2DF_SAMPLE_MODE  VTSS_BIT(1)
