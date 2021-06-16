#include <qtphonemes.h>

N::SpeechWords:: SpeechWords (void)
               : flags       (0   )
               , start       (0   )
               , pre_pause   (0   )
               , wmark       (0   )
               , sourceix    (0   )
               , length      (0   )
{
}

N::SpeechWords:: SpeechWords(const SpeechWords & words)
{
  assign ( words ) ;
}

N::SpeechWords::~SpeechWords(void)
{
}

N::SpeechWords & N::SpeechWords::operator = (const SpeechWords & words)
{
  return assign(words) ;
}

N::SpeechWords & N::SpeechWords::assign(const SpeechWords & words)
{
  nMemberCopy ( words , flags     ) ;
  nMemberCopy ( words , start     ) ;
  nMemberCopy ( words , pre_pause ) ;
  nMemberCopy ( words , wmark     ) ;
  nMemberCopy ( words , sourceix  ) ;
  nMemberCopy ( words , length    ) ;
  return ME                         ;
}
