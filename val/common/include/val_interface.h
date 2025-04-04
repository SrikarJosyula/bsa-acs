/** @file
 * Copyright (c) 2016-2025, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef __VAL_INTERFACE_H__
#define __VAL_INTERFACE_H__

#include "pal_interface.h"

/* set G_PRINT_LEVEL to one of the below values in your application entry
  to control the verbosity of the prints */
#define ACS_PRINT_ERR   5      /* Only Errors. use this to de-clutter the terminal and focus only on specifics */
#define ACS_PRINT_WARN  4      /* Only warnings & errors. use this to de-clutter the terminal and focus only on specifics */
#define ACS_PRINT_TEST  3      /* Test description and result descriptions. THIS is DEFAULT */
#define ACS_PRINT_DEBUG 2      /* For Debug statements. contains register dumps etc */
#define ACS_PRINT_INFO  1      /* Print all statements. Do not use unless really needed */


#define ACS_STATUS_FAIL      0x90000000
#define ACS_STATUS_ERR       0xEDCB1234  //some impropable value?
#define ACS_STATUS_SKIP      0x10000000
#define ACS_STATUS_PASS      0x0
#define ACS_STATUS_NIST_PASS 0x1
#define ACS_INVALID_INDEX    0xFFFFFFFF

#define NOT_IMPLEMENTED         0x4B1D  /* Feature or API not implemented */

#define VAL_EXTRACT_BITS(data, start, end) ((data >> start) & ((1ul << (end-start+1))-1))

#if defined(TARGET_EMULATION) || defined(TARGET_BM_BOOT)
#define TRUE 1
#define FALSE 0
#endif

typedef char char8_t;
/* GENERIC VAL APIs */
void val_allocate_shared_mem(void);
void val_free_shared_mem(void);
void val_print(uint32_t level, char8_t *string, uint64_t data);
void val_print_raw(uint64_t uart_addr, uint32_t level, char8_t *string,
                                                                uint64_t data);
void val_print_primary_pe(uint32_t level, char8_t *string, uint64_t data, uint32_t index);
void val_print_test_start(char8_t *string);
void val_print_test_end(uint32_t status, char8_t *string);
void val_set_test_data(uint32_t index, uint64_t addr, uint64_t test_data);
void val_get_test_data(uint32_t index, uint64_t *data0, uint64_t *data1);
uint32_t val_strncmp(char8_t *str1, char8_t *str2, uint32_t len);
void    *val_memcpy(void *dest_buffer, void *src_buffer, uint32_t len);
uint64_t val_time_delay_ms(uint64_t time_ms);

/* VAL PE APIs */

typedef enum {
  PE_FEAT_MPAM,
  PE_FEAT_PMU,
  PE_FEAT_RAS
} PE_FEAT_NAME;

uint32_t val_pe_create_info_table(uint64_t *pe_info_table);
void     val_pe_free_info_table(void);
uint32_t val_pe_get_num(void);
uint64_t val_pe_get_mpid_index(uint32_t index);
uint32_t val_pe_get_pmu_gsiv(uint32_t index);
uint32_t val_pe_get_gmain_gsiv(uint32_t index);
uint32_t val_pe_get_gicc_trbe_interrupt(uint32_t index);
uint64_t val_pe_get_mpid(void);
uint32_t val_pe_get_index_mpid(uint64_t mpid);
uint32_t val_pe_install_esr(uint32_t exception_type, void (*esr)(uint64_t, void *));
uint32_t val_pe_get_primary_index(void);
uint64_t val_get_primary_mpidr(void);
uint32_t val_get_pe_architecture(uint32_t index);
void     val_smbios_create_info_table(uint64_t *smbios_info_table);
void     val_smbios_free_info_table(void);
uint32_t val_get_num_smbios_slots(void);

void     val_execute_on_pe(uint32_t index, void (*payload)(void), uint64_t args);
uint32_t val_pe_feat_check(PE_FEAT_NAME pe_feature);

