#.json initilization
import json

pinDriver = open('test.json')
pinDriver_dict = json.load(pinDriver)

#formatting variables declared
includesHeader = ("/******************************************************************************\n"
" *                              I N C L U D E S                               *\n"
" *****************************************************************************/")

definesHeader = ("/******************************************************************************\n"
" *                               D E F I N E S                                *\n"
" *****************************************************************************/")

typedefsHeader = ("/******************************************************************************\n"
" *                              T Y P E D E F S                               *\n"
" *****************************************************************************/")

privFuncDecHeader = ("/******************************************************************************\n"
" *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *\n"
" *****************************************************************************/")

privDataDefHeader = ("/******************************************************************************\n"
" *              P R I V A T E   D A T A   D E F I N I T I O N S               *\n"
" *****************************************************************************/")

pubDataDefHeader = ("/******************************************************************************\n"
" *              P U B L I C   D A T A   D E F I N I T I O N S               *\n"
" *****************************************************************************/")

privFuncHeader = ("/******************************************************************************\n"
" *                     P R I V A T E   F U N C T I O N S                      *\n"
" *****************************************************************************/")

pubFuncHeader = ("/******************************************************************************\n"
" *                      P U B L I C   F U N C T I O N S                       *\n"
" *****************************************************************************/")

print(includesHeader)
print("#include <stdint.h>")
print("\n", end = "")
print(definesHeader)
print("\n\n", end = "")
print(typedefsHeader)

print("typedef enum")
print("{")

for i in range(len(pinDriver_dict)):
    print("\t", end = "")
    print(pinDriver_dict[i].get('name'), end = "")
    print(",")

print("\tHW_PIN_COUNT")
print("} HW_PIN_E;")

print(privFuncDecHeader)
print("\n\n", end = "")
print(privDataDefHeader)
print("\n\n", end = "")
print(pubDataDefHeader)
print("\n\n", end = "")
print(privFuncHeader)
print("\n\n", end = "")
print(pubFuncHeader)
