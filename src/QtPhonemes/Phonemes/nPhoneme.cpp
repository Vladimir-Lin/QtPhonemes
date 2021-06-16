#include <qtphonemes.h>

typedef struct             {
  unsigned int   Mnemonic  ; // Up to 4 characters.  The first char is in the l.s.byte
  unsigned int   Flags     ; // bits 16-19 place of articulation
  unsigned short Program   ; // index into phondata file
  unsigned char  Code      ; // the phoneme number
  unsigned char  Type      ; // Speak::PhonemeTypes
  unsigned char  StartType ; // Speak::PhonemeTypes
  unsigned char  EndType   ; // Speak::PhonemeTypes
  unsigned char  Length    ; // for vowels, in mS/2;  for phSTRESS phonemes, this is the stress/tone type
  unsigned char  LengthMod ; // a length_mod group number, used to access length_mod_tab
} PHONEME_TAB              ;

const unsigned char PausePhonemes[8] = {
  0                                    ,
  N::Speak::PcPauseVShort              ,
  N::Speak::PcPauseShort               ,
  N::Speak::PcPause                    ,
  N::Speak::PcPauseLong                ,
  N::Speak::PcGlottalStop              ,
  N::Speak::PcPauseLong                ,
  N::Speak::PcPauseLong              } ;

N::Phoneme:: Phoneme(void)
{
}

N::Phoneme:: Phoneme(const Phoneme & phoneme)
{
  assign ( phoneme ) ;
}

N::Phoneme::~Phoneme(void)
{
}

N::Phoneme & N::Phoneme::assign(const Phoneme & phoneme)
{
  nMemberCopy ( phoneme , Mnemonic  ) ;
  nMemberCopy ( phoneme , Flags     ) ;
  nMemberCopy ( phoneme , Program   ) ;
  nMemberCopy ( phoneme , Code      ) ;
  nMemberCopy ( phoneme , Type      ) ;
  nMemberCopy ( phoneme , StartType ) ;
  nMemberCopy ( phoneme , EndType   ) ;
  nMemberCopy ( phoneme , Length    ) ;
  nMemberCopy ( phoneme , LengthMod ) ;
  return ME                           ;
}

N::Phoneme & N::Phoneme::operator = (const Phoneme & phoneme)
{
  return assign ( phoneme ) ;
}

int N::Phoneme::packetSize(void)
{
  return sizeof(PHONEME_TAB) ;
}

QString N::Phoneme::MnemonicString (void)
{
  unsigned int word = Mnemonic                  ;
  int          ix                               ;
  char         buf [ 5 ]                        ;
  char       * p = buf                          ;
  for ( ix = 3 ; ix >= 0 ; ix-- )               {
    if ( ( *p = word >> ( ix * 8 ) ) != 0 ) p++ ;
  }                                             ;
  *p = 0                                        ;
  return QString(buf)                           ;
}

bool N::Phoneme::is(unsigned int mnemonic)
{
  return ( Mnemonic == mnemonic ) ;
}

bool N::Phoneme::isPause(void)
{
  return ( Type == Speak::PAUSE ) ;
}

bool N::Phoneme::isUnstressed(void)
{
  return ( ( Flags & Speak::UNSTRESSED ) > 0 ) ;
}

int N::Phoneme::set(int index,QByteArray & data)
{
  PHONEME_TAB * PHT = (PHONEME_TAB *)( data.data() + index ) ;
  #define MemberCopy(o,m) m = o->m
  MemberCopy ( PHT , Mnemonic  ) ;
  MemberCopy ( PHT , Flags     ) ;
  MemberCopy ( PHT , Program   ) ;
  MemberCopy ( PHT , Code      ) ;
  MemberCopy ( PHT , Type      ) ;
  MemberCopy ( PHT , StartType ) ;
  MemberCopy ( PHT , EndType   ) ;
  MemberCopy ( PHT , Length    ) ;
  MemberCopy ( PHT , LengthMod ) ;
  #undef MemberCopy
  return index + sizeof(PHONEME_TAB) ;
}

int N::Phoneme::Packet(QByteArray & data)
{
  PHONEME_TAB PHT                ;
  #define MemberCopy(o,m) o.m = m
  MemberCopy ( PHT , Mnemonic  ) ;
  MemberCopy ( PHT , Flags     ) ;
  MemberCopy ( PHT , Program   ) ;
  MemberCopy ( PHT , Code      ) ;
  MemberCopy ( PHT , Type      ) ;
  MemberCopy ( PHT , StartType ) ;
  MemberCopy ( PHT , EndType   ) ;
  MemberCopy ( PHT , Length    ) ;
  MemberCopy ( PHT , LengthMod ) ;
  #undef MemberCopy
  data.append((const char *)&PHT,sizeof(PHONEME_TAB)) ;
  return sizeof(PHONEME_TAB) ;
}

void N::Phoneme::report(void)
{
  N::printf(QString("Mnemonic = %1").arg(MnemonicString()),true) ;
  #define R(item) printf(#item " = %d\n",item)
  R ( Mnemonic  ) ;
  R ( Flags     ) ;
  R ( Program   ) ;
  R ( Code      ) ;
  R ( Type      ) ;
  R ( StartType ) ;
  R ( EndType   ) ;
  R ( Length    ) ;
  R ( LengthMod ) ;
  #undef  R
}
