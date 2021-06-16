#include <qtphonemes.h>

#define NUMBER_OF_SAMPLES 100

short N::Acoustics::NaturalSamples[NUMBER_OF_SAMPLES] =                        {
   -310 ,  -400 ,   530 ,   356 ,   224 ,   89 ,    23 ,   -10 ,   -58 ,   -16 ,
    461 ,   599 ,   536 ,   701 ,   770 ,  605 ,   497 ,   461 ,   560 ,   404 ,
    110 ,   224 ,   131 ,   104 ,   -97 ,  155 ,   278 ,  -154 , -1165 ,  -598 ,
    737 ,   125 ,  -592 ,    41 ,    11 , -247 ,   -10 ,    65 ,    92 ,    80 ,
   -304 ,    71 ,   167 ,    -1 ,   122 ,  233 ,   161 ,   -43 ,   278 ,   479 ,
    485 ,   407 ,   266 ,   650 ,   134 ,   80 ,   236 ,    68 ,   260 ,   269 ,
    179 ,    53 ,   140 ,   275 ,   293 ,  296 ,   104 ,   257 ,   152 ,   311 ,
    182 ,   263 ,   245 ,   125 ,   314 ,  140 ,    44 ,   203 ,   230 ,  -235 ,
   -286 ,    23 ,   107 ,    92 ,   -91 ,   38 ,   464 ,   443 ,   176 ,    98 ,
   -784 , -2449 , -1891 , -1045 , -1600 ,-1462 , -1384 , -1261 ,  -949 ,  -730
}                                                                              ;

short N::Acoustics::FormantHz   [ 10 ] = {
   280 ,  688 , 1064 , 2806 , 3260       ,
  3700 , 6500 , 7000 , 8000 ,  280     } ;

short N::Acoustics::Bandwidth   [ 10 ] = {
    89 ,  160 ,   70 ,  160 ,  200       ,
   200 ,  500 ,  500 ,  500 ,   89     } ;

short N::Acoustics::ParallelAmp [ 10 ] = {
     0 ,   59 ,   59 ,   59 ,   59       ,
    59 ,   59 ,    0 ,    0 ,    0     } ;

short N::Acoustics::ParallelBW  [ 10 ] = {
    59 ,   59 ,   89 ,  149 ,  200       ,
   200 ,  500 ,    0 ,    0 ,    0     } ;

// scale output from different voicing sources
static int ScaleWavTable [ 4 ] = { 45 , 38 , 45 , 45 } ;

///////////////////////////////////////////////////////////////////////////////

N::Klatt:: Klatt         ( void             )
         : Model         ( Speak::Cascade   )
         , Glottis       ( Speak::Impulsive )
         , SampleRate    ( 22050            )
         , FlutterF0     ( 20               )
         , ScaleWave     ( 38               )
         , SampleFactor  ( 3.0              )
         , OutSL         ( 0                )
         , Skewness      ( 0                )
         , nLast         ( 0                )
         , FlutterCount  ( 0                )
         , SampleCount   ( 0                )
         , NoiseFactor   ( 0.750            )
         , NaturalFactor ( 0.028            )
         , KlattNoise    ( 0.000            )
         , KlattVoice    ( 0.000            )
         , KlattSource   ( 0.000            )
         , GlotLast      ( 0.000            )
         , vWave         ( 0.000            )
         , vLast         ( 0.000            )
{
}

N::Klatt::~Klatt (void)
{
}

/******************************************************************************\
 *                                                                            *
 * Noise spectrum is tilted down by soft low-pass filter having a pole near   *
 * the origin in the z-plane, i.e. output = input + (0.75 * lastoutput)       *
 *                                                                            *
 * Equation : Noise(n+1) = Random + ( Noise(n) * Factor )                     *
 *            where Factor = 0.75 in this case                                *
\******************************************************************************/

#ifdef _MSC_VER
#define getrandom(min,max) ((rand()%(int)(((max)+1)-(min)))+(min))
#else
#define getrandom(min,max) ((rand()%(long)(((max)+1)-(min)))+(min))
#endif

double N::Klatt::Noise(void)
{
  nRand = (int) getrandom(-8191,8191)     ;
  nLast = nRand + ( NoiseFactor * nLast ) ;
  return  nLast                           ;
}

