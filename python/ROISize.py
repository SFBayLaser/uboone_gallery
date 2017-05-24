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

# hit GoF histograms for each plane
fig = plt.figure(figsize=(10,8))
plt.scatter(dfU['hit_ch'].values,dfU['ROIsize'].values,label='U Plane',color='b',alpha=0.1)
plt.grid()
plt.xlabel("Channel",fontweight='bold')
plt.ylabel("ROI Size [ticks]",fontweight='bold')
plt.title("ROI size vs. Channel Number [U Plane]",fontweight='bold')
plt.ylim([0,500])
plt.legend(loc=1,fontsize=18)
plt.savefig('ROIsize_vs_chan_Uplane.png')

fig = plt.figure(figsize=(10,8))
plt.scatter(dfV['hit_ch'].values,dfV['ROIsize'].values,label='V Plane',color='g',alpha=0.1)
plt.grid()
plt.xlabel("Channel",fontweight='bold')
plt.ylabel("ROI Size [ticks]",fontweight='bold')
plt.title("ROI size vs. Channel Number [V Plane]",fontweight='bold')
plt.ylim([0,500])
plt.legend(loc=1,fontsize=18)
plt.savefig('ROIsize_vs_chan_Vplane.png')

fig = plt.figure(figsize=(10,8))
plt.scatter(dfY['hit_ch'].values,dfY['ROIsize'].values,label='Y Plane',color='r',alpha=0.1)
plt.grid()
plt.xlabel("Channel",fontweight='bold')
plt.ylabel("ROI Size [ticks]",fontweight='bold')
plt.title("ROI size vs. Channel Number [Y Plane]",fontweight='bold')
plt.ylim([0,500])
plt.legend(loc=1,fontsize=18)
plt.savefig('ROIsize_vs_chan_Yplane.png')
