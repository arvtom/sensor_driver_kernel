/**
*  \file    error_manager.c
*  \brief   Error manager, which uses individual bits to represent errors.
*  \author  arvtom
*/

/*------------------------------Includes------------------------------*/
#include "error_manager.h"

/*------------------------------Defines------------------------------*/

/*------------------------------Variables / Macro calls------------------------------*/
MODULE_LICENSE("GPL");

/*------------------------------Public functions------------------------------*/
/**
* \brief Set requested error bit in error variable
*/
void error_manager_set_u32(uint32_t *p_err_var, uint32_t err_bit)
{
    *p_err_var |= err_bit;
}

/**
* \brief Clear requested error bit in error variable
*/
void error_manager_clear_u32(uint32_t *p_err_var, uint32_t err_bit)
{
    *p_err_var &= (~ err_bit);
}

/**
* \brief Check if requested error bit is set in error variable
*/
bool error_manager_check_u32(uint32_t *p_err_var, uint32_t err_bit)
{
    if (0U == (*p_err_var & err_bit))
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*------------------------------Private functions------------------------------*/
