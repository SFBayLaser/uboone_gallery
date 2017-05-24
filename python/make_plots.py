import sys,os

if (len(sys.argv) != 2):

    print
    print 'Run as:'
    print 'python make_plots.py INPUT_ROOT_TTREE_FILE'
    print
    sys.exit()

inputfile = sys.argv[-1]

if (os.path.isfile(inputfile) == False):
    print
    print 'input file %s not a valid file'%inputfile
    print
    sys.exit()

if (inputfile.find('.root') < 0): 
    print
    print 'input file %s needs to be a valid ROOT file'%inputfile
    print
    sys.exit()

cmd = 'python SummedADC_vs_Integral.py %s'%inputfile
print
print 'RUNNING %s'%cmd
print
os.system(cmd)

cmd = 'python HitGoF.py %s'%inputfile
print
print 'RUNNING %s'%cmd
print
os.system(cmd)

cmd = 'python ROISize.py %s'%inputfile
print
print 'RUNNING %s'%cmd
print
os.system(cmd)

cmd = 'python HitvsTime.py %s'%inputfile
print
print 'RUNNING %s'%cmd
print
os.system(cmd)

cmd = 'python NHits_vs_Chan.py %s'%inputfile
print
print 'RUNNING %s'%cmd
print
os.system(cmd)

cmd = 'python ROI_vs_Hit_times.py %s'%inputfile
print
print 'RUNNING %s'%cmd
print
os.system(cmd)

sys.exit()
