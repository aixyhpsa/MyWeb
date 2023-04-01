#ifndef http_parser_h
#define http_parser_h
  //#ifdef __cplusplus
  //extern "C" {
  //#endif

/* Also update SONAME in the Makefile whenever you change these. */
#define HTTP_PARSER_VERSION_MAJOR 2
#define HTTP_PARSER_VERSION_MINOR 7
#define HTTP_PARSER_VERSION_PATCH 1

#include <sys/types.h>
#if defined(_WIN32) && !defined(__MINGW32__) && \
  (!defined(_MSC_VER) || _MSC_VER<1600) && !defined(__WINE__)
#include <BaseTsd.h>
#include <stddef.h>
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

// 使用-DHTTP_PARSER_STRICT=0来减少检查，但运行得更快
#ifndef HTTP_PARSER_STRICT
# define HTTP_PARSER_STRICT 1
#endif

// 允许最大标题大小。如果在包含此头文件之前未定义宏，则使用默认值。
// 要更改最大报头大小，请在构建环境中定义宏(e.g. -DHTTP_MAX_HEADER_SIZE=<value>).
// 为了消除对头大小的有效限制，将宏定义为一个非常大的数字(e.g. -DHTTP_MAX_HEADER_SIZE=0x7fffffff)
#ifndef HTTP_MAX_HEADER_SIZE
# define HTTP_MAX_HEADER_SIZE (80*1024)
#endif

typedef struct http_parser http_parser;
typedef struct http_parser_settings http_parser_settings;

// 回调应该返回非零以指示错误。解析器将停止执行。
// 唯一的例外是on_headers_complete。
// 在HTTP_RESPONSE解析器中，从on_headers_complete返回'1'将告诉解析器它不应该期望正文。
// 当接收到对HEAD请求的响应时使用，该HEAD请求可能包含“Content-Length”或“Transfer-Encoding: chunked”报头，表示body的存在。
//
// 从on_headers_complete返回' 2 '将告诉解析器，它不应该在这个连接上期望body或任何进一步的响应。
// 这对于处理不包含' Upgrade '或' Connection: Upgrade '头的CONNECT请求的响应非常有用。
//
// Http_data_cb不返回数据块。
// 它将为每个字符串调用任意多次。例如，你可能会得到10个on_url的回调，每个回调只提供几个字符的数据。
#include <functional>
using http_data_cb = std::function<int(http_parser*, const char*, size_t)>;
using http_cb = std::function<int(http_parser*)>;

  //typedef int (*http_data_cb) (http_parser*, const char *at, size_t length);
  //typedef int (*http_cb) (http_parser*);


/* Status Codes */
#define HTTP_STATUS_MAP(XX)                                                 \
  XX(100, CONTINUE,                        Continue)                        \
  XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)             \
  XX(102, PROCESSING,                      Processing)                      \
  XX(200, OK,                              OK)                              \
  XX(201, CREATED,                         Created)                         \
  XX(202, ACCEPTED,                        Accepted)                        \
  XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)   \
  XX(204, NO_CONTENT,                      No Content)                      \
  XX(205, RESET_CONTENT,                   Reset Content)                   \
  XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
  XX(207, MULTI_STATUS,                    Multi-Status)                    \
  XX(208, ALREADY_REPORTED,                Already Reported)                \
  XX(226, IM_USED,                         IM Used)                         \
  XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
  XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
  XX(302, FOUND,                           Found)                           \
  XX(303, SEE_OTHER,                       See Other)                       \
  XX(304, NOT_MODIFIED,                    Not Modified)                    \
  XX(305, USE_PROXY,                       Use Proxy)                       \
  XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
  XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
  XX(400, BAD_REQUEST,                     Bad Request)                     \
  XX(401, UNAUTHORIZED,                    Unauthorized)                    \
  XX(402, PAYMENT_REQUIRED,                Payment Required)                \
  XX(403, FORBIDDEN,                       Forbidden)                       \
  XX(404, NOT_FOUND,                       Not Found)                       \
  XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
  XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
  XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)   \
  XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
  XX(409, CONFLICT,                        Conflict)                        \
  XX(410, GONE,                            Gone)                            \
  XX(411, LENGTH_REQUIRED,                 Length Required)                 \
  XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
  XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
  XX(414, URI_TOO_LONG,                    URI Too Long)                    \
  XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
  XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
  XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
  XX(421, MISDIRECTED_REQUEST,             Misdirected Request)             \
  XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)            \
  XX(423, LOCKED,                          Locked)                          \
  XX(424, FAILED_DEPENDENCY,               Failed Dependency)               \
  XX(426, UPGRADE_REQUIRED,                Upgrade Required)                \
  XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
  XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
  XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)   \
  XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
  XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
  XX(502, BAD_GATEWAY,                     Bad Gateway)                     \
  XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
  XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                 \
  XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
  XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)         \
  XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)            \
  XX(508, LOOP_DETECTED,                   Loop Detected)                   \
  XX(510, NOT_EXTENDED,                    Not Extended)                    \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required) \