/* GIC VAL APIs */
uint32_t    val_gic_create_info_table(uint64_t *gic_info_table);
uint32_t    val_gic_route_interrupt_to_pe(uint32_t int_id, uint64_t mpidr);

typedef enum {
  GIC_INFO_VERSION=1,
  GIC_INFO_SEC_STATES,
  GIC_INFO_AFFINITY_NS,
  GIC_INFO_ENABLE_GROUP1NS,
  GIC_INFO_SGI_NON_SECURE,
  GIC_INFO_SGI_NON_SECURE_LEGACY,
  GIC_INFO_DIST_BASE,
  GIC_INFO_CITF_BASE,
  GIC_INFO_NUM_RDIST,
  GIC_INFO_RDIST_BASE,
  GIC_INFO_NUM_ITS,
  GIC_INFO_ITS_BASE,
  GIC_INFO_NUM_MSI_FRAME,
  GIC_INFO_NUM_GICR_GICRD
}GIC_INFO_e;

uint32_t val_gic_get_info(GIC_INFO_e type);
void     val_gic_free_info_table(void);
uint32_t val_gic_install_isr(uint32_t int_id, void (*isr)(void));
uint32_t val_gic_end_of_interrupt(uint32_t int_id);
void val_gic_cpuif_init(void);
uint32_t val_gic_request_irq(uint32_t irq_num, uint32_t mapped_irq_num, void *isr);
void val_gic_free_irq(uint32_t irq_num, uint32_t mapped_irq_num);
uint32_t val_gic_get_intr_trigger_type(uint32_t int_id, INTR_TRIGGER_INFO_TYPE_e *trigger_type);
uint32_t val_gic_its_configure(void);
uint32_t val_gic_its_get_base(uint32_t its_id, uint64_t *its_base);
uint32_t val_gic_request_msi(uint32_t bdf, uint32_t device_id, uint32_t its_id,
                             uint32_t int_id, uint32_t msi_index);
void val_gic_free_msi(uint32_t bdf, uint32_t device_id, uint32_t its_id,
                      uint32_t int_id, uint32_t msi_index);
uint32_t val_get_num_nongic_ctrl(void);

/*TIMER VAL APIs */
typedef enum {
  TIMER_INFO_CNTFREQ = 1,
  TIMER_INFO_PHY_EL1_INTID,
  TIMER_INFO_PHY_EL1_FLAGS,
  TIMER_INFO_VIR_EL1_INTID,
  TIMER_INFO_VIR_EL1_FLAGS,
  TIMER_INFO_PHY_EL2_INTID,
  TIMER_INFO_PHY_EL2_FLAGS,
  TIMER_INFO_VIR_EL2_INTID,
  TIMER_INFO_VIR_EL2_FLAGS,
  TIMER_INFO_NUM_PLATFORM_TIMERS,
  TIMER_INFO_IS_PLATFORM_TIMER_SECURE,
  TIMER_INFO_SYS_CNTL_BASE,
  TIMER_INFO_SYS_CNT_BASE_N,
  TIMER_INFO_FRAME_NUM,
  TIMER_INFO_SYS_INTID,
  TIMER_INFO_SYS_TIMER_STATUS
}TIMER_INFO_e;

void     val_timer_create_info_table(uint64_t *timer_info_table);
void     val_timer_free_info_table(void);
uint64_t val_timer_get_info(TIMER_INFO_e info_type, uint64_t instance);
void     val_timer_set_phy_el1(uint64_t timeout);
void     val_timer_set_vir_el1(uint64_t timeout);
void val_platform_timer_get_entry_index(uint64_t instance, uint32_t *block, uint32_t *index);
uint64_t val_get_phy_el2_timer_count(void);

/* Watchdog VAL APIs */
typedef enum {
  WD_INFO_COUNT = 1,
  WD_INFO_CTRL_BASE,
  WD_INFO_REFRESH_BASE,
  WD_INFO_GSIV,
  WD_INFO_ISSECURE,
  WD_INFO_IS_EDGE
}WD_INFO_TYPE_e;

