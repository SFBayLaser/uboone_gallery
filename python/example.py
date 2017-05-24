import sys,os

try:
    import matplotlib.pyplot as plt
    import numpy as np
    print
    print 'you are setup and ready to go!'
    print
    sys.exit()
except ImportError:
    print
    print 'Cannot import matplotlib or numpy'
    print 'Try adding the necessary packages by running setup_python.sh'
    print 'this expands your PYTHONPATH as follows : '
    print 'export PYTHONPATH=/uboone/app/users/wketchum/python_install/lib/python2.7/site-packages:$PYTHONPATH'
    print
    print 'EXIT '
    print
    sys.exit()
