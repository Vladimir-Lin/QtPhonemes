#include <qtphonemes.h>

// instructions

#define i_RETURN        0x0001
#define i_CONTINUE      0x0002
#define i_NOT           0x0003

// Group 0 instrcutions with 8 bit operand.  These values go into bits 8-15 of the instruction
#define i_CHANGE_PHONEME 0x01
#define i_REPLACE_NEXT_PHONEME 0x02
#define i_INSERT_PHONEME 0x03
#define i_APPEND_PHONEME 0x04
#define i_APPEND_IFNEXTVOWEL 0x05
#define i_VOICING_SWITCH 0x06
#define i_PAUSE_BEFORE   0x07
#define i_PAUSE_AFTER    0x08
#define i_LENGTH_MOD     0x09
#define i_SET_LENGTH     0x0a
#define i_LONG_LENGTH    0x0b
#define i_CHANGE_PHONEME2 0x0c  // not yet used
#define i_IPA_NAME       0x0d

#define i_CHANGE_IF      0x10  // 0x10 to 0x14

#define i_ADD_LENGTH     0x0c


// conditions and jumps
#define i_CONDITION  0x2000
#define i_OR         0x1000  // added to i_CONDITION

#define i_JUMP       0x6000
#define i_JUMP_FALSE 0x6800
#define i_SWITCH_NEXTVOWEL 0x6a00
#define i_SWITCH_PREVVOWEL 0x6c00
#define MAX_JUMP     255  // max jump distance

// multi-word instructions
#define i_CALLPH     0x9100
#define i_PITCHENV   0x9200
#define i_AMPENV     0x9300
#define i_VOWELIN    0xa100
#define i_VOWELOUT   0xa200
#define i_FMT        0xb000
#define i_WAV        0xc000
#define i_VWLSTART   0xd000
#define i_VWLENDING  0xe000
#define i_WAVADD     0xf000

// conditions
#define i_isDiminished 0x80
#define i_isUnstressed 0x81
#define i_isNotStressed 0x82
#define i_isStressed   0x83
#define i_isMaxStress  0x84

#define i_isBreak      0x85
#define i_isWordStart  0x86
#define i_notWordStart 0x87
#define i_isWordEnd    0x88
#define i_isAfterStress 0x89
#define i_isNotVowel   0x8a
#define i_isFinalVowel 0x8b
#define i_isVoiced     0x8c
#define i_isFirstVowel 0x8d
#define i_isSecondVowel 0x8e
#define i_isSeqFlag1   0x8f
#define i_IsTranslationGiven 0x90


// place of articulation
#define i_isVel      0x28

// phflags
#define i_isSibilant   0x45    // bit 5 in phflags
#define i_isPalatal    0x49    // bit 9 in phflags
#define i_isRhotic     0x57    // bit 23 in phflags
#define i_isFlag1      0x5c
#define i_isFlag2      0x5d
#define i_isFlag3      0x5e

#define i_StressLevel  0x800

#define pd_FMT    0
#define pd_WAV    1
#define pd_VWLSTART 2
#define pd_VWLEND 3
#define pd_ADDWAV 4

#define pd_INSERTPHONEME   i_INSERT_PHONEME
#define pd_APPENDPHONEME   i_APPEND_PHONEME
#define pd_CHANGEPHONEME   i_CHANGE_PHONEME
#define pd_LENGTHMOD       i_SET_LENGTH

#define pd_FORNEXTPH     0x2
#define pd_DONTLENGTHEN  0x4
#define pd_REDUCELENGTHCHANGE 0x8

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

// pitch,speed
static int embedded_default[16] = {0,    50,175,100, 50,  0,  0,  0,175,0,0,0,0,0,0,0} ;
static int embedded_max    [16] = {0,0x7fff,750,300, 99, 99, 99,  0,750,0,0,0,0,4,0,0} ;
static const unsigned char amp_emphasis[5] = {16, 16, 10, 16, 22};
extern int           phonemeDataSize    ;
extern unsigned char phonemeData    [ ] ;
extern int           PhonemeIndexSize   ;
extern unsigned char PhonemeIndex   [ ] ;
static const int param_defaults[16] = {
    0 , // silence (internal use)
  175 , // rate wpm
  100 , // volume
   50 , // pitch
   50 , // range
    0 , // punctuation
    0 , // capital letters
    0 , // wordgap
    0 , // options
    0 , // intonation
    0 ,
    0 ,
    0 , // emphasis
    0 , // line length
    0 , // voice type
    0
};