/******************************************************************************\
 *                                                                            *
 * Generate a low pass filtered train of impulses as an approximation of      *
 * a natural excitation waveform. Low-pass filter the differentiated impulse  *
 * with a critically-damped second-order filter, time constant proportional   *
 * to Kopen.                                                                  *
 *                                                                            *
\******************************************************************************/

double N::Klatt::Impulsive(void)
{
  double DoubleT[] = { 0.0 , 13000000.0 , -13000000.0 }           ;
  vWave = ( ( nPer >= 0 ) && ( nPer < 3 ) ) ? DoubleT[nPer] : 0.0 ;
  return Rsn[RGL] . F1 ( vWave )                                  ;
}

/******************************************************************************\
 *                                                                            *
 * vWave is the differentiated glottal flow waveform, there is a weak         *
 * spectral zero around 800 Hz, magic constants a,b reset pitch               *
 * synchronously.                                                             *
 *                                                                            *
 * dPulse = ShapeA - ShapeB                                                   *
 * vWave  = vWave  + dPulse                                                   *
 * Result = vWave  * NaturalFactor                                            *
 *                                                                            *
\******************************************************************************/

double N::Klatt::Natural(void)
{
  if (nPer >= nOpen)               { // Initialize vWave
    vWave = 0.0                    ;
    return  0.0                    ;
  }                                ;
  //////////////////////////////////
  PulseShapeA -= PulseShapeB       ;
  vWave       += PulseShapeA       ;
  return ( vWave * NaturalFactor ) ;
}

/******************************************************************************\
 *                                                                            *
 * Allows the use of a glottal excitation waveform sampled from a real voice. *
 *                                                                            *
\******************************************************************************/

double N::Klatt::Sampled(void)
{
  int    itemp                                        ;
  int    currentValue                                 ;
  int    nextValue                                    ;
  double ftemp                                        ;
  double result                                       ;
  double diffValue                                    ;
  double tempDiff                                     ;
  /////////////////////////////////////////////////////
  nKickOut ( nEqual( T0 , 0 ) , 0 )                   ;
  /////////////////////////////////////////////////////
  ftemp        = nSafeCast ( double , nPer          ) ;
  ftemp        = ftemp / T0                           ;
  ftemp        = ftemp * NumSamples                   ;
  itemp        = nSafeCast ( int    , ftemp         ) ;
  tempDiff     = ftemp - nSafeCast ( double , itemp ) ;
  /////////////////////////////////////////////////////
  currentValue = NaturalSamples [ itemp     ]         ;
  nextValue    = NaturalSamples [ itemp + 1 ]         ;
  /////////////////////////////////////////////////////
  diffValue    = nSafeCast ( double , nextValue     ) ;
  diffValue   -= nSafeCast ( double , currentValue  ) ;
  diffValue   *= tempDiff                             ;
  /////////////////////////////////////////////////////
  result       = NaturalSamples[itemp]                ;
  result      += diffValue                            ;
  result      *= SampleFactor                         ;
  /////////////////////////////////////////////////////
  return result                                       ;
}

/******************************************************************************\
 *                                                                            *
 * Convert formant freqencies and bandwidth into resonator difference         *
 * equation constants.                                                        *
 *                                                                            *
 * r =  exp(-pi bw t)                                                         *
 * c =  -r^2                                                                  *
 * b = r * 2*cos(2 pi f t)                                                    *
 * a = 1.0 - b - c                                                            *
 *                                                                            *
\******************************************************************************/

void N::Klatt::Set(int Frequency,int Bandwidth,Resonator & resonator,bool initialize)
{
  double r                           ;
  double arg                         ;
  if (initialize)                    {
    resonator . p1 = 0               ;
    resonator . p2 = 0               ;
  }                                  ;
  arg         = MinusPiT * Bandwidth ;
  r           = ::exp ( arg )        ;
  resonator.C = - nSquare ( r )      ;
  arg         = TwoPiT * Frequency   ;
  resonator.B = r * cos(arg) * 2.0   ;
  resonator.E1()                     ;
}

/******************************************************************************\
 *                                                                            *
 * Convert formant freqencies and bandwidth into anti-resonator difference    *
 * equation constants.                                                        *
 *                                                                            *
\******************************************************************************/

