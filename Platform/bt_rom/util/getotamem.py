#coding:utf-8
import sys

#print (sys.argv)
pxipoffect="mem_xip_flash_offset"
pbtname="mem_local_name_length"
pbtaddr="mem_lap"
prole="mem_hsp_role"
pautorole="mem_auto_hsp_role"
psvn="mem_public_code_svn_version_code"
pvpaddr="mem_vp_addr"
padvname="mem_tws_fac_adv_name"

writedat=open(sys.argv[2],"w")
with open(sys.argv[1],"r")as fmemmapformat:
    #print(len(fmemmapformat.readlines()))
    for line in fmemmapformat.readlines():
        line=line.strip('\n')
        if len(line):
            memmap_dic=line.split(' ')
            if memmap_dic[1] == pbtaddr:
                #print("find btaddr")
                writedat.write("a5\n01\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == psvn:
               #print("find psvn")
                writedat.write("a5\n02\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == pbtname:
                #print("find btname")
                writedat.write("a5\n03\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == pxipoffect:
                #print("find xip")
                writedat.write("a5\n04\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == prole:
                #print("find role")
                writedat.write("a5\n05\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == pautorole:
                #print("find pautorole")
                writedat.write("a5\n06\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == pvpaddr:
                #print("find pvpaddr")
                writedat.write("a5\n07\n"+memmap_dic[0][4:]+"\n"+memmap_dic[0][2:4]+"\n")
            if memmap_dic[1] == padvname:
                advname1=memmap_dic[0][4:]
                advname2=memmap_dic[0][2:4]
                
writedat.write("a5\n08\n"+advname1+"\n"+advname2+"\n")
writedat.close()



