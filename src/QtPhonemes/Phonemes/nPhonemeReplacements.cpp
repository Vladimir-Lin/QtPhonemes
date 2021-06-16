#include <qtphonemes.h>

N::PhonemeReplacements:: PhonemeReplacements         (void)
                       : QList<PhonemeReplacement *> (    )
{
}

N::PhonemeReplacements::~PhonemeReplacements(void)
{
}

void N::PhonemeReplacements::append(Phonemes & phonemes,char * string)
{
  int     n                                        ;
  int     phon                                     ;
  int     flags = 0                                ;
  char    phon1 [ 1024 ]                           ;
  char    phon2 [ 1024 ]                           ;
  QString S                                        ;
  //////////////////////////////////////////////////
  strcpy(phon2,"NULL")                             ;
  n = sscanf(string,"%d %s %s",&flags,phon1,phon2) ;
  if ( n < 2 ) return                              ;
  //////////////////////////////////////////////////
  S    = QString(phon1)                            ;
  phon = phonemes.Lookup(S)                        ;
  if (nEqual(phon,0)) return                       ;
  S    = QString(phon2)                            ;
  //////////////////////////////////////////////////
  PhonemeReplacement * replace                     ;
  replace        = new PhonemeReplacement ( )      ;
  replace -> Old  = phon                           ;
  replace -> New  = phonemes.Lookup(S)             ;
  replace -> Type = flags                          ;
  push_back ( replace )                            ;
}