N::Synthesizer:: Synthesizer       (void    )
               : Speed1            (130     )
               , Speed2            (121     )
               , Speed3            (118     )
               , Version           (0x014709)
               , SampleRate        (22050   )
               , samplerate_native (0       )
               , samplerate_base   (22050   )
               , Amplitude         (60      )
{
  ParamentStack * stack                                           ;
  stack = new ParamentStack ( )                                   ;
  ::memcpy ( Values , embedded_default , sizeof(int ) *  16     ) ;
  ::memcpy ( Max    , embedded_max     , sizeof(int ) *  16     ) ;
  Data  . clear  (                                              ) ;
  Data  . append ( (const char *)phonemeData ,phonemeDataSize   ) ;
  Index . clear  (                                              ) ;
  Index . append ( (const char *)PhonemeIndex,PhonemeIndexSize  ) ;
  Paraments << stack                                              ;
  ::memcpy(Paraments[0]->parameter,param_defaults,sizeof(int)*16 ) ;
  int * piv = (int *)Data.data()                                  ;
  Version    = piv[0]                                             ;
  SampleRate = piv[1]                                             ;
}

N::Synthesizer:: Synthesizer(const Synthesizer & synthesizer)
{
  assign ( synthesizer ) ;
}

N::Synthesizer::~Synthesizer(void)
{
}

N::Synthesizer & N::Synthesizer::operator = (const Synthesizer & synthesizer)
{
  return assign(synthesizer) ;
}

N::Synthesizer & N::Synthesizer::assign(const Synthesizer & synthesizer)
{
  nMemberCopy ( synthesizer , WCMDQ             )                   ;
  nMemberCopy ( synthesizer , Speed1            )                   ;
  nMemberCopy ( synthesizer , Speed2            )                   ;
  nMemberCopy ( synthesizer , Speed3            )                   ;
  nMemberCopy ( synthesizer , samplerate_native )                   ;
  nMemberCopy ( synthesizer , samplerate_base   )                   ;
  ::memcpy ( Values , synthesizer.Values , sizeof(int ) *  16     ) ;
  ::memcpy ( Max    , synthesizer.Max    , sizeof(int ) *  16     ) ;
  return ME                                                         ;
}

void N::Synthesizer::setHome(QString speechHome)
{
  QDir D(speechHome)                                                    ;
  Directories [ Speech        ] = D                                     ;
  Directories [ Dictionary    ] = D . absoluteFilePath ( "dictionary" ) ;
  Directories [ Voices        ] = D . absoluteFilePath ( "voices"     ) ;
  Directories [ Mbrola        ] = D . absoluteFilePath ( "mbrola"     ) ;
  Directories [ MbrolaPhoneme ] = D . absoluteFilePath ( "mbrolaph"   ) ;
  Directories [ Sources       ] = D . absoluteFilePath ( "sources"    ) ;
}

QString N::Synthesizer::Path(int catalog,QString filename)
{
  return Directories[catalog].absoluteFilePath(filename) ;
}

void N::Synthesizer::SonicSpeed(int value)
{ // value, multiplier * 1024
  VarArgs args        ;
  args << SONIC_SPEED ;
  args << value       ;
  WCMDQ << args       ;
}

void N::Synthesizer::ParseData(void)
{
  unsigned char * wav = (unsigned char *)Data.data() ;
  Version    = 0                                     ;
  SampleRate = 0                                     ;
  for (int i=0;i<4;i++)                              {
    Version    += ( wav[i  ] << ( i * 8 ) )          ;
    SampleRate += ( wav[i+4] << ( i * 8 ) )          ;
  }                                                  ;
}

int N::Synthesizer::GetAmplitude(void)
{
  int amplitude = ( Values[EMBED_A] * 55 ) / 100        ;
  int factor    =   Values[EMBED_F]                     ;
  Amplitude = ( amplitude * amp_emphasis[factor] ) / 16 ;
  return Amplitude                                      ;
}

