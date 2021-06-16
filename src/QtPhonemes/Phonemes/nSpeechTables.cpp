#include <qtphonemes.h>

#define LOPT_DIERESES              1
#define LOPT_IT_LENGTHEN           2
#define LOPT_PREFIXES              3
#define LOPT_REGRESSIVE_VOICING    4
#define LOPT_UNPRONOUNCABLE        5
#define LOPT_LENGTH_MODS           6
#define LOPT_SONORANT_MIN          7
#define LOPT_WORD_MERGE            8
#define LOPT_MAXAMP_EOC            9
#define LOPT_REDUCE               10
#define LOPT_COMBINE_WORDS        11
#define LOPT_REDUCE_T             12
#define LOPT_CAPS_IN_WORD         13
#define LOPT_IT_DOUBLING          14
#define LOPT_ALT                  15
#define LOPT_BRACKET_PAUSE        16
#define LOPT_ANNOUNCE_PUNCT       17
#define LOPT_LONG_VOWEL_THRESHOLD 18
#define LOPT_SUFFIX               19
#define LOPT_APOSTROPHE           20
#define N_LOPTS                   21

#define SSML_SPEAK        1
#define SSML_VOICE        2
#define SSML_PROSODY      3
#define SSML_SAYAS        4
#define SSML_MARK         5
#define SSML_SENTENCE     6
#define SSML_PARAGRAPH    7
#define SSML_PHONEME      8
#define SSML_SUB          9
#define SSML_STYLE       10
#define SSML_AUDIO       11
#define SSML_EMPHASIS    12
#define SSML_BREAK       13
#define SSML_IGNORE_TEXT 14
#define HTML_BREAK       15
#define HTML_NOSPACE     16
#define SSML_CLOSE     0x20

#define SAYAS_CHARS        0x12
#define SAYAS_GLYPHS       0x13
#define SAYAS_SINGLE_CHARS 0x14
#define SAYAS_KEY          0x24
#define SAYAS_DIGITS       0x40
#define SAYAS_DIGITS1      0xc1

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

typedef struct           {
   const char * Mnemonic ;
   int          value    ;
} MnemonicTable          ;

static MnemonicTable mnem_rules[] = {
  { "unpr"   , 0x01 }               ,
  { "w_alt1" , 0x11 }               ,
  { "w_alt2" , 0x12 }               ,
  { "w_alt3" , 0x13 }               ,
  { "w_alt4" , 0x14 }               ,
  { "w_alt5" , 0x15 }               ,
  { "w_alt6" , 0x16 }               ,
  { "w_alt"  , 0x11 }               ,
  { "p_alt1" , 0x21 }               ,
  { "p_alt2" , 0x22 }               ,
  { "p_alt3" , 0x23 }               ,
  { "p_alt4" , 0x24 }               ,
  { "p_alt5" , 0x25 }               ,
  { "p_alt6" , 0x26 }               ,
  { "p_alt"  , 0x21 }               ,
  { NULL     ,   -1 }             } ;

