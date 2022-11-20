#pragma once

namespace fs
{

class Calculator
{
  public:
    virtual float calculate(float theta) const noexcept = 0;
};

}
