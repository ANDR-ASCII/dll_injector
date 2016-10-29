#pragma once

#include <cassert>
#include "ilogger.h"

namespace Injector
{

template <typename TObj>
class InjectorLogger : public ILogger
{
public:
	typedef void(TObj::*BindMethodType)(QString const&) const;

	InjectorLogger(TObj const* object, BindMethodType method)
		: m_obj(object)
		, m_method(method)
	{
		assert(m_obj);
		assert(m_method);
	}

	virtual void operator()(QString const& logInfo) const override
	{
		(m_obj->*m_method)(logInfo);
	}

private:
	TObj const* m_obj;
	BindMethodType m_method;
};

}
