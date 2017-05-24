import sys,os

try:
    import matplotlib
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd
    import ROOT
    from root_numpy import root2array, root2rec, tree2array, array2root
except ImportError:
    print
    print 'Cannot import matplotlib or numpy'
    print 'EXIT '
    print
    sys.exit()

fname = sys.argv[-1]

f = ROOT.TFile(fname)
t = f.Get('tree_hit')

arr = tree2array(t)

df = pd.DataFrame(tree2array(t,stop=200000))

df['ROIsize'] = df.apply(lambda x : x['hit_end_tick']-x['hit_start_tick'], axis=1 )

print 'DataFrame with %i columns and %i entries'%(df.shape[1],df.shape[0])

dfY = df.query("hit_view == 2")
dfV = df.query("hit_view == 1")
dfU = df.query("hit_view == 0")

plt.rcParams.update({'font.size': 20})

chan_U_v = []
nhit_U_v = []
chan_V_v = []
nhit_V_v = []
chan_Y_v = []
nhit_Y_v = []

for chan in xrange(2400):
    dfchan = dfU.query('hit_ch == %i'%chan)
    if (dfchan.shape[0] == 0): continue
    nhits = dfchan.shape[0]
    chan_U_v.append(chan)
    nhit_U_v.append(float(nhits))

for chan in xrange(2400,4800):
    dfchan = dfV.query('hit_ch == %i'%chan)
    if (dfchan.shape[0] == 0): continue
    nhits = dfchan.shape[0]
    chan_V_v.append(chan)
    nhit_V_v.append(float(nhits))

for chan in xrange(4800,8256):
    dfchan = dfY.query('hit_ch == %i'%chan)
    if (dfchan.shape[0] == 0): continue
    nhits = dfchan.shape[0]
    chan_Y_v.append(chan)
    nhit_Y_v.append(float(nhits))

fig = plt.figure(figsize=(10,8))
plt.errorbar(chan_U_v,nhit_U_v,yerr=np.sqrt(nhit_U_v),color='b',fmt='o',label='U Plane')
plt.grid()
plt.xlabel("Channel",fontweight='bold')
plt.ylabel("Number of Hits",fontweight='bold')
plt.title("Number of Hits vs. Chan Number [U Plane]",fontweight='bold')
plt.legend(loc=1,fontsize=18)
plt.savefig('Nhits_vs_chan_Uplane.png')

fig = plt.figure(figsize=(10,8))
plt.errorbar(chan_V_v,nhit_V_v,yerr=np.sqrt(nhit_V_v),color='g',fmt='o',label='V Plane')
plt.grid()
plt.xlabel("Channel",fontweight='bold')
plt.ylabel("Number of Hits",fontweight='bold')
plt.title("Number of Hits vs. Chan Number [V Plane]",fontweight='bold')
plt.savefig('Nhits_vs_chan_Vplane.png')


fig = plt.figure(figsize=(10,8))
plt.errorbar(chan_Y_v,nhit_Y_v,yerr=np.sqrt(nhit_Y_v),color='r',fmt='o',label='Y Plane')
plt.grid()
plt.xlabel("Channel",fontweight='bold')
plt.ylabel("Number of Hits",fontweight='bold')
plt.title("Number of Hits vs. Chan Number [Y Plane]",fontweight='bold')
plt.legend(loc=1,fontsize=18)
plt.savefig('Nhits_vs_chan_Yplane.png')
