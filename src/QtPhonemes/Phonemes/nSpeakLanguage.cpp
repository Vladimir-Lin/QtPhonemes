#include <qtphonemes.h>

typedef struct             {
  const char   * name      ;
  int            offset    ;
  unsigned short range_min ;
  unsigned short range_max ;
  int            language  ;
  int            flags     ;
} nSpeakAlphabet           ;

#define L(c1,c2)  (c1<<8)+c2 // combine two characters into an integer for translator name

#define L_qa   0x716100
#define L_grc  0x677263 // grc  Ancient Greek
#define L_jbo  0x6a626f // jbo  Lojban
#define L_pap  0x706170 // pap  Papiamento]
#define L_qvi  0x717669 // qvi  Kichwa
#define L_shs  0x736873 // shs  Shuswap / Secwepemctsin
#define L_zhy  0x7a6879 // zhy

// alphabet flags
#define AL_DONT_NAME    0x01 // don't speak the alphabet name
#define AL_NOT_LETTERS  0x02 // don't use the language for speaking letters
#define AL_WORDS        0x04 // use the language to speak words
#define AL_NOT_CODE     0x08 // don't speak the character code
#define AL_NO_SYMBOL    0x10 // don't repeat "symbol" or "character"
#define AL_NO_LNAME     ( AL_DONT_NAME | AL_NOT_LETTERS )
#define AL_NCWORDS      ( AL_NOT_CODE  | AL_WORDS       )

// start of unicode pages for character sets
#define OFFSET_GREEK      0x0380
#define OFFSET_CYRILLIC   0x0420
#define OFFSET_ARMENIAN   0x0530
#define OFFSET_HEBREW     0x0590
#define OFFSET_ARABIC     0x0600
#define OFFSET_THAANA     0x0780 // Divehi/Maldives
#define OFFSET_DEVANAGARI 0x0900
#define OFFSET_BENGALI    0x0980
#define OFFSET_GURMUKHI   0x0a00
#define OFFSET_GUJARATI   0x0a80
#define OFFSET_ORIYA      0x0b00
#define OFFSET_TAMIL      0x0b80
#define OFFSET_TELUGU     0x0c00
#define OFFSET_KANNADA    0x0c80
#define OFFSET_MALAYALAM  0x0d00
#define OFFSET_SINHALA    0x0d80
#define OFFSET_THAI       0x0e00
#define OFFSET_LAO        0x0e80
#define OFFSET_TIBET      0x0f00
#define OFFSET_MYANMAR    0x1000
#define OFFSET_GEORGIAN   0x10a0
#define OFFSET_KOREAN     0x1100
#define OFFSET_ETHIOPIC   0x1200
#define OFFSET_BRAILLE    0x2800
#define OFFSET_JAPANESE   0x3040
#define OFFSET_CHINESE    0x3100

// character ranges must be listed in ascending order
nSpeakAlphabet PrivateAlphabets [] =                                     {
  { "_el"     ,OFFSET_GREEK     ,0x0380,0x03ff,L('e','l'),AL_NO_LNAME  } ,
  { "_cyr"    ,OFFSET_CYRILLIC  ,0x0400,0x052f,0         ,0            } ,
  { "_hy"     ,OFFSET_ARMENIAN  ,0x0530,0x058f,L('h','y'),AL_WORDS     } ,
  { "_he"     ,OFFSET_HEBREW    ,0x0590,0x05ff,0         ,0            } ,
  { "_ar"     ,OFFSET_ARABIC    ,0x0600,0x06ff,0         ,0            } ,
  { "_dv"     ,OFFSET_THAANA    ,0x0780,0x07bf,0         ,0            } ,
  { "_hi"     ,OFFSET_DEVANAGARI,0x0900,0x097f,L('h','i'),AL_WORDS     } ,
  { "_bn"     ,OFFSET_BENGALI   ,0x0980,0x09ff,L('b','n'),AL_WORDS     } ,
  { "_gur"    ,OFFSET_GURMUKHI  ,0x0a00,0x0a7f,L('p','a'),AL_WORDS     } ,
  { "_gu"     ,OFFSET_GUJARATI  ,0x0a80,0x0aff,0         ,0            } ,
  { "_or"     ,OFFSET_ORIYA     ,0x0b00,0x0b7f,0         ,0            } ,
  { "_ta"     ,OFFSET_TAMIL     ,0x0b80,0x0bff,L('t','a'),AL_WORDS     } ,
  { "_te"     ,OFFSET_TELUGU    ,0x0c00,0x0c7f,L('t','e'),0            } ,
  { "_kn"     ,OFFSET_KANNADA   ,0x0c80,0x0cff,L('k','n'),AL_WORDS     } ,
  { "_ml"     ,OFFSET_MALAYALAM ,0x0d00,0x0d7f,L('m','l'),AL_WORDS     } ,
  { "_si"     ,OFFSET_SINHALA   ,0x0d80,0x0dff,L('s','i'),AL_WORDS     } ,
  { "_th"     ,OFFSET_THAI      ,0x0e00,0x0e7f,0         ,0            } ,
  { "_lo"     ,OFFSET_LAO       ,0x0e80,0x0eff,0         ,0            } ,
  { "_ti"     ,OFFSET_TIBET     ,0x0f00,0x0fff,0         ,0            } ,
  { "_my"     ,OFFSET_MYANMAR   ,0x1000,0x109f,0         ,0            } ,
  { "_ka"     ,OFFSET_GEORGIAN  ,0x10a0,0x10ff,L('k','a'),AL_WORDS     } ,
  { "_ko"     ,OFFSET_KOREAN    ,0x1100,0x11ff,L('k','o'),AL_WORDS     } ,
  { "_eth"    ,OFFSET_ETHIOPIC  ,0x1200,0x139f,0         ,0            } ,
  { "_braille",OFFSET_BRAILLE   ,0x2800,0x28ff,0         ,AL_NO_SYMBOL } ,
  { "_ja"     ,OFFSET_JAPANESE  ,0x3040,0x30ff,0         ,AL_NOT_CODE  } ,
  { "_zh"     ,OFFSET_CHINESE   ,0x3100,0x9fff,0         ,AL_NOT_CODE  } ,
  { "_ko"     ,0xa700           ,0xa700,0xd7ff,L('k','o'),AL_NCWORDS   } ,
  { NULL      ,0                ,0     ,0     ,0         ,0            }
}                                                                        ;

nSpeakAlphabet * FromName(const char * name)
{
  nSpeakAlphabet * alphabet                                                 ;
  for ( alphabet = PrivateAlphabets ; alphabet->name != NULL ; alphabet++ ) {
    if ( strcmp ( name , &alphabet->name[1] ) == 0 ) return alphabet        ;
  }                                                                         ;
  return NULL                                                               ;
}

nSpeakAlphabet * FromChar(int c)
{ // Find the alphabet from a character.
  nSpeakAlphabet * alphabet = PrivateAlphabets        ;
  while ( alphabet->name != NULL )                    {
    if ( c <= alphabet->range_max )                   {
      if ( c >= alphabet->range_min ) return alphabet ;
                                 else break           ;
    }                                                 ;
    alphabet++                                        ;
  }                                                   ;
  return NULL                                         ;
}
