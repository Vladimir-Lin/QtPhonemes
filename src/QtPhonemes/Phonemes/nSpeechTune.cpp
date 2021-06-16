#include <qtphonemes.h>

#define SYL_RISE       1
#define SYL_EMPHASIS   2
#define SYL_END_CLAUSE 4

#define PITCHfall     0
#define PITCHrise     2
#define PITCHfrise    4 // and 3 must be for the variant preceded by 'r'
#define PITCHfrise2   6 // and 5 must be the 'r' variant
#define PITCHrisefall 8

#define N_TONE_HEAD_TABLE    13
#define N_TONE_NUCLEUS_TABLE 13

#define SECONDARY        3
#define PRIMARY          4
#define PRIMARY_STRESSED 6
#define PRIMARY_LAST     7

#define T_EMPH 1

#define OPTION_EMPHASIZE_ALLCAPS     0x100
#define OPTION_EMPHASIZE_PENULTIMATE 0x200

typedef struct                    {
  char          name         [12] ;
  unsigned char flags        [ 4] ;
  signed   char head_extend  [ 8] ;
  unsigned char prehead_start     ;
  unsigned char prehead_end       ;
  unsigned char stressed_env      ;
  unsigned char stressed_drop     ;
  unsigned char secondary_drop    ;
  unsigned char unstressed_shape  ;
  unsigned char onset             ;
  unsigned char head_start        ;
  unsigned char head_end          ;
  unsigned char head_last         ;
  unsigned char head_max_steps    ;
  unsigned char n_head_extend     ;
  signed   char unstr_start  [ 3] ; // for: onset, head, last
  signed   char unstr_end    [ 3] ;
  unsigned char nucleus0_env      ; // pitch envelope, tonic syllable is at end, no tail
  unsigned char nucleus0_max      ;
  unsigned char nucleus0_min      ;
  unsigned char nucleus1_env      ; // when followed by a tail
  unsigned char nucleus1_max      ;
  unsigned char nucleus1_min      ;
  unsigned char tail_start        ;
  unsigned char tail_end          ;
  unsigned char split_nucleus_env ;
  unsigned char split_nucleus_max ;
  unsigned char split_nucleus_min ;
  unsigned char split_tail_start  ;
  unsigned char split_tail_end    ;
  unsigned char split_tune        ;
  unsigned char spare        [ 8] ;
  int           spare2            ; // the struct length should be a multiple of 4 bytes
} TUNE                            ;

typedef struct                 {
  unsigned char pre_start      ;
  unsigned char pre_end        ;
  unsigned char body_start     ;
  unsigned char body_end       ;
  int         * body_drops     ;
  unsigned char body_max_steps ;
  char          body_lower_u   ;
  unsigned char n_overflow     ;
  signed char * overflow       ;
} TONE_HEAD                    ;

typedef struct             {
  unsigned char pitch_env0 ; /* pitch envelope, tonic syllable at end */
  unsigned char tonic_max0 ;
  unsigned char tonic_min0 ;
  unsigned char pitch_env1 ; /* followed by unstressed */
  unsigned char tonic_max1 ;
  unsigned char tonic_min1 ;
  short       * backwards  ;
  unsigned char tail_start ;
  unsigned char tail_end   ;
  unsigned char flags      ;
} TONE_NUCLEUS             ;

/* indexed by stress */
static int min_drop[] = { 6, 7, 9, 9,20,20,20,25} ;

// pitch change during the main part of the clause
static int drops_0[8] = { 9, 9,16,16,16,23,55,32} ;

// overflow table values are 64ths of the body pitch range (between body_start and body_end)
static signed char oflow     [] = { 0, 40, 24,  8,  0} ;
static signed char oflow_emf [] = {10, 52, 32, 20, 10} ;
static signed char oflow_less[] = { 6, 38, 24, 14,  4} ;

