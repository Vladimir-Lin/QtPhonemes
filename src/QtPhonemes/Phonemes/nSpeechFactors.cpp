#include <qtphonemes.h>

// convert from words-per-minute to internal speed factor
// Use this to calibrate speed for wpm 80-350
static unsigned char speed_lookup[] =
{
 255, 255, 255, 255, 255,   // 80
 253, 249, 245, 242, 238,   // 85
 235, 232, 228, 225, 222,   // 90
 218, 216, 213, 210, 207,   // 95
 204, 201, 198, 196, 193,   // 100
 191, 188, 186, 183, 181,   // 105
 179, 176, 174, 172, 169,   // 110
 168, 165, 163, 161, 159,   // 115
 158, 155, 153, 152, 150,   // 120
 148, 146, 145, 143, 141,   // 125
 139, 137, 136, 135, 133,   // 130
 131, 130, 129, 127, 126,   // 135
 124, 123, 122, 120, 119,   // 140
 118, 117, 115, 114, 113,   // 145
 112, 111, 110, 109, 107,   // 150
 106, 105, 104, 103, 102,   // 155
 101, 100,  99,  98,  97,   // 160
  96,  95,  94,  93,  92,   // 165
  91,  90,  89,  89,  88,   // 170
  87,  86,  85,  84,  83,   // 175
  82,  82,  81,  80,  80,   // 180
  79,  78,  77,  76,  76,   // 185
  75,  75,  74,  73,  72,   // 190
  71,  71,  70,  69,  69,   // 195
  68,  67,  67,  66,  66,   // 200
  65,  64,  64,  63,  62,   // 205
  62,  61,  61,  60,  59,   // 210
  59,  58,  58,  57,  57,   // 215
  56,  56,  55,  54,  54,   // 220
  53,  53,  52,  52,  52,   // 225
  51,  50,  50,  49,  49,   // 230
  48,  48,  47,  47,  46,   // 235
  46,  46,  45,  45,  44,   // 240
  44,  44,  43,  43,  42,   // 245
  41,  40,  40,  40,  39,   // 250
  39,  39,  38,  38,  38,   // 255
  37,  37,  37,  36,  36,   // 260
  35,  35,  35,  35,  34,   // 265
  34,  34,  33,  33,  33,   // 270
  32,  32,  31,  31,  31,   // 275
  30,  30,  30,  29,  29,   // 280
  29,  29,  28,  28,  27,   // 285
  27,  27,  27,  26,  26,   // 290
  26,  26,  25,  25,  25,   // 295
  24,  24,  24,  24,  23,   // 300
  23,  23,  23,  22,  22,   // 305
  22,  21,  21,  21,  21,   // 310
  20,  20,  20,  20,  19,   // 315
  19,  19,  18,  18,  17,   // 320
  17,  17,  16,  16,  16,   // 325
  16,  16,  16,  15,  15,   // 330
  15,  15,  14,  14,  14,   // 335
  13,  13,  13,  12,  12,   // 340
  12,  12,  11,  11,  11,   // 345
  11,  10,  10,  10,   9,   // 350
   9,   9,   8,   8,   8,   // 355
};

// speed_factor1 adjustments for speeds 350 to 374: pauses
static unsigned char pause_factor_350[] =
{
  22,22,22,22,22,22,22,21,21,21         , // 350
  21,20,20,19,19,18,17,16,15,15         , // 360
  15,15,15,15,15
}                                       ;                // 370

// wav_factor adjustments for speeds 350 to 450
// Use this to calibrate speed for wpm 350-450
static unsigned char wav_factor_350[] =
{
 120, 121, 120, 119, 119,   // 350
 118, 118, 117, 116, 116,   // 355
 115, 114, 113, 112, 112,   // 360
 111, 111, 110, 109, 108,   // 365
 107, 106, 106, 104, 103,   // 370
 103, 102, 102, 102, 101,   // 375
 101,  99,  98,  98,  97,   // 380
  96,  96,  95,  94,  93,   // 385
  91,  90,  91,  90,  89,   // 390
  88,  86,  85,  86,  85,   // 395
  85,  84,  82,  81,  80,   // 400
  79,  77,  78,  78,  76,   // 405
  77,  75,  75,  74,  73,   // 410
  71,  72,  70,  69,  69,   // 415
  69,  67,  65,  64,  63,   // 420
  63,  63,  61,  61,  59,   // 425
  59,  59,  58,  56,  57,   // 430
  58,  56,  54,  53,  52,   // 435
  52,  53,  52,  52,  50,   // 440
  48,  47,  47,  45,  46,   // 445
  45
}                       ;   // 450

