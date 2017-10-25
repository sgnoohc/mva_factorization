# cd CMSSW_9_0_2_patch1/ ; cmsenv
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_9_2_3 ;
cmsenv
cd - 
export PATH=/usr/local/cuda-8.0/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
export THEANO_FLAGS='mode=FAST_RUN,device=gpu0,floatX=float32' 
# python mnist_cnn.py
export TF_CPP_MIN_LOG_LEVEL=2