enum http_status
  {
#define XX(num, name, string) HTTP_STATUS_##name = num,
  HTTP_STATUS_MAP(XX)
#undef XX
  };


/* Request Methods */
#define HTTP_METHOD_MAP(XX)         \
  XX(0,  DELETE,      DELETE)       \
  XX(1,  GET,         GET)          \
  XX(2,  HEAD,        HEAD)         \
  XX(3,  POST,        POST)         \
  XX(4,  PUT,         PUT)          \
  /* pathological */                \
  XX(5,  CONNECT,     CONNECT)      \
  XX(6,  OPTIONS,     OPTIONS)      \
  XX(7,  TRACE,       TRACE)        \
  /* WebDAV */                      \
  XX(8,  COPY,        COPY)         \
  XX(9,  LOCK,        LOCK)         \
  XX(10, MKCOL,       MKCOL)        \
  XX(11, MOVE,        MOVE)         \
  XX(12, PROPFIND,    PROPFIND)     \
  XX(13, PROPPATCH,   PROPPATCH)    \
  XX(14, SEARCH,      SEARCH)       \
  XX(15, UNLOCK,      UNLOCK)       \
  XX(16, BIND,        BIND)         \
  XX(17, REBIND,      REBIND)       \
  XX(18, UNBIND,      UNBIND)       \
  XX(19, ACL,         ACL)          \
  /* subversion */                  \
  XX(20, REPORT,      REPORT)       \
  XX(21, MKACTIVITY,  MKACTIVITY)   \
  XX(22, CHECKOUT,    CHECKOUT)     \
  XX(23, MERGE,       MERGE)        \
  /* upnp */                        \
  XX(24, MSEARCH,     M-SEARCH)     \
  XX(25, NOTIFY,      NOTIFY)       \
  XX(26, SUBSCRIBE,   SUBSCRIBE)    \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                    \
  XX(28, PATCH,       PATCH)        \
  XX(29, PURGE,       PURGE)        \
  /* CalDAV */                      \
  XX(30, MKCALENDAR,  MKCALENDAR)   \
  /* RFC-2068, section 19.6.1.2 */  \
  XX(31, LINK,        LINK)         \
  XX(32, UNLINK,      UNLINK)       \

enum http_method
  {
#define XX(num, name, string) HTTP_##name = num,
  HTTP_METHOD_MAP(XX)
#undef XX
  };

enum http_parser_type { HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH };

// 标记http_parser的值。标志字段
enum flags
  { F_CHUNKED               = 1 << 0
  , F_CONNECTION_KEEP_ALIVE = 1 << 1
  , F_CONNECTION_CLOSE      = 1 << 2
  , F_CONNECTION_UPGRADE    = 1 << 3
  , F_TRAILING              = 1 << 4
  , F_UPGRADE               = 1 << 5
  , F_SKIPBODY              = 1 << 6
  , F_CONTENTLENGTH         = 1 << 7
  };