N::SpeechFactors:: SpeechFactors       (void)
                 : pause_factor        (0   )
                 , clause_pause_factor (0   )
                 , min_pause           (0   )
                 , wav_factor          (0   )
                 , lenmod_factor       (0   )
                 , lenmod2_factor      (0   )
                 , min_sample_len      (0   )
                 , loud_consonants     (0   )
{
  memset(fast_settings,0,sizeof(int)*8) ;
}

N::SpeechFactors:: SpeechFactors(const SpeechFactors & factors)
{
  assign ( factors ) ;
}

N::SpeechFactors::~SpeechFactors(void)
{
}

N::SpeechFactors & N::SpeechFactors::operator = (const SpeechFactors & factors)
{
  return assign(factors) ;
}

N::SpeechFactors & N::SpeechFactors::assign(const SpeechFactors & factors)
{
  nMemberCopy ( factors , pause_factor        )                ;
  nMemberCopy ( factors , clause_pause_factor )                ;
  nMemberCopy ( factors , min_pause           )                ;
  nMemberCopy ( factors , wav_factor          )                ;
  nMemberCopy ( factors , lenmod_factor       )                ;
  nMemberCopy ( factors , lenmod2_factor      )                ;
  nMemberCopy ( factors , min_sample_len      )                ;
  nMemberCopy ( factors , loud_consonants     )                ;
  ::memcpy(fast_settings,factors.fast_settings,sizeof(int )*8) ;
  return ME                                                    ;
}

