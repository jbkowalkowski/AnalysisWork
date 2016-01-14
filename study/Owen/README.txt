ParaviewExtract_module.cc will spit out:
	one .VTP file with all the tracks
	three .VTI files with rawdigits, one for each plane
These will go into the current directory.
Currently there is no use of event-specific filenames, so running the tool will overwrite files still in the directory from previous runs.

extract_raw_tracks.fcl is just extract_raw.fcl with the tracking algorithms uncommented