// 映射与errno相关的常量
// 所提供的参数应该是另一个宏并且是一个带有两个参数的宏。
// 这个宏定义应该是http_errno_description内部使用的，不是给用户用的
#define HTTP_ERRNO_MAP(XX)                                           \
  /* No error */                                                     \
  XX(OK, "成功")                                                  \
                                                                     \
  /* Callback-related errors */                                      \
  XX(CB_message_begin, "on_message_begin回调失败")       \
  XX(CB_url, "on_url回调失败")                           \
  XX(CB_header_field, "on_header_field回调失败")         \
  XX(CB_header_value, "on_header_value回调失败")         \
  XX(CB_headers_complete, "on_headers_complete回调失败") \
  XX(CB_body, "on_body回调失败")                         \
  XX(CB_message_complete, "on_message_complete回调失败") \
  XX(CB_status, "on_status回调失败")                     \
  XX(CB_chunk_header, "on_chunk_header回调失败")         \
  XX(CB_chunk_complete, "on_chunk_complete回调失败")     \
                                                                     \
  /* Parsing-related errors */                                       \
  XX(INVALID_EOF_STATE, "流在一个意想不到的时间结束")        \
  XX(HEADER_OVERFLOW,                                                \
     "看到太多头字节;溢出检测")                \
  XX(CLOSED_CONNECTION,                                              \
     "完成连接后收到的数据:关闭消息")      \
  XX(INVALID_VERSION, "无效的HTTP版本")                        \
  XX(INVALID_STATUS, "无效的HTTP状态码")                     \
  XX(INVALID_METHOD, "无效的HTTP方法")                          \
  XX(INVALID_URL, "无效的URL")                                     \
  XX(INVALID_HOST, "无效的host")                                   \
  XX(INVALID_PORT, "无效的port")                                   \
  XX(INVALID_PATH, "无效的path")                                   \
  XX(INVALID_QUERY_STRING, "无效的查询字符")                   \
  XX(INVALID_FRAGMENT, "无效的片段(fragment)")                           \
  XX(LF_EXPECTED, "期望LF字符(LF character expected)")                           \
  XX(INVALID_HEADER_TOKEN, "头文件中的字符无效(invalid character in header)")            \
  XX(INVALID_CONTENT_LENGTH,                                         \
     "内容长度报头中的无效字符(invalid character in content-length header)")                   \
  XX(UNEXPECTED_CONTENT_LENGTH,                                      \
     "意外的内容长度头(unexpected content-length header)")                             \
  XX(INVALID_CHUNK_SIZE,                                             \
     "块大小报头中的无效字符(invalid character in chunk size header)")                       \
  XX(INVALID_CONSTANT, "无效的常量字符串(invalid constant string)")                    \
  XX(INVALID_INTERNAL_STATE, "遇到意外的内部状态(encountered unexpected internal state)")\
  XX(STRICT, "严格模式断言失败(strict mode assertion failed)")                         \
  XX(PAUSED, "解析器暂停")                                     \
  XX(UNKNOWN, "发生未知错误")


/* Define HPE_* values for each errno value above */
#define HTTP_ERRNO_GEN(n, s) HPE_##n,
enum http_errno {
  HTTP_ERRNO_MAP(HTTP_ERRNO_GEN)
};
#undef HTTP_ERRNO_GEN


/* Get an http_errno value from an http_parser */
// 从http_parser中获取一个http_errno值
#define HTTP_PARSER_ERRNO(p)            ((enum http_errno) (p)->http_errno)


struct http_parser {
  /** PRIVATE **/
  unsigned int type : 2;         /* enum http_parser_type */
  unsigned int flags : 8;        /* F_* values from 'flags' enum; semi-public */
  unsigned int state : 7;        /* enum state from http_parser.c */
  unsigned int header_state : 7; /* enum header_state from http_parser.c */
  unsigned int index : 7;        /* index into current matcher */
  unsigned int lenient_http_headers : 1;

  uint32_t nread;          /* # bytes read in various scenarios */
  uint64_t content_length; /* # bytes in body (0 if no Content-Length header) */

  /** READ-ONLY **/
  unsigned short http_major;
  unsigned short http_minor;
  unsigned int status_code : 16; /* responses only */
  unsigned int method : 8;       /* requests only */
  unsigned int http_errno : 7;

  /* 1 = Upgrade header was present and the parser has exited because of that.
   * 0 = No upgrade header present.
   * Should be checked when http_parser_execute() returns in addition to
   * error checking.
   */
  // 1 = 出现了升级报头，解析器因此退出。
  // 0 = 不存在升级标头。
  unsigned int upgrade : 1;

  /** PUBLIC **/
  // 用于获取“connection”或“socket”对象的钩子的指针
  // 我估计和event.data.ptr的作用差不多
  void *data; /* A pointer to get hook to the "connection" or "socket" object */
};


