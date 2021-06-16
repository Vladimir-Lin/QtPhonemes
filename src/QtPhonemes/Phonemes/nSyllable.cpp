#include <qtphonemes.h>

#define SYL_RISE        1
#define SYL_EMPHASIS    2
#define SYL_END_CLAUSE  4

N::Syllable:: Syllable (void)
{
}

N::Syllable:: Syllable(const Syllable & syllable)
{
  assign ( syllable ) ;
}

N::Syllable::~Syllable (void)
{
}

N::Syllable & N::Syllable::assign(const Syllable & syllable)
{
  nMemberCopy ( syllable , Stress      ) ;
  nMemberCopy ( syllable , Envelope    ) ;
  nMemberCopy ( syllable , Flags       ) ;
  nMemberCopy ( syllable , NextPhoneme ) ;
  nMemberCopy ( syllable , Pitch1      ) ;
  nMemberCopy ( syllable , Pitch2      ) ;
  return ME                              ;
}

N::Syllable & N::Syllable::operator = (const Syllable & syllable)
{
  return assign ( syllable ) ;
}

void N::Syllable::setPitch(int base,int drop)
{
  if (base < 0) base = 0         ;
  ////////////////////////////////
  Pitch2 = base                  ;
  if (drop < 0)                  {
    Flags |= SYL_RISE            ;
    Pitch1 = Pitch2 - drop       ;
  }  else                        {
    Pitch1 = Pitch2 + drop       ;
  }                              ;
  ////////////////////////////////
  if (Pitch1 <   0) Pitch1 =   0 ;
  if (Pitch1 > 254) Pitch1 = 254 ;
  if (Pitch2 > 254) Pitch2 = 254 ;
}
