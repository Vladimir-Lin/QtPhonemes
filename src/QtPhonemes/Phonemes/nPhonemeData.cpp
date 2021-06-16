#include <qtphonemes.h>

N::PhonemeData:: PhonemeData (void)
{
}

N::PhonemeData:: PhonemeData(const PhonemeData & data)
{
  assign ( data ) ;
}

N::PhonemeData::~PhonemeData(void)
{
}

N::PhonemeData & N::PhonemeData::operator = (const PhonemeData & data)
{
  return assign(data) ;
}

N::PhonemeData & N::PhonemeData::assign(const PhonemeData & data)
{
  nMemberCopy ( data , pd_control )                                ;
  nMemberCopy ( data , pitch_env  )                                ;
  nMemberCopy ( data , amp_env    )                                ;
  ::memcpy(pd_param        ,data.pd_param        ,sizeof(int )*16) ;
  ::memcpy(sound_addr      ,data.sound_addr      ,sizeof(int )* 5) ;
  ::memcpy(sound_param     ,data.sound_param     ,sizeof(int )* 5) ;
  ::memcpy(vowel_transition,data.vowel_transition,sizeof(int )* 4) ;
  ::memcpy(ipa_string      ,data.ipa_string      ,sizeof(char)*18) ;
  return ME                                                        ;
}
