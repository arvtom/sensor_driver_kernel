/**
*  \file    error_manager.h
*  \brief   Error manager, which uses individual bits to represent errors.
*  \author  arvtom
*/

/*------------------------------Includes------------------------------*/
#include <linux/module.h>
#include <linux/types.h>

/*------------------------------Defines------------------------------*/

/*------------------------------Structures / enumerators------------------------------*/

/*------------------------------Public function prototypes------------------------------*/
void error_manager_set_u32(uint32_t *p_err_var, uint32_t err_bit);
void error_manager_clear_u32(uint32_t *p_err_var, uint32_t err_bit);
bool error_manager_check_u32(uint32_t *p_err_var, uint32_t err_bit);

/*------------------------------Private function prototypes------------------------------*/
