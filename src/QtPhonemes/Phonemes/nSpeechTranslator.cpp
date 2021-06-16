#include <qtphonemes.h>

typedef struct             {
  const char   * name      ;
  int            offset    ;
  unsigned short range_min ;
  unsigned short range_max ;
  int            language  ;
  int            flags     ;
} nSpeakAlphabet           ;

extern nSpeakAlphabet * FromName(const char * name) ;
extern nSpeakAlphabet * FromChar(int          c   ) ;

#define L(c1,c2) (c1<<8)+c2 // combine two characters into an integer for translator name

#define L_qa   0x716100
#define L_grc  0x677263 // grc  Ancient Greek
#define L_jbo  0x6a626f // jbo  Lojban
#define L_pap  0x706170 // pap  Papiamento]
#define L_qvi  0x717669 // qvi  Kichwa
#define L_shs  0x736873 // shs  Shuswap / Secwepemctsin
#define L_zhy  0x7a6879 // zhy

#define REPLACED_E 'E' // 'e' replaced by silent e

#define N_WORD_PHONEMES  200          // max phonemes in a word
#define N_WORD_BYTES     160          // max bytes for the UTF8 characters in a word
#define N_CLAUSE_WORDS   300          // max words in a clause
#define N_RULE_GROUP2    120          // max num of two-letter rule chains
#define N_HASH_DICT     1024
#define N_CHARSETS        20
#define N_LETTER_GROUPS   95          // maximum is 127-32

/* dictionary flags, word 1 */
// bits 0-3  stressed syllable,  bit 6=unstressed
#define FLAG_SKIPWORDS        0x80
#define FLAG_PREPAUSE        0x100

#define FLAG_STRESS_END      0x200  // full stress if at end of clause
#define FLAG_STRESS_END2     0x400  // full stress if at end of clause, or only followed by unstressed
#define FLAG_UNSTRESS_END    0x800  // reduce stress at end of clause
#define FLAG_SPELLWORD      0x1000  // re-translate the word as individual letters, separated by spaces
#define FLAG_ABBREV         0x2000  // spell as letters, even with a vowel, OR use specified pronunciation rather than split into letters
#define FLAG_DOUBLING       0x4000  // doubles the following consonant

#define BITNUM_FLAG_ALT         14  // bit number of FLAG_ALT_TRANS - 1
#define FLAG_ALT_TRANS      0x8000  // language specific
#define FLAG_ALT2_TRANS    0x10000  // language specific
#define FLAG_ALT3_TRANS    0x20000  // language specific
#define FLAG_ALT4_TRANS    0x40000  // language specific
#define FLAG_ALT5_TRANS    0x80000  // language specific
#define FLAG_ALT6_TRANS   0x100000  // language specific

#define FLAG_COMBINE      0x800000  // combine with the next word
#define FLAG_ALLOW_DOT  0x01000000  // ignore '.' after word (abbreviation)
#define FLAG_NEEDS_DOT  0x02000000  // only if the word is followed by a dot
#define FLAG_WAS_UNPRONOUNCABLE  0x04000000  // the unpronounceable routine was used
#define FLAG_MAX3       0x08000000  // limit to 3 repeats
#define FLAG_PAUSE1     0x10000000  // shorter prepause
#define FLAG_TEXTMODE   0x20000000  // word translates to replacement text, not phonemes
#define BITNUM_FLAG_TEXTMODE    29

#define FLAG_FOUND_ATTRIBUTES     0x40000000  // word was found in the dictionary list (has attributes)
#define FLAG_FOUND      0x80000000  // pronunciation was found in the dictionary list

// dictionary flags, word 2
#define FLAG_VERBF             0x1  /* verb follows */
#define FLAG_VERBSF            0x2  /* verb follows, may have -s suffix */
#define FLAG_NOUNF             0x4  /* noun follows */
#define FLAG_PASTF             0x8  /* past tense follows */
#define FLAG_VERB             0x10  /* pronunciation for verb */
#define FLAG_NOUN             0x20  /* pronunciation for noun */
#define FLAG_PAST             0x40  /* pronunciation for past tense */
#define FLAG_VERB_EXT        0x100  /* extend the 'verb follows' */
#define FLAG_CAPITAL         0x200  /* pronunciation if initial letter is upper case */
#define FLAG_ALLCAPS         0x400  // only if the word is all capitals
#define FLAG_ACCENT          0x800  // character name is base-character name + accent name
#define FLAG_HYPHENATED     0x1000  // multiple-words, but needs hyphen between parts 1 and 2
#define FLAG_SENTENCE       0x2000  // only if the clause is a sentence
#define FLAG_ONLY           0x4000
#define FLAG_ONLY_S         0x8000
#define FLAG_STEM          0x10000  // must have a suffix
#define FLAG_ATEND         0x20000  // use this pronunciation if at end of clause
#define FLAG_ATSTART       0x40000  // use this pronunciation if at start of clause
#define FLAG_NATIVE        0x80000  // not if we've switched translators
#define FLAG_LOOKUP_SYMBOL 0x40000000  // to indicate called from Lookup()

#define BITNUM_FLAG_ALLCAPS   0x2a
#define BITNUM_FLAG_HYPHENATED  0x2c
#define BITNUM_FLAG_ONLY      0x2e
#define BITNUM_FLAG_ONLY_S    0x2f

// wordflags, flags in source word
#define FLAG_ALL_UPPER     0x1    /* no lower case letters in the word */
#define FLAG_FIRST_UPPER   0x2    /* first letter is upper case */
#define FLAG_UPPERS        0x3    // FLAG_ALL_UPPER | FLAG_FIRST_UPPER
#define FLAG_HAS_PLURAL    0x4    /* upper-case word with s or 's lower-case ending */
#define FLAG_PHONEMES      0x8    /* word is phonemes */
#define FLAG_LAST_WORD     0x10   /* last word in clause */
#define FLAG_EMBEDDED      0x40   /* word is preceded by embedded commands */
#define FLAG_HYPHEN        0x80
#define FLAG_NOSPACE       0x100  // word is not seperated from previous word by a space
#define FLAG_FIRST_WORD    0x200  // first word in clause
#define FLAG_FOCUS         0x400   // the focus word of a clause
#define FLAG_EMPHASIZED    0x800
#define FLAG_EMPHASIZED2   0xc00  // FLAG_FOCUS | FLAG_EMPHASIZED
#define FLAG_DONT_SWITCH_TRANSLATOR  0x1000
#define FLAG_SUFFIX_REMOVED  0x2000
#define FLAG_HYPHEN_AFTER    0x4000
#define FLAG_ORDINAL       0x8000   // passed to TranslateNumber() to indicate an ordinal number
#define FLAG_HAS_DOT       0x10000  // dot after this word
#define FLAG_COMMA_AFTER   0x20000  // comma after this word
#define FLAG_MULTIPLE_SPACES 0x40000  // word is preceded by multiple spaces, newline, or tab
#define FLAG_INDIVIDUAL_DIGITS 0x80000  // speak number as individual digits
#define FLAG_DELETE_WORD     0x100000   // don't speak this word, it has been spoken as part of the previous word
#define FLAG_CHAR_REPLACED   0x200000   // characters have been replaced by .replace in the *_rules
#define FLAG_TRANSLATOR2     0x400000   // retranslating using a different language

#define FLAG_SUFFIX_VOWEL  0x08000000   // remember an initial vowel from the suffix
#define FLAG_NO_TRACE      0x10000000   // passed to TranslateRules() to suppress dictionary lookup printout
#define FLAG_NO_PREFIX     0x20000000
#define FLAG_UNPRON_TEST   0x80000000   // do unpronounability test on the beginning of the word

// prefix/suffix flags (bits 8 to 14, bits 16 to 22) don't use 0x8000, 0x800000
#define SUFX_E        0x0100   // e may have been added
#define SUFX_I        0x0200   // y may have been changed to i
#define SUFX_P        0x0400   // prefix
#define SUFX_V        0x0800   // suffix means use the verb form pronunciation
#define SUFX_D        0x1000   // previous letter may have been doubled
#define SUFX_F        0x2000   // verb follows
#define SUFX_Q        0x4000   // don't retranslate
#define SUFX_T        0x10000   // don't affect the stress position in the stem
#define SUFX_B        0x20000  // break, this character breaks the word into stem and suffix (used with SUFX_P)
#define SUFX_A        0x40000  // remember that the suffix starts with a vowel
#define SUFX_M        0x80000  // bit 19, allow multiple suffixes

#define SUFX_UNPRON     0x8000   // used to return $unpron flag from *_rules

#define FLAG_ALLOW_TEXTMODE  0x02  // allow dictionary to translate to text rather than phonemes
#define FLAG_SUFX       0x04
#define FLAG_SUFX_S     0x08
#define FLAG_SUFX_E_ADDED 0x10

// codes in dictionary rules
#define RULE_PRE           1
#define RULE_POST          2
#define RULE_PHONEMES      3
#define RULE_PH_COMMON     4 // At start of rule. Its phoneme string is used by subsequent rules
#define RULE_CONDITION     5 // followed by condition number (byte)
#define RULE_GROUP_START   6
#define RULE_GROUP_END     7
#define RULE_PRE_ATSTART   8   // as RULE_PRE but also match with 'start of word'
#define RULE_LINENUM       9  // next 2 bytes give a line number, for debugging purposes
#define RULE_SPACE        32   // ascii space
#define RULE_SYLLABLE     21    // @
#define RULE_STRESSED     10   // &
#define RULE_DOUBLE       11   // %
#define RULE_INC_SCORE    12   // +
#define RULE_DEL_FWD      13   // #
#define RULE_ENDING       14   // S
#define RULE_DIGIT        15   // D digit
#define RULE_NONALPHA     16   // Z non-alpha
#define RULE_LETTERGP     17   // A B C H F G Y   letter group number
#define RULE_LETTERGP2    18   // L + letter group number
#define RULE_CAPITAL      19   // !   word starts with a capital letter
#define RULE_REPLACEMENTS 20  // section for character replacements
#define RULE_SKIPCHARS    23   // J
#define RULE_NO_SUFFIX    24   // N
#define RULE_NOTVOWEL     25   // K
#define RULE_IFVERB       26   // V
#define RULE_DOLLAR       28   // $ commands
#define RULE_NOVOWELS     29   // X no vowels up to word boundary
#define RULE_SPELLING     31   // W while spelling letter-by-letter
#define RULE_LAST_RULE    31

#define LETTERGP_A      0
#define LETTERGP_B      1
#define LETTERGP_C      2
#define LETTERGP_H      3
#define LETTERGP_F      4
#define LETTERGP_G      5
#define LETTERGP_Y      6
#define LETTERGP_VOWEL2 7

#define N_LOPTS                   21
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

// stress_rule
#define STRESSPOSN_1L 0 // 1st syllable
#define STRESSPOSN_2L 1 // 2nd syllable
#define STRESSPOSN_2R 2 // penultimate
#define STRESSPOSN_1R 3 // final syllable
#define STRESSPOSN_3R 4 // antipenultimate

#define BREAK_THOUSANDS 0x49249248

#define NUM_THOUS_SPACE       0x4
#define NUM_DECIMAL_COMMA     0x8
#define NUM_SWAP_TENS         0x10
#define NUM_AND_UNITS         0x20
#define NUM_HUNDRED_AND       0x40
#define NUM_SINGLE_AND        0x80
#define NUM_SINGLE_STRESS     0x100
#define NUM_SINGLE_VOWEL      0x200
#define NUM_OMIT_1_HUNDRED    0x400
#define NUM_1900              0x800
#define NUM_ALLOW_SPACE       0x1000
#define NUM_DFRACTION_1       0x2000
#define NUM_DFRACTION_2       0x4000
#define NUM_DFRACTION_3       0x6000
#define NUM_DFRACTION_4       0x8000
#define NUM_DFRACTION_5       0xa000
#define NUM_DFRACTION_6       0xc000
#define NUM_DFRACTION_7       0xe000    // lang=si, alternative form of number for decimal fraction digits (except the last)
#define NUM_ORDINAL_DOT       0x10000
#define NUM_NOPAUSE           0x20000
#define NUM_AND_HUNDRED       0x40000
#define NUM_THOUSAND_AND      0x80000
#define NUM_VIGESIMAL         0x100000
#define NUM_OMIT_1_THOUSAND   0x200000
#define NUM_ZERO_HUNDRED      0x400000
#define NUM_HUNDRED_AND_DIGIT 0x800000
#define NUM_ROMAN             0x1000000
#define NUM_ROMAN_CAPITALS    0x2000000
#define NUM_ROMAN_AFTER       0x4000000
#define NUM_ROMAN_ORDINAL     0x8000000
#define NUM_SINGLE_STRESS_L   0x10000000

#define NUM2_THOUSANDS_VAR1   0x40
#define NUM2_THOUSANDS_VAR2   0x80
#define NUM2_THOUSANDS_VAR3   0xc0
#define NUM2_THOUSANDS_VAR4   0x100
#define NUM2_THOUSANDS_VAR5   0x140

#define NUM2_ORDINAL_NO_AND   0x800
#define NUM2_MULTIPLE_ORDINAL 0x1000
#define NUM2_NO_TEEN_ORDINALS 0x2000
#define NUM2_MYRIADS          0x4000
#define NUM2_ENGLISH_NUMERALS 0x8000
#define NUM2_PERCENT_BEFORE   0x10000

#define PITCHfall     0
#define PITCHrise     2
#define PITCHfrise    4 // and 3 must be for the variant preceded by 'r'
#define PITCHfrise2   6 // and 5 must be the 'r' variant
#define PITCHrisefall 8

#define SYL_RISE       1
#define SYL_EMPHASIS   2
#define SYL_END_CLAUSE 4

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

#define S_NO_DIM            0x02
#define S_FINAL_DIM         0x04
#define S_FINAL_DIM_ONLY    0x06
#define S_FINAL_NO_2        0x10
#define S_NO_AUTO_2         0x20
#define S_2_TO_HEAVY        0x40
#define S_FIRST_PRIMARY     0x80
#define S_FINAL_STRESS_C    0x100
#define S_FINAL_SPANISH     0x200
#define S_2_SYL_2           0x1000
#define S_INITIAL_2         0x2000
#define S_MID_DIM           0x10000
#define S_PRIORITY_STRESS   0x20000
#define S_EO_CLAUSE1        0x40000
#define S_FINAL_LONG        0x80000
#define S_HYPEN_UNSTRESS    0x100000
#define S_NO_EOC_LENGTHEN   0x200000

#define SFLAG_SEQCONTINUE      0x01   // a liquid or nasal after a vowel, but not followed by a vowel
#define SFLAG_EMBEDDED         0x02   // there are embedded commands before this phoneme
#define SFLAG_SYLLABLE         0x04   // vowel or syllabic consonant
#define SFLAG_LENGTHEN         0x08   // lengthen symbol : included after this phoneme
#define SFLAG_DICTIONARY       0x10   // the pronunciation of this word was listed in the xx_list dictionary
#define SFLAG_SWITCHED_LANG    0x20   // this word uses phonemes from a different language
#define SFLAG_PROMOTE_STRESS   0x40   // this unstressed word can be promoted to stressed

#define SFLAG_PREV_PAUSE     0x1000   // consider previous phoneme as pause
#define SFLAG_NEXT_PAUSE     0x2000   // consider next phoneme as pause

#define CLAUSE_BIT_SENTENCE  0x80000
#define CLAUSE_BIT_CLAUSE    0x40000
#define CLAUSE_BIT_VOICE     0x20000
#define CLAUSE_BITS_INTONATION 0x7000

// embedded command numbers
#define EMBED_P     1   // pitch
#define EMBED_S     2   // speed (used in setlengths)
#define EMBED_A     3   // amplitude/volume
#define EMBED_R     4   // pitch range/expression
#define EMBED_H     5   // echo/reverberation
#define EMBED_T     6   // different tone for announcing punctuation (not used)
#define EMBED_I     7   // sound icon
#define EMBED_S2    8   // speed (used in synthesize)
#define EMBED_Y     9   // say-as commands
#define EMBED_M    10   // mark name
#define EMBED_U    11   // audio uri
#define EMBED_B    12   // break
#define EMBED_F    13   // emphasis
#define EMBED_C    14   // capital letter indication

#define N_EMBEDDED_VALUES    15

#define N_REMOVE_ACCENT 0x25e

// additional Latin characters beyond the ascii character set
#define MAX_WALPHA  0x24f
// indexed by character - 0x80
// 0=not alphabetic, 0xff=lower case, 0xfe=no case, 0xfd=use wchar_tolower
//   other=value to add to upper case to convert to lower case
static unsigned char walpha_tab[MAX_WALPHA-0x7f] = {
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, // 080
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, // 090
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0xfe,    0,    0,    0,    0,    0, // 0a0
    0,    0,    0,    0,    0, 0xff,    0,    0,    0,    0, 0xfe,    0,    0,    0,    0,    0, // 0b0
   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32, // 0c0
   32,   32,   32,   32,   32,   32,   32,    0,   32,   32,   32,   32,   32,   32,   32, 0xff, // 0d0
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0e0
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,    0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0f0
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 100
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 110
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 120
 0xfd, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, 0xfe,    1, 0xff,    1, 0xff,    1, 0xff,    1, // 130
 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, 0xfe,    1, 0xff,    1, 0xff,    1, 0xff, // 140
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 150
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 160
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, 0xfd,    1, 0xff,    1, 0xff,    1, 0xff, 0xff, // 170
 0xff,  210,    1, 0xff,    1, 0xff,  206,    1, 0xff,  205,  205,    1, 0xff, 0xfe,   79,  202, // 180
  203,    1, 0xff,  205,  207, 0xff,  211,  209,    1, 0xff, 0xff, 0xfe,  211,  213, 0xff,  214, // 190
    1, 0xff,    1, 0xff,    1, 0xff,  218,    1, 0xff,  218, 0xfe, 0xfe,    1, 0xff,  218,    1, // 1a0
 0xff,  217,  217,    1, 0xff,    1, 0xff,  219,    1, 0xff, 0xfe, 0xfe,    1, 0xff, 0xfe, 0xff, // 1b0
 0xfe, 0xfe, 0xfe, 0xfe,    2, 0xff, 0xff,    2, 0xff, 0xff,    2, 0xff, 0xff,    1, 0xff,    1, // 1c0
 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, 0xff,    1, 0xff, // 1d0
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 1e0
 0xfe,    2, 0xff, 0xff,    1, 0xff, 0xfd, 0xfd,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 1f0
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 200
    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 210
 0xfd, 0xfe,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff, // 220
    1, 0xff,    1, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfd,    1, 0xff, 0xfd, 0xfd, 0xfe, // 230
 0xfe,    1, 0xff, 0xfd,   69,   71,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff,    1, 0xff}; // 240

static const short wchar_tolower[] = {
  0x130, 0x069,
  0x178, 0x0ff,
  0x1f6, 0x195,
  0x1f7, 0x1bf,
  0x220, 0x19e,
  0x23a, 0x2c65,
  0x23d, 0x19a,
  0x23e, 0x2c66,
  0x243, 0x180,
  0    ,0 };

static const short wchar_toupper[] = {
  0x0b5, 0x39c,
  0x0df, 0x0df,
  0x0ff, 0x178,
  0x131, 0x049,
  0x17f, 0x053,
  0x180, 0x243,
  0x195, 0x1f6,
  0x19a, 0x23d,
  0x19e, 0x220,
  0x1bf, 0x1f7,
  0x1c6, 0x1c4,
  0x1c9, 0x1c7,
  0x1cc, 0x1ca,
  0x1dd, 0x18e,
  0x1f3, 0x1f1,
  0    , 0 };

// brackets, also 0x2014 to 0x021f which don't need to be in this list
static const unsigned short brackets[] = {
  '(',')','[',']','{','}','<','>','"','\'','`',
  0xab,0xbb,  // double angle brackets
  0x300a,0x300b,  // double angle brackets (ideograph)
  0xe000+'<',  // private usage area
  0
};

// unicode ranges for non-ascii digits 0-9
static const int number_ranges[] = {
  0x660, 0x6f0,  // arabic
  0x966, 0x9e6, 0xa66, 0xae6, 0xb66, 0xbe6, 0xc66, 0xce6, 0xd66,  // indic
  0xe50, 0xed0, 0xf20, 0x1040, 0x1090,
  0 };  // these must be in ascending order

static const char * UCase_ga[] = {"bp","bhf","dt","gc","hA","mb","nd","ng","ts","tA","nA",NULL};

static char stress_phonemes[] = {
  N::Speak::PcStressD           ,
  N::Speak::PcStressU           ,
  N::Speak::PcStress2           ,
  N::Speak::PcStress3           ,
  N::Speak::PcStressP           ,
  N::Speak::PcStressP2          ,
  N::Speak::PcStressTonic     } ;

static char consonant_types [16] = {0,0,0,1,1,1,1,1,1,1,0,0,0,0, 0, 0} ;
static char guess_ru        [16] = {0,0,1,1,2,3,3,4,5,6,7,7,8,9,10,11} ;
static char guess_ru_v      [16] = {0,0,1,1,2,2,3,3,4,5,6,7,7,8, 9,10} ; // for final phoneme is a vowel
static char guess_ru_t      [16] = {0,0,1,2,3,3,3,4,5,6,7,7,7,8, 9,10} ; // for final phoneme is an unvoiced stop

static unsigned char remove_accent[N_REMOVE_ACCENT] =            {
  'a','a','a','a','a','a','a','c','e','e','e','e','i','i','i','i', // 0c0
  'd','n','o','o','o','o','o', 0, 'o','u','u','u','u','y','t','s', // 0d0
  'a','a','a','a','a','a','a','c','e','e','e','e','i','i','i','i', // 0e0
  'd','n','o','o','o','o','o', 0 ,'o','u','u','u','u','y','t','y', // 0f0
  'a','a','a','a','a','a','c','c','c','c','c','c','c','c','d','d', // 100
  'd','d','e','e','e','e','e','e','e','e','e','e','g','g','g','g', // 110
  'g','g','g','g','h','h','h','h','i','i','i','i','i','i','i','i', // 120
  'i','i','i','i','j','j','k','k','k','l','l','l','l','l','l','l', // 130
  'l','l','l','n','n','n','n','n','n','n','n','n','o','o','o','o', // 140
  'o','o','o','o','r','r','r','r','r','r','s','s','s','s','s','s', // 150
  's','s','t','t','t','t','t','t','u','u','u','u','u','u','u','u', // 160
  'u','u','u','u','w','w','y','y','y','z','z','z','z','z','z','s', // 170
  'b','b','b','b', 0,  0, 'o','c','c','d','d','d','d','d','e','e', // 180
  'e','f','f','g','g','h','i','i','k','k','l','l','m','n','n','o', // 190
  'o','o','o','o','p','p','y', 0,  0, 's','s','t','t','t','t','u', // 1a0
  'u','u','v','y','y','z','z','z','z','z','z','z', 0,  0,  0, 'w', // 1b0
  't','t','t','k','d','d','d','l','l','l','n','n','n','a','a','i', // 1c0
  'i','o','o','u','u','u','u','u','u','u','u','u','u','e','a','a', // 1d0
  'a','a','a','a','g','g','g','g','k','k','o','o','o','o','z','z', // 1e0
  'j','d','d','d','g','g','w','w','n','n','a','a','a','a','o','o', // 1f0
  'a','a','a','a','e','e','e','e','i','i','i','i','o','o','o','o', // 200
  'r','r','r','r','u','u','u','u','s','s','t','t','y','y','h','h', // 210
  'n','d','o','o','z','z','a','a','e','e','o','o','o','o','o','o', // 220
  'o','o','y','y','l','n','t','j','d','q','a','c','c','l','t','s', // 230
  'z', 0,  0, 'b','u','v','e','e','j','j','q','q','r','r','y','y', // 240
  'a','a','a','b','o','c','d','d','e','e','e','e','e','e'
}                                                                ;

// Translate character codes 0xA0 to 0xFF into their unicode values
// ISO_8859_1 is set as default
static const unsigned short ISO_8859_1[0x60] = {
  0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, // a0
  0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af, // a8
  0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, // b0
  0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf, // b8
  0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, // c0
  0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, // c8
  0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, // d0
  0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df, // d8
  0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, // e0
  0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef, // e8
  0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, // f0
  0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff, // f8
};

static const unsigned short ISO_8859_2[0x60] = {
  0x00a0, 0x0104, 0x02d8, 0x0141, 0x00a4, 0x013d, 0x015a, 0x00a7, // a0
  0x00a8, 0x0160, 0x015e, 0x0164, 0x0179, 0x00ad, 0x017d, 0x017b, // a8
  0x00b0, 0x0105, 0x02db, 0x0142, 0x00b4, 0x013e, 0x015b, 0x02c7, // b0
  0x00b8, 0x0161, 0x015f, 0x0165, 0x017a, 0x02dd, 0x017e, 0x017c, // b8
  0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7, // c0
  0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e, // c8
  0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7, // d0
  0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df, // d8
  0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7, // e0
  0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f, // e8
  0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7, // f0
  0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9, // f8
};

static const unsigned short ISO_8859_3[0x60] = {
  0x00a0, 0x0126, 0x02d8, 0x00a3, 0x00a4, 0x0000, 0x0124, 0x00a7, // a0
  0x00a8, 0x0130, 0x015e, 0x011e, 0x0134, 0x00ad, 0x0000, 0x017b, // a8
  0x00b0, 0x0127, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x0125, 0x00b7, // b0
  0x00b8, 0x0131, 0x015f, 0x011f, 0x0135, 0x00bd, 0x0000, 0x017c, // b8
  0x00c0, 0x00c1, 0x00c2, 0x0000, 0x00c4, 0x010a, 0x0108, 0x00c7, // c0
  0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, // c8
  0x0000, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x0120, 0x00d6, 0x00d7, // d0
  0x011c, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x016c, 0x015c, 0x00df, // d8
  0x00e0, 0x00e1, 0x00e2, 0x0000, 0x00e4, 0x010b, 0x0109, 0x00e7, // e0
  0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef, // e8
  0x0000, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x0121, 0x00f6, 0x00f7, // f0
  0x011d, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x016d, 0x015d, 0x02d9, // f8
};

static const unsigned short ISO_8859_4[0x60] = {
  0x00a0, 0x0104, 0x0138, 0x0156, 0x00a4, 0x0128, 0x013b, 0x00a7, // a0
  0x00a8, 0x0160, 0x0112, 0x0122, 0x0166, 0x00ad, 0x017d, 0x00af, // a8
  0x00b0, 0x0105, 0x02db, 0x0157, 0x00b4, 0x0129, 0x013c, 0x02c7, // b0
  0x00b8, 0x0161, 0x0113, 0x0123, 0x0167, 0x014a, 0x017e, 0x014b, // b8
  0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e, // c0
  0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x012a, // c8
  0x0110, 0x0145, 0x014c, 0x0136, 0x00d4, 0x00d5, 0x00d6, 0x00d7, // d0
  0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x0168, 0x016a, 0x00df, // d8
  0x0101, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x012f, // e0
  0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x012b, // e8
  0x0111, 0x0146, 0x014d, 0x0137, 0x00f4, 0x00f5, 0x00f6, 0x00f7, // f0
  0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x0169, 0x016b, 0x02d9, // f8
};

static const unsigned short ISO_8859_5[0x60] = {
  0x00a0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, // a0  Cyrillic
  0x0408, 0x0409, 0x040a, 0x040b, 0x040c, 0x00ad, 0x040e, 0x040f, // a8
  0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, // b0
  0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f, // b8
  0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, // c0
  0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f, // c8
  0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, // d0
  0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f, // d8
  0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, // e0
  0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f, // e8
  0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457, // f0
  0x0458, 0x0459, 0x045a, 0x045b, 0x045c, 0x00a7, 0x045e, 0x045f, // f8
};

static const unsigned short ISO_8859_7[0x60] = {
  0x00a0, 0x2018, 0x2019, 0x00a3, 0x20ac, 0x20af, 0x00a6, 0x00a7, // a0  Greek
  0x00a8, 0x00a9, 0x037a, 0x00ab, 0x00ac, 0x00ad, 0x0000, 0x2015, // a8
  0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x0384, 0x0385, 0x0386, 0x00b7, // b0
  0x0388, 0x0389, 0x038a, 0x00bb, 0x038c, 0x00bd, 0x038e, 0x038f, // b8
  0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, // c0
  0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f, // c8
  0x03a0, 0x03a1, 0x0000, 0x03a3, 0x03a4, 0x03a5, 0x03a6, 0x03a7, // d0
  0x03a8, 0x03a9, 0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03ae, 0x03af, // d8
  0x03b0, 0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7, // e0
  0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf, // e8
  0x03c0, 0x03c1, 0x03c2, 0x03c3, 0x03c4, 0x03c5, 0x03c6, 0x03c7, // f0
  0x03c8, 0x03c9, 0x03ca, 0x03cb, 0x03cc, 0x03cd, 0x03ce, 0x0000, // f8
};

static const unsigned short ISO_8859_9[0x60] = {
  0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, // a0
  0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af, // a8
  0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, // b0
  0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf, // b8
  0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, // c0
  0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, // c8
  0x011e, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, // d0
  0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0130, 0x015e, 0x00df, // d8
  0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, // e0
  0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef, // e8
  0x011f, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, // f0
  0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0131, 0x015f, 0x00ff, // f8
};

static const unsigned short ISO_8859_14[0x60] = {
  0x00a0, 0x1e02, 0x1e03, 0x00a3, 0x010a, 0x010b, 0x1e0a, 0x00a7, // a0  Welsh
  0x1e80, 0x00a9, 0x1e82, 0x1e0b, 0x1ef2, 0x00ad, 0x00ae, 0x0178, // a8
  0x1e1e, 0x1e1f, 0x0120, 0x0121, 0x1e40, 0x1e41, 0x00b6, 0x1e56, // b0
  0x1e81, 0x1e57, 0x1e83, 0x1e60, 0x1ef3, 0x1e84, 0x1e85, 0x1e61, // b8
  0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, // c0
  0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, // c8
  0x0174, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x1e6a, // d0
  0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x0176, 0x00df, // d8
  0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, // e0
  0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef, // e8
  0x0175, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x1e6b, // f0
  0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x0177, 0x00ff, // f8
};

static const unsigned short KOI8_R[0x60] = {
  0x2550, 0x2551, 0x2552, 0x0451, 0x2553, 0x2554, 0x2555, 0x2556, // a0  Russian
  0x2557, 0x2558, 0x2559, 0x255a, 0x255b, 0x255c, 0x255d, 0x255e, // a8
  0x255f, 0x2560, 0x2561, 0x0401, 0x2562, 0x2563, 0x2564, 0x2565, // b0
  0x2566, 0x2567, 0x2568, 0x2569, 0x256a, 0x256b, 0x256c, 0x00a9, // b8
  0x044e, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, // c0
  0x0445, 0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, // c8
  0x043f, 0x044f, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, // d0
  0x044c, 0x044b, 0x0437, 0x0448, 0x044d, 0x0449, 0x0447, 0x044a, // d8
  0x042e, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, // e0
  0x0425, 0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, // e8
  0x041f, 0x042f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, // f0
  0x042c, 0x042b, 0x0417, 0x0428, 0x042d, 0x0429, 0x0427, 0x042a, // f8
};

static const unsigned short ISCII[0x60] =                       {
  0x0020, 0x0901, 0x0902, 0x0903, 0x0905, 0x0906, 0x0907, 0x0908, // a0
  0x0909, 0x090a, 0x090b, 0x090e, 0x090f, 0x0910, 0x090d, 0x0912, // a8
  0x0913, 0x0914, 0x0911, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, // b0
  0x091a, 0x091b, 0x091c, 0x091d, 0x091e, 0x091f, 0x0920, 0x0921, // b8
  0x0922, 0x0923, 0x0924, 0x0925, 0x0926, 0x0927, 0x0928, 0x0929, // c0
  0x092a, 0x092b, 0x092c, 0x092d, 0x092e, 0x092f, 0x095f, 0x0930, // c8
  0x0931, 0x0932, 0x0933, 0x0934, 0x0935, 0x0936, 0x0937, 0x0938, // d0
  0x0939, 0x0020, 0x093e, 0x093f, 0x0940, 0x0941, 0x0942, 0x0943, // d8
  0x0946, 0x0947, 0x0948, 0x0945, 0x094a, 0x094b, 0x094c, 0x0949, // e0
  0x094d, 0x093c, 0x0964, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, // e8
  0x0020, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, // f0
  0x0037, 0x0038, 0x0039, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, // f8
}                                                               ;

static const unsigned short * charsets [ N_CHARSETS ] = {
  ISO_8859_1  ,
  ISO_8859_1  ,
  ISO_8859_2  ,
  ISO_8859_3  ,
  ISO_8859_4  ,
  ISO_8859_5  ,
  ISO_8859_1  ,
  ISO_8859_7  ,
  ISO_8859_1  ,
  ISO_8859_9  ,
  ISO_8859_1  ,
  ISO_8859_1  ,
  ISO_8859_1  ,
  ISO_8859_1  ,
  ISO_8859_14 ,
  ISO_8859_1  ,
  ISO_8859_1  ,
  ISO_8859_1  ,
  KOI8_R      ,
  ISCII
}             ;

