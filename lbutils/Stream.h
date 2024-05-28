/*
 * Stream.h
 *
 *  Created on: 5 maj 2021
 *      Author: lechu
 */

#ifndef STREAM_H_
#define STREAM_H_

#include "stdint.h"
#include "myUtils.h"

class Stream {
public:
	static constexpr uint32_t BUFFER_EMPTY_FLAG_U32 = 0xffffff00;	///< flaga pustego bufora dla wartosci 16-bitowej
	static constexpr uint16_t BUFFER_EMPTY_FLAG_U16 = 0xff00;		///< flaga pustego bufora dla wartosci 8-bitowej
	static constexpr uint32_t BUFFER_UNDERRUN_FLAG_U32 = 0xffffffff;
	static constexpr uint16_t BUFFER_UNDERRUN_FLAG_U16 = 0xffff;
	static constexpr uint8_t BUFFER_UNDERRUN_FLAG_U8 = 0xff;
	virtual ~Stream();
};

#endif /* STREAM_H_ */
