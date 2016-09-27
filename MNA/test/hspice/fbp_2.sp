file:fbp_2.sp       bandpass filter
* center frequency :
* analysis pole/zero(.pz), ac from 70khz-170khz
* ac current souce input
.options post acct  
.pz v(8) iin
.ac lin 300 70k 170k
.probe ac vp(8)
****************** circuit definition
iin 2 0  1 ac=1
r1 2 0 1k
c1 2 0 .137683nf
c2 2 3 31.6141nf
l2 2 3 35.1403uh
l3 3 0 339.941uh
c3 3 0 4.14505nf
c5 4 0 .463053nf
c4 3 4 9.86309nf
l4 3 4 111.158uh
c6 4 5 1.19190nf
c7 5 0 1.38666nf
l8 5 6 1592.78uh
c8 6 0 2.48487nf
c9 5 7 .405088nf
l9 7 8 4557.33uh
r2 8 0 1k
.end