void N::Klatt::Zero(int Frequency,int Bandwidth,Resonator & resonator,bool initialize)
{
  double r                           ;
  double arg                         ;
  if (initialize)                    {
    resonator . p1 = 0               ;
    resonator . p2 = 0               ;
  }                                  ;
  ////////////////////////////////////
  Frequency   = -Frequency           ;
  ////////////////////////////////////
  arg         = MinusPiT * Bandwidth ;
  r           = ::exp(arg)           ;
  resonator.C = - nSquare ( r )      ;
  arg         = TwoPiT * Frequency   ;
  resonator.B = r * cos(arg) * 2.0   ;
  resonator.E1()                     ;
  ////////////////////////////////////
  resonator.E2()                     ;
  /****************************************************************************\
   *                                                                          *
   * Now convert to antiresonator coefficients (a'=1/a, b'=b/a, c'=c/a)       *
   * If f == 0 then rp->a gets set to 0 which makes a'=1/a set a', b' and c'  *
   * to INF, causing an audible sound spike when triggered (e.g. apiration    *
   * with the nasal register set to f=0, bw=0).                               *
   *                                                                          *
   ****************************************************************************/
}

void N::Klatt::SetBreath(int Frequency,int Bandwidth,bool initialize)
{
  nFullLoop ( i , Speak::Peaks )                    {
    Set ( Frequency , Bandwidth , Breath [ i ] , initialize ) ;
  }                                                           ;
}

void N::Klatt::dRsn(int index)
{
  Rsn[ index ].dA = ( NextRsn[ index ].A - Rsn[ index ].A ) / 64.0 ;
  Rsn[ index ].dB = ( NextRsn[ index ].B - Rsn[ index ].B ) / 64.0 ;
  Rsn[ index ].dC = ( NextRsn[ index ].C - Rsn[ index ].C ) / 64.0 ;
}

double N::Klatt::ChainResonation(double input)
{
  double chain                     ;
  chain = Rsn[Rnz ] . R2 ( input ) ;
  chain = Rsn[Rnpc] . F1 ( chain ) ;
  chain = Rsn[R8c ] . F1 ( chain ) ;
  chain = Rsn[R7c ] . F1 ( chain ) ;
  chain = Rsn[R6c ] . F1 ( chain ) ;
  chain = Rsn[R5c ] . F2 ( chain ) ;
  chain = Rsn[R4c ] . F2 ( chain ) ;
  chain = Rsn[R3c ] . F2 ( chain ) ;
  chain = Rsn[R2c ] . F2 ( chain ) ;
  chain = Rsn[R1c ] . F2 ( chain ) ;
  return chain                     ;
}

/******************************************************************************\
 * This function adds F0 flutter, as specified in:                            *
 *                                                                            *
 * "Analysis, synthesis and perception of voice quality variations among      *
 * female and male talkers" D.H. Klatt and L.C. Klatt                         *
 * JASA 87(2) February 1990.                                                  *
 *                                                                            *
 * Flutter is added by applying a quasi-random element constructed from three *
 * slowly varying sine waves.                                                 *
 *                                                                            *
\******************************************************************************/

void N::Klatt::Flutter(KlattFrame & frame)
{
  double delta_f0,fla,flb,flc,fld,fle                     ;
  /////////////////////////////////////////////////////////
  fla           = nSafeCast ( double , FlutterF0  ) /  50 ;
  flb           = nSafeCast ( double , OriginalF0 ) / 100 ;
#ifdef TurnOnHalfFrequency
  flc           = sin ( 2 * M_PI * 12.7 * FlutterCount )  ;
  fld           = sin ( 2 * M_PI *  7.1 * FlutterCount )  ;
  fle           = sin ( 2 * M_PI *  4.7 * FlutterCount )  ;
#else
  flc           = sin (     M_PI * 12.7 * FlutterCount )  ;
  fld           = sin (     M_PI *  7.1 * FlutterCount )  ;
  fle           = sin (     M_PI *  4.7 * FlutterCount )  ;
#endif
  delta_f0      = fla * flb * ( flc + fld + fle ) * 10.0  ;
  frame.F0hz10 += nSafeCast ( int , delta_f0 )            ;
  FlutterCount ++                                         ;
}