// Tables of the relative lengths of vowels, depending on the
// type of the two phonemes that follow
// indexes are the "length_mod" value for the following phonemes
// use this table if vowel is not the last in the word
static unsigned char length_mods_en[100] = {
  /*  a   ,   t   s   n   d   z   r   N   <- next */
  100,120,100,105,100,110,110,100, 95, 100,  /* a  <- next2 */
  105,120,105,110,125,130,135,115,125, 100,  /* , */
  105,120, 75,100, 75,105,120, 85, 75, 100,  /* t */
  105,120, 85,105, 95,115,120,100, 95, 100,  /* s */
  110,120, 95,105,100,115,120,100,100, 100,  /* n */
  105,120,100,105, 95,115,120,110, 95, 100,  /* d */
  105,120,100,105,105,122,125,110,105, 100,  /* z */
  105,120,100,105,105,122,125,110,105, 100,  /* r */
  105,120, 95,105,100,115,120,110,100, 100,  /* N */
  100,120,100,100,100,100,100,100,100, 100
}; // SPARE

// as above, but for the last syllable in a word
static unsigned char length_mods_en0[100] = {
  /*  a   ,   t   s   n   d   z   r    N  <- next */
  100,150,100,105,110,115,110,110,110, 100,  /* a  <- next2 */
  105,150,105,110,125,135,140,115,135, 100,  /* , */
  105,150, 90,105, 90,122,135,100, 90, 100,  /* t */
  105,150,100,105,100,122,135,100,100, 100,  /* s */
  105,150,100,105,105,115,135,110,105, 100,  /* n */
  105,150,100,105,105,122,130,120,125, 100,  /* d */
  105,150,100,105,110,122,125,115,110, 100,  /* z */
  105,150,100,105,105,122,135,120,105, 100,  /* r */
  105,150,100,105,105,115,135,110,105, 100,  /* N */
  100,100,100,100,100,100,100,100,100, 100
}; // SPARE

static unsigned char length_mods_equal[100] = {
  /*  a   ,   t   s   n   d   z   r   N   <- next */
  110,120,100,110,110,110,110,110,110, 110,  /* a  <- next2 */
  110,120,100,110,110,110,110,110,110, 110,  /* , */
  110,120,100,110,100,110,110,110,100, 110,  /* t */
  110,120,100,110,110,110,110,110,110, 110,  /* s */
  110,120,100,110,110,110,110,110,110, 110,  /* n */
  110,120,100,110,110,110,110,110,110, 110,  /* d */
  110,120,100,110,110,110,110,110,110, 110,  /* z */
  110,120,100,110,110,110,110,110,110, 110,  /* r */
  110,120,100,110,110,110,110,110,110, 110,  /* N */
  110,120,100,110,110,110,110,110,110, 110
}; // SPARE

static unsigned char * length_mod_tabs[6] = {
  length_mods_en                            ,
  length_mods_en                            , // 1
  length_mods_en0                           , // 2
  length_mods_equal                         , // 3
  length_mods_equal                         , // 4
  length_mods_equal                           // 5
}                                           ;

static const char transpose_map_latin [] =                      {
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  27, 28, 29,  0,  0, 30, 31, 32, 33, 34, 35, 36,  0, 37, 38,  0,
   0,  0,  0, 39,  0,  0, 40,  0, 41,  0, 42,  0, 43,  0,  0,  0,
   0,  0,  0, 44,  0, 45,  0, 46,  0,  0,  0,  0,  0, 47,  0,  0,
   0, 48,  0,  0,  0,  0,  0,  0,  0, 49,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0, 50,  0, 51,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 52,  0,  0,  0,  0,
   0, 53,  0, 54,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55,  0, 56,  0, 57,  0
}                                                               ;

static const unsigned char stress_amps2        [ ] = { 18, 18, 20, 20, 20, 22, 22, 20} ;
static const short         stress_lengths2     [8] = {182,140,220,220,220,240,260,280} ;
static const wchar_t       empty_wstring       [1] = {0};
static const wchar_t       punct_in_word       [2] = {'\'', 0};  // allow hyphen within words
static const unsigned char default_tunes       [6] = {  0, 1, 2, 3, 0, 0};
static const short         stress_lengths_equal[8] = {230,230,230,230,  0,  0,230,230} ;
static const unsigned char stress_amps_equal   [8] = { 18, 18, 18, 18, 18, 18, 18, 18} ;
static const short         stress_lengths_fr   [8] = {190,170,190,200,  0,  0,190,240} ;
static const unsigned char stress_amps_fr      [8] = { 18, 16, 18, 18, 18, 18, 18, 18} ;
static const unsigned char stress_amps_sk      [8] = { 17, 16, 20, 20, 20, 22, 22, 21} ;
static const short         stress_lengths_sk   [8] = {190,190,210,210,  0,  0,210,210} ;
static const short         stress_lengths_ta   [8] = {200,200,210,210,  0,  0,230,230} ;
static const short         stress_lengths_ta2  [8] = {230,230,240,240,  0,  0,260,260} ;
static const unsigned char stress_amps_ta      [8] = { 18, 18, 18, 18, 20, 20, 22, 22} ;

/* index by 0=. 1=, 2=?, 3=! 4=none, 5=emphasized */
static unsigned char punctuation_to_tone[8][6] = {
  { 0, 1, 2, 3, 0, 4}                            ,
  { 0, 1, 2, 3, 0, 4}                            ,
  { 5, 6, 2, 3, 0, 4}                            ,
  { 5, 7, 1, 3, 0, 4}                            ,
  { 8, 9,10, 3, 0, 0}                            ,
  { 8, 8,10, 3, 0, 0}                            ,
  {11,11,11,11, 0, 0}                            ,
  {12,12,12,12, 0, 0}                          } ;

// ignore these characters
static const unsigned short chars_ignore_default[] = {
  0x00ad , 1                                         , // soft hyphen
  0x200c , 1                                         , // zero width non-joiner
  0x200d , 1                                         , // zero width joiner
  0      , 0                                       } ;

// alternatively, ignore characters but allow zero-width-non-joiner (lang-fa)
static const unsigned short chars_ignore_zwnj_hyphen[] = {
  0x00ad ,  1                                            , // soft hyphen
  0x200c , '-'                                           , // zero width non-joiner, replace with hyphen
  0x200d ,  1                                            , // zero width joiner
  0      ,  0                                          } ;

// common letter pairs, encode these as a single byte
//  2 bytes, using the transposed character codes
static const short pairs_ru[] = {
0x010c, //  ла   21052  0x23
0x010e, //  на   18400
0x0113, //  та   14254
0x0301, //  ав   31083
0x030f, //  ов   13420
0x060e, //  не   21798
0x0611, //  ре   19458
0x0903, //  ви   16226
0x0b01, //  ак   14456
0x0b0f, //  ок   17836
0x0c01, //  ал   13324
0x0c09, //  ил   16877
0x0e01, //  ан   15359
0x0e06, //  ен   13543  0x30
0x0e09, //  ин   17168
0x0e0e, //  нн   15973
0x0e0f, //  он   22373
0x0e1c, //  ын   15052
0x0f03, //  во   24947
0x0f11, //  ро   13552
0x0f12, //  со   16368
0x100f, //  оп   19054
0x1011, //  рп   17067
0x1101, //  ар   23967
0x1106, //  ер   18795
0x1109, //  ир   13797
0x110f, //  ор   21737
0x1213, //  тс   25076
0x1220, //  яс   14310
0x7fff};
//0x040f  ог   12976
//0x1306  ет   12826
//0x0f0d  мо   12688

static const unsigned int replace_cyrillic_latin[] =
{
  0x430 , 'a',
  0x431 , 'b',
  0x446 , 'c',
  0x45b , 0x107,
  0x447 , 0x10d,
  0x45f , 'd'+(0x17e<<16),
  0x455 , 'd'+('z'<<16),
  0x434 , 'd',
  0x452 , 0x111,
  0x435 , 'e',
  0x444 , 'f',
  0x433 , 'g',
  0x445 , 'h',
  0x438 , 'i',
  0x458 , 'j',
  0x43a , 'k',
  0x459 , 'l'+('j'<<16),
  0x43b , 'l',
  0x43c , 'm',
  0x45a , 'n'+('j'<<16),
  0x43d , 'n',
  0x43e , 'o',
  0x43f , 'p',
  0x440 , 'r',
  0x441 , 's',
  0x448 , 0x161,
  0x442 , 't',
  0x443 , 'u',
  0x432 , 'v',
  0x437 , 'z',
  0x436 , 0x17e,
  0x453 , 0x111,
  0x45c , 0x107,
0};  // ѓ  ѕ  ќ

static const unsigned char ru_vowels     [] = {
  0x10,0x15,0x31,0x18,0x1e,0x23,0x2b,0x2d,0x2e,0x2f,
  0xb9,0xc9,0x91,0x8f,0x36,0};  //also kazakh
static const unsigned char ru_consonants [] = {
  0x11,0x12,0x13,0x14,0x16,0x17,0x19,0x1a,0x1b,0x1c,
  0x1d,0x1f,0x20,0x21,0x22,0x24,0x25,0x26,0x27,0x28,
  0x29,0x2a,0x2c,0x73,0x7b,0x83,0x9b,0};
static const char          ru_soft       [] = {0x2c,0x19,0x27,0x29,0} ; // letter group B  [k ts; s;]
static const char          ru_hard       [] = {0x2a,0x16,0x26,0x28,0} ; // letter group H  [S Z ts]
static const char          ru_nothard    [] = {
  0x11,0x12,0x13,0x14,0x17,0x19,0x1a,0x1b,0x1c,0x1d,
  0x1f,0x20,0x21,0x22,0x24,0x25,0x27,0x29,0x2c,0};
static const char          ru_voiced     [] = {0x11,0x12,0x13,0x14,0x16,0x17,0} ; // letter group G  (voiced obstruents)
static const char          ru_ivowels    [] = {0x2c,0x2e,0x2f,0x31,0} ; // letter group Y  (iotated vowels & soft-sign)

static const short         stress_lengths_zh[8] = {230,150,230,230,230,  0,240,250} ; // 1=tone5. end-of-sentence, 6=tone 1&4, 7=tone 2&3
static const unsigned char stress_amps_zh   [ ] = { 22, 16, 22, 22, 22, 22, 22, 22} ;

static const short stress_lengths_vi[8] = {150, 150,  180, 180,  210, 230,  230, 240};
static const unsigned char stress_amps_vi[] = {16,16, 16,16, 22,22, 22,22 };
static wchar_t vowels_vi[] = {
  0x61, 0xe0, 0xe1, 0x1ea3, 0xe3, 0x1ea1,			// a
  0x103, 0x1eb1, 0x1eaf, 0x1eb3, 0x1eb5, 0x1eb7,	// ă
  0xe2, 0x1ea7, 0x1ea5, 0x1ea9, 0x1eab, 0x1ead,	// â
  0x65, 0xe8, 0xe9, 0x1ebb, 0x1ebd, 0x1eb9,			// e
  0xea, 0x1ec1, 0x1ebf, 0x1ec3, 0x1ec5, 0x1ec7,	// i
  0x69, 0xec, 0xed, 0x1ec9, 0x129, 0x1ecb,			// i
  0x6f, 0xf2, 0xf3, 0x1ecf, 0xf5, 0x1ecd,			// o
  0xf4, 0x1ed3, 0x1ed1, 0x1ed5, 0x1ed7, 0x1ed9, 	// ô
  0x1a1, 0x1edd, 0x1edb, 0x1edf, 0x1ee1, 0x1ee3,	// ơ
  0x75, 0xf9, 0xfa, 0x1ee7, 0x169, 0x1ee5,			// u
  0x1b0, 0x1eeb, 0x1ee9, 0x1eed, 0x1eef, 0x1ef1,	// ư
  0x79, 0x1ef3, 0xfd, 0x1ef7, 0x1ef9, 0x1ef5, 0 };	// y

static const unsigned char stress_amps_tr[8] = {18,16, 20,21, 20,21, 21,20 };
static const short stress_lengths_tr[8] = {190,180, 200,230, 0,0, 240,250};
static const short stress_lengths_th[8] = {230,150, 230,230, 230,0, 230,250};
static const unsigned char stress_amps_th[] = {22,16, 22,22, 22,22, 22,22 };
static const short stress_lengths_bn[8] = {180, 180,  210, 210,  0, 0,  230, 240};
static const unsigned char stress_amps_bn[8] = {18,18, 18,18, 20,20, 22,22 };
// Set letter types for Indic scripts, Devanagari, Tamill, etc
static const char dev_consonants2[] = {0x02,0x03,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x7b,0x7c,0x7e,0x7f,0};
static const char dev_vowels2[] = {0x60,0x61, 0x55,0x56,0x57,0x62,0x63,0};  // non-consecutive vowels and vowel-signs
static const short stress_lengths_cy[8] = {170,220, 180,180, 0, 0, 250,270};
static const unsigned char stress_amps_cy[8] = {17,15, 18,18, 0,0, 22,20 };    // 'diminished' is used to mark a quieter, final unstressed syllable
static const short stress_lengths_da[8] = {160,140, 200,200, 0,0, 220,230};
static const short stress_lengths_de[8] = {150,130, 200,200,  0, 0, 270,270};
static const unsigned char stress_amps_de[] = {20,20, 20,20, 20,22, 22,20 };
static const short stress_lengths_sw[8] = {160, 170,  200, 200,    0,   0,  320, 340};
static const unsigned char stress_amps_sw[] = {16,12, 19,19, 20,22, 22,21 };
static const unsigned char stress_amps_sv[] = {16,16, 20,20, 20,22, 22,21 };
static const short stress_lengths_sv[8] = {160,135, 220,220, 0,0, 250,280};
static const short stress_lengths_sq[8] = {150, 150,  180, 180,  0, 0,  300, 300};
static const unsigned char stress_amps_sq[8] = {16,12, 16,16, 20,20, 21,19 };
static const char *sk_voiced = "bdgjlmnrvwzaeiouy";
static const unsigned char stress_amps_ru[] = {16,16, 18,18, 20,24, 24,22 };
static const short stress_lengths_ru[8] = {150,140, 220,220, 0,0, 260,280};
static const char ru_ivowels2[] = {0x2c,0x15,0x18,0x2e,0x2f,0};   // add more vowels to letter group Y  (iotated vowels & soft-sign)
static const short stress_lengths_en[8] = {182,140, 220,220, 0,0, 248,275};
static const short stress_lengths_el[8] = {155, 180,  210, 210,  0, 0,  270, 300};
static const unsigned char stress_amps_el[8] = {15,12, 20,20, 20,22, 22,21 };    // 'diminished' is used to mark a quieter, final unstressed syllable
// character codes offset by 0x380
static const char el_vowels[] = {0x10,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x35,0x37,0x39,0x3f,0x45,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0};
static const char el_fvowels[] = {0x2d,0x2e,0x2f,0x35,0x37,0x39,0x45,0x4d,0}; // ε η ι υ  έ ή ί ύ _
static const char el_voiceless[]= {0x38,0x3a,0x3e,0x40,0x42,0x43,0x44,0x46,0x47,0};  // θ κ ξ π ς σ τ φ χ _
static const char el_consonants[]={0x32,0x33,0x34,0x36,0x38,0x3a,0x3b,0x3c,0x3d,0x3e,0x40,0x41,0x42,0x43,0x44,0x46,0x47,0x48,0};
static const wchar_t el_char_apostrophe[] = {0x3c3,0};  // σ _
static const short stress_lengths_eo[8] = {150, 150,  230, 180,    0,   0,  300, 320};
static const unsigned char stress_amps_eo[] = {16,14, 20,20, 20,22, 22,21 };
static const wchar_t eo_char_apostrophe[2] = {'l',0};
static const short stress_lengths_es[8] = {180, 190,  230, 180,  0, 0,  240, 270};
static const unsigned char stress_amps_es[8] = {16,12, 18,18, 20,20, 20,20 };    // 'diminished' is used to mark a quieter, final unstressed syllable
static const wchar_t ca_punct_within_word[] = {'\'',0xb7,0};   // ca: allow middle-dot within word
static const short stress_lengths_eu[8] = {200, 200,  200, 200,  0, 0,  210, 230};  // very weak stress
static const unsigned char stress_amps_eu[8] = {16,16, 18,18, 18,18, 18,18 };
// Convert characters in the range 0x620 to 0x6cc to the range 1 to 63.
// 0 indicates no translation for this character
static const char transpose_map_fa[] = {
 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,  // 0x620
16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,  0,  0,  0,  0,  0,  // 0x630
 0, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,  // 0x640
42, 43,  0,  0, 44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 0x650
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 0x660
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 45,  0,  // 0x670
 0,  0,  0,  0,  0,  0, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 0x680
 0,  0,  0,  0,  0,  0,  0,  0, 47,  0,  0,  0,  0,  0,  0,  0,  // 0x690
 0,  0,  0,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0, 49,  // 0x6a0
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 0x6b0
50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 51  };           // 0x6c0
static const unsigned char stress_amps_fi[8] = {18,16, 22,22, 20,22, 22,22 };
static const short stress_lengths_fi[8] = {150,180, 200,200, 0,0, 210,250};
static const short stress_lengths_hi[8] = {190, 190,  210, 210,  0, 0,  230, 250};
static const unsigned char stress_amps_hi[8] = {17,14, 20,19, 20,22, 22,21 };
static const short stress_lengths_ro[8] = {170, 170,  180, 180,  0, 0,  240, 260};
static const unsigned char stress_amps_ro[8] = {15,13, 18,18, 20,22, 22,21 };
static const short stress_lengths_pt[8] = {170, 115,  210, 240,  0, 0,  260, 280};
static const unsigned char stress_amps_pt[8] = {16,11, 19,21, 20,22, 22,21 };    // 'diminished' is used to mark a quieter, final unstressed syllable
static const short stress_lengths_pl[8] = {160, 190,  175, 175,  0, 0,  200, 210};
static const unsigned char stress_amps_pl[8] = {17,13, 19,19, 20,22, 22,21 };    // 'diminished' is used to mark a quieter, final unstressed syllable
static const unsigned char stress_amps_om[] = {18,15, 20,20, 20,22, 22,22 };
static const short stress_lengths_om[8] = {200,200, 200,200, 0,0, 200,200};
static const short stress_lengths_no[8] = {160,140, 200,200, 0,0, 220,230};
static const short stress_lengths_nl[8] = {160,135, 210,210,  0, 0, 260,280};
static wchar_t vowels_cyrillic[] = {0x440,  // also include 'р' [R]
         0x430,0x435,0x438,0x439,0x43e,0x443,0x44b,0x44d,0x44e,0x44f,0x450,0x451,0x456,0x457,0x45d,0x45e,0};
static const unsigned char stress_amps_mk[8] = {17,17, 20,20, 20,22, 22,21 };
static const short stress_lengths_mk[8] = {180,160, 200,200, 0,0, 220,230};
static const unsigned char stress_amps_lv[8] = {17,13, 20,20, 20,22, 22,21 };
static const short stress_lengths_lv[8] = {180,130, 210,210, 0,0, 210,210};
static const unsigned char stress_amps_ku[8] = {18,18, 20,20, 20,22, 22,21 };
static const short stress_lengths_ku[8] = {180,180, 190,180, 0,0, 230,240};
static const char ko_ivowels[] = {0x63,0x64,0x67,0x68,0x6d,0x72,0x74,0x75,0};  // y and i vowels
static const unsigned char ko_voiced[] = {0x02,0x05,0x06,0xab,0xaf,0xb7,0xbc,0};  // voiced consonants, l,m,n,N
static const char ka_vowels[] = {0x30,0x34,0x38,0x3d,0x43,0x55,0x57,0};
static const char ka_consonants[] =
        {0x31,0x32,0x33,0x35,0x36,0x37,0x39,0x3a,0x3b,0x3c,0x3e,0x3f,0x40,0x41,0x42,0x44,
        0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x56,0};
static const short stress_lengths_jbo[8] = {145,145, 170,160, 0,0, 330,350};
static const wchar_t jbo_punct_within_word[] = {'.',',','\'',0x2c8,0};  // allow period and comma within a word, also stress marker (from LOPT_CAPS_IN_WORD)
static const short stress_lengths_it[8] = {150, 140,  170, 170,  0, 0,  300, 330};
static const unsigned char stress_amps_it[8] = {15,14, 19,19, 20,22, 22,20 };
static const short stress_lengths_is[8] = {180,160, 200,200, 0,0, 240,250};
static const wchar_t is_lettergroup_B[] = {'c','f','h','k','p','t','x',0xfe,0};  // voiceless conants, including 'þ'  ?? 's'
static const short stress_lengths_id[8] = {160, 200,  180, 180,  0, 0,  220, 240};
static const unsigned char stress_amps_id[8] = {16,18, 18,18, 20,22, 22,21 };
static const short stress_lengths_hy[8] = {250, 200,  250, 250,  0, 0,  250, 250};
static const char hy_vowels[] = {0x31, 0x35, 0x37, 0x38, 0x3b, 0x48, 0x55, 0};
static const char hy_consonants[] = {0x32,0x33,0x34,0x36,0x39,0x3a,0x3c,0x3d,0x3e,0x3f,
        0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x56,0};
static const unsigned char stress_amps_hu[8] = {17,17, 19,19, 20,22, 22,21 };
static const short stress_lengths_hu[8] = {185,195, 195,190, 0,0, 210,220};
static const unsigned char stress_amps_hr[8] = {17,17, 20,20, 20,22, 22,21 };
static const short stress_lengths_hr[8] = {180,160, 200,200, 0,0, 220,230};
static const short stress_lengths_sr[8] = {160,150, 200,200, 0,0, 250,260};

N::SpeechTranslator:: SpeechTranslator(void)
{
  Reset ( ) ;
}

N::SpeechTranslator:: SpeechTranslator(const SpeechTranslator & translator)
{
  assign ( translator ) ;
}

N::SpeechTranslator::~SpeechTranslator(void)
{
}

N::SpeechTranslator & N::SpeechTranslator::assign(const SpeechTranslator & translator)
{
  nMemberCopy ( translator , translatorName      )                     ;
  nMemberCopy ( translator , transposeMax        )                     ;
  nMemberCopy ( translator , transposeMin        )                     ;
  nMemberCopy ( translator , phonemesRepeatCount )                     ;
  nMemberCopy ( translator , phonemeTabIndex     )                     ;
  nMemberCopy ( translator , dictCondition       )                     ;
  nMemberCopy ( translator , dictMinSize         )                     ;
  nMemberCopy ( translator , nGroups2            )                     ;
  nMemberCopy ( translator , expectVerb          )                     ;
  nMemberCopy ( translator , expectPast          )                     ;
  nMemberCopy ( translator , expectVerbs         )                     ;
  nMemberCopy ( translator , expectNoun          )                     ;
  nMemberCopy ( translator , prevLastStress      )                     ;
  nMemberCopy ( translator , wordVowelCount      )                     ;
  nMemberCopy ( translator , wordStressedCount   )                     ;
  nMemberCopy ( translator , clauseUpperCount    )                     ;
  nMemberCopy ( translator , clauseLowerCount    )                     ;
  nMemberCopy ( translator , prepauseTimeout     )                     ;
  nMemberCopy ( translator , endStressedVowel    )                     ;
  nMemberCopy ( translator , clauseTerminator    )                     ;
  nMemberCopy ( translator , letterBitsOffset    )                     ;
  nMemberCopy ( translator , dataDictList        )                     ;
  options = translator.options                                         ;
  memcpy(prevDictFlags ,translator.prevDictFlags ,(sizeof(int  )*  2)) ;
  memcpy(dictionaryName,translator.dictionaryName,(sizeof(char )* 40)) ;
  memcpy(phonemeOut    ,translator.phonemeOut    ,(sizeof(char )*500)) ;
  memcpy(phonemesRepeat,translator.phonemesRepeat,(sizeof(char )* 20)) ;
  memcpy(stressLengths ,translator.stressLengths ,(sizeof(short)*  8)) ;
  memcpy(stressAmps    ,translator.stressAmps    ,(sizeof(char )*  8)) ;
  memcpy(stressAmpsR   ,translator.stressAmpsR   ,(sizeof(char )*  8)) ;
  memcpy(letterBits    ,translator.letterBits    ,(sizeof(char )*256)) ;
  memcpy(groups2Count  ,translator.groups2Count  ,(sizeof(char )*256)) ;
  memcpy(groups2Start  ,translator.groups2Start  ,(sizeof(char )*256)) ;
  memcpy(groups2Name   ,translator.groups2Name   ,(sizeof(int  )*120)) ;
//  unsigned char          punctToTone    [8][6] ;
//  char                 * dataDictRules         ;
//  char                 * clauseEnd             ;
//  const char           * transposeMap          ;
//  const short          * frequentPairs         ;
//  const unsigned short * charsetA0             ;
//  const unsigned short * charsIgnore           ;
//  const wchar_t        * charPlusApostrophe    ;
//  const wchar_t        * punctWithinWord       ;
//  char                 * dictHashTab    [1024] ;
//  char                 * letterGroups   [  95] ;
//  char                 * groups1        [ 256] ;
//  char                 * groups2        [ 120] ;
//  char                 * groups3        [ 128] ;
//  const wchar_t        * wLetterGroups  [   8] ;
  return ME                    ;
}

N::SpeechTranslator & N::SpeechTranslator::operator = (const SpeechTranslator & translator)
{
  return assign ( translator ) ;
}

bool N::SpeechTranslator::Select(const char * name)
{
  int name2  = 0                                       ;
  int length = strlen(name)                            ;
  if (length>2)                                        {
    if (strncmp(name,"grc",3)==0) name2 = L_grc ;   else
    if (strncmp(name,"pap",3)==0) name2 = L_pap ;   else
    if (strncmp(name,"jbo",3)==0) name2 = L_jbo ;   else
    if (strncmp(name,"zhy",3)==0) name2 = L_zhy        ;
  }                                                    ;
  if (name2<=0)                                        {
    if (length==2)                                     {
      while ( (*name) != 0)                            {
        name2 = (name2 << 8) + *name++                 ;
      }                                                ;
    }                                                  ;
  }                                                    ;
  //////////////////////////////////////////////////////
  #define CASTER(A,B,C) case L(A,B) : C () ; return true
  #define DASTER(A,  C) case A      : C () ; return true
  switch (name2)                                       {
    CASTER ( 'a' , 'f' , SelectAF  )                   ;
    CASTER ( 'a' , 'm' , SelectAM  )                   ;
    CASTER ( 'a' , 'n' , SelectAN  )                   ;
    CASTER ( 'a' , 'r' , SelectAR  )                   ;
    CASTER ( 'a' , 'z' , SelectAZ  )                   ;
    CASTER ( 'b' , 'g' , SelectBG  )                   ;
    CASTER ( 'b' , 'n' , SelectBN  )                   ;
    CASTER ( 'b' , 'o' , SelectBO  )                   ;
    CASTER ( 'b' , 's' , SelectBS  )                   ;
    CASTER ( 'c' , 'a' , SelectCA  )                   ;
    CASTER ( 'c' , 's' , SelectCS  )                   ;
    CASTER ( 'c' , 'y' , SelectCY  )                   ;
    CASTER ( 'd' , 'a' , SelectDA  )                   ;
    CASTER ( 'd' , 'e' , SelectDE  )                   ;
    CASTER ( 'd' , 'v' , SelectDV  )                   ;
    CASTER ( 'e' , 'l' , SelectEL  )                   ;
    CASTER ( 'e' , 'n' , SelectEN  )                   ;
    CASTER ( 'e' , 'o' , SelectEO  )                   ;
    CASTER ( 'e' , 's' , SelectES  )                   ;
    CASTER ( 'e' , 't' , SelectET  )                   ;
    CASTER ( 'e' , 'u' , SelectEU  )                   ;
    CASTER ( 'f' , 'a' , SelectFA  )                   ;
    CASTER ( 'f' , 'i' , SelectFI  )                   ;
    CASTER ( 'f' , 'r' , SelectFR  )                   ;
    CASTER ( 'g' , 'u' , SelectGU  )                   ;
    CASTER ( 'h' , 'i' , SelectHI  )                   ;
    CASTER ( 'h' , 'r' , SelectHR  )                   ;
    CASTER ( 'h' , 't' , SelectHT  )                   ;
    CASTER ( 'h' , 'u' , SelectHU  )                   ;
    CASTER ( 'h' , 'y' , SelectHY  )                   ;
    CASTER ( 'i' , 'd' , SelectID  )                   ;
    CASTER ( 'i' , 's' , SelectIS  )                   ;
    CASTER ( 'i' , 't' , SelectIT  )                   ;
    CASTER ( 'j' , 'p' , SelectJP  )                   ;
    CASTER ( 'k' , 'a' , SelectKA  )                   ;
    CASTER ( 'k' , 'k' , SelectKK  )                   ;
    CASTER ( 'k' , 'l' , SelectKL  )                   ;
    CASTER ( 'k' , 'n' , SelectKN  )                   ;
    CASTER ( 'k' , 'o' , SelectKO  )                   ;
    CASTER ( 'k' , 'u' , SelectKU  )                   ;
    CASTER ( 'l' , 'a' , SelectLA  )                   ;
    CASTER ( 'l' , 't' , SelectLT  )                   ;
    CASTER ( 'l' , 'v' , SelectLV  )                   ;
    CASTER ( 'm' , 'k' , SelectMK  )                   ;
    CASTER ( 'm' , 't' , SelectMT  )                   ;
    CASTER ( 'm' , 'l' , SelectML  )                   ;
    CASTER ( 'm' , 'r' , SelectMR  )                   ;
    CASTER ( 'm' , 's' , SelectMS  )                   ;
    CASTER ( 'n' , 'e' , SelectNE  )                   ;
    CASTER ( 'n' , 'l' , SelectNL  )                   ;
    CASTER ( 'n' , 'o' , SelectNO  )                   ;
    CASTER ( 'o' , 'm' , SelectOM  )                   ;
    CASTER ( 'p' , 'a' , SelectPA  )                   ;
    CASTER ( 'p' , 'l' , SelectPL  )                   ;
    CASTER ( 'p' , 't' , SelectPT  )                   ;
    CASTER ( 'r' , 'o' , SelectRO  )                   ;
    CASTER ( 'r' , 'u' , SelectRU  )                   ;
    CASTER ( 'r' , 'w' , SelectRW  )                   ;
    CASTER ( 's' , 'i' , SelectSI  )                   ;
    CASTER ( 's' , 'k' , SelectSK  )                   ;
    CASTER ( 's' , 'l' , SelectSL  )                   ;
    CASTER ( 's' , 'q' , SelectSQ  )                   ;
    CASTER ( 's' , 'r' , SelectSR  )                   ;
    CASTER ( 's' , 'v' , SelectSV  )                   ;
    CASTER ( 's' , 'w' , SelectSW  )                   ;
    CASTER ( 't' , 'a' , SelectTA  )                   ;
    CASTER ( 't' , 'e' , SelectTE  )                   ;
    CASTER ( 't' , 'h' , SelectTH  )                   ;
    CASTER ( 't' , 'n' , SelectTN  )                   ;
    CASTER ( 't' , 'r' , SelectTR  )                   ;
    CASTER ( 't' , 't' , SelectTT  )                   ;
    CASTER ( 'u' , 'k' , SelectUK  )                   ;
    CASTER ( 'u' , 'r' , SelectUR  )                   ;
    CASTER ( 'v' , 'i' , SelectVI  )                   ;
    CASTER ( 'w' , 'o' , SelectWO  )                   ;
    CASTER ( 'z' , 'h' , SelectZH  )                   ;
    DASTER ( L_grc     , SelectGRC )                   ;
    DASTER ( L_pap     , SelectPAP )                   ;
    DASTER ( L_jbo     , SelectJBO )                   ;
    DASTER ( L_zhy     , SelectZHY )                   ;
     // disable check for unpronouncable words
    default                                            :
      options.param[LOPT_UNPRONOUNCABLE] = 1           ;
    break                                              ;
  }                                                    ;
  #undef  CASTER
  #undef  DASTER
  return false                                         ;
}

void N::SpeechTranslator::setLanguage(int name)
{
  translatorName = name                     ;
  if ( options.numbers & NUM_DECIMAL_COMMA) { // use . and ; for thousands and decimal separators
    options.thousandsSep = '.'              ;
    options.decimalSep   = ','              ;
  }                                         ;
  if ( options.numbers & NUM_THOUS_SPACE)   {
    options.thousandsSep = 0                ; // don't allow thousands separator, except space
  }                                         ;
}

void N::SpeechTranslator::SelectAF(void)
{
  static const short stress_lengths_af[8] = {170,140,220,220,  0,  0,250,270} ;
  Setup ( stress_lengths_af , NULL )                                          ;
  options . stressRule              = STRESSPOSN_1L                           ;
  options . vowelPause              = 0x30                                    ;
  options . param [ LOPT_DIERESES ] = 1                                       ;
  options . param [ LOPT_PREFIXES ] = 1                                       ;
  /* add 'y' to vowels                                                       */
  setLetterVowel ( 'y' )                                                      ;
  options . numbers                 = NUM_SWAP_TENS                           |
                                      NUM_HUNDRED_AND                         |
                                      NUM_SINGLE_AND                          |
                                      NUM_ROMAN                               |
                                      NUM_1900                                ;
  options . accents                 = 1                                       ;
  setLanguage ( L('a','f') )                                                  ;
}

void N::SpeechTranslator::SelectAM(void)
{ // Amharic, Ethiopia
  Setup ( stress_lengths_fr , stress_amps_fr )                   ;
  letterBitsOffset                   = OFFSET_ETHIOPIC           ;
  options.stressRule                 = STRESSPOSN_1L             ;
  options.stressFlags                = S_NO_AUTO_2 | S_FINAL_DIM ; // don't use secondary stress
  options.lengthMods0                = options.lengthMods        ; // don't lengthen vowels in the last syllable
  options.param[LOPT_UNPRONOUNCABLE] = 1                         ; // disable check for unpronouncable words
  options.numbers                    = NUM_OMIT_1_HUNDRED        ;
  setLanguage ( L('a','m') )                                     ;
}

