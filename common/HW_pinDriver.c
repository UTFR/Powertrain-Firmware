/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "HW_pinDriver.h"
#include <stdint.h>
#include <stddef.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/

void placeholder(void)
{
}
/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/

  
/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/


/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/


/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/


/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

void HW_pinConfig_init(HW_pinConfig_S const * const config)
{
    if (config != NULL)
    {
        for (uint8_t i = 0; i < HW_PIN_COUNT; ++i)
        {
            pinMode(config[i].pinID, config[i].pinDirection);
        }
    }
}

void HW_pinConfig_initDigOut(HW_pinConfig_S const * const config)
{
    if (config != NULL)
    {
        for (uint8_t i = 0; i < HW_PIN_COUNT; ++i)
        {
            if (config[i].pinDirection == OUTPUT)
            {
                digitalWrite(config[i].pinID, config[i].initState);
            }
        }
    }
}

// Just here so that compilation can succeed. 

int main(void) 
{
    return 0;
}