void N::SpeechFactors :: setSpeed (
       VoicePacket  & voice       ,
       int            control     ,
       Synthesizer  & synthesizer )
{
  int    x                                                                    ;
  int    s1                                                                   ;
  int    wpm                                                                  ;
  int    wpm2                                                                 ;
  int    wpm_value                                                            ;
  double sonic                                                                ;
  /////////////////////////////////////////////////////////////////////////////
  loud_consonants =   0                                                       ;
  min_sample_len  = 450                                                       ;
  lenmod_factor   = 110 ; // controls the effect of FRFLAG_LEN_MOD reduce length change
  lenmod2_factor  = 100                                                       ;
  min_pause       =   5                                                       ;
  /////////////////////////////////////////////////////////////////////////////
  wpm = synthesizer.Values [ Synthesizer::EMBED_S ]                           ;
  if ( nEqual ( control , 2 ) )                                               {
    wpm = synthesizer.Values [ Synthesizer::EMBED_S2 ]                        ;
  }                                                                           ;
  wpm_value = wpm                                                             ;
  if (voice.SpeedPercent > 0) wpm = ( wpm * voice.SpeedPercent ) / 100        ;
  /////////////////////////////////////////////////////////////////////////////
  if ( ( control & 2 ) > 0 ) synthesizer . SonicSpeed ( 1024 )                ;
  /////////////////////////////////////////////////////////////////////////////
  if ( ( wpm_value > 450)                                                    ||
       ( ( wpm_value > fast_settings[0] ) && ( wpm > 350 ) ) )                {
    wpm2 = wpm                                                                ;
    wpm  = 175                                                                ;
    // set special eSpeak speed parameters for Sonic use
    // The eSpeak output will be speeded up by at least x2
    x = 73                                                                    ;
    if ( ( control & 1 ) > 0 )                                                {
      synthesizer.Speed1 = ( x * voice.SpeedF1 ) / 256                        ;
      synthesizer.Speed2 = ( x * voice.SpeedF2 ) / 256                        ;
      synthesizer.Speed3 = ( x * voice.SpeedF3 ) / 256                        ;
    }                                                                         ;
    if ( ( control & 2 ) > 0 )                                                {
      sonic = ( (double) wpm2 ) / wpm                                         ;
      synthesizer . SonicSpeed ( (int)( sonic * 1024.0 ) )                    ;
      pause_factor        =  85                                               ;
      clause_pause_factor =  80                                               ;
      min_pause           =  22                                               ;
      min_sample_len      = 450 * 2                                           ;
      wav_factor          = 211                                               ;
      lenmod_factor       = 210                                               ;
      lenmod2_factor      = 170                                               ;
    }                                                                         ;
    return                                                                    ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ( wpm > 450 ) wpm = 450                                                  ;
  if ( wpm > 360 ) loud_consonants = ( ( wpm - 360 ) / 8 )                    ;
  wpm2 = wpm                                                                  ;
  if ( wpm >  359 )  wpm2 = 359                                               ;
  if ( wpm <   80 )  wpm2 =  80                                               ;
  x = speed_lookup [ wpm2 -  80 ]                                             ;
  if ( wpm >= 380 ) x = 7                                                     ;
  if ( wpm >= 400 ) x = 6                                                     ;
  /////////////////////////////////////////////////////////////////////////////
  if ( ( control & 1 ) > 0 )                                                  {
    // set speed factors for different syllable positions within a word
    // these are used in CalcLengths()
    synthesizer.Speed1 = ( x * voice.SpeedF1 ) / 256                          ;
    synthesizer.Speed2 = ( x * voice.SpeedF2 ) / 256                          ;
    synthesizer.Speed3 = ( x * voice.SpeedF3 ) / 256                          ;
    if ( x <= 7 )                                                             {
      synthesizer.Speed1 = x                                                  ;
      synthesizer.Speed2 = x - 1                                              ;
      synthesizer.Speed3 = x - 1                                              ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ( ( control & 2 ) == 0 ) return                                          ;
  // these are used in synthesis file
  if ( wpm > 350 )                                                            {
    lenmod_factor  =  85 - ( wpm - 350 ) / 3                                  ;
    lenmod2_factor =  60 - ( wpm - 350 ) / 8                                  ;
  }  else
  if ( wpm > 250 )                                                            {
    lenmod_factor  = 110 - ( wpm - 250 ) / 4                                  ;
    lenmod2_factor = 110 - ( wpm - 250 ) / 2                                  ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  s1 = ( x * voice.SpeedF1 ) / 256                                            ;
  /////////////////////////////////////////////////////////////////////////////
  if ( wpm >= 170 )                                                           {
    // reduced speed adjustment, used for playing recorded sounds
    wav_factor = 110 + ( 150 * s1 ) / 128                                     ;
  } else                                                                      {
    // = 215 at 170 wpm
    wav_factor = 128 + ( 128 * s1 ) / 130                                     ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ( wpm >= 350 )                                                           {
    wav_factor = wav_factor_350 [ wpm - 350 ]                                 ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ( wpm >= 390 )                                                           {
    min_sample_len = 450 - ( wpm - 400 ) / 2                                  ;
    if ( wpm > 440 ) min_sample_len = 420 - ( wpm - 440 )                     ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  // adjust for different sample rates
  min_sample_len     *= synthesizer.samplerate_native                         ;
  min_sample_len     /= synthesizer.samplerate_base                           ;
  pause_factor        = ( 256 * s1 ) / 115                                    ; // full speed adjustment, used for pause length
  clause_pause_factor = 0                                                     ;
  /////////////////////////////////////////////////////////////////////////////
  if ( wpm > 430 ) pause_factor = 12                                     ; else
  if ( wpm > 400 ) pause_factor = 13                                     ; else
  if ( wpm > 374 ) pause_factor = 14                                     ; else
  if ( wpm > 350 ) pause_factor = pause_factor_350 [ wpm - 350 ]              ;
  if ( clause_pause_factor == 0 )                                             {
    // restrict the reduction of pauses between clauses
    if ( ( clause_pause_factor = pause_factor ) < 16 )                        {
      clause_pause_factor = 16                                                ;
    }                                                                         ;
  }                                                                           ;
}

int N::SpeechFactors :: Length (int pause,int control)
{
  int length                                                           ;
  if ( control == 0 )                                                  {
    if ( pause >= 200 ) length = ( pause * clause_pause_factor ) / 256 ;
                   else length = ( pause * pause_factor        ) / 256 ;
  } else length = ( pause * wav_factor ) / 256                         ;
  if ( length < min_pause) length = min_pause                          ;
  // mS, limit the amount to which pauses can be shortened
  return length                                                        ;
}
