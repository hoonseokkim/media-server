submodule update

0. gcc.mk: Relase build 설정 -> 줄바꿈 필요

# RELEASE ?= 0 # default debug
# UNICODE ?= 0 # default ansi
RELEASE ?= 1
UNICODE ?= 0


///////////////////////////////////////////////////////////////////////////////////////
// librtsp

Makefile: 
# muhwan: add aio include, source
$(ROOT)/libaio/include
SOURCE_PATHS = source source/client source/server source/utils source/sdp source/server/aio

include/rtp-over-rtsp.h
// 1. muhwan: add pre-define
typedef struct rtsp_server_t rtsp_server_t;
// 2. muhwan: add onrtp_svr callback pointer
void (*onrtp_svr)(void* param, rtsp_server_t *rtsp, uint8_t channel, const void* data, uint16_t bytes);	
// 3. muhwan: rtp_over_rtsp(rtsp_server_t 파라메터 추가)
// const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t *rtp, const uint8_t* data, const uint8_t* end);
const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t* rtp, const uint8_t* data, const uint8_t* end, rtsp_server_t *rtsp);


source/client/rtsp-client-internal.h 
// muhwan: USER_AGENT 수정 
//#define USER_AGENT "RTSP client v0.1"
#define USER_AGENT "4DLiveClient-1.0.0"


source/client/rtp-over-rtsp.c
// 1. muhwan: rtp_over_rtsp(rtsp_server_t 파라메터 추가)
// const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t *rtp, const uint8_t* data, const uint8_t* end)
const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t* rtp, const uint8_t* data, const uint8_t* end, rtsp_server_t* rtsp)
// 2. muhwan: onrtp_svr() 호출루틴 추가 
else if(rtp->onrtp_svr && rtsp) 
	rtp->onrtp_svr(rtp->param, rtsp, rtp->channel, rtp->data, rtp->length);


include/rtsp-server-aio.h
// muhwan: add rtsp_server_t param
// void (*onrtp)(void* param, uint8_t channel, const void* data, uint16_t bytes);
void (*onrtp)(void* param, rtsp_server_t* rtsp, uint8_t channel, const void* data, uint16_t bytes);


source/aio/rtsp-server-tcp.c 
// 1. muhwan modify 65->20 (기존)
//#define TIMEOUT_RECV 65000
#define TIMEOUT_RECV 20000
#define TIMEOUT_SEND 10000
// 2. muhwan: call-back 함수 변경
// session->rtp.onrtp = handler->onrtp;
session->rtp.onrtp = NULL;
session->rtp.onrtp_svr = handler->onrtp;
// 3. muhwan: 
// p = rtp_over_rtsp(&session->rtp, p, end);
p = rtp_over_rtsp(&session->rtp, p, end, session->rtsp);


	
source/client/rtsp-client.c
// 1. muhwan: 초기화
rtsp->rtp.onrtp_svr = NULL; // muhwan add
// 2. muhwan: RTSP_RECORD 추가
case RTSP_RECORD:	return rtsp_client_record_onreply(rtsp, parser);	
// 2.  muhwan 
//p = rtp_over_rtsp(&rtsp->rtp, p, end);
p = rtp_over_rtsp(&rtsp->rtp, p, end, NULL);





//////////////////////////////////////////////////////////////////////////////////////

Deprecated

1. librtsp

Makefile: 
- add aio include, source	--> rtsp server에 aio 빌드설정 없음

include/rtsp-server-aio.h
// muhwan modify for rtsp push server
//const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t *rtp, const uint8_t* data, const uint8_t* end);
const uint8_t* rtp_over_rtsp(struct rtp_over_rtsp_t* rtp, const uint8_t* data, const uint8_t* end, rtsp_server_t *rtsp);


source/aio/rtsp-server-tcp.c  
- copy from client/rtp-over-rtsp.c	--> rtsp 세션별 RTP/TCP callback 기능 없음 --> 불필요해짐 push server, client 기능 처리 구분 하도록 수정
  rtp_over_tcp_state_t, rtp_over_rtsp_t, rtp_alloc(), rtp_over_rtsp() 	
- rtsp session 파라메터 전달.  

rtsp-media.c
- moddify &media->ice.remotes[media->ice.remote_count]->address  --> 포인터 변수 에러 --> 업데이트 후 불필요

rtsp-client-announce.c 
- force aggregation mode --> aggregation 모드 설정방법 없으려나??? --> 불필요해짐 삭제해도 잘됨.

include/rtsp-client.h
- add rtsp_client_options()		--> API 누락으로 추가 --> 업데이트 후 불필요
- add rtsp_client_setup_mode()	--> record mode 추가하기 위한 함수 없음  --> 보류

source/client/rtsp-client-setup.c
- add rtsp_client_setup_mode() --> record 모드 함수 추가  --> 수정보류

source/client/rtsp-client.c
- add RECORD method 추가 --> rtsp client RECORD 처리기능 없음

source/client/rtsp-client-play.c
- "RTP-Info" header 없는 경우 에외처리 추가  --> rtsp client 용 


- RECV Timeout 변경 
aio/rtsp-server-tcp.c
#define TIMEOUT_RECV 20000

source/client/rtsp-client-internal.h 
- USER_AGENT 수정 : #define USER_AGENT "4DLive-1.0.0"  