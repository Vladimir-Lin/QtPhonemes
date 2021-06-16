#include <qtphonemes.h>

N::LanguageOptions:: LanguageOptions(void)
{
}

N::LanguageOptions:: LanguageOptions(const LanguageOptions & options)
{
  assign ( options ) ;
}

N::LanguageOptions::~LanguageOptions(void)
{
}

N::LanguageOptions & N::LanguageOptions::assign(const LanguageOptions & options)
{
  nMemberCopy ( options , Id                   )                               ;
  nMemberCopy ( options , wordGap              )                               ;
  nMemberCopy ( options , vowelPause           )                               ;
  nMemberCopy ( options , stressRule           )                               ;
  nMemberCopy ( options , stressFlags          )                               ;
  nMemberCopy ( options , unstressedWD1        )                               ;
  nMemberCopy ( options , unstressedWD2        )                               ;
  nMemberCopy ( options , numbers              )                               ;
  nMemberCopy ( options , numbers2             )                               ;
  nMemberCopy ( options , breakNumbers         )                               ;
  nMemberCopy ( options , maxRoman             )                               ;
  nMemberCopy ( options , minRoman             )                               ;
  nMemberCopy ( options , thousandsSep         )                               ;
  nMemberCopy ( options , decimalSep           )                               ;
  nMemberCopy ( options , maxDigits            )                               ;
  nMemberCopy ( options , accents              )                               ;
  nMemberCopy ( options , toneLanguage         )                               ;
  nMemberCopy ( options , intonationGroup      )                               ;
  nMemberCopy ( options , longStop             )                               ;
  nMemberCopy ( options , phonemeChange        )                               ;
  nMemberCopy ( options , testing              )                               ;
  nMemberCopy ( options , listx                )                               ;
  nMemberCopy ( options , ourAlphabet          )                               ;
  nMemberCopy ( options , altAlphabet          )                               ;
  nMemberCopy ( options , altAlphabetLanguage  )                               ;
  nMemberCopy ( options , maxLengthMod         )                               ;
  nMemberCopy ( options , lengthenTonic        )                               ;
  nMemberCopy ( options , suffixAddE           )                               ;
  nMemberCopy ( options , maxInitialConsonants )                               ;
  nMemberCopy ( options , spellingStress       )                               ;
  nMemberCopy ( options , toneNumbers          )                               ;
  nMemberCopy ( options , ideographs           )                               ;
  nMemberCopy ( options , textmode             )                               ;
  nMemberCopy ( options , dotlessI             )                               ;
  memcpy(param        ,options.param        ,(sizeof(int)*MaxLanguageOptions)) ;
  memcpy(param2       ,options.param2       ,(sizeof(int)*MaxLanguageOptions)) ;
  memcpy(tunes        ,options.tunes        ,(sizeof(char)*6)                ) ;
  memcpy(asciiLanguage,options.asciiLanguage,(sizeof(char)*8)                ) ;
//  unsigned char      * lengthMods          ;
//  unsigned char      * lengthMods0         ;
//  const char         * ordinalIndicator    ;
//  const unsigned int * replaceChars        ;
  return ME                           ;
}

N::LanguageOptions & N::LanguageOptions::operator = (const LanguageOptions & options)
{
  return assign ( options ) ;
}
