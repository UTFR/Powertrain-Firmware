from pinDriver_formatting import includesHeader
from pinDriver_formatting import definesHeader
from pinDriver_formatting import typedefsHeader
from pinDriver_formatting import privFuncDecHeader
from pinDriver_formatting import privDataDefHeader
from pinDriver_formatting import pubDataDefHeader
from pinDriver_formatting import privFuncHeader
from pinDriver_formatting import pubFuncHeader
from pinDriver_formatting import include_two

#.json initilization
import json

pinDriver = open('test.json')
pinDriver_list = json.load(pinDriver)

print("{}\n{}\n\n{}\n\n\n{}".format(includesHeader, include_two, definesHeader, typedefsHeader))

print("typedef enum")
print("{")

for dict in pinDriver_list:
    print("\t{},".format(dict.get('name')))

print("\tHW_PIN_COUNT")
print("} HW_PIN_E;")

print("\n{}\n\n\n{}\n\n\n{}\n\n\n{}\n\n\n{}".format(privFuncDecHeader, privDataDefHeader, pubDataDefHeader, privFuncHeader, pubFuncHeader))