/******************************************************************************\
 *                                                                            *
 * Reset selected parameters pitch-synchronously.                             *
 *                                                                            *
 ******************************************************************************
 *                                                                            *
 * Constant B0 controls shape of glottal pulse as a function of desired       *
 * duration of open phase N0 (Note that N0 is specified in terms of 40,000    *
 * samples/sec of speech)                                                     *
 *                                                                            *
 * Assume voicing waveform V(t) has form: ( k1 * t^2 ) - ( k2 * t^3 )         *
 *                                                                            *
 * If the radiation characterivative, a temporal derivative is folded in, and *
 * we go from continuous time to discrete integers n:                         *
 *   dV/dt = vwave[n]                                                         *
 *         = sum over i=1,2,...,n of { a - (i * b) }                          *
 *         = a n  -  b/2 n^2                                                  *
 *                                                                            *
 * where the  constants a and b control the detailed shape and amplitude of   *
 * the voicing waveform over the open potion of the voicing cycle "nopen".    *
 *                                                                            *
 * Let integral of dV/dt have no net dc flow --> a = (b * nopen) / 3          *
 *                                                                            *
 * Let maximum of dUg(n)/dn be constant --> b = gain / (nopen * nopen)        *
 * meaning as nopen gets bigger, V has bigger peak proportional to n          *
 *                                                                            *
 * Thus, to generate the table below for 40 <= nopen <= 263:                  *
 * B0[nopen - 40] = 1920000 / (nopen * nopen)                                 *
 *                                                                            *
\******************************************************************************/

short PitchRpB0[224] =                                                {
  1200 , 1142 , 1088 , 1038 ,  991 ,  948 ,  907 ,  869 ,  833 ,  799 ,
   768 ,  738 ,  710 ,  683 ,  658 ,  634 ,  612 ,  590 ,  570 ,  551 ,
   533 ,  515 ,  499 ,  483 ,  468 ,  454 ,  440 ,  427 ,  415 ,  403 ,
   391 ,  380 ,  370 ,  360 ,  350 ,  341 ,  332 ,  323 ,  315 ,  307 ,
   300 ,  292 ,  285 ,  278 ,  272 ,  265 ,  259 ,  253 ,  247 ,  242 ,
   237 ,  231 ,  226 ,  221 ,  217 ,  212 ,  208 ,  204 ,  199 ,  195 ,
   192 ,  188 ,  184 ,  180 ,  177 ,  174 ,  170 ,  167 ,  164 ,  161 ,
   158 ,  155 ,  153 ,  150 ,  147 ,  145 ,  142 ,  140 ,  137 ,  135 ,
   133 ,  131 ,  128 ,  126 ,  124 ,  122 ,  120 ,  119 ,  117 ,  115 ,
   113 ,  111 ,  110 ,  108 ,  106 ,  105 ,  103 ,  102 ,  100 ,   99 ,
    97 ,   96 ,   95 ,   93 ,   92 ,   91 ,   90 ,   88 ,   87 ,   86 ,
    85 ,   84 ,   83 ,   82 ,   80 ,   79 ,   78 ,   77 ,   76 ,   75 ,
    75 ,   74 ,   73 ,   72 ,   71 ,   70 ,   69 ,   68 ,   68 ,   67 ,
    66 ,   65 ,   64 ,   64 ,   63 ,   62 ,   61 ,   61 ,   60 ,   59 ,
    59 ,   58 ,   57 ,   57 ,   56 ,   56 ,   55 ,   55 ,   54 ,   54 ,
    53 ,   53 ,   52 ,   52 ,   51 ,   51 ,   50 ,   50 ,   49 ,   49 ,
    48 ,   48 ,   47 ,   47 ,   46 ,   46 ,   45 ,   45 ,   44 ,   44 ,
    43 ,   43 ,   42 ,   42 ,   41 ,   41 ,   41 ,   41 ,   40 ,   40 ,
    39 ,   39 ,   38 ,   38 ,   38 ,   38 ,   37 ,   37 ,   36 ,   36 ,
    36 ,   36 ,   35 ,   35 ,   35 ,   35 ,   34 ,   34 ,   33 ,   33 ,
    33 ,   33 ,   32 ,   32 ,   32 ,   32 ,   31 ,   31 ,   31 ,   31 ,
    30 ,   30 ,   30 ,   30 ,   29 ,   29 ,   29 ,   29 ,   28 ,   28 ,
    28 ,   28 ,   27 ,   27
}                                                                     ;

