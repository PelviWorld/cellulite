#pragma once
#include <string>

class MoveTecModBus;

class ILaingController
{
  public:
    ILaingController() = default;

    explicit ILaingController( const std::string& device )
    {
    }

    virtual ~ILaingController() = default;

    ILaingController( const ILaingController& ) = delete;

    ILaingController& operator=( const ILaingController& ) = delete;

    ILaingController( ILaingController&& ) = delete;

    ILaingController& operator=( ILaingController&& ) = delete;
};
