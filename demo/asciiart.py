import sys

def _input():
    if sys.version_info[0] == 2:
        return raw_input()
    else:
        return input()

def main():
    s = _input().replace("\n","")
    for i in s.split(" "):
        print(".0x%X" %int(i))
    nframe = int(s.split(" ")[2])
    nline = int(s.split(" ")[1])
    for iframe in range(nframe):
        print("# frame %d" %iframe)
        s = _input() # skip line
        for iline in range(nline):
            s = _input()
            for c in s:
                print(".0x%X" %ord(c))
            print(".0x0A")
        print(".0x0 # trailing zero for a frame")
    return
    
    
if __name__ == "__main__":
    main()
