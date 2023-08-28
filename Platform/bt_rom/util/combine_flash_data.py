#coding:utf-8
import sys
import os



if __name__ == '__main__':
    # print(sys.argv)
    # Check input size align 2
    input_args_cnt = len(sys.argv)
    if input_args_cnt < 2:
        print('Wrong!!! Input params error.')
    elif input_args_cnt % 2 != 0:
        print('Wrong!!! Input params error2.')
    else:
        fileCount = int((input_args_cnt - 2)/2)
        fileOffset = []
        filePath = []

        offsetOld = 0

        outfilePath = sys.argv[1]
        outfile = open(outfilePath, "w")

        for i in range(fileCount):
            offsetStr = sys.argv[2 * i + 2 + 0].replace('0x', '')
            pathStr = sys.argv[2 * i + 2 + 1]
            # print('fileOffset: 0x' + str(offsetStr) + ', filePath: ' + str(pathStr))

            offset = int(offsetStr, 16)

            fileOffset.append(offset)
            filePath.append(pathStr)

            if i != 0:
                # Sort file list
                if offset < fileOffset[i - 1]:
                    tmp = fileOffset[i - 1]
                    fileOffset[i - 1] = offset
                    fileOffset[i] = tmp

                    tmp = filePath[i - 1]
                    filePath[i - 1] = pathStr
                    filePath[i] = tmp

        saveOffset = 0
        isErrorWork = 0
        for i in range(fileCount):
            # print('After Sort: fileOffset: ' + str(hex(fileOffset[i])) + ', filePath: ' + str(filePath[i]))
            if saveOffset <= fileOffset[i]:
                for j in range(fileOffset[i] - saveOffset):
                    outfile.write('FF\n')
                    saveOffset += 1
                fileSize = 0
                with open(filePath[i], "r") as input_file:
                    for line in input_file.readlines():
                        outfile.write(line)
                        saveOffset += 1
                        fileSize += 1

                print(str(os.path.abspath(filePath[i])) + ' Offset: ' + str(hex(fileOffset[i]))
                      + ' Length: ' + str(fileSize) + ' (' + str(hex(fileSize)) + ')')
            else:
                print('Error!!! File : ' + str(os.path.abspath(filePath[i - 1])) + ' is too large')
                isErrorWork = True
                break

        outfile.close()
        if isErrorWork:
            os.remove(outfilePath)
        else:
            # reserve 4KB for save.
            limitSize = 4 * 1024
            flashUnit = 1024*1024/8
            needFlashSize = 0
            for i in range(1000):
                if (i*flashUnit - limitSize) > saveOffset:
                    needFlashSize = i
                    break

            print(str(os.path.abspath(outfilePath))
                  + ' Length: ' + str(saveOffset) + ' (' + str(hex(saveOffset)) + ')'
                  + ' Need Flash Size: ' + str(needFlashSize) + 'Mbit')



