#!/usr/bin/python2
import sys

inf=open(sys.argv[1])
a=[i.split() for i in inf.readlines()]
#b=0
c=0
ins=0
bran=0
load=0
store=0
fp=0
mem=0
L2=0
SSE=0
uops=0
i=1
ps=0
pd=0
ss=0
sd=0
simd=0
line=0
print "start"
while i <len(a):
	if len(a[i])<7:
		i+=1
		continue
#	b+=long(a[i][0])
	c+=int(a[i][0])
	ins+=int(a[i][1])
	bran+=int(a[i][2])
	load+=int(a[i][3])
	store+=int(a[i][4])
	fp+=int(a[i][5])	
	mem+=int(a[i][6]) 
	L2+=int(a[i][7])
	L2+=int(a[i][8])
	SSE+=int(a[i][9])
	uops+=int(a[i][11])
	ps+=int(a[i][12])
	pd+=int(a[i][13])
	ss+=int(a[i][14])
	sd+=int(a[i][15])
	simd+=int(a[i][16])
	line+=1
	i+=1
#print "MLP",float(b)/c
print "Monitor Time=",line
print "IPC",float(ins)/c
print "Cycle(E9)",float(c)/1000000000
print "ins(E9)",float(ins)/1000000000
print "branch(E9)",float(bran)/1000000000
print "float(E9)",float(fp)/1000000000
print "load(E9)",float(load)/1000000000
print "store(E9)",float(store)/1000000000
print "integer(E9)", float(ins-load-bran-store-fp)/1000000000
print "Total SSE_FP_opearaions(E9)",float(ps+pd+ss+sd)/1000000000
print "Total SSE_Ins(E9)",float(simd)/1000000000
print "Float/SSE_FP", float(fp)/(ps+pd+ss+sd)
print "integer_FP_Ratio", float(ins-load-bran-store-fp)/(fp)
print "L2_Acess(E9)", float(L2)/1000000000
print "mem(E9)",float(mem)/1000000000
print "Flop_Operation_Intensity",float(fp)/(mem*64)
print "Bops_Operation_Intensity",float(ins-load-bran-store)/(mem*64)
print "FAI",float(fp)/(L2*64+SSE*8)
print "BAI",float(ins-load-bran-store)/(L2*64+SSE*8)
print "FP Ratio",float(fp)/(uops)
print "BOP Ratio",float(ins-load-bran-store)/(ins)
print "Bran Ratio",float(bran)/(ins)
print "LS Ratio",float(load+store)/(ins)
print "ins/uops",float(ins)/(uops)
print "BOPS(E9)=",float(ins-load-bran-store)/(line*1000000000)
