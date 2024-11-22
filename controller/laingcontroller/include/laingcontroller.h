#pragma once
#include <memory>
#include "ilaingcontroller.h"
#include <laingvalue.h>

class MoveTecModBus;

class LaingController final : public ILaingController
{
  public:
    explicit LaingController( const std::string& device );

    LaingController( const LaingController& ) = delete;

    LaingController& operator=( const LaingController& ) = delete;

    LaingController( LaingController&& ) = delete;

    LaingController& operator=( LaingController&& ) = delete;

    ~LaingController() override;

    std::uint16_t getTableHeight( AXIS axis ) const override;

    void moveToUserPosition( AXIS axis, USER_POSITION pos ) const override;

    void referenceRun( AXIS axis ) const override;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
