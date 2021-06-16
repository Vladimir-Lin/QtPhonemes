#include <qtphonemes.h>

N::SpeechMatch:: SpeechMatch (void)
{
}

N::SpeechMatch:: SpeechMatch(const SpeechMatch & match)
{
  assign ( match ) ;
}

N::SpeechMatch::~SpeechMatch(void)
{
}

N::SpeechMatch & N::SpeechMatch::operator = (const SpeechMatch & match)
{
  return assign(match) ;
}

N::SpeechMatch & N::SpeechMatch::assign(const SpeechMatch & match)
{
  nMemberCopy ( match , points   ) ;
  nMemberCopy ( match , phonemes ) ;
  nMemberCopy ( match , end_type ) ;
  nMemberCopy ( match , del_fwd  ) ;
  return ME                        ;
}
