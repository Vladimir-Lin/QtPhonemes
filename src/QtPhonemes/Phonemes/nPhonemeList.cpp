#include <qtphonemes.h>

N::PhonemeList:: PhonemeList (void)
               : synthflags  (0   )
               , phcode      (0   )
               , stresslevel (0   )
               , sourceix    (0   )
               , wordstress  (0   )
               , tone_ph     (0   )
               , length      (0   )
               , envelope    (0   )
               , type        (0   )
               , prepause    (0   )
               , postpause   (0   )
               , amp         (0   )
               , newword     (0   )
               , pitch1      (0   )
               , pitch2      (0   )
               , ph          (NULL)
{
}

N::PhonemeList:: PhonemeList(const PhonemeList & list)
{
  assign ( list ) ;
}

N::PhonemeList::~PhonemeList(void)
{
}

N::PhonemeList & N::PhonemeList::operator = (const PhonemeList & list)
{
  return assign(list) ;
}

N::PhonemeList & N::PhonemeList::assign(const PhonemeList & list)
{
  nMemberCopy ( list , synthflags  ) ;
  nMemberCopy ( list , phcode      ) ;
  nMemberCopy ( list , stresslevel ) ;
  nMemberCopy ( list , sourceix    ) ;
  nMemberCopy ( list , wordstress  ) ;
  nMemberCopy ( list , tone_ph     ) ;
  nMemberCopy ( list , ph          ) ;
  nMemberCopy ( list , length      ) ;
  nMemberCopy ( list , envelope    ) ;
  nMemberCopy ( list , type        ) ;
  nMemberCopy ( list , prepause    ) ;
  nMemberCopy ( list , postpause   ) ;
  nMemberCopy ( list , amp         ) ;
  nMemberCopy ( list , newword     ) ;
  nMemberCopy ( list , pitch1      ) ;
  nMemberCopy ( list , pitch2      ) ;
  return ME                          ;
}

void N::PhonemeList::setPhonemeCode(unsigned char code)
{
  phcode      = code ;
  stresslevel = 0    ;
  tone_ph     = 0    ;
  sourceix    = 0    ;
}