struct http_parser_settings {
  http_cb      on_message_begin;
  http_data_cb on_url;
  http_data_cb on_status;
  http_data_cb on_header_field;
  http_data_cb on_header_value;
  http_cb      on_headers_complete;
  http_data_cb on_body;
  http_cb      on_message_complete;
  /* When on_chunk_header is called, the current chunk length is stored
   * in parser->content_length.
   */
  http_cb      on_chunk_header;
  http_cb      on_chunk_complete;
};


enum http_parser_url_fields
  { UF_SCHEMA           = 0
  , UF_HOST             = 1
  , UF_PORT             = 2
  , UF_PATH             = 3
  , UF_QUERY            = 4
  , UF_FRAGMENT         = 5
  , UF_USERINFO         = 6
  , UF_MAX              = 7
  };


/* Result structure for http_parser_parse_url().
 *
 * Callers should index into field_data[] with UF_* values iff field_set
 * has the relevant (1 << UF_*) bit set. As a courtesy to clients (and
 * because we probably have padding left over), we convert any port to
 * a uint16_t.
 */
struct http_parser_url {
  uint16_t field_set;           /* Bitmask of (1 << UF_*) values */
  uint16_t port;                /* Converted UF_PORT string */

  struct {
    uint16_t off;               /* Offset into buffer in which field starts */
    uint16_t len;               /* Length of run in buffer */
  } field_data[UF_MAX];
};


/* Returns the library version. Bits 16-23 contain the major version number,
 * bits 8-15 the minor version number and bits 0-7 the patch level.
 * Usage example:
 *
 *   unsigned long version = http_parser_version();
 *   unsigned major = (version >> 16) & 255;
 *   unsigned minor = (version >> 8) & 255;
 *   unsigned patch = version & 255;
 *   printf("http_parser v%u.%u.%u\n", major, minor, patch);
 */
// 返回库版本。第16-23位包含主要版本号，
// 第8-15位包含次要版本号，
// 第0-7位包含补丁级别。使用的例子:
unsigned long http_parser_version(void);

void http_parser_init(http_parser *parser, enum http_parser_type type);


/* Initialize http_parser_settings members to 0*/
// 将http_parser_settings成员初始化为0
void http_parser_settings_init(http_parser_settings *settings);


/* Executes the parser. Returns number of parsed bytes. Sets
 * `parser->http_errno` on error. */
// 执行解析器。返回已经解析的字节数。设置`parser->http_errno`为错误
size_t http_parser_execute(http_parser *parser,
                           const http_parser_settings *settings,
                           const char *data,
                           size_t len);


/* If http_should_keep_alive() in the on_headers_complete or
 * on_message_complete callback returns 0, then this should be
 * the last message on the connection.
 * If you are the server, respond with the "Connection: close" header.
 * If you are the client, close the connection.
 */
// 如果http_should_keep_alive()在on_headers_complete或on_message_complete回调函数中返回0，
// 那么这应该是连接上的最后一条消息。
// 如果您是服务器，则使用“Connection: close”报头进行响应。
// 如果您是客户端，请关闭连接。
int http_should_keep_alive(const http_parser *parser);

/* Returns a string version of the HTTP method. */
// 返回HTTP方法的字符串版本。
const char *http_method_str(enum http_method m);

/* Return a string name of the given error */
// 返回给定错误的字符串名称
const char *http_errno_name(enum http_errno err);

/* Return a string description of the given error */
// 返回给定错误的字符串描述
const char *http_errno_description(enum http_errno err);

/* Initialize all http_parser_url members to 0 */
void http_parser_url_init(struct http_parser_url *u);

/* Parse a URL; return nonzero on failure */
// 解析URL;失败时返回非零
int http_parser_parse_url(const char *buf, size_t buflen,
                          int is_connect,
                          struct http_parser_url *u);

/* Pause or un-pause the parser; a nonzero value pauses */
// 暂停或取消暂停解析器;非零值暂停
void http_parser_pause(http_parser *parser, int paused);

/* Checks if this is the final chunk of the body. */
// 检查这是否是身体的最后一块。
int http_body_is_final(const http_parser *parser);

  //#ifdef __cplusplus
  //}
  //#endif
#endif
