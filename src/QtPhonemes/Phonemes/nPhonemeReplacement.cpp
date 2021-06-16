#include <qtphonemes.h>

N::PhonemeReplacement:: PhonemeReplacement (void)
                      : Old                (0   )
                      , New                (0   )
                      , Type               (0   )
{
}

N::PhonemeReplacement:: PhonemeReplacement(const PhonemeReplacement & replace)
{
  assign ( replace ) ;
}

N::PhonemeReplacement::~PhonemeReplacement(void)
{
}

N::PhonemeReplacement & N::PhonemeReplacement::operator = (const PhonemeReplacement & replace)
{
  return assign(replace) ;
}

N::PhonemeReplacement & N::PhonemeReplacement::assign(const PhonemeReplacement & replace)
{
  nMemberCopy ( replace , Old  ) ;
  nMemberCopy ( replace , New  ) ;
  nMemberCopy ( replace , Type ) ;
  return ME                      ;
}