static TONE_HEAD tone_head_table[N_TONE_HEAD_TABLE] = {
  { 46, 57, 78, 50, drops_0, 3, 7, 5, oflow     }     , //  0 statement
  { 46, 57, 78, 46, drops_0, 3, 7, 5, oflow     }     , //  1 comma
  { 46, 57, 78, 46, drops_0, 3, 7, 5, oflow     }     , //  2 question
  { 46, 57, 90, 50, drops_0, 3, 9, 5, oflow_emf }     , //  3 exclamation
  { 46, 57, 78, 50, drops_0, 3, 7, 5, oflow     }     , //  4 statement, emphatic
  { 46, 57, 74, 55, drops_0, 4, 7, 5, oflow_less}     , //  5 statement, less intonation
  { 46, 57, 74, 55, drops_0, 4, 7, 5, oflow_less}     , //  6 comma, less intonation
  { 46, 57, 74, 55, drops_0, 4, 7, 5, oflow_less}     , //  7 comma, less intonation, less rise
  { 46, 57, 78, 50, drops_0, 3, 7, 5, oflow     }     , //  8 pitch raises at end of sentence
  { 46, 57, 78, 46, drops_0, 3, 7, 5, oflow     }     , //  9 comma
  { 46, 57, 78, 50, drops_0, 3, 7, 5, oflow     }     , // 10  question
  { 34, 41, 41, 32, drops_0, 3, 7, 5, oflow_less}     , // 11 test
  { 46, 57, 55, 50, drops_0, 3, 7, 5, oflow_less}       // 12 test
};

static TONE_NUCLEUS tone_nucleus_table[N_TONE_NUCLEUS_TABLE] = {
   {PITCHfall,   64, 8,  PITCHfall,   70,18, NULL, 24, 12, 0},     // 0 statement
   {PITCHfrise,  80,18,  PITCHfrise2, 78,22, NULL, 34, 52, 0},     // 1 comma
   {PITCHfrise,  88,22,  PITCHfrise2, 82,22, NULL, 34, 64, 0},     // 2 question
   {PITCHfall,   92, 8,  PITCHfall,   92,80, NULL, 76,  8, T_EMPH},  // 3 exclamation

   {PITCHfall,   86, 4,  PITCHfall,   94,66, NULL, 34, 10, 0},     // 4 statement, emphatic
   {PITCHfall,   62,10,  PITCHfall,   62,20, NULL, 28, 16, 0},     // 5 statement, less intonation
   {PITCHfrise,  68,18,  PITCHfrise2, 68,22, NULL, 30, 44, 0},     // 6 comma, less intonation
   {PITCHfrise2, 64,16,  PITCHfall,   66,32, NULL, 32, 18, 0},     // 7 comma, less intonation, less rise
   {PITCHrise,   68,46,  PITCHfall,   42,32, NULL, 46, 58, 0},     // 8 pitch raises at end of sentence
   {PITCHfrise,  78,24,  PITCHfrise2, 72,22, NULL, 42, 52, 0},     // 9 comma
   {PITCHfrise,  88,34,  PITCHfall,   64,32, NULL, 46, 82, 0},     // 10  question
   {PITCHfall,   56,12,  PITCHfall,   56,20, NULL, 24, 12, 0},     // 11 test
   {PITCHfall,   70,18,  PITCHfall,   70,24, NULL, 32, 20, 0},     // 12 test
};

////////////////////////////////////////////////////////////////////////

int PitchSegment                (
      N::Syllables & syllables  ,
      int            start      ,
      int            end        ,
      TONE_HEAD    * th         ,
      TONE_NUCLEUS * tn         ,
      int            MinStress  ,
      int            Continuing )
