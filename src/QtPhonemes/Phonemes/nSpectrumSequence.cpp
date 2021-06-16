#include <qtphonemes.h>

N::SpectrumSequence:: SpectrumSequence (void)
{
}

N::SpectrumSequence:: SpectrumSequence(const SpectrumSequence & spectrum)
{
  assign ( spectrum ) ;
}

N::SpectrumSequence::~SpectrumSequence(void)
{
}

N::SpectrumSequence & N::SpectrumSequence::operator = (const SpectrumSequence & spectrum)
{
  return assign(spectrum) ;
}

N::SpectrumSequence & N::SpectrumSequence::assign(const SpectrumSequence & spectrum)
{
  nMemberCopy ( spectrum , Flags  ) ;
  nMemberCopy ( spectrum , Frames ) ;
  return ME                         ;
}


#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

frameref_t *LookupSpect(PHONEME_TAB *this_ph, int which, FMT_PARAMS *fmt_params,  int *n_frames, PHONEME_LIST *plist)
{//===================================================================================================================
	int  ix;
	int  nf;
	int  nf1;
	int  seq_break;
	frameref_t *frames;
	int  length1;
	int  length_std;
	int  length_factor;
	SPECT_SEQ *seq, *seq2;
	SPECT_SEQK *seqk, *seqk2;
	frame_t *frame;
	static frameref_t frames_buf[N_SEQ_FRAMES];

	seq = (SPECT_SEQ *)(&phondata_ptr[fmt_params->fmt_addr]);
	seqk = (SPECT_SEQK *)seq;
	nf = seq->n_frames;


	if(nf >= N_SEQ_FRAMES)
		nf = N_SEQ_FRAMES - 1;

	seq_len_adjust = fmt_params->fmt2_lenadj + fmt_params->fmt_length;
	seq_break = 0;

	for(ix=0; ix<nf; ix++)
	{
		if(seq->frame[0].frflags & FRFLAG_KLATT)
			frame = &seqk->frame[ix];
		else
			frame = (frame_t *)&seq->frame[ix];
		frames_buf[ix].frame = frame;
		frames_buf[ix].frflags = frame->frflags;
		frames_buf[ix].length = frame->length;
		if(frame->frflags & FRFLAG_VOWEL_CENTRE)
			seq_break = ix;
	}

	frames = &frames_buf[0];
	if(seq_break > 0)
	{
		if(which==1)
		{
			nf = seq_break + 1;
		}
		else
		{
			frames = &frames_buf[seq_break];  // body of vowel, skip past initial frames
			nf -= seq_break;
		}
	}

	// do we need to modify a frame for blending with a consonant?
	if((this_ph->type == phVOWEL) && (fmt_params->fmt2_addr == 0) && (fmt_params->use_vowelin))
	{
		seq_len_adjust += FormantTransition2(frames,nf,fmt_params->transition0,fmt_params->transition1,NULL,which);
	}

	length1 = 0;
	nf1 = nf - 1;
	for(ix=0; ix<nf1; ix++)
		length1 += frames[ix].length;

	if(fmt_params->fmt2_addr != 0)
	{
		// a secondary reference has been returned, which is not a wavefile
		// add these spectra to the main sequence
		seq2 = (SPECT_SEQ *)(&phondata_ptr[fmt_params->fmt2_addr]);
		seqk2 = (SPECT_SEQK *)seq2;

		// first frame of the addition just sets the length of the last frame of the main seq
		nf--;
		for(ix=0; ix<seq2->n_frames; ix++)
		{
			if(seq2->frame[0].frflags & FRFLAG_KLATT)
				frame = &seqk2->frame[ix];
			else
				frame = (frame_t *)&seq2->frame[ix];

			frames[nf].length = frame->length;
			if(ix > 0)
			{
				frames[nf].frame = frame;
				frames[nf].frflags = frame->frflags;
			}
			nf++;
		}
		wavefile_ix = 0;
	}

	if(length1 > 0)
	{
		if(which==2)
		{
			// adjust the length of the main part to match the standard length specified for the vowel
			//   less the front part of the vowel and any added suffix

			length_std = fmt_params->std_length + seq_len_adjust - 45;
			if(length_std < 10)
				length_std = 10;
			if(plist->synthflags & SFLAG_LENGTHEN)
				length_std += (phoneme_tab[phonLENGTHEN]->std_length * 2);  // phoneme was followed by an extra : symbol

// can adjust vowel length for stressed syllables here


			length_factor = (length_std * 256)/ length1;

			for(ix=0; ix<nf1; ix++)
			{
				frames[ix].length = (frames[ix].length * length_factor)/256;
			}
		}
		else
		{
			if(which == 1)
			{
				// front of a vowel
				if(fmt_params->fmt_control == 1)
				{
					// This is the default start of a vowel.
					// Allow very short vowels to have shorter front parts
					if(fmt_params->std_length < 130)
						frames[0].length = (frames[0].length * fmt_params->std_length)/130;
				}
			}
			else
			{
				//not a vowel
				if(fmt_params->std_length > 0)
				{
					seq_len_adjust += (fmt_params->std_length - length1);
				}
			}

			if(seq_len_adjust != 0)
			{
				length_factor = ((length1 + seq_len_adjust) * 256)/length1;
				for(ix=0; ix<nf1; ix++)
				{
					frames[ix].length = (frames[ix].length * length_factor)/256;
				}
			}
		}
	}

	*n_frames = nf;
	return(frames);
}  //  end of LookupSpect

#endif