void     val_wd_create_info_table(uint64_t *wd_info_table);
void     val_wd_free_info_table(void);
uint64_t val_wd_get_info(uint32_t index, WD_INFO_TYPE_e info_type);

/* PCIE VAL APIs */
void     val_pcie_enumerate(void);
void     val_pcie_create_info_table(uint64_t *pcie_info_table);
uint32_t val_pcie_create_device_bdf_table(void);
addr_t val_pcie_get_ecam_base(uint32_t rp_bdf);
void *val_pcie_bdf_table_ptr(void);
void     val_pcie_free_info_table(void);
void val_pcie_read_ext_cap_word(uint32_t bdf, uint32_t ext_cap_id, uint8_t offset, uint16_t *val);
uint32_t val_pcie_get_pcie_type(uint32_t bdf);
uint32_t val_pcie_p2p_support(void);
uint32_t val_pcie_dev_p2p_support(uint32_t bdf);
uint32_t val_pcie_is_onchip_peripheral(uint32_t bdf);
uint32_t val_pcie_device_port_type(uint32_t bdf);
uint32_t val_pcie_find_capability(uint32_t bdf, uint32_t cid_type,
                                           uint32_t cid, uint32_t *cid_offset);
void val_pcie_disable_bme(uint32_t bdf);
void val_pcie_enable_bme(uint32_t bdf);
void val_pcie_disable_msa(uint32_t bdf);
void val_pcie_enable_msa(uint32_t bdf);
uint32_t val_pcie_is_msa_enabled(uint32_t bdf);
void val_pcie_clear_urd(uint32_t bdf);
uint32_t val_pcie_is_urd(uint32_t bdf);
void val_pcie_enable_eru(uint32_t bdf);
void val_pcie_disable_eru(uint32_t bdf);
uint32_t val_pcie_bitfield_check(uint32_t bdf, uint64_t *bf_entry);
uint32_t val_pcie_register_bitfields_check(uint64_t *bf_info_table, uint32_t table_size);
uint32_t val_pcie_function_header_type(uint32_t bdf);
void val_pcie_get_mmio_bar(uint32_t bdf, void *base);
uint32_t val_pcie_get_downstream_function(uint32_t bdf, uint32_t *dsf_bdf);
uint8_t val_pcie_is_host_bridge(uint32_t bdf);
uint32_t val_pcie_mem_get_offset(uint32_t bdf, PCIE_MEM_TYPE_INFO_e mem_type);
void val_pcie_read_acsctrl(uint32_t arr[][1]);
void val_pcie_write_acsctrl(uint32_t arr[][1]);

/* IO-VIRT APIs */

typedef enum {
  SMMU_NUM_CTRL = 1,
  SMMU_CTRL_BASE,
  SMMU_CTRL_ARCH_MAJOR_REV,
  SMMU_IOVIRT_BLOCK,
  SMMU_SSID_BITS,
  SMMU_IN_ADDR_SIZE,
  SMMU_OUT_ADDR_SIZE
}SMMU_INFO_e;

typedef enum {
  SMMU_CAPABLE     = 1,
  SMMU_CHECK_DEVICE_IOVA,
  SMMU_START_MONITOR_DEV,
  SMMU_STOP_MONITOR_DEV,
  SMMU_CREATE_MAP,
  SMMU_UNMAP,
  SMMU_IOVA_PHYS,
  SMMU_DEV_DOMAIN,
  SMMU_GET_ATTR,
  SMMU_SET_ATTR,
}SMMU_OPS_e;

typedef enum {
  NUM_PCIE_RC = 1,
  RC_SEGMENT_NUM,
  RC_ATS_ATTRIBUTE,
  RC_MEM_ATTRIBUTE,
  RC_IOVIRT_BLOCK,
  RC_SMMU_BASE
}PCIE_RC_INFO_e;

typedef enum {
  ITS_NUM_GROUPS = 1,
  ITS_GROUP_NUM_BLOCKS,
  ITS_GET_ID_FOR_BLK_INDEX,
  ITS_GET_GRP_INDEX_FOR_ID,
  ITS_GET_BLK_INDEX_FOR_ID
} ITS_INFO_e;

