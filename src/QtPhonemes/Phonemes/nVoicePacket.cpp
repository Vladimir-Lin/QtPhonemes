#include <qtphonemes.h>

#define MAX_PITCH_VALUE  101

// embedded command numbers
#define EMBED_P            1   // pitch
#define EMBED_S            2   // speed (used in setlengths)
#define EMBED_A            3   // amplitude/volume
#define EMBED_R            4   // pitch range/expression
#define EMBED_H            5   // echo/reverberation
#define EMBED_T            6   // different tone for announcing punctuation (not used)
#define EMBED_I            7   // sound icon
#define EMBED_S2           8   // speed (used in synthesize)
#define EMBED_Y            9   // say-as commands
#define EMBED_M           10   // mark name
#define EMBED_U           11   // audio uri
#define EMBED_B           12   // break
#define EMBED_F           13   // emphasis
#define EMBED_C           14   // capital letter indication
#define N_EMBEDDED_VALUES 15


static unsigned char default_heights[N::Speak::Peaks] = {130,128,120,116,100,100,128,128,128};  // changed for v.1.47
static unsigned char default_widths [N::Speak::Peaks] = {140,128,128,160,171,171,128,128,128};
//  static unsigned char default_heights[Speak::Peaks] = {128,128,120,120,110,110,128,128,128};  // previous version
//  static unsigned char default_widths [Speak::Peaks] = {128,128,128,160,171,171,128,128,128};
static int breath_widths[N::Speak::Peaks] = {0,200,200,400,400,400,600,600,600};

int tone_points[12] = {600,170, 1200,135, 2000,110, 3000,110, -1,0};
//int tone_points[12] = {250,200,  400,170, 600,170, 1200,135, 2000,110, -1,0};

// limit the rate of change for each formant number
//static int formant_rate_22050[9] = {50, 104, 165, 230, 220, 220, 220, 220, 220};  // values for 22kHz sample rate
//static int formant_rate_22050[9] = {240, 180, 180, 180, 180, 180, 180, 180, 180};  // values for 22kHz sample rate
static int formant_rate_22050[9] = {240, 170, 170, 170, 170, 170, 170, 170, 170};  // values for 22kHz sample rate
int formant_rate[9];         // values adjusted for actual sample rate


N::VoicePacket:: VoicePacket (void)
{
}

N::VoicePacket::~VoicePacket (void)
{
}

void N::VoicePacket::Reset(SpeechFactors & factor,int sampleRate)
{
  // Set voice to the default values
  int pk                                                                    ;
  // default is:  pitch 80,118
  PitchBase               = 0x47000                                         ;
  PitchRange              = 4104                                            ;
  //  default is:  pitch 80,117
//  PitchBase               = 0x47000    ;
//  PitchRange              = 3996       ;
  FormantFactor           = 256                                             ;
  SpeedPercent            = 100                                             ;
  EchoDelay               =   0                                             ;
  EchoAmp                 =   0                                             ;
  Flutter                 =  64                                             ;
  nHarmonicPeaks          =   5                                             ;
  PeakShape               =   0                                             ;
  Voicing                 =  64                                             ;
  ConsonantAmp            =  90        ;  // change from 100 to 90 for v.1.47
  ConsonantAmpv           = 100                                             ;
  SampleRate              = sampleRate                                      ;
  Roughness               =   2                                             ;
  factor.fast_settings[0] = 450                                             ;
  factor.fast_settings[1] = 800                                             ;
  factor.fast_settings[2] = 175                                             ;
  memset(KlattV,0,sizeof(int)*8)                                            ;
//  InitBreath();
  for ( pk = 0 ; pk < Speak::Peaks ; pk++ )                                 {
    Freq        [ pk ] = 256                                                ;
    Height      [ pk ] = default_heights [pk] * 2                           ;
    Width       [ pk ] = default_widths  [pk] * 2                           ;
    Breath      [ pk ] = 0                                                  ;
    Breathw     [ pk ] = breath_widths[pk]            ; // default breath formant woidths
    FreqAdd     [ pk ] = 0                                                  ;
    // adjust formant smoothing depending on sample rate
    FormantRate [ pk ] = ( formant_rate_22050 [ pk ] * 22050 ) / SampleRate ;
  }                                                                         ;
  // This table provides the opportunity for tone control.
  // Adjustment of harmonic amplitudes, steps of 8Hz
  // value of 128 means no change
  memset(ToneAdjust,128,sizeof(char)*Speak::ToneAdjust)           ;
  setToneAdjust(tone_points)                                                ;
  // default values of speed factors
  SpeedF1 = 256                                                             ;
  SpeedF2 = 238                                                             ;
  SpeedF3 = 232                                                             ;
}

