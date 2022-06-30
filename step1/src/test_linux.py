import os
import sys
import subprocess


def main(argv):
    
    # shows all the packages isntalled by running rpm -qa
    errcode = subprocess.call(["rpm", "-qa"  ],stderr=subprocess.DEVNULL)
    
    # prints operational system info using os library
    print(os.uname())

                 
if __name__ == '__main__':
    main(sys.argv)  