void N::Klatt::ResetPitch(KlattFrame & frame)
{
  int    temp                                                                 ;
  double temp1                                                                ;
  /////////////////////////////////////////////////////////////////////////////
  if ( frame.F0hz10 > 0 )                                                     {
    /* T0 is 4* the number of samples in one pitch period                    */
    T0       = ( 40 * SampleRate ) / frame.F0hz10                             ;
    AmpVoice = Acoustics::toLinear ( frame.tmpAVdb )                ;
    /* Duration of period before amplitude modulation                        */
    nModul = T0                                                               ;
    if ( frame.tmpAVdb > 0 ) nModul >>= 1                                     ;
    /* Breathiness of voicing waveform                                       */
    AmpBreth = Acoustics::toLinear ( frame.Aturb , 0.1 )            ;
    /* Set open phase of glottal period where  40 <= open phase <= 263       */
    nOpen = ( 4 * frame.Kopen )                                               ;
    if (Glottis==Speak::Impulsive && nOpen>263) nOpen = 263         ;
    if (nOpen >= (T0-1)                                 ) nOpen = T0 - 2      ;
    /* F0 max = 1000 Hz                                                      */
    if (nOpen <  40                                     ) nOpen = 40          ;
    /* Reset a & b, which determine shape of "natural" glottal waveform      */
    PulseShapeB = PitchRpB0   [ nOpen - 40 ]                                  ;
    PulseShapeA = PulseShapeB * nOpen * 0.333                                 ;
    /* Reset width of "impulsive" glottal pulse                              */
    temp  = SampleRate / nOpen                                                ;
    Set ( 0 , temp , Rsn[RGL] )                                               ;
    /* Make gain at F1 about constant                                        */
    temp1            = nOpen * 0.00833                                        ;
    Rsn [ RGL ] . A *= nSquare ( temp1 )                                      ;
    /*************************************************************************
     * Truncate skewness so as not to exceed duration of closed phase of     *
     * glottal period.                                                       *
     *************************************************************************/
    temp  = T0 - nOpen                                                        ;
    if ( frame . Kskew > temp ) frame.Kskew =   temp                          ;
    if ( Skewness >= 0        ) Skewness    =   frame . Kskew                 ;
                           else Skewness    = - frame . Kskew                 ;
    /* Add skewness to closed portion of voicing period                      */
    T0       = T0 + Skewness                                                  ;
    Skewness =    - Skewness                                                  ;
  } else                                                                      {
    /* Default for f0 undefined                                              */
    T0          = 4                                                           ;
    AmpVoice    = 0.0                                                         ;
    nModul      = T0                                                          ;
    AmpBreth    = 0.0                                                         ;
    PulseShapeA = 0.0                                                         ;
    PulseShapeB = 0.0                                                         ;
  }                                                                           ;
  /* Reset these pars pitch synchronously or at update rate if f0=0          */
  if ( NotEqual ( T0 , 4 ) || ( nEqual ( ns , 0 ) ) )                         {
    /* Set one-pole low-pass filter that tilts glottal source                */
    Decay = ( 0.033 * frame.TLTdb )                                           ;
    if ( Decay > 0.0 ) OneMd = 1.0 - Decay                                    ;
                  else OneMd = 1.0                                            ;
  }                                                                           ;
}

/******************************************************************************
 *                                                                            *
 * Converts synthesis parameters to a waveform.                               *
 *                                                                            *
 ******************************************************************************/

