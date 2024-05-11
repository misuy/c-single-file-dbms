#ifndef PROTOCOL_UTIL_H_
#define PROTOCOL_UTIL_H_

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "protocol_types.h"

void thrift_print_query(ProtocolQuery *query);
void thrift_print_protocol_query_result(ProtocolQueryResult *result);

#endif