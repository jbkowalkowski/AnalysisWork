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

