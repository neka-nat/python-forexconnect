#pragma once
class Order2Go2 IAddRef
{
 public:
  virtual ~IAddRef();
  virtual long addRef() = 0;
  virtual long release() = 0;
};

typedef double DATE;

