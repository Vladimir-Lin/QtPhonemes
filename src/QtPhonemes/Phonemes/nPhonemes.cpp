#include <qtphonemes.h>

N::Phonemes:: Phonemes            (void)
            : QMap<int,Phoneme *> (    )
{
}

N::Phonemes::~Phonemes (void)
{
}

N::Phonemes & N::Phonemes::operator += (const Phonemes & phoneme)
{
  CUIDs K = phoneme.keys()     ;
  int   k                      ;
  foreach (k,K)                {
    int c = phoneme[k] -> Code ;
    ME[c] = phoneme[k]         ;
  }                            ;
  return ME                    ;
}

bool N::Phonemes::is(QString name)
{
  return ( Name.toLower() == name.toLower() ) ;
}

int N::Phonemes::Code (unsigned int Mnemonic)
{
  CUIDs K = keys()             ;
  int   i                      ;
  foreach (i,K)                {
    if (ME[i] -> is(Mnemonic)) {
      return ME[i] -> Code     ;
    }                          ;
  }                            ;
  return 0                     ;
}

int N::Phonemes::Code(char a,char b)
{
  unsigned int c = a ;
  c <<= 8            ;
  c  += b            ;
  return Code(c)     ;
}

int N::Phonemes::Lookup(QString code)
{
  unsigned int  Mnemonic = 0            ;
  QString       R        = code.left(4) ;
  unsigned char c                       ;
  for (int i=0;i<R.length();i++)        {
    c = R.at(i).toLatin1()              ;
    Mnemonic |= ( c << ( i * 8 ) )      ;
  }                                     ;
  return Code ( Mnemonic )              ;
}

int N::Phonemes::set(int index,QByteArray & data)
{
  unsigned char * p = (unsigned char *)data.data() ;
  char            name [ 32 ]                      ;
  p = & p [ index ]                                ;
  int phonemes = (int)p[0]                         ;
  Includes     = (int)p[1]                         ;
  Equivalence  = (int)((int *)p)[1]                ;
  index       += 8                                 ;
  memset ( name , 0     , 32 )                     ;
  memcpy ( name , &p[8] , 32 )                     ;
  Name = QString::fromUtf8(name)                   ;
  index       += 32                                ;
  for (int i=0;i<phonemes;i++)                     {
    Phoneme * phoneme                              ;
    phoneme = new Phoneme ( )                      ;
    index   = phoneme -> set(index,data)           ;
    ME[i]   = phoneme                              ;
  }                                                ;
  return index                                     ;
}

int N::Phonemes::Packet(QByteArray & data)
{
  QByteArray Block                                  ;
  QByteArray N = Name.toUtf8()                      ;
  int        L = N.size()                           ;
  Block.resize(40)                                  ;
  unsigned char * p = (unsigned char *)Block.data() ;
  memset(p,0,40)                                    ;
  ///////////////////////////////////////////////////
  p[0]          = (unsigned char)count()            ;
  p[1]          = (unsigned char)Includes           ;
  ((int *)p)[1] = Equivalence                       ;
  ///////////////////////////////////////////////////
  if (L>31) L = 31                                  ;
  if (L>0)                                          {
    char * n = (char *)N.data()                     ;
    memcpy(&p[8],n,L)                               ;
  }                                                 ;
  ///////////////////////////////////////////////////
  CUIDs K = keys()                                  ;
  int   k                                           ;
  foreach (k,K) ME[k] -> Packet(data)               ;
  ///////////////////////////////////////////////////
  data.append(Block)                                ;
  return data . size ( )                            ;
}

int N::Phonemes::Syllables(unsigned char * phonemes)
{
  int count = 0                             ;
  int phon                                  ;
  while ( ( phon = *phonemes++ ) != 0 )     {
    if (contains(phon))                     {
      if (ME[phon] -> Type == Speak::VOWEL) {
        count++                             ;
      }                                     ;
    }                                       ;
  }                                         ;
  return count                              ;
}

