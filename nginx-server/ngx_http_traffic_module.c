#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "traffic_resolv.h"

typedef struct
{
	ngx_int_t traffic_control;
}ngx_http_traffic_loc_conf_t;

//static ngx_int_t ngx_http_dnsmasq_init(ngx_conf_t *cf);
static void *ngx_http_traffic_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_traffic_control(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_traffic_handler(ngx_http_request_t *r);
static void ngx_http_request_body_process(ngx_http_request_t *r);

static u_char* outstr = NULL;
static u_char* outdata = NULL;

static ngx_command_t ngx_http_traffic_commands[] = {
	{
	ngx_string("traffic_control"),
	NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
	ngx_http_traffic_control,
	NGX_HTTP_LOC_CONF_OFFSET,
	offsetof(ngx_http_traffic_loc_conf_t, traffic_control),
	NULL 
	},

	ngx_null_command
};
/*
 * static u_char ngx_test_default_string[] = "Default String: Hello, world!";
 * */
 //structure for the OPENVPN context
static ngx_http_module_t ngx_http_traffic_module_ctx = {
	NULL, /* preconfiguration */
	NULL, //ngx_http_dnsmasq_init, /* postconfiguration */
	NULL, /* create main configuration */
	NULL, /* init main configuration */
	NULL, /* create server configuration */
	NULL, /* merge server configuration */
	ngx_http_traffic_create_loc_conf, /* create location configuration */
	NULL /* merge location configuration */
};
ngx_module_t ngx_http_traffic_module = {
	NGX_MODULE_V1,
	&ngx_http_traffic_module_ctx, /* module context */
	ngx_http_traffic_commands, /* module directives */
	NGX_HTTP_MODULE, /* module type */
	NULL, /* init master */
	NULL, /* init module */
	NULL, /* init process */
	NULL, /* init thread */
	NULL, /* exit thread */
	NULL, /* exit process */
	NULL, /* exit master */
	NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_traffic_handler(ngx_http_request_t *r)
{
	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out;
	
	ngx_http_traffic_loc_conf_t* my_conf;
	ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "ngx_http_traffic_handler is called!");
	my_conf = ngx_http_get_module_loc_conf(r, ngx_http_traffic_module);

	if (my_conf->traffic_control == NGX_CONF_UNSET
	|| my_conf->traffic_control == 0)
	{
		ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "traffic_control:traffic control has not enable");
		return NGX_HTTP_NOT_ALLOWED;
	}


	/* we response to 'POST' requests only */
	if (!(r->method & NGX_HTTP_POST)) 
	{
		return NGX_HTTP_NOT_ALLOWED;
	}


	

	rc = ngx_http_read_client_request_body(r,ngx_http_request_body_process);

	if(rc != NGX_OK){
		return rc;
	}
	if(!outstr)
		outstr = (u_char*)"sucess\n";

	ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "traffic_control1:%s", outstr);
	ngx_str_set(&r->headers_out.content_type, "text/html");
	
	/* allocate a buffer for your response body */
	b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if (b == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	/* attach this buffer to the buffer chain */
	out.buf = b;
	out.next = NULL;
	/* adjust the pointers of the buffer */
	b->pos = outdata;
	b->last = outdata + ngx_strlen(outdata);
	b->memory = 1; /* this buffer is in memory */
	b->last_buf = 1; /* this is the last buffer in the buffer chain */
	
	ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "traffic_control2:%s, %d", b->pos, ngx_strlen(outdata));
	/* set the status line */
	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = ngx_strlen(outdata);
	/* send the headers of your response */
	rc = ngx_http_send_header(r);
	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
		return rc;
	}
	/* send the buffer chain of your response */
	return ngx_http_output_filter(r, &out);
}

static void *ngx_http_traffic_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_traffic_loc_conf_t* local_conf = NULL;


	local_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_traffic_loc_conf_t));
	if (local_conf == NULL)
	{
		return NULL;
	}
	local_conf->traffic_control = NGX_CONF_UNSET;
	return local_conf;
}
/*
 * static char *ngx_http_test_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
 * {
 * ngx_http_test_loc_conf_t* prev = parent;
 * ngx_http_test_loc_conf_t* conf = child;
 * ngx_conf_merge_str_value(conf->hello_string, prev->hello_string, ngx_test_default_string);
 * ngx_conf_merge_value(conf->hello_counter, prev->hello_counter, 0);
 * return NGX_CONF_OK;
 * }*/

static char *
ngx_http_traffic_control(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	ngx_http_traffic_loc_conf_t* local_conf;
	char* rv = ngx_conf_set_flag_slot(cf, cmd, conf);
	
	ngx_http_core_loc_conf_t *clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_traffic_handler;	
	
	local_conf = conf;
	ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "traffic_control3:%d", local_conf->traffic_control);
	return rv;
}



static void ngx_http_request_body_process(ngx_http_request_t *r)
{
	ngx_chain_t *chain;
	ngx_buf_t *buf;
	u_char *str;

	chain = r->request_body->bufs;
	buf = chain->buf;
	str = buf->pos;

	if( buf == NULL){
		ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "traffic_control:body process buf null");
	}
	else{
		ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "openvpn_control4:%s", buf->pos);
		outstr = (u_char*)json_analysis((char*)str, (char**)&outdata);
		if(!outstr)
		{
			ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "traffic_control:return null");
		}
	}

}
