#include <qtphonemes.h>

N::WaveGenerator:: WaveGenerator     (void)
                 : PitchEnv          (NULL)
                 , Pitch             (0   )
                 , PitchIndex        (0   )
                 , dPitch            (0   )
                 , PitchBase         (0   )
                 , PitchRange        (0   )
                 , MixWaveFile       (NULL)
                 , nMixWaveFile      (0   )
                 , MixWaveScale      (0   )
                 , MixWaveAmp        (0   )
                 , MixWaveFileIndex  (0   )
                 , MixWaveFileMax    (0   )
                 , MixWaveFileOffset (0   )
                 , Amplitude         (0   )
                 , AmplitudeV        (0   )
                 , AmplitudeFormant  (0   )
{
}

N::WaveGenerator:: WaveGenerator(const WaveGenerator & generator)
{
  assign ( generator ) ;
}

N::WaveGenerator::~WaveGenerator(void)
{
}

N::WaveGenerator & N::WaveGenerator::assign(const WaveGenerator & generator)
{
  nMemberCopy ( generator , Pitch             ) ;
  nMemberCopy ( generator , PitchIndex        ) ;
  nMemberCopy ( generator , dPitch            ) ;
  nMemberCopy ( generator , PitchBase         ) ;
  nMemberCopy ( generator , PitchRange        ) ;
  nMemberCopy ( generator , nMixWaveFile      ) ;
  nMemberCopy ( generator , MixWaveScale      ) ;
  nMemberCopy ( generator , MixWaveAmp        ) ;
  nMemberCopy ( generator , MixWaveFileIndex  ) ;
  nMemberCopy ( generator , MixWaveFileMax    ) ;
  nMemberCopy ( generator , MixWaveFileOffset ) ;
  nMemberCopy ( generator , Amplitude         ) ;
  nMemberCopy ( generator , AmplitudeV        ) ;
  nMemberCopy ( generator , AmplitudeFormant  ) ;
//  unsigned char * PitchEnv                      ;
//  unsigned char * MixWaveFile                   ;
  return ME                                     ;
}

N::WaveGenerator & N::WaveGenerator::operator = (const WaveGenerator & generator)
{
  return assign ( generator ) ;
}

void N::WaveGenerator::setPitch (KlattFrame & frame)
{
  frame.F0hz10  = Pitch ;
  frame.F0hz10 *= 10    ;
  frame.F0hz10 /= 4096  ;
}
