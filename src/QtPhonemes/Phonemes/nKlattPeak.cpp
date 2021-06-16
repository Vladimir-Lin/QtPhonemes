#include <qtphonemes.h>

N::KlattPeak:: KlattPeak (void)
             : Freq      (0   )
             , BW        (0   )
             , Amp       (0   )
             , BP        (0   )
             , Frequency (0   )
             , Bandwidth (0   )
             , Amplitude (0   )
             , Parallel  (0   )
             , dF        (0   )
             , dB        (0   )
             , dA        (0   )
             , dP        (0   )
{
}

N::KlattPeak::~KlattPeak(void)
{
}

N::KlattPeak & N::KlattPeak::operator ++ (void)
{
  Frequency += dF             ;
  Bandwidth += dB             ;
  Amplitude += dA             ;
  Parallel  += dP             ;
  Freq       = (int)Frequency ;
  BW         = (int)Bandwidth ;
  Amp        = (int)Amplitude ;
  BP         = (int)Parallel  ;
  return (*this)              ;
}

N::KlattPeak & N::KlattPeak::operator = (KlattPeak & peak)
{
  nMemberCopy ( peak , Freq      ) ;
  nMemberCopy ( peak , BW        ) ;
  nMemberCopy ( peak , Amp       ) ;
  nMemberCopy ( peak , BP        ) ;
  nMemberCopy ( peak , Frequency ) ;
  nMemberCopy ( peak , Bandwidth ) ;
  nMemberCopy ( peak , Amplitude ) ;
  nMemberCopy ( peak , Parallel  ) ;
  nMemberCopy ( peak , dF        ) ;
  nMemberCopy ( peak , dB        ) ;
  nMemberCopy ( peak , dA        ) ;
  nMemberCopy ( peak , dP        ) ;
  return (*this)                   ;
}