static MnemonicTable mnem_flags[] = {
  { "$1"           , 0x41   }       ,
  { "$2"           , 0x42   }       ,
  { "$3"           , 0x43   }       ,
  { "$4"           , 0x44   }       ,
  { "$5"           , 0x45   }       ,
  { "$6"           , 0x46   }       ,
  { "$7"           , 0x47   }       ,
  { "$u"           , 0x48   }       ,
  { "$u1"          , 0x49   }       ,
  { "$u2"          , 0x4a   }       ,
  { "$u3"          , 0x4b   }       ,
  { "$u+"          , 0x4c   }       ,
  { "$u1+"         , 0x4d   }       ,
  { "$u2+"         , 0x4e   }       ,
  { "$u3+"         , 0x4f   }       ,
  { "$pause"       ,    8   }       ,
  { "$strend"      ,    9   }       ,
  { "$strend2"     ,   10   }       ,
  { "$unstressend" ,   11   }       ,
  { "$abbrev"      ,   13   }       ,
  { "$double"      ,   14   }       ,
  { "$alt"         ,   15   }       ,
  { "$alt1"        ,   15   }       ,
  { "$alt2"        ,   16   }       ,
  { "$alt3"        ,   17   }       ,
  { "$alt4"        ,   18   }       ,
  { "$alt5"        ,   19   }       ,
  { "$alt6"        ,   20   }       ,
  { "$combine"     ,   23   }       ,
  { "$dot"         ,   24   }       ,
  { "$hasdot"      ,   25   }       ,
  { "$max3"        ,   27   }       ,
  { "$brk"         ,   28   }       ,
  { "$text"        ,   29   }       ,
  { "$verbf"       , 0x20   }       ,
  { "$verbsf"      , 0x21   }       ,
  { "$nounf"       , 0x22   }       ,
  { "$pastf"       , 0x23   }       ,
  { "$verb"        , 0x24   }       ,
  { "$noun"        , 0x25   }       ,
  { "$past"        , 0x26   }       ,
  { "$verbextend"  , 0x28   }       ,
  { "$capital"     , 0x29   }       ,
  { "$allcaps"     , 0x2a   }       ,
  { "$accent"      , 0x2b   }       ,
  { "$sentence"    , 0x2d   }       ,
  { "$only"        , 0x2e   }       ,
  { "$onlys"       , 0x2f   }       ,
  { "$stem"        , 0x30   }       ,
  { "$atend"       , 0x31   }       ,
  { "$atstart"     , 0x32   }       ,
  { "$native"      , 0x33   }       ,
  { "$?"           , 100    }       ,
  { "$textmode"    , 200    }       ,
  { "$phonememode" , 201    }       ,
  { NULL           ,   -1   }     } ;

static MnemonicTable voiceGenders [] = {
  { "unknown" , 0 }                    ,
  { "male"    , 1 }                    ,
  { "female"  , 2 }                    ,
  { NULL      , 0 }                  } ;

static MnemonicTable options_tab[] =    {
  { "reduce_t" , LOPT_REDUCE_T      }   ,
  { "bracket"  , LOPT_BRACKET_PAUSE }   ,
  { NULL       , -1                 } } ;

static MnemonicTable keyword_tab[] =                         {
    { "name"             , V_NAME                        }   ,
    { "language"         , V_LANGUAGE                    }   ,
    { "gender"           , V_GENDER                      }   ,
    { "formant"          , V_FORMANT                     }   ,
    { "pitch"            , V_PITCH                       }   ,
    { "phonemes"         , V_PHONEMES                    }   ,
    { "translator"       , V_TRANSLATOR                  }   ,
    { "dictionary"       , V_DICTIONARY                  }   ,
    { "stressLength"     , V_STRESSLENGTH                }   ,
    { "stressAmp"        , V_STRESSAMP                   }   ,
    { "stressAdd"        , V_STRESSADD                   }   ,
    { "intonation"       , V_INTONATION                  }   ,
    { "tunes"            , V_TUNES                       }   ,
    { "dictrules"        , V_DICTRULES                   }   ,
    { "stressrule"       , V_STRESSRULE                  }   ,
    { "stressopt"        , V_STRESSOPT                   }   ,
    { "charset"          , V_CHARSET                     }   ,
    { "replace"          , V_REPLACE                     }   ,
    { "words"            , V_WORDGAP                     }   ,
    { "echo"             , V_ECHO                        }   ,
    { "flutter"          , V_FLUTTER                     }   ,
    { "roughness"        , V_ROUGHNESS                   }   ,
    { "clarity"          , V_CLARITY                     }   ,
    { "tone"             , V_TONE                        }   ,
    { "voicing"          , V_VOICING                     }   ,
    { "breath"           , V_BREATH                      }   ,
    { "breathw"          , V_BREATHW                     }   ,
    { "numbers"          , V_NUMBERS                     }   ,
    { "option"           , V_OPTION                      }   ,
    { "mbrola"           , V_MBROLA                      }   ,
    { "consonants"       , V_CONSONANTS                  }   ,
    { "klatt"            , V_KLATT                       }   ,
    { "fast_test2"       , V_FAST                        }   ,
    { "speed"            , V_SPEED                       }   ,
    { "dict_min"         , V_DICTMIN                     }   ,
    { "alphabet2"        , V_ALPHABET2                   }   ,
    { "l_dieresis"       , 0x100+LOPT_DIERESES           }   ,
    { "l_lengthen"       , 0x100+LOPT_IT_LENGTHEN        }   ,
    { "l_prefix"         , 0x100+LOPT_PREFIXES           }   ,
    { "l_regressive_v"   , 0x100+LOPT_REGRESSIVE_VOICING }   ,
    { "l_unpronouncable" , 0x100+LOPT_UNPRONOUNCABLE     }   ,
    { "l_sonorant_min"   , 0x100+LOPT_SONORANT_MIN       }   ,
    { "l_length_mods"    , 0x100+LOPT_LENGTH_MODS        }   ,
    { "apostrophe"       , 0x100+LOPT_APOSTROPHE         }   ,
    { NULL               , 0                             } } ;

