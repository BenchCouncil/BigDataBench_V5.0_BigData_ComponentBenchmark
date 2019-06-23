#!/usr/bin/python2

import sys
import subprocess
import os, os.path
import exceptions
import re, string
import time

path=sys.argv[1]
interval=float(sys.argv[2])
def popen(cmd):
    p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    return out + err 

def interval_times(interval):
    while True:
        yield time.time()
        time.sleep(interval)
def log_write(path, times, *collectors):
    if os.path.exists(path): raise HmonErr('log file already exists', path)
    f = open(path, 'a')
    f.write('\t'.join(['timestamp'] + list_merge(*[c.values for c in collectors])) + '\n')
    for t in times:
        f.write('\t'.join([str(t)]+ list_merge(*[c() for c in collectors])) + '\n')
        f.flush()
    f.close()

f=open(path,'a')
#codes=['r3c:k','r3c:u','rc0:k','rc0:u','rc4','r1c0','r2c0','rfec1','rc9','r108','r412e','r4f2e','r0143','r0243','r80','r81','rfc2','r8aa','r86','r283','r1aa','r2062','r1dc','r2dc','r4dc','r8dc','r10dc']
#events=['cycles_k','cycles_u','ins_k','ins_u','bran','load','store','fp','itlb_miss','dtlb_miss','l2cache_miss','l2cache_reference','L1Dcache_hit','L1Dcache_miss','L1I_READS','L1I_miss','uops','cisc','fron_stall','inst_queue_full','ins_de','BUS_DRDY','rob_stall','rs_stall','ld_stall','fpcw_stall','br_stall']
codes=['r3c','rc0','rc4','r1c0','r2c0','r10','re06f','rf024','rc026','r307','rc1','ra0','r1ca','r2ca','r4ca','r8ca','r1fc7']
events=['cycles','ins','br','load','store','fp','mem_bus','L2_in','L2_out','SSE_store','X87','uops','pack_single','scalar_single','pack_double','scalar_double','simd']
print len(codes)
print len(events)
f.write(' '.join(events)+'\n')
while True:
	a=','.join(codes)
	#print a
	out = popen("sudo perf stat -a -e %s sleep %f"%(a,interval))
	counters = re.findall('\s+(\d+)\s+raw', out, re.M)
	#print out 
        #print counters
	f.write(' '.join(counters)+ '\n')
	f.flush()
f.close()
#print counters
