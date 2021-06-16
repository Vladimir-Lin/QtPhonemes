#include <qtphonemes.h>

enum             {
  V_NAME = 1     ,
  V_LANGUAGE     ,
  V_GENDER       ,
  V_TRANSLATOR   ,
  V_PHONEMES     ,
  V_DICTIONARY   ,
  V_FORMANT      ,
  V_PITCH        ,
  V_ECHO         ,
  V_FLUTTER      ,
  V_ROUGHNESS    ,
  V_CLARITY      ,
  V_TONE         ,
  V_VOICING      ,
  V_BREATH       ,
  V_BREATHW      ,
  V_WORDGAP      ,
  V_INTONATION   ,
  V_TUNES        ,
  V_STRESSLENGTH ,
  V_STRESSAMP    ,
  V_STRESSADD    ,
  V_DICTRULES    ,
  V_STRESSRULE   ,
  V_STRESSOPT    ,
  V_CHARSET      ,
  V_NUMBERS      ,
  V_OPTION       ,
  V_MBROLA       ,
  V_KLATT        ,
  V_FAST         ,
  V_SPEED        ,
  V_DICTMIN      ,
  V_ALPHABET2    ,
  V_REPLACE      ,
  V_CONSONANTS } ;

N::VoiceFeature:: VoiceFeature (void)
                : Name         (""  )
                , Appear       (""  )
                , Language     (""  )
                , Priority     (0   )
                , Gender       (1   )
                , Age          (20  )
                , Variant      (0   )
                , XX1          (0   )
                , Score        (0   )
                , Spare        (NULL)
{
}

N::VoiceFeature::~VoiceFeature (void)
{
}

QString N::VoiceFeature::Strip(QString Line)
{
  if (Line.length()<=0) return ""                      ;
  int length = Line.toUtf8().size()                    ;
  char * p = new char[length+1]                        ;
  memset(p,0,length+1)                                 ;
  p <= Line                                            ;
  //////////////////////////////////////////////////////
  if (p[0]=='#')                                       {
    delete [] p                                        ;
    return ""                                          ;
  }                                                    ;
  //////////////////////////////////////////////////////
  length = strlen(p)                                   ;
  while ( (--length > 0) && isspace ( p [ length ] ) ) {
    p[length] = 0                                      ;
  }                                                    ;
  //////////////////////////////////////////////////////
  char * cm = NULL                                     ;
  if ( ( cm = strstr(p,"//")) != NULL) *cm = 0         ;
  QString X(p)                                         ;
  delete [] p                                          ;
  return X                                             ;
}

bool N::VoiceFeature::Parse(int control)
{
  if (Data.size()<=0) return false         ;
  QString     S = QString::fromUtf8(Data)  ;
  QStringList L = S.split('\n')            ;
  bool        T = ( ( control & 2 ) == 2 ) ;
  L = File::PurifyLines(L)       ;
  foreach (S,L)                            {
    S = Strip(S)                           ;
    if (S.length()>0) Assign ( S , T )     ;
  }                                        ;
  return true                              ;
}