static MnemonicTable ssmltags[] =      {
  { "speak"     , SSML_SPEAK       }   ,
  { "voice"     , SSML_VOICE       }   ,
  { "prosody"   , SSML_PROSODY     }   ,
  { "say-as"    , SSML_SAYAS       }   ,
  { "mark"      , SSML_MARK        }   ,
  { "s"         , SSML_SENTENCE    }   ,
  { "p"         , SSML_PARAGRAPH   }   ,
  { "phoneme"   , SSML_PHONEME     }   ,
  { "sub"       , SSML_SUB         }   ,
  { "tts:style" , SSML_STYLE       }   ,
  { "audio"     , SSML_AUDIO       }   ,
  { "emphasis"  , SSML_EMPHASIS    }   ,
  { "break"     , SSML_BREAK       }   ,
  { "metadata"  , SSML_IGNORE_TEXT }   ,
  { "br"        , HTML_BREAK       }   ,
  { "li"        , HTML_BREAK       }   ,
  { "dd"        , HTML_BREAK       }   ,
  { "img"       , HTML_BREAK       }   ,
  { "td"        , HTML_BREAK       }   ,
  { "h1"        , SSML_PARAGRAPH   }   ,
  { "h2"        , SSML_PARAGRAPH   }   ,
  { "h3"        , SSML_PARAGRAPH   }   ,
  { "h4"        , SSML_PARAGRAPH   }   ,
  { "hr"        , SSML_PARAGRAPH   }   ,
  { "script"    , SSML_IGNORE_TEXT }   ,
  { "style"     , SSML_IGNORE_TEXT }   ,
  { "font"      , HTML_NOSPACE     }   ,
  { "b"         , HTML_NOSPACE     }   ,
  { "i"         , HTML_NOSPACE     }   ,
  { "strong"    , HTML_NOSPACE     }   ,
  { "em"        , HTML_NOSPACE     }   ,
  { "code"      , HTML_NOSPACE     }   ,
  { NULL        , 0                } } ;

static MnemonicTable mnem_gender[] = {
  { "male"    , 1 }                  ,
  { "female"  , 2 }                  ,
  { "neutral" , 3 }                  ,
  { NULL      , 0 }                } ;


static MnemonicTable mnem_volume[] = {
  { "default" , 100 }                ,
  { "silent"  ,   0 }                ,
  { "x-soft"  ,  30 }                ,
  { "soft"    ,  65 }                ,
  { "medium"  , 100 }                ,
  { "loud"    , 150 }                ,
  { "x-loud"  , 230 }                ,
  { NULL      ,  -1 }              } ;

