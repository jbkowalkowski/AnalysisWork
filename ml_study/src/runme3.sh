
# produce noise-only files
python only_noise.py sig_no_noise sig_time_noise noise_time
python only_noise.py sig_no_noise sig_freq_noise noise_freq

# given ranges, extract signal and noise, produce 4 combintation of
# output for the given sequence length
# 1) signal with no noise
# 2) signal + time noise
# 3) signal + freq noise
# 4) time noise
# 5) freq noise
# 6) truth values
# make sure to subtract off the median
# ranges that are too short will be padded with noise, wrapping around the wire
# ranges that are too long will be used to produce more than more sample
# extract signal and noise using ranges
python extract_ranges.py sigs 20 sig_no_noise noise_time noise_freq sig_ranges

# pad the range to a given size
# be careful with the noise here
python pad_ranges.py 20 sig_ranges sig_ranges_20
python pad_ranges.py 50 sig_ranges sig_ranges_50
python pad_ranges.py 100 sig_ranges sig_ranges_100

# the previous runs do this
# produce all samples with noise mixed in
# signal+freq
# python mix.py sig_no_noise.csv noise_freq.csv sig_freq.csv
# signal+time
# python mix.py sig_no_noise.csv noise_time.csv sig_time.csv
# signal+time+freq
# python mix.py noise_freq.csv sig_time.csv noise_freq_time.csv
