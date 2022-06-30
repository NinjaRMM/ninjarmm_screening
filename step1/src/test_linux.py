import os
import sys
import subprocess


def main(argv):
    
    errcode = subprocess.call(["rpm", "-qa"  ],stderr=subprocess.DEVNULL)
    print(os.uname())

                 
if __name__ == '__main__':
    main(sys.argv)  