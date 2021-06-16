#include <qtphonemes.h>

extern int           IntonationSize     ;
extern unsigned char Intonations    [ ] ;
extern int           PhonemeTableSize   ;
extern unsigned char PhonemeTable   [ ] ;

bool N::Acoustics::DefaultIntonations(QByteArray & Intonation)
{
  Intonation . clear  (                                            ) ;
  Intonation . append ( (const char *)Intonations , IntonationSize ) ;
  return              ( Intonation.size() > 0                      ) ;
}

bool N::Acoustics::DefaultPhonemes(QByteArray & phoneme)
{
  phoneme . clear  (                                               ) ;
  phoneme . append ( (const char *)PhonemeTable , PhonemeTableSize ) ;
  return           ( phoneme.size() > 0                            ) ;
}