/* Calculate pitches until next RESET or tonic syllable, or end.
   Increment pitch if stress is >= min_stress. Used for tonic segment */
{
  int                stress                                             ;
  int                pitch     = 0                                      ;
  int                increment = 0                                      ;
  int                n_primary = 0                                      ;
  int                n_steps   = 0                                      ;
  int                initial                                            ;
  int                overflow  = 0                                      ;
  int                n_overflow                                         ;
  int                pitch_range                                        ;
  int                pitch_range_abs                                    ;
  int              * drops     = th->body_drops                         ;
  signed char      * overflow_tab                                       ;
  static signed char continue_tab[5] = {-26, 32, 20, 8, 0}              ;
  ///////////////////////////////////////////////////////////////////////
  pitch_range     = (th->body_end - th->body_start) << 8                ;
  pitch_range_abs = abs(pitch_range)                                    ;
  ///////////////////////////////////////////////////////////////////////
  if ( Continuing )                                                     {
    initial      = 0                                                    ;
    overflow     = 0                                                    ;
    n_overflow   = 5                                                    ;
    overflow_tab = continue_tab                                         ;
    increment    = pitch_range / ( th->body_max_steps -1 )              ;
  } else                                                                {
    n_overflow   = th->n_overflow                                       ;
    overflow_tab = th->overflow                                         ;
    initial      = 1                                                    ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  while ( start < end )                                                 {
    stress = syllables[start]->Stress                                    ;
    if ( initial || (stress >= MinStress))                              { // a primary stress
      if ( ( initial ) || ( stress == 5 ) )                             {
        initial  = 0                                                    ;
        overflow = 0                                                    ;
        n_steps = n_primary = syllables.Increments(start,end,MinStress) ;
        if ( n_steps > th->body_max_steps )                             {
          n_steps = th->body_max_steps                                  ;
        }                                                               ;
        if ( n_steps > 1 ) increment = pitch_range / (n_steps -1)       ;
                      else increment = 0                                ;
        pitch = th->body_start << 8                                     ;
      } else                                                            {
        if ( n_steps > 0 ) pitch += increment ; else                    {
          pitch = (th->body_end << 8)                                   +
                  (pitch_range_abs * overflow_tab[overflow++]) /64      ;
          if ( overflow >= n_overflow )                                 {
            overflow     = 0                                            ;
            overflow_tab = th->overflow                                 ;
          }                                                             ;
        }                                                               ;
      }                                                                 ;
      n_steps   --                                                      ;
      n_primary --                                                      ;
      if ( ( tn->backwards ) && ( n_primary < 2 ) )                     {
        pitch = tn->backwards[n_primary] << 8                           ;
      }                                                                 ;
    }                                                                   ;
    /////////////////////////////////////////////////////////////////////
    if ( stress >= PRIMARY )                                            {
      syllables[start]->Stress = PRIMARY_STRESSED                        ;
      syllables[start]->setPitch((pitch >> 8),drops[stress])             ;
    } else
    if ( stress >= SECONDARY )                                          {
      syllables[start]->setPitch((pitch >> 8),drops[stress]);
    } else                                                              {
      /* unstressed, drop pitch if preceded by PRIMARY                 */
      if ( (syllables[start-1]->Stress & 0x3f) >= SECONDARY)             {
        syllables[start]->setPitch                                       (
          (pitch >> 8) - th->body_lower_u                               ,
          drops[stress]                                               ) ;
      } else                                                            {
        syllables[start]->setPitch((pitch >> 8),drops[stress])           ;
      }                                                                 ;
    }                                                                   ;
    start++                                                             ;
  }                                                                     ;
  return start                                                          ;
}

////////////////////////////////////////////////////////////////////////

N::SpeechTune:: SpeechTune        (void)
              : prehead_start     (0   )
              , prehead_end       (0   )
              , stressed_env      (0   )
              , stressed_drop     (0   )
              , secondary_drop    (0   )
              , unstressed_shape  (0   )
              , onset             (0   )
              , head_start        (0   )
              , head_end          (0   )
              , head_last         (0   )
              , head_max_steps    (0   )
              , n_head_extend     (0   )
              , nucleus0_env      (0   )
              , nucleus0_max      (0   )
              , nucleus0_min      (0   )
              , nucleus1_env      (0   )
              , nucleus1_max      (0   )
              , nucleus1_min      (0   )
              , tail_start        (0   )
              , tail_end          (0   )
              , split_nucleus_env (0   )
              , split_nucleus_max (0   )
              , split_nucleus_min (0   )
              , split_tail_start  (0   )
              , split_tail_end    (0   )
              , split_tune        (0   )
              , spare2            (0   )
{
  memset(name       ,0,sizeof(char)*12) ;
  memset(flags      ,0,sizeof(char)* 4) ;
  memset(head_extend,0,sizeof(char)* 8) ;
  memset(unstr_start,0,sizeof(char)* 3) ;
  memset(unstr_end  ,0,sizeof(char)* 3) ;
  memset(spare      ,0,sizeof(char)* 8) ;
}

N::SpeechTune:: SpeechTune(const SpeechTune & tune)
{
  assign ( tune ) ;
}

N::SpeechTune::~SpeechTune(void)
{
}

N::SpeechTune & N::SpeechTune::operator = (const SpeechTune & tune)
{
  return assign(tune) ;
}

N::SpeechTune & N::SpeechTune::assign(const SpeechTune & tune)
{
  nMemberCopy ( tune , prehead_start     )               ;
  nMemberCopy ( tune , prehead_end       )               ;
  nMemberCopy ( tune , stressed_env      )               ;
  nMemberCopy ( tune , stressed_drop     )               ;
  nMemberCopy ( tune , secondary_drop    )               ;
  nMemberCopy ( tune , unstressed_shape  )               ;
  nMemberCopy ( tune , onset             )               ;
  nMemberCopy ( tune , head_start        )               ;
  nMemberCopy ( tune , head_end          )               ;
  nMemberCopy ( tune , head_last         )               ;
  nMemberCopy ( tune , head_max_steps    )               ;
  nMemberCopy ( tune , n_head_extend     )               ;
  nMemberCopy ( tune , nucleus0_env      )               ;
  nMemberCopy ( tune , nucleus0_max      )               ;
  nMemberCopy ( tune , nucleus0_min      )               ;
  nMemberCopy ( tune , nucleus1_env      )               ;
  nMemberCopy ( tune , nucleus1_max      )               ;
  nMemberCopy ( tune , nucleus1_min      )               ;
  nMemberCopy ( tune , tail_start        )               ;
  nMemberCopy ( tune , tail_end          )               ;
  nMemberCopy ( tune , split_nucleus_env )               ;
  nMemberCopy ( tune , split_nucleus_max )               ;
  nMemberCopy ( tune , split_nucleus_min )               ;
  nMemberCopy ( tune , split_tail_start  )               ;
  nMemberCopy ( tune , split_tail_end    )               ;
  nMemberCopy ( tune , split_tune        )               ;
  nMemberCopy ( tune , spare2            )               ;
  ::memcpy(name       ,tune.name       ,sizeof(char)*12) ;
  ::memcpy(flags      ,tune.flags      ,sizeof(char)* 4) ;
  ::memcpy(head_extend,tune.head_extend,sizeof(char)* 8) ;
  ::memcpy(unstr_start,tune.unstr_start,sizeof(char)* 3) ;
  ::memcpy(unstr_end  ,tune.unstr_end  ,sizeof(char)* 3) ;
  ::memcpy(spare      ,tune.spare      ,sizeof(char)* 8) ;
  return ME                                              ;
}

int N::SpeechTune::packetSize(void)
{
  return sizeof(TUNE) ;
}

void N::SpeechTune::set(int index,QByteArray & data)
{
  TUNE * tune = NULL                                      ;
  char * d    = (char *)data.data()                       ;
  tune = (TUNE *) ( d + index )                           ;
  #define MemberCopy(o,m) m = o->m
  MemberCopy ( tune , prehead_start     )                 ;
  MemberCopy ( tune , prehead_end       )                 ;
  MemberCopy ( tune , stressed_env      )                 ;
  MemberCopy ( tune , stressed_drop     )                 ;
  MemberCopy ( tune , secondary_drop    )                 ;
  MemberCopy ( tune , unstressed_shape  )                 ;
  MemberCopy ( tune , onset             )                 ;
  MemberCopy ( tune , head_start        )                 ;
  MemberCopy ( tune , head_end          )                 ;
  MemberCopy ( tune , head_last         )                 ;
  MemberCopy ( tune , head_max_steps    )                 ;
  MemberCopy ( tune , n_head_extend     )                 ;
  MemberCopy ( tune , nucleus0_env      )                 ;
  MemberCopy ( tune , nucleus0_max      )                 ;
  MemberCopy ( tune , nucleus0_min      )                 ;
  MemberCopy ( tune , nucleus1_env      )                 ;
  MemberCopy ( tune , nucleus1_max      )                 ;
  MemberCopy ( tune , nucleus1_min      )                 ;
  MemberCopy ( tune , tail_start        )                 ;
  MemberCopy ( tune , tail_end          )                 ;
  MemberCopy ( tune , split_nucleus_env )                 ;
  MemberCopy ( tune , split_nucleus_max )                 ;
  MemberCopy ( tune , split_nucleus_min )                 ;
  MemberCopy ( tune , split_tail_start  )                 ;
  MemberCopy ( tune , split_tail_end    )                 ;
  MemberCopy ( tune , split_tune        )                 ;
  MemberCopy ( tune , spare2            )                 ;
  #undef MemberCopy
  ::memcpy(name       ,tune->name       ,sizeof(char)*12) ;
  ::memcpy(flags      ,tune->flags      ,sizeof(char)* 4) ;
  ::memcpy(head_extend,tune->head_extend,sizeof(char)* 8) ;
  ::memcpy(unstr_start,tune->unstr_start,sizeof(char)* 3) ;
  ::memcpy(unstr_end  ,tune->unstr_end  ,sizeof(char)* 3) ;
  ::memcpy(spare      ,tune->spare      ,sizeof(char)* 8) ;
}

int N::SpeechTune::Packet(QByteArray & data)
{
  TUNE   tune                                            ;
  char * d = (char *)&tune                               ;
  #define MemberCopy(o,m) o.m = m
  MemberCopy ( tune , prehead_start     )                ;
  MemberCopy ( tune , prehead_end       )                ;
  MemberCopy ( tune , stressed_env      )                ;
  MemberCopy ( tune , stressed_drop     )                ;
  MemberCopy ( tune , secondary_drop    )                ;
  MemberCopy ( tune , unstressed_shape  )                ;
  MemberCopy ( tune , onset             )                ;
  MemberCopy ( tune , head_start        )                ;
  MemberCopy ( tune , head_end          )                ;
  MemberCopy ( tune , head_last         )                ;
  MemberCopy ( tune , head_max_steps    )                ;
  MemberCopy ( tune , n_head_extend     )                ;
  MemberCopy ( tune , nucleus0_env      )                ;
  MemberCopy ( tune , nucleus0_max      )                ;
  MemberCopy ( tune , nucleus0_min      )                ;
  MemberCopy ( tune , nucleus1_env      )                ;
  MemberCopy ( tune , nucleus1_max      )                ;
  MemberCopy ( tune , nucleus1_min      )                ;
  MemberCopy ( tune , tail_start        )                ;
  MemberCopy ( tune , tail_end          )                ;
  MemberCopy ( tune , split_nucleus_env )                ;
  MemberCopy ( tune , split_nucleus_max )                ;
  MemberCopy ( tune , split_nucleus_min )                ;
  MemberCopy ( tune , split_tail_start  )                ;
  MemberCopy ( tune , split_tail_end    )                ;
  MemberCopy ( tune , split_tune        )                ;
  MemberCopy ( tune , spare2            )                ;
  #undef MemberCopy
  ::memcpy(tune.name       ,name       ,sizeof(char)*12) ;
  ::memcpy(tune.flags      ,flags      ,sizeof(char)* 4) ;
  ::memcpy(tune.head_extend,head_extend,sizeof(char)* 8) ;
  ::memcpy(tune.unstr_start,unstr_start,sizeof(char)* 3) ;
  ::memcpy(tune.unstr_end  ,unstr_end  ,sizeof(char)* 3) ;
  ::memcpy(tune.spare      ,spare      ,sizeof(char)* 8) ;
  data.append((const char *)d,sizeof(TUNE)) ;
  return sizeof(TUNE)                       ;
}

int N::SpeechTune::setHeadIntonation (
      Syllables & syllables          ,
      int         start              ,
      int         end                )
{
  int   stress                                                           ;
  int   ix                                                               ;
  int   pitch           = 0                                              ;
  int   increment       = 0                                              ;
  int   n_steps         = 0                                              ;
  int   stage           = 0                                              ; // onset, head, last
  int   initial         = 1                                              ;
  int   overflow_ix     = 0                                              ;
  int   pitch_range     = ( head_end - head_start ) << 8                 ;
  int   pitch_range_abs = ::abs ( pitch_range )                          ;
  int * drops           = drops_0                                        ; // this should be controled by tune->head_drops
  int   n_unstressed    = 0                                              ;
  int   unstressed_ix   = 0                                              ;
  int   unstressed_inc                                                   ;
  int   used_onset      = 0                                              ;
  int   head_final      = end                                            ;
  int   secondary       = 2                                              ; // 2
  ////////////////////////////////////////////////////////////////////////
  if ( onset     == 255 ) stage = 1                                      ; // no onset specified
  if ( head_last != 255 )                                                {
    // find the last primary stress in the body
    for ( ix = end - 1 ; ix >= start ; ix-- )                            {
       if ( syllables[ix]->Stress >= 4 )                                  {
         head_final = ix                                                 ;
         break                                                           ;
       }                                                                 ;
    }                                                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  while ( start < end )                                                  {
    stress = syllables[start]->Stress                                     ;
    if ( initial || ( stress >= 4 ) )                                    {
      // a primary stress
      if ( ( initial ) || ( stress == 5 ) )                              {
        initial     = 0                                                  ;
        overflow_ix = 0                                                  ;
        if ( onset == 255 )                                              {
          n_steps = syllables.Increments(start,head_final,4)             ;
          pitch   = head_start << 8                                      ;
        } else                                                           {
          // a pitch has been specified for the onset syllable, don't include it in the pitch incrementing
          n_steps    = syllables.Increments(start+1,head_final,4)        ;
          pitch      = onset << 8                                        ;
          used_onset = 1                                                 ;
        }                                                                ;
        if ( n_steps > head_max_steps) n_steps = head_max_steps          ;
        if ( n_steps > 1 )                                               {
          increment = pitch_range / ( n_steps - 1 )                      ;
        } else increment = 0                                             ;
      } else
      if ( start == head_final )                                         {
        // a pitch has been specified for the last primary stress before the nucleus
        pitch = head_last << 8                                           ;
        stage = 2                                                        ;
      } else                                                             {
        if ( used_onset )                                                {
          stage      = 1                                                 ;
          used_onset = 0                                                 ;
          pitch      = head_start << 8                                   ;
          n_steps++                                                      ;
        } else
        if ( n_steps > 0 ) pitch += increment ; else                     {
          pitch = (head_end << 8)                                        +
                  (pitch_range_abs * head_extend[overflow_ix++]) / 64    ;
          if ( overflow_ix >= n_head_extend ) overflow_ix = 0            ;
        }                                                                ;
      }                                                                  ;
      n_steps--                                                          ;
    }                                                                    ;
    if ( stress >= PRIMARY )                                             {
      n_unstressed  = syllables.Unstressed(start+1,end,secondary)        ;
      unstressed_ix = 0                                                  ;
      syllables[start]->Stress   = PRIMARY_STRESSED                       ;
      syllables[start]->Envelope = stressed_env                           ;
      syllables[start]->setPitch ( ( pitch >> 8 ) , stressed_drop )       ;
    } else
    if ( stress >= secondary )                                           {
      n_unstressed  = syllables.Unstressed(start,end,secondary)          ;
      unstressed_ix = 0                                                  ;
      syllables[start]->setPitch( (pitch >> 8) , drops[stress] )          ;
    } else                                                               {
      if(n_unstressed > 1)                                               {
        unstressed_inc = ( unstr_end[stage] - unstr_start[stage])        /
                         ( n_unstressed     - 1                 )        ;
      } else unstressed_inc = 0                                          ;
      syllables[start]->setPitch                                          (
        (pitch >> 8)                                                     +
        unstr_start[stage]                                               +
        (unstressed_inc * unstressed_ix)                                 ,
        drops[stress]                                                  ) ;
      unstressed_ix++                                                    ;
    }                                                                    ;
    start++                                                              ;
  }                                                                      ;
  return start                                                           ;
}

int N::SpeechTune::Pitches(Syllables & syllables,int start,int end,int toneflags)
// Calculate pitch values for the vowels in this tone group
{
  int ix   = start                                                     ;
  int prev = syllables.Previous                                        ;
  int drop                                                             ;
  /* vowels before the first primary stress                           */
  syllables.setPitchGradient(ix,ix+prev,prehead_start,prehead_end)     ;
  ix += prev                                                           ;
  /* body of tonic segment                                            */
  if ( toneflags & OPTION_EMPHASIZE_PENULTIMATE )                      {
    syllables.TonePosition = syllables.TonePosition2                   ;
    // put tone on the penultimate stressed word
  }                                                                    ;
  ix = setHeadIntonation(syllables,ix,syllables.TonePosition)          ;
  if (syllables.NoTonic) return 0                                      ;
  if (syllables.Tail == 0)                                             {
     syllables.PitchEnvelope = nucleus0_env                            ;
     drop                    = nucleus0_max - nucleus0_min             ;
     syllables[ix++]->setPitch(nucleus0_min,drop)                       ;
  } else                                                               {
     syllables.PitchEnvelope = nucleus1_env                            ;
     drop                    = nucleus1_max - nucleus1_min             ;
     syllables[ix++]->setPitch(nucleus1_min,drop)                       ;
  }                                                                    ;
  syllables[syllables.TonePosition]->Envelope = syllables.PitchEnvelope ;
  if (syllables[syllables.TonePosition]->Stress == PRIMARY)             {
    syllables[syllables.TonePosition]->Stress = PRIMARY_STRESSED        ;
  }                                                                    ;
  /* tail, after the tonic syllable                                   */
  syllables.setPitchGradient(ix,end,tail_start,tail_end)               ;
  return syllables.PitchEnvelope                                       ;
}

int N::SpeechTune::Pitches(Syllables & syllables,int start,int end,int control,int index,int toneflags)
{
  int            ix                                                    ;
  TONE_HEAD    * th                                                    ;
  TONE_NUCLEUS * tn                                                    ;
  int            drop                                                  ;
  int            continuing = 0                                        ;
  //////////////////////////////////////////////////////////////////////
  if ( control == 0 ) return Pitches (syllables,start,end,toneflags)   ;
  if ( start   >  0 ) continuing = 1                                   ;
  th = &tone_head_table    [ index ]                                   ;
  tn = &tone_nucleus_table [ index ]                                   ;
  ix = start                                                           ;
  /* vowels before the first primary stress                           */
  syllables.setPitchGradient(ix           ,ix+syllables.Previous       ,
                             th->pre_start,th->pre_end               ) ;
  ix += syllables.Previous                                             ;
  /* body of tonic segment                                            */
  if ( toneflags & OPTION_EMPHASIZE_PENULTIMATE )                      {
    // put tone on the penultimate stressed word
    syllables.TonePosition = syllables.TonePosition2                   ;
  }                                                                    ;
  ix = PitchSegment                                                    (
         syllables                                                     ,
         ix                                                            ,
         syllables.TonePosition                                        ,
         th                                                            ,
         tn                                                            ,
         PRIMARY                                                       ,
         continuing                                                  ) ;
  if (syllables.NoTonic) return 0                                      ;
  /* tonic syllable                                                   */
  if ( tn->flags & T_EMPH )                                            {
    syllables[ix]->Flags |= SYL_EMPHASIS                                ;
  }                                                                    ;
  if ( syllables.Tail == 0 )                                           {
    syllables.PitchEnvelope = tn->pitch_env0                           ;
    drop                    = tn->tonic_max0 - tn->tonic_min0          ;
    syllables[ix]->setPitch(tn->tonic_min0,drop)                        ;
    ix++                                                               ;
  } else                                                               {
    syllables.PitchEnvelope = tn->pitch_env1                           ;
    drop                    = tn->tonic_max1 - tn->tonic_min1          ;
    syllables[ix]->setPitch(tn->tonic_min1,drop)                        ;
    ix++                                                               ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  syllables[syllables.TonePosition]->Envelope = syllables.PitchEnvelope ;
  if (syllables[syllables.TonePosition]->Stress == PRIMARY)             {
    syllables[syllables.TonePosition]->Stress = PRIMARY_STRESSED        ;
  }                                                                    ;
  /* tail, after the tonic syllable                                   */
  syllables.setPitchGradient(ix,end,tn->tail_start,tn->tail_end)       ;
  return syllables.PitchEnvelope                                       ;
}

void N::SpeechTune::report(void)
{
  printf("Name = %s\n",name) ;
  #define R(item) printf(#item " = %d\n",item)
  R ( prehead_start     ) ;
  R ( prehead_end       ) ;
  R ( stressed_env      ) ;
  R ( stressed_drop     ) ;
  R ( secondary_drop    ) ;
  R ( unstressed_shape  ) ;
  R ( onset             ) ;
  R ( head_start        ) ;
  R ( head_end          ) ;
  R ( head_last         ) ;
  R ( head_max_steps    ) ;
  R ( n_head_extend     ) ;
  R ( nucleus0_env      ) ;
  R ( nucleus0_max      ) ;
  R ( nucleus0_min      ) ;
  R ( nucleus1_env      ) ;
  R ( nucleus1_max      ) ;
  R ( nucleus1_min      ) ;
  R ( tail_start        ) ;
  R ( tail_end          ) ;
  R ( split_nucleus_env ) ;
  R ( split_nucleus_max ) ;
  R ( split_nucleus_min ) ;
  R ( split_tail_start  ) ;
  R ( split_tail_end    ) ;
  R ( split_tune        ) ;
  R ( spare2            ) ;
  #undef  R
  #define R(item,total)         \
    printf(#item " = ")       ; \
    for (int i=0;i<total;i++) { \
      printf("[%d]",item[i])  ; \
    }                         ; \
    printf("\n")
  R(flags      ,4) ;
  R(head_extend,8) ;
  R(unstr_start,3) ;
  R(unstr_end  ,3) ;
  R(spare      ,8) ;
  #undef  R
}
