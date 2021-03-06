/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "yaksi.h"
#include "yaksu.h"
#include <assert.h>

int yaksi_request_create(yaksi_request_s ** request)
{
    int rc = YAKSA_SUCCESS;
    yaksi_request_s *req;

    req = (yaksi_request_s *) malloc(sizeof(yaksi_request_s));
    YAKSU_ERR_CHKANDJUMP(!req, rc, YAKSA_ERR__OUT_OF_MEM, fn_fail);

    rc = yaksu_handle_pool_elem_alloc(yaksi_global.request_handle_pool, &req->id, req);
    YAKSU_ERR_CHECK(rc, fn_fail);

    yaksu_atomic_store(&req->cc, 0);

    rc = yaksur_request_create_hook(req);
    YAKSU_ERR_CHECK(rc, fn_fail);

    *request = req;

  fn_exit:
    return rc;
  fn_fail:
    goto fn_exit;
}

int yaksi_request_free(yaksi_request_s * request)
{
    int rc = YAKSA_SUCCESS;

    rc = yaksur_request_free_hook(request);
    YAKSU_ERR_CHECK(rc, fn_fail);

    rc = yaksu_handle_pool_elem_free(yaksi_global.request_handle_pool, request->id);
    YAKSU_ERR_CHECK(rc, fn_fail);

    free(request);

  fn_exit:
    return rc;
  fn_fail:
    goto fn_exit;
}

int yaksi_request_get(yaksa_request_t request, struct yaksi_request_s **yaksi_request)
{
    int rc = YAKSA_SUCCESS;
    uint32_t id = (uint32_t) request;

    rc = yaksu_handle_pool_elem_get(yaksi_global.request_handle_pool, id,
                                    (const void **) yaksi_request);
    YAKSU_ERR_CHECK(rc, fn_fail);

  fn_exit:
    return rc;
  fn_fail:
    goto fn_exit;
}
