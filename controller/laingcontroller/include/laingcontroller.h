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

    std::uint16_t getTableHeight() const;

    void moveToUserPosition( USER_POSITION pos ) const;

    void referenceRun() const;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