void N::SpeechTranslator::SelectAR(void)
{ // Arabic
  letterBitsOffset                   = OFFSET_ARABIC       ;
  options.numbers                    = NUM_SWAP_TENS       |
                                       NUM_AND_UNITS       |
                                       NUM_HUNDRED_AND     |
                                       NUM_OMIT_1_HUNDRED  |
                                       NUM_AND_HUNDRED     |
                                       NUM_THOUSAND_AND    |
                                       NUM_OMIT_1_THOUSAND ;
  options.param[LOPT_UNPRONOUNCABLE] = 1                   ;
  setLanguage ( L('a','r') )                               ;
}

void N::SpeechTranslator::SelectBG(void)
{ // Bulgarian
  setCyrillicLetters (      )                                 ;
  setLetterVowel     ( 0x2a )                                 ;
  charsetA0                              = charsets[5]        ; // ISO-8859-5
  options.param[LOPT_UNPRONOUNCABLE    ] = 0x432              ; // [v]  don't count this character at start of word
  options.param[LOPT_REGRESSIVE_VOICING] = 0x107              ; // devoice at end of word, and change voicing to match a following consonant (except v)
  options.param[LOPT_REDUCE            ] = 2                  ;
  options.stressRule                     = STRESSPOSN_2R      ;
  options.numbers                        = NUM_DECIMAL_COMMA  |
                                           NUM_ALLOW_SPACE    |
                                           NUM_OMIT_1_HUNDRED |
                                           NUM_HUNDRED_AND    |
                                           NUM_AND_UNITS      |
                                           NUM_SINGLE_AND     |
                                           NUM_ROMAN          |
                                           NUM_ROMAN_ORDINAL  |
                                           NUM_ROMAN_CAPITALS ;
  options.thousandsSep                   = ' '                ; // don't allow dot as thousands separator
  setLanguage ( L('b','g') )                                  ;
}

void N::SpeechTranslator::SelectBN(void)
{ // Bengali
  Setup ( stress_lengths_bn , stress_amps_bn )    ;
  options.lengthMods0  = options.lengthMods       ; // don't lengthen vowels in the last syllable
  options.stressRule   = STRESSPOSN_1L            ;
  options.stressFlags  =  S_MID_DIM | S_FINAL_DIM ; // use 'diminished' for unstressed final syllable
  letterBitsOffset     = OFFSET_BENGALI           ;
  setIndicLetters    (                          ) ; // call this after setting OFFSET_BENGALI
  setLetterBitsRange ( LETTERGP_B , 0x01 , 0x01 ) ; // candranindu
  setLetterBitsRange ( LETTERGP_F , 0x3e , 0x4c ) ; // vowel signs, but not virama
  options.numbers      = NUM_SWAP_TENS            ;
  options.breakNumbers = 0x24924aa8               ; // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  setLanguage ( L('b','n') )                      ;
}

void N::SpeechTranslator::SelectBO(void)
{ // Tibet
  options.stressRule                 = STRESSPOSN_1L ;
  letterBitsOffset                   = OFFSET_TIBET  ;
  setLetterBitsRange ( LETTERGP_A , 0x71 , 0x7d )    ; // vowel signs
  setLetterBitsRange ( LETTERGP_B , 0x71 , 0x81 )    ; // vowel signs and subjoined letters
  setLetterBitsRange ( LETTERGP_B , 0x90 , 0xbc )    ;
  setLetterBitsRange ( LETTERGP_C , 0x40 , 0x6c )    ; // consonant letters (not subjoined)
  options.param[LOPT_UNPRONOUNCABLE] = 1             ; // disable check for unpronouncable words
  options.numbers                    = 1             ;
  setLanguage ( L('b','o') )                         ;
}

void N::SpeechTranslator::SelectCY(void)
{ // Welsh
  Setup ( stress_lengths_cy , stress_amps_cy )                       ;
  charsetA0                        = charsets[14]                    ; // ISO-8859-14
  options.stressRule               = STRESSPOSN_2R                   ;
  //  options.lengthMods0              = options.lengthMods              ; // don't lengthen vowels in the last syllable
//  options.intonationGroup         = 4                               ;
// 'diminished' is an unstressed final syllable
  options.stressFlags              = S_FINAL_DIM_ONLY | S_FINAL_NO_2 ;
  options.unstressedWD1            = 0                               ;
  options.unstressedWD2            = 2                               ;
  options.param[LOPT_SONORANT_MIN] = 120                             ; // limit the shortening of sonorants before short vowels
  options.numbers                  = NUM_OMIT_1_HUNDRED              ;
  setLetterVowel ( 'w' )                                             ; // add letter to vowels and remove from consonants
  setLetterVowel ( 'y' )                                             ;
  setLanguage ( L('c','y') )                                         ;
}

void N::SpeechTranslator::SelectDA(void)
{ // Danish
  Setup ( stress_lengths_da , NULL )                ;
  options.stressRule           = STRESSPOSN_1L      ;
  options.param[LOPT_PREFIXES] = 1                  ;
  setLetterVowel('y')                               ;
  options.numbers              = NUM_DECIMAL_COMMA  |
                                 NUM_SWAP_TENS      |
                                 NUM_HUNDRED_AND    |
                                 NUM_OMIT_1_HUNDRED |
                                 NUM_ORDINAL_DOT    |
                                 NUM_1900           |
                                 NUM_ROMAN          |
                                 NUM_ROMAN_CAPITALS |
                                 NUM_ROMAN_ORDINAL  ;
  setLanguage ( L('d','a') )                        ;
}

void N::SpeechTranslator::SelectDE(void)
{
  Setup( stress_lengths_de, stress_amps_de)                    ;
  options.stressRule                       = STRESSPOSN_1L     ;
  options.wordGap                          = 0x8               ; // don't use linking phonemes
  options.vowelPause                       = 0x30              ;
  options.param[LOPT_PREFIXES]             = 1                 ;
  options.param[LOPT_REGRESSIVE_VOICING]   = 0x100             ; // devoice at end of word
  options.param[LOPT_LONG_VOWEL_THRESHOLD] = 175/2             ;
  options.numbers                          = NUM_DECIMAL_COMMA |
                                             NUM_SWAP_TENS     |
                                             NUM_ALLOW_SPACE   |
                                             NUM_ORDINAL_DOT   |
                                             NUM_ROMAN         ;
//  options.numbers = NUM_DECIMAL_COMMA | NUM_SWAP_TENS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_ALLOW_SPACE | NUM_ORDINAL_DOT | NUM_ROMAN;
  setLetterVowel ( 'y' )                                       ;
  options.param[LOPT_UNPRONOUNCABLE]       = 2                 ; // use de_rules for unpronouncable rules
  setLanguage ( L('d','e') )                                   ;
}

void N::SpeechTranslator::SelectDV(void)
{ // Divehi (Maldives)
  Setup(stress_lengths_ta,stress_amps_ta);
  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  letterBitsOffset = OFFSET_THAANA;
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags =  S_MID_DIM | S_FINAL_DIM;   // use 'diminished' for unstressed final syllable
  setLetterBitsRange(LETTERGP_B,0x26,0x30);   // vowel signs, and virama
  options.breakNumbers = 0x14a8;  // 1000, 100,000  10,000,000
  options.numbers = 1;
  setLanguage ( L('d','v') )                                  ;
}

void N::SpeechTranslator::SelectEN(void)
{
  Setup(stress_lengths_en,NULL);
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = 0x08;
  options.numbers = NUM_HUNDRED_AND | NUM_ROMAN | NUM_1900;
  options.param[LOPT_COMBINE_WORDS] = 2;       // allow "mc" to cmbine with the following word
  options.suffixAddE = 'e';
  options.param[LOPT_UNPRONOUNCABLE] = 2;   // use en_rules for unpronouncable rules
  setLetterBits(6,"aeiouy");  // Group Y: vowels, including y
  setLanguage ( L('e','n') )                                  ;
}

void N::SpeechTranslator::SelectEL(void)
{ // Greek
  Setup(stress_lengths_el,stress_amps_el);
  charsetA0 = charsets[7];   // ISO-8859-7
  charPlusApostrophe = el_char_apostrophe;
  letterBitsOffset = OFFSET_GREEK;
  memset(letterBits,0,sizeof(char)*256);
  setLetterBits(LETTERGP_A,el_vowels);
  setLetterBits(LETTERGP_VOWEL2,el_vowels);
  setLetterBits(LETTERGP_B,el_voiceless);
  setLetterBits(LETTERGP_C,el_consonants);
  setLetterBits(LETTERGP_Y,el_fvowels);    // front vowels: ε η ι υ _
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_DIM_ONLY;  // mark unstressed final syllables as diminished
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_SONORANT_MIN] = 130;  // limit the shortening of sonorants before short vowels
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA;
  options.numbers2 = 0x2 | NUM2_MULTIPLE_ORDINAL | NUM2_ORDINAL_NO_AND;   // variant form of numbers before thousands
  setLanguage ( L('e','l') )                                  ;
}

void N::SpeechTranslator::SelectGRC(void)
{ // Ancient Greek
  Setup(stress_lengths_el,stress_amps_el);
  charsetA0 = charsets[7];   // ISO-8859-7
  charPlusApostrophe = el_char_apostrophe;
  letterBitsOffset = OFFSET_GREEK;
  memset(letterBits,0,sizeof(letterBits));
  setLetterBits(LETTERGP_A,el_vowels);
  setLetterBits(LETTERGP_VOWEL2,el_vowels);
  setLetterBits(LETTERGP_B,el_voiceless);
  setLetterBits(LETTERGP_C,el_consonants);
  setLetterBits(LETTERGP_Y,el_fvowels);    // front vowels: ε η ι υ _
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_DIM_ONLY;  // mark unstressed final syllables as diminished
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_SONORANT_MIN] = 130;  // limit the shortening of sonorants before short vowels
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA;
  options.numbers2 = 0x2 | NUM2_MULTIPLE_ORDINAL | NUM2_ORDINAL_NO_AND;   // variant form of numbers before thousands
  // ancient greek
  options.param[LOPT_UNPRONOUNCABLE] = 1;
  setLanguage ( L_grc )                                  ;
}

void N::SpeechTranslator::SelectEO(void)
{
  Setup(stress_lengths_eo,stress_amps_eo);
  charsetA0 = charsets[3];  // ISO-8859-3
  charPlusApostrophe = eo_char_apostrophe;
//  options.wordGap = 1;
  options.vowelPause = 2;
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags =  S_FINAL_DIM_ONLY | S_FINAL_NO_2;
//  options.unstressedWD1 = 3;
  options.unstressedWD2 = 2;
  options.numbers = NUM_DECIMAL_COMMA | NUM_OMIT_1_HUNDRED | NUM_ALLOW_SPACE | NUM_ROMAN;
  setLanguage ( L('e','o') )                                  ;
}

void N::SpeechTranslator::SelectES(void)
{ // Spanish
  Setup(stress_lengths_es,stress_amps_es);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  // stress last syllable if it doesn't end in vowel or "s" or "n"
  // 'diminished' is an unstressed final syllable
  options.stressFlags = S_FINAL_SPANISH | S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_SONORANT_MIN] = 120;  // limit the shortening of sonorants before short vowels
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_ROMAN | NUM_ROMAN_AFTER;
  options.numbers2 = NUM2_MULTIPLE_ORDINAL | NUM2_ORDINAL_NO_AND;
  options.param[LOPT_UNPRONOUNCABLE] = 2;   // use es_rules for unpronouncable rules
  setLanguage ( L('e','s') )                                  ;
}

void N::SpeechTranslator::SelectAN(void)
{ // Aragonese
  Setup(stress_lengths_es,stress_amps_es);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  // stress last syllable if it doesn't end in vowel or "s" or "n"
  // 'diminished' is an unstressed final syllable
  options.stressFlags = S_FINAL_SPANISH | S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_SONORANT_MIN] = 120;  // limit the shortening of sonorants before short vowels
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_ROMAN | NUM_ROMAN_AFTER;
  options.numbers2 = NUM2_MULTIPLE_ORDINAL | NUM2_ORDINAL_NO_AND;
  options.stressFlags = S_FINAL_SPANISH | S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.numbers2 = NUM2_ORDINAL_NO_AND;
  setLanguage ( L('a','n') )                                  ;
}

void N::SpeechTranslator::SelectCA(void)
{ // Catalan
  Setup(stress_lengths_es,stress_amps_es);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  // stress last syllable if it doesn't end in vowel or "s" or "n"
  // 'diminished' is an unstressed final syllable
  options.stressFlags = S_FINAL_SPANISH | S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_SONORANT_MIN] = 120;  // limit the shortening of sonorants before short vowels
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_ROMAN | NUM_ROMAN_AFTER;
  options.numbers2 = NUM2_MULTIPLE_ORDINAL | NUM2_ORDINAL_NO_AND;
  // stress last syllable unless word ends with a vowel
  punctWithinWord = ca_punct_within_word;
  options.stressFlags = S_FINAL_SPANISH | S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_NO_AUTO_2;
  setLanguage ( L('c','a') )                                  ;
}

void N::SpeechTranslator::SelectPAP(void)
{ // Papiamento
  Setup(stress_lengths_es,stress_amps_es);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  // stress last syllable if it doesn't end in vowel or "s" or "n"
  // 'diminished' is an unstressed final syllable
  options.stressFlags = S_FINAL_SPANISH | S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_SONORANT_MIN] = 120;  // limit the shortening of sonorants before short vowels
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_ROMAN | NUM_ROMAN_AFTER;
  options.numbers2 = NUM2_MULTIPLE_ORDINAL | NUM2_ORDINAL_NO_AND;
  // stress last syllable unless word ends with a vowel
  options.stressFlags = S_FINAL_STRESS_C | S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_NO_AUTO_2;
  setLanguage ( L_pap  )                                  ;
}

void N::SpeechTranslator::SelectEU(void)
{ // basque
  Setup(stress_lengths_eu,stress_amps_eu);
  options.stressRule = STRESSPOSN_2L;  // ?? second syllable ??
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA | NUM_AND_UNITS | NUM_HUNDRED_AND | NUM_OMIT_1_HUNDRED | NUM_VIGESIMAL;
  setLanguage ( L('e','u') )                                  ;
}

void N::SpeechTranslator::SelectFA(void)
{ // Farsi
  transposeMin = 0x620;
  transposeMax = 0x6cc;
  transposeMap = transpose_map_fa;
  letterBitsOffset = OFFSET_ARABIC;
  options.numbers = NUM_AND_UNITS | NUM_HUNDRED_AND;
  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words
  charsIgnore = chars_ignore_zwnj_hyphen;   // replace ZWNJ by hyphen
  setLanguage ( L('f','a') )                                  ;
}

void N::SpeechTranslator::SelectET(void)
{ // Estonian
  Setup(stress_lengths_fi,stress_amps_fi);
  charsetA0 = charsets[4];   // ISO-8859-4
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_2_TO_HEAVY;  // move secondary stress from light to a following heavy syllable
  options.param[LOPT_IT_DOUBLING] = 1;
  options.longStop = 130;
  options.numbers = NUM_DECIMAL_COMMA + NUM_ALLOW_SPACE;
  setLetterVowel('y');
//  options.maxInitialConsonants = 2;  // BUT foreign words may have 3
  options.spellingStress = 1;
  options.intonationGroup = 3;  // less intonation, don't raise pitch at comma
  setLanguage ( L('e','t') )                                  ;
}

void N::SpeechTranslator::SelectFI(void)
{ // Finnish
  Setup(stress_lengths_fi,stress_amps_fi);
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_2_TO_HEAVY;  // move secondary stress from light to a following heavy syllable
  options.param[LOPT_IT_DOUBLING] = 1;
  options.longStop = 130;
  options.numbers = NUM_DECIMAL_COMMA + NUM_ALLOW_SPACE;
  setLetterVowel('y');
//  options.maxInitialConsonants = 2;  // BUT foreign words may have 3
  options.spellingStress = 1;
  options.intonationGroup = 3;  // less intonation, don't raise pitch at comma
  setLanguage ( L('f','i') )                                  ;
}

void N::SpeechTranslator::SelectFR(void)
{ // french
  Setup(stress_lengths_fr,stress_amps_fr);
  options.stressRule = STRESSPOSN_1R;      // stress on final syllable
  options.stressFlags = S_NO_AUTO_2 | S_FINAL_DIM;  // don't use secondary stress
  options.param[LOPT_IT_LENGTHEN] = 1;    // remove lengthen indicator from unstressed syllables
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.accents = 2;   // Say "Capital" after the letter.
  options.numbers = NUM_SINGLE_STRESS | NUM_DECIMAL_COMMA | NUM_ALLOW_SPACE | NUM_OMIT_1_HUNDRED | NUM_NOPAUSE | NUM_ROMAN | NUM_ROMAN_CAPITALS | NUM_ROMAN_AFTER | NUM_VIGESIMAL | NUM_DFRACTION_4;
  setLetterVowel('y');
  setLanguage ( L('f','r') )                                  ;
}

void N::SpeechTranslator::SelectGA(void)
{ // irish
  options.stressRule = STRESSPOSN_1L;
  options.numbers = 1;
  options.accents = 2;  // 'capital' after letter name
  options.param[LOPT_UNPRONOUNCABLE] = 3;  // don't count apostrophe
  setLanguage ( L('g','a') )                                  ;
}

void N::SpeechTranslator::SelectHI(void)
{  // Hindi
  Setup(stress_lengths_hi,stress_amps_hi);
  charsetA0 = charsets[19];   // ISCII
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = 6;      // stress on last heaviest syllable, excluding final syllable
  options.stressFlags =  S_MID_DIM | S_FINAL_DIM;   // use 'diminished' for unstressed final syllable
  options.numbers = NUM_SWAP_TENS;
  options.breakNumbers = 0x14aa8;  // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  letterBitsOffset = OFFSET_DEVANAGARI;
  setIndicLetters ( ) ;
  setLanguage ( L('h','i') )                                  ;
}

void N::SpeechTranslator::SelectNE(void)
{ // Nepali
  Setup(stress_lengths_hi,stress_amps_hi);
  charsetA0 = charsets[19];   // ISCII
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = 6;      // stress on last heaviest syllable, excluding final syllable
  options.stressFlags =  S_MID_DIM | S_FINAL_DIM;   // use 'diminished' for unstressed final syllable
  options.numbers = NUM_SWAP_TENS;
  options.breakNumbers = 0x14aa8;  // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  letterBitsOffset = OFFSET_DEVANAGARI;
  Setup(stress_lengths_equal,stress_amps_equal);
  options.breakNumbers = 0x2aaaa8;
  options.maxDigits = 22;
  options.numbers2 |= NUM2_ENGLISH_NUMERALS;
  setIndicLetters ( ) ;
  setLanguage ( L('n','e') )                                  ;
}

void N::SpeechTranslator::SelectPA(void)
{ // Punjabi
  Setup(stress_lengths_hi,stress_amps_hi);
  charsetA0 = charsets[19];   // ISCII
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = 6;      // stress on last heaviest syllable, excluding final syllable
  options.stressFlags =  S_MID_DIM | S_FINAL_DIM;   // use 'diminished' for unstressed final syllable
  options.numbers = NUM_SWAP_TENS;
  options.breakNumbers = 0x14aa8;  // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  letterBitsOffset = OFFSET_DEVANAGARI;
  letterBitsOffset = OFFSET_GURMUKHI;
  setIndicLetters ( ) ;
  setLanguage ( L('p','a') )                                  ;
}

void N::SpeechTranslator::SelectGU(void)
{ // Gujarati
  Setup(stress_lengths_hi,stress_amps_hi);
  charsetA0 = charsets[19];   // ISCII
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = 6;      // stress on last heaviest syllable, excluding final syllable
  options.stressFlags =  S_MID_DIM | S_FINAL_DIM;   // use 'diminished' for unstressed final syllable
  options.numbers = NUM_SWAP_TENS;
  options.breakNumbers = 0x14aa8;  // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  letterBitsOffset = OFFSET_DEVANAGARI;
  Setup(stress_lengths_equal,stress_amps_equal);
  letterBitsOffset = OFFSET_GUJARATI;
  options.stressRule = STRESSPOSN_2R;
  setIndicLetters ( ) ;
  setLanguage ( L('g','u') )                                  ;
}

void N::SpeechTranslator::SelectHR(void)
{ // Croatian
  Setup(stress_lengths_hr,stress_amps_hr);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_NO_2;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x3;
  options.maxInitialConsonants = 5;
  options.spellingStress = 1;
  options.accents = 1;
  options.numbers = NUM_SINGLE_STRESS | NUM_HUNDRED_AND | NUM_OMIT_1_HUNDRED | NUM_DECIMAL_COMMA | NUM_THOUS_SPACE | NUM_DFRACTION_2 | NUM_ROMAN_CAPITALS;
  options.numbers2 = 0xa + NUM2_THOUSANDS_VAR5;  // variant numbers before thousands,milliards
  options.replaceChars = replace_cyrillic_latin;
  options.ourAlphabet = OFFSET_CYRILLIC;   // don't say "cyrillic" before letter names
  setLetterVowel('y');
  setLetterVowel('r');
  setLanguage ( L('h','r') )                                  ;
}

void N::SpeechTranslator::SelectBS(void)
{ // Bosnian
  Setup(stress_lengths_hr,stress_amps_hr);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_NO_2;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x3;
  options.maxInitialConsonants = 5;
  options.spellingStress = 1;
  options.accents = 1;
  options.numbers = NUM_SINGLE_STRESS | NUM_HUNDRED_AND | NUM_OMIT_1_HUNDRED | NUM_DECIMAL_COMMA | NUM_THOUS_SPACE | NUM_DFRACTION_2 | NUM_ROMAN_CAPITALS;
  options.numbers2 = 0xa + NUM2_THOUSANDS_VAR5;  // variant numbers before thousands,milliards
  options.replaceChars = replace_cyrillic_latin;
  options.ourAlphabet = OFFSET_CYRILLIC;   // don't say "cyrillic" before letter names
  setLetterVowel('y');
  setLetterVowel('r');
  setLanguage ( L('b','s') )                                  ;
}

void N::SpeechTranslator::SelectSR(void)
{ // Serbian
  Setup(stress_lengths_sr,stress_amps_hr);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_NO_2;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x3;
  options.maxInitialConsonants = 5;
  options.spellingStress = 1;
  options.accents = 1;
  options.numbers = NUM_SINGLE_STRESS | NUM_HUNDRED_AND | NUM_OMIT_1_HUNDRED | NUM_DECIMAL_COMMA | NUM_THOUS_SPACE | NUM_DFRACTION_2 | NUM_ROMAN_CAPITALS;
  options.numbers2 = 0xa + NUM2_THOUSANDS_VAR5;  // variant numbers before thousands,milliards
  options.replaceChars = replace_cyrillic_latin;
  options.ourAlphabet = OFFSET_CYRILLIC;   // don't say "cyrillic" before letter names
  setLetterVowel('y');
  setLetterVowel('r');
  setLanguage ( L('s','r') )                                  ;
}

void N::SpeechTranslator::SelectHT(void)
{ // Haitian Creole
//  memcpy(stressLengths,stress_lengths_fr,sizeof(stressLengths));
  options.stressRule = STRESSPOSN_1R;      // stress on final syllable
  options.stressFlags = S_NO_AUTO_2 | S_FINAL_DIM;  // don't use secondary stress
  options.numbers = NUM_SINGLE_STRESS | NUM_OMIT_1_HUNDRED | NUM_NOPAUSE | NUM_ROMAN | NUM_VIGESIMAL | NUM_DFRACTION_4;
  setLanguage ( L('h','t') )                                  ;
}

void N::SpeechTranslator::SelectHU(void)
{ // Hungarian
  Setup(stress_lengths_hu,stress_amps_hu);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.vowelPause = 0x20;
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_NO_AUTO_2 | 0x8000 | S_HYPEN_UNSTRESS;
  options.unstressedWD1 = 2;
  options.param[LOPT_IT_DOUBLING] = 1;
  options.param[LOPT_ANNOUNCE_PUNCT] = 2;  // don't break clause before announcing . ? !
  options.numbers = NUM_DFRACTION_5 | NUM_ALLOW_SPACE | NUM_ROMAN | NUM_ROMAN_ORDINAL | NUM_ROMAN_CAPITALS | NUM_ORDINAL_DOT | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND;
  options.thousandsSep = ' ';   // don't allow dot as thousands separator
  options.decimalSep = ',';
  options.maxRoman = 899;
  options.minRoman = 1;
  setLetterVowel('y');
  options.spellingStress = 1;
  setLengthMods(3);  // all equal
  setLanguage ( L('h','u') )                                  ;
}

void N::SpeechTranslator::SelectHY(void)
{ // Armenian
  Setup(stress_lengths_hy,NULL);
  options.stressRule = STRESSPOSN_1R;  // default stress on final syllable
  letterBitsOffset = OFFSET_ARMENIAN;
  memset(letterBits,0,sizeof(char)*256);
  setLetterBits(LETTERGP_A,hy_vowels);
  setLetterBits(LETTERGP_VOWEL2,hy_vowels);
  setLetterBits(LETTERGP_C,hy_consonants);
  options.maxInitialConsonants = 6;
  options.numbers = NUM_DECIMAL_COMMA | NUM_ALLOW_SPACE | NUM_OMIT_1_HUNDRED;
//  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words
  setLanguage ( L('h','y') )                                  ;
}

void N::SpeechTranslator::SelectID(void)
{ // Indonesian
  Setup(stress_lengths_id,stress_amps_id);
  options.stressRule = STRESSPOSN_2R;
  options.numbers = NUM_DECIMAL_COMMA | NUM_ALLOW_SPACE | NUM_ROMAN;
  options.stressFlags =  S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.accents = 2;  // "capital" after letter name
  setLanguage ( L('i','d') )                                  ;
}

void N::SpeechTranslator::SelectMS(void)
{ // Malay
  Setup(stress_lengths_id,stress_amps_id);
  options.stressRule = STRESSPOSN_2R;
  options.numbers = NUM_DECIMAL_COMMA | NUM_ALLOW_SPACE | NUM_ROMAN;
  options.stressFlags =  S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.accents = 2;  // "capital" after letter name
  setLanguage ( L('m','s') )                                  ;
}

void N::SpeechTranslator::SelectIS(void)
{ // Icelandic
  Setup(stress_lengths_is,NULL);
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_NO_2;
  options.param[LOPT_IT_LENGTHEN] = 0x11;    // remove lengthen indicator from unstressed vowels
  options.param[LOPT_REDUCE] = 2;
  resetLetterBits(0x18);
  setLetterBits(4,"kpst");   // Letter group F
  setLetterBits(3,"jvr");    // Letter group H
  wLetterGroups[1] = is_lettergroup_B;
  setLetterVowel('y');
  options.numbers = NUM_DECIMAL_COMMA | NUM_SINGLE_AND | NUM_HUNDRED_AND | NUM_AND_UNITS | NUM_1900;
  options.numbers2 = 0x2;
  setLanguage ( L('i','s') )                                  ;
}

void N::SpeechTranslator::SelectIT(void)
{ // Italian
  Setup(stress_lengths_it,stress_amps_it);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_NO_2 | S_PRIORITY_STRESS;
  options.vowelPause = 1;
  options.unstressedWD1 = 2;
  options.unstressedWD2 = 2;
  options.param[LOPT_IT_LENGTHEN] = 2;    // remove lengthen indicator from unstressed or non-penultimate syllables
  options.param[LOPT_IT_DOUBLING] = 2;    // double the first consonant if the previous word ends in a stressed vowel
  options.param[LOPT_SONORANT_MIN] = 130;  // limit the shortening of sonorants before short vowels
  options.param[LOPT_REDUCE] = 1;        // reduce vowels even if phonemes are specified in it_list
  options.param[LOPT_ALT] = 2;      // call ApplySpecialAttributes2() if a word has $alt or $alt2
  options.numbers = NUM_SINGLE_VOWEL | NUM_OMIT_1_HUNDRED |NUM_DECIMAL_COMMA | NUM_ROMAN | NUM_DFRACTION_1;
  options.accents = 2;   // Say "Capital" after the letter.
  setLetterVowel('y');
  setLanguage ( L('i','t') )                                  ;
}

void N::SpeechTranslator::SelectJBO(void)
{ // Lojban
  Setup(stress_lengths_jbo,NULL);
  options.stressRule = STRESSPOSN_2R;
  options.vowelPause = 0x20c;  // pause before a word which starts with a vowel, or after a word which ends in a consonant
//  options.wordGap = 1;
  punctWithinWord = jbo_punct_within_word;
  options.param[LOPT_CAPS_IN_WORD] = 2;  // capitals indicate stressed syllables
  setLetterVowel('y');
  options.maxLengthMod = 368;
  setLanguage ( L_jbo )                                  ;
}

void N::SpeechTranslator::SelectJP(void)
{ // Japanese
  setLanguage ( L('j','p') ) ;
}

void N::SpeechTranslator::SelectKA(void)
{ // Georgian
  // character codes offset by 0x1080
  Setup(stress_lengths_ta,stress_amps_ta);
  memset(letterBits,0,sizeof(char)*256);
  setLetterBits(LETTERGP_A,ka_vowels);
  setLetterBits(LETTERGP_C,ka_consonants);
  setLetterBits(LETTERGP_VOWEL2,ka_vowels);
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_NO_2;
  letterBitsOffset = OFFSET_GEORGIAN;
//  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words
  options.maxInitialConsonants = 7;
  options.numbers = NUM_VIGESIMAL | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED |NUM_OMIT_1_THOUSAND | NUM_DFRACTION_5 | NUM_ROMAN;
  options.altAlphabet = OFFSET_CYRILLIC;
  options.altAlphabetLanguage = L('r','u');
  setLanguage ( L('k','a') )                                  ;
}

void N::SpeechTranslator::SelectKK(void)
{ // Kazakh
  letterBitsOffset = OFFSET_CYRILLIC;
  memset(letterBits,0,sizeof(char)*256);
  setLetterBits(LETTERGP_A,(char *)ru_vowels);
  setLetterBits(LETTERGP_C,(char *)ru_consonants);
  setLetterBits(LETTERGP_VOWEL2,(char *)ru_vowels);
  Setup(stress_lengths_tr,stress_amps_tr);
  options.stressRule = 7;   // stress on the last syllable, before any explicitly unstressed syllable
  options.stressFlags = S_NO_AUTO_2 + S_NO_EOC_LENGTHEN;  //no automatic secondary stress, don't lengthen at end-of-clause
  options.lengthenTonic = 0;
  options.param[LOPT_SUFFIX] = 1;
  options.numbers =  NUM_OMIT_1_HUNDRED | NUM_DFRACTION_6 ;
  options.maxInitialConsonants = 2;
  setLengthMods(3);  // all equal
  setLanguage ( L('k','k') )                                  ;
}

void N::SpeechTranslator::SelectKL(void)
{ // Greenlandic
  Setup(stress_lengths_equal,stress_amps_equal);
  options.stressRule = 12;
  options.stressFlags = S_NO_AUTO_2;
  options.numbers = NUM_DECIMAL_COMMA | NUM_SWAP_TENS | NUM_HUNDRED_AND | NUM_OMIT_1_HUNDRED | NUM_ORDINAL_DOT | NUM_1900 | NUM_ROMAN | NUM_ROMAN_CAPITALS | NUM_ROMAN_ORDINAL;
  setLanguage ( L('k','l') )                                  ;
}

void N::SpeechTranslator::SelectKO(void)
{ // Korean, TEST
  letterBitsOffset = OFFSET_KOREAN;
  options.ourAlphabet = 0xa700;
  memset(letterBits,0,sizeof(char)*256);
  setLetterBitsRange(LETTERGP_A,0x61,0x75);
  setLetterBits(LETTERGP_Y,ko_ivowels);
  setLetterBits(LETTERGP_G,(const char *)ko_voiced);
  options.stressRule = 8;   // ?? 1st syllable if it is heavy, else 2nd syllable
  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words
  options.numbers = NUM_OMIT_1_HUNDRED;
  options.numbers2 = NUM2_MYRIADS;
  options.breakNumbers = 0x1111110;
  options.maxDigits = 20;
  setLanguage ( L('k','o') )                                  ;
}

void N::SpeechTranslator::SelectKU(void)
{ // Kurdish
  Setup(stress_lengths_ku,stress_amps_ku);
  charsetA0 = charsets[9];   // ISO-8859-9 - Latin5
  options.stressRule = 7;   // stress on the last syllable, before any explicitly unstressed syllable
  options.numbers = NUM_HUNDRED_AND | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED | NUM_AND_HUNDRED;
  options.maxInitialConsonants = 2;
  setLanguage ( L('k','u') )                                  ;
}

void N::SpeechTranslator::SelectLA(void)
{ // Latin
  charsetA0 = charsets[4];   // ISO-8859-4, includes a,e,i,o,u-macron
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_NO_AUTO_2;
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_DIERESES] = 1;
  options.numbers = NUM_ROMAN;
  options.maxRoman = 5000;
  setLanguage ( L('l','a') )                                  ;
}

void N::SpeechTranslator::SelectLT(void)
{ // Lithuanian
  charsetA0 = charsets[4]; // ISO-8859-4
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_NO_AUTO_2;
  options.unstressedWD1 = 0;
  options.unstressedWD2 = 2;
  options.param[LOPT_DIERESES] = 1;
  options.numbers = NUM_DECIMAL_COMMA | NUM_OMIT_1_HUNDRED | NUM_DFRACTION_4 | NUM_ORDINAL_DOT;
  options.numbers2 = NUM2_THOUSANDS_VAR4;
  options.maxRoman = 5000;
  setLanguage ( L('l','t') )                                  ;
}