int N::Phonemes::toMnemonics(const char * phonemes,char * mnemonics)
{
  unsigned char       phcode                                ;
  unsigned char       c                                     ;
  unsigned int        mnem                                  ;
  int                 mlen = 0                              ;
  static const char * stress_chars = "==,,'*  "             ;
  while ( ( phcode = *phonemes++) > 0 )                     {
    if ( phcode == 255        ) continue                    ;
    if (! contains ( phcode ) ) continue                    ;
    if ( ( ME[phcode] -> Type    == Speak::STRESS)         &&
         ( ME[phcode] -> Length  <= 4                   )  &&
         ( ME[phcode] -> Program == 0                   ) ) {
      int len = ME[phcode] -> Length                        ;
      if ( len >  1)                                        {
        *mnemonics++ = stress_chars[len]                    ;
        mlen++                                              ;
      }                                                     ;
    } else                                                  {
      mnem = ME[phcode] -> Mnemonic                         ;
      while ( ( c = ( mnem & 0xff ) ) != 0 )                {
        *mnemonics++ = c                                    ;
        mnem         = mnem >> 8                            ;
        mlen++                                              ;
      }                                                     ;
      if ( phcode == Speak::PcSwitch )            {
         while ( isalpha(*phonemes) )                       {
           *mnemonics++ = *phonemes++                       ;
           mlen++                                           ;
         }                                                  ;
      }                                                     ;
    }                                                       ;
  }                                                         ;
  *mnemonics = 0                                            ;
  return mlen                                               ;
}

int N::Phonemes::toPhonemes(const char * mnemonics,char * phonemes,unsigned char * bad)
{
  int            ix                                                        ;
  unsigned char  c                                                         ;
  int            count         ; /* num. of matching characters */
  int            max           ; /* highest num. of matching found so far */
  int            max_ph        ; /* corresponding phoneme with highest matching */
  int            consumed                                                  ;
  unsigned int   mnemonic_word                                             ;
  int            mlen = 0                                                  ;
  const char *   p    = mnemonics                                          ;
  CUIDs          K    = keys()                                             ;
  //////////////////////////////////////////////////////////////////////////
  if ( NotNull ( bad ) ) *bad = 0                                          ;
  while ( isspace(*p) ) p++                                                ;
  while ( ( ( c = (*p) ) != 0 ) && !isspace(c) )                           {
    consumed = 0                                                           ;
    switch ( c )                                                           {
      case '|'                                                             :
        // used to separate phoneme mnemonics if needed,
        // to prevent characters being treated as a multi-letter mnemonic
        if ( ( c = p[1] ) == '|')                                          {
          // treat double || as a word-break symbol, drop through
          // to the default case with c = '|'
        } else                                                             {
          p++                                                              ;
          break                                                            ;
        }                                                                  ;
      default                                                              :
        // lookup the phoneme mnemonic, find the phoneme with
        // the highest number of matching characters
        max    = -1                                                        ;
        max_ph =  0                                                        ;
        for ( int i = 0 ; i < K.count() ; i++ )                            {
          ix = K[i]                                                        ;
          if (ME[ix] -> Type==Speak::INVALID) continue                     ;
          count         = 0                                                ;
          mnemonic_word = ME[ix] -> Mnemonic                               ;
          while ( ( ( c = p[count] ) > ' ') && ( count < 4 )              &&
                  ( c == ( ( mnemonic_word >> ( count * 8 ) ) & 0xff ) ) ) {
            count++                                                        ;
          }                                                                ;
          if ( ( count > max ) && ( ( count == 4 )                        ||
               ( ( ( mnemonic_word >> ( count * 8 ) ) & 0xff ) == 0 ) ) )  {
            max    = count                                                 ;
            max_ph = ME[ix] -> Code                                        ;
          }                                                                ;
        }                                                                  ;
        if ( max_ph == 0 )                                                 {
          // not recognised, report and ignore
          if ( NotNull(bad) )                                              {
            bad[0] = *p                                                    ;
            bad[1] = 0                                                     ;
          }                                                                ;
          *phonemes++ = 0                                                  ;
          return mlen                                                      ;
        }                                                                  ;
        if ( max <= 0 ) max = 1                                            ;
        p += ( consumed + max )                                            ;
        *phonemes++ = (char)(max_ph)                                       ;
        if (  max_ph == Speak::PcSwitch )                        {
          // Switch Language: this phoneme is followed by a text string
          char * p_lang = phonemes                                         ;
          while ( !isspace( c = (*p) ) && ( c != 0 ) )                     {
            p++                                                            ;
            *phonemes++ = tolower(c)                                       ;
            mlen++                                                         ;
          }                                                                ;
          *phonemes = 0                                                    ;
          if ( c == 0 )                                                    {
            if ( strcmp ( p_lang , "en" ) == 0 )                           {
              *p_lang = 0                                                  ;
              // don't need "en", it's assumed by default
              return mlen                                                  ;
            }                                                              ;
          } else                                                           {
            *phonemes++ = '|'                                              ;
            mlen++                                                         ;
            // more phonemes follow, terminate language string with separator
          }                                                                ;
        }                                                                  ;
        break                                                              ;
      }                                                                    ;
    break                                                                  ;
  }                                                                        ;
  /* terminate the encoded string */
  *phonemes = 0                                                            ;
  return mlen                                                              ;
}

