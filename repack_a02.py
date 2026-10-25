#!/usr/bin/env python3
# Repack boot.img A02 (header v2): ganti kernel/cmdline/dtb/ramdisk, sisanya tetap.
import sys,struct,hashlib,argparse
p=argparse.ArgumentParser()
p.add_argument("boot");p.add_argument("kernel");p.add_argument("out")
p.add_argument("--cmdline-add",default="");p.add_argument("--ramdisk");p.add_argument("--dtb")
a=p.parse_args()
d=open(a.boot,"rb").read();pg=2048
al=lambda x:(x+pg-1)//pg*pg
h=list(struct.unpack("<8s10I16s512s32s1024s",d[:1632]))
ver=h[9];assert d[:8]==b"ANDROID!" and ver==2 and h[8]==pg
ks,rs,ss=h[1],h[3],h[5];assert ss==0
dsz,daddr=struct.unpack("<IQ",d[1648:1660])
o=pg;k=d[o:o+ks];o+=al(ks);r=d[o:o+rs];o+=al(rs);o+=al(ss);dtb=d[o:o+dsz]
k=open(a.kernel,"rb").read()
if a.ramdisk:r=open(a.ramdisk,"rb").read()
if a.dtb:dtb=open(a.dtb,"rb").read()
cmd=(h[12]+h[14]).rstrip(b"\0").decode()
for t in a.cmdline_add.split():
    if t.split("=")[0] not in [c.split("=")[0] for c in cmd.split()]:cmd+=" "+t
cb=cmd.encode();assert len(cb)<1536
h[1]=len(k);h[3]=len(r);h[12]=cb[:512].ljust(512,b"\0");h[14]=cb[512:].ljust(1024,b"\0")
s=hashlib.sha1()
for blob in(k,r,b"",b"",dtb):
    s.update(blob);s.update(struct.pack("<I",len(blob)))
h[13]=s.digest().ljust(32,b"\0")
hdr=struct.pack("<8s10I16s512s32s1024s",*h)+struct.pack("<IQI",0,0,1660)+struct.pack("<IQ",len(dtb),daddr)
pad=lambda b:b+b"\0"*(al(len(b))-len(b))
img=pad(hdr)+pad(k)+pad(r)+pad(dtb)
tot=len(d)
assert len(img)<=tot,"kebesaran: %d > %d"%(len(img),tot)
open(a.out,"wb").write(img.ljust(tot,b"\0"))
print("OK",a.out,"kernel",len(k),"ramdisk",len(r),"dtb",len(dtb),"total",tot);print("cmdline:",cmd)
