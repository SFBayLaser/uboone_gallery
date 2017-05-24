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

plt.rcParams.update({'font.size': 20})

# hit GoF histograms for each plane
fig = plt.figure(figsize=(10,8))
BINS = np.linspace(0,6400,1000)
vals,bc = np.histogram(df['hit_time'].values,bins=BINS)
bc = 0.5*(bc[:-1]+bc[1:])
plt.errorbar(bc,vals,np.sqrt(vals),fmt='o',color='b')
#plt.hist(,bins=BINS,histtype='step',lw=2,color='b',normed=True)
plt.grid()
plt.xlabel("Hit Time [tick]",fontweight='bold')
plt.ylabel("Number of Hits [normalized]",fontweight='bold')
plt.title('Hit Time Distribution',fontweight='bold')
plt.savefig('hit_time_distrib.png')
