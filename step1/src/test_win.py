import sys
import os
import pprint
import wmi
from windows_tools.updates import get_windows_updates

def main(argv):
    
    # it uses the package windows_tools to print all the windows patches
    for update in get_windows_updates(filter_duplicates=True):
        pprint.pprint(update)
    
    # uses the package wmi to get information about current computer
    c = wmi.WMI()   
    my_system = c.Win32_ComputerSystem()[0]     
    print(f"Manufacturer: {my_system.Manufacturer}")
    print(f"Model: {my_system. Model}")
    print(f"Name: {my_system.Name}")
    print(f"NumberOfProcessors: {my_system.NumberOfProcessors}")
    print(f"SystemType: {my_system.SystemType}")
    print(f"SystemFamily: {my_system.SystemFamily}")
      
                 
if __name__ == '__main__':
    main(sys.argv)  