bool N::VoiceFeature::Assign(QString Line,bool toneOnly)
{
  QStringList P = Line.split(' ')                                       ;
  if (P.count()<1) return false                                         ;
  int key                                                               ;
  key = Acoustics::Lookup(Speak::KeywordTable,P[0])                     ;
  if (key<=0) return false                                              ;
  ///////////////////////////////////////////////////////////////////////
  switch ( key )                                                        {
    case V_NAME         : setName       ( Line , toneOnly ) ; break     ;
    case V_LANGUAGE     : setLanguage   ( Line            ) ; break     ;
    case V_GENDER       : setGender     ( Line            ) ; break     ;
    case V_TRANSLATOR   : setTranslator ( Line            ) ; break     ;
    case V_PHONEMES     : setPhoneme    ( Line            ) ; break     ;
    case V_DICTIONARY   : setDictionary ( Line            ) ; break     ;
    case V_FORMANT      : break                                         ;
    case V_PITCH        : break                                         ;
    case V_ECHO         : break                                         ;
    case V_FLUTTER      : break                                         ;
    case V_ROUGHNESS    : break                                         ;
    case V_CLARITY      : break                                         ;
    case V_TONE         : break                                         ;
    case V_VOICING      : break                                         ;
    case V_BREATH       : break                                         ;
    case V_BREATHW      : break                                         ;
    case V_WORDGAP      : break                                         ;
    case V_INTONATION   : break                                         ;
    case V_TUNES        : break                                         ;
    case V_STRESSLENGTH : break                                         ;
    case V_STRESSAMP    : break                                         ;
    case V_STRESSADD    : break                                         ;
    case V_DICTRULES    : break                                         ;
    case V_STRESSRULE   : break                                         ;
    case V_STRESSOPT    : break                                         ;
    case V_CHARSET      : break                                         ;
    case V_NUMBERS      : break                                         ;
    case V_OPTION       : break                                         ;
    case V_MBROLA       : break                                         ;
    case V_KLATT        : break                                         ;
    case V_FAST         : break                                         ;
    case V_SPEED        : break                                         ;
    case V_DICTMIN      : break                                         ;
    case V_ALPHABET2    : break                                         ;
    case V_REPLACE      : break                                         ;
    case V_CONSONANTS   : break                                         ;
    default             : break                                         ;
  }                                                                     ;
  return true                                                           ;
}

void N::VoiceFeature::setName(QString Line,bool toneOnly)
{
  QString     N = Line.simplified() ;
  QStringList L = N   .split(' ')   ;
  if (toneOnly   ) return           ;
  if (L.count()<2) return           ;
  Name = L[1]                       ;
}

void N::VoiceFeature::setLanguage(QString Line)
{
  QString     N = Line.simplified() ;
  QStringList L = N   .split(' ')   ;
  if (L.count()<2) return           ;
  if (Language.length()<=0)         {
    Language   = L[1]               ;
    Phoneme    = L[1]               ;
    Translator = L[1]               ;
    Dictionary = L[1]               ;
  }                                 ;
  if (!Languages.contains(L[1]))    {
    Languages << L[1]               ;
  }                                 ;
  if (L.count()<3) return           ;
  Priority = L[2].toInt()           ;
}

void N::VoiceFeature::setTranslator(QString Line)
{
  QString     N = Line.simplified() ;
  QStringList L = N   .split(' ')   ;
  if (L.count()<2) return           ;
  Translator = L[1]                 ;
}

void N::VoiceFeature::setPhoneme(QString Line)
{
  QString     N = Line.simplified() ;
  QStringList L = N   .split(' ')   ;
  if (L.count()<2) return           ;
  Phoneme = L[1]                    ;
}

void N::VoiceFeature::setDictionary(QString Line)
{
  QString     N = Line.simplified() ;
  QStringList L = N   .split(' ')   ;
  if (L.count()<2) return           ;
  Dictionary = L[1]                 ;
}

void N::VoiceFeature::setGender(QString Line)
{
  QString     N = Line.simplified()                                          ;
  QStringList L = N   .split(' ')                                            ;
  if (L.count()<3) return                                                    ;
  Gender = Acoustics::Lookup(Speak::MnemonicGender,L[1]) ;
  Age    = L[2].toInt()                                                      ;
}

bool N::VoiceFeature::is(QString name)
{
  return ( Name.toLower() == name.toLower() ) ;
}

QStringList N::VoiceFeature::PossiblePhonemes (void)
{
  QStringList phonemes         ;
  QString     S                ;
  if (Phoneme.length()>0)      {
    phonemes << Phoneme        ;
  }                            ;
  foreach (S,Languages)        {
    if (!phonemes.contains(S)) {
      phonemes << S            ;
    }                          ;
  }                            ;
  return phonemes              ;
}

QStringList N::VoiceFeature::PossibleDictionary (void)
{
  QStringList dictionary         ;
  QString     S                  ;
  if (Dictionary.length()>0)     {
    dictionary << Phoneme        ;
  }                              ;
  foreach (S,Languages)          {
    if (!dictionary.contains(S)) {
      dictionary << S            ;
    }                            ;
  }                              ;
  return dictionary              ;
}