void N::VoicePacket::setFormant(QString Formant)
{ // Set parameters for a formant
  QString     F = Formant . simplified (   )                              ;
  QStringList L = F       . split      (' ')                              ;
  int         formant                                                     ;
  int         freq    = 100                                               ;
  int         height  = 100                                               ;
  int         width   = 100                                               ;
  int         freqadd = 0                                                 ;
  /////////////////////////////////////////////////////////////////////////
  if (L.count()<3) return                                                 ;
  formant = L[1].toInt()                                                  ;
  freq    = L[2].toInt()                                                  ;
  if ( L.count() >= 4 ) height  = L[3].toInt()                            ;
  if ( L.count() >= 5 ) width   = L[4].toInt()                            ;
  if ( L.count() >= 6 ) freqadd = L[5].toInt()                            ;
  if ( ( formant < 0 ) || ( formant > 8 ) ) return                        ;
  if ( freq   >= 0 ) Freq   [formant] = (int)(((double)freq  ) * 2.56001) ;
  if ( height >= 0 ) Height [formant] = (int)(((double)height) * 2.56001) ;
  if ( width  >= 0 ) Width  [formant] = (int)(((double)width ) * 2.56001) ;
  FreqAdd [ formant ] = freqadd                                           ;
}

void N::VoicePacket::setPitch(QString pitch)
{
  QString     F      = pitch . simplified (   )       ;
  QStringList L      = F     . split      (' ')       ;
  if (L.count()<3) return                             ;
  int         pitch1 = L[1].toInt()                   ;
  int         pitch2 = L[2].toInt()                   ;
  double      factor = (double)( pitch1 - 82 ) / 82.0 ;
  PitchBase     = ( pitch1 - 9      ) <<  12          ;
  PitchRange    = ( pitch2 - pitch1 ) *  108          ;
  FormantFactor = (int)( ( 1 + factor / 4 ) * 256 )   ;
  // nominal formant shift for a different voice pitch
}

void N::VoicePacket::setToneAdjust(int * tonePts)
{
  int    ix                                              ;
  int    pt                                              ;
  int    y                                               ;
  int    freq1   = 0                                     ;
  int    freq2                                           ;
  int    height1 = tonePts[1]                            ;
  int    height2                                         ;
  double rate                                            ;
  ////////////////////////////////////////////////////////
  for ( pt=0 ; pt < 12 ; pt += 2 )                       {
    if ( tonePts[pt] == -1 )                             {
      tonePts[pt] = Speak::ToneAdjust * 8      ;
      if ( pt > 0 ) tonePts[pt+1] = tonePts[pt-1]        ;
    }                                                    ;
    freq2   = tonePts [ pt     ] / 8                     ; // 8Hz steps
    height2 = tonePts [ pt + 1 ]                         ;
    if ( ( freq2 - freq1 ) > 0 )                         {
      rate = ((double)(height2-height1)) / (freq2-freq1) ;
      for ( ix = freq1 ; ix < freq2 ; ix++ )             {
        y = height1 + (int)( rate * ( ix - freq1 ) )     ;
        if ( y > 255 ) y = 255                           ;
        ToneAdjust [ ix ] = y                            ;
      }                                                  ;
    }                                                    ;
    freq1   = freq2                                      ;
    height1 = height2                                    ;
  }                                                      ;
}

void N::VoicePacket::setPitchFormants(int Values[16])
{
  int ix                                                              ;
  int factor      = 256                                               ;
  int pitch_value = Values[EMBED_P]                                   ;
  /////////////////////////////////////////////////////////////////////
  // adjust formants to give better results for a different voice pitch
  if ( pitch_value > MAX_PITCH_VALUE ) pitch_value = MAX_PITCH_VALUE  ;
  if ( pitch_value > 50 )                                             {
    // only adjust if the pitch is higher than normal
    factor = 256 + ( 25 * ( pitch_value - 50 ) ) / 50                 ;
  }                                                                   ;
  for ( ix = 0 ; ix <= 5 ; ix++ )                                     {
    Freq[ix] = ( Freq2[ix] * factor ) / 256                           ;
  }                                                                   ;
  factor = Values[EMBED_T] * 3                                        ;
  Height[0] = ( Height2[0] * ( 256 - factor * 2 ) ) / 256             ;
  Height[1] = ( Height2[1] * ( 256 - factor     ) ) / 256             ;
}

int N::VoicePacket::ReadTonePoints(char * string,int * tonePts)
{
  int ix                                                               ;
  for ( ix = 0 ; ix < 12 ; ix++ ) tonePts[ix] = -1                     ;
  ix = sscanf(string,"%d %d %d %d %d %d %d %d %d %d"                   ,
         &tonePts[0],&tonePts[1],&tonePts[2],&tonePts[3],&tonePts[4]   ,
         &tonePts[5],&tonePts[6],&tonePts[7],&tonePts[8],&tonePts[9] ) ;
  return ix                                                            ;
}
