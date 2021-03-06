/*
 * m2m projuct
 *
 * FileName: m2m.h
 *
 * Description: m2m sdk and the 3rd party Application need to be included.
 *
 * Author: skylli
 */
#include "../config/config.h"
#include "m2m_type.h"
#ifndef _M2M_H_
#define _M2M_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define M2M_LOG_ALL    (0)
#define M2M_LOG_DEBUG  (1)
#define M2M_LOG        (2)
#define M2M_LOG_WARN   (3)
#define M2M_LOG_ERROR  (4)

#ifndef M2M_VERSION_MAJOR
#include "BuildValue.h"
#endif

#define m2m_printf  printf
typedef enum M2M_BASE_CMD{
    M2M_BCMD_ONLINE_CHECK = 0,
    M2M_BCMD_ONLINE_CHECK_ACK,

    M2M_BCMD_MAX
}M2M_base_CMD;
typedef enum M2M_RETURN_T
{
/************* request cmd  服务器 设备端 支持， 对于 发起会话的 app 端并不需支持******************************/
    M2M_REQUEST_DATA = 100, 
    M2M_REQUEST_NET_SET_SECRETKEY = 101,     
    M2M_REQUEST_SESSION_SET_SECRETKEY = 102, 
    M2M_REQUEST_BROADCAST = 102,
    M2M_REQUEST_BROADCAST_ACK = 103,
    M2M_REQUEST_OBSERVER_RQ = 104,	//在回调中返回时 receive 不为 NULL 则为 M2M_obs_payload_T 会携带 notify index
    M2M_REQUEST_NOTIFY_PUSH = 105,	
    M2M_REQUEST_NOTIFY_ACK = 106, //在回调中返回 receive 不为 NULL 则为 M2M_obs_payload_T 会携带 notify index
    
/***************** http return *************************/
    M2M_HTTP_OK = 200,
    M2M_HTTP_CREATED = 201,
    M2M_HTTP_NO_CONTENT = 204,
    M2M_HTTP_BROADCAST = 205,
    
    M2M_ERR_NOACK       = 32,
    M2M_HTTP_ID_NOMATHC = 33,
    M2M_HTTP_TOKEN_NOMATCH = 34,
    M2M_HTTP_MSGID_NOMATCH = 35,
    M2M_HTTP_SECRET_ERR     = 36,
    M2M_HTTP_INVALID_PROTOCODE_ERR = 37,
    
/*****************client inner error*******************/
    M2M_ERR_NOERR = 0,
    M2M_ERR_NULL = -1,
    M2M_ERR_INVALID = -2,
    
    M2M_ERR_SENDERR = -3,
    M2M_ERR_SOCKETERR = -4,
    M2M_ERR_IGNORE = -5,
    M2M_ERR_RETRANSMIT = -6,
    M2M_ERR_TIMEOUT     =-7,
	M2M_ERR_REQUEST_DESTORY	=-8,
	M2M_ERR_OBSERVER_DISCARD = -9,//回调中返回时， receive 一旦不为空，则 receive 为 M2M_obs_payload_T 格式

    M2M_ERR_PROTO_PKT_BUILD = -13
/*****************************************/
}M2M_Return_T;

typedef enum PKT_ACK_TYPE_T{
	TYPE_ACK_MUST = 0,
	TYPE_ACK_NONE,
	TYPE_ACK_MAX
}Pkt_ack_type_T;

typedef enum ENCRYPT_TYPT_T{
    M2M_ENC_TYPE_NOENC = 0,     // 目前只有 ping 包没有加密.
    M2M_ENC_TYPE_AES128,    // 4*32 bit key.
    M2M_ENC_TYPE_BROADCAST, // 广播包不需要加密.*/
    M2M_ENC_TYPE_MAX
}Encrypt_type_T;

#define ENC_KEY_LEN	(16)
/******
*  init --->M2M_SESSION_STA_NOTOKEN ---> (client)send token request --->server 
*      |                                |                                 |
*      |                             receive token  <----- creat token <--|
*      |   M2M_SESSION_STA_BUILDED <----|                                 |
*      |             |                                                    |
*      |             V                                                    |
*      |<--M2M_SESSION_STA_TOKEN_NO_MATCH <--- bed token --- server <-----|
*****/
typedef enum M2M_SESSION_STA{
    M2M_SESSION_STA_NOTOKEN = 0,
    M2M_SESSION_STA_TOKEN_NO_MATCH,
    M2M_SESSION_STA_HAS_TOKEN
    
}M2M_session_Sta;

typedef struct M2M_ADDRESS_T
{
    u8 len;
    u8 ip[16];
    u16 port;
} M2M_Address_T;

typedef struct ENC_T{
    Encrypt_type_T type;
    u8 keylen;      //
    u16 unused;
    u8 key[0];  //秘钥
}Enc_T;

typedef struct FUNC_ARG{
    m2m_func func;
    void *p_user_arg;
}Func_arg;

typedef void (*userCallbackFunc)
    (
        M2M_Return_T code,
        u16 len,
        u8 *p_data,
        void *p_args
    );
typedef void (*onNetFunc)
    (   M2M_Return_T code,
        u16 len,
        void *p_data,
        void *p_args);

typedef struct M2M_PACKET_T{
    u32 len;
    u8* p_data;
}M2M_packet_T;
typedef struct M2M_PAYLOAD_T{
    u32 len;
    u8 p_data[0];
}M2M_payload_T;
typedef struct M2M_OBS_PAYLOAD_T{
    void *p_obs_node;
    M2M_packet_T *p_payload;
}M2M_obs_payload_T;

// 路由表中每一项的结构

#define ID_LEN  (16)
typedef struct  M2M_ID_T{
    u8 id[ID_LEN];
}M2M_id_T;
typedef struct M2M_ROUTER_LIST_T{
    u32 alive_tm;
    u32 addr_ip;
    M2M_id_T id;

}M2M_Router_list_T;

//AP mode data
typedef struct{
    u16 ssidlen;
    u16 passwordlen;
    u8 p_ssid_pw[0];

}LM2M_router_conf;
//device mac
typedef struct LM2M_LOCAL_T{
	u32 idh;
	u32 idl;
}Lm2m_local_T;
//
typedef struct LM2M_DATA_T{
	u8 ver;  //版本号
	u8 cmd; // 命令
	u8 idx; // 序号, 预留给 sensor 使用
	u16 len; // payload 长度
	u8 data[0];//预留
}Lm2m_data_T;

typedef struct{
    u8 version;     // 验证数,表示协议和协议版本，目前是 0x01
    u8 cmd;         // 命令类型
    u8 idx;         // 序号, 预留
	u8 nouse;
	u16 len;         // 数据长度
    char payload[0];// 数据
} WIFI_PACKET1;
typedef struct LM2M_OTA_data{
  u16 port;
  char ip[16];
  char name[60];
}Lm2m_ota_data;

#ifdef __cplusplus
}
#endif

#endif/*_M2M_H_*/

