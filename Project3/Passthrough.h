#pragma once
#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include "Project3.h"

namespace Project3
{
	class PassthroughEncoder : public ICompressionAlg
	{
	public:
		PassthroughEncoder();
		virtual ~PassthroughEncoder();

		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData);
	};

	class PassthroughDecoder : public ICompressionAlg
	{
	public:
		PassthroughDecoder();
		virtual ~PassthroughDecoder();

		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData);
	};
}

#endif