void N::SpeechTranslator::SelectLV(void)
{ // latvian
  Setup(stress_lengths_lv,stress_amps_lv);
  options.stressRule = STRESSPOSN_1L;
  options.spellingStress = 1;
  charsetA0 = charsets[4];   // ISO-8859-4
  options.numbers = NUM_DECIMAL_COMMA | NUM_OMIT_1_HUNDRED | NUM_DFRACTION_4 | NUM_ORDINAL_DOT;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_EO_CLAUSE1;
  setLanguage ( L('l','v') )                                  ;
}

void N::SpeechTranslator::SelectMK(void)
{ // Macedonian
  Setup(stress_lengths_mk,stress_amps_mk);
  charsetA0 = charsets[5];   // ISO-8859-5
  wLetterGroups[0] = vowels_cyrillic ;
  wLetterGroups[7] = vowels_cyrillic ;
  letterBitsOffset = OFFSET_CYRILLIC ;
  options.stressRule = STRESSPOSN_3R ; // antipenultimate
  options.numbers = NUM_DECIMAL_COMMA | NUM_AND_UNITS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_DFRACTION_2;
  options.numbers2 = 0x8a;  // variant numbers before thousands,milliards
  setLanguage ( L('m','k') )                                  ;
}

void N::SpeechTranslator::SelectMT(void)
{ // Maltese
  charsetA0 = charsets[3];   // ISO-8859-3
  options.param[LOPT_REGRESSIVE_VOICING] = 0x100;  // devoice at end of word
  options.stressRule = STRESSPOSN_2R;   // penultimate
  options.numbers = 1;
  setLanguage ( L('m','t') )                                  ;
}

void N::SpeechTranslator::SelectNL(void)
{ // Dutch
  options.stressRule = STRESSPOSN_1L;
  options.vowelPause = 0x30;  // ??
  options.param[LOPT_DIERESES] = 1;
  options.param[LOPT_PREFIXES] = 1;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x100;  // devoice at end of word
  setLetterVowel('y');
  options.numbers = NUM_DECIMAL_COMMA | NUM_SWAP_TENS | NUM_OMIT_1_HUNDRED | NUM_OMIT_1_THOUSAND | NUM_ALLOW_SPACE | NUM_1900 | NUM_ORDINAL_DOT;
  options.ordinalIndicator = "e";
  options.stressFlags = S_FIRST_PRIMARY;
  memcpy(stressLengths,stress_lengths_nl,sizeof(short)*8);
  setLanguage ( L('n','l') )                                  ;
}

void N::SpeechTranslator::SelectNO(void)
{ // Norwegian
  Setup(stress_lengths_no,NULL);
  options.stressRule = STRESSPOSN_1L;
  setLetterVowel('y');
  options.numbers = NUM_DECIMAL_COMMA | NUM_HUNDRED_AND | NUM_ALLOW_SPACE | NUM_1900 | NUM_ORDINAL_DOT;
  setLanguage ( L('n','o') )                                  ;
}

void N::SpeechTranslator::SelectOM(void)
{
  Setup(stress_lengths_om,stress_amps_om);
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2 | 0x80000;
  setLanguage ( L('o','m') )                                  ;
}

void N::SpeechTranslator::SelectPL(void)
{ // Polish
  Setup(stress_lengths_pl,stress_amps_pl);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_DIM_ONLY;  // mark unstressed final syllables as diminished
  options.param[LOPT_REGRESSIVE_VOICING] = 0x9;
  options.maxInitialConsonants = 7; // for example: wchrzczony :)
  options.numbers = NUM_DECIMAL_COMMA | NUM_ALLOW_SPACE | NUM_DFRACTION_2;
  options.numbers2 = NUM2_THOUSANDS_VAR3;
  options.param[LOPT_COMBINE_WORDS] = 4 + 0x100;  // combine 'nie' (marked with $alt2) with some 1-syllable (and 2-syllable) words (marked with $alt)
  setLetterVowel('y');
  setLanguage ( L('p','l') )                                  ;
}

void N::SpeechTranslator::SelectPT(void)
{ // Portuguese
  Setup(stress_lengths_pt,stress_amps_pt);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_1R;        // stress on final syllable
  options.stressFlags =  S_FINAL_DIM_ONLY | S_FINAL_NO_2 | S_INITIAL_2 | S_PRIORITY_STRESS;
  options.numbers = NUM_DECIMAL_COMMA | NUM_DFRACTION_2 | NUM_HUNDRED_AND | NUM_AND_UNITS | NUM_ROMAN;
  options.numbers2 = NUM2_MULTIPLE_ORDINAL | NUM2_NO_TEEN_ORDINALS | NUM2_ORDINAL_NO_AND;
  setLetterVowel('y');
  resetLetterBits(0x2);
  setLetterBits(1,"bcdfgjkmnpqstvxz");      // B  hard consonants, excluding h,l,r,w,y
  options.param[LOPT_ALT] = 2;      // call ApplySpecialAttributes2() if a word has $alt or $alt2
  options.accents = 2;  // 'capital' after letter name
  setLanguage ( L('p','t') )                                  ;
}

void N::SpeechTranslator::SelectRO(void)
{ // Romanian
  Setup(stress_lengths_ro,stress_amps_ro);
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_STRESS_C + S_FINAL_DIM_ONLY;
  charsetA0 = charsets[2];   // ISO-8859-2
  options.numbers = NUM_DECIMAL_COMMA | NUM_ALLOW_SPACE | NUM_DFRACTION_3 | NUM_AND_UNITS | NUM_ROMAN;
  options.numbers2 = 0x1e;  // variant numbers before all thousandplex
  setLanguage ( L('r','o') )                                  ;
}

void N::SpeechTranslator::SelectRU(void)
{ // Modern Russian
  Setup ( stress_lengths_ru , stress_amps_ru ) ;
  setCyrillicLetters ( ) ;
  setLetterBits(6,ru_ivowels2);
  options.param[LOPT_UNPRONOUNCABLE] = 0x432;    // [v]  don't count this character at start of word
  options.param[LOPT_REGRESSIVE_VOICING] = 1;
  options.param[LOPT_REDUCE] = 2;
  options.stressRule = 5;
  options.stressFlags = S_NO_AUTO_2;
  options.numbers = NUM_DECIMAL_COMMA | NUM_OMIT_1_HUNDRED;
  options.numbers2 = 0x2 + NUM2_THOUSANDS_VAR1;  // variant numbers before thousands
  options.phonemeChange = 1;
  options.testing = 2;
  setLanguage ( L('r','u') )                                  ;
}

void N::SpeechTranslator::SelectRW(void)
{ // Kiryarwanda
  options.stressRule = STRESSPOSN_2R;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words.  Need to allow "bw'" prefix
  options.numbers = NUM_HUNDRED_AND | NUM_AND_UNITS | NUM_DFRACTION_2 | NUM_AND_HUNDRED;
  options.numbers2 = 0x200;  // say "thousands" before its number
  setLanguage ( L('r','w') )                                  ;
}

void N::SpeechTranslator::SelectSK(void)
{ // Slovak
  Setup(stress_lengths_sk,stress_amps_sk);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x3;
  options.maxInitialConsonants = 5;
  options.spellingStress = 1;
  options.param[LOPT_COMBINE_WORDS] = 4;  // combine some prepositions with the following word
  options.numbers = NUM_OMIT_1_HUNDRED | NUM_DFRACTION_2 | NUM_ROMAN;
  options.numbers2 = NUM2_THOUSANDS_VAR2;
  options.thousandsSep = STRESSPOSN_1L;   //no thousands separator
  options.decimalSep = ',';
  setLetterVowel('y');
  setLetterVowel('r');
  resetLetterBits(0x20);
  setLetterBits(5,sk_voiced);
  setLanguage ( L('s','k') )                                  ;
}

void N::SpeechTranslator::SelectCS(void)
{ // Czech
  Setup(stress_lengths_sk,stress_amps_sk);
  charsetA0 = charsets[2];   // ISO-8859-2
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags = S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x3;
  options.maxInitialConsonants = 5;
  options.spellingStress = 1;
  options.param[LOPT_COMBINE_WORDS] = 4;  // combine some prepositions with the following word
  options.numbers = NUM_OMIT_1_HUNDRED | NUM_DFRACTION_2 | NUM_ROMAN;
  options.numbers2 = NUM2_THOUSANDS_VAR2;
  options.thousandsSep = STRESSPOSN_1L;   //no thousands separator
  options.decimalSep = ',';
  options.numbers2 = 0x108;  // variant numbers before milliards
  setLetterVowel('y');
  setLetterVowel('r');
  resetLetterBits(0x20);
  setLetterBits(5,sk_voiced);
  setLanguage ( L('c','s') )                                  ;
}

void N::SpeechTranslator::SelectSI(void)
{ // Sinhala
  Setup(stress_lengths_ta,stress_amps_ta);
  options.lengthMods0 = options.lengthMods;  // don't lengthen vowels in the last syllable
  options.stressRule = STRESSPOSN_1L;
  options.stressFlags =  S_FINAL_DIM_ONLY | S_FINAL_NO_2;
  options.spellingStress = 1;
  letterBitsOffset = OFFSET_SINHALA;
  memset ( letterBits , 0 , sizeof(char) * 256 ) ;
  setLetterBitsRange(LETTERGP_A,0x05,0x16);   // vowel letters
  setLetterBitsRange(LETTERGP_A,0x4a,0x73);   // + vowel signs, and virama
  setLetterBitsRange(LETTERGP_B,0x4a,0x73);   // vowel signs, and virama
  setLetterBitsRange(LETTERGP_C,0x1a,0x46);   // the main consonant range
  options.param[LOPT_UNPRONOUNCABLE] = 1;   // disable check for unpronouncable words
  options.suffixAddE = letterBitsOffset + 0x4a;   //virama
  options.numbers =  NUM_OMIT_1_THOUSAND | NUM_SINGLE_STRESS_L | NUM_DFRACTION_7;
  options.numbers2 =  NUM2_PERCENT_BEFORE;
  options.breakNumbers = 0x14aa8;  // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  setLanguage ( L('s','i') )                                  ;
}

void N::SpeechTranslator::SelectSL(void)
{ // Slovenian
  charsetA0                              = charsets[2]        ; // ISO-8859-2
  options.stressRule                     = STRESSPOSN_2R      ; // Temporary
  options.stressFlags                    = S_NO_AUTO_2        ;
  options.param[LOPT_REGRESSIVE_VOICING] = 0x103              ;
  options.param[LOPT_UNPRONOUNCABLE    ] = 0x76               ; // [v]  don't count this character at start of word
  options.param[LOPT_ALT               ] = 2                  ; // call ApplySpecialAttributes2() if a word has $alt or $alt2
  options.param[LOPT_IT_LENGTHEN       ] = 1                  ; // remove lengthen indicator from unstressed syllables
  letterBits['r']                       |= 0x80               ; // add 'r' to letter group 7, vowels for Unpronouncable test
  options.numbers                        = NUM_DECIMAL_COMMA  |
                                           NUM_ALLOW_SPACE    |
                                           NUM_SWAP_TENS      |
                                           NUM_OMIT_1_HUNDRED |
                                           NUM_DFRACTION_2    |
                                           NUM_ORDINAL_DOT    |
                                           NUM_ROMAN          ;
  options.numbers2                       = 0x100              ; // plural forms of millions etc
  options.thousandsSep                   = ' '                ; // don't allow dot as thousands separator
  setLanguage ( L('s','l') )                                  ;
}

void N::SpeechTranslator::SelectSQ(void)
{ // Albanian
  Setup ( stress_lengths_sq , stress_amps_sq ) ;
  options.stressRule  = STRESSPOSN_2R          ;
  options.stressFlags = S_FINAL_DIM_ONLY       |
                        S_FINAL_NO_2           |
                        S_FINAL_STRESS_C       ;
  setLetterVowel ( 'y' )                       ;
  options.numbers     = NUM_DECIMAL_COMMA      |
                        NUM_HUNDRED_AND        |
                        NUM_AND_UNITS          |
                        NUM_DFRACTION_4        ;
  options.accents     = 2                      ; // "capital" after letter name
  setLanguage ( L('s','q') )                   ;
}

void N::SpeechTranslator::SelectSV(void)
{ // Swedish
  Setup ( stress_lengths_sv , stress_amps_sv ) ;
  options.stressRule = STRESSPOSN_1L           ;
  setLetterVowel ( 'y' )                       ;
  options.numbers    = NUM_SINGLE_STRESS       |
                       NUM_DECIMAL_COMMA       |
                       NUM_ALLOW_SPACE         |
                       NUM_1900                ;
  options.accents    = 1                       ;
  setLanguage ( L('s','v') )                   ;
}

void N::SpeechTranslator::SelectSW(void)
{ // Swahili
  Setup ( stress_lengths_sw , stress_amps_sw )      ;
  options.lengthMods0          = options.lengthMods ;  // don't lengthen vowels in the last syllable
  options.vowelPause           = 1                  ;
  options.stressRule           = STRESSPOSN_2R      ;
  options.stressFlags          = S_FINAL_DIM_ONLY   |
                                 S_FINAL_NO_2       ;
  options.maxInitialConsonants = 4                  ; // for example: mwngi
  options.numbers              = NUM_AND_UNITS      |
                                 NUM_HUNDRED_AND    |
                                 NUM_SINGLE_AND     |
                                 NUM_OMIT_1_HUNDRED ;
  options.breakNumbers         = 0x49249268         ; // for languages which have numbers for 100,000 and 1,000,000
  setLanguage ( L('s','w') )                        ;
}

void N::SpeechTranslator::SelectTN(void)
{ // Setswana
  Setup ( stress_lengths_sw , stress_amps_sw )      ;
  options.lengthMods0          = options.lengthMods ;  // don't lengthen vowels in the last syllable
  options.vowelPause           = 1                  ;
  options.stressRule           = STRESSPOSN_2R      ;
  options.stressFlags          = S_FINAL_DIM_ONLY   |
                                 S_FINAL_NO_2       ;
  options.maxInitialConsonants = 4                  ; // for example: mwngi
  options.numbers              = NUM_AND_UNITS      |
                                 NUM_HUNDRED_AND    |
                                 NUM_SINGLE_AND     |
                                 NUM_OMIT_1_HUNDRED ;
  options.breakNumbers         = 0x49249268         ; // for languages which have numbers for 100,000 and 1,000,000
  setLanguage ( L('t','n') )                        ;
}

void N::SpeechTranslator::SelectTA(void)
{ // Tamil
  Setup ( stress_lengths_ta2 , stress_amps_ta )        ;
  options.lengthMods0            = options.lengthMods  ; // don't lengthen vowels in the last syllable
  options.stressRule             = STRESSPOSN_1L       ;
  options.stressFlags            =  S_FINAL_DIM_ONLY   ; // use 'diminished' for unstressed final syllable
  options.spellingStress         = 1                   ;
  options.breakNumbers           = 0x14a8              ; // 1000, 100,000  10,000,000
  Setup ( stress_lengths_ta  , NULL           )        ;
  letterBitsOffset               = OFFSET_TAMIL        ;
  options.numbers                = NUM_OMIT_1_THOUSAND ;
  options.param[LOPT_WORD_MERGE] = 1                   ; // don't break vowels betwen words
  setIndicLetters    (                          )      ; // call this after setting OFFSET_
  setLetterBitsRange ( LETTERGP_B , 0x4e , 0x4e )      ; // chillu-virama (unofficial)
  setLanguage        ( L('t','a')               )      ;
}

void N::SpeechTranslator::SelectML(void)
{ // Malayalam
  Setup ( stress_lengths_ta2 , stress_amps_ta )   ;
  options.lengthMods0    = options.lengthMods     ; // don't lengthen vowels in the last syllable
  options.stressRule     = STRESSPOSN_1L          ;
  options.stressFlags    = S_FINAL_DIM_ONLY       ; // use 'diminished' for unstressed final syllable
  options.spellingStress = 1                      ;
  options.breakNumbers   = 0x14a8                 ; // 1000, 100,000  10,000,000
  letterBitsOffset       = OFFSET_MALAYALAM       ;
  options.numbers        = NUM_OMIT_1_THOUSAND    ;
  setIndicLetters    (                          ) ; // call this after setting OFFSET_
  setLetterBitsRange ( LETTERGP_B , 0x4e , 0x4e ) ; // chillu-virama (unofficial)
  setLanguage        ( L('m','l')               ) ;
}

void N::SpeechTranslator::SelectKN(void)
{ // Kannada
  Setup ( stress_lengths_ta2 , stress_amps_ta )   ;
  options.lengthMods0    = options.lengthMods     ; // don't lengthen vowels in the last syllable
  options.stressRule     = STRESSPOSN_1L          ;
  options.stressFlags    = S_FINAL_DIM_ONLY       ; // use 'diminished' for unstressed final syllable
  options.spellingStress = 1                      ;
  options.breakNumbers   = 0x14a8                 ; // 1000, 100,000  10,000,000
  letterBitsOffset       = OFFSET_KANNADA         ;
  options.numbers        = 0x1                    ;
  setIndicLetters    (                          ) ; // call this after setting OFFSET_
  setLetterBitsRange ( LETTERGP_B , 0x4e , 0x4e ) ; // chillu-virama (unofficial)
  setLanguage        ( L('k','n')               ) ;
}

void N::SpeechTranslator::SelectMR(void)
{ // Marathi
  Setup ( stress_lengths_ta2 , stress_amps_ta )   ;
  options.lengthMods0    = options.lengthMods     ; // don't lengthen vowels in the last syllable
  options.stressRule     = STRESSPOSN_1L          ;
  options.stressFlags    = S_FINAL_DIM_ONLY       ; // use 'diminished' for unstressed final syllable
  options.spellingStress = 1                      ;
  options.breakNumbers   = 0x14a8                 ; // 1000, 100,000  10,000,000
  letterBitsOffset       = OFFSET_DEVANAGARI      ;
  setIndicLetters    (                          ) ; // call this after setting OFFSET_
  setLetterBitsRange ( LETTERGP_B , 0x4e , 0x4e ) ; // chillu-virama (unofficial)
  setLanguage        ( L('m','r')               ) ;
}

void N::SpeechTranslator::SelectTE(void)
{ // Telugu
  Setup ( stress_lengths_ta2 , stress_amps_ta )   ;
  options.lengthMods0    = options.lengthMods     ; // don't lengthen vowels in the last syllable
  options.stressRule     = STRESSPOSN_1L          ;
  options.stressFlags    = S_FINAL_DIM_ONLY       ; // use 'diminished' for unstressed final syllable
  options.spellingStress = 1                      ;
  options.breakNumbers   = 0x14a8                 ; // 1000, 100,000  10,000,000
  letterBitsOffset       = OFFSET_TELUGU          ;
  options.numbers        = 0x1                    ;
  setIndicLetters    (                          ) ; // call this after setting OFFSET_
  setLetterBitsRange ( LETTERGP_B , 0x4e , 0x4e ) ; // chillu-virama (unofficial)
  setLanguage        ( L('t','e')               ) ;
}

void N::SpeechTranslator::SelectTH(void)
{ // Thai
  Setup ( stress_lengths_th , stress_amps_th ) ;
  options.stressRule   = 0                     ; // stress on final syllable of a "word"
  options.stressFlags  = S_NO_DIM              ; // don't automatically set diminished stress (may be set in the intonation module)
  options.toneLanguage = 1                     ; // Tone language, use  CalcPitches_Tone() rather than CalcPitches()
  options.lengthMods0  = options.lengthMods    ; // don't lengthen vowels in the last syllable
//  options.toneNumbers  = 1                     ; // a number after letters indicates a tone number (eg. pinyin or jyutping)
  options.wordGap      = 0x21                  ; // length of a final vowel is less dependent on the next consonant, don't merge consonant with next word
  setLanguage ( L('t','h') )                   ;
}

void N::SpeechTranslator::SelectTR(void)
{ // Turkish
  Setup ( stress_lengths_tr , stress_amps_tr )       ;
  charsetA0                    = charsets[9]         ; // ISO-8859-9 - Latin5
  options.stressRule           = 7                   ; // stress on the last syllable, before any explicitly unstressed syllable
  options.stressFlags          = S_NO_AUTO_2         ; //no automatic secondary stress
  options.dotlessI             = 1                   ;
  options.param[LOPT_SUFFIX]   = 1                   ;
  options.numbers              = NUM_SINGLE_STRESS   |
                                 NUM_DECIMAL_COMMA   |
                                 NUM_OMIT_1_HUNDRED  |
                                 NUM_OMIT_1_THOUSAND |
                                 NUM_DFRACTION_2     ;
  options.maxInitialConsonants = 2                   ;
  setLanguage ( L('t','r') )                         ;
}

void N::SpeechTranslator::SelectAZ(void)
{ // Azerbaijan
  Setup ( stress_lengths_tr , stress_amps_tr )       ;
  charsetA0                    = charsets[9]         ; // ISO-8859-9 - Latin5
  options.stressRule           = 7                   ; // stress on the last syllable, before any explicitly unstressed syllable
  options.stressFlags          = S_NO_AUTO_2         ; //no automatic secondary stress
  options.dotlessI             = 1                   ;
  options.param[LOPT_SUFFIX]   = 1                   ;
  options.numbers              = NUM_SINGLE_STRESS   |
                                 NUM_DECIMAL_COMMA   |
                                 NUM_ALLOW_SPACE     |
                                 NUM_OMIT_1_HUNDRED  |
                                 NUM_OMIT_1_THOUSAND |
                                 NUM_DFRACTION_2     ;
  options.maxInitialConsonants = 2                   ;
  setLanguage ( L('a','z') )                         ;
}

void N::SpeechTranslator::SelectTT(void)
{ // Tatar
  setCyrillicLetters (                                    ) ;
  Setup              ( stress_lengths_fr , stress_amps_fr ) ;
  options.stressRule  = STRESSPOSN_1R                       ; // stress on final syllable
  options.stressFlags = S_NO_AUTO_2                         ; //no automatic secondary stress
  options.numbers     = NUM_SINGLE_STRESS                   |
                        NUM_DECIMAL_COMMA                   |
                        NUM_OMIT_1_HUNDRED                  |
                        NUM_OMIT_1_THOUSAND                 |
                        NUM_DFRACTION_4                     ;
  setLanguage ( L('t','t') )                                ;
}

void N::SpeechTranslator::SelectUK(void)
{ // Ukrainian
  setCyrillicLetters ( )                     ;
  options.param[LOPT_UNPRONOUNCABLE] = 0x432 ; // [v]  don't count this character at start of word
  setLanguage ( L('u','k') )                 ;
}

void N::SpeechTranslator::SelectUR(void)
{ // Urdu
  letterBitsOffset                   = OFFSET_ARABIC ;
  options.param[LOPT_UNPRONOUNCABLE] = 1             ; // disable check for unpronouncable words
  options.numbers                    = NUM_SWAP_TENS ;
  options.breakNumbers               = 0x52a8        ; // for languages which have numbers for 100,000 and 100,00,000, eg Hindi
  setLanguage ( L('u','r') )                         ;
}

void N::SpeechTranslator::SelectVI(void)
{
  Setup ( stress_lengths_vi , stress_amps_vi )  ;
  options.lengthMods0   = options.lengthMods    ; // don't lengthen vowels in the last syllable
  options.stressRule    = STRESSPOSN_1L         ;
  options.wordGap       = 0x21                  ; // length of a final vowel is less dependent on the next consonant, don't merge consonant with next word
//  options.vowelPause    = 4                     ;
  wLetterGroups[0]      = vowels_vi             ;
  wLetterGroups[7]      = vowels_vi             ;
  options.toneLanguage  = 1                     ; // Tone language, use  CalcPitches_Tone() rather than CalcPitches()
  options.unstressedWD1 = 2                     ;
  options.numbers       = NUM_DECIMAL_COMMA     |
                          NUM_HUNDRED_AND_DIGIT |
                          NUM_DFRACTION_4       |
                          NUM_ZERO_HUNDRED      ;
  setLanguage ( L('v','i') )                    ;
}

void N::SpeechTranslator::SelectWO(void)
{
  options.stressRule = STRESSPOSN_1L       ;
  options.numbers    = NUM_AND_UNITS       |
                       NUM_HUNDRED_AND     |
                       NUM_OMIT_1_HUNDRED  |
                       NUM_OMIT_1_THOUSAND |
                       NUM_SINGLE_STRESS   ;
  setLanguage ( L('w','o') )               ;
}

void N::SpeechTranslator::SelectZH(void)
{
  Setup ( stress_lengths_zh , stress_amps_zh ) ;
  options.stressRule   = STRESSPOSN_1R         ; // stress on final syllable of a "word"
  options.stressFlags  = S_NO_DIM              ; // don't automatically set diminished stress (may be set in the intonation module)
  options.vowelPause   = 0                     ;
  options.toneLanguage = 1                     ; // Tone language, use  CalcPitches_Tone() rather than CalcPitches()
  options.lengthMods0  = options.lengthMods    ; // don't lengthen vowels in the last syllable
  options.toneNumbers  = 1                     ; // a number after letters indicates a tone number (eg. pinyin or jyutping)
  options.ideographs   = 1                     ;
  options.ourAlphabet  = 0x3100                ;
  options.wordGap      = 0x21                  ; // length of a final vowel is less dependent on the next consonant, don't merge consonant with next word
  options.textmode     = 1                     ;
  options.listx        = 1                     ; // compile zh_listx after zh_list
  setLanguage ( L('z','h') )                   ;
}

void N::SpeechTranslator::SelectZHY(void)
{
  Setup ( stress_lengths_zh , stress_amps_zh ) ;
  options.stressRule   = STRESSPOSN_1R         ; // stress on final syllable of a "word"
  options.stressFlags  = S_NO_DIM              ; // don't automatically set diminished stress (may be set in the intonation module)
  options.vowelPause   = 0                     ;
  options.toneLanguage = 1                     ; // Tone language, use  CalcPitches_Tone() rather than CalcPitches()
  options.lengthMods0  = options.lengthMods    ; // don't lengthen vowels in the last syllable
  options.toneNumbers  = 1                     ; // a number after letters indicates a tone number (eg. pinyin or jyutping)
  options.ideographs   = 1                     ;
  options.ourAlphabet  = 0x3100                ;
  options.wordGap      = 0x21                  ; // length of a final vowel is less dependent on the next consonant, don't merge consonant with next word
  setLanguage ( L_zhy )                        ;
}

void N::SpeechTranslator::Reset(void)
{
  int ix                                                          ;
  /////////////////////////////////////////////////////////////////
  charsetA0         = charsets[1] ; // ISO-8859-1, this is for when the input is not utf8
  dictionaryName[0] = 0                                           ;
  dictCondition     = 0                                           ;
  dictMinSize       = 0                                           ;
  dictSkipWords     = 0                                           ;
  dataDictRules     = NULL        ; // language_1   translation rules file
  dataDictList      = NULL        ; // language_2   dictionary lookup file
  transposeMin      = 0x60                                        ;
  transposeMax      = 0x17f                                       ;
  transposeMap      = transpose_map_latin                         ;
  frequentPairs     = NULL                                        ;
  letterBitsOffset  = 0                                           ;
  ::memset ( letterBits    , 0 , sizeof(char   ) * 256 )          ;
  ::memset ( wLetterGroups , 0 , sizeof(wchar_t) *   8 )          ;
  // 0-6 sets of characters matched by A B C H F G Y  in pronunciation rules
  // these may be set differently for different languages
  setLetterBits ( 0 , "aeiou"                ) ; // A  vowels, except y
  setLetterBits ( 1 , "bcdfgjklmnpqstvxz"    ) ; // B  hard consonants, excluding h,r,w
  setLetterBits ( 2 , "bcdfghjklmnpqrstvwxz" ) ; // C  all consonants
  setLetterBits ( 3 , "hlmnr"                ) ; // H  'soft' consonants
  setLetterBits ( 4 , "cfhkpqstx"            ) ; // F  voiceless consonants
  setLetterBits ( 5 , "bdgjlmnrvwyz"         ) ; // G voiced
  setLetterBits ( 6 , "eiy"                  ) ; // Letter group Y, front vowels
  setLetterBits ( 7 , "aeiouy"               ) ; // vowels, including y
  charPlusApostrophe = empty_wstring                              ;
  punctWithinWord    = punct_in_word                              ;
  charsIgnore        = chars_ignore_default                       ;
  /////////////////////////////////////////////////////////////////
  for ( ix = 0 ; ix < 8 ; ix++ )                                  {
    stressAmps    [ix] = stress_amps2    [ix]                     ;
    stressAmpsR   [ix] = stress_amps2    [ix] - 1                 ;
    stressLengths [ix] = stress_lengths2 [ix]                     ;
  }                                                               ;
  /////////////////////////////////////////////////////////////////
  options.maxLengthMod                         = 500              ;
  options.lengthenTonic                        = 20               ;
  options.stressRule                           = STRESSPOSN_2R    ;
  options.unstressedWD1                        = 1                ;
  options.unstressedWD2                        = 3                ;
  options.param  [ LOPT_SONORANT_MIN         ] = 95               ;
  options.param  [ LOPT_LONG_VOWEL_THRESHOLD ] = 190/2            ;
  options.param  [ LOPT_MAXAMP_EOC           ] = 19               ;
  options.param  [ LOPT_UNPRONOUNCABLE       ] = 's'              ; // don't count this character at start of word
  options.param  [ LOPT_BRACKET_PAUSE        ] = 4                ; // pause at bracket
  options.param2 [ LOPT_BRACKET_PAUSE        ] = 2                ; // pauses when announcing bracket names
  options.maxInitialConsonants                 = 3                ;
  options.replaceChars                         = NULL             ;
  options.asciiLanguage[0]                     = 0                ; // Non-Latin alphabet languages, use this language to speak Latin words, default is English
  options.altAlphabetLanguage                  = L('e','n')       ;
  /////////////////////////////////////////////////////////////////
  setLengthMods ( 201 )                                           ;
  options.longStop                             = 100              ;
  options.maxRoman                             =  49              ;
  options.minRoman                             =   2              ;
  options.thousandsSep                         = ','              ;
  options.decimalSep                           = '.'              ;
  options.breakNumbers                         = BREAK_THOUSANDS  ; // 1000, 1000,000  1,000,000 etc
  options.maxDigits                            = 14               ;
  ::memcpy ( punctToTone  ,punctuation_to_tone,sizeof(char)*8*6 ) ;
  ::memcpy ( options.tunes,default_tunes      ,sizeof(char)*6   ) ;
}

void N::SpeechTranslator::setLetterVowel(int c)
{ // keep value for group 6 (front vowels e,i,y)
  letterBits[c] = (letterBits[c] & 0x40) | 0x81 ;
}

void N::SpeechTranslator::resetLetterBits(int groups)
{
  unsigned int ix                  ;
  unsigned int mask = ~groups      ;
  for ( ix = 0 ; ix < 256 ; ix++ ) {
    letterBits[ix] &= mask         ;
  }                                ;
}

void N::SpeechTranslator::setLetterBits(int group,const char * string)
{
  int           bits = (1L << group) ;
  unsigned char c                    ;
  while ( ( c = *string++ ) != 0)    {
    letterBits [ c ] |= bits         ;
  }                                  ;
}

void N::SpeechTranslator::setLetterBitsRange(int group,int first,int last)
{
  int bits = (1L << group)               ;
  int ix                                 ;
  for ( ix = first ; ix <= last ; ix++ ) {
    letterBits[ix] |= bits               ;
  }                                      ;
}

void N::SpeechTranslator::setLengthMods(int value)
{
  int value2 = ( value % 100 )                    ;
  options.lengthMods  = length_mod_tabs[value2]   ;
  options.lengthMods0 = length_mod_tabs[value2]   ;
  if ( ( value2 = ( value / 100 ) ) != 0)         {
    options.lengthMods0 = length_mod_tabs[value2] ;
  }                                               ;
}

void N::SpeechTranslator::setCyrillicLetters(void)
{ // character codes offset by 0x420
  charsetA0        = charsets[18]                           ; // KOI8-R
  transposeMin     = 0x430                                  ; // convert cyrillic from unicode into range 0x01 to 0x22
  transposeMax     = 0x451                                  ;
  transposeMap     = NULL                                   ;
  frequentPairs    = pairs_ru                               ;
  letterBitsOffset = OFFSET_CYRILLIC                        ;
  memset        ( letterBits , 0  , sizeof(char) * 256    ) ;
  setLetterBits ( LETTERGP_A      , (char *)ru_vowels     ) ;
  setLetterBits ( LETTERGP_B      , ru_soft               ) ;
  setLetterBits ( LETTERGP_C      , (char *)ru_consonants ) ;
  setLetterBits ( LETTERGP_H      , ru_hard               ) ;
  setLetterBits ( LETTERGP_F      , ru_nothard            ) ;
  setLetterBits ( LETTERGP_G      , ru_voiced             ) ;
  setLetterBits ( LETTERGP_Y      , ru_ivowels            ) ;
  setLetterBits ( LETTERGP_VOWEL2 ,(char *)ru_vowels      ) ;
}

