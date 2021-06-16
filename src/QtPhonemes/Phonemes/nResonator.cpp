#include <qtphonemes.h>

N::Resonator:: Resonator (void)
             : p1        (0   )
             , p2        (0   )
             , A         (0   )
             , B         (0   )
             , C         (0   )
             , dA        (0   )
             , dB        (0   )
             , dC        (0   )
{
}

N::Resonator::~Resonator(void)
{
}

N::Resonator & N::Resonator::operator = (Resonator & resonator)
{
  nMemberCopy ( resonator , p1 ) ;
  nMemberCopy ( resonator , p2 ) ;
  nMemberCopy ( resonator , A  ) ;
  nMemberCopy ( resonator , B  ) ;
  nMemberCopy ( resonator , C  ) ;
  nMemberCopy ( resonator , dA ) ;
  nMemberCopy ( resonator , dB ) ;
  nMemberCopy ( resonator , dC ) ;
  return (*this)                 ;
}

void N::Resonator::dN(void)
{
  A += dA ;
  B += dB ;
  C += dC ;
}

double N::Resonator::Mix(double input)
{
  register double x = 0 ;
  x += ( A * input )    ;
  x += ( B * p1    )    ;
  x += ( C * p2    )    ;
  return x              ;
}

double N::Resonator::F1(double input)
{
  double x = Mix(input) ;
  p2 = p1               ;
  p1 = x                ;
  return x              ;
}

double N::Resonator::F2(double input)
{
  double x = Mix(input) ;
  p2 = p1               ;
  p1 = x                ;
  dN ()                 ;
  return x              ;
}

double N::Resonator::R1(double input)
{
  double x = Mix(input) ;
  p2 = p1               ;
  p1 = input            ;
  return x              ;
}

double N::Resonator::R2(double input)
{
  double x = Mix(input) ;
  p2 = p1               ;
  p1 = input            ;
  dN ()                 ;
  return x              ;
}

// A = 1.0 - B - C
void N::Resonator::E1(void)
{
  A  = 1.0 ;
  A -= B   ;
  A -= C   ;
}

// convert to antiresonator coefficients (a'=1/a, b'=b/a, c'=c/a)
void N::Resonator::E2(void)
{
  nDropOut ( A == 0 ) ;
  A  = 1.0 / A        ;
  B *=      -A        ;
  C *=      -A        ;
}
