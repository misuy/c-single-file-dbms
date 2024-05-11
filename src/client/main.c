#include <thrift/c_glib/protocol/thrift_binary_protocol.h>
#include <thrift/c_glib/transport/thrift_buffered_transport.h>
#include <thrift/c_glib/transport/thrift_socket.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "../protocol/query_executor.h"

#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: lab3-client [port] [queryfile (optional)]\n");
        return 1;
    }


    int port = atoi(argv[1]);

    FILE *in;
    if (argc == 3)
        in = fopen(argv[2], "r");
    else
        in = stdin;

    ThriftSocket *socket;
    ThriftTransport *transport;
    ThriftProtocol *protocol;
    QueryExecutorIf *client;
    GError *error = NULL;
    ProtocolQueryResult *query_result = NULL;

    socket = g_object_new
    (
        THRIFT_TYPE_SOCKET,
        "hostname", "localhost",
        "port", port,
        NULL
    );

    transport = g_object_new
    (
        THRIFT_TYPE_BUFFERED_TRANSPORT,
        "transport", socket,
        NULL
    );

    protocol = g_object_new
    (
        THRIFT_TYPE_BINARY_PROTOCOL,
        "transport", transport,
        NULL
    );

    thrift_transport_open(transport, &error);

    client = g_object_new
    (
        TYPE_QUERY_EXECUTOR_CLIENT,
        "input_protocol", protocol,
        "output_protocol", protocol,
        NULL
    );

    FiniteAutomata *automata = finite_automata_new(tokens_default(), DEFAULT_TOKENS_COUNT, finite_automata_alphabet_default());
    yypstate *parser = yypstate_new();

    char *buffer = 0;
    uint64_t len = 0;
    uint8_t spin = 1;
    while (spin)
    {
        printf("Type your query here:\n");
        fflush(in);
        getline(&buffer, &len, in);
        if (strcmp(buffer, "exit") == 0)
        {
            spin = 0;
            continue;
        }

        printf("Your query: %s\n", buffer);

        Text *text = text_new(buffer);
        Node *node = 0;

        int res = yypull_parse(parser, automata, text, &node);
        if (res)
            node_print(stdout, node, "error", 1);
        else
        {
            ProtocolQuery *query = thrift_parse_query(node);
            query_result = g_object_new(TYPE_PROTOCOL_QUERY_RESULT, NULL);
            //thrift_print_query(query);
            printf("sending query...\n");
            query_executor_if_execute(client, &query_result, query, &error);
            printf("got response\n");
            thrift_print_protocol_query_result(query_result);
            error = NULL;
            query_result = NULL;
        }
        node_free(node);
        printf("\n");
    }


    return 0;
}