int N::Klatt::Waveform(KlattFrame & frame,WaveGenerator & generator)
{
  int    value                                                                ;
  int    z2                                                                   ;
  int    sample                                                               ;
  char   c                                                                    ;
  double temp                                                                 ;
  double outbypas                                                             ;
  double out                                                                  ;
  double frics                                                                ;
  double glotout                                                              ;
  double aspiration                                                           ;
  double parGlotout                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  //  add f0 flutter                                                         //
  /////////////////////////////////////////////////////////////////////////////
  Flutter ( frame )                                                           ;
  /* Main loop, for each output sample of current frame:                     */
  for (ns=0;ns<nSPFr;ns++)                                                    {
    /* Get low-passed random number for aspiration and frication noise       */
    KlattNoise = Noise()                                                      ;
    /*************************************************************************\
     * Amplitude modulate noise (reduce noise amplitude during               *
     * second half of glottal period) if voicing simultaneously present.     *
     *************************************************************************/
    if (nPer > nModul) KlattNoise *= (double) 0.5                             ;
    /* Compute frication noise                                               */
    frics = ( AmpFrica * KlattNoise )                                         ;
    /*************************************************************************\
     * Compute voicing waveform. Run glottal source simulation at 4 times    *
     * normal sample rate to minimize quantization noise in period of female *
     * voice.                                                                *
     *************************************************************************/
    for (int n=0;n<4;n++)                                                     {
      switch (Glottis)                                                        {
        case Speak::Impulsive                                       :
          KlattVoice = Impulsive ( )                                          ;
        break                                                                 ;
        case Speak::Natural                                         :
          KlattVoice = Natural   ( )                                          ;
        break                                                                 ;
        case Speak::Sampled                                         :
          KlattVoice = Sampled   ( )                                          ;
        break                                                                 ;
      }                                                                       ;
      /* Reset period when counter 'nper' reaches T0                         */
      if (nPer >= T0)                                                         {
        nPer = 0                                                              ;
        ResetPitch ( frame )                                                  ;
      }                                                                       ;
      /***********************************************************************\
       * Low-pass filter voicing waveform before downsampling from 4*samrate *
       * to samrate samples/sec.  Resonator f=.09*samrate, bw=.06*samrate    *
       ***********************************************************************/
      KlattVoice = Rsn[RLP].F1(KlattVoice)                                    ;
      /* Increment counter that keeps track of 4*samrate samples per sec     */
      nPer ++                                                                 ;
    }                                                                         ;
    /*************************************************************************\
     * Tilt spectrum of voicing source down by soft low-pass filtering,      *
     * amount of tilt determined by TLTdb                                    *
     *************************************************************************/
    KlattVoice = ( KlattVoice * OneMd ) + ( vLast * Decay )                   ;
    vLast      =   KlattVoice                                                 ;
    /*************************************************************************\
     * Add breathiness during glottal open phase. Amount of breathiness      *
     * determined by parameter Aturb Use nrand rather than noise because     *
     * noise is low-passed.                                                  *
     *************************************************************************/
    if (nPer < nOpen) KlattVoice += ( AmpBreth * nRand )                      ;
    /* Set amplitude of voicing                                              */
    glotout     = ( AmpVoice    * KlattVoice )                                ;
    parGlotout  = ( ParAmpVoice * KlattVoice )                                ;
    /* Compute aspiration amplitude and add to voicing source                */
    aspiration  = ( AmpAspir    * KlattNoise )                                ;
    glotout    += aspiration                                                  ;
    parGlotout += aspiration                                                  ;
    /*************************************************************************\
     * Cascade vocal tract, excited by laryngeal sources.                    *
     * Nasal antiresonator, then formants FNP, F5, F4, F3, F2, F1            *
     *************************************************************************/
    out = 0                                                                   ;
    if (Model != Speak::All ) out = ChainResonation ( glotout )     ;
    /* Excite parallel F1 and FNP by voicing waveform                        */
    /* Source is voicing plus aspiration                                     */
    KlattSource = parGlotout                                                  ;
    /*************************************************************************\
     * Standard parallel vocal tract Formants F6,F5,F4,F3,F2,                *
     * outputs added with alternating sign. Sound source for other parallel  *
     * resonators is frication plus first difference of voicing waveform.    *
     *************************************************************************/
    out         += Rsn[R1p ] . F1 ( KlattSource )                             ;
    out         += Rsn[Rnpp] . F1 ( KlattSource )                             ;
    KlattSource  = frics + parGlotout - GlotLast                              ;
    GlotLast     = parGlotout                                                 ;
    for (int i=R2p;i<=R6p;i++) out = Rsn[i].F1(KlattSource) - out             ;
    ///////////////////////////////////////////////////////////////////////////
    outbypas = ( AmpBypas * KlattSource )                                     ;
    out      = outbypas - out                                                 ;
    out      = Rsn[Rout] . F1 ( out )                                         ;
    temp     = (int)( out * generator.Amplitude * AmpGain0 )                  ;
    ///////////////////////////////////////////////////////////////////////////
    z2 = 0                                                                    ;
    /* questionable calculations                                             */
    if ( generator.MixWaveFileIndex < generator.nMixWaveFile )                {
      if ( generator.MixWaveScale == 0 ) /* a 16 bit sample */                {
        c       = generator . MixWaveFile[ generator . MixWaveFileIndex + 1 ] ;
        sample  = generator . MixWaveFile[ generator . MixWaveFileIndex     ] ;
        sample += ( c * 256 )                                                 ;
        generator.MixWaveFileIndex += 2                                       ;
      } else                                                                  { // a 8 bit sample, scaled
        sample  = generator . MixWaveFile[ generator . MixWaveFileIndex     ] ;
        sample *= generator . MixWaveScale                                    ;
        generator.MixWaveFileIndex ++                                         ;
      }                                                                       ;
      z2    = sample * generator.AmplitudeV / 1024                            ;
      z2    = ( z2 * generator.MixWaveAmp ) / 40                              ;
      temp += z2                                                              ;
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    if ( fadeout > 0 )                                                        {
      fadeout--                                                               ;
      temp *= fadeout                                                         ;
      temp /= 64                                                              ;
    }                                                                         ;
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      // if fadeout is set, fade to zero over 64 samples, to avoid clicks at end of synthesis
    value = (int)temp + ((echo_buf[echo_tail++]*echo_amp) >> 8);
      if(echo_tail >= N_ECHO_BUF)
          echo_tail=0;

      if (value < -32768)
      {
          value = -32768;
      }

      if (value > 32767)
      {
          value =  32767;
      }

      *out_ptr++ = value;
      *out_ptr++ = value >> 8;

      echo_buf[echo_head++] = value;
      if(echo_head >= N_ECHO_BUF)
          echo_head = 0;

      sample_count++;
      if(out_ptr >= out_end)
      {
          return(1);
      }
#endif
  }                                                                           ;
  return 0                                                                    ;
}

/******************************************************************************
 *                                                                            *
 * Use parameters from the input frame to set up resonator coefficients.      *
 *                                                                            *
 *****************************************************************************/

void N::Klatt::Initialize(KlattFrame & frame)
{
  static double amp_par_factor[7] = {0.6, 0.4, 0.15, 0.06, 0.04, 0.022, 0.03} ;
  double        amp_par       [7]                                             ;
  int           Gain0_tmp                                                     ;
  int           FNZ               = (int)KlattFrame::F_NZ                     ;
  /////////////////////////////////////////////////////////////////////////////
  OriginalF0    = frame.F0hz10 / 10                                           ;
  frame.tmpAVdb = frame.AVdb   -  7                                           ;
  if (frame.tmpAVdb < 0) frame.tmpAVdb = 0                                    ;
  /////////////////////////////////////////////////////////////////////////////
  AmpAspir    = Acoustics::toLinear ( frame.ASP   , 0.05 )          ;
  AmpFrica    = Acoustics::toLinear ( frame.AF    , 0.25 )          ;
  ParAmpVoice = Acoustics::toLinear ( frame.AVpdb        )          ;
  AmpBypas    = Acoustics::toLinear ( frame.AB    , 0.05 )          ;
  /////////////////////////////////////////////////////////////////////////////
  for(int i=0;i<=6;i++)                                                       {
    // parallel amplitudes F1 to F6, and parallel nasal pole
    amp_par[i] = Acoustics::toLinear(frame.Ap[i],amp_par_factor[i]) ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  Gain0_tmp = frame.Gain0 - 3                                                 ;
  if (Gain0_tmp <= 0) Gain0_tmp = 57                                          ;
  AmpGain0 = Acoustics::toLinear(Gain0_tmp) / ScaleWave             ;
  /////////////////////////////////////////////////////////////////////////////
  /* Set coefficients of variable cascade resonators                         */
  for (int i=1;i<=9;i++)                                                      {
    // formants 1 to 8, plus nasal pole
    Set    ( frame.Fhz    [i] , frame.Bhz    [i] , Rsn    [i] )               ;
    if (i <= 5)                                                               {
      Set  ( frame.nextFhz[i] , frame.nextBhz[i] , NextRsn[i] )               ;
      dRsn ( i                                                )               ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  /* nasal zero anti-resonator                                               */
  Zero ( frame.Fhz    [FNZ] , frame.Bhz    [FNZ] , Rsn    [Rnz] )             ;
  Zero ( frame.nextFhz[FNZ] , frame.nextBhz[FNZ] , NextRsn[Rnz] )             ;
  dRsn ( FNZ                                                    )             ;
  /////////////////////////////////////////////////////////////////////////////
  /* Set coefficients of parallel resonators, and amplitude of outputs       */
  for (int i=0;i<=6;i++)                                                      {
    Set(frame.Fhz[i],frame.Bphz[i],Rsn[Rparallel+i])                          ;
    Rsn[Rparallel+i].A *= amp_par[i]                                          ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  /* output low-pass filter                                                  */
  Set(0.0,SampleRate/2,Rsn[Rout])                                             ;
}

///////////////////////////////////////////////////////////////////////////////

void N::Klatt::Reset(int control)
{
  if ( nEqual ( control , 2 ) )             { // Full reset
    FLPhz    = ( 950 * SampleRate ) / 10000 ;
    BLPhz    = ( 630 * SampleRate ) / 10000 ;
    MinusPiT = -M_PI / SampleRate           ;
    TwoPiT   = -2.0  * MinusPiT             ;
    Set ( FLPhz , BLPhz , Rsn[RLP] )        ;
  }                                         ;
  ///////////////////////////////////////////
  if ( control > 0 )                        {
    nPer   = 0                              ;
    T0     = 0                              ;
    nOpen  = 0                              ;
    nModul = 0                              ;
    /////////////////////////////////////////
    for (int i = RGL ; i < N_RSN ; i++)     {
      Rsn[i].p1 = 0                         ;
      Rsn[i].p2 = 0                         ;
    }                                       ;
  }                                         ;
  ///////////////////////////////////////////
  for (int i=0;i <= R6p; i++)               {
    Rsn[i].p1 = 0                           ;
    Rsn[i].p2 = 0                           ;
  }                                         ;
}

void N::Klatt::Initialize(void)
{
  //////////////////////////////////////////////////////////
  SampleCount    = 0                                       ;
  Model          = Speak::Cascade                ;
  SampleRate     = 22050                                   ;
  Glottis        = Speak::Impulsive              ;
  ScaleWave      = ScaleWavTable[Glottis]                  ;
  NaturalSamples = Acoustics::NaturalSamples     ;
  NumSamples     = NUMBER_OF_SAMPLES                       ;
  SampleFactor   = 3.0                                     ;
  nSPFr          = ( SampleRate * 10 ) / 1000              ;
  OutSL          = 0                                       ;
  FlutterF0      = 20                                      ;
  //////////////////////////////////////////////////////////
  Reset ( 2 )                                              ;
  //////////////////////////////////////////////////////////
  // set default values for frame parameters
  for (int i=0;i<=KlattFrame::F_NP;i++)                    {
    Frame . Fhz  [i] = Acoustics::FormantHz  [i] ;
    Frame . Bhz  [i] = Acoustics::Bandwidth  [i] ;
    Frame . Ap   [i] = Acoustics::ParallelAmp[i] ;
    Frame . Bphz [i] = Acoustics::ParallelBW [i] ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  Frame.nextBhz[KlattFrame::F_NZ] = Acoustics::Bandwidth[KlattFrame::F_NZ] ;
  //////////////////////////////////////////////////////////
  Frame . F0hz10 = 1000                                    ;
  Frame . AVdb   =   59                                    ;
  Frame . ASP    =    0                                    ;
  Frame . Kopen  =   40                                    ;
  Frame . Aturb  =    0                                    ;
  Frame . TLTdb  =    0                                    ;
  Frame . AF     =   50                                    ;
  Frame . Kskew  =    0                                    ;
  Frame . AB     =    0                                    ;
  Frame . AVpdb  =    0                                    ;
  Frame . Gain0  =   62                                    ; // 60
  //////////////////////////////////////////////////////////
  SetBreath ( 2000 , 200 , true )                          ;
}
