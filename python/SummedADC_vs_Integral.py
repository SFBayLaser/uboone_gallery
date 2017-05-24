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

df = pd.DataFrame(tree2array(t,stop=100000))

df['summed_to_integral'] = df.apply(lambda x : (x['hit_sumadc']-x['hit_integral']) / (0.5*(x['hit_sumadc']+x['hit_integral'])), axis=1 )

print 'DataFrame with %i columns and %i entries'%(df.shape[1],df.shape[0])

dfY = df.query("hit_view == 2")
dfV = df.query("hit_view == 1")
dfU = df.query("hit_view == 0")

plt.rcParams.update({'font.size': 20})

# hit GoF histograms for each plane
fig = plt.figure(figsize=(10,8))
BINS = np.linspace(-2,2,100)
plt.hist(dfU['summed_to_integral'].values,bins=BINS,histtype='step',lw=2,label='U Plane',color='b',normed=True)
plt.hist(dfV['summed_to_integral'].values,bins=BINS,histtype='step',lw=2,label='V Plane',color='g',normed=True)
plt.hist(dfY['summed_to_integral'].values,bins=BINS,histtype='step',lw=2,label='Y Plane',color='r',normed=True)
plt.grid()
plt.xlabel("0.5 * [Integral - Summed ADC ] / [ Integral + Summed ADC] ",fontweight='bold')
plt.ylabel("Number of Hits [normalized]",fontweight='bold')
plt.title('Summed ADC to Integral Comparison',fontweight='bold')
plt.yscale('log')
plt.legend(loc=1,fontsize=18)
plt.savefig('summedadc_to_integral_compare.png')
