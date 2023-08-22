#ifndef _rtp_over_rtsp_h_
#define _rtp_over_rtsp_h_

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

// muhwan add
typedef struct rtsp_server_t rtsp_server_t;

struct rtp_over_rtsp_t
{
	int state; // 0-all done, other-need more interleaved data
	uint8_t channel;
	uint16_t length;
	uint16_t bytes;
	uint16_t capacity;
	uint8_t* data;

	void (*onrtp)(void* param, uint8_t channel, const void* data, uint16_t bytes);
	// 2. muhwan: add 
	void (*onrtp_svr)(void* param, rtsp_server_t *rtsp, uint8_t channel, const void* data, uint16_t bytes);	
	void* param;
};

// muhwan: rtp_over_rtsp(rtsp_server_t 파라메터 추가)
// const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t *rtp, const uint8_t* data, const uint8_t* end);
const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t* rtp, const uint8_t* data, const uint8_t* end, rtsp_server_t *rtsp);

#if defined(__cplusplus)
}
#endif
#endif /* !_rtp_over_rtsp_h_ */