QStringList N::Synthesizer::SupportedVoices(void)
{
  QStringList V                        ;
  for (int i=0;i<Features.count();i++) {
    QString F = Features[i].Name       ;
    if (F.length()>0) V << F           ;
  }                                    ;
  return V                             ;
}

int N::Synthesizer::InstallWords(unsigned short * prog)
{
  static const char n_words[16] = {0,1,0,0,1,1,0,1,1,2,4,0,0,0,0,0} ;
  int               instn                                           ;
  int               instn2                                          ;
  int               instn_type                                      ;
  int               n                                               ;
  int               type2                                           ;
  ///////////////////////////////////////////////////////////////////
  instn      = *prog                                                ;
  instn_type = instn >> 12                                          ;
  if ( ( n = n_words[instn_type] ) > 0) return n                    ;
  ///////////////////////////////////////////////////////////////////
  switch ( instn_type )                                             {
    case 0                                                          :
      if ( ( ( instn & 0xf00 ) >> 8 ) == i_IPA_NAME )               {
        n = ( ( instn & 0xff ) + 1 ) / 2                            ;
        return ( n + 1 )                                            ;
      }                                                             ;
    return 1                                                        ;
    case 6                                                          :
      type2 = ( instn & 0xf00 ) >> 9                                ;
      if ( ( type2 == 5 ) || ( type2 == 6 ) ) return 12             ; // switch on vowel type
    return 1                                                        ;
    case 2                                                          :
    case 3                                                          :
      // a condition, check for a 2-word instruction
      if ( ( ( n = instn & 0x0f00 ) == 0x600) || ( n == 0x0d00 ) )  {
        return 2                                                    ;
      }                                                             ;
    return 1                                                        ;
    default                                                         :
      // instn_type 11 to 15, 2 words
      instn2 = prog[2]                                              ;
      if ( ( instn2 >> 12 ) == 0xf )                                {
        // This instruction is followed by addWav(), 2 more words
        return 4                                                    ;
      }                                                             ;
      if ( instn2 == i_CONTINUE ) return 3                          ;
      return 2                                                      ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  return 2                                                          ;
}

bool N::Synthesizer::containsPhoneme(QString phoneme)
{
  for (int i=0;i<phonemes.count();i++)       {
    if (phonemes[i].is(phoneme)) return true ;
  }                                          ;
  return false                               ;
}

int N::Synthesizer::PickPhoneme(QStringList ps)
{
  QString S                                ;
  foreach (S,ps)                           {
    if (containsPhoneme(S))                {
      for (int i=0;i<phonemes.count();i++) {
        if (phonemes[i].is(S)) return i    ;
      }                                    ;
    }                                      ;
  }                                        ;
  return -1                                ;
}

int N::Synthesizer::PickDictionary(QStringList Dictionaries)
{
  QString S                                 ;
  foreach (S,Dictionaries)                  {
    for (int i=0;i<Translators.count();i++) {
      if (Translators[i].is(S)) return i    ;
    }                                       ;
  }                                         ;
  return -1                                 ;
}

bool N::Synthesizer::Initialize(void)
{
  /////////////////////////////////////////////////////////////////////
  /*                        Load Phoneme Table                       */
  /////////////////////////////////////////////////////////////////////
  QString phontab = Directories[Speech].absoluteFilePath("phontab")   ;
  QFileInfo F(phontab)                                                ;
  QByteArray phtab                                                    ;
  if (F.exists())                                                     {
    File      :: toByteArray     ( phontab , phtab )                  ;
  } else                                                              {
    Acoustics :: DefaultPhonemes ( phtab           )                  ;
  }                                                                   ;
  if (phtab.size()>0)                                                 {
    int * pht   = (int *)phtab.data()                                 ;
    int   total = pht[0]                                              ;
    int   index = 4                                                   ;
    for (int i=0;i<total;i++)                                         {
      Phonemes ps                                                     ;
      phonemes << ps                                                  ;
      index = phonemes[i].set(index,phtab)                            ;
    }                                                                 ;
    phtab . clear ( )                                                 ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  /*                        Load Phoneme Data                        */
  /////////////////////////////////////////////////////////////////////
  QString phdata = Directories[Speech].absoluteFilePath("phondata")   ;
  QFileInfo FD(phdata)                                                ;
  if (FD.exists())                                                    {
    File::toByteArray(phdata,Data)                                    ;
    if (Data.size()>0)                                                {
      int * piv = (int *)Data.data()                                  ;
      Version    = piv[0]                                             ;
      SampleRate = piv[1]                                             ;
    }                                                                 ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  /*                        Load Phoneme Index                       */
  /////////////////////////////////////////////////////////////////////
  QString phi = Directories[Speech].absoluteFilePath("phonindex")     ;
  QFileInfo FI(phi)                                                   ;
  if (FI.exists()) File::toByteArray(phi,Index)                       ;
  /////////////////////////////////////////////////////////////////////
  /*                         Load Intonations                        */
  /////////////////////////////////////////////////////////////////////
  QString inn = Directories[Speech].absoluteFilePath("intonations")   ;
  QFileInfo FN(phi)                                                   ;
  if (FN.exists())                                                    {
    QByteArray TN                                                     ;
    File::toByteArray(inn,TN)                                         ;
    if (TN.size()>0) Tunes . append ( TN )                            ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  /*                      Load Dictionary files                      */
  /////////////////////////////////////////////////////////////////////
  QDir          B  = Directories[Dictionary]                          ;
  QFileInfoList Bs = B.entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
  QStringList   Ps                                                    ;
  for (int i=0;i<Bs.count();i++)                                      {
    QString n = Bs[i].fileName()                                      ;
    if (!Ps.contains(n)) Ps << n                                      ;
  }                                                                   ;
  for (int i=0;i<Ps.count();i++)                                      {
    QString   name = Ps[i]                                            ;
    QString   file = Directories[Dictionary].absoluteFilePath(name)   ;
    QFileInfo F(file)                                                 ;
    SpeechTranslator translator                                       ;
    int               index = Translators.count()                     ;
    Translators << translator                                         ;
    if (F.exists())                                                   {
      QByteArray D                                                    ;
      if (File::toByteArray(file,D))                                  {
        Translators[index].dataDictList = D                           ;
        Translators[index].Install(name)                              ;
      } else                                                          {
        Translators.takeAt(index)                                     ;
      }                                                               ;
    }                                                                 ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  QDir          D  = Directories[Voices]                              ;
  QFileInfoList Ds = D.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot) ;
  QList<QDir>   Ls                                                    ;
  QStringList   Fs                                                    ;
  Ls << D                                                             ;
  if (Ds.count()>0)                                                   {
    for (int i=0;i<Ds.count();i++) Ls << Ds[i].absoluteFilePath()     ;
  }                                                                   ;
  for (int i=0;i<Ls.count();i++)                                      {
    D  = Ls[i]                                                        ;
    Ds = D.entryInfoList(QDir::Files|QDir::NoDotAndDotDot)            ;
    if (Ds.count()>0)                                                 {
      for (int j=0;j<Ds.count();j++) Fs << Ds[j].absoluteFilePath()   ;
    }                                                                 ;
  }                                                                   ;
  for (int i=0;i<Fs.count();i++)                                      {
    QString      Filename = Fs[i]                                     ;
    QFileInfo    FI(Filename)                                         ;
    QString      Name = FI.fileName()                                 ;
    VoiceFeature feature                                              ;
    int          index = Features.count()                             ;
    Features << feature                                               ;
    if (File::toByteArray(Filename,Features[index].Data))             {
      Features[index].Appear = Name                                   ;
      if (!Features[index].Parse(0))                                  {
        Features.takeAt(index)                                        ;
      }                                                               ;
    } else                                                            {
      Features.takeAt(index)                                          ;
    }                                                                 ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  return true                                                         ;
}

bool N::Synthesizer::InstallPhoneme(int translator,int phoneme,bool recursing)
{
  int Includes = phonemes[phoneme].Includes                      ;
  if ( Includes > 0 ) InstallPhoneme(translator,Includes-1,true) ;
  Translators[translator].phonemes += phonemes[phoneme]          ;
  return true                                                    ;
}

int N::Synthesizer::Pick(int Priority,QString Voice)
{
  int feature       = -1                                                ;
  for (int i=0;feature<0 && i<Features.count();i++)                     {
    if (Features[i].is(Voice)) feature = i                              ;
  }                                                                     ;
  if (feature<0) return Supports.count()                                ;
  ///////////////////////////////////////////////////////////////////////
  QStringList SLS       = Features[feature].Languages                   ;
  QStringList dicts     = Features[feature].PossibleDictionary()        ;
  int         translate = PickDictionary(dicts)                         ;
  SpeechTranslator nst                                                  ;
  int               nid = Translators.count()                           ;
  Translators << nst                                                    ;
  bool done = false                                                     ;
  for (int i=0;!done && SLS.count();i++)                                {
    QString LSS = SLS[i]                                                ;
    done = Translators[nid].Select(LSS.toUtf8().constData())            ;
  }                                                                     ;
  if (!done)                                                            {
    Translators.takeAt(nid)                                             ;
    return Supports.count()                                             ;
  }                                                                     ;
  if (translate>=0)                                                     {
    Translators[nid].dataDictList = Translators[translate].dataDictList ;
    if (!Translators[nid].Load())                                       {
      return Supports.count()                                           ;
    }                                                                   ;
  }                                                                     ;
  translate = nid                                                       ;
  ///////////////////////////////////////////////////////////////////////
  int phoneme = PickPhoneme(Features[feature].PossiblePhonemes())       ;
  if (phoneme<0) return Supports.count()                                ;
  InstallPhoneme(translate,phoneme,false)                               ;
  ///////////////////////////////////////////////////////////////////////
  WaveSynthesis     ws                                                  ;
  WaveGenerator     wg                                                  ;
  VoicePacket       vp                                                  ;
  SonicPitch        sp                                                  ;
  FormantParament * fp = new FormantParament ( )                        ;
  PhonemeData       pd                                                  ;
  SpeechFactors     sf                                                  ;
  FeatureMap    [ Priority ] = feature                                  ;
  TranslatorMap [ Priority ] = translate                                ;
  VoiceMap      [ Priority ] = vp                                       ;
  SynthMap      [ Priority ] = ws                                       ;
  WaveMap       [ Priority ] = wg                                       ;
  SonicMap      [ Priority ] = sp                                       ;
  DataMap       [ Priority ] = pd                                       ;
  Formants      [ Priority ] = fp                                       ;
  Factors       [ Priority ] = sf                                       ;
  Supports      [ Priority ] = Voice                                    ;
    Parse ( Priority , 0 ) ;
  return Supports.count()                                               ;
}

bool N::Synthesizer::Parse(int Priority,int control)
{
  if ( ! FeatureMap    . contains(Priority) ) return false  ;
  if ( ! TranslatorMap . contains(Priority) ) return false  ;
  int feature = FeatureMap[Priority]                        ;
  if (Features[feature].Data.size()<=0) return false        ;
  QString     S = QString::fromUtf8(Features[feature].Data) ;
  QStringList L = S.split('\n')                             ;
  L = File::PurifyLines(L)                                  ;
  foreach (S,L)                                             {
    S = Features[feature].Strip(S)                          ;
    if (S.length()>0) Assign ( Priority , S , control )     ;
  }                                                         ;
  return true                                               ;
}

bool N::Synthesizer::Assign(int Priority,QString Line,int control)
{
  QStringList P = Line.split(' ')                                       ;
  if (P.count()<1) return false                                         ;
  int key                                                               ;
  key = Acoustics::Lookup(Speak::KeywordTable,P[0])                     ;
  if (key<=0) return false                                              ;
  bool toneOnly = ( ( control & 2 ) == 2 )                              ;
  int feature   = FeatureMap    [ Priority ]                            ;
  int translate = TranslatorMap [ Priority ]                            ;
  ///////////////////////////////////////////////////////////////////////
  switch ( key )                                                        {
    case V_NAME                                                         :
      Features [ feature  ] . setName       ( Line , toneOnly )         ;
    break                                                               ;
    case V_LANGUAGE                                                     :
      Features [ feature  ] . setLanguage   ( Line            )         ;
    break                                                               ;
    case V_GENDER                                                       :
      Features [ feature  ] . setGender     ( Line            )         ;
    break                                                               ;
    case V_TRANSLATOR                                                   :
      Features [ feature  ] . setTranslator ( Line            )         ;
    break                                                               ;
    case V_PHONEMES                                                     :
      Features [ feature  ] . setPhoneme    ( Line            )         ;
    break                                                               ;
    case V_DICTIONARY                                                   :
      Features [ feature  ] . setDictionary ( Line            )         ;
    break                                                               ;
    case V_FORMANT                                                      :
      VoiceMap [ Priority ] . setFormant    ( Line            )         ;
    break                                                               ;
    case V_PITCH                                                        :
      VoiceMap [ Priority ] . setPitch      ( Line            )         ;
    break                                                               ;
    case V_ECHO                                                         :
    break                                                               ;
    case V_FLUTTER                                                      :
    break                                                               ;
    case V_ROUGHNESS                                                    :
    break                                                               ;
    case V_CLARITY                                                      :
    break                                                               ;
    case V_TONE                                                         :
    break                                                               ;
    case V_VOICING                                                      :
    break                                                               ;
    case V_BREATH                                                       :
    break                                                               ;
    case V_BREATHW                                                      :
    break                                                               ;
    case V_WORDGAP                                                      :
    break                                                               ;
    case V_INTONATION                                                   :
    break                                                               ;
    case V_TUNES                                                        :
    break                                                               ;
    case V_STRESSLENGTH                                                 :
    break                                                               ;
    case V_STRESSAMP                                                    :
    break                                                               ;
    case V_STRESSADD                                                    :
    break                                                               ;
    case V_DICTRULES                                                    :
    break                                                               ;
    case V_STRESSRULE                                                   :
    break                                                               ;
    case V_STRESSOPT                                                    :
    break                                                               ;
    case V_CHARSET                                                      :
    break                                                               ;
    case V_NUMBERS                                                      :
    break                                                               ;
    case V_OPTION                                                       :
    break                                                               ;
    case V_MBROLA                                                       :
    break                                                               ;
    case V_KLATT                                                        :
    break                                                               ;
    case V_FAST                                                         :
    break                                                               ;
    case V_SPEED                                                        :
    break                                                               ;
    case V_DICTMIN                                                      :
    break                                                               ;
    case V_ALPHABET2                                                    :
    break                                                               ;
    case V_REPLACE                                                      :
    break                                                               ;
    case V_CONSONANTS                                                   :
    break                                                               ;
    default                                                             :
    break                                                               ;
  }                                                                     ;
  return true                                                           ;
}

void N::Synthesizer::SetEmbedded(int Priority,int control,int value)
{ // there was an embedded command in the text at this point
  int sign    = 0                                          ;
  int command = control & 0x1f                             ;
  //////////////////////////////////////////////////////////
  if ( ( control & 0x60 ) == 0x60 ) sign = -1         ; else
  if ( ( control & 0x60 ) == 0x40 ) sign =  1              ;
  if ( command < 16 )                                      {
    if (sign == 0) Values [ command ]  =   value ;      else
                   Values [ command ] += ( value * sign )  ;
    Values [ command ] = SynthMap[Priority].Restraint      (
                           Values       [ command ]        ,
                           embedded_max [ command ]      ) ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  switch ( command )                                       {
    case EMBED_T                                           :
      SynthMap[Priority].SetEcho                           (
        VoiceMap [ Priority ]                              ,
        Values   [ EMBED_H  ]                              ,
        GetAmplitude()                                   ) ;
    case EMBED_P                                           :
      VoiceMap [ Priority ] . setPitchFormants ( Values  ) ;
    break                                                  ;
    case EMBED_A                                           : // amplitude
      SynthMap [ Priority ] . Amplitude = GetAmplitude ( ) ;
    break                                                  ;
    case EMBED_F                                           : // emphasis
      SynthMap [ Priority ] . Amplitude = GetAmplitude ( ) ;
    break                                                  ;
    case EMBED_H                                           :
      SynthMap[Priority].SetEcho                           (
        VoiceMap [ Priority ]                              ,
        Values   [ EMBED_H  ]                              ,
        GetAmplitude()                                   ) ;
    break                                                  ;
  }                                                        ;
}
