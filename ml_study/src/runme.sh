

# no noise
cat >myconfig.fcl <<HERE
#include "standard_detsim_uboone.fcl"
source.inputCommands:["keep *","drop *_*_*_Detsim"]
physics.producers.daq.GenNoise:0
HERE
lar --process-name detsim2 -c ./myconfig.fcl prodgenie_detsim.root -o sig_no_noise.root

# time noise
cat >myconfig.fcl <<HERE
#include "standard_detsim_uboone.fcl"
source.inputCommands:["keep *","drop *_*_*_Detsim"]
physics.producers.daq.GenNoise:1
HERE
lar --process-name detsim2 -c ./myconfig.fcl prodgenie_detsim.root -o sig_time_noise.root

# freq noise
cat >myconfig.fcl <<HERE
#include "standard_detsim_uboone.fcl"
source.inputCommands:["keep *","drop *_*_*_Detsim"]
physics.producers.daq.GenNoise:2
HERE
lar --process-name detsim2 -c ./myconfig.fcl prodgenie_detsim.root -o sig_freq_noise.root

exit 0

# extract ranges

cat >myextract.fcl <<EOF
#include "extract_wf.fcl"
physics.analyzers.extractor.file_name: "sig_no_noise_"
physics.analyzers.extractor.zeros: 5
physics.analyzers.extractor.find_sigs: true
hysics.analyzers.extractor.as_mask: false
physics.analyzers.extractor.use_delta: false
EOF
lar -c ./myextract.fcl sig_no_noise.root

cat >myextract.fcl <<EOF
#include "extract_wf.fcl"
physics.analyzers.extractor.file_name: "sig_time_noise_"
physics.analyzers.extractor.zeros: 5
physics.analyzers.extractor.find_sigs: false
hysics.analyzers.extractor.as_mask: false
physics.analyzers.extractor.use_delta: false
EOF
lar -c ./myextract.fcl sig_time_noise.root

cat >myextract.fcl <<EOF
#include "extract_wf.fcl"
physics.analyzers.extractor.file_name: "sig_freq_noise_"
physics.analyzers.extractor.zeros: 5
physics.analyzers.extractor.find_sigs: false
hysics.analyzers.extractor.as_mask: false
physics.analyzers.extractor.use_delta: false
EOF
lar -c ./myextract.fcl sig_freq_noise.root

# produce noise-only files
python only_noise.py sig_no_noise sig_time_noise noise_time.csv
python only_noise.py sig_no_noise sig_freq_noise noise_freq.csv

# extract signal and noise using ranges
lar --process-name detsim3 -c ~jbk/checkouts/AnalysisWork/ml_study/src/extract_raw_tracks2.fcl sig_no_noise.root -o pig.root
lar --process-name detsim3 -c ~jbk/checkouts/AnalysisWork/ml_study/src/extract_raw_tracks2.fcl sig_time_noise.root -o pig.root
lar --process-name detsim3 -c ~jbk/checkouts/AnalysisWork/ml_study/src/extract_raw_tracks2.fcl sig_freq_noise.root -o pig.root

# produce all samples with noise mixed in
# signal+freq
python mix.py sig_no_noise.csv noise_freq.csv sig_freq.csv
# signal+time
python mix.py sig_no_noise.csv noise_time.csv sig_time.csv
# signal+time+freq
python mix.py noise_freq.csv sig_time.csv noise_freq_time.csv
