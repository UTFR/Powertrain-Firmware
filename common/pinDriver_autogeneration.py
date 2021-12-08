from pinDriver_formatting import includesHeader
from pinDriver_formatting import definesHeader
from pinDriver_formatting import typedefsHeader
from pinDriver_formatting import privFuncDecHeader
from pinDriver_formatting import privDataDefHeader
from pinDriver_formatting import pubDataDefHeader
from pinDriver_formatting import privFuncHeader
from pinDriver_formatting import pubFuncHeader
from pinDriver_formatting import include_one
from pinDriver_formatting import include_two

#.json initilization
import json

pinDriver = open('test.json')
pinDriver_list = json.load(pinDriver)

#print statements for constant formatting
print("{}\n{}\n{}\n\n{}\n\n\n{}\n\n\n{}\n\n\n{}\n\n\n{}\n".format(includesHeader, include_one, include_two, definesHeader, typedefsHeader, privFuncDecHeader, privDataDefHeader, pubDataDefHeader))

#print statements for array declaration
print("extern const HW_pinConfig_S HW_pinConfig[HW_PIN_COUNT];")
print("const HW_pinConfig_S HW_pinConfig[HW_PIN_COUNT] =")

print("{")

for dict in pinDriver_list:
    print("\t[{}] = \n\t{{\n\t\t.pinID = {},\n\t\t.pinDirection = {},".format(dict.get('name'), dict.get('pinID'), dict.get('pinDirection')))

    if dict.get('pinDirection') == "OUTPUT":
        print("\t\t.initState = {},".format(dict.get('initState')))
        if dict == pinDriver_list[-1]:
            print("\t}")
        else:
            print("\t},")
    
    else:
        if dict == pinDriver_list[-1]:
            print("\t}")
        else:
            print("\t},")

print("};")
print("{}\n\n\n{}".format(privFuncHeader, pubFuncHeader))