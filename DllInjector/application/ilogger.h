#pragma once

#include <QString>

namespace Injector
{

class ILogger
{
public:
	virtual void operator()(QString const& logInfo) const = 0;
};

}
