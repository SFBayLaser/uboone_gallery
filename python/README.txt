Produce plots by running:

source setup_python.sh
python make_plots.py ttree_file.root

Warning:
I've noticed that setting up after gallery itself was setup with
the bash script provided by Wes in the top folder of this directory
caused me to not be able to use some python packages
To make these plots use a fresh terminal from which all you setup
is the bash script above

EXAMPLE root input file to make plots:
/uboone/data/users/davidc1/hitana.root
This file was made from a subset of Marco's 1k events which
pass selection 2 in MCC7 but processed w/ MCC8:
/uboone/app/users/mdeltutt/larsoft_v06_26_01_01/filter/filtered_file_list.txt
