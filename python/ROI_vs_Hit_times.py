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

df['dt_start'] = df.apply(lambda x : x['hit_start_tick']-x['hit_roi_start'], axis=1 )
df['dt_end']   = df.apply(lambda x : x['hit_end_tick']  -x['hit_roi_end']  , axis=1 )

print 'DataFrame with %i columns and %i entries'%(df.shape[1],df.shape[0])

dfY = df.query("hit_view == 2")
dfV = df.query("hit_view == 1")
dfU = df.query("hit_view == 0")

plt.rcParams.update({'font.size': 20,"legend.fontsize":14})

fig = plt.figure(figsize=(10,8))

BINS = np.linspace(0,400,100)
plt.hist(dfU['hit_roi_size'].values,bins=BINS,lw=2,histtype='step',color='b',label='U Plane',normed=True)
plt.hist(dfV['hit_roi_size'].values,bins=BINS,lw=2,histtype='step',color='g',label='V Plane',normed=True)
plt.hist(dfY['hit_roi_size'].values,bins=BINS,lw=2,histtype='step',color='r',label='Y Plane',normed=True)
plt.yscale('log')
plt.grid()
plt.legend(loc=4,prop={'weight':'bold'})
plt.xlabel("ROI Size [ticks]",fontweight='bold')
plt.ylabel("Number of Hits",fontweight='bold')
plt.title("",fontweight='bold')
plt.savefig('ROI_size.png')
#plt.show()

fig = plt.figure(figsize=(10,8))

BINPOS = np.linspace(0,  500, 100)
BINNEG = np.linspace(-500, 0, 100)

plt.hist(dfY['dt_start'].values,BINPOS,histtype='step',lw=2,color='r',linestyle='solid', label='Hit start - ROI start [Y plane]',normed=True)
plt.hist(dfY['dt_end'].values,  BINNEG,histtype='step',lw=2,color='r',linestyle='dashed',label='Hit end - ROI end [Y plane]',normed=True)
plt.hist(dfV['dt_start'].values,BINPOS,histtype='step',lw=2,color='g',linestyle='solid', label='Hit start - ROI start [V plane]',normed=True)
plt.hist(dfV['dt_end'].values,  BINNEG,histtype='step',lw=2,color='g',linestyle='dashed',label='Hit end - ROI end [V plane]',normed=True)
plt.hist(dfU['dt_start'].values,BINPOS,histtype='step',lw=2,color='b',linestyle='solid', label='Hit start - ROI start [U plane]',normed=True)
plt.hist(dfU['dt_end'].values,  BINNEG,histtype='step',lw=2,color='b',linestyle='dashed',label='Hit end - ROI end [U plane]',normed=True)

plt.grid()
plt.xlabel("Tick Difference",fontweight='bold')
plt.ylabel("Number of Hits",fontweight='bold')
plt.title("",fontweight='bold')
plt.legend(loc=4,prop={'weight':'bold'})
plt.yscale('log')
plt.savefig('ROI_vs_hit_start_end_diff.png')
#plt.show()
