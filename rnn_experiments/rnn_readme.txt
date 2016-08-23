This is an overview of software in this folder.


General usage:
-------------

Software is meant to be run in conjuction with a YAML file which contains settings for experiment.  YAML file is required argument:
	
	python <script> <yaml file>


YAML file:
---------

YAML file contains all changable perameters currently incorpated into scripts.  Copy and rename YAML file to generate new experiments. 

Within YAML file is an 'expname' setting which is used to generate output file names for experiment.  Be sure to set this if you do not want your output files overwritten.

Each experiment consists of a training and classify script.  Both use the same YAML file.


Training and Classifying:
------------------------

Training scripts build models assumed to be one or more recurrent layers or same type, with a single dense output layer, and a merge layer if needed.

Three different script sets have evolved.  Some script sets ignore some YAML settings.  For example ww_train.py trains a whole wire as one sequence and ignores the sequence length setting.

	rnn_train.py & rnn_classify.py: this script takes whole wire sequences and slices into smaller sequences for training and testin; able to use bidirectional training if specified

	ww_train.py & rnn_classify.pt: this script assumes whole wire sequences and ignores sequence length specification; able to use bidirectional training if specified
	
	multiwire_train.py & multiwire_classify.py: assumes whole wire sequences and ignores sequence length specification; able to perform same function as ww in addition can add corresponding timesteps from neighboring wires as attributes; able to use bidirectional training if specified


