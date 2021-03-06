#include <stdlib.h>
#include <ctime>

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
void RandomInit( unsigned int seed )
{
    srand( seed );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void RandomInit()
{
    // 165983 - самое большое простое число, которое я нашёл в интернете
    const time_t seconds = time( NULL );
    const uint32_t seed = static_cast< uint32_t >( seconds % 165983 );
    srand( seed );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetRandomBool()
{
    return ( ( rand() % 2 ) == 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int GetRandom( int range )
{
    return ( rand() % range );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
float GetRandomFloat()
{
    return ( static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
float GetDispersion()
{
    const float coef = GetRandomFloat();
    return ( coef - 0.5f ) * 2.0f;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