void     val_iovirt_create_info_table(uint64_t *iovirt_info_table);
void     val_iovirt_free_info_table(void);
uint32_t val_iovirt_get_rc_smmu_index(uint32_t rc_seg_num, uint32_t rid);
uint64_t val_smmu_get_info(SMMU_INFO_e, uint32_t index);
uint64_t val_iovirt_get_smmu_info(SMMU_INFO_e type, uint32_t index);

typedef enum {
    DMA_NUM_CTRL = 1,
    DMA_HOST_INFO,
    DMA_PORT_INFO,
    DMA_TARGET_INFO,
    DMA_HOST_COHERENT,
    DMA_HOST_IOMMU_ATTACHED,
    DMA_HOST_PCI
} DMA_INFO_e;

void     val_dma_create_info_table(uint64_t *dma_info_ptr);
uint64_t val_dma_get_info(DMA_INFO_e type, uint32_t index);
uint32_t val_dma_start_from_device(void *buffer, uint32_t length, uint32_t index);
uint32_t val_dma_iommu_check_iova(uint32_t ctrl_index, addr_t dma_addr, addr_t cpu_addr);


/* POWER and WAKEUP APIs */
void     val_debug_brk(uint32_t data);

typedef enum {
    PER_FLAG_MSI_ENABLED = 0x2
}PERIPHERAL_FLAGS_e;

/* Peripheral Tests APIs */
typedef enum {
  NUM_USB,
  NUM_SATA,
  NUM_UART,
  NUM_ALL,
  USB_BASE0,
  USB_FLAGS,
  USB_GSIV,
  USB_BDF,
  USB_INTERFACE_TYPE,
  USB_PLATFORM_TYPE,
  SATA_BASE0,
  SATA_BASE1,
  SATA_FLAGS,
  SATA_GSIV,
  SATA_BDF,
  SATA_INTERFACE_TYPE,
  SATA_PLATFORM_TYPE,
  UART_BASE0,
  UART_BASE1,
  UART_WIDTH,
  UART_GSIV,
  UART_FLAGS,
  UART_BAUDRATE,
  UART_INTERFACE_TYPE,
  ANY_BASE0,
  ANY_FLAGS,
  ANY_GSIV,
  ANY_BDF,
  MAX_PASIDS
}PERIPHERAL_INFO_e;

void     val_peripheral_create_info_table(uint64_t *peripheral_info_table);
void     val_peripheral_free_info_table(void);
uint64_t val_peripheral_get_info(PERIPHERAL_INFO_e info_type, uint32_t index);
uint32_t val_peripheral_is_pcie(uint32_t bdf);
void     val_peripheral_dump_info(void);

/* Memory Tests APIs */
#define MEM_ALIGN_4K       0x1000
#define MEM_ALIGN_8K       0x2000
#define MEM_ALIGN_16K      0x4000
#define MEM_ALIGN_32K      0x8000
#define MEM_ALIGN_64K      0x10000

/* Mem Map APIs */
void val_mmap_add_region(uint64_t va_base, uint64_t pa_base,
                uint64_t length, uint64_t attributes);
uint32_t val_setup_mmu(void);
uint32_t val_enable_mmu(void);

/* Identify memory type using MAIR attribute, refer to ARM ARM VMSA for details */

#define MEM_NORMAL_WB_IN_OUT(attr) (((attr & 0xcc) == 0xcc) || (((attr & 0x7) >= 5) && (((attr >> 4) & 0x7) >= 5)))
#define MEM_NORMAL_NC_IN_OUT(attr) (attr == 0x44)
#define MEM_DEVICE(attr) ((attr & 0xf0) == 0)
#define MEM_SH_INNER(sh) (sh == 0x3)

void     val_memory_create_info_table(uint64_t *memory_info_table);
void     val_memory_free_info_table(void);
uint64_t val_memory_get_info(addr_t addr, uint64_t *attr);
uint32_t val_memory_get_entry_index(uint32_t type, uint32_t instance);
void val_pe_cache_clean_invalidate_range(uint64_t start_addr, uint64_t length);
void val_pe_cache_invalidate_range(uint64_t start_addr, uint64_t length);