void N::SpeechTranslator::setIndicLetters(void)
{
  memset ( letterBits , 0 , sizeof(char) * 256      )          ;
  setLetterBitsRange ( LETTERGP_A , 0x04 , 0x14     )          ; // vowel letters
  setLetterBitsRange ( LETTERGP_A , 0x3e , 0x4d     )          ; // + vowel signs, and virama
  setLetterBits      ( LETTERGP_A , dev_vowels2     )          ; // + extra vowels and vowel signs
  setLetterBitsRange ( LETTERGP_B , 0x3e , 0x4d     )          ; // vowel signs, and virama
  setLetterBits      ( LETTERGP_B , dev_vowels2     )          ; // + extra vowels and vowel signs
  setLetterBitsRange ( LETTERGP_C , 0x15 , 0x39     )          ; // the main consonant range
  setLetterBits      ( LETTERGP_C , dev_consonants2 )          ; // + additional consonants
  setLetterBitsRange ( LETTERGP_Y , 0x04 , 0x14     )          ; // vowel letters
  setLetterBitsRange ( LETTERGP_Y , 0x3e , 0x4c     )          ; // + vowel signs
  setLetterBits      ( LETTERGP_Y , dev_vowels2     )          ; // + extra vowels and vowel signs
  options.param[LOPT_UNPRONOUNCABLE] = 1                       ; // disable check for unpronouncable words
  options.suffixAddE                 = letterBitsOffset + 0x4d ; //virama
}

void N::SpeechTranslator::Setup(const short * lengths,const unsigned char * amps)
{
  if ( NotNull ( lengths ) )                                 {
    ::memcpy ( stressLengths , lengths , sizeof(short) * 8 ) ;
  }                                                          ;
  if ( NotNull ( amps    ) )                                 {
    ::memcpy ( stressAmps    , amps    , sizeof(char ) * 8 ) ;
  }                                                          ;
}

int N::SpeechTranslator::isLetterGroup(char * word,int group,int pre)
{ // match the word against a list of utf-8 strings
  char * p                                     ;
  char * w                                     ;
  int    len = 0                               ;
  //////////////////////////////////////////////
  p = letterGroups[group]                      ;
  if ( p == NULL ) return 0                    ;
  while ( (*p) != RULE_GROUP_END)              {
    if ( pre )                                 {
      len = strlen(p)                          ;
      w   = word - len + 1                     ;
    } else w = word                            ;
    while ( ( (*p) == (*w) ) && ( (*w) != 0) ) {
      w++                                      ;
      p++                                      ;
    }                                          ;
    if ( (*p) == 0)                            {
      if ( pre ) return len                    ;
      return ( w - word )                      ; // matched a complete string
    }                                          ;
    while ( *p++ != 0)                         ; // skip to end of string
  }                                            ;
  return 0                                     ;
}

bool N::SpeechTranslator::isLetter(int letter,int group)
{
  int letter2;
  if ( NotNull(wLetterGroups[group]))                           {
    if ( wcschr ( wLetterGroups[group] , letter ) ) return true ;
    return false                                                ;
  }                                                             ;
  if ( group > 7 ) return false                                 ;
  if ( letterBitsOffset > 0)                                    {
    letter2 = letter - letterBitsOffset                         ;
    if ( ( letter2 > 0) && (letter2 < 0x100) ) letter = letter2 ;
                                          else return false     ;
  } else                                                        {
    if ( ( letter >= 0xc0 ) && ( letter < N_REMOVE_ACCENT ) )   {
      unsigned char lb                                          ;
      lb  = letterBits [ remove_accent [ ( letter - 0xc0 ) ] ]  ;
      lb &= ( 1L << group )                                     ;
      return ( lb != 0 )                                        ;
    }                                                           ;
  }                                                             ;
  if ( ( letter >= 0 ) && ( letter < 0x100 ) )                  {
    unsigned char lb = letterBits[letter]                       ;
    lb &= ( 1L << group )                                       ;
    return ( lb != 0 )                                          ;
  }                                                             ;
  return false                                                  ;
}

bool N::SpeechTranslator::isVowel(int letter)
{
  return isLetter ( letter , LETTERGP_VOWEL2 ) ;
}

int N::SpeechTranslator::toUtf8(unsigned int c,char * buf)
{
  static char unsigned code[4] = {0,0xc0,0xe0,0xf0} ;
  int n_bytes                                       ;
  int shift                                         ;
  ///////////////////////////////////////////////////
  if ( c < 0x80 )                                   {
    buf[0] = c                                      ;
    return 1                                        ;
  }                                                 ;
  ///////////////////////////////////////////////////
  if ( c >= 0x110000 )                              {
    buf[0] = ' '                                    ;
    return 1                                        ;
  }                                                 ;
  ///////////////////////////////////////////////////
  if ( c < 0x00800) n_bytes = 1 ; else
  if ( c < 0x10000) n_bytes = 2 ; else
                    n_bytes = 3 ;
  ///////////////////////////////////////////////////
  shift  = 6 * n_bytes                              ;
  buf[0] = code [ n_bytes ] | ( c >> shift )        ;
  for (int j = 0 ; j < n_bytes ; j++ )              {
    shift    -= 6                                   ;
    buf[j+1]  = 0x80 + ( ( c >> shift ) & 0x3f )    ;
  }                                                 ;
  ///////////////////////////////////////////////////
  return ( n_bytes + 1 )                            ;
}

int N::SpeechTranslator::toGrapheme(int & grapheme,const char * buf,bool backward)
{
  int c1                                                     ;
  int bytes = 0                                              ;
  static const unsigned char mask[4] = {0xff,0x1f,0x0f,0x07} ;
  // find the start of the next/previous character
  while ( ( (*buf) & 0xc0) == 0x80)                          {
    // skip over non-initial bytes of a multi-byte utf8 character
    if (backward) buf--                                      ;
             else buf++                                      ;
  }                                                          ;
  if ( (c1 = *buf++) & 0x80 )                                {
    if ( ( c1 & 0xe0 ) == 0xc0 ) bytes = 1              ; else
    if ( ( c1 & 0xf0 ) == 0xe0 ) bytes = 2              ; else
    if ( ( c1 & 0xf8 ) == 0xf0 ) bytes = 3                   ;
    c1 &= mask[bytes]                                        ;
    for (int i = 0 ; i < bytes ; i++ )                       {
      c1 = (c1 << 6) + ( (*buf++) & 0x3f)                    ;
    }                                                        ;
  }                                                          ;
  grapheme = c1                                              ;
  return ( bytes + 1 )                                       ;
}

int N::SpeechTranslator::nBytes(unsigned char c)
{
  if ( c < 0x80 ) return 1 ;
  if ( c < 0xe0 ) return 2 ;
  if ( c < 0xf0 ) return 3 ;
  return                 4 ;
}

void N::SpeechTranslator::setSpellingStress(char * phonemes,int control,int chars)
{ // Individual letter names, reduce the stress of some.
  int           ix                                                  ;
  unsigned int  c                                                   ;
  int           n_stress = 0                                        ;
  int           prev     = 0                                        ;
  int           count                                               ;
  unsigned char buf [ 200 ]                                         ;
  ///////////////////////////////////////////////////////////////////
  for ( ix = 0 ; ( c = phonemes[ix] ) != 0 ; ix++ )                 {
    if ( ( c    == Speak::PcStressP )                    &&
         ( prev != Speak::PcSwitch  ) ) n_stress++        ;
    buf [ ix ] = prev = c                                           ;
  }                                                                 ;
  buf [ ix ] = 0                                                    ;
  count      = 0                                                    ;
  prev       = 0                                                    ;
  for ( ix = 0 ; ( c = buf[ix] ) != 0 ; ix++ )                      {
    if ( ( c    == Speak::PcStressP )                    &&
         ( chars > 1                          )                    &&
         ( prev != Speak::PcSwitch  )                   ) {
      count++                                                       ;
      if ( options.spellingStress == 1 )                            {
        // stress on initial letter when spelling
        if ( count > 1 ) c = Speak::PcStress3             ;
      } else                                                        {
        if ( count != n_stress )                                    {
          if ( ( ( count % 3 ) != 0) || ( count == n_stress-1 ) )   {
            c = Speak::PcStress3                          ; // reduce to secondary stress
          }                                                         ;
        }                                                           ;
      }                                                             ;
    } else
    if ( c == 0xff )                                                {
      if ( ( control <  2 ) || ( ix == 0 ) ) continue               ; // don't insert pauses
      if (   control == 4) c = Speak::PcPause             ; // pause after each character
      if ( ( ( count % 3 ) == 0 ) || ( control > 2 ) )              {
        c = Speak::PcPauseNoLink                          ; // pause following a primary stress
      } else c = Speak::PcPauseVShort                     ;
    }                                                               ;
    *phonemes++ = prev = c                                          ;
  }                                                                 ;
  if ( control >= 2 ) *phonemes++ = Speak::PcPauseNoLink  ;
  *phonemes = 0                                                     ;
}

