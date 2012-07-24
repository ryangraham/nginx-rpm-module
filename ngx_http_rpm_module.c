/*
 * Copyright (C) 2012 Ryan Graham
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_rpm_filter_init(ngx_conf_t *cf);

static ngx_http_module_t ngx_http_rpm_module_ctx = {
  NULL,
  ngx_http_rpm_filter_init, /* post configuration: add our header filter */
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
};

ngx_module_t ngx_http_rpm_module = {
  NGX_MODULE_V1,
  &ngx_http_rpm_module_ctx,
  NULL,
  NGX_HTTP_MODULE,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NGX_MODULE_V1_PADDING
};

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;

static ngx_int_t ngx_http_rpm_header_filter(ngx_http_request_t *r)
{
  if(r->headers_out.status > 499 && r->headers_out.status < 601) {
    (void) ngx_atomic_fetch_add(ngx_stat_500s, 1);
  }
  return ngx_http_next_header_filter(r);
}

static ngx_int_t ngx_http_rpm_filter_init(ngx_conf_t *cf)
{
  ngx_http_next_header_filter = ngx_http_top_header_filter;
  ngx_http_top_header_filter = ngx_http_rpm_header_filter;

  return NGX_OK;
}
