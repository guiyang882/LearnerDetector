#! /bin/python

import os

# tx,ty,bx,by --> tx,ty,width,height
def convertStandardFile(sourcepath, destpath):
    if os.path.exists(sourcepath) == False:
        print "Source Path Does Not Exists !"
        return

    writehandle = open(destpath, 'w')

    with open(sourcepath, 'r') as readhandle:
        for line in readhandle:
            line = line.strip()
            def dealf(x):
                if x != "NaN":
                    return int(x)
                return -1
            tmp_res = map(dealf, line.split(','))
            tmp_res[2] = tmp_res[2] - tmp_res[0]
            tmp_res[3] = tmp_res[3] - tmp_res[1]

            def saveformat(x):
                info = str(x[0]) + "," + str(x[1]) + "," + str(x[2]) + "," + str(x[3])
                return info
            print >> writehandle, saveformat(tmp_res)
    writehandle.close()



if __name__ == "__main__":
    sourcepath = "/Users/liuguiyang/Documents/CodeProj/ConsoleProj/TLD/data/Car24/program.txt"
    savepath = "/Users/liuguiyang/Documents/CodeProj/ConsoleProj/TLD/data/Car24/new.program.txt"
    convertStandardFile(sourcepath, savepath);
