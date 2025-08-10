#include "gyrocom.h"

#include <map>

namespace
{
} // namespace

class GyroCom::Impl
{
  public:
    Impl( double _pos1, double _pos2, double _pos3 )
    {
      level.emplace( 0, _pos1 );
      level.emplace( 1, _pos2 );
      level.emplace( 2, _pos3 );
    }
    Impl( const Impl& ) = delete;
    Impl& operator=( const Impl& ) = delete;
    Impl( Impl&& ) = delete;
    Impl& operator=( Impl&& ) = delete;

    double readDouble()
    {
      return level.at( actualPos );
    }
    void setPosition( int pos )
    {
      if( level.contains( pos ) )
      {
        actualPos = pos;
      }
    }

    ~Impl()
    {
    }
    int actualPos = 0;
    std::map< int, double > level;
};

GyroCom::GyroCom( double pos1, double pos2, double pos3 )
  : m_pImpl{ std::make_unique< Impl >( pos1, pos2, pos3 ) }
{
}

GyroCom::~GyroCom() = default;

void GyroCom::setPosition( int pos )
{
  m_pImpl->setPosition( pos );
}

double GyroCom::readDouble()
{
  return m_pImpl->readDouble();
}
