// Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
// SPDX-License-Identifier: MIT


#ifndef _MSCC_ETHERNET_BOARD_API_H_
#define _MSCC_ETHERNET_BOARD_API_H_

#include <stdint.h>

#include <mscc/ethernet/board/api/types.h>
#include <mscc/ethernet/board/api/base.h>
#include <mscc/ethernet/board/api/synce.h>
#include <mscc/ethernet/board/api/tod.h>
#include <mscc/ethernet/board/api/poe.h>
#include <mscc/ethernet/board/api/phy_driver.h>
#include <mscc/ethernet/board/api/sfp_driver.h>
#include <mscc/ethernet/board/api/cpu_port.h>
#include <mscc/ethernet/board/api/hdr_start.h>  // ALL INCLUDE ABOVE THIS LINE

/** \brief Board instance struct */
struct meba_inst {
    int                      api_version;            /**< The MEBA version of the board implementation */
    size_t                   instance_size;          /**< The size of this structure (for compatibility checking) */
    void                     *private_data;          /**< Private (implementation) state data; */
    meba_board_interface_t   iface;                  /**< Board interface functions */
    meba_board_props_t       props;                  /**< Public board properties */
    meba_api_t               api;                    /**< Board API entrypoints */
    meba_api_synce_t         *api_synce;             /**< SyncE board API entrypoints */
    meba_api_tod_t           *api_tod;               /**< TOD board API entrypoints */
    meba_api_poe_t           *api_poe;               /**< PoE board API entrypoints */
    int                      synce_spi_if_fd;        /**< File descriptor of SyncE SPI interface.  */
    meba_api_cpu_port_t      *api_cpu_port;          /**< CPU Port API entrypoints */
};

/**
 * \brief Initialize board instance.
 *
 * \param callouts_size [IN] The size of the callouts structure
 * \param callouts      [IN] The system interface functions the board API should use
 *
 * The function must allocate and initialize the instance
 * strucure - the callouts are are copied to the allocated
 * instance.
 *
 * The return value should convey whether the expected hardware is
 * present. During execution of this function the MESA layer is not
 * (yet) initialized and must not be used.
 *
 * If local state is required by the board implementation, the instance
 * `private_data` pointer may be used to store this.
 *
 * At the time of returning from this function, the
 * meba_board_props_t `props` member must be initialized in order for
 * the MESA API to be instantiated.
 *
 * \return The board instance (or NULL)
 **/
typedef struct meba_inst *(*meba_initialize_t)(
        size_t callouts_size,
        const meba_board_interface_t *callouts);

/**
 * \brief Initialize board instance (used for static linking)
 *
 * \param callouts_size [IN] The size of the callouts structure
 * \param callouts      [IN] The system interface functions the board API should use
 *
 * \return The board instance (or NULL)
 **/
struct meba_inst *meba_initialize(size_t callouts_size,
                                  const meba_board_interface_t *callouts);

#include <mscc/ethernet/board/api/hdr_end.h>
#endif  // _MSCC_ETHERNET_BOARD_API_H_