static MnemonicTable mnem_rate[] = {
  { "default" , 100 }              ,
  { "x-slow"  ,  60 }              ,
  { "slow"    ,  80 }              ,
  { "medium"  , 100 }              ,
  { "fast"    , 125 }              ,
  { "x-fast"  , 160 }              ,
  { NULL      ,  -1 }            } ;

static MnemonicTable mnem_pitch[] = {
  { "default" , 100 }               ,
  { "x-low"   ,  70 }               ,
  { "low"     ,  85 }               ,
  { "medium"  , 100 }               ,
  { "high"    , 110 }               ,
  { "x-high"  , 120 }               ,
  { NULL      ,  -1 }             } ;

static MnemonicTable mnem_range[] = {
  { "default" , 100 }               ,
  { "x-low"   ,  20 }               ,
  { "low"     ,  50 }               ,
  { "medium"  , 100 }               ,
  { "high"    , 140 }               ,
  { "x-high"  , 180 }               ,
  { NULL      ,  -1 }             } ;

static MnemonicTable key_names[] = {
  { "space "        , 0xe020 }     ,
  { "tab "          , 0xe009 }     ,
  { "underscore "   , 0xe05f }     ,
  { "double-quote " , '"'    }     ,
  { NULL            , 0      }   } ;

static MnemonicTable mnem_punct[] = {
  { "none" ,  1 }                   ,
  { "all"  ,  2 }                   ,
  { "some" ,  3 }                   ,
  { NULL   , -1 }                 } ;

static MnemonicTable mnem_capitals[] = {
  { "no"       ,   0 }                 ,
  { "spelling" ,   2 }                 ,
  { "icon"     ,   1 }                 ,
  { "pitch"    ,  20 }                 ,
  { NULL       , -1  }               } ;

static MnemonicTable mnem_interpret_as[] = {
  { "characters" , SAYAS_CHARS        }    ,
  { "tts:char"   , SAYAS_SINGLE_CHARS }    ,
  { "tts:key"    , SAYAS_KEY          }    ,
  { "tts:digits" , SAYAS_DIGITS       }    ,
  { "telephone"  , SAYAS_DIGITS1      }    ,
  { NULL         , -1                 }  } ;

static MnemonicTable mnem_sayas_format[] = {
  { "glyphs" ,  1 }                        ,
  { NULL     , -1 }                      } ;

static MnemonicTable mnem_break[] = {
  { "none"     ,  0 }               ,
  { "x-weak"   ,  1 }               ,
  { "weak"     ,  2 }               ,
  { "medium"   ,  3 }               ,
  { "strong"   ,  4 }               ,
  { "x-strong" ,  5 }               ,
  { NULL       , -1 }             } ;

static MnemonicTable mnem_emphasis[] = {
  { "none"     ,  1 }                  ,
  { "reduced"  ,  2 }                  ,
  { "moderate" ,  3 }                  ,
  { "strong"   ,  4 }                  ,
  { "x-strong" ,  5 }                  ,
  { NULL       , -1 }                } ;

