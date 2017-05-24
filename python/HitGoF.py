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
df = pd.DataFrame(tree2array(t))

print 'DataFrame with %i columns and %i entries'%(df.shape[1],df.shape[0])

dfY = df.query("hit_view == 2")
dfV = df.query("hit_view == 1")
dfU = df.query("hit_view == 0")

plt.rcParams.update({'font.size': 20})

# hit GoF histograms for each plane
fig = plt.figure(figsize=(10,8))
BINS = np.linspace(-1,10,100)
plt.hist(dfU['hit_gof'].values,bins=BINS,histtype='step',lw=2,label='U Plane',color='b',normed=True)
plt.hist(dfV['hit_gof'].values,bins=BINS,histtype='step',lw=2,label='V Plane',color='g',normed=True)
plt.hist(dfY['hit_gof'].values,bins=BINS,histtype='step',lw=2,label='Y Plane',color='r',normed=True)
plt.grid()
plt.xlabel("Hit Goodness of Fit",fontweight='bold')
plt.ylabel("Number of Hits [normalized]",fontweight='bold')
plt.title('Goodness of Fit Distributions',fontweight='bold')
plt.yscale('log')
plt.legend(loc=1,fontsize=18)
plt.savefig('gof_distributions.png')

# GoF vs. channel numbers
fig = plt.figure(figsize=(10,8))
plt.scatter(dfU['hit_ch'].values,dfU['hit_gof'].values,color='b',alpha=0.1)
plt.ylim([0.1,100])
#plt.yscale('log')
plt.xlabel('Channel Number',fontweight='bold')
plt.ylabel('Goodness of Fit',fontweight='bold')
plt.title('U Plane GoF vs. Channel Number.',fontweight='bold')
plt.grid()
plt.savefig('gof_vs_chan_Uplane.png')

fig = plt.figure(figsize=(10,8))
plt.scatter(dfV['hit_ch'].values,dfV['hit_gof'].values,color='g',alpha=0.1)
plt.ylim([0.1,100])
#plt.yscale('log')
plt.xlabel('Channel Number',fontweight='bold')
plt.ylabel('Goodness of Fit',fontweight='bold')
plt.title('V Plane GoF vs. Channel Number.',fontweight='bold')
plt.grid()
plt.savefig('gof_vs_chan_Vplane.png')

fig = plt.figure(figsize=(10,8))
plt.scatter(dfY['hit_ch'].values,dfY['hit_gof'].values,color='r',alpha=0.1)
plt.ylim([0.1,100])
#plt.yscale('log')
plt.xlabel('Channel Number',fontweight='bold')
plt.ylabel('Goodness of Fit',fontweight='bold')
plt.title('Y Plane GoF vs. Channel Number.',fontweight='bold')
plt.grid()
plt.savefig('gof_vs_chan_Yplane.png')