int N::SpeechTranslator::RemoveEnding(char * word,int endType,char * wordCopy)
{
  int          i                                                              ;
  int          len                                                            ;
  int          len_ending                                                     ;
  int          end_flags                                                      ;
  char       * word_end                                                       ;
  const char * p                                                              ;
  char         ending[12]                                                     ;
  /////////////////////////////////////////////////////////////////////////////
//  static const char * add_e_additions  [] = {"c", "rs", "ir", "ur", "ath", "ns", "lu", NULL } ;
  static const char * add_e_additions  [] = {"c", "rs", "ir", "ur", "ath", "ns",  "u", NULL } ;
  static const char * add_e_exceptions [] = { "ion", NULL }                   ;
  /////////////////////////////////////////////////////////////////////////////
  for ( word_end = word ; *word_end != ' ' ; word_end++ )                     {
    if(*word_end == REPLACED_E) *word_end = 'e'                               ;
  }                                                                           ;
  i = word_end - word                                                         ;
  if ( NotNull(wordCopy) )                                                    {
    memcpy(wordCopy,word,i)                                                   ;
    wordCopy[i] = 0                                                           ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  for ( len_ending = i = (endType & 0x3f) ; i>0 ; i-- )                       {
    word_end--                                                                ;
    while ( ( (*word_end) & 0xc0 ) == 0x80 )                                  {
      word_end--                                                              ;
      len_ending++                                                            ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  for ( i=0 ; i < len_ending ; i++ )                                          {
    ending  [i] = word_end[i]                                                 ;
    word_end[i] = ' '                                                         ;
  }                                                                           ;
  ending[i] = 0                                                               ;
  word_end --                                                                 ;
  end_flags = (endType & 0xfff0) | FLAG_SUFX                                  ;
  if ( endType & SUFX_I )                                                     {
    if ( word_end[0] == 'i' ) word_end[0] = 'y'                               ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ( endType & SUFX_E )                                                     {
    if (translatorName == L('n','l'))                                         {
      if ( ( ( word_end[ 0] & 0x80 ) == 0         )                          &&
           ( ( word_end[-1] & 0x80 ) == 0         )                          &&
            isVowel  ( word_end [-1]              )                          &&
            isLetter ( word_end [ 0] , LETTERGP_C )                          &&
           !isVowel  ( word_end [-2]              )                         ) {
        word_end [ 1 ] = word_end [  0 ]                                      ;
        word_end [ 0 ] = word_end [ -1 ]                                      ;
        word_end [ 2 ] = ' '                                                  ;
      }                                                                       ;
    } else
    if ( translatorName == L('e','n'))                                        {
      if ( isLetter ( word_end [ -1 ] , LETTERGP_VOWEL2 )                    &&
           isLetter ( word_end [  0 ] ,               1 )                   ) {
        for ( i = 0 ; ( p = add_e_exceptions[i] ) != NULL ; i++ )             {
          len = strlen(p)                                                     ;
          if ( memcmp ( p , &word_end[1-len] , len ) == 0 ) break             ;
        }                                                                     ;
        if ( IsNull(p) ) end_flags |= FLAG_SUFX_E_ADDED                       ;
      } else                                                                  {
        for ( i = 0 ; ( p = add_e_additions [i] ) != NULL ; i++ )             {
          len = strlen(p)                                                     ;
          if ( memcmp ( p , &word_end[1-len] , len ) == 0 )                   {
            end_flags |= FLAG_SUFX_E_ADDED                                    ;
            break                                                             ;
          }                                                                   ;
        }                                                                     ;
      }                                                                       ;
    } else
    if ( options.suffixAddE != 0) end_flags |= FLAG_SUFX_E_ADDED              ;
    ///////////////////////////////////////////////////////////////////////////
    if ( end_flags & FLAG_SUFX_E_ADDED )                                      {
      toUtf8 ( options.suffixAddE , &word_end[1] )                            ;
#ifdef ___NOT_YES_READY_PLEASE_REMOVE_THIS_AFTER_FINISHED__
      if ( optionPhonemes == 2)                                               {
        fprintf(f_trans,"add e\n")                                            ;
      }                                                                       ;
#endif
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ( ( endType & SUFX_V) && ( expectVerb == 0 ) ) expectVerb = 1            ;
  if ( ( strcmp(ending,"s")==0) || (strcmp(ending,"es")==0))                  {
    end_flags |= FLAG_SUFX_S                                                  ;
  }                                                                           ;
  if ( ending[0] == '\'' ) end_flags &= ~FLAG_SUFX                            ;
  return end_flags                                                            ;
}

int N::SpeechTranslator::Hash(const char * string)
{
  int c                                    ;
  int chars = 0                            ;
  int hash  = 0                            ;
  //////////////////////////////////////////
  while ( ( c = (*string++ & 0xff)) != 0 ) {
    hash = (hash * 8    ) + c              ;
    hash = (hash & 0x3ff) ^ (hash >> 8)    ; /* exclusive or */
    chars++                                ;
  }                                        ;
  return ( ( hash + chars ) & 0x3ff )      ;  // a 10 bit hash code
}

bool N::SpeechTranslator::is(QString name)
{
  QString D = QString(dictionaryName)      ;
  return ( D.toLower() == name.toLower() ) ;
}

void N::SpeechTranslator::Install(QString name)
{
  ::memset(dictionaryName,0,40                )     ;
  ::strcpy(dictionaryName,name.toUtf8().data())     ;
}

bool N::SpeechTranslator::Load(void)
{
  if (dataDictList.size()<=0) return false                  ;
  ///////////////////////////////////////////////////////////
  int * pw     = (int *)dataDictList.data()                 ;
  int   length = pw[1]                                      ;
  if ( dataDictList.size()<=(N_HASH_DICT + sizeof(int)*2) ) {
    return false                                            ;
  }                                                         ;
  if ( pw[0]  != N_HASH_DICT ) return false                 ;
  if ( length <= 0           ) return false                 ;
  if ( length >  0x8000000   ) return false                 ;
  dataDictRules = & dataDictList.data() [ length ]          ;
  ///////////////////////////////////////////////////////////
  SetupRules ( )                                            ;
  int    hash                                               ;
  char * p = &  dataDictList .data() [ 8 ]                  ;
  for ( hash = 0 ; hash < N_HASH_DICT ; hash++ )            {
    dictHashTab[hash] = p                                   ;
    while ( ( length = *p ) != 0 ) p += length              ;
    p++                                                     ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  return true                                               ;
}

// Called after dictionary 1 is loaded, to set up table of entry points
// for translation rule chains for single-letters and two-letter combinations
void N::SpeechTranslator::SetupRules(void)
{
  int             ix                                   ;
  char          * p                                    ;
  char          * p_name                               ;
  unsigned int  * pw                                   ;
  unsigned char   c                                    ;
  unsigned char   c2                                   ;
  int             len                                  ;
  //////////////////////////////////////////////////////
  nGroups2 = 0                                         ;
  for ( ix = 0 ; ix < 256 ; ix++ )                     {
    groups1      [ ix ] = NULL                         ;
    groups2Count [ ix ] = 0                            ;
    groups2Start [ ix ] = 255                          ; // indicates "not set"
  }                                                    ;
  //////////////////////////////////////////////////////
  ::memset ( letterGroups , 0 , sizeof(char) *  95 )   ;
  ::memset ( groups3      , 0 , sizeof(char) * 128 )   ;
  //////////////////////////////////////////////////////
  p = dataDictRules                                    ;
  while ( (*p) != 0)                                   {
    if ( (*p) != RULE_GROUP_START) break               ;
    ////////////////////////////////////////////////////
    p++                                                ;
    if ( p[0] == RULE_REPLACEMENTS)                    {
      pw = (unsigned int *)(((unsigned long)p+4) & ~3) ; // advance to next word boundary
      options.replaceChars = pw                        ;
      while ( pw[0] != 0 ) pw += 2                     ; // find the end of the replacement list, each entry is 2 words.
      p = (char *)(pw+1)                               ;
      continue                                         ;
    }                                                  ;
    ////////////////////////////////////////////////////
    if ( p[0] == RULE_LETTERGP2 )                      {
      ix = p[1] - 'A'                                  ;
      p += 2                                           ;
      if ( ( ix >= 0 ) && ( ix < N_LETTER_GROUPS ) )   {
        letterGroups[ix] = p                           ;
      }                                                ;
    } else                                             {
      len    = strlen(p)                               ;
      p_name = p                                       ;
      c      = p_name[0]                               ;
      c2     = p_name[1]                               ;
      p     += (len+1)                                 ;
      if ( len == 1 ) groups1 [ c      ] = p ; else
      if ( len == 0 ) groups1 [ 0      ] = p ; else
      // index by offset from letter base
      if ( c   == 1 ) groups3 [ c2 - 1 ] = p ; else    {
        if ( groups2Start[c] == 255 )                  {
          groups2Start[c] = nGroups2                   ;
        }                                              ;
        groups2Count[ c        ]++                     ;
        groups2     [ nGroups2 ] = p                   ;
        groups2Name [ nGroups2 ] = ( c + ( c2 << 8 ) ) ;
        nGroups2++                                     ;
      }                                                ;
    }                                                  ;
    // skip over all the rules in this group
    while ( (*p) != RULE_GROUP_END)                    {
      p += ( strlen(p) + 1 )                           ;
    }                                                  ;
    p++                                                ;
  }                                                    ;
}

bool N::SpeechTranslator::StressCondition (
       PhonemeLists & list                ,
       int            condition           ,
       int            control             ,
       bool           translator          )
// condition:
// 0 if diminished
// 1 if unstressed
// 2 if not stressed
// 3 if stressed
// 4 if max stress
{
  static int condition_level [ 4 ] = {1,2,4,15}            ;
  int        stress_level                                  ;
  int        phcode                                        ;
  int        index = 0                                     ;
  //////////////////////////////////////////////////////////
  phcode = list[0]->phcode                                 ;
  if ( phonemes[phcode]->Type == Speak::VOWEL )            {
    index = 0                                              ;
  } else                                                   { // consonant, get stress from the following vowel
    phcode = list[1]->phcode                               ;
    if ( phonemes[phcode]->Type == Speak::VOWEL )          {
      index = 1                                            ;
    } else return false                                    ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  stress_level = list[index]->stresslevel & 0x0f           ;
  if ( translator )                                        {
    if ( ( control & 1 )                                  &&
       ( list[0]->synthflags & SFLAG_DICTIONARY)          && // ??? questionable
       ( ( options.param[LOPT_REDUCE] & 1 ) == 0 ) )       {
      return false                                         ;
    }                                                      ;
    if ( ( options.param[LOPT_REDUCE] & 0x2)              &&
          ( stress_level >= list[index]->wordstress ) )    {
       // treat the most stressed syllable in an unstressed word as stressed
       stress_level = 4                                    ;
    }                                                      ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  if ( condition == 4 )                                    {
    return ( stress_level >= list[0]->wordstress )         ;
  }                                                        ;
  if ( condition == 3 )                                    { // if stressed
    if ( stress_level > 3 ) return true                    ;
  } else                                                   {
    if ( stress_level < condition_level[condition])        {
      return true                                          ;
    }                                                      ;
  }                                                        ;
  return false                                             ;
}

void N::SpeechTranslator::PitchesTone(PhonemeLists & list)
{ //  clause_tone: 0=. 1=, 2=?, 3=! 4=none
  int ix                                                                      ;
  int count_stressed  = 0                                                     ;
  int final_stressed  = 0                                                     ;
  int tone_ph                                                                 ;
  int pause                                                                   ;
  int tone_promoted                                                           ;
  int tph                                                                     ;
  int prev_tph                                                                ;
  int prevw_tph                                                               ;
  int prev_p                                                                  ;
  int pitch_adjust    = 0 ; // pitch gradient through the clause - inital value
  int pitch_decrement = 0 ; //   decrease by this for each stressed syllable
  int pitch_low       = 0 ; //   until it drops to this
  int pitch_high      = 0 ; //   then reset to this
  // count number of stressed syllables
  for (int i=0;i<list.count();i++)                                            {
    if ((list[i]->type==Speak::VOWEL) && (list[i]->stresslevel >= 4))         {
      if ( count_stressed == 0 ) final_stressed = i                           ;
      if ( list[i]->stresslevel >= 4)                                         {
        final_stressed = i                                                    ;
        count_stressed++                                                      ;
      }                                                                       ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  list[final_stressed]->stresslevel = 7                                       ;
  // language specific, changes to tones
  if ( translatorName == L('v','i'))                                          { // LANG=vi
    if ( list[final_stressed]->tone_ph == 0 )                                 {
      list[final_stressed]->tone_ph = phonemes.Code('7')                      ;
      // change default tone (tone 1) to falling tone at end of clause
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  pause         = 1                                                           ;
  tone_promoted = 0                                                           ;
  ix            = 0                                                           ;
  prev_p        = 0                                                           ;
  prev_tph      = Speak::PcPause                                              ;
  prevw_tph     = Speak::PcPause                                              ;
  /////////////////////////////////////////////////////////////////////////////
  // perform tone sandhi
  for (int i = 0 ; i < list.count() ; i++, ix++)                              {
    if ( (list[ix]->type == Speak::PAUSE)                                    &&
         (list[ix]->ph->Length > 50)                                        ) {
      // there is a pause since the previous vowel
      pause = 1                                                               ;
      prevw_tph = Speak::PcPause                                              ;  // forget previous tone
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    // forget across word boundaries
    if ( list[ix]->newword ) prev_tph = Speak::PcPause                        ;
    if ( list[ix]->synthflags & SFLAG_SYLLABLE)                               {
      tone_ph = list[ix]->tone_ph                                             ;
      tph     = tone_ph                                                       ; // Mandarin
      if ( translatorName == L('z','h') )                                     {
        if ( tone_ph == 0 )                                                   {
          if ( pause || tone_promoted )                                       {
            tone_ph       = phonemes.Code('5','5')                            ; // no previous vowel, use tone 1
            tone_promoted = 1                                                 ;
          } else                                                              {
            tone_ph = phonemes.Code('1','1')                                  ; // default tone 5
          }                                                                   ;
          list[ix]->tone_ph = tone_ph                                         ;
          tph               = tone_ph                                         ;
        } else tone_promoted = 0                                              ;
        ///////////////////////////////////////////////////////////////////////
        if ( i == final_stressed )                                            {
          if ( ( phonemes[tph]->Mnemonic == 0x3535 )                         ||
               ( phonemes[tph]->Mnemonic == 0x3135 )                        ) {
            // change sentence final tone 1 or 4 to stress 6, not 7
            list[final_stressed]->stresslevel = 6                              ;
          }                                                                   ;
        }                                                                     ;
        ///////////////////////////////////////////////////////////////////////
        if ( phonemes[prevw_tph]->Mnemonic == 0x343132 )                      { // [214]
          if ( phonemes[tph]->Mnemonic == 0x343132)                           { // [214]
            list[prev_p]->tone_ph = phonemes.Code('3','5')                     ;
          } else                                                              {
            list[prev_p]->tone_ph = phonemes.Code('2','1')                     ;
          }                                                                   ;
        }                                                                     ;
        if ( ( phonemes[prev_tph]->Mnemonic == 0x3135)                        &&
             ( phonemes[tph     ]->Mnemonic == 0x3135) )                       { //  [51] + [51]
          list[prev_p]->tone_ph = phonemes.Code('5','3')                       ;
        }                                                                     ;
        ///////////////////////////////////////////////////////////////////////
        if ( phonemes[tph]->Mnemonic == 0x3131)                                { // [11] Tone 5
          // tone 5, change its level depending on the previous tone (across word boundaries)
          if ( phonemes[prevw_tph]->Mnemonic == 0x3535)                        {
            list[ix]->tone_ph = phonemes.Code('2','2')                         ;
          }                                                                   ;
          if ( phonemes[prevw_tph]->Mnemonic == 0x3533)                        {
            list[ix]->tone_ph = phonemes.Code('3','3')                         ;
          }                                                                   ;
          if ( phonemes[prevw_tph]->Mnemonic == 0x343132)                      {
            list[ix]->tone_ph = phonemes.Code('4','4')                         ;
          }                                                                   ;
          // tone 5 is unstressed (shorter)
          list[ix]->stresslevel = 0                                            ; // diminished stress
        }                                                                     ;
      }                                                                       ;
      /////////////////////////////////////////////////////////////////////////
      prev_p    = ix                                                          ;
      prevw_tph = prev_tph = tph                                              ;
      pause     = 0                                                           ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  // convert tone numbers to pitch
  for (int i = 0 ; i < list.count() ; i++ )                                   {
    if (list[i]->synthflags & SFLAG_SYLLABLE)                                  {
      tone_ph = list[i]->tone_ph                                               ;
      if ( list[i]->stresslevel != 0 )                                         {
        // TEST, consider all syllables as stressed
        if ( i == final_stressed )                                            {
          // the last stressed syllable
          pitch_adjust  = pitch_low                                           ;
        } else                                                                {
          pitch_adjust -= pitch_decrement                                     ;
          if ( pitch_adjust <= pitch_low ) pitch_adjust = pitch_high          ;
        }                                                                     ;
      }                                                                       ;
      if ( tone_ph ==0 )                                                      {
        tone_ph = Speak::PcDefaultTone                              ;
        // no tone specified, use default tone 1
        list[i]->tone_ph = tone_ph                                             ;
      }                                                                       ;
      list[i]->pitch1 = pitch_adjust + phonemes[tone_ph]->StartType             ;
      list[i]->pitch2 = pitch_adjust + phonemes[tone_ph]->EndType               ;
    }                                                                         ;
  }                                                                           ;
}

void N::SpeechTranslator::Pitches (
       PhonemeLists & list        ,
       SpeechTune   & tune        ,
       Syllables    & syllables   ,
       int            clauseType  ,
       int            toneflags   )
{ //  clause_type: 0=. 1=, 2=?, 3=! 4=none
  int ix                                                                 ;
  int x                                                                  ;
  int st_ix                                                              ;
  int n_st             = 0                                               ;
  int option                                                             ;
  int group_tone                                                         ;
  int group_tone_emph                                                    ;
  int group_tone_comma                                                   ;
  int ph_start         = 0                                               ;
  int st_start                                                           ;
  int st_clause_end                                                      ;
  int count                                                              ;
  int n_primary        = 0                                               ;
  int count_primary                                                      ;
  int ph_end           = list.count()                                    ;
  ////////////////////////////////////////////////////////////////////////
  for ( ix = 0 ; ix < ( list.count() - 1 ) ; ix++ )                      {
    if ( list[ix]->synthflags & SFLAG_SYLLABLE)                          {
      Syllable * syllable                                                ;
      syllable = new Syllable ( )                                        ;
      syllables               << syllable                                ;
      syllables[n_st]->Flags       = 0                                   ;
      syllables[n_st]->Envelope    = PITCHfall                           ;
      syllables[n_st]->NextPhoneme = list[ix]->type                      ;
      syllables[n_st]->Stress      = list[ix]->stresslevel               ;
      n_st++                                                             ;
      if ( list[ix]->stresslevel >= 4 ) n_primary++                      ;
    } else
    if ( (list[ix]->ph->Code == Speak::PcPauseClause)          &&
         ( n_st > 0 ) )                                                  {
      syllables[n_st-1]->Flags |= SYL_END_CLAUSE                          ;
    }                                                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  syllables[n_st]->Stress = 0                                             ; // extra 0 entry at the end
  if ( n_st == 0 ) return                                                ; // nothing to do
  ////////////////////////////////////////////////////////////////////////
  if ( options.toneLanguage == 1)                                        {
    PitchesTone ( list )                                                 ;
    return                                                               ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  option = options.intonationGroup                                       ;
  if ( option >= 8 ) option = 1                                          ;
  if ( option == 0 )                                                     {
    group_tone       = options . tunes [ clauseType ]                    ;
    group_tone_emph  = options . tunes [ 5          ]                    ;
    group_tone_comma = options . tunes [ 1          ]                    ;
  } else                                                                 {
    group_tone       = punctToTone[option][clauseType]                   ;
    group_tone_emph  = punctToTone[option][5         ]                   ; // emphatic form of statement
    group_tone_comma = punctToTone[option][1         ]                   ; // emphatic form of statement
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  /* incomplete clause, used for abbreviations such as Mr. Dr. Mrs.     */
  if ( clauseType == 4 ) syllables.NoTonic = 1                           ;
                    else syllables.NoTonic = 0                           ;
  ////////////////////////////////////////////////////////////////////////
  st_start      = 0                                                      ;
  count_primary = 0                                                      ;
  for ( st_ix = 0;  st_ix < n_st ; st_ix++ )                             {
    if ( syllables[st_ix]->Stress >= 4 ) count_primary++                  ;
    if ( syllables[st_ix]->Stress == 6 )                                  {
      // reduce the stress of the previous stressed syllable (review only the previous few syllables)
      for ( ix = st_ix - 1 ; ix >= st_start && ix >= (st_ix-3) ; ix-- )  {
        if ( syllables[ix]->Stress == 6 ) break                           ;
        if ( syllables[ix]->Stress == 4 )                                 {
          syllables[ix]->Stress = 3                                       ;
          break                                                          ;
        }                                                                ;
      }                                                                  ;
      // are the next primary syllables also emphasized ?
      for ( ix = st_ix + 1 ; ix < n_st ; ix++ )                          {
        if ( syllables[ix]->Stress == 4 ) break                           ;
        if ( syllables[ix]->Stress == 6 )                                 {
          // emphasize this syllable, but don't end the current tone group
          syllables[st_ix]->Flags  = SYL_EMPHASIS                         ;
          syllables[st_ix]->Stress = 5                                    ;
          break                                                          ;
        }                                                                ;
      }                                                                  ;
    }                                                                    ;
    if ( syllables[st_ix]->Stress == 6 )                                  {
      // an emphasized syllable, end the tone group after the next primary stress
      syllables[st_ix]->Flags = SYL_EMPHASIS                              ;
      count                  = 0                                         ;
      if ( ( n_primary - count_primary ) > 1 ) count = 1                 ;
      for ( ix = st_ix + 1 ; ix < n_st ; ix++ )                          {
        if ( syllables[ix]->Stress >  4 ) break                           ;
        if ( syllables[ix]->Stress == 4 )                                 {
          count++                                                        ;
          if ( count > 1 ) break                                         ;
        }                                                                ;
      }                                                                  ;
      syllables.PitchVowels(st_start,ix,n_st)                            ;
      if ( ( ix < n_st ) || ( clauseType == 0 ) )                        {
        tune . Pitches                                                   (
          syllables                                                      ,
          st_start                                                       ,
          ix                                                             ,
          option                                                         ,
          group_tone_emph                                                ,
          toneflags                                                    ) ;
      } else                                                             {
        tune . Pitches                                                   (
          syllables                                                      ,
          st_start                                                       ,
          ix                                                             ,
          option                                                         ,
          group_tone                                                     ,
          toneflags                                                    ) ;
      }                                                                  ;
      st_start = ix                                                      ;
    }                                                                    ;
    if ( ( st_start < st_ix                        )                    &&
         ( syllables[st_ix]->Flags & SYL_END_CLAUSE )                   ) {
      // end of clause after this syllable, indicated by a phonPAUSE_CLAUSE phoneme
      st_clause_end = st_ix + 1                                          ;
      syllables.PitchVowels(st_start,st_clause_end,st_clause_end)        ;
      tune . Pitches                                                     (
        syllables                                                        ,
        st_start                                                         ,
        st_clause_end                                                    ,
        option                                                           ,
        group_tone_comma                                                 ,
        toneflags                                                      ) ;
      st_start = st_clause_end                                           ;
    }                                                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  if ( st_start < st_ix )                                                {
    syllables . PitchVowels ( st_start , st_ix , n_st )                  ;
    tune . Pitches                                                       (
      syllables                                                          ,
      st_start                                                           ,
      st_ix                                                              ,
      option                                                             ,
      group_tone                                                         ,
      toneflags                                                        ) ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  // unpack pitch data
  st_ix = 0                                                              ;
  for ( ix = ph_start ; ix < ph_end ; ix++ )                             {
    list[ix]->stresslevel = syllables[st_ix]->Stress                       ;
    if ( list[ix]->synthflags & SFLAG_SYLLABLE)                           {
      list[ix]->pitch1   = syllables[st_ix]->Pitch1                        ;
      list[ix]->pitch2   = syllables[st_ix]->Pitch2                        ;
      list[ix]->envelope = PITCHfall                                      ;
      if ( syllables[st_ix]->Flags & SYL_RISE )                           {
        list[ix]->envelope = PITCHrise                                    ;
      } else
      if (list[ix]->stresslevel > 5)                                      {
        list[ix]->envelope = syllables[st_ix]->Envelope                    ;
      }                                                                  ;
      if (list[ix]->pitch1 > list[ix]->pitch2)                             {
        // swap so that pitch2 is the higher
        x               = list[ix]->pitch1                                ;
        list[ix]->pitch1 = list[ix]->pitch2                                ;
        list[ix]->pitch2 = x                                              ;
      }                                                                  ;
      if ( list[ix]->tone_ph )                                            {
        x = ( list[ix]->pitch1 + list[ix]->pitch2 ) / 2                    ;
        list[ix]->pitch1 = x + phonemes[list[ix]->tone_ph]->StartType       ;
        list[ix]->pitch2 = x + phonemes[list[ix]->tone_ph]->EndType         ;
      }                                                                  ;
      if (syllables[st_ix]->Flags & SYL_EMPHASIS)                         {
        list[ix]->stresslevel |= 8                                        ; // emphasized
      }                                                                  ;
      st_ix++                                                            ;
    }                                                                    ;
  }                                                                      ;
}

int N::SpeechTranslator::isspace2(unsigned int c)
{ // can't use isspace() because on Windows, isspace(0xe1) gives TRUE !
  int c2                                                     ;
  if ( ( ( c2 = (c & 0xff) ) == 0) || ( c > ' ' ) ) return 0 ;
  return 1                                                   ;
}

int N::SpeechTranslator::iswalpha2(int c)
{
  if (   c < 0x80                            ) return isalpha  ( c ) ;
  if ( ( c > 0x3040     ) && ( c <= 0xa700 ) ) return 1              ; // japanese, chinese characters
  if (   c > MAX_WALPHA                      ) return iswalpha ( c ) ;
  return ( walpha_tab [ c - 0x80 ] )                                 ;
}

int N::SpeechTranslator::iswlower2(int c)
{
  if ( c < 0x80                   ) return islower (c) ;
  if ( c > MAX_WALPHA             ) return iswlower(c) ;
  if ( walpha_tab[c-0x80] == 0xff ) return true        ;
  return false                                         ;
}

int N::SpeechTranslator::iswupper2(int c)
{
  int x;
  if ( c < 0x80                                          ) return isupper (c) ;
  if ( c > MAX_WALPHA                                    ) return iswupper(c) ;
  if ( ( ( x = walpha_tab[c-0x80] ) > 0) && ( x < 0xfe ) ) return 1           ;
  return 0                                                                    ;
}

int N::SpeechTranslator::towlower2(unsigned int c)
{
  int x                                                   ;
  int ix                                                  ;
  // check for non-standard upper to lower case conversions
  if ( c == 'I' )                                         {
    if ( options.dotlessI ) c = 0x131                     ; // I -> ı
  }                                                       ;
  if ( c < 0x80       ) return tolower (c)                ;
  if ( c > MAX_WALPHA ) return towlower(c)                ;
  if ( ( x = walpha_tab[c-0x80] ) >= 0xfe ) return c      ; // this is not an upper case letter
  if ( x == 0xfd )                                        {
    // special cases, lookup translation table
    for ( ix = 0 ; wchar_tolower[ix] != 0 ; ix+=2 )       {
      if ( wchar_tolower[ix] == (int)c )                  {
        return ( wchar_tolower [ ix + 1 ] )               ;
      }                                                   ;
    }                                                     ;
  }                                                       ;
  return ( c + x )                                        ; // convert to lower case
}

int N::SpeechTranslator::towupper2(unsigned int c)
{
  int ix;
  if ( c > MAX_WALPHA            ) return ( towupper(c) ) ;
  // check whether a previous character code is the upper-case equivalent of this character
  if ( towlower2(c-32) == (int)c ) return ( c - 32      ) ; // yes, use it
  if ( towlower2(c- 1) == (int)c ) return ( c -  1      ) ;
  for ( ix = 0 ; wchar_toupper[ix] != 0 ; ix+=2 )         {
    if ( wchar_toupper[ix] == (int)c )                    {
      return ( wchar_toupper [ ix + 1 ] )                 ;
    }                                                     ;
  }                                                       ;
  return c                                                ; // no
}

int N::SpeechTranslator::IsRomanU(unsigned int c)
{
  if ( (c=='I')            ||
       (c=='V')            ||
       (c=='X')            ||
       (c=='L')  ) return 1 ;
  return 0                  ;
}

int N::SpeechTranslator::lookupwchar(const unsigned short *list,int c)
{ // Is the character c in the list ?
  int ix                                      ;
  for ( ix = 0 ; list[ix] != 0 ; ix++ )       {
    if ( list [ ix ] == c ) return ( ix + 1 ) ;
  }                                           ;
  return 0                                    ;
}

int N::SpeechTranslator::lookupwchar2(const unsigned short * list,int c)
{
  // Replace character c by another character.
  // Returns 0 = not found, 1 = delete character
  int ix                                               ;
  for ( ix = 0 ; list[ix] != 0 ; ix+=2 )               {
    if ( list [ ix ] == c ) return ( list [ ix + 1 ] ) ;
  }                                                    ;
  return 0                                             ;
}

int N::SpeechTranslator::IsBracket(int c)
{
  if ( ( c >= 0x2014 ) && ( c <= 0x201f ) ) return 1 ;
  return lookupwchar ( brackets , c )                ;
}

int N::SpeechTranslator::IsAlpha(unsigned int c)
{
  // Replacement for iswalph() which also checks for some in-word symbols
  static const unsigned short extra_indic_alphas[] =     {
    0xa70 , 0xa71                                        , // Gurmukhi: tippi, addak
    0                                                  } ;
  if ( iswalpha2(c) ) return 1                           ;
  if ( c < 0x300    ) return 0                           ;
  if ( ( c >= 0x901 ) && ( c <= 0xdf7 ) )                {
    // Indic scripts: Devanagari, Tamil, etc
    if ( ( c & 0x7f ) < 0x64 ) return 1                  ;
    if ( lookupwchar(extra_indic_alphas,c)!=0 ) return 1 ;
    if ( ( c >= 0xd7a ) && ( c <= 0xd7f )     ) return 1 ; // malaytalam chillu characters
    return 0                                             ;
  }                                                      ;
  if ( ( c >= 0x5b0 ) && ( c <= 0x5c2 ) ) return 1       ; // Hebrew vowel marks
  if (   c == 0x0605) return 1                           ;
  if ( ( c >= 0x64b ) && ( c <= 0x65e  ) ) return 1      ; // arabic vowel marks
  if ( ( c >= 0x300 ) && ( c <= 0x36f  ) ) return 1      ; // combining accents
  if ( ( c >= 0x780 ) && ( c <= 0x7b1  ) ) return 1      ; // taani/divehi (maldives)
  if ( ( c >= 0xf40 ) && ( c <= 0xfbc  ) ) return 1      ; // tibetan
  if ( ( c >= 0x1100) && ( c <= 0x11ff ) ) return 1      ; //Korean jamo
  if ( ( c >= 0x2800) && ( c <= 0x28ff ) ) return 1      ; // braille
  if ( ( c >  0x3040) && ( c <= 0xa700 ) ) return 1      ; // Chinese/Japanese.  Should never get here, but Mac OS 10.4's iswalpha seems to be broken, so just make sure
  return 0                                               ;
}

int N::SpeechTranslator::IsNumber(unsigned int c)
{
  if ( ( c >= '0' ) && ( c <= '9' ) ) return 1 ;
  return 0                                     ;
}

int N::SpeechTranslator::IsDigit(unsigned int c)
{
  if ( iswdigit(c)                      ) return 1 ;
  if ( ( c >= 0x966 ) && ( c <= 0x96f ) ) return 1 ;
  return 0                                         ;
}

int N::SpeechTranslator::IsSpace(unsigned int c)
{
  if (  c == 0                          ) return 0 ;
  if (( c >= 0x2500 ) && ( c < 0x25a0 ) ) return 1 ; // box drawing characters
  return iswspace(c)                               ;
}

int N::SpeechTranslator::IsAllUpper(const char * word)
{
  int c                                          ;
  while ( ( (*word) != 0 ) && !isspace2(*word) ) {
    word += toGrapheme(c,word)                   ;
    if ( !iswupper2(c) ) return 0                ;
  }                                              ;
  return 1                                       ;
}

int N::SpeechTranslator::isHexDigit(int c)
{
  if ( ( c >= '0' ) && ( c <= '9' ) ) return ( c - '0'      ) ;
  if ( ( c >= 'a' ) && ( c <= 'f' ) ) return ( c - 'a' + 10 ) ;
  if ( ( c >= 'A' ) && ( c <= 'F' ) ) return ( c - 'A' + 10 ) ;
  return -1                                                   ;
}

int N::SpeechTranslator::NonAsciiNumber(int letter)
{ // Change non-ascii digit into ascii digit '0' to '9', (or -1 if not)
  const int * p                                        ;
  int         base                                     ;
  for ( p = number_ranges ; (base = *p) != 0 ; p++ )   {
    if ( letter <  base     ) break                    ; // not found
    if ( letter < (base+10) ) return (letter-base+'0') ;
  }                                                    ;
  return -1                                            ;
}

bool N::SpeechTranslator::thousandsGroup(const char * word,int groupLen)
{ // Is this a group of 3 digits which looks like a thousands group?
  if ( IsNumber(word[groupLen]) || IsNumber(-1) ) return false ;
  for (int i=0 ; i < groupLen ; i++)                           {
    if( ! IsNumber(word[i]) ) return false                     ;
  }                                                            ;
  return true                                                  ;
}

bool N::SpeechTranslator::IsNumberHuE(const char * word,int thousandplex,int value)
{ // lang-hu: variant form of numbers when followed by hyphen and a suffix starting with 'a' or 'e' (but not a, e, az, ez, azt, ezt, att. ett
  if ( ( word[0] == 'a' ) || ( word[0] == 'e' ) )                         {
    if ( ( word[1] == ' ' ) || (word[1] == 'z')                          ||
       ( ( word[1] == 't' ) && ( word[2] == 't' ) )        ) return false ;
    if ( ( ( thousandplex == 1 )                                         ||
         ( ( value % 1000 ) == 0 ) ) && ( word[1] == 'l' ) ) return false ;   // 1000-el
    return true                                                           ;
  }                                                                       ;
  return false                                                            ;
}

char * N::SpeechTranslator::wstrchr(const char * s,int c)
{
  if ( c >= 0x80 ) return NULL ;
  return strchr((char *)s,c)   ;
}

int N::SpeechTranslator::Substitute (
      unsigned int  c             ,
      unsigned int  next_in       ,
      int         * insert        ,
      int         * wordflags     )
{
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  int ix;
  unsigned int word;
  unsigned int new_c, c2, c_lower;
  int          upper_case = 0;
  static int   ignore_next = 0;
  const unsigned int *replace_chars;

  if(ignore_next)
  {
          ignore_next = 0;
          return(8);
  }
  if(c == 0) return(0);

  if((replace_chars = tr->langopts.replace_chars) == NULL)
          return(c);

  // there is a list of character codes to be substituted with alternative codes

  if(iswupper2(c_lower = c))
  {
          c_lower = towlower2(c);
          upper_case = 1;
  }

  new_c = 0;
  for(ix=0; (word = replace_chars[ix]) != 0; ix+=2)
  {
          if(c_lower == (word & 0xffff))
          {
                  if((word >> 16) == 0)
                  {
                          new_c = replace_chars[ix+1];
                          break;
                  }
                  if((word >> 16) == (unsigned int)towlower2(next_in))
                  {
                          new_c = replace_chars[ix+1];
                          ignore_next = 1;
                          break;
                  }
          }
  }

  if(new_c == 0)
          return(c);    // no substitution

  if(new_c & 0xffe00000)
  {
          // there is a second character to be inserted
          // don't convert the case of the second character unless the next letter is also upper case
          c2 = new_c >> 16;
          if(upper_case && iswupper2(next_in))
                  c2 = towupper2(c2);
          *insert = c2;
          new_c &= 0xffff;
  }

  if(upper_case)
          new_c = towupper2(new_c);

  *wordflags |= FLAG_CHAR_REPLACED;
  return(new_c);
#endif
  return 0 ;
}

bool N::SpeechTranslator::irishUpperCase(char * word,int c)
{
  int          len                                 ;
  const char * p                                   ;
  //////////////////////////////////////////////////
  if ( translatorName != L('g','a') ) return false ;
  for (int i = 0 ; NotNull(UCase_ga[i]) ; i++ )    {
    p = UCase_ga[i]                                ;
    if (IsNull(p)) return false                    ;
    len = strlen ( p )                             ;
    if ( ( word[-len]==' '                      ) &&
         ( memcmp(&word[-len+1], p, len-1) == 0))  {
      if ( ( c == p[len-1] )                      ||
         ( ( p[len-1]=='A' )                      &&
             isVowel(c) ) ) return true            ;
    }                                              ;
  }                                                ;
  return false                                     ;
}

wchar_t * N::SpeechTranslator::SsmlAttribute (
            wchar_t    * attr              ,
            wchar_t    * pw                ,
            const char * name              )
{ // Gets the value string for an attribute.
  int ix                                                      ;
  attr[0] = 0                                                 ;
  while ( (*pw) != 0)                                         {
    if ( iswspace(pw[-1]) )                                   {
      ix = 0                                                  ;
      while ( (*pw) == name[ix] )                             {
        pw++                                                  ;
        ix++                                                  ;
      }                                                       ;
      if ( name[ix] == 0 )                                    { // found the attribute, now get the value
        while ( iswspace(*pw) ) pw++                          ;
        if    ( (*pw) == '='  ) pw++                          ;
        while ( iswspace(*pw) ) pw++                          ;
        if ( ( (*pw) == '"') || ( (*pw) == '\'')) return pw+1 ;
                                             else return attr ;
      }                                                       ;
    }                                                         ;
    pw++                                                      ;
  }                                                           ;
  return NULL                                                 ;
}

int N::SpeechTranslator::VowelStress    (
      unsigned char * phs               ,
      signed   char * vowel_stress      ,
      int           & vowel_count       ,
      int           & stressed_syllable ,
      int             control           )
{ // control = 1, set stress to 1 for forced unstressed vowels
  unsigned char   phcode                                                 ;
  unsigned char * ph_out       = phs                                     ;
  int             count        =  1                                      ;
  int             max_stress   = -1                                      ;
  int             ix                                                     ;
  int             j                                                      ;
  int             stress       = -1                                      ;
  int             primary_posn =  0                                      ;
//          PHONEME_TAB *ph;
  ////////////////////////////////////////////////////////////////////////
  vowel_stress[0] = 1                                                    ;
  while ( ( ( phcode = *phs++ ) != 0 )                                  &&
        ( count  < ( N_WORD_PHONEMES / 2 ) - 1 ) )                       {
    if ( ! phonemes.contains(phcode) ) continue                          ;
    if ( ( phonemes[phcode]->Type    == Speak::STRESS )        &&
         ( phonemes[phcode]->Program == 0                             ) ) {
      /* stress marker, use this for the following vowel                */
      if ( phcode == Speak::PcStressPrev )                     {
        /* primary stress on preceeding vowel                           */
        j = count - 1                                                    ;
        while ( ( j > 0                  )                              &&
                ( stressed_syllable == 0 )                              &&
                ( vowel_stress[j]   <  4 )                             ) {
          if ( ( vowel_stress[j] != 0 ) && ( vowel_stress[j] != 1 ) )    {
            /* don't promote a phoneme which must be unstressed         */
            vowel_stress[j] = 4                                          ;
            if ( max_stress < 4 )                                        {
              max_stress   = 4                                           ;
              primary_posn = j                                           ;
            }                                                            ;
            /* reduce any preceding primary stress markers              */
            for ( ix = 1 ; ix < j ; ix++ )                               {
              if ( vowel_stress[ix] == 4 ) vowel_stress[ix] = 3          ;
            }                                                            ;
            break                                                        ;
          }                                                              ;
          j--                                                            ;
        }                                                                ;
      } else                                                             {
        if ( (phonemes[phcode]->Length<4) || (stressed_syllable==0) )     {
          stress = phonemes[phcode]->Length                               ;
          if ( stress > max_stress ) max_stress = stress                 ;
        }                                                                ;
      }                                                                  ;
      continue                                                           ;
    }                                                                    ;
    if ( ( phonemes[phcode]->Type == Speak::VOWEL       )      &&
        !( phonemes[phcode]->Flags & Speak::NONSYLLABIC ) )     {
      vowel_stress[count] = (char)stress                                 ;
      if ( ( stress >= 4 ) && ( stress >= max_stress ) )                 {
        primary_posn = count                                             ;
        max_stress   = stress                                            ;
      }                                                                  ;
      if ( ( stress < 0 ) && ( control & 1 )                            &&
           ( phonemes[phcode]->Flags & Speak::UNSTRESSED )    ) {
        /* weak vowel, must be unstressed */
        vowel_stress[count] = 1                                          ;
      }                                                                  ;
      count++                                                            ;
      stress = -1                                                        ;
    } else
    if ( phcode == Speak::PcSyllabic )                         {
      // previous consonant phoneme is syllablic
      vowel_stress[count] = (char)stress                                 ;
      if ( ( stress==0 ) && ( control & 1 ) ) vowel_stress[count++] = 1  ; // syllabic consonant, usually unstressed
    }                                                                    ;
    *ph_out++ = phcode                                                   ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  vowel_stress[count] = 1                                                ;
  *ph_out             = 0                                                ;
  ////////////////////////////////////////////////////////////////////////
  /* has the position of the primary stress been specified by $1, $2, etc? */
  if ( stressed_syllable > 0 )                                           {
    if ( stressed_syllable >= count ) stressed_syllable = count - 1      ; // the final syllable
    vowel_stress[stressed_syllable] = 4                                  ;
    max_stress                      = 4                                  ;
    primary_posn                    = stressed_syllable                  ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  if ( max_stress == 5 )                                                 {
    // priority stress, replaces any other primary stress marker
    for ( ix = 1 ; ix < count ; ix++ )                                   {
      if ( vowel_stress[ix] == 4 )                                       {
        if ( options.stressFlags & S_PRIORITY_STRESS)                    {
          vowel_stress[ix] = 1                                           ;
        } else                                                           {
          vowel_stress[ix] = 3                                           ;
        }                                                                ;
      }                                                                  ;
      if ( vowel_stress[ix] == 5 )                                       {
        vowel_stress[ix] = 4                                             ;
        primary_posn     = ix                                            ;
      }                                                                  ;
    }                                                                    ;
    max_stress = 4                                                       ;
  }                                                                      ;
  stressed_syllable = primary_posn                                       ;
  vowel_count       = count                                              ;
  return max_stress                                                      ;
}

void N::SpeechTranslator::ChangeStress(char * word,int newStress)
{
  int             ix                                                     ;
  unsigned char * p                                                      ;
  int             max_stress                                             ;
  int             vowel_count           ; // num of vowels + 1
  int             stressed_syllable = 0 ; // position of stressed syllable
  unsigned char   phonetic     [ N_WORD_PHONEMES     ]                   ;
  signed char     vowel_stress [ N_WORD_PHONEMES / 2 ]                   ;
  ////////////////////////////////////////////////////////////////////////
  strcpy ( (char *)phonetic , word )                                     ;
  max_stress = VowelStress                                               (
                 phonetic                                                ,
                 vowel_stress                                            ,
                 vowel_count                                             ,
                 stressed_syllable                                       ,
                 0                                                     ) ;
  ////////////////////////////////////////////////////////////////////////
  if ( newStress >= 4)                                                   {
    // promote to primary stress
    for ( ix = 1 ; ix < vowel_count ; ix++ )                             {
      if ( vowel_stress[ix] >= max_stress )                              {
        vowel_stress[ix] = newStress                                     ;
        break                                                            ;
      }                                                                  ;
    }                                                                    ;
  } else                                                                 {
    // remove primary stress
    for ( ix = 1 ; ix < vowel_count ; ix++ )                             {
      if ( vowel_stress[ix] > newStress )                                {
        // >= allows for diminished stress (=1)
        vowel_stress[ix] = newStress                                     ;
      }                                                                  ;
    }                                                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  // write out phonemes
  ix = 1                                                                 ;
  p  = phonetic                                                          ;
  while ( (*p) != 0)                                                     {
    if (phonemes.contains(*p))                                           {
      if ( ( phonemes[*p]->Type == Speak::VOWEL       )                  &&
          !( phonemes[*p]->Flags & Speak::NONSYLLABIC )                 ) {
        if ( ( vowel_stress[ix] == 0) || ( vowel_stress[ix] > 1 ) )      {
          *word++ = stress_phonemes[(unsigned char)vowel_stress[ix]]     ;
        }                                                                ;
        ix++                                                             ;
      }                                                                  ;
    }                                                                    ;
    *word++ = *p++                                                       ;
  }                                                                      ;
  *word = 0                                                              ;
}

void N::SpeechTranslator::SetStress      (
       char         * output           ,
       unsigned int * dictionary_flags ,
       int            tonic            ,
       int            control          )
{ /* Guess stress pattern of word.  This is language specific
     'output' is used for input and output
     'dictionary_flags' has bits 0-3   position of stressed vowel
     (if > 0) or unstressed (if == 7) or syllables 1 and 2 (if == 6)
     bits 8...  dictionary flags
     If 'tonic' is set (>= 0), replace highest stress by this value.
      control:  bit 0   This is an individual symbol, not a word           */
  unsigned char   phcode                                                 ;
  unsigned char * p                                                      ;
  int             stress                                                 ;
  int             max_stress                                             ;
  int             vowel_count                                            ; // num of vowels + 1
  int             ix                                                     ;
  int             v                                                      ;
  int             v_stress                                               ;
  int             stressed_syllable                                      ; // position of stressed syllable
  int             max_stress_posn                                        ;
  int             unstressed_word = 0                                    ;
  char          * max_output                                             ;
  int             final_ph                                               ;
  int             final_ph2                                              ;
  int             mnem                                                   ;
  int             post_tonic                                             ; // currently not used
  int             opt_length                                             ;
  int             done                                                   ;
  int             stressflags                                            ;
  int             dflags = 0                                             ;
  int             first_primary                                          ;
  int             long_vowel                                             ;
  signed char     vowel_stress    [ N_WORD_PHONEMES / 2 ]                ;
  char            syllable_weight [ N_WORD_PHONEMES / 2 ]                ;
  char            vowel_length    [ N_WORD_PHONEMES / 2 ]                ;
  unsigned char   phonetic        [ N_WORD_PHONEMES     ]                ;
  ////////////////////////////////////////////////////////////////////////
  /* stress numbers  STRESS_BASE +
     0  diminished, unstressed within a word
     1  unstressed, weak
     2
     3  secondary stress
     4  main stress                                                    */
  stressflags = options.stressFlags                                     ;
  if ( IsNull(dictionary_flags) ) dflags = dictionary_flags[0]          ;
  /* copy input string into internal buffer                            */
  for ( ix = 0 ; ix < N_WORD_PHONEMES ; ix++ )                          {
    phonetic[ix] = output[ix]                                           ;
    // check for unknown phoneme codes
    if (!phonemes.contains(phonetic[ix]))                               {
      phonetic[ix] = Speak::PcSchwa                           ;
    }                                                                   ;
    if (phonetic[ix] == 0) break                                        ;
  }                                                                     ;
  if ( ix == 0 ) return                                                 ;
  ///////////////////////////////////////////////////////////////////////
  final_ph   = phonetic[ix-1]                                           ;
  final_ph2  = phonetic[ix-2]                                           ;
  max_output = output + (N_WORD_PHONEMES-3)                             ; /* check for overrun */
  // any stress position marked in the xx_list dictionary ?
  stressed_syllable = dflags & 0x7                                      ;
  if ( dflags & 0x8 )                                                   {
    // this indicates a word without a primary stress
    stressed_syllable = dflags & 0x3                                    ;
    unstressed_word   = 1                                               ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  max_stress = VowelStress                                              (
                 phonetic                                               ,
                 vowel_stress                                           ,
                 vowel_count                                            ,
                 stressed_syllable                                      ,
                 1                                                    ) ;
  if ( ( max_stress < 0 ) && dictionary_flags ) max_stress = 0          ;
  ///////////////////////////////////////////////////////////////////////
  // heavy or light syllables
  ix = 1                                                                ;
  for ( p = phonetic ; (*p) != 0 ; p++ )                                {
    if ( ( phonemes[p[0]]->Type  == Speak::VOWEL       ) &&
        !( phonemes[p[0]]->Flags &  Speak::NONSYLLABIC ) )     {
      int weight     = 0                                                ;
      int lengthened = 0                                                ;
      if ( phonemes[p[1]]->Code == Speak::PcLengthen )         {
        lengthened = 1                                                  ;
      }                                                                 ;
      if (lengthened || (phonemes[p[0]]->Flags & Speak::LONG)) {
        // long vowel, increase syllable weight
        weight++                                                        ;
      }                                                                 ;
      vowel_length [ ix ] = weight                                      ;
      if ( lengthened ) p++                                             ; // advance over phonLENGTHEN
      if (consonant_types[phonemes[p[1]]->Type]                         &&
          ( ( phonemes[p[2]]->Type != Speak::VOWEL )          ||
            ( phonemes[p[1]]->Flags & Speak::LONG  ) ) )       {
        // followed by two consonants, a long consonant, or consonant and end-of-word
        weight++                                                        ;
      }                                                                 ;
      syllable_weight[ix] = weight                                      ;
      ix++                                                              ;
    }                                                                   ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  switch ( options.stressRule )                                         {
    case 8                                                              :
      // stress on first syllable, unless it is a light syllable followed by a heavy syllable
      if ( ( syllable_weight[1] > 0) || (syllable_weight[2] == 0 ) )    {
        break                                                           ;
      }                                                                 ;
      // else drop through to case 1
    case 1                                                              :
      // stress on second syllable
      if ( ( stressed_syllable == 0 ) && ( vowel_count > 2 ) )          {
        stressed_syllable = 2                                           ;
        if ( max_stress == 0 )                                          {
          vowel_stress[stressed_syllable] = 4                           ;
        }                                                               ;
        max_stress = 4                                                  ;
      }                                                                 ;
    break                                                               ;
    case 10                                                             : // penultimate, but final if only 1 or 2 syllables
      if ( stressed_syllable == 0 )                                     {
        if ( vowel_count < 4 )                                          {
          vowel_stress[vowel_count - 1] = 4                             ;
          max_stress                    = 4                             ;
          break                                                         ;
        }                                                               ;
      }                                                                 ;
      // drop through to next case
    case 2                                                              :
      // a language with stress on penultimate vowel
      if ( stressed_syllable == 0 )                                     {
        /* no explicit stress - stress the penultimate vowel */
        max_stress = 4                                                  ;
        if ( vowel_count > 2 )                                          {
          stressed_syllable = vowel_count - 2                           ;
          if ( stressflags & (S_FINAL_SPANISH | S_FINAL_STRESS_C) )     {
            // LANG=Spanish, stress on last vowel if the word ends in a consonant other than 'n' or 's'
            if ( phonemes[final_ph]->Type == Speak::VOWEL )    {
              if ( stressflags & S_FINAL_STRESS_C )                     {
                stressed_syllable = vowel_count - 1                     ;
              } else                                                    {
                mnem = phonemes[final_ph]->Mnemonic                      ;
                if ( translatorName == L('a','n') )                     {
                  if ( ( ( mnem != 's' ) && ( mnem !='n' ) )           ||
                      phonemes[final_ph2]->Type == Speak::VOWEL)
                    stressed_syllable = vowel_count - 1                 ; // stress on last syllable
              } else                                                    {
                if ( ( mnem == 's' ) &&
                 ( phonemes[final_ph2]->Type == Speak::NASAL)) {
                  // -ns  stress remains on penultimate syllable
                } else
                if ( ( ( phonemes[final_ph]->Type != Speak::NASAL) &&
                       (mnem != 's')) ||
                     ( phonemes[final_ph2]->Type != Speak::VOWEL)) {
                  stressed_syllable = vowel_count - 1                   ;
                }                                                       ;
              }                                                         ;
            }                                                           ;
          }                                                             ;
        }                                                               ;
        if ( stressflags & S_FINAL_LONG )                               {
          // stress on last syllable if it has a long vowel, but previous syllable has a short vowel
          if ( vowel_length[vowel_count - 1] > vowel_length[vowel_count - 2]) {
            stressed_syllable = vowel_count - 1                               ;
          }                                                             ;
        }                                                               ;
        if ( ( vowel_stress[stressed_syllable] == 0 )                  ||
             ( vowel_stress[stressed_syllable] == 1 ) )                 {
          // but this vowel is explicitly marked as unstressed
            if ( stressed_syllable > 1 ) stressed_syllable--            ;
                                    else stressed_syllable++            ;
          }                                                             ;
        } else                                                          {
          stressed_syllable = 1                                         ;
        }                                                               ;
        // only set the stress if it's not already marked explicitly
        if ( vowel_stress[stressed_syllable] < 0 )                      {
          // don't stress if next and prev syllables are stressed
          if ( ( vowel_stress[stressed_syllable-1] < 4 )               ||
               ( vowel_stress[stressed_syllable+1] < 4 )              ) {
            vowel_stress[stressed_syllable] = max_stress                ;
          }                                                             ;
        }                                                               ;
      }                                                                 ;
    break                                                               ;
    case 3                                                              :
      // stress on last vowel
      if ( stressed_syllable == 0 )                                     {
        /* no explicit stress - stress the final vowel */
        stressed_syllable = vowel_count - 1                             ;
        while ( stressed_syllable > 0 )                                 {
          // find the last vowel which is not unstressed
          if ( vowel_stress[stressed_syllable] < 0 )                    {
            vowel_stress[stressed_syllable] = 4                         ;
            break                                                       ;
          } else stressed_syllable--                                    ;
        }                                                               ;
        max_stress = 4                                                  ;
      }                                                                 ;
    break                                                               ;
    case 4                                                              : // stress on antipenultimate vowel
      if ( stressed_syllable == 0 )                                     {
        stressed_syllable = vowel_count - 3                             ;
        if ( stressed_syllable < 1 ) stressed_syllable = 1              ;
        if ( max_stress == 0 )                                          {
          vowel_stress[stressed_syllable] = 4                           ;
        }                                                               ;
        max_stress = 4                                                  ;
      }                                                                 ;
    break                                                               ;
    case 5                                                              :
      // LANG=Russian
      if ( stressed_syllable == 0 )                                     {
        /* no explicit stress - guess the stress from the number of syllables */
        stressed_syllable = vowel_count - 3                             ;
        if ( vowel_count < 16 )                                         {
          if ( phonemes[final_ph]->Type == Speak::VOWEL )
            stressed_syllable = guess_ru_v[vowel_count]                 ;
          else
          if ( phonemes[final_ph]->Type == Speak::STOP  )
            stressed_syllable = guess_ru_t[vowel_count]                 ;
          else stressed_syllable = guess_ru[vowel_count]                ;
        }                                                               ;
        vowel_stress[stressed_syllable] = 4                             ;
        max_stress                      = 4                             ;
      }                                                                 ;
    break                                                               ;
    case 6                                                              : // LANG=hi stress on the last heaviest syllable
      if ( stressed_syllable == 0 )                                     {
        int wt                                                          ;
        int max_weight = -1                                             ;
        // find the heaviest syllable, excluding the final syllable
        for ( ix = 1 ; ix < (vowel_count-1) ; ix++ )                    {
          if ( vowel_stress[ix] < 0 )                                   {
            if ( ( wt = syllable_weight[ix] ) >= max_weight)            {
              max_weight        = wt                                    ;
              // prev_stressed = stressed_syllable                         ;
              stressed_syllable = ix                                    ;
            }                                                           ;
          }                                                             ;
        }                                                               ;
        if ( ( syllable_weight[vowel_count-1] == 2 ) &&  ( max_weight< 2 ) ) {
          // the only double=heavy syllable is the final syllable, so stress this
          stressed_syllable = vowel_count-1                             ;
        } else
        if ( max_weight <= 0 )                                          {
          // all syllables, exclusing the last, are light. Stress the first syllable
          stressed_syllable = 1                                         ;
        }                                                               ;
        vowel_stress[stressed_syllable] = 4                             ;
        max_stress                      = 4                             ;
      }                                                                 ;
    break                                                               ;
    case 7                                                              :
      // LANG=tr, the last syllable for any vowel marked explicitly as unstressed
      if ( stressed_syllable == 0 )                                     {
        stressed_syllable = vowel_count - 1                             ;
        for ( ix=1 ; ix < vowel_count ; ix++ )                          {
          if ( vowel_stress[ix] == 1 )                                  {
            stressed_syllable = ix - 1                                  ;
            break                                                       ;
          }                                                             ;
        }                                                               ;
        vowel_stress[stressed_syllable] = 4                             ;
        max_stress                      = 4                             ;
       }                                                                ;
    break                                                               ;
    case 9                                                              :
      // mark all as stressed
      for ( ix = 1 ; ix < vowel_count ; ix++ )                          {
        if ( vowel_stress[ix] < 0 ) vowel_stress[ix] = 4                ;
      }                                                                 ;
    break                                                               ;
    case 12                                                             :
      // LANG=kl (Greenlandic)
      long_vowel = 0                                                    ;
      for ( ix = 1 ; ix < vowel_count ; ix++ )                          {
        if ( vowel_stress[ix] == 4 ) vowel_stress[ix] = 3               ; // change marked stress (consonant clusters) to secondary (except the last)
        if ( vowel_length[ix] >  0 )                                    {
          long_vowel       = ix                                         ;
          vowel_stress[ix] = 3                                          ;
          // give secondary stress to all long vowels
        }                                                               ;
      }                                                                 ;
      // 'stressed_syllable' gives the last marked stress
      if ( stressed_syllable == 0 )                                     {
        // no marked stress, choose the last long vowel
        if ( long_vowel > 0 ) stressed_syllable = long_vowel ; else     {
          // no long vowels or consonant clusters
          if ( vowel_count > 5 ) stressed_syllable = vowel_count - 3    ; // more than 4 syllables
                            else stressed_syllable = vowel_count - 1    ;
        }                                                               ;
      }                                                                 ;
      vowel_stress[stressed_syllable] = 4                               ;
      max_stress                      = 4                               ;
    break                                                               ;
  }                                                                     ;
  /* now guess the complete stress pattern */
  if ( max_stress < 4 ) stress = 4                                      ; /* no primary stress marked, use for 1st syllable */
                   else stress = 3                                      ;
  if ( unstressed_word == 0 )                                           {
    if ( ( stressflags & S_2_SYL_2 ) && ( vowel_count == 3 ) )          {
      // Two syllable word, if one syllable has primary stress, then give the other secondary stress
      if ( vowel_stress[1] == 4 ) vowel_stress[2] = 3                   ;
      if ( vowel_stress[2] == 4 ) vowel_stress[1] = 3                   ;
    }                                                                   ;
    if ( ( stressflags & S_INITIAL_2 ) && ( vowel_stress[1] < 0 ) )     {
      // If there is only one syllable before the primary stress, give it a secondary stress
      if ( ( vowel_count > 3 ) && ( vowel_stress[2] >= 4 ) )            {
        vowel_stress[1] = 3                                             ;
      }                                                                 ;
    }                                                                   ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  done          = 0                                                     ;
  first_primary = 0                                                     ;
  for ( v = 1 ; v < vowel_count ; v++ )                                 {
    if ( vowel_stress[v] < 0 )                                          {
      if ( ( stressflags & S_FINAL_NO_2 )                              &&
           ( stress < 4 ) && ( v == vowel_count-1 ) )                   {
        // flag: don't give secondary stress to final vowel
      } else
      if ( ( stressflags & 0x8000 ) && ( done == 0 ) )                  {
        vowel_stress[v] = (char)stress                                  ;
        done            = 1                                             ;
        stress          = 3                                             ; /* use secondary stress for remaining syllables */
      } else
      if ( ( vowel_stress[v-1] <= 1 )                                  &&
         ( ( vowel_stress[v+1] <= 1 )                                  ||
         ( ( stress == 4 ) && ( vowel_stress[v+1] <= 2 ) ) ) )          {
        /* trochaic: give stress to vowel surrounded by unstressed vowels */
        if ( (stress == 3) && ( stressflags & S_NO_AUTO_2 ) ) continue  ; // don't use secondary stress
        if ( ( v > 1 ) && ( stressflags & S_2_TO_HEAVY )               &&
             (syllable_weight[v]==0) && (syllable_weight[v+1] > 0) )    {
          // don't put secondary stress on a light syllable which is followed by a heavy syllable
          continue                                                      ;
        }                                                               ;
        // should start with secondary stress on the first syllable, or should it count back from
        // the primary stress and put secondary stress on alternate syllables?
        vowel_stress[v] = (char)stress                                  ;
        done            = 1                                             ;
        stress          = 3                                             ; /* use secondary stress for remaining syllables */
      }                                                                 ;
    }                                                                   ;
    /////////////////////////////////////////////////////////////////////
    if ( vowel_stress[v] >= 4 )                                         {
      if ( first_primary == 0 ) first_primary = v                  ; else
      if ( stressflags & S_FIRST_PRIMARY )                              {
        // reduce primary stresses after the first to secondary
        vowel_stress[v] = 3                                             ;
      }                                                                 ;
    }                                                                   ;
  }                                                                     ;
  if ( ( unstressed_word ) && ( tonic < 0 ) )                           {
    if ( vowel_count <= 2 ) tonic = options.unstressedWD1               ; /* monosyllable - unstressed */
                       else tonic = options.unstressedWD2               ; /* more than one syllable, used secondary stress as the main stress */
  }                                                                     ;
  max_stress      = 0                                                   ;
  max_stress_posn = 0                                                   ;
  for ( v = 1 ; v < vowel_count ; v++ )                                 {
    if ( vowel_stress[v] >= max_stress )                                {
      max_stress      = vowel_stress[v]                                 ;
      max_stress_posn = v                                               ;
    }                                                                   ;
  }                                                                     ;
  if (tonic >= 0)                                                       {
    /* find position of highest stress, and replace it by 'tonic'      */
    /* don't disturb an explicitly set stress by 'unstress-at-end' flag*/
    if ( ( tonic > max_stress ) || ( max_stress <= 4 ) )                {
      vowel_stress[max_stress_posn] = (char)tonic                       ;
    }                                                                   ;
    max_stress = tonic                                                  ;
  }                                                                     ;
  /* produce output phoneme string */
  p = phonetic                                                          ;
  v = 1                                                                 ;
  if ( !(control & 1) && phonemes.contains(*p) )                        {
    while ( ( phonemes[*p]->Type == Speak::STRESS )           ||
            ( (*p) == Speak::PcEndWord ) )                    {
      p++                                                               ;
    }                                                                   ;
    if ( ( options.vowelPause & 0x30                     )             &&
         ( phonemes[*p]->Type == Speak::VOWEL ) )              {
      // word starts with a vowel
      if ( ( options.vowelPause & 0x20) && (vowel_stress[1] >= 4))      {
        *output++ = Speak::PcPauseNoLink                      ; // not to be replaced by link
      } else                                                            {
        *output++ = Speak::PcPauseVShort                      ; // break, but no pause
      }                                                                 ;
    }                                                                   ;
  }                                                                     ;
  p          = phonetic                                                 ;
  post_tonic = 0                                                        ;
  while ( ( ( phcode = *p++ ) != 0 ) && ( output < max_output ) )       {
    if (!phonemes.contains(phcode)) continue                            ;
    if ( phonemes[phcode]->Type == Speak::PAUSE )              {
      prevLastStress = 0                                                ;
    } else
    if ( ( ( phonemes[phcode]->Type == Speak::VOWEL         ) &&
          !( phonemes[phcode]->Flags & Speak::NONSYLLABIC ) ) ||
           ( (*p) == Speak::PcSyllabic ) )                    {
      // a vowel, or a consonant followed by a syllabic consonant marker
      v_stress         = vowel_stress[v]                                ;
      prevLastStress   = v_stress                                       ;
      if ( vowel_stress[v-1] >= max_stress ) post_tonic = 1             ;
      if ( v_stress <= 1 )                                              {
        if ( ( v > 1 ) && ( max_stress >= 2 )                          &&
             ( stressflags & S_FINAL_DIM ) && (v == (vowel_count-1) ) ) {
          // option: mark unstressed final syllable as diminished
          v_stress = 0                                                  ;
        } else
        if ( ( stressflags & S_NO_DIM )                                ||
             ( v == 1 )                                                ||
             ( v == (vowel_count-1) )                                 ) {
          // first or last syllable, or option 'don't set diminished stress'
          v_stress = 1                                                  ;
        } else
        if ( ( v                           == (vowel_count-2) )        &&
             ( vowel_stress[vowel_count-1] <= 1               )       ) {
          // penultimate syllable, followed by an unstressed final syllable
          v_stress = 1                                                  ;
        } else                                                          {
         // unstressed syllable within a word
          if ( ( vowel_stress[v-1] < 0   )                             ||
             ( ( stressflags & S_MID_DIM ) == 0 )                     ) {
            v_stress        = 0                                         ; /* change to 0 (diminished stress) */
            vowel_stress[v] = v_stress                                  ;
          }                                                             ;
        }                                                               ;
      }                                                                 ;
      ///////////////////////////////////////////////////////////////////
      if ( ( v_stress == 0 ) || ( v_stress > 1 ) )                      {
        // mark stress of all vowels except 1 (unstressed)
        *output++ = stress_phonemes[v_stress]                           ;
      }                                                                 ;
      if ( vowel_stress[v] > max_stress )                               {
        max_stress = vowel_stress[v]                                    ;
      }                                                                 ;
      ///////////////////////////////////////////////////////////////////
      if ( ( (*p) == Speak::PcLengthen                     ) &&
         ( ( opt_length = options.param[LOPT_IT_LENGTHEN] ) & 1 )    )  {
        // remove lengthen indicator from non-stressed syllables
        int shorten = 0                                                 ;
        if ( opt_length & 0x10 )                                        {
          // only allow lengthen indicator on the highest stress syllable in the word
          if ( v != max_stress_posn ) shorten = 1                       ;
        } else
        if ( v_stress < 4 )                                             {
          // only allow lengthen indicator if stress >= 4.
          shorten = 1                                                   ;
        }                                                               ;
        if ( shorten ) p++                                              ;
      }                                                                 ;
      ///////////////////////////////////////////////////////////////////
      if ( (v_stress >= 4) && (options.param[LOPT_IT_LENGTHEN] == 2))   {
        // LANG=Italian, lengthen penultimate stressed vowels, unless followed by 2 consonants
        if ( ( v == (vowel_count - 2) ) && ( syllable_weight[v] == 0) ) {
          *output++ = phcode                                            ;
          phcode    = Speak::PcLengthen                       ;
        }                                                               ;
      }                                                                 ;
      v++                                                               ;
    }                                                                   ;
    if ( phcode != 1 ) *output++ = phcode                               ;
  }                                                                     ;
  *output++ = 0                                                         ;
}

int N::SpeechTranslator::Transpose(char * text)
{ // transpose cyrillic alphabet (for example) into ascii
  // (single byte) character codes
  // return: number of bytes, bit 6: 1=used compression
  int           c                                                       ;
  int           c2                                                      ;
  int           ix                                                      ;
  int           offset                                                  ;
  int           min                                                     ;
  int           max                                                     ;
  const char  * map                                                     ;
  char        * p = text                                                ;
  char        * p2                                                      ;
  int           all_alpha = 1                                           ;
  int           bits                                                    ;
  int           acc                                                     ;
  int           pairs_start                                             ;
  const short * pairs_list                                              ;
  char          buf[N_WORD_BYTES]                                       ;
  ///////////////////////////////////////////////////////////////////////
  p2          = buf                                                     ;
  offset      = transposeMin - 1                                        ;
  min         = transposeMin                                            ;
  max         = transposeMax                                            ;
  map         = transposeMap                                            ;
  pairs_start = max - min + 2                                           ;
  ///////////////////////////////////////////////////////////////////////
  do                                                                    {
    p += toGrapheme(c,p)                                                ;
    if ( c != 0 )                                                       {
      if ( ( c >= min ) && ( c <= max ) )                               {
        if ( IsNull(map) )                                              {
          *p2++ = c - offset                                            ;
        } else                                                          {
          // get the code from the transpose map
          if ( map[c - min] > 0 )                                       {
            *p2++ = map[c - min]                                        ;
          } else                                                        {
            p2        += toUtf8(c,p2)                                   ;
            all_alpha  = 0                                              ;
          }                                                             ;
        }                                                               ;
      } else                                                            {
        p2        += toUtf8(c,p2)                                       ;
        all_alpha  = 0                                                  ;
      }                                                                 ;
    }                                                                   ;
  } while ( c != 0 )                                                    ;
  ///////////////////////////////////////////////////////////////////////
  *p2 = 0                                                               ;
  if ( all_alpha )                                                      {
    // compress to 6 bits per character
    acc  = 0                                                            ;
    bits = 0                                                            ;
    p    = buf                                                          ;
    p2   = buf                                                          ;
    while ( ( c = *p++ ) != 0 )                                         {
      if ( ( pairs_list = frequentPairs ) != NULL )                     {
        c2 = c + (*p << 8)                                              ;
        for ( ix = 0 ; c2 >= pairs_list[ix] ; ix++ )                    {
          if ( c2 == pairs_list[ix] )                                   {
            // found an encoding for a 2-character pair
            c = ix + pairs_start                                        ; // 2-character codes start after the single letter codes
            p++                                                         ;
            break                                                       ;
          }                                                             ;
        }                                                               ;
      }                                                                 ;
      acc   = ( acc << 6 ) + ( c & 0x3f )                               ;
      bits += 6                                                         ;
      if ( bits >= 8 )                                                  {
        bits -= 8                                                       ;
        *p2++ = (acc >> bits)                                           ;
      }                                                                 ;
    }                                                                   ;
    if ( bits > 0 ) *p2++ = (acc << (8-bits))                           ;
    *p2 = 0                                                             ;
    ix  = p2 - buf                                                      ;
    memcpy ( text , buf , ix )                                          ;
  } else return ( strlen ( text ) )                                     ;
  // bit 6 indicates compressed characters
  return ( ix | 0x40 )                                                  ;
}

void N::SpeechTranslator::SpecialAttribute(char * phs,int dict_flags)
{ // apply after the translation is complete
  int    ix                                                              ;
  int    len                                                             ;
  char * p                                                               ;
  ////////////////////////////////////////////////////////////////////////
  len = strlen(phs)                                                      ;
  if ( options.param[LOPT_ALT] & 2 )                                     {
    for ( ix = 0 ; ix < ( len - 1 ) ; ix++ )                             {
      if ( phs[ix] == Speak::PcStressP )                                 {
        p = &phs[ix+1]                                                   ;
        if ( ( dict_flags & FLAG_ALT2_TRANS ) != 0 )                     {
          if ( (*p) == phonemes.Code((unsigned int)'E') )                {
            *p = phonemes.Code((unsigned int)'e')                        ;
          }                                                              ;
          if ( (*p) == phonemes.Code((unsigned int)'O') )                {
            *p = phonemes.Code((unsigned int)'o')                        ;
          }                                                              ;
        } else                                                           {
          if ( (*p) == phonemes.Code((unsigned int)'e') )                {
            *p = phonemes.Code((unsigned int)'E')                        ;
          }                                                              ;
          if ( (*p) == phonemes.Code((unsigned int)'o') )                {
            *p = phonemes.Code((unsigned int)'O')                        ;
          }                                                              ;
        }                                                                ;
        break                                                            ;
      }                                                                  ;
    }                                                                    ;
  }                                                                      ;
}

void N::SpeechTranslator::Match   (
       SpeechMatch & result       ,
       char        * word[]       ,
       char        * word_start   ,
       int           group_length ,
       char        * rule         ,
       int           word_flags   ,
       int           dict_flags   )
{
  unsigned char rb                        ; // current instuction from rule
  unsigned char letter                    ; // current letter from input word, single byte
  unsigned char last_letter                                                ;
  unsigned char condition_num                                              ;
  int           letter_w                  ; // current letter, wide character
  int           letter_xbytes             ; // number of extra bytes of multibyte character (num bytes - 1)
  int           ix                                                         ;
  int           match_type                ; /* left, right, or consume */
  int           failed                                                     ;
  int           unpron_ignore                                              ;
  int           consumed                  ; /* number of letters consumed from input */
  int           syllable_count                                             ;
  int           vowel                                                      ;
  int           letter_group                                               ;
  int           distance_right                                             ;
  int           distance_left                                              ;
  int           lg_pts                                                     ;
  int           n_bytes                                                    ;
  int           add_points                                                 ;
  int           command                                                    ;
  int           check_atstart                                              ;
  int           total_consumed            ; /* letters consumed for best match */
  char        * pre_ptr                                                    ;
  char        * post_ptr                  ; /* pointer to first character after group */
  char        * rule_start                ; /* start of current match template */
  char        * p                                                          ;
  char        * common_phonemes           ; /* common to a group of entries */
  char        * group_chars                                                ;
  char          word_buf [ N_WORD_BYTES ]                                  ;
  SpeechMatch   match                                                      ;
  //////////////////////////////////////////////////////////////////////////
  group_chars = *word                                                      ;
  if ( IsNull(rule) )                                                      {
    result.points = 0                                                      ;
    (*word)++                                                              ;
    return                                                                 ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  total_consumed  = 0                                                      ;
  common_phonemes = NULL                                                   ;
  match_type      = 0                                                      ;
  result.points   = 0                                                      ;
  result.phonemes = ""                                                     ;
  result.end_type = 0                                                      ;
  result.del_fwd  = NULL                                                   ;
  //////////////////////////////////////////////////////////////////////////
  /* search through dictionary rules                                      */
  while ( rule[0] != RULE_GROUP_END )                                      {
    unpron_ignore  = word_flags & FLAG_UNPRON_TEST                         ;
    match_type     =  0                                                    ;
    consumed       =  0                                                    ;
    letter         =  0                                                    ;
    distance_right = -6                                                    ; /* used to reduce points for matches further away the current letter */
    distance_left  = -2                                                    ;
    check_atstart  =  0                                                    ;
    match.points   =  1                                                    ;
    match.end_type =  0                                                    ;
    match.del_fwd  = NULL                                                  ;
    pre_ptr        = *word                                                 ;
    post_ptr       = *word + group_length                                  ;
    /* work through next rule until end, or until no-match proved */
    rule_start     = rule                                                  ;
    failed         =  0                                                    ;
    while ( !failed )                                                      {
      rb = *rule++                                                         ;
      if ( rb <= RULE_LINENUM )                                            {
        switch ( rb )                                                      {
          case 0                                                           :
            // no phoneme string for this rule, use previous common rule
            if ( common_phonemes != NULL )                                 {
              match.phonemes = common_phonemes                             ;
              while ( ( ( rb  = *match.phonemes++ ) != 0 )                &&
                        ( rb != RULE_PHONEMES     )                      ) {
                if ( rb == RULE_CONDITION ) match.phonemes ++              ; // skip over condition number
                if ( rb == RULE_LINENUM   ) match.phonemes += 2            ; // skip over line number
              }                                                            ;
            } else match.phonemes = ""                                     ;
            rule  --                                                       ; // so we are still pointing at the 0
            failed = 2                                                     ; // matched OK
          break                                                            ;
          case RULE_PRE_ATSTART                                            : // pre rule with implied 'start of word'
            check_atstart = 1                                              ;
            unpron_ignore = 0                                              ;
            match_type    = RULE_PRE                                       ;
          break                                                            ;
          case RULE_PRE                                                    :
            match_type = RULE_PRE                                          ;
            if ( word_flags & FLAG_UNPRON_TEST )                           {
              // checking the start of the word for unpronouncable character sequences, only
              // consider rules which explicitly match the start of a word
              // Note: Those rules now use RULE_PRE_ATSTART
              failed = 1                                                   ;
            }                                                              ;
          break                                                            ;
          case RULE_POST                                                   :
            match_type = RULE_POST                                         ;
          break                                                            ;
          case RULE_PHONEMES                                               :
            match.phonemes = rule                                          ;
            failed         = 2                                             ; // matched OK
          break                                                            ;
          case RULE_PH_COMMON                                              :
            common_phonemes = rule                                         ;
          break                                                            ;
          case RULE_CONDITION                                              :
            /* conditional rule, next byte gives condition number         */
            condition_num = *rule++                                        ;
            if ( condition_num >= 32 )                                     {
              // allow the rule only if the condition number is NOT set
              if ( ( dictCondition & (1L << (condition_num-32))) != 0 )    {
                failed = 1                                                 ;
              }                                                            ;
            } else                                                         {
              // allow the rule only if the condition number is set
              if ( ( dictCondition & (1L << condition_num)) == 0)          {
                failed = 1                                                 ;
              }                                                            ;
            }                                                              ;
            if ( !failed ) match.points++                                  ;
            // add one point for a matched conditional rule
          break                                                            ;
          case RULE_LINENUM                                                :
            rule += 2                                                      ;
          break                                                            ;
        }                                                                  ;
        continue                                                           ;
      }                                                                    ;
      //////////////////////////////////////////////////////////////////////
      add_points = 0                                                       ;
      switch ( match_type )                                                {
        case 0                                                             :
          /* match and consume this letter                                */
          last_letter = letter                                             ;
          letter      = *post_ptr++                                        ;
          if ( ( letter == rb )                                           ||
               ( ( letter == (unsigned char)REPLACED_E ) && (rb=='e') )  ) {
            if ( ( letter & 0xc0 ) != 0x80) add_points = 21                ;
            // don't add point for non-initial UTF-8 bytes
            consumed++                                                     ;
          } else failed = 1                                                ;
        break                                                              ;
        case RULE_POST                                                     :
          /* continue moving fowards                                      */
          distance_right += 6                                              ;
          if ( distance_right > 18 ) distance_right = 19                   ;
          last_letter     = letter                                         ;
          letter_xbytes   = toGrapheme(letter_w,post_ptr) - 1              ;
          letter          = *post_ptr++                                    ;
          switch ( rb )                                                    {
            case RULE_LETTERGP                                             :
              letter_group = *rule++ - 'A'                                 ;
              if (isLetter(letter_w,letter_group))                         {
                lg_pts     = 20                                            ;
                if(letter_group==2) lg_pts = 19                            ; // fewer points for C, general consonant
                add_points = ( lg_pts - distance_right )                   ;
                post_ptr  += letter_xbytes                                 ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_LETTERGP2                                            : // match against a list of utf-8 strings
              letter_group = *rule++ - 'A'                                 ;
              n_bytes = isLetterGroup(post_ptr-1,letter_group,0)           ;
              if ( n_bytes > 0 )                                           {
                add_points = ( 20      - distance_right )                  ;
                post_ptr  += ( n_bytes - 1              )                  ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_NOTVOWEL                                             :
              if ( isLetter(letter_w,0) || ((letter_w == ' ')             &&
                   (word_flags & FLAG_SUFFIX_VOWEL) ) )                    {
                failed = 1                                                 ;
              } else                                                       {
                add_points = ( 20 - distance_right )                       ;
                post_ptr  += letter_xbytes                                 ;
              }                                                            ;
            break                                                          ;
            case RULE_DIGIT                                                :
              if ( IsDigit(letter_w) )                                     {
                add_points = ( 20 - distance_right )                       ;
                post_ptr  += letter_xbytes                                 ;
              } else
              if ( options.toneNumbers )                                   {
                // also match if there is no digit
                add_points = ( 20 - distance_right )                       ;
                post_ptr  --                                               ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_NONALPHA                                             :
              if ( ! iswalpha2(letter_w) )                                 {
                add_points  = ( 21 - distance_right )                      ;
                post_ptr   += letter_xbytes                                ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_DOUBLE                                               :
              if ( letter == last_letter )                                 {
                add_points = ( 21 - distance_right )                       ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_DOLLAR                                               :
              command = *rule++                                            ;
              if ( command == 0x01 )                                       {
                match.end_type = SUFX_UNPRON                               ; // $unpron
              } else
              if ( ( command & 0xf0 ) == 0x10 )                            {
                // $w_alt
                if ( dict_flags                                            &
                     (1 << (BITNUM_FLAG_ALT + (command & 0xf))))           {
                  add_points = 23                                          ;
                } else failed = 1                                          ;
              } else
              if ( ( command & 0xf0 ) == 0x20)                             {
                // $p_alt
                // make a copy of the word up to the post-match characters
                ix = *word - word_start + consumed + group_length + 1      ;
                memcpy(word_buf, word_start-1, ix)                         ;
                word_buf [ ix     ] = ' '                                  ;
                word_buf [ ix + 1 ] = 0                                    ;
//                if (LookupFlags(&word_buf[1]) & (1 << (BITNUM_FLAG_ALT + (command & 0xf))))
//                  add_points = 23;
//                else  failed = 1;
              }                                                            ;
            break                                                          ;
            case '-'                                                       :
              if ( (  letter == '-' )                                     ||
                   ( (letter == ' ') && (word_flags & FLAG_HYPHEN_AFTER))) {
                add_points  = ( 22 - distance_right )                      ; // one point more than match against space
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_SYLLABLE                                             :
            { /* more than specified number of vowel letters to the right */
              char * p           = post_ptr + letter_xbytes                ;
              int    vowel_count = 0                                       ;
              syllable_count     = 1                                       ;
              while ( *rule == RULE_SYLLABLE )                             {
                rule           ++                                          ;
                syllable_count += 1                                        ; /* number of syllables to match */
              }                                                            ;
              vowel = 0                                                    ;
              while ( letter_w != RULE_SPACE )                             {
                if ( ( vowel==0 ) && isLetter(letter_w,LETTERGP_VOWEL2))   {
                  // this is counting vowels which are separated by non-vowel letters
                  vowel_count++                                            ;
                }                                                          ;
                vowel = isLetter(letter_w,LETTERGP_VOWEL2)                 ;
                p    += toGrapheme ( letter_w , p )                        ;
              }                                                            ;
              if ( syllable_count <= vowel_count )                         {
                add_points = ( 18 + syllable_count - distance_right )      ;
              } else failed = 1                                            ;
            }                                                              ;
            break                                                          ;
            case RULE_NOVOWELS                                             :
            {
              char * p = post_ptr + letter_xbytes                          ;
              while ( letter_w != RULE_SPACE )                             {
                if ( isLetter(letter_w,LETTERGP_VOWEL2) )                  {
                  failed = 1                                               ;
                  break                                                    ;
                }                                                          ;
                p += toGrapheme ( letter_w , p )                           ;
              }                                                            ;
              if ( ! failed ) add_points = ( 19 - distance_right )         ;
            }                                                              ;
            break                                                          ;
            case RULE_SKIPCHARS                                            :
            {
              // Used for lang=Tamil, used to match on the next word after an unknown word ending
              // only look until the end of the word (including the end-of-word marker)
              // Jx  means 'skip characters until x', where 'x' may be '_' for 'end of word'
              char * p  = post_ptr + letter_xbytes                         ;
              char * p2 = p                                                ;
              int    rule_w                                                ; // skip characters until this
              toGrapheme(rule_w,rule)                                      ;
              while ( (letter_w != rule_w) && (letter_w != RULE_SPACE))    {
                p2  = p                                                    ;
                p  += toGrapheme(letter_w,p)                               ;
              }                                                            ;
              if ( letter_w == rule_w ) post_ptr = p2                      ;
            }                                                              ;
            break                                                          ;
            case RULE_INC_SCORE                                            :
              add_points = 20                                              ; // force an increase in points
            break                                                          ;
            case RULE_DEL_FWD                                              :
              // find the next 'e' in the word and replace by 'E'
              for ( p = *word + group_length ; p < post_ptr ; p++ )        {
                if ( *p == 'e' )                                           {
                  match.del_fwd = p                                        ;
                  break                                                    ;
                }                                                          ;
              }                                                            ;
            break                                                          ;
            case RULE_ENDING                                               :
            {
              int end_type                                                 ;
              // next 3 bytes are a (non-zero) ending type. 2 bytes of flags + suffix length
              end_type = (  rule [ 0 ] << 16   )                           +
                         (( rule [ 1 ]  & 0x7f ) << 8)                     +
                          ( rule [ 2 ]  & 0x7f )                           ;
              // don't match a suffix rule if there are no previous syllables (needed for lang=tr).
              if ( ( wordVowelCount == 0 )                                &&
                  !( end_type & SUFX_P   )                                &&
                   ( options.param[LOPT_SUFFIX] & 1 ) ) failed = 1 ; else  {
                match.end_type = end_type                                  ;
                rule          += 3                                         ;
              }                                                            ;
            }                                                              ;
            break                                                          ;
            case RULE_NO_SUFFIX                                            :
              if ( word_flags & FLAG_SUFFIX_REMOVED ) failed     = 1       ; // a suffix has been removed
                                                 else add_points = 1       ;
            break                                                          ;
            default                                                        :
              if ( letter == rb )                                          {
                if ( ( letter & 0xc0 ) != 0x80)                            {
                  // not for non-initial UTF-8 bytes
                  add_points = ( 21 - distance_right )                     ;
                }                                                          ;
              } else failed = 1                                            ;
            break                                                          ;
          }                                                                ;
        break                                                              ;
        case RULE_PRE                                                      :
          /* match backwards from start of current group */
          distance_left += 2                                               ;
          if ( distance_left > 18 ) distance_left = 19                     ;
          last_letter   = *pre_ptr                                         ;
          pre_ptr--                                                        ;
          letter_xbytes = toGrapheme ( letter_w , pre_ptr , true ) - 1     ;
          letter        = *pre_ptr                                         ;
          switch ( rb )                                                    {
            case RULE_LETTERGP                                             :
              letter_group = *rule++ - 'A'                                 ;
              if ( isLetter ( letter_w , letter_group ) )                  {
                lg_pts = 20                                                ;
                if ( letter_group==2 ) lg_pts = 19                         ; // fewer points for C, general consonant
                add_points = ( lg_pts - distance_left )                    ;
                pre_ptr   -= letter_xbytes                                 ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_LETTERGP2                                            :
              // match against a list of utf-8 strings
              letter_group = *rule++ - 'A'                                 ;
              n_bytes      = isLetterGroup (pre_ptr,letter_group,1)        ;
              if ( n_bytes >0 )                                            {
                add_points = ( 20      - distance_right )                  ;
                pre_ptr   -= ( n_bytes - 1              )                  ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_NOTVOWEL                                             :
              if ( ! isLetter ( letter_w , 0 ) )                           {
                add_points = ( 20 - distance_left )                        ;
                pre_ptr   -= letter_xbytes                                 ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_DOUBLE                                               :
              if ( letter == last_letter ) add_points = (21-distance_left) ;
                                      else failed     = 1                  ;
            break                                                          ;
            case RULE_DIGIT                                                :
              if ( IsDigit(letter_w) )                                     {
                add_points = ( 21 - distance_left )                        ;
                pre_ptr   -= letter_xbytes                                 ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_NONALPHA                                             :
              if ( ! iswalpha2(letter_w) )                                 {
                add_points = ( 21 - distance_right )                       ;
                pre_ptr   -= letter_xbytes                                 ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_SYLLABLE                                             :
              /* more than specified number of vowels to the left         */
              syllable_count = 1                                           ;
              while ( *rule == RULE_SYLLABLE )                             {
                rule           ++                                          ;
                syllable_count ++                                          ; /* number of syllables to match */
              }                                                            ;
              if ( syllable_count <= wordVowelCount )                      {
                add_points = ( 18 + syllable_count - distance_left )       ;
              } else failed = 1                                            ;
            break                                                          ;
            case RULE_STRESSED                                             :
              if ( wordStressedCount > 0 ) add_points = 19                 ;
                                      else failed     =  1                 ;
            break                                                          ;
            case RULE_NOVOWELS                                             :
            {
              char * p = pre_ptr - letter_xbytes - 1                       ;
              while ( letter_w != RULE_SPACE )                             {
                if ( isLetter(letter_w,LETTERGP_VOWEL2) )                  {
                  failed = 1                                               ;
                  break                                                    ;
                }                                                          ;
                p -= toGrapheme(letter_w,p,true)                           ;
              }                                                            ;
              if ( ! failed ) add_points = 3                               ;
            }                                                              ;
            break                                                          ;
            case RULE_IFVERB                                               :
              if ( expectVerb ) add_points = 1                             ;
                           else failed     = 1                             ;
            break                                                          ;
            case RULE_CAPITAL                                              :
              if ( word_flags & FLAG_FIRST_UPPER ) add_points = 1          ;
                                              else failed     = 1          ;
            break                                                          ;
            case '.'                                                       :
              // dot in pre- section, match on any dot before this point in the word
              for ( p = pre_ptr ; *p != ' ' ; p-- )                        {
                if ( (*p) == '.' )                                         {
                  add_points = 50                                          ;
                  break                                                    ;
                }                                                          ;
              }                                                            ;
              if ( (*p) == ' ') failed = 1                                 ;
            break                                                          ;
            case '-'                                                       :
              if ( ( letter == '-' )                                      ||
                 ( ( letter == ' ' ) && ( word_flags & FLAG_HYPHEN ) ) )   {
                add_points = (22-distance_right)                           ;
                // one point more than match against space
              } else failed = 1                                            ;
            break                                                          ;
            default                                                        :
              if ( letter == rb )                                          {
                if ( letter == RULE_SPACE ) add_points = 4 ; else          {
                  if ( ( letter & 0xc0 ) != 0x80 )                         {
                    // not for non-initial UTF-8 bytes
                    add_points = ( 21 - distance_left )                    ;
                  }                                                        ;
                }                                                          ;
              } else failed = 1                                            ;
            break                                                          ;
        }                                                                  ;
        break                                                              ;
      }                                                                    ;
      if ( failed == 0 ) match.points += add_points                        ;
    }                                                                      ;
    ////////////////////////////////////////////////////////////////////////
    if ( ( failed == 2 ) && ( unpron_ignore == 0 ) )                       {
      // do we also need to check for 'start of word' ?
      if ( ( check_atstart == 0 ) || ( pre_ptr[-1] == ' ' ) )              {
        if ( check_atstart ) result.points += 4                            ;
        /* matched OK, is this better than the last best match ?          */
        if ( match.points >= result.points)                                {
          result         = match                                           ;
          total_consumed = consumed                                        ;
        }                                                                  ;
        if (// ( option_phonemes == 2 )                                     &&
             ( match.points > 0                    )                      &&
             ( ( word_flags & FLAG_NO_TRACE ) == 0 )                     ) {
          // show each rule that matches, and it's points score
          int  pts                                                         ;
          char decoded_phonemes[80]                                        ;
          pts = match.points                                               ;
          if ( group_length > 1 ) pts += 35                                ; // to account for an extra letter matching
          phonemes.toMnemonics(match.phonemes,decoded_phonemes)            ;
//          fprintf(f_trans,"%3d\t%s [%s]\n",pts,DecodeRule(group_chars, group_length, rule_start, word_flags), decoded_phonemes);
        }                                                                  ;
      }                                                                    ;
    }                                                                      ;
    while( *rule++ != 0)                                                   ;
    /* skip phoneme string to reach start of next template                */
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
//  if ( ( option_phonemes == 2 ) && ((word_flags & FLAG_NO_TRACE)==0) )     {
//    if ( group_length <= 1 ) fprintf(f_trans,"\n")                         ;
//  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  /* advance input data pointer                                           */
  total_consumed += group_length                                           ;
  if ( total_consumed == 0 ) total_consumed = 1                            ; /* always advance over 1st letter */
  *word += total_consumed                                                  ;
  if ( result.points == 0 ) result.phonemes = ""                           ;
}

int N::SpeechTranslator::Rules      (
      char         * p_start      ,
      char         * phonemes     ,
      int            ph_size      ,
      char         * end_phonemes ,
      int            word_flags   ,
      unsigned int * dict_flags   )
{ /* Translate a word bounded by space characters
     Append the result to 'phonemes' and any standard prefix/suffix in 'end_phonemes' */
  unsigned char     c                                                        ;
  unsigned char     c2                                                       ;
  unsigned int      c12                                                      ;
  int               wc = 0                                                   ;
  int               wc_bytes                                                 ;
  char            * p2              ; /* copy of p for use in double letter chain match */
  int               found                                                    ;
  int               g               ; /* group chain number                 */
  int               g1              ; /* first group for this letter        */
  int               n                                                        ;
  int               letter                                                   ;
  int               any_alpha   = 0                                          ;
  int               ix                                                       ;
  unsigned int      digit_count = 0                                          ;
  char            * p                                                        ;
  nSpeakAlphabet  * alphabet                                                 ;
  int               dict_flags0 = 0                                          ;
  SpeechMatch       match1                                                   ;
  SpeechMatch       match2                                                   ;
  char              ph_buf[40]                                               ;
  char              word_copy [ N_WORD_BYTES ]                               ;
  static const char str_pause [ 2 ] = { Speak::PcPauseNoLink , 0 } ;
  ////////////////////////////////////////////////////////////////////////////
  if ( IsNull(dataDictRules) ) return 0                                      ;
  if ( NotNull(dict_flags  ) ) dict_flags0 = dict_flags[0]                   ;
  for ( ix = 0 ; ix < ( N_WORD_BYTES-1 ) ; )                                 {
    c               = p_start[ix]                                            ;
    word_copy[ix++] = c                                                      ;
    if ( c == 0 ) break                                                      ;
  }                                                                          ;
  word_copy[ix] = 0                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if (// ( option_phonemes == 2 ) &&
       ( ( word_flags & FLAG_NO_TRACE ) == 0 ) )                             {
    char         wordbuf [ 120 ]                                             ;
    unsigned int ix                                                          ;
    for ( ix = 0                                                             ;
          ( ( c   = p_start[ix]) != ' ' )                                   &&
            ( c  != 0                   )                                   &&
            ( ix  < (sizeof(wordbuf)-1) )                                    ;
          ix++                                                             ) {
      wordbuf[ix] = c                                                        ;
    }                                                                        ;
    wordbuf[ix] = 0                                                          ;
//    if ( word_flags & FLAG_UNPRON_TEST ) fprintf(f_trans,"Unpronouncable? '%s'\n",wordbuf);
//                                    else fprintf(f_trans,"Translate '%s'\n",wordbuf);
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  p                 = p_start                                                ;
  wordVowelCount    = 0                                                      ;
  wordStressedCount = 0                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  if ( NotNull(end_phonemes) ) end_phonemes[0] = 0                           ;
  while ( ( ( c = (*p) ) != ' ' ) && ( c != 0 ) )                            {
    wc_bytes = toGrapheme ( wc , p )                                         ;
    if ( IsAlpha(wc) ) any_alpha++                                           ;
    n = groups2Count [ c ]                                                   ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    if ( IsDigit(wc) && ( ( options.toneNumbers == 0) || !any_alpha))        {
      // lookup the number in *_list not *_rules
      char string[8];
      char buf[40];
      string[0] = '_';
      memcpy(&string[1],p,wc_bytes);
      string[1+wc_bytes] = 0;
      Lookup(tr, string,buf);
      if ( ++digit_count >= 2 )                                              {
        strcat(buf,str_pause);
        digit_count=0;
      }                                                                      ;
      AppendPhonemes(tr,phonemes,ph_size,buf);
      p += wc_bytes;
      continue;
    } else                                                                   {
                  digit_count = 0;
                  found = 0;

		  if(((ix = wc - tr->letter_bits_offset) >= 0) && (ix < 128))
		  {
			  if(tr->groups3[ix] != NULL)
			  {
				  MatchRule(tr, &p, p_start, wc_bytes, tr->groups3[ix], &match1, word_flags, dict_flags0);
				  found = 1;
			  }
		  }

		  if(!found && (n > 0))
		  {
			  /* there are some 2 byte chains for this initial letter */
			  c2 = p[1];
			  c12 = c + (c2 << 8);   /* 2 characters */

			  g1 = tr->groups2_start[c];
			  for(g=g1; g < (g1+n); g++)
			  {
				  if(tr->groups2_name[g] == c12)
				  {
					  found = 1;

					  p2 = p;
					  MatchRule(tr, &p2, p_start, 2, tr->groups2[g], &match2, word_flags, dict_flags0);
					  if(match2.points > 0)
						  match2.points += 35;   /* to acount for 2 letters matching */

					  /* now see whether single letter chain gives a better match ? */
					  MatchRule(tr, &p, p_start, 1, tr->groups1[c], &match1, word_flags, dict_flags0);

					  if(match2.points >= match1.points)
					  {
						  // use match from the 2-letter group
						  memcpy(&match1,&match2,sizeof(MatchRecord));
						  p = p2;
					  }
				  }
			  }
		  }

		  if(!found)
		  {
			  /* alphabetic, single letter chain */
			  if(tr->groups1[c] != NULL)
				  MatchRule(tr, &p, p_start, 1, tr->groups1[c], &match1, word_flags, dict_flags0);
			  else
			  {
				  // no group for this letter, use default group
				  MatchRule(tr, &p, p_start, 0, tr->groups1[0], &match1, word_flags, dict_flags0);

				  if((match1.points == 0) && ((option_sayas & 0x10) == 0))
				  {
					  n = utf8_in(&letter,p-1)-1;

					  if(tr->letter_bits_offset > 0)
					  {
						  // not a Latin alphabet, switch to the default Latin alphabet language
						  if((letter <= 0x241) && iswalpha2(letter))
						  {
							  sprintf(phonemes,"%c%s",phonSWITCH,tr->langopts.ascii_language);
							  return(0);
						  }
					  }
#ifdef deleted
// can't switch to a tone language, because the tone-phoneme numbers are not valid for the original language
					  if((letter >= 0x4e00) && (letter < 0xa000) && (tr->langopts.ideographs != 1))
					  {
						  // Chinese ideogram
						  sprintf(phonemes,"%czh",phonSWITCH);
						  return(0);
					  }
#endif

					  // is it a bracket ?
					  if(letter == 0xe000+'(')
					  {
						  if(pre_pause < tr->langopts.param2[LOPT_BRACKET_PAUSE])
							  pre_pause = tr->langopts.param2[LOPT_BRACKET_PAUSE];  // a bracket, aleady spoken by AnnouncePunctuation()
					  }
					  if(IsBracket(letter))
					  {
						  if(pre_pause < tr->langopts.param[LOPT_BRACKET_PAUSE])
							  pre_pause = tr->langopts.param[LOPT_BRACKET_PAUSE];
					  }

					  // no match, try removing the accent and re-translating the word
					  if((letter >= 0xc0) && (letter < N_REMOVE_ACCENT) && ((ix = remove_accent[letter-0xc0]) != 0))
					  {
						  // within range of the remove_accent table
						  if((p[-2] != ' ') || (p[n] != ' '))
						  {
							  // not the only letter in the word
							  p2 = p-1;
							  p[-1] = ix;
							  while((p[0] = p[n]) != ' ')  p++;
							  while(n-- > 0) *p++ = ' ';  // replacement character must be no longer than original

							  if(tr->langopts.param[LOPT_DIERESES] && (lookupwchar(diereses_list,letter) > 0))
							  {
								  // vowel with dieresis, replace and continue from this point
								  p = p2;
								  continue;
							  }

							  phonemes[0] = 0;  // delete any phonemes which have been produced so far
							  p = p_start;
							  tr->word_vowel_count = 0;
							  tr->word_stressed_count = 0;
							  continue;  // start again at the beginning of the word
						  }
					  }

					  if(((alphabet = AlphabetFromChar(letter)) != NULL)  && (alphabet->offset != tr->letter_bits_offset))
					  {
						  if(tr->langopts.alt_alphabet == alphabet->offset)
						  {
							  sprintf(phonemes,"%c%s",phonSWITCH, WordToString2(tr->langopts.alt_alphabet_lang));
							  return(0);
						  }
						  if(alphabet->flags & AL_WORDS)
						  {
							  // switch to the nominated language for this alphabet
							  sprintf(phonemes,"%c%s",phonSWITCH, WordToString2(alphabet->language));
							  return(0);
						  }
					  }
				  }
			  }

			  if(match1.points == 0)
			  {
				  if((wc >= 0x300) && (wc <= 0x36f))
				  {
					  // combining accent inside a word, ignore
				  }
				  else if(IsAlpha(wc))
				  {
					  if((any_alpha > 1) || (p[wc_bytes-1] > ' '))
					  {
						  // an unrecognised character in a word, abort and then spell the word
						  phonemes[0] = 0;
						  if(dict_flags != NULL)
							  dict_flags[0] |= FLAG_SPELLWORD;
						  break;
					  }
				  }
				  else
				  {
					  LookupLetter(tr, wc, -1, ph_buf, 0);
					  if(ph_buf[0])
					  {
						  match1.phonemes = ph_buf;
						  match1.points = 1;
					  }
				  }
				  p += (wc_bytes-1);
			  }
			  else
			  {
				  tr->phonemes_repeat_count = 0;
			  }
		  }
	  }

	  if(match1.phonemes == NULL)
		  match1.phonemes = "";

	  if(match1.points > 0)
	  {
		  if(word_flags & FLAG_UNPRON_TEST)
			  return(match1.end_type | 1);

#ifdef deleted
// ?? allow $unpr while translating rules, not just on initial FLAG_UNPRON_TEST
		  if((match1.end_type & SUFX_UNPRON) && !(word_flags & FLAG_SUFFIX_REMOVED))
			  return(match1.end_type);
#endif

		  if((match1.phonemes[0] == phonSWITCH) && ((word_flags & FLAG_DONT_SWITCH_TRANSLATOR)==0))
		  {
			  // an instruction to switch language, return immediately so we can re-translate
			  strcpy(phonemes,match1.phonemes);
			  return(0);
		  }

		  match1.end_type &= ~SUFX_UNPRON;

		  if((match1.end_type != 0) && (end_phonemes != NULL))
		  {
			  /* a standard ending has been found, re-translate the word without it */
			  if((match1.end_type & SUFX_P) && (word_flags & FLAG_NO_PREFIX))
			  {
				  // ignore the match on a prefix
			  }
			  else
			  {
				  if((match1.end_type & SUFX_P) && ((match1.end_type & 0x7f) == 0))
				  {
					  // no prefix length specified
					  match1.end_type |= p - p_start;
				  }
				  strcpy(end_phonemes,match1.phonemes);
				  memcpy(p_start,word_copy,strlen(word_copy));
				  return(match1.end_type);
			  }
		  }
		  if(match1.del_fwd != NULL)
			  *match1.del_fwd = REPLACED_E;
		  AppendPhonemes(tr,phonemes,ph_size,match1.phonemes);
	  }
#endif
  }                                                                          ;
  memcpy ( p_start , word_copy , N_WORD_BYTES )                              ;
  return 0                                                                   ;
}

const char * N::SpeechTranslator :: Entry     (
               const char         * word      ,
               const char         * word2     ,
               char               * phonetic  ,
               unsigned int       * flags     ,
               int                  end_flags ,
               QList<SpeechWords> & wtab      )
{
  char        * p                                                        ;
  char        * next                                                     ;
  int           hash                                                     ;
  int           phoneme_len                                              ;
  int           wlen                                                     ;
  unsigned char flag                                                     ;
  unsigned int  dictionary_flags                                         ;
  unsigned int  dictionary_flags2                                        ;
  int           condition_failed = 0                                     ;
  int           n_chars                                                  ;
  int           no_phonemes                                              ;
  int           skipwords                                                ;
  int           ix                                                       ;
  int           c                                                        ;
  const char  * word_end                                                 ;
  const char  * word1            = word                                  ;
  int           wflags           = 0                                     ;
  int           lookup_symbol    = flags[1] & FLAG_LOOKUP_SYMBOL         ;
  char          word_buf       [ N_WORD_BYTES + 1 ]                      ;
  char          dict_flags_buf [ 80               ]                      ;
  ////////////////////////////////////////////////////////////////////////
  if ( wtab.count() > 0 ) wflags = wtab[0].flags                         ;
  if ( transposeMin > 0)                                                 {
    wlen = strlen(word)                                                  ;
    if ( wlen > N_WORD_BYTES )                                           {
      memset ( word_buf , 0    , N_WORD_BYTES + 1 )                      ;
      memcpy ( word_buf , word , N_WORD_BYTES     )                      ;
    } else strcpy ( word_buf , word )                                    ;
    wlen = Transpose ( word_buf )                                        ;
    word = word_buf                                                      ;
  } else                                                                 {
    wlen = strlen ( word )                                               ;
  }                                                                      ;
  hash = Hash ( word )                                                   ;
  p    = dictHashTab [ hash ]                                            ;
  if ( IsNull(p) )                                                       {
    if ( NotNull(flags) ) *flags = 0                                     ;
    return NULL                                                          ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  // Find the first entry in the list for this hash value which matches.
  // This corresponds to the last matching entry in the *_list file.
  while ( (*p) != 0)                                                     {
    next = p + p[0]                                                      ;
    if ( ( ( p[1] & 0x7f ) != wlen                      )               ||
           ( memcmp ( word , &p[2] , wlen & 0x3f ) != 0 )              ) {
      // bit 6 of wlen indicates whether the word has been compressed;
      // so we need to match on this also.
      p = next                                                           ;
      continue                                                           ;
    }                                                                    ;
    /* found matching entry. Decode the phonetic string                 */
    word_end          = word2                                            ;
    dictionary_flags  = 0                                                ;
    dictionary_flags2 = 0                                                ;
    no_phonemes       = p[1] & 0x80                                      ;
    p                += ( ( p[1] & 0x3f ) + 2 )                          ;
    //////////////////////////////////////////////////////////////////////
    if ( no_phonemes )                                                   {
      phonetic[0] = 0                                                    ;
      phoneme_len = 0                                                    ;
    } else                                                               {
      strcpy ( phonetic , p )                                            ;
      phoneme_len  = strlen(p)                                           ;
      p           += ( phoneme_len + 1 )                                 ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    while ( p < next )                                                   {
      // examine the flags which follow the phoneme string
      flag = *p++                                                        ;
      if ( flag >= 100 )                                                 {
        // conditional rule
        if ( flag >= 132 )                                               {
          // fail if this condition is set
          if ( ( dictCondition & ( 1 << ( flag - 132 ) ) ) != 0 )        {
            condition_failed = 1                                         ;
          }                                                              ;
        } else                                                           {
          // allow only if this condition is set
          if ( ( dictCondition & ( 1 << ( flag - 100 ) ) ) == 0 )        {
            condition_failed = 1                                         ;
          }                                                              ;
        }                                                                ;
      } else
      if ( flag > 80 )                                                   {
        // flags 81 to 90  match more than one word
        // This comes after the other flags
        n_chars   = next - p                                             ;
        skipwords = flag - 80                                            ;
        // don't use the contraction if any of the words are emphasized
        //  or has an embedded command, such as MARK
        if ( wtab.count() > 0 )                                          {
          for ( ix = 0 ; ix <= skipwords ; ix++ ) if (ix < wtab.count()) {
            if ( wtab[ix].flags & FLAG_EMPHASIZED2)                      {
              condition_failed = 1                                       ;
            }                                                            ;
          }                                                              ;
        }                                                                ;
        //////////////////////////////////////////////////////////////////
        if ( memcmp ( word2 , p , n_chars ) != 0 ) condition_failed = 1  ;
        if ( condition_failed )                                          {
          p = next                                                       ;
          break                                                          ;
        }                                                                ;
        //////////////////////////////////////////////////////////////////
        dictionary_flags |= FLAG_SKIPWORDS                           ;
        dictSkipWords     = skipwords                                ;
        p                 = next                                     ;
        word_end          = word2 + n_chars                          ;
      } else
      if ( flag > 64 )                                                   {
        // stressed syllable information, put in bits 0-3
        dictionary_flags = (dictionary_flags & ~0xf) | (flag & 0xf)      ;
        if ( ( flag & 0xc ) == 0xc )                                     {
          dictionary_flags |= FLAG_STRESS_END                            ;
        }                                                                ;
      } else
      if ( flag >= 32 )                                                  {
        dictionary_flags2 |= ( 1L << ( flag - 32 ) )                     ;
      } else                                                             {
        dictionary_flags  |= ( 1L << flag )                              ;
      }                                                                  ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( condition_failed )                                              {
      condition_failed = 0                                               ;
      continue                                                           ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( ( end_flags & FLAG_SUFX ) == 0 )                                {
      // no suffix has been removed
      if ( dictionary_flags2 & FLAG_STEM ) continue                      ; // this word must have a suffix
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( ( end_flags & SUFX_P                              )            &&
         ( dictionary_flags2 & (FLAG_ONLY | FLAG_ONLY_S) ) ) continue    ; // $only or $onlys, don't match if a prefix has been removed
    //////////////////////////////////////////////////////////////////////
    if ( end_flags & FLAG_SUFX )                                         {
      // a suffix was removed from the word
      if ( dictionary_flags2 & FLAG_ONLY ) continue                      ; // no match if any suffix
      if ( ( dictionary_flags2 & FLAG_ONLY_S    )                       &&
           ( ( end_flags & FLAG_SUFX_S ) == 0 ) )                        {
        // only a 's' suffix allowed, but the suffix wasn't 's'
        continue                                                         ;
      }                                                                  ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_HYPHENATED )                           {
      if ( ! ( wflags & FLAG_HYPHEN_AFTER ) ) continue                   ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_CAPITAL )                              {
      if ( ! ( wflags & FLAG_FIRST_UPPER ) ) continue                    ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_ALLCAPS )                              {
      if ( ! ( wflags & FLAG_ALL_UPPER ) ) continue                      ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags & FLAG_NEEDS_DOT )                             {
      if ( ! ( wflags & FLAG_HAS_DOT ) ) continue                        ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( ( dictionary_flags2 & FLAG_ATEND )                             &&
         ( word_end          < clauseEnd  )                             &&
         ( lookup_symbol    == 0          )                            ) {
      // only use this pronunciation if it's the last word of the clause, or called from Lookup()
      continue                                                           ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( ( dictionary_flags2 & FLAG_ATSTART )                           &&
        !( wtab[0].flags & FLAG_FIRST_WORD  ) )                          {
      // only use this pronunciation if it's the first word of a clause
      continue                                                           ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( ( dictionary_flags2 & FLAG_SENTENCE         )                  &&
        !( clauseTerminator  & CLAUSE_BIT_SENTENCE ) )                   {
     // only if this clause is a sentence , i.e. terminator is {. ? !} not {, : :}
     continue                                                            ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_VERB )                                 {
      // this is a verb-form pronunciation
      if ( expectVerb || ( expectVerbs && ( end_flags & FLAG_SUFX_S ) )) {
        // OK, we are expecting a verb
        if ( ( translatorName  == L('e','n')      )                     &&
             ( prevDictFlags[0] & FLAG_ALT6_TRANS )                     &&
             ( end_flags        & FLAG_SUFX_S     )                    ) {
          // lang=en, don't use verb form after 'to' if the word has 's' suffix
          continue                                                       ;
        }                                                                ;
      } else                                                             {
        /* don't use the 'verb' pronunciation unless we are expecting a verb */
        continue                                                         ;
      }                                                                  ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_PAST )                                 {
      if ( ! expectPast )                                                {
        /* don't use the 'past' pronunciation unless we are expecting past tense */
        continue                                                         ;
      }                                                                  ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_NOUN )                                 {
      if ( ( ! expectNoun ) || ( end_flags & SUFX_V ) )                  {
        /* don't use the 'noun' pronunciation unless we are expecting a noun */
        continue                                                         ;
      }                                                                  ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags2 & FLAG_NATIVE )                               {
//      if ( tr != translator) continue                                    ;
      // don't use if we've switched translators
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( dictionary_flags & FLAG_ALT2_TRANS )                            {
      // language specific
      if ( ( translatorName == L('h','u')          )                    &&
           !(  prevDictFlags[0] & FLAG_ALT_TRANS ) ) continue            ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( flags != NULL )                                                 {
      flags[0] = dictionary_flags | FLAG_FOUND_ATTRIBUTES                ;
      flags[1] = dictionary_flags2                                       ;
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( phoneme_len == 0 )                                              {
//      if ( option_phonemes == 2 )                                        {
//        print_dictionary_flags(flags, dict_flags_buf, sizeof(dict_flags_buf));
//        fprintf(f_trans,"Flags:  %s  %s\n", word1, dict_flags_buf);
//      }                                                                  ;
      return NULL                                                        ;
      // no phoneme translation found here, only flags. So use rules
    }                                                                    ;
    //////////////////////////////////////////////////////////////////////
    if ( flags != NULL )                                                 {
      flags[0] |= FLAG_FOUND                                             ;
      // this flag indicates word was found in dictionary
    }                                                                    ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    if ( option_phonemes == 2 )                                          {
      char ph_decoded[N_WORD_PHONEMES]                                   ;
      int  textmode                                                      ;
      DecodePhonemes(phonetic,ph_decoded)                                ;
      if ( ( dictionary_flags & FLAG_TEXTMODE ) == 0) textmode = 0       ;
                                                 else textmode = 1       ;
      if ( textmode == translator->langopts.textmode )                   {
        // only show this line if the word translates to phonemes, not replacement text
        if ( ( dictionary_flags & FLAG_SKIPWORDS) && (wtab != NULL))     {
          // matched more than one word
          // (check for wtab prevents showing RULE_SPELLING byte when speaking individual letters)
          memcpy(word_buf,word2,word_end-word2);
          word_buf[word_end-word2-1] = 0;
          fprintf(f_trans,"Found: '%s %s\n",word1,word_buf);
        } else                                                           {
          fprintf(f_trans,"Found: '%s",word1)                            ;
        }                                                                ;
        print_dictionary_flags(flags, dict_flags_buf, sizeof(dict_flags_buf));
        fprintf(f_trans,"' [%s]  %s\n", ph_decoded,dict_flags_buf)       ;
      }                                                                  ;
    }                                                                    ;
#endif
    ix = toGrapheme ( c , word )                                         ;
    if ( ( word[ix] == 0 ) && ! IsAlpha ( c ) )                          {
      flags[0] |= FLAG_MAX3                                              ;
    }                                                                    ;
    return word_end                                                      ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  return NULL                                                            ;
}

bool N::SpeechTranslator :: Lookup    (
       char              ** wordptr   ,
       char               * ph_out    ,
       unsigned int       * flags     ,
       int                  end_flags ,
       QList<SpeechWords> & wtab      )
/* Lookup a specified word in the word dictionary.
   Returns phonetic data in 'phonetic' and bits in 'flags'
   end_flags:  indicates if a suffix has been removed
*/
{
  int           length = 0                                                  ;
  const char  * found  = NULL                                               ;
  const char  * word1  = *wordptr                                           ;
  const char  * word2  = *wordptr                                           ;
  unsigned char c                                                           ;
  int           nbytes                                                      ;
  int           len                                                         ;
  char          word[N_WORD_BYTES]                                          ;
  ///////////////////////////////////////////////////////////////////////////
  while ( ( word2[ nbytes =  nBytes(*word2) ] == ' ' )                     &&
          ( word2[ nbytes + 1               ] == '.' )                    ) {
    // look for an abbreviation of the form a.b.c
    // try removing the spaces between the dots and looking for a match
    memcpy ( &word[length] , word2 , nbytes )                               ;
    length            += nbytes                                             ;
    word [ length++ ]  = '.'                                                ;
    word2             += ( nbytes + 3 )                                     ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  if ( length > 0 )                                                         {
    // found an abbreviation containing dots
    nbytes = 0                                                              ;
    while ( ( ( c = word2[nbytes] ) != 0 ) && ( c != ' ' ) )                {
      nbytes++                                                              ;
    }                                                                       ;
    memcpy ( &word[length] , word2 , nbytes )                               ;
    word [ length + nbytes ] = 0                                            ;
    found = Entry(word,word2,ph_out,flags,end_flags,wtab)                   ;
    if ( found )                                                            {
      // set the skip words flag
      flags[0]     |= FLAG_SKIPWORDS                                        ;
      dictSkipWords = length                                                ;
      return true                                                           ;
    }                                                                       ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  for ( length = 0 ; length < ( N_WORD_BYTES - 1 ) ; length++ )             {
    if ( ( ( c = *word1++ ) == 0 ) || ( c == ' ' ) ) break                  ;
    if ( ( c == '.' ) && ( length > 0 ) && ( IsNumber ( word[length-1] )) ) {
      // needed for lang=hu, eg. "december 2.-ig"
      break                                                                 ;
    }                                                                       ;
    word [ length ] = c                                                     ;
  }                                                                         ;
  word[length] = 0                                                          ;
  found        = Entry(word,word1,ph_out,flags,end_flags,wtab)              ;
  ///////////////////////////////////////////////////////////////////////////
  if ( flags[0] & FLAG_MAX3 )                                               {
    if ( strcmp ( ph_out , phonemesRepeat ) == 0 )                          {
      phonemesRepeatCount++                                                 ;
      if ( phonemesRepeatCount > 3 ) ph_out[0] = 0                          ;
    } else                                                                  {
      strncpy(phonemesRepeat,ph_out,20)                                     ;
      phonemesRepeatCount = 1                                               ;
    }                                                                       ;
  } else                                                                    {
    phonemesRepeatCount = 0                                                 ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  if ( ( IsNull(found) ) && ( flags[1] & FLAG_ACCENT ) )                    {
    int letter                                                              ;
    word2 = word                                                            ;
    if ( (*word2) == '_') word2++                                           ;
    len   = toGrapheme ( letter , word2 )                                   ;
//    LookupAccentedLetter(tr,letter, ph_out);
    found = word2 + len                                                     ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  if ( found == 0 )                                                         {
    ph_out[0] = 0                                                           ;
    // try modifications to find a recognised word
    if ( ( end_flags & FLAG_SUFX_E_ADDED ) && ( word[length-1] == 'e' ) )   {
      // try removing an 'e' which has been added by RemoveEnding
      word[length-1] = 0                                                    ;
      found          = Entry(word,word1,ph_out,flags,end_flags,wtab)        ;
    } else
    if ( ( end_flags & SUFX_D ) && ( word[length-1] == word[length-2] ) )   {
      // try removing a double letter
      word[length-1] = 0                                                    ;
      found          = Entry(word,word1,ph_out,flags,end_flags,wtab)        ;
    }                                                                       ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  if ( IsNull(found) )                                                      {
    ph_out[0] = 0                                                           ;
    return false                                                            ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  // if textmode is the default, then words which have phonemes are marked.
  if ( options.textmode )*flags ^= FLAG_TEXTMODE                            ;
  ///////////////////////////////////////////////////////////////////////////
  if (!(*flags & FLAG_TEXTMODE)) return true                                ;
  // the word translates to replacement text, not to phonemes
  ///////////////////////////////////////////////////////////////////////////
  if ( end_flags & FLAG_ALLOW_TEXTMODE )                                    {
    // only use replacement text if this is the original word, not if a prefix or suffix has been removed
    wordReplacement [ 0 ] = 0                                               ;
    wordReplacement [ 1 ] = ' '                                             ;
    // replacement word, preceded by zerochar and space
    sprintf(&wordReplacement[2],"%s ",ph_out)                               ;
    word1    = *wordptr                                                     ;
    *wordptr = &wordReplacement[2]                                          ;
//    if ( option_phonemes == 2 )                                             {
//      len = found - word1                                                   ;
      // include multiple matching words
//      memcpy(word,word1,len)                                                ;
//      word[len] = 0                                                         ;
//      fprintf(f_trans,"Replace: %s  %s\n",word,*wordptr)                    ;
//    }                                                                       ;
  } else                                                                    {
    // check lang=hu  január 21.-ig  (error: suffix repeated ??)
//    flags[0] &= ~FLAG_SKIPWORDS                                             ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  ph_out[0] = 0                                                             ;
  return false                                                              ;
}

bool N::SpeechTranslator::Lookup(const char * word,char * ph_out)
{
  bool               found                                            ;
  unsigned int       flags[  2 ]                                      ;
  int                say_as                                           ;
  char             * word1 = (char *)word                             ;
  char               text [ 80 ]                                      ;
  QList<SpeechWords> Words                                            ;
  /////////////////////////////////////////////////////////////////////
  flags[0] = 0                                                        ;
  flags[1] = FLAG_LOOKUP_SYMBOL                                       ;
  found    = Lookup ( &word1,ph_out,flags,FLAG_ALLOW_TEXTMODE,Words ) ;
  /////////////////////////////////////////////////////////////////////
  if ( ! (flags[0] & FLAG_TEXTMODE) ) return found                    ;
  say_as      = OptionSayAs                                           ;
  OptionSayAs = 0                                                     ;
  // don't speak replacement word as letter names
  text[0] = 0                                                         ;
  strncpy ( &text[1] , word1 , 78 )                                   ;
//  found = TranslateWord(tr, &text[1], 0, NULL, NULL)                  ;
  strcpy(ph_out,wordPhonemes)                                         ;
  OptionSayAs = say_as                                                ;
  /////////////////////////////////////////////////////////////////////
  return found                                                        ;
}

int N::SpeechTranslator::LookupFlags(const char * word)
{
  char               buf   [ 100 ]        ;
  unsigned int       flags [ 2   ]        ;
  char             * word1 = (char *)word ;
  QList<SpeechWords> Words                ;
  flags[0] = flags[1] = 0                 ;
  Lookup ( &word1,buf,flags,0,Words )     ;
  return flags [ 0 ]                      ;
}

void N::SpeechTranslator::WordEmbeddedCmd(UUIDs Cmds)
{ // Process embedded commands for emphasis, sayas, and break
  SUID embedded_cmd                                                ;
  SUID value                                                       ;
  //////////////////////////////////////////////////////////////////
  do                                                               {
    embedded_cmd = Cmds[0]                                         ;
    value        = embedded_cmd >> 8                               ;
    Cmds.takeAt(0)                                                 ;
    switch ( embedded_cmd & 0x1f )                                 {
      case EMBED_Y                                                 :
        OptionSayAs    = value                                     ;
      break                                                        ;
      case EMBED_F                                                 :
        OptionEmphasis = value                                     ;
      break                                                        ;
      case EMBED_B                                                 : // break command
        if ( value == 0 ) PrePause = 0                             ; // break=none
                     else PrePause += value                        ;
      break                                                        ;
    }                                                              ;
  } while ( ( ( embedded_cmd & 0x80 ) == 0 ) && Cmds.count() > 0 ) ;
}  // end of Word_EmbeddedCmd

bool N::SpeechTranslator::GetWord(QString & word,char * p)
{
  int  len  = p[1] & 0x7F        ;
  int  wlen = p[1] & 0x3F        ;
  char w[128]                    ;
  word . clear ( )               ;
  nKickOut ( (*p) == 0 , false ) ;
  if ( wlen != len )             { // compressed
    memcpy(w,&p[2],wlen)         ;
    w[wlen] = 0                  ;
        printf("Compressed\n") ;
  } else                         {
    memcpy(w,&p[2],wlen)         ;
    w[wlen] = 0                  ;
  }                              ;
  word = QString(w)              ;
  return true                    ;
}

bool N::SpeechTranslator::GetPhonemes(char * p,char * phs)
{
  bool noPhoneme = p[1] & 0x80  ;
  if ( noPhoneme )              {
    phs[0] = 0                  ;
    return false                ;
  }                             ;
  p += ( ( p[1] & 0x3f ) + 2 )  ;
  strcpy ( phs , p )            ;
  return ( strlen ( phs ) > 0 ) ;
}

int N::SpeechTranslator::GetFlags(char * p,char * flags)
{
  int  size      =   p[0]              ;
  int  len       = ( p[1] & 0x3F ) + 2 ;
  bool noPhoneme =   p[1] & 0x80       ;
  if ( ! noPhoneme )                   {
    char * s = p + len                 ;
    len ++                             ;
    len += strlen(s)                   ;
  }                                    ;
  size -= len                          ;
  if (size<=0) return 0                ;
  memcpy(flags,&p[len],size)           ;
  return size                          ;
}

void N::SpeechTranslator::listings(void)
{
  int    hash                                             ;
  int    offset                                           ;
  int    len                                              ;
  int    wlen                                             ;
  char   m[512]                                           ;
  char   e[512]                                           ;
  char   f[512]                                           ;
  char * p                                                ;
  char * n                                                ;
  char * d = dataDictList.data()                          ;
  if (dataDictList.size()<=0) return                      ;
  QString X                                               ;
  for ( hash = 0 ; hash < N_HASH_DICT ; hash++ )          {
    p      = dictHashTab[hash]                            ;
    offset = ( p - d )                                    ;
    printf("Hash [%5d] = %d\n",hash,offset)               ;
    while ( (*p) != 0 )                                   {
      n    = p + p[0]                                     ;
      len  = p[1] & 0x3F                                  ;
      wlen = p[1] & 0x7F                                  ;
      if ( //( len == wlen ) &&
           GetWord ( X , p ) )         {
        int fs                                            ;
        GetPhonemes(p,m)                                  ;
        fs = GetFlags(p,f)                                ;
        phonemes.toMnemonics(m,e)                         ;
        N::printf                                         (
          QString("%1 = [ %2 | %3 ], %4").arg(X).arg(QString(m)).arg(QString(e)).arg(fs),true) ;
      }                                                   ;
      p    = n                                            ;
    }                                                     ;
  }                                                       ;
}
