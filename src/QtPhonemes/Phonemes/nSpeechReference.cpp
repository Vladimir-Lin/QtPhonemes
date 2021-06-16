#include <qtphonemes.h>

N::SpeechReference:: SpeechReference (void)
                   : length          (0   )
                   , frflags         (0   )
                   , frame           (NULL)
{
}

N::SpeechReference:: SpeechReference(const SpeechReference & reference)
{
  assign ( reference ) ;
}

N::SpeechReference::~SpeechReference(void)
{
}

N::SpeechReference & N::SpeechReference::operator = (const SpeechReference & reference)
{
  return assign(reference) ;
}

N::SpeechReference & N::SpeechReference::assign(const SpeechReference & reference)
{
  nMemberCopy ( reference , frflags ) ;
  nMemberCopy ( reference , length  ) ;
  nMemberCopy ( reference , frame   ) ;
  return ME                           ;
}

N::SpeechFrame * N::SpeechReference::duplicate(SpeechReference & source,int length)
{
  if ( IsNull(source.frame) ) return NULL  ;
  if ( IsNull(       frame) ) return NULL  ;
  source.length = length                   ;
  frame -> copy ( *(source.frame) , true ) ;
  length = 0                               ;
  return frame                             ;
}
