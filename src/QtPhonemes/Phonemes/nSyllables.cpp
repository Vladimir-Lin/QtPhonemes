#include <qtphonemes.h>

#define SECONDARY        3
#define PRIMARY          4
#define PRIMARY_STRESSED 6
#define PRIMARY_LAST     7

N::Syllables:: Syllables         ( void  )
             : QList<Syllable *> (       )
             , PitchEnvelope     (       )
             , Previous          (    -1 )
             , Body              (     0 )
             , Tail              (     0 )
             , LastPrimary       (    -1 )
             , TonePosition      (    -1 )
             , TonePosition2     (    -1 )
             , NoTonic           ( false )
{
}

N::Syllables::~Syllables (void)
{
}

void N::Syllables::PitchVowels(int Start,int End,int ClauseEnd)
{
  int MaxStress          = 0                                                  ;
  int MaxStressPosition  = 0 ; // last syllable ot the highest stress
  int MaxStressPosition2 = 0 ; // penuntimate syllable of the highest stress
  int Stress                                                                  ;
  /////////////////////////////////////////////////////////////////////////////
  if ( Start   >  End     ) return                                            ;
  if ( count() <= 0       ) return                                            ;
  if ( Start   >= count() ) return                                            ;
  if ( End     >  count() ) End = count()                                     ;
  /////////////////////////////////////////////////////////////////////////////
  Previous    = -1          ; // number of vowels before 1st primary stress
  Body        =  0                                                            ;
  Tail        =  0          ; // number between tonic syllable and next primary
  LastPrimary = -1                                                            ;
  /////////////////////////////////////////////////////////////////////////////
  for (int i = Start ; i < End ; i++ )                                        {
    Stress = at(i) -> Stress                                                  ; // marked stress level
    if ( Stress >= MaxStress)                                                 {
      if ( Stress > MaxStress) MaxStressPosition2 = i                         ;
                          else MaxStressPosition2 = MaxStressPosition         ;
      MaxStressPosition = i                                                   ;
      MaxStress         = Stress                                              ;
    }                                                                         ;
    if ( Stress >= PRIMARY )                                                  {
      if ( Previous < 0 ) Previous = i - Start                                ;
      LastPrimary = i                                                         ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (Previous < 0) Previous = End                                            ;
  Tail          = End - MaxStressPosition - 1                                 ;
  TonePosition  = MaxStressPosition                                           ;
  TonePosition2 = MaxStressPosition2                                          ;
  if (NoTonic)                                                                {
    TonePosition  = End                                                       ;
    TonePosition2 = End ; // next position after the end of the truncated clause
  } else
  if ( LastPrimary >= 0 )                                                     {
    if ( End == ClauseEnd ) ME[ LastPrimary ] -> Stress = PRIMARY_LAST        ;
  } else                                                                      {
    ME[ TonePosition ] -> Stress  = PRIMARY_LAST                              ;
  }                                                                           ;
}

void N::Syllables::PitchVowels(RangeInt Range,int ClauseEnd)
{
  if (Range.isWrong()) return                   ;
  PitchVowels ( Range.L , Range.R , ClauseEnd ) ;
}

int N::Syllables::Increments(int Start,int End,int MinStress)
{
  int Count = 0                           ;
  int Stress                              ;
  if ( Start   >  End     ) return 0      ;
  if ( count() <= 0       ) return 0      ;
  if ( Start   >= count() ) return 0      ;
  if ( End     >  count() ) End = count() ;
  while ( Start < End )                   {
    Stress = at(Start) -> Stress          ;
    Start++                               ;
    if ( Stress >= PRIMARY_LAST) break    ;
    if ( Stress >= MinStress   ) Count++  ;
  }                                       ;
  return Count                            ;
}

int N::Syllables::Increments(RangeInt Range,int MinStress)
{
  if (Range.isWrong()) return 0                       ;
  return Increments ( Range.L , Range.R , MinStress ) ;
}

int N::Syllables::Unstressed(int Start,int End,int Limit)
{
  int i                                   ;
  if ( Start   >  End     ) return 0      ;
  if ( count() <= 0       ) return 0      ;
  if ( Start   >= count() ) return 0      ;
  if ( End     >  count() ) End = count() ;
  for ( i = Start ; i <= End; i++ )       {
    if ( at(i) -> Stress >= Limit) break  ;
  }                                       ;
  return ( i - Start )                    ;
}

int N::Syllables::Unstressed(RangeInt Range,int Limit)
{
  if (Range.isWrong()) return Range . Distance () ;
  return Unstressed ( Range.L , Range.R , Limit ) ;
}

void N::Syllables::setPitchGradient(int Start,int End,int pitchStart,int pitchEnd)
{
  static int syllable_min_drop[] =                    {
    6 ,  7 ,  9 ,  9 , 20 , 20 , 20 , 25            } ;
  /////////////////////////////////////////////////////
  int ix                                              ;
  int stress                                          ;
  int pitch                                           ;
  int increment                                       ;
  int n_increments                                    ;
  int drop                                            ;
  /////////////////////////////////////////////////////
  if ( Start   >  End     ) return                    ;
  if ( count() <= 0       ) return                    ;
  if ( Start   >= count() ) return                    ;
  if ( End     >  count() ) End = count()             ;
  /////////////////////////////////////////////////////
  increment    = ( pitchEnd - pitchStart ) << 8       ;
  n_increments =        End -      Start              ;
  /////////////////////////////////////////////////////
  if ( n_increments <= 0 ) return                     ;
  if ( n_increments >  1 ) increment /= n_increments  ;
  /////////////////////////////////////////////////////
  pitch = pitchStart << 8                             ;
  /////////////////////////////////////////////////////
  for ( ix = Start ; ix < End ; ix++ )                {
    stress = at(ix) -> Stress                         ;
    if ( increment > 0 )                              {
      ME[ix] -> setPitch((pitch >> 8),-(increment >> 8)) ;
      pitch += increment                              ;
    } else                                            {
      drop = - ( increment >> 8 )                     ;
      if ( drop < syllable_min_drop [ stress ] )      {
        drop = syllable_min_drop [ stress ]           ;
      }                                               ;
      pitch += increment                              ;
      if ( drop > 18 ) drop = 18                      ;
      ME[ix] -> setPitch((pitch >> 8),drop)           ;
    }                                                 ;
  }                                                   ;
}

void N::Syllables::setPitchGradient(RangeInt Range,RangeInt Pitch)
{
  if ( Range . isWrong() ) return                            ;
  setPitchGradient ( Range.L , Range.R , Pitch.L , Pitch.R ) ;
}