void N::Phonemes::append             (
       SpeechTranslator & translator ,
       const char       * phoneme    ,
       char             * string     ,
       int                size       )
{
  /* Add new phoneme string "ph" to "string"
     Keeps count of the number of vowel phonemes in the word, and whether these
     can be stressed syllables.  These values can be used in translation rules
   */
  const char   * p = phoneme                                      ;
  unsigned char  c                                                ;
  int            unstress_mark = 0                                ;
  int            length        = strlen(phoneme) + strlen(string) ;
  if ( length >= size ) return                                    ;
  /////////////////////////////////////////////////////////////////
  /* any stressable vowel ?                                      */
  while ( ( c = *p++ ) != 0 )                                     {
    if ( !contains(c) ) continue                                  ;
    if ( ME[c] -> Type == Speak::STRESS )                         {
      if ( ME[c] -> Length < 4 ) unstress_mark = 1                ;
    } else                                                        {
      if ( ME[c] -> Type == Speak::VOWEL )                        {
        if ( ( ( ME[c] -> Flags & Speak::UNSTRESSED ) == 0 )     &&
             ( unstress_mark == 0 ) )                             {
           translator.wordStressedCount++                         ;
        }                                                         ;
        unstress_mark = 0                                         ;
        translator.wordVowelCount++                               ;
      }                                                           ;
    }                                                             ;
  }                                                               ;
  if ( NotNull(string) ) strcat(string,phoneme)                   ;
}

QString N::Phonemes::toString(unsigned int mnemonic)
{ // Convert a language mnemonic word into a string
  char   buf[5]                                   ;
  char * p = buf                                  ;
  /////////////////////////////////////////////////
  for (int i = 3 ; i >= 0 ; i-- )                 {
    if ( ( ( *p = mnemonic >> (i*8)) ) != 0 ) p++ ;
  }                                               ;
  *p = 0                                          ;
  return QString ( buf )                          ;
}

unsigned int N::Phonemes::toWord(const char * string)
{ // Convert a language name string to a word such as L('e','n')
  int          ix                                                   ;
  int          c                                                    ;
  unsigned int value = 0                                            ;
  for ( ix = 0 ; ( ix < 4 ) && ( ( c = string[ix] ) != 0 ) ; ix++ ) {
    value = (value << 8) | (c & 0xff)                               ;
  }                                                                 ;
  return value                                                      ;
}

void N::Phonemes::report(void)
{
  printf("================================\n") ;
  N :: printf ( Name , true )                  ;
  #define R(item) printf(#item " = %d\n",item)
  R ( Includes    ) ;
  R ( Equivalence ) ;
  #undef  R
  //////////////////////////////////////////////
  CUIDs K = keys()                             ;
  int   k                                      ;
  foreach (k,K)                                {
    printf("==============================\n") ;
    ME[k] -> report()                          ;
  }                                            ;
}

int N::Acoustics::BuildPhonemes  (
      QList<Phonemes> & phonemes ,
      QByteArray      & data     )
{
  unsigned char * p     = (unsigned char *)data.data() ;
  int             index = 4                            ;
  int             n     = (int)p[0]                    ;
  for (int i=0;i<n;i++)                                {
    Phonemes phs                                       ;
    int      cnt = phonemes.count()                    ;
    phonemes << phs                                    ;
    index = phonemes[cnt].set(index,data)              ;
  }                                                    ;
  return phonemes.count()                              ;
}