/* MPAM tests APIs */
#define MPAM_INVALID_INFO 0xFFFFFFFF
#define SRAT_INVALID_INFO 0xFFFFFFFF
#define HMAT_INVALID_INFO 0xFFFFFFFF

void val_mpam_create_info_table(uint64_t *mpam_info_table);
void val_mpam_free_info_table(void);

typedef enum {
  MPAM_RSRC_TYPE_PE_CACHE,
  MPAM_RSRC_TYPE_MEMORY,
  MPAM_RSRC_TYPE_SMMU,
  MPAM_RSRC_TYPE_MEM_SIDE_CACHE,
  MPAM_RSRC_TYPE_ACPI_DEVICE,
  MPAM_RSRC_TYPE_UNKNOWN = 0xFF  /* 0x05-0xFE Reserved for future use */
} MPAM_RSRC_LOCATOR_TYPE;

/* MPAM info request types*/
typedef enum {
  MPAM_MSC_RSRC_COUNT,
  MPAM_MSC_RSRC_RIS,
  MPAM_MSC_RSRC_TYPE,
  MPAM_MSC_BASE_ADDR,
  MPAM_MSC_ADDR_LEN,
  MPAM_MSC_RSRC_DESC1,
  MPAM_MSC_RSRC_DESC2,
  MPAM_MSC_OF_INTR,
  MPAM_MSC_OF_INTR_FLAGS,
  MPAM_MSC_ERR_INTR,
  MPAM_MSC_ERR_INTR_FLAGS,
  MPAM_MSC_NRDY,
  MPAM_MSC_ID,
  MPAM_MSC_INTERFACE_TYPE
} MPAM_INFO_e;

/* SRAT */
typedef enum {
  SRAT_MEM_NUM_MEM_RANGE,
  SRAT_MEM_BASE_ADDR,
  SRAT_MEM_ADDR_LEN,
  SRAT_GICC_PROX_DOMAIN,
  SRAT_GICC_PROC_UID,
  SRAT_GICC_REMOTE_PROX_DOMAIN
} SRAT_INFO_e;

/* HMAT APIs */
void val_hmat_create_info_table(uint64_t *hmat_info_table);
void val_hmat_free_info_table(void);

/* SRAT APIs */
void val_srat_create_info_table(uint64_t *srat_info_table);
void val_srat_free_info_table(void);
uint64_t val_srat_get_info(SRAT_INFO_e type, uint64_t prox_domain);

/* PCC related APIs */
void val_pcc_create_info_table(uint64_t *pcc_info_table);
void *val_pcc_cmd_response(uint32_t subspace_id, uint32_t command, void *data, uint32_t data_size);
uint32_t val_pcc_get_ss_info_idx(uint32_t subspace_id);
void val_pcc_free_info_table(void);

/*Cache related info APIs*/
#define INVALID_CACHE_INFO 0xFFFFFFFFFFFFFFFF
#define CACHE_TABLE_EMPTY 0xFFFFFFFF
#define DEFAULT_CACHE_IDX 0xFFFFFFFF

typedef enum {
  CACHE_TYPE,
  CACHE_SIZE,
  CACHE_ID,
  CACHE_NEXT_LEVEL_IDX,
  CACHE_PRIVATE_FLAG
} CACHE_INFO_e;

void val_cache_create_info_table(uint64_t *cache_info_table);
void val_cache_free_info_table(void);
uint64_t val_cache_get_info(CACHE_INFO_e type, uint32_t cache_index);
uint32_t val_cache_get_llc_index(void);
uint32_t val_cache_get_pe_l1_cache_res(uint32_t res_index);

uint64_t val_pmu_reg_read(uint32_t reg_id);
void val_pmu_reg_write(uint32_t reg_id, uint64_t write_data);
void val_pmu_cycle_counter_start(void);
void val_pmu_cycle_counter_stop(void);

#endif
