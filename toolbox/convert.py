# coding=utf-8

import os

# tx,ty,bx,by --> tx,ty,width,height
def convertStandardFile(sourcepath, destpath):
    if os.path.exists(sourcepath) == False:
        print("Source Path Does Not Exists !")
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


import xml.dom.minidom

# 主要是解析图像对应的标签的区域的数据
def parseXmlFile(filepath):
    dom = xml.dom.minidom.parse(filepath)
    root = dom.documentElement
    vals = root.getElementsByTagName('object')
    data = []
    for item in vals:
        tmp = item.getElementsByTagName('bndbox')[0]
        xmin = tmp.getElementsByTagName('xmin')[0].firstChild.data
        ymin = tmp.getElementsByTagName('ymin')[0].firstChild.data
        xmax = tmp.getElementsByTagName('xmax')[0].firstChild.data
        ymax = tmp.getElementsByTagName('ymax')[0].firstChild.data
        data.append([int(xmin), int(ymin), int(xmax)-int(xmin), int(ymax)-int(ymin)])
    return data

# xml file ---> tx, ty, width, height
def convertXml2Txt(srclist):
    readlists, writelists = [], []
    with open(srclist, 'r') as handle:
        for line in handle:
            line = line.strip()
            readlists.append(line)
            writelists.append(line.replace("xml", "txt"))
    for i in range(0, len(readlists)):
        item = parseXmlFile(readlists[i])
        with open(writelists[i], 'w') as handle:
            for tmp in item:
                print("%d,%d,%d,%d" % (tmp[0], tmp[1], tmp[2], tmp[3]), file=handle)


if __name__ == "__main__":
    srclist = "/Users/liuguiyang/Desktop/airplane-remote/ground.list"
    convertXml2Txt(srclist)











