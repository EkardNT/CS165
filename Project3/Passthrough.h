#pragma once
#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include "Project3.h"

namespace Project3
{
	class PassthroughEncoder : public ICompressionAlg
	{
	public:
		PassthroughEncoder(std::shared_ptr<std::ostream> outputStream);
		virtual ~PassthroughEncoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();
	};

	class PassthroughDecoder : public ICompressionAlg
	{
	public:
		PassthroughDecoder(std::shared_ptr<std::ostream> outputStream);
		virtual ~PassthroughDecoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();
	};
}

#endif