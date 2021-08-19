/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "HW_pinDriver_componentSpecific.h"
#include <stdint.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef HW_PIN_DRIVER_H
#define HW_PIN_DRIVER_H

// Just to get it to compile, fix later!!
void placeholder(void);

#define OUTPUT 1U
#define INPUT 0U
#define HIGH 1U
#define LOW 0U
#define pinMode(id, dir) placeholder()
#define digitalWrite(id, state) placeholder()

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
typedef struct
{
    uint8_t pinID;
    uint8_t pinDirection;
    uint8_t initState;
} HW_pinConfig_S;

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
void HW_pinConfig_init(HW_pinConfig_S const * const config);
void HW_pinConfig_initDigOut(HW_pinConfig_S const * const config);

#endif /* HW_PIN_DRIVER_H */