QString N::Acoustics::Name             (
          Speak::SynthesisTables table ,
          int                    value )
{
  MnemonicTable * mnem = NULL          ;
  int             i    = 0             ;
  switch (table)                       {
    case Speak::MnemonicRules          :
      mnem = mnem_rules                ;
    break                              ;
    case Speak::MnemonicFlags          :
      mnem = mnem_flags                ;
    break                              ;
    case Speak::VoiceGenders           :
      mnem = voiceGenders              ;
    break                              ;
    case Speak::OptionsTable           :
      mnem = options_tab               ;
    break                              ;
    case Speak::KeywordTable           :
      mnem = keyword_tab               ;
    break                              ;
    case Speak::SsmlTags               :
      mnem = ssmltags                  ;
    break                              ;
    case Speak::MnemonicGender         :
      mnem = mnem_gender               ;
    break                              ;
    case Speak::MnemonicVolume         :
      mnem = mnem_volume               ;
    break                              ;
    case Speak::MnemonicRate           :
      mnem = mnem_rate                 ;
    break                              ;
    case Speak::MnemonicPitch          :
      mnem = mnem_pitch                ;
    break                              ;
    case Speak::MnemonicRange          :
      mnem = mnem_range                ;
    break                              ;
    case Speak::MnemonicKeys           :
      mnem = key_names                 ;
    break                              ;
    case Speak::MnemonicPunct          :
      mnem = mnem_punct                ;
    break                              ;
    case Speak::MnemonicCapitals       :
      mnem = mnem_capitals             ;
    break                              ;
    case Speak::MnemonicInterpret      :
      mnem = mnem_interpret_as         ;
    break                              ;
    case Speak::SayAsFormat            :
      mnem = mnem_sayas_format         ;
    break                              ;
    case Speak::MnemonicBreak          :
      mnem = mnem_break                ;
    break                              ;
    case Speak::MnemonicEmphasis       :
      mnem = mnem_emphasis             ;
    break                              ;
  }                                    ;
  if (IsNull(mnem)) return QString("") ;
  while ( NotNull(mnem[i].Mnemonic) )  {
    if (nEqual(mnem[i].value,value))   {
      return QString(mnem[i].Mnemonic) ;
    }                                  ;
    i++                                ;
  }                                    ;
  return QString("")                   ;
}

int N::Acoustics::Lookup           (
      Speak::SynthesisTables table ,
      QString                name  )
{
  MnemonicTable * mnem = NULL            ;
  int             i    = 0               ;
  switch (table)                         {
    case Speak::MnemonicRules            :
      mnem = mnem_rules                  ;
    break                                ;
    case Speak::MnemonicFlags            :
      mnem = mnem_flags                  ;
    break                                ;
    case Speak::VoiceGenders             :
      mnem = voiceGenders                ;
    break                                ;
    case Speak::OptionsTable             :
      mnem = options_tab                 ;
    break                                ;
    case Speak::KeywordTable             :
      mnem = keyword_tab                 ;
    break                                ;
    case Speak::SsmlTags                 :
      mnem = ssmltags                    ;
    break                                ;
    case Speak::MnemonicGender           :
      mnem = mnem_gender                 ;
    break                                ;
    case Speak::MnemonicVolume           :
      mnem = mnem_volume                 ;
    break                                ;
    case Speak::MnemonicRate             :
      mnem = mnem_rate                   ;
    break                                ;
    case Speak::MnemonicPitch            :
      mnem = mnem_pitch                  ;
    break                                ;
    case Speak::MnemonicRange            :
      mnem = mnem_range                  ;
    break                                ;
    case Speak::MnemonicKeys             :
      mnem = key_names                   ;
    break                                ;
    case Speak::MnemonicPunct            :
      mnem = mnem_punct                  ;
    break                                ;
    case Speak::MnemonicCapitals         :
      mnem = mnem_capitals               ;
    break                                ;
    case Speak::MnemonicInterpret        :
      mnem = mnem_interpret_as           ;
    break                                ;
    case Speak::SayAsFormat              :
      mnem = mnem_sayas_format           ;
    break                                ;
    case Speak::MnemonicBreak            :
      mnem = mnem_break                  ;
    break                                ;
    case Speak::MnemonicEmphasis         :
      mnem = mnem_emphasis               ;
    break                                ;
  }                                      ;
  if (IsNull(mnem)) return -1            ;
  while ( NotNull(mnem[i].Mnemonic) )    {
    if (QString(mnem[i].Mnemonic)==name) {
      return mnem[i].value               ;
    }                                    ;
    i++                                  ;
  }                                      ;
  return mnem[i].value                   ;
}
