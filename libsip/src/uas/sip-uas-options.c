#include "sip-uas-transaction.h"

/*
OPTIONS sip:carol@chicago.com SIP/2.0
Via: SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bKhjhs8ass877
Max-Forwards: 70
To: <sip:carol@chicago.com>
From: Alice <sip:alice@atlanta.com>;tag=1928301774
Call-ID: a84b4c76e66710
CSeq: 63104 OPTIONS
Contact: <sip:alice@pc33.atlanta.com>
Accept: application/sdp
Content-Length: 0

SIP/2.0 200 OK
Via: SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bKhjhs8ass877;received=192.0.2.4
To: <sip:carol@chicago.com>;tag=93810874
From: Alice <sip:alice@atlanta.com>;tag=1928301774
Call-ID: a84b4c76e66710
CSeq: 63104 OPTIONS
Contact: <sip:carol@chicago.com>
Contact: <mailto:carol@chicago.com>
Allow: INVITE, ACK, CANCEL, OPTIONS, BYE
Accept: application/sdp
Accept-Encoding: gzip
Accept-Language: en
Supported: foo
Content-Type: application/sdp
Content-Length: 274
*/
int sip_uas_onoptions(struct sip_uas_transaction_t* t, const struct sip_message_t* req, void* p)
{
	static const char* headers[] = { "Accept", "Accept-Encoding", "Accept-Language" };

	int i;
	struct sip_param_t param;
	const struct cstring_t* header;
	
	for (i = 0; i < sizeof(headers) / sizeof(headers[0]); i++)
	{
		header = sip_message_get_header_by_name(req, headers[i]);
		if (!header) continue;
		param.name.p = headers[i];
		param.name.n = strlen(headers[i]);
		param.value.p = header->p;
		param.value.n = header->n;
		sip_params_push(&t->reply->headers, &param);
	}

	// Alternatively, a server receiving an OPTIONS request with a Max-Forwards header 
	// field value of 0 MAY respond to the request regardless of the Request - URI.
	if (0 == req->maxforwards)
	{
	}

	// If the response is generated by a proxy, the Allow header field 
	// SHOULD be omitted as it is ambiguous since a proxy is method agnostic.
	sip_uas_add_header(t, "Allow", "OPTIONS, INVITE, ACK, BYE, CANCEL, MESSAGE, INFO, SUBSCRIBE, NOTIFY, PUBLISH, PRACK, REFER, UPDATE");
	//sip_uas_add_header("Supported", "foo");
	//sip_uas_add_header("Warning", "");

	// 200 - OK
	// 486 - Busy Here
	return sip_uas_transaction_noninvite_reply(t, 200, NULL, 0, p);
}
