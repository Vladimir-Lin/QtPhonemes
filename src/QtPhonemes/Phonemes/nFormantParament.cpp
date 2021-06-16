#include <qtphonemes.h>

N::FormantParament:: FormantParament (void)
                   : fmt_control     (0   )
                   , use_vowelin     (0   )
                   , fmt_addr        (0   )
                   , fmt_length      (0   )
                   , fmt_amp         (0   )
                   , fmt2_addr       (0   )
                   , fmt2_lenadj     (0   )
                   , wav_addr        (0   )
                   , wav_amp         (0   )
                   , transition0     (0   )
                   , transition1     (0   )
                   , std_length      (0   )
{
}

N::FormantParament:: FormantParament(const FormantParament & formant)
{
  assign ( formant ) ;
}

N::FormantParament::~FormantParament(void)
{
}

N::FormantParament & N::FormantParament::operator = (const FormantParament & formant)
{
  return assign(formant) ;
}

N::FormantParament & N::FormantParament::assign(const FormantParament & formant)
{
  nMemberCopy ( formant , fmt_control ) ;
  nMemberCopy ( formant , use_vowelin ) ;
  nMemberCopy ( formant , fmt_addr    ) ;
  nMemberCopy ( formant , fmt_length  ) ;
  nMemberCopy ( formant , fmt_amp     ) ;
  nMemberCopy ( formant , fmt2_addr   ) ;
  nMemberCopy ( formant , fmt2_lenadj ) ;
  nMemberCopy ( formant , wav_addr    ) ;
  nMemberCopy ( formant , wav_amp     ) ;
  nMemberCopy ( formant , transition0 ) ;
  nMemberCopy ( formant , transition1 ) ;
  nMemberCopy ( formant , std_length  ) ;
  return ME                             ;
}

void N::FormantParament::Reset(void)
{
  fmt_control  = 0 ;
  use_vowelin  = 0 ;
  fmt_addr     = 0 ;
  fmt_length   = 0 ;
  fmt_amp      = 0 ;
  fmt2_addr    = 0 ;
  fmt2_lenadj  = 0 ;
  wav_addr     = 0 ;
  wav_amp      = 0 ;
  transition0  = 0 ;
  transition1  = 0 ;
  std_length   = 0 ;
}
