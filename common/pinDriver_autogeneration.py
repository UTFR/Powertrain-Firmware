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

#include statements declared as variables
include_one = "#include \"HW_pinDriver.h\""
include_two = "#include <stdint.h>"

#print statements for constant formatting
print(includesHeader)
print(include_one)
print(include_two)
print("\n", end = "")
print(definesHeader)
print("\n\n", end = "")
print(typedefsHeader)
print("\n\n", end = "")
print(privFuncDecHeader)
print("\n\n", end = "")
print(privDataDefHeader)
print("\n\n", end = "")
print(pubDataDefHeader)
print("\n", end = "")

#print statements for array declaration
print("extern const HW_pinConfig_S HW_pinConfig[HW_PIN_COUNT];")
print("const HW_pinConfig_S HW_pinConfig[HW_PIN_COUNT] =")

print("{")

#if pinDriver_dict != None:

for i in range(len(pinDriver_dict)):
    print("\t[", end = "")
    print(pinDriver_dict[i].get('name'), end = "")
    print("] =")
    print("\t{")
    print("\t\t.pinID =", pinDriver_dict[i].get('pinID'), end = "")
    print(",")
    print("\t\t.pinDirection =", pinDriver_dict[i].get('pinDirection'), end = "")
    print(",")

    if pinDriver_dict[i].get('pinDirection') == "OUTPUT":
        print("\t\t.initState =", pinDriver_dict[i].get('initState'), end = "")
        print(",")
        if pinDriver_dict[i] == pinDriver_dict[-1]:
            print("\t}")
        else:
            print("\t},")

    else:
        if pinDriver_dict[i] == pinDriver_dict[-1]:
            print("\t}")
        else:
            print("\t},")

print("};")
print(privFuncHeader)
print("\n\n", end = "")
print(pubFuncHeader)   