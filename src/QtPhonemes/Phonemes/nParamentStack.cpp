#include <qtphonemes.h>

N::ParamentStack:: ParamentStack (void)
                 : type          (0   )
{
  memset(parameter,0,sizeof(int)*16) ;
}

N::ParamentStack:: ParamentStack(const ParamentStack & stack)
{
  assign ( stack ) ;
}

N::ParamentStack::~ParamentStack(void)
{
}

N::ParamentStack & N::ParamentStack::operator = (const ParamentStack & stack)
{
  return assign(stack) ;
}

N::ParamentStack & N::ParamentStack::assign(const ParamentStack & stack)
{
  nMemberCopy ( stack , type )                       ;
  ::memcpy(parameter,stack.parameter,sizeof(int)*16) ;
  return ME                                          ;
